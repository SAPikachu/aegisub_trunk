// Copyright (c) 2005, Rodrigo Braz Monteiro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Aegisub Group nor the names of its contributors
//     may be used to endorse or promote products derived from this software
//     without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Aegisub Project http://www.aegisub.org/
//
// $Id$

/// @file ass_file.cpp
/// @brief Overall storage of subtitle files, undo management and more
/// @ingroup subs_storage

#include "config.h"

#ifndef AGI_PRE
#include <algorithm>
#include <fstream>
#include <inttypes.h>
#include <list>

#include <wx/filename.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#endif

#include "ass_attachment.h"
#include "ass_dialogue.h"
#include "ass_file.h"
#include "ass_override.h"
#include "ass_style.h"
#include "compat.h"
#include "main.h"
#include "standard_paths.h"
#include "subtitle_format.h"
#include "text_file_reader.h"
#include "text_file_writer.h"
#include "utils.h"
#include "version.h"

namespace std {
	template<>
	void swap(AssFile &lft, AssFile &rgt) {
		lft.swap(rgt);
	}
}

AssFile::AssFile ()
: commitId(0)
, loaded(false)
{
}

AssFile::~AssFile() {
	background_delete_clear(Line);
}

/// @brief Load generic subs
void AssFile::Load(const wxString &_filename, wxString charset, bool addToRecent) {
	try {
		// Get proper format reader
		const SubtitleFormat *reader = SubtitleFormat::GetReader(_filename);
		if (!reader) {
			wxMessageBox("Unknown file type","Error loading file",wxOK | wxICON_ERROR | wxCENTER);
			return;
		}

		AssFile temp;
		reader->ReadFile(&temp, _filename, charset);

		bool found_style = false;
		bool found_dialogue = false;

		// Check if the file has at least one style and at least one dialogue line
		for (entryIter it = temp.Line.begin(); it != temp.Line.end(); ++it) {
			ASS_EntryType type = (*it)->GetType();
			if (type == ENTRY_STYLE) found_style = true;
			if (type == ENTRY_DIALOGUE) found_dialogue = true;
			if (found_style && found_dialogue) break;
		}

		// And if it doesn't add defaults for each
		if (!found_style)
			temp.InsertStyle(new AssStyle);
		if (!found_dialogue)
			temp.InsertDialogue(new AssDialogue);

		swap(temp);
	}
	catch (agi::UserCancelException const&) {
		return;
	}
	// Real exception
	catch (agi::Exception &e) {
		wxMessageBox(lagi_wxString(e.GetChainedMessage()), "Error loading file", wxOK | wxICON_ERROR | wxCENTER);
		return;
	}
	// Other error
	catch (...) {
		wxMessageBox("Unknown error","Error loading file",wxOK | wxICON_ERROR | wxCENTER);
		return;
	}

	// Set general data
	loaded = true;
	filename = _filename;
	StandardPaths::SetPathValue("?script", wxFileName(filename).GetPath());

	// Save backup of file
	if (CanSave() && OPT_GET("App/Auto/Backup")->GetBool()) {
		wxFileName file(filename);
		if (file.FileExists()) {
			wxString path = lagi_wxString(OPT_GET("Path/Auto/Backup")->GetString());
			if (path.empty()) path = file.GetPath();
			wxFileName dstpath(StandardPaths::DecodePath(path + "/"));
			if (!dstpath.DirExists())
				wxMkdir(dstpath.GetPath());

			dstpath.SetFullName(file.GetName() + ".ORIGINAL." + file.GetExt());

			wxCopyFile(file.GetFullPath(), dstpath.GetFullPath(), true);
		}
	}

	// Add comments and set vars
	AddComment(wxString("Script generated by Aegisub ") + GetAegisubLongVersionString());
	AddComment("http://www.aegisub.org/");
	SetScriptInfo("ScriptType","v4.00+");

	// Push the initial state of the file onto the undo stack
	UndoStack.clear();
	RedoStack.clear();
	undoDescription.clear();
	autosavedCommitId = savedCommitId = commitId + 1;
	Commit("", COMMIT_NEW);

	// Add to recent
	if (addToRecent) AddToRecent(filename);
	FileOpen(filename);
}

void AssFile::Save(wxString filename, bool setfilename, bool addToRecent, wxString encoding) {
	const SubtitleFormat *writer = SubtitleFormat::GetWriter(filename);
	if (!writer)
		throw "Unknown file type.";

	if (setfilename) {
		autosavedCommitId = savedCommitId = commitId;
		this->filename = filename;
		StandardPaths::SetPathValue("?script", wxFileName(filename).GetPath());
	}

	FileSave();

	writer->WriteFile(this, filename, encoding);

	if (addToRecent) {
		AddToRecent(filename);
	}
}

