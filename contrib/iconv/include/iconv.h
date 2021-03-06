 / *   C o p y r i g h t   ( C )   1 9 9 9 - 2 0 0 3 ,   2 0 0 5 - 2 0 0 6 ,   2 0 0 8 - 2 0 0 9   F r e e   S o f t w a r e   F o u n d a t i o n ,   I n c .  
       T h i s   f i l e   i s   p a r t   o f   t h e   G N U   L I B I C O N V   L i b r a r y .  
  
       T h e   G N U   L I B I C O N V   L i b r a r y   i s   f r e e   s o f t w a r e ;   y o u   c a n   r e d i s t r i b u t e   i t  
       a n d / o r   m o d i f y   i t   u n d e r   t h e   t e r m s   o f   t h e   G N U   L i b r a r y   G e n e r a l   P u b l i c  
       L i c e n s e   a s   p u b l i s h e d   b y   t h e   F r e e   S o f t w a r e   F o u n d a t i o n ;   e i t h e r   v e r s i o n   2  
       o f   t h e   L i c e n s e ,   o r   ( a t   y o u r   o p t i o n )   a n y   l a t e r   v e r s i o n .  
  
       T h e   G N U   L I B I C O N V   L i b r a r y   i s   d i s t r i b u t e d   i n   t h e   h o p e   t h a t   i t   w i l l   b e  
       u s e f u l ,   b u t   W I T H O U T   A N Y   W A R R A N T Y ;   w i t h o u t   e v e n   t h e   i m p l i e d   w a r r a n t y   o f  
       M E R C H A N T A B I L I T Y   o r   F I T N E S S   F O R   A   P A R T I C U L A R   P U R P O S E .     S e e   t h e   G N U  
       L i b r a r y   G e n e r a l   P u b l i c   L i c e n s e   f o r   m o r e   d e t a i l s .  
  
       Y o u   s h o u l d   h a v e   r e c e i v e d   a   c o p y   o f   t h e   G N U   L i b r a r y   G e n e r a l   P u b l i c  
       L i c e n s e   a l o n g   w i t h   t h e   G N U   L I B I C O N V   L i b r a r y ;   s e e   t h e   f i l e   C O P Y I N G . L I B .  
       I f   n o t ,   w r i t e   t o   t h e   F r e e   S o f t w a r e   F o u n d a t i o n ,   I n c . ,   5 1   F r a n k l i n   S t r e e t ,  
       F i f t h   F l o o r ,   B o s t o n ,   M A   0 2 1 1 0 - 1 3 0 1 ,   U S A .     * /  
  
 / *   W h e n   i n s t a l l e d ,   t h i s   f i l e   i s   c a l l e d   " i c o n v . h " .   * /  
  
 # i f n d e f   _ L I B I C O N V _ H  
 # d e f i n e   _ L I B I C O N V _ H  
  
 # d e f i n e   _ L I B I C O N V _ V E R S I O N   0 x 0 1 0 D         / *   v e r s i o n   n u m b e r :   ( m a j o r < < 8 )   +   m i n o r   * /  
  
 # i f   1   & &   B U I L D I N G _ L I B I C O N V  
 # d e f i n e   L I B I C O N V _ D L L _ E X P O R T E D   _ _ a t t r i b u t e _ _ ( ( _ _ v i s i b i l i t y _ _ ( " d e f a u l t " ) ) )  
 # e l s e  
 # d e f i n e   L I B I C O N V _ D L L _ E X P O R T E D  
 # e n d i f  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   _ _ d e c l s p e c   ( d l l i m p o r t )   i n t   _ l i b i c o n v _ v e r s i o n ;   / *   L i k e w i s e   * /  
  
 / *   W e   w o u l d   l i k e   t o   # i n c l u d e   a n y   s y s t e m   h e a d e r   f i l e   w h i c h   c o u l d   d e f i n e  
       i c o n v _ t ,   1 .   i n   o r d e r   t o   e l i m i n a t e   t h e   r i s k   t h a t   t h e   u s e r   g e t s   c o m p i l a t i o n  
       e r r o r s   b e c a u s e   s o m e   o t h e r   s y s t e m   h e a d e r   f i l e   i n c l u d e s   / u s r / i n c l u d e / i c o n v . h  
       w h i c h   d e f i n e s   i c o n v _ t   o r   d e c l a r e s   i c o n v   a f t e r   t h i s   f i l e ,   2 .   w h e n   c o m p i l i n g  
       f o r   L I B I C O N V _ P L U G ,   w e   n e e d   t h e   p r o p e r   i c o n v _ t   t y p e   i n   o r d e r   t o   p r o d u c e  
       b i n a r y   c o m p a t i b l e   c o d e .  
       B u t   g c c ' s   # i n c l u d e _ n e x t   i s   n o t   p o r t a b l e .   T h u s ,   o n c e   l i b i c o n v ' s   i c o n v . h  
       h a s   b e e n   i n s t a l l e d   i n   / u s r / l o c a l / i n c l u d e ,   t h e r e   i s   n o   w a y   a n y   m o r e   t o  
       i n c l u d e   t h e   o r i g i n a l   / u s r / i n c l u d e / i c o n v . h .   W e   s i m p l y   h a v e   t o   g e t   a w a y  
       w i t h o u t   i t .  
       A d   1 .   T h e   r i s k   t h a t   a   s y s t e m   h e a d e r   f i l e   d o e s  
       # i n c l u d e   " i c o n v . h "     o r     # i n c l u d e _ n e x t   " i c o n v . h "  
       i s   s m a l l .   T h e y   a l l   d o   # i n c l u d e   < i c o n v . h > .  
       A d   2 .   T h e   i c o n v _ t   t y p e   i s   a   p o i n t e r   t y p e   i n   a l l   c a s e s   I   h a v e   s e e n .   ( I t  
       h a s   t o   b e   a   s c a l a r   t y p e   b e c a u s e   ( i c o n v _ t ) ( - 1 )   i s   a   p o s s i b l e   r e t u r n   v a l u e  
       f r o m   i c o n v _ o p e n ( ) . )   * /  
  
 / *   D e f i n e   i c o n v _ t   o u r s e l v e s .   * /  
 # u n d e f   i c o n v _ t  
 # d e f i n e   i c o n v _ t   l i b i c o n v _ t  
 t y p e d e f   v o i d *   i c o n v _ t ;  
  
 / *   G e t   s i z e _ t   d e c l a r a t i o n .  
       G e t   w c h a r _ t   d e c l a r a t i o n   i f   i t   e x i s t s .   * /  
 # i n c l u d e   < s t d d e f . h >  
  
 / *   G e t   e r r n o   d e c l a r a t i o n   a n d   v a l u e s .   * /  
 # i n c l u d e   < e r r n o . h >  
 / *   S o m e   s y s t e m s ,   l i k e   S u n O S   4 ,   d o n ' t   h a v e   E I L S E Q .   S o m e   s y s t e m s ,   l i k e   B S D / O S ,  
       h a v e   E I L S E Q   i n   a   d i f f e r e n t   h e a d e r .     O n   t h e s e   s y s t e m s ,   d e f i n e   E I L S E Q  
       o u r s e l v e s .   * /  
 # i f n d e f   E I L S E Q  
 # d e f i n e   E I L S E Q    
 # e n d i f  
  
  
 # i f d e f   _ _ c p l u s p l u s  
 e x t e r n   " C "   {  
 # e n d i f  
  
  
 / *   A l l o c a t e s   d e s c r i p t o r   f o r   c o d e   c o n v e r s i o n   f r o m   e n c o d i n g    f r o m c o d e    t o  
       e n c o d i n g    t o c o d e  .   * /  
 # i f n d e f   L I B I C O N V _ P L U G  
 # d e f i n e   i c o n v _ o p e n   l i b i c o n v _ o p e n  
 # e n d i f  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   i c o n v _ t   i c o n v _ o p e n   ( c o n s t   c h a r *   t o c o d e ,   c o n s t   c h a r *   f r o m c o d e ) ;  
  
 / *   C o n v e r t s ,   u s i n g   c o n v e r s i o n   d e s c r i p t o r    c d  ,   a t   m o s t    * i n b y t e s l e f t    b y t e s  
       s t a r t i n g   a t    * i n b u f  ,   w r i t i n g   a t   m o s t    * o u t b y t e s l e f t    b y t e s   s t a r t i n g   a t  
        * o u t b u f  .  
       D e c r e m e n t s    * i n b y t e s l e f t    a n d   i n c r e m e n t s    * i n b u f    b y   t h e   s a m e   a m o u n t .  
       D e c r e m e n t s    * o u t b y t e s l e f t    a n d   i n c r e m e n t s    * o u t b u f    b y   t h e   s a m e   a m o u n t .   * /  
 # i f n d e f   L I B I C O N V _ P L U G  
 # d e f i n e   i c o n v   l i b i c o n v  
 # e n d i f  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   s i z e _ t   i c o n v   ( i c o n v _ t   c d ,     c h a r *   *   i n b u f ,   s i z e _ t   * i n b y t e s l e f t ,   c h a r *   *   o u t b u f ,   s i z e _ t   * o u t b y t e s l e f t ) ;  
  
 / *   F r e e s   r e s o u r c e s   a l l o c a t e d   f o r   c o n v e r s i o n   d e s c r i p t o r    c d  .   * /  
 # i f n d e f   L I B I C O N V _ P L U G  
 # d e f i n e   i c o n v _ c l o s e   l i b i c o n v _ c l o s e  
 # e n d i f  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   i n t   i c o n v _ c l o s e   ( i c o n v _ t   c d ) ;  
  
  
 # i f n d e f   L I B I C O N V _ P L U G  
  
 / *   N o n s t a n d a r d   e x t e n s i o n s .   * /  
  
 # i f   1  
 # i f   0  
 / *   T r u 6 4   w i t h   D e s k t o p   T o o l k i t   C   h a s   a   b u g :   < s t d i o . h >   m u s t   b e   i n c l u d e d   b e f o r e  
       < w c h a r . h > .  
       B S D / O S   4 . 0 . 1   h a s   a   b u g :   < s t d d e f . h > ,   < s t d i o . h >   a n d   < t i m e . h >   m u s t   b e  
       i n c l u d e d   b e f o r e   < w c h a r . h > .     * /  
 # i n c l u d e   < s t d d e f . h >  
 # i n c l u d e   < s t d i o . h >  
 # i n c l u d e   < t i m e . h >  
 # e n d i f  
 # i n c l u d e   < w c h a r . h >  
 # e n d i f  
  
 / *   A   t y p e   t h a t   h o l d s   a l l   m e m o r y   n e e d e d   b y   a   c o n v e r s i o n   d e s c r i p t o r .  
       A   p o i n t e r   t o   s u c h   a n   o b j e c t   c a n   b e   u s e d   a s   a n   i c o n v _ t .   * /  
 t y p e d e f   s t r u c t   {  
     v o i d *   d u m m y 1 [ 2 8 ] ;  
 # i f   1  
     m b s t a t e _ t   d u m m y 2 ;  
 # e n d i f  
 }   i c o n v _ a l l o c a t i o n _ t ;  
  
 / *   A l l o c a t e s   d e s c r i p t o r   f o r   c o d e   c o n v e r s i o n   f r o m   e n c o d i n g    f r o m c o d e    t o  
       e n c o d i n g    t o c o d e    i n t o   p r e a l l o c a t e d   m e m o r y .   R e t u r n s   a n   e r r o r   i n d i c a t o r  
       ( 0   o r   - 1   w i t h   e r r n o   s e t ) .   * /  
 # d e f i n e   i c o n v _ o p e n _ i n t o   l i b i c o n v _ o p e n _ i n t o  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   i n t   i c o n v _ o p e n _ i n t o   ( c o n s t   c h a r *   t o c o d e ,   c o n s t   c h a r *   f r o m c o d e ,  
                                                         i c o n v _ a l l o c a t i o n _ t *   r e s u l t p ) ;  
  
 / *   C o n t r o l   o f   a t t r i b u t e s .   * /  
 # d e f i n e   i c o n v c t l   l i b i c o n v c t l  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   i n t   i c o n v c t l   ( i c o n v _ t   c d ,   i n t   r e q u e s t ,   v o i d *   a r g u m e n t ) ;  
  
 / *   H o o k   p e r f o r m e d   a f t e r   e v e r y   s u c c e s s f u l   c o n v e r s i o n   o f   a   U n i c o d e   c h a r a c t e r .   * /  
 t y p e d e f   v o i d   ( * i c o n v _ u n i c o d e _ c h a r _ h o o k )   ( u n s i g n e d   i n t   u c ,   v o i d *   d a t a ) ;  
 / *   H o o k   p e r f o r m e d   a f t e r   e v e r y   s u c c e s s f u l   c o n v e r s i o n   o f   a   w i d e   c h a r a c t e r .   * /  
 t y p e d e f   v o i d   ( * i c o n v _ w i d e _ c h a r _ h o o k )   ( w c h a r _ t   w c ,   v o i d *   d a t a ) ;  
 / *   S e t   o f   h o o k s .   * /  
 s t r u c t   i c o n v _ h o o k s   {  
     i c o n v _ u n i c o d e _ c h a r _ h o o k   u c _ h o o k ;  
     i c o n v _ w i d e _ c h a r _ h o o k   w c _ h o o k ;  
     v o i d *   d a t a ;  
 } ;  
  
 / *   F a l l b a c k   f u n c t i o n .     I n v o k e d   w h e n   a   s m a l l   n u m b e r   o f   b y t e s   c o u l d   n o t   b e  
       c o n v e r t e d   t o   a   U n i c o d e   c h a r a c t e r .     T h i s   f u n c t i o n   s h o u l d   p r o c e s s   a l l  
       b y t e s   f r o m   i n b u f   a n d   m a y   p r o d u c e   r e p l a c e m e n t   U n i c o d e   c h a r a c t e r s   b y   c a l l i n g  
       t h e   w r i t e _ r e p l a c e m e n t   c a l l b a c k   r e p e a t e d l y .     * /  
 t y p e d e f   v o i d   ( * i c o n v _ u n i c o d e _ m b _ t o _ u c _ f a l l b a c k )  
                           ( c o n s t   c h a r *   i n b u f ,   s i z e _ t   i n b u f s i z e ,  
                             v o i d   ( * w r i t e _ r e p l a c e m e n t )   ( c o n s t   u n s i g n e d   i n t   * b u f ,   s i z e _ t   b u f l e n ,  
                                                                                   v o i d *   c a l l b a c k _ a r g ) ,  
                             v o i d *   c a l l b a c k _ a r g ,  
                             v o i d *   d a t a ) ;  
 / *   F a l l b a c k   f u n c t i o n .     I n v o k e d   w h e n   a   U n i c o d e   c h a r a c t e r   c o u l d   n o t   b e   c o n v e r t e d  
       t o   t h e   t a r g e t   e n c o d i n g .     T h i s   f u n c t i o n   s h o u l d   p r o c e s s   t h e   c h a r a c t e r   a n d  
       m a y   p r o d u c e   r e p l a c e m e n t   b y t e s   ( i n   t h e   t a r g e t   e n c o d i n g )   b y   c a l l i n g   t h e  
       w r i t e _ r e p l a c e m e n t   c a l l b a c k   r e p e a t e d l y .     * /  
 t y p e d e f   v o i d   ( * i c o n v _ u n i c o d e _ u c _ t o _ m b _ f a l l b a c k )  
                           ( u n s i g n e d   i n t   c o d e ,  
                             v o i d   ( * w r i t e _ r e p l a c e m e n t )   ( c o n s t   c h a r   * b u f ,   s i z e _ t   b u f l e n ,  
                                                                                   v o i d *   c a l l b a c k _ a r g ) ,  
                             v o i d *   c a l l b a c k _ a r g ,  
                             v o i d *   d a t a ) ;  
 # i f   1  
 / *   F a l l b a c k   f u n c t i o n .     I n v o k e d   w h e n   a   n u m b e r   o f   b y t e s   c o u l d   n o t   b e   c o n v e r t e d   t o  
       a   w i d e   c h a r a c t e r .     T h i s   f u n c t i o n   s h o u l d   p r o c e s s   a l l   b y t e s   f r o m   i n b u f   a n d   m a y  
       p r o d u c e   r e p l a c e m e n t   w i d e   c h a r a c t e r s   b y   c a l l i n g   t h e   w r i t e _ r e p l a c e m e n t  
       c a l l b a c k   r e p e a t e d l y .     * /  
 t y p e d e f   v o i d   ( * i c o n v _ w c h a r _ m b _ t o _ w c _ f a l l b a c k )  
                           ( c o n s t   c h a r *   i n b u f ,   s i z e _ t   i n b u f s i z e ,  
                             v o i d   ( * w r i t e _ r e p l a c e m e n t )   ( c o n s t   w c h a r _ t   * b u f ,   s i z e _ t   b u f l e n ,  
                                                                                   v o i d *   c a l l b a c k _ a r g ) ,  
                             v o i d *   c a l l b a c k _ a r g ,  
                             v o i d *   d a t a ) ;  
 / *   F a l l b a c k   f u n c t i o n .     I n v o k e d   w h e n   a   w i d e   c h a r a c t e r   c o u l d   n o t   b e   c o n v e r t e d   t o  
       t h e   t a r g e t   e n c o d i n g .     T h i s   f u n c t i o n   s h o u l d   p r o c e s s   t h e   c h a r a c t e r   a n d   m a y  
       p r o d u c e   r e p l a c e m e n t   b y t e s   ( i n   t h e   t a r g e t   e n c o d i n g )   b y   c a l l i n g   t h e  
       w r i t e _ r e p l a c e m e n t   c a l l b a c k   r e p e a t e d l y .     * /  
 t y p e d e f   v o i d   ( * i c o n v _ w c h a r _ w c _ t o _ m b _ f a l l b a c k )  
                           ( w c h a r _ t   c o d e ,  
                             v o i d   ( * w r i t e _ r e p l a c e m e n t )   ( c o n s t   c h a r   * b u f ,   s i z e _ t   b u f l e n ,  
                                                                                   v o i d *   c a l l b a c k _ a r g ) ,  
                             v o i d *   c a l l b a c k _ a r g ,  
                             v o i d *   d a t a ) ;  
 # e l s e  
 / *   I f   t h e   w c h a r _ t   t y p e   d o e s   n o t   e x i s t ,   t h e s e   t w o   f a l l b a c k   f u n c t i o n s   a r e   n e v e r  
       i n v o k e d .     T h e i r   a r g u m e n t   l i s t   t h e r e f o r e   d o e s   n o t   m a t t e r .     * /  
 t y p e d e f   v o i d   ( * i c o n v _ w c h a r _ m b _ t o _ w c _ f a l l b a c k )   ( ) ;  
 t y p e d e f   v o i d   ( * i c o n v _ w c h a r _ w c _ t o _ m b _ f a l l b a c k )   ( ) ;  
 # e n d i f  
 / *   S e t   o f   f a l l b a c k s .   * /  
 s t r u c t   i c o n v _ f a l l b a c k s   {  
     i c o n v _ u n i c o d e _ m b _ t o _ u c _ f a l l b a c k   m b _ t o _ u c _ f a l l b a c k ;  
     i c o n v _ u n i c o d e _ u c _ t o _ m b _ f a l l b a c k   u c _ t o _ m b _ f a l l b a c k ;  
     i c o n v _ w c h a r _ m b _ t o _ w c _ f a l l b a c k   m b _ t o _ w c _ f a l l b a c k ;  
     i c o n v _ w c h a r _ w c _ t o _ m b _ f a l l b a c k   w c _ t o _ m b _ f a l l b a c k ;  
     v o i d *   d a t a ;  
 } ;  
  
 / *   R e q u e s t s   f o r   i c o n v c t l .   * /  
 # d e f i n e   I C O N V _ T R I V I A L P                         0     / *   i n t   * a r g u m e n t   * /  
 # d e f i n e   I C O N V _ G E T _ T R A N S L I T E R A T E       1     / *   i n t   * a r g u m e n t   * /  
 # d e f i n e   I C O N V _ S E T _ T R A N S L I T E R A T E       2     / *   c o n s t   i n t   * a r g u m e n t   * /  
 # d e f i n e   I C O N V _ G E T _ D I S C A R D _ I L S E Q       3     / *   i n t   * a r g u m e n t   * /  
 # d e f i n e   I C O N V _ S E T _ D I S C A R D _ I L S E Q       4     / *   c o n s t   i n t   * a r g u m e n t   * /  
 # d e f i n e   I C O N V _ S E T _ H O O K S                       5     / *   c o n s t   s t r u c t   i c o n v _ h o o k s   * a r g u m e n t   * /  
 # d e f i n e   I C O N V _ S E T _ F A L L B A C K S               6     / *   c o n s t   s t r u c t   i c o n v _ f a l l b a c k s   * a r g u m e n t   * /  
  
 / *   L i s t i n g   o f   l o c a l e   i n d e p e n d e n t   e n c o d i n g s .   * /  
 # d e f i n e   i c o n v l i s t   l i b i c o n v l i s t  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   v o i d   i c o n v l i s t   ( i n t   ( * d o _ o n e )   ( u n s i g n e d   i n t   n a m e s c o u n t ,  
                                                                             c o n s t   c h a r   *   c o n s t   *   n a m e s ,  
                                                                             v o i d *   d a t a ) ,  
                                               v o i d *   d a t a ) ;  
  
 / *   C a n o n i c a l i z e   a n   e n c o d i n g   n a m e .  
       T h e   r e s u l t   i s   e i t h e r   a   c a n o n i c a l   e n c o d i n g   n a m e ,   o r   n a m e   i t s e l f .   * /  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   c o n s t   c h a r   *   i c o n v _ c a n o n i c a l i z e   ( c o n s t   c h a r   *   n a m e ) ;  
  
 / *   S u p p o r t   f o r   r e l o c a t a b l e   p a c k a g e s .     * /  
  
 / *   S e t s   t h e   o r i g i n a l   a n d   t h e   c u r r e n t   i n s t a l l a t i o n   p r e f i x   o f   t h e   p a c k a g e .  
       R e l o c a t i o n   s i m p l y   r e p l a c e s   a   p a t h n a m e   s t a r t i n g   w i t h   t h e   o r i g i n a l   p r e f i x  
       b y   t h e   c o r r e s p o n d i n g   p a t h n a m e   w i t h   t h e   c u r r e n t   p r e f i x   i n s t e a d .     B o t h  
       p r e f i x e s   s h o u l d   b e   d i r e c t o r y   n a m e s   w i t h o u t   t r a i l i n g   s l a s h   ( i . e .   u s e   " "  
       i n s t e a d   o f   " / " ) .     * /  
 e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   v o i d   l i b i c o n v _ s e t _ r e l o c a t i o n _ p r e f i x   ( c o n s t   c h a r   * o r i g _ p r e f i x ,  
 	 	 	 	 	         c o n s t   c h a r   * c u r r _ p r e f i x ) ;  
  
 # e n d i f  
  
 / *   m i n g w 3 2 ' s   e r r n o   i s   n o t   m s v c ' s   e r r n o ,   w h i c h   m a k e s   d i r e c t l y   g e t t i n g   i c o n v ' s  
       e r r o r s   i m p o s s i b l e   w h e n   c o m b i n i n g   t h e m   * /  
 / / # d e f i n e   e r r n o _ W O R K A R O U N D   1  
 / / e x t e r n   L I B I C O N V _ D L L _ E X P O R T E D   i n t   e r r n o ;  
 # i f d e f   _ _ c p l u s p l u s  
 }  
 # e n d i f  
  
 # e n d i f   / *   _ L I B I C O N V _ H   * /  