wxString AssFile::AutoSave() {
	if (!loaded || commitId == autosavedCommitId)
		return "";

	wxFileName origfile(filename);
	wxString path = lagi_wxString(OPT_GET("Path/Auto/Save")->GetString());
	if (!path)
		path = origfile.GetPath();
	path = StandardPaths::DecodePath(path + "/");

	wxFileName dstpath(path);
	if (!dstpath.DirExists())
		wxMkdir(path);

	wxString name = origfile.GetName();
	if (name.empty())
		dstpath.SetFullName("Untitled.AUTOSAVE.ass");
	else
		dstpath.SetFullName(name + ".AUTOSAVE.ass");

	Save(dstpath.GetFullPath(), false, false);

	autosavedCommitId = commitId;

	return dstpath.GetFullPath();
}

void AssFile::SaveMemory(std::vector<char> &dst) {
	// Check if subs contain at least one style
	// Add a default style if they don't for compatibility with libass/asa
	if (GetStyles().Count() == 0)
		InsertStyle(new AssStyle);

	// Prepare vector
	dst.clear();
	dst.reserve(0x4000);

	// Write file
	for (entryIter cur = Line.begin(); cur != Line.end(); ++cur) {
		wxCharBuffer buffer = ((*cur)->GetEntryData() + "\r\n").utf8_str();
		copy(buffer.data(), buffer.data() + buffer.length(), back_inserter(dst));
	}
}

bool AssFile::CanSave() const {
	const SubtitleFormat *writer = SubtitleFormat::GetWriter(filename);
	return writer && writer->CanSave(this);
}

void AssFile::AddLine(wxString data, int *version, AssAttachment **attach) {
	// Is this line an attachment filename?
	bool isFilename = data.StartsWith("fontname: ") || data.StartsWith("filename: ");

	// If there's an attachment in progress, deal with it first as an
	// attachment data line can appear to be other things
	if (*attach) {
		// Check if it's valid data
		bool validData = data.size() > 0 && data.size() <= 80;
		for (size_t i = 0; i < data.size(); ++i) {
			if (data[i] < 33 || data[i] >= 97) {
				validData = false;
				break;
			}
		}

		// Data is over, add attachment to the file
		if (!validData || isFilename) {
			(*attach)->Finish();
			Line.push_back(*attach);
			*attach = NULL;
		}
		else {
			// Insert data
			(*attach)->AddData(data);

			// Done building
			if (data.Length() < 80) {
				(*attach)->Finish();
				Line.push_back(*attach);
				*attach = NULL;
				return;
			}
		}
	}

	if (data.empty()) return;

	// Section header
	if (data[0] == '[' && data.Last() == ']') {
		// Ugly hacks to allow intermixed v4 and v4+ style sections
		wxString low = data.Lower();
		if (low == "[v4 styles]") {
			data = "[V4+ Styles]";
			*version = 0;
		}
		else if (low == "[v4+ styles]") {
			data = "[V4+ Styles]";
			*version = 1;
		}
		else if (low == "[v4++ styles]") {
			data = "[V4+ Styles]";
			*version = 2;
		}

		Line.push_back(new AssEntry(data, data));
		return;
	}

	// If the first nonblank line isn't a header pretend it starts with [Script Info]
	if (Line.empty())
		Line.push_back(new AssEntry("[Script Info]", "[Script Info]"));

	wxString group = Line.back()->group;
	wxString lowGroup = group.Lower();

	// Attachment
	if (lowGroup == "[fonts]" || lowGroup == "[graphics]") {
		if (isFilename) {
			*attach = new AssAttachment(data.Mid(10), group);
		}
	}
	// Dialogue
	else if (lowGroup == "[events]") {
		if (data.StartsWith("Dialogue:") || data.StartsWith("Comment:"))
			Line.push_back(new AssDialogue(data, *version));
		else if (data.StartsWith("Format:"))
			Line.push_back(new AssEntry("Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text", group));
	}
	// Style
	else if (lowGroup == "[v4+ styles]") {
		if (data.StartsWith("Style:"))
			Line.push_back(new AssStyle(data, *version));
		else if (data.StartsWith("Format:"))
			Line.push_back(new AssEntry("Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding", group));
	}
	// Script info
	else if (lowGroup == "[script info]") {
		// Comment
		if (data.StartsWith(";")) {
			// Skip stupid comments added by other programs
			// Of course, we'll add our own in place later... ;)
			return;
		}

		if (data.StartsWith("ScriptType:")) {
			wxString versionString = data.Mid(11).Trim(true).Trim(false).Lower();
			int trueVersion;
			if (versionString == "v4.00")
				trueVersion = 0;
			else if (versionString == "v4.00+")
				trueVersion = 1;
			else if (versionString == "v4.00++")
				trueVersion = 2;
			else throw
				"Unknown SSA file format version";
			if (trueVersion != *version) {
				if (!(trueVersion == 2 && *version == 1)) {
					wxLogMessage("Warning: File has the wrong extension.");
				}
				*version = trueVersion;
			}
		}

		// Everything
		Line.push_back(new AssEntry(data, group));
	}
	// Unrecognized group
	else {
		Line.push_back(new AssEntry(data, group));
	}
}

void AssFile::Clear() {
	background_delete_clear(Line);

	loaded = false;
	filename.clear();
	UndoStack.clear();
	RedoStack.clear();
	undoDescription.clear();
}

void AssFile::LoadDefault(bool defline) {
	Clear();

	// Write headers
	AssAttachment *attach = 0;
	int version = 1;
	AddLine("[Script Info]", &version, &attach);
	AddLine("Title: Default Aegisub file", &version, &attach);
	AddLine("ScriptType: v4.00+", &version, &attach);
	AddLine("WrapStyle: 0", &version, &attach);
	AddLine("ScaledBorderAndShadow: yes", &version, &attach);
	AddLine("Collisions: Normal", &version, &attach);
	if (!OPT_GET("Subtitle/Default Resolution/Auto")->GetBool()) {
		AddLine(wxString::Format("PlayResX: %" PRId64, OPT_GET("Subtitle/Default Resolution/Width")->GetInt()), &version, &attach);
		AddLine(wxString::Format("PlayResY: %" PRId64, OPT_GET("Subtitle/Default Resolution/Height")->GetInt()), &version, &attach);
	}
	AddLine("YCbCr Matrix: None", &version, &attach);

	InsertStyle(new AssStyle);

	AddLine("[Events]", &version, &attach);
	AddLine("Format:  Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text", &version, &attach);

	if (defline)
		Line.push_back(new AssDialogue);

	autosavedCommitId = savedCommitId = commitId + 1;
	Commit("", COMMIT_NEW);
	loaded = true;
	StandardPaths::SetPathValue("?script", "");
	FileOpen("");
}

void AssFile::swap(AssFile &that) throw() {
	// Intentionally does not swap undo stack related things
	std::swap(loaded, that.loaded);
	std::swap(commitId, that.commitId);
	std::swap(undoDescription, that.undoDescription);
	std::swap(Line, that.Line);
}

AssFile::AssFile(const AssFile &from)
: undoDescription(from.undoDescription)
, commitId(from.commitId)
, filename(from.filename)
, loaded(from.loaded)
{
	std::transform(from.Line.begin(), from.Line.end(), std::back_inserter(Line), std::mem_fun(&AssEntry::Clone));
}
AssFile& AssFile::operator=(AssFile from) {
	std::swap(*this, from);
	return *this;
}

static bool try_insert(std::list<AssEntry*> &lines, AssEntry *entry) {
	if (lines.empty()) return false;

	// Search for insertion point
	std::list<AssEntry*>::iterator it = lines.end();
	do {
		--it;
		if ((*it)->group == entry->group) {
			lines.insert(++it, entry);
			return true;
		}
	} while (it != lines.begin());

	return false;
}

void AssFile::InsertStyle(AssStyle *style) {
	if (try_insert(Line, style)) return;

	// No styles found, add them
	Line.push_back(new AssEntry("[V4+ Styles]", "[V4+ Styles]"));
	Line.push_back(new AssEntry("Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding", "[V4+ Styles]"));
	Line.push_back(style);
}

void AssFile::InsertAttachment(AssAttachment *attach) {
	if (try_insert(Line, attach)) return;

	// Didn't find a group of the appropriate type so create it
	Line.push_back(new AssEntry(attach->group, attach->group));
	Line.push_back(attach);
}

void AssFile::InsertAttachment(wxString filename) {
	wxString group("[Graphics]");

	wxString ext = filename.Right(4).Lower();
	if (ext == ".ttf" || ext == ".ttc" || ext == ".pfb")
		group = "[Fonts]";

	std::auto_ptr<AssAttachment> newAttach(new AssAttachment(wxFileName(filename).GetFullName(), group));
	newAttach->Import(filename);

	InsertAttachment(newAttach.release());
}

void AssFile::InsertDialogue(AssDialogue *diag) {
	if (try_insert(Line, diag)) return;

	// Didn't find a group of the appropriate type so create it
	Line.push_back(new AssEntry("[Events]", "[Events]"));
	Line.push_back(new AssEntry("Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text", "[Events]"));
	Line.push_back(diag);
}

wxString AssFile::GetScriptInfo(wxString key) const {
	key.MakeLower();
	key += ":";
	bool GotIn = false;

	for (std::list<AssEntry*>::const_iterator cur = Line.begin(); cur != Line.end(); ++cur) {
		if ((*cur)->group == "[Script Info]") {
			GotIn = true;
			wxString curText = (*cur)->GetEntryData();
			if (curText.Lower().StartsWith(key))
				return curText.Mid(key.size()).Trim(true).Trim(false);
		}
		else if (GotIn) return "";
	}

	return "";
}

int AssFile::GetScriptInfoAsInt(const wxString key) const {
	long temp = 0;
	GetScriptInfo(key).ToLong(&temp);
	return temp;
}

void AssFile::SetScriptInfo(wxString const& key, wxString const& value) {
	wxString search_key = key.Lower() + ":";
	size_t key_size = search_key.size();
	std::list<AssEntry*>::iterator script_info_end;
	bool found_script_info = false;

	for (std::list<AssEntry*>::iterator cur = Line.begin(); cur != Line.end(); ++cur) {
		if ((*cur)->group == "[Script Info]") {
			found_script_info = true;
			wxString cur_text = (*cur)->GetEntryData().Left(key_size).Lower();

			if (cur_text == search_key) {
				if (value.empty()) {
					delete *cur;
					Line.erase(cur);
				}
				else {
					(*cur)->SetEntryData(key + ": " + value);
				}
				return;
			}
			script_info_end = cur;
		}
		else if (found_script_info) {
			if (value.size())
				Line.insert(script_info_end, new AssEntry(key + ": " + value, "[Script Info]"));
			return;
		}
	}

	// Found a script info section, but not this key or anything after it,
	// so add it at the end of the file
	if (found_script_info)
		Line.push_back(new AssEntry(key + ": " + value, "[Script Info]"));
	// Script info section not found, so add it at the beginning of the file
	else {
		Line.push_front(new AssEntry(key + ": " + value, "[Script Info]"));
		Line.push_front(new AssEntry("[Script Info]", "[Script Info]"));
	}
}

void AssFile::GetResolution(int &sw,int &sh) const {
	sw = GetScriptInfoAsInt("PlayResX");
	sh = GetScriptInfoAsInt("PlayResY");

	// Gabest logic?
	if (sw == 0 && sh == 0) {
		sw = 384;
		sh = 288;
	} else if (sw == 0) {
		if (sh == 1024)
			sw = 1280;
		else
			sw = sh * 4 / 3;
	} else if (sh == 0) {
		// you are not crazy; this doesn't make any sense
		if (sw == 1280)
			sh = 1024;
		else
			sh = sw * 3 / 4;
	}
}

void AssFile::AddComment(wxString comment) {
	comment.Prepend("; ");
	int step = 0;

	for (std::list<AssEntry*>::iterator cur = Line.begin(); cur != Line.end(); ++cur) {
		// Start of group
		if (step == 0 && (*cur)->group == "[Script Info]")
			step = 1;

		// First line after a ;
		else if (step == 1 && !(*cur)->GetEntryData().StartsWith(";")) {
			Line.insert(cur, new AssEntry(comment, "[Script Info]"));
			break;
		}
	}
}

wxArrayString AssFile::GetStyles() const {
	wxArrayString styles;
	for (std::list<AssEntry*>::const_iterator cur = Line.begin(); cur != Line.end(); ++cur) {
		if (AssStyle *curstyle = dynamic_cast<AssStyle*>(*cur))
			styles.Add(curstyle->name);
	}
	return styles;
}

AssStyle *AssFile::GetStyle(wxString name) {
	for (entryIter cur=Line.begin();cur!=Line.end();cur++) {
		AssStyle *curstyle = dynamic_cast<AssStyle*>(*cur);
		if (curstyle && curstyle->name == name)
			return curstyle;
	}
	return NULL;
}

void AssFile::AddToRecent(wxString file) {
	config::mru->Add("Subtitle", STD_STR(file));
	wxFileName filepath(file);
	OPT_SET("Path/Last/Subtitles")->SetString(STD_STR(filepath.GetPath()));
}

wxString AssFile::GetWildcardList(int mode) {
	if (mode == 0) return SubtitleFormat::GetWildcards(0);
	else if (mode == 1) return "Advanced Substation Alpha (*.ass)|*.ass";
	else if (mode == 2) return SubtitleFormat::GetWildcards(1);
	else return "";
}

int AssFile::Commit(wxString desc, int type, int amendId, AssEntry *single_line) {
	++commitId;
	// Allow coalescing only if it's the last change and the file has not been
	// saved since the last change
	if (commitId == amendId+1 && RedoStack.empty() && savedCommitId+1 != commitId && autosavedCommitId+1 != commitId) {
		// If only one line changed just modify it instead of copying the file
		if (single_line) {
			entryIter this_it = Line.begin(), undo_it = UndoStack.back().Line.begin();
			while (*this_it != single_line) {
				++this_it;
				++undo_it;
			}
			delete *undo_it;
			*undo_it = single_line->Clone();
		}
		else {
			UndoStack.back() = *this;
		}
		AnnounceCommit(type);
		return commitId;
	}

	RedoStack.clear();

	// Place copy on stack
	undoDescription = desc;
	UndoStack.push_back(*this);

	// Cap depth
	int depth = std::max<int>(OPT_GET("Limits/Undo Levels")->GetInt(), 2);
	while ((int)UndoStack.size() > depth) {
		UndoStack.pop_front();
	}

	if (UndoStack.size() > 1 && OPT_GET("App/Auto/Save on Every Change")->GetBool() && !filename.empty() && CanSave())
		Save(filename);

	AnnounceCommit(type);
	return commitId;
}

void AssFile::Undo() {
	if (UndoStack.size() <= 1) return;

	RedoStack.push_back(AssFile());
	std::swap(RedoStack.back(), *this);
	UndoStack.pop_back();
	*this = UndoStack.back();

	AnnounceCommit(COMMIT_NEW);
}

void AssFile::Redo() {
	if (RedoStack.empty()) return;

	std::swap(*this, RedoStack.back());
	UndoStack.push_back(*this);
	RedoStack.pop_back();

	AnnounceCommit(COMMIT_NEW);
}

wxString AssFile::GetUndoDescription() const {
	return IsUndoStackEmpty() ? "" : UndoStack.back().undoDescription;
}

wxString AssFile::GetRedoDescription() const {
	return IsRedoStackEmpty() ? "" : RedoStack.back().undoDescription;
}

bool AssFile::CompStart(const AssDialogue* lft, const AssDialogue* rgt) {
	return lft->Start < rgt->Start;
}
bool AssFile::CompEnd(const AssDialogue* lft, const AssDialogue* rgt) {
	return lft->End < rgt->End;
}
bool AssFile::CompStyle(const AssDialogue* lft, const AssDialogue* rgt) {
	return lft->Style < rgt->Style;
}
bool AssFile::CompActor(const AssDialogue* lft, const AssDialogue* rgt) {
	return lft->Actor < rgt->Actor;
}
bool AssFile::CompEffect(const AssDialogue* lft, const AssDialogue* rgt) {
	return lft->Effect < rgt->Effect;
}
bool AssFile::CompLayer(const AssDialogue* lft, const AssDialogue* rgt) {
	return lft->Layer < rgt->Layer;
}

void AssFile::Sort(CompFunc comp, std::set<AssDialogue*> const& limit) {
	Sort(Line, comp, limit);
}
namespace {
	struct AssEntryComp : public std::binary_function<const AssEntry*, const AssEntry*, bool> {
		AssFile::CompFunc comp;
		bool operator()(const AssEntry* a, const AssEntry* b) const {
			return comp(static_cast<const AssDialogue*>(a), static_cast<const AssDialogue*>(b));
		}
	};

	inline bool is_dialogue(AssEntry *e, std::set<AssDialogue*> const& limit) {
		AssDialogue *d = dynamic_cast<AssDialogue*>(e);
		return d && (limit.empty() || limit.count(d));
	}
}
void AssFile::Sort(std::list<AssEntry*> &lst, CompFunc comp, std::set<AssDialogue*> const& limit) {
	AssEntryComp compE;
	compE.comp = comp;
	// Sort each block of AssDialogues separately, leaving everything else untouched
	for (entryIter begin = lst.begin(); begin != lst.end(); ++begin) {
		if (!is_dialogue(*begin, limit)) continue;
		entryIter end = begin;
		while (end != lst.end() && is_dialogue(*end, limit)) ++end;

		// used instead of std::list::sort for partial list sorting
		std::list<AssEntry*> tmp;
		tmp.splice(tmp.begin(), lst, begin, end);
		tmp.sort(compE);
		lst.splice(end, tmp);

		begin = --end;
	}
}
void AssFile::Sort(std::list<AssDialogue*> &lst, CompFunc comp) {
	lst.sort(comp);
}

AssFile *AssFile::top;
