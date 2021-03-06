/ * 
   *   F i l e n a m e :   c f _ t e s t c a s e . c 
   * 
   *     C o p y r i g h t   ��  2 0 0 7 - 2 0 1 4   U n i t e d   S t a t e s   G o v e r n m e n t   a s   r e p r e s e n t e d   b y   t h e   
   *     A d m i n i s t r a t o r   o f   t h e   N a t i o n a l   A e r o n a u t i c s   a n d   S p a c e   A d m i n i s t r a t i o n .   
   *     A l l   O t h e r   R i g h t s   R e s e r v e d .     
   * 
   *     T h i s   s o f t w a r e   w a s   c r e a t e d   a t   N A S A ' s   G o d d a r d   S p a c e   F l i g h t   C e n t e r . 
   *     T h i s   s o f t w a r e   i s   g o v e r n e d   b y   t h e   N A S A   O p e n   S o u r c e   A g r e e m e n t   a n d   m a y   b e   
   *     u s e d ,   d i s t r i b u t e d   a n d   m o d i f i e d   o n l y   p u r s u a n t   t o   t h e   t e r m s   o f   t h a t   
   *     a g r e e m e n t .   
   * 
   *   P u r p o s e :   T h i s   f i l e   c o n t a i n s   a   u n i t   t e s t   c a s e   f o r   t h e   c f   a p p l i c a t i o n   
   *   
   * / 
 
 / * 
   *   I n c l u d e s 
   * / 
 
 
 # i n c l u d e   " c f e . h " 
 # i n c l u d e   " u t a s s e r t . h " 
 # i n c l u d e   " u t t e s t . h " 
 # i n c l u d e   " u t l i s t . h " 
 # i n c l u d e   " c f _ a p p . h " 
 # i n c l u d e   " c f _ e v e n t s . h " 
 # i n c l u d e   " c f _ m s g i d s . h " 
 
 # i n c l u d e   " u t _ c f e _ t b l _ s t u b s . h " 
 # i n c l u d e   " u t _ c f e _ t b l _ h o o k s . h " 
 # i n c l u d e   " u t _ c f e _ e v s _ s t u b s . h " 
 # i n c l u d e   " u t _ c f e _ e v s _ h o o k s . h " 
 # i n c l u d e   " u t _ c f e _ s b _ s t u b s . h " 
 # i n c l u d e   " u t _ c f e _ s b _ h o o k s . h " 
 # i n c l u d e   " u t _ c f e _ e s _ s t u b s . h " 
 # i n c l u d e   " u t _ o s a p i _ s t u b s . h " 
 # i n c l u d e   " u t _ o s f i l e a p i _ s t u b s . h " 
 # i n c l u d e   " u t _ c f e _ f s _ s t u b s . h " 
 # i n c l u d e   " c f d p _ p r o v i d e s . h " / *   f o r   c f d p _ c y c l e _ e a c h _ t r a n s a c t i o n   * / 
 
 # i n c l u d e   " c f _ p l a y b a c k . h " 
 # i n c l u d e   " c f _ c a l l b a c k s . h " 
 
 
 v o i d   C F _ S e t u p ( v o i d ) ; 
 v o i d   C F _ T e a r d o w n ( v o i d ) ; 
 
 / *   p r e v e n t s   t h e   f o l l o w i n g   c o m p i l e r   e r r o r   w h e n   m i s c . h   i s   # i n c l u d e d   * / 
 / *   . . / s r c / P R I / m i s c . h : 7 0 :   e r r o r :   e x p e c t e d    )    b e f o r e    *    t o k e n           * / 
 v o i d   m i s c _ _ s e t _ t r a n s _ s e q _ n u m   ( u _ i n t _ 4   v a l u e ) ; 
 
 
 e x t e r n   c f _ c o n f i g _ t a b l e _ t                                 C F _ C o n f i g T a b l e ; 
 e x t e r n   U t _ C F E _ F S _ R e t u r n C o d e T a b l e _ t             U t _ C F E _ F S _ R e t u r n C o d e T a b l e [ U T _ C F E _ F S _ M A X _ I N D E X ] ; 
 e x t e r n   U t _ O S F I L E A P I _ H o o k T a b l e _ t                   U t _ O S F I L E A P I _ H o o k T a b l e ; 
 e x t e r n   U t _ C F E _ E S _ H o o k T a b l e _ t                         U t _ C F E _ E S _ H o o k T a b l e ; 
 e x t e r n   U t _ O S A P I _ H o o k T a b l e _ t                           U t _ O S A P I _ H o o k T a b l e ; 
 
 
 t y p e d e f   s t r u c t 
 { 
     u i n t 8                   C H d r [ C F E _ S B _ C M D _ H D R _ S I Z E ] ; / / C C S D S   H e a d e r 
     C F _ P D U _ H d r _ t     P H d r ; / / P D U   H d r 
     u i n t 8                   D a t a [ 2 0 0 ] ; 
 } C F _ P D U _ M s g _ t ; 
 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *     C F   C u s t o m   H o o k   F u n c t i o n s   n e e d e d   f o r   u n i t   t e s t 
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 u i n t 3 2   C F E _ E S _ G e t P o o l B u f H o o k C a l l C n t ; 
 i n t 3 2   C F E _ E S _ G e t P o o l B u f H o o k ( u i n t 3 2   * * B u f P t r ,   C F E _ E S _ M e m H a n d l e _ t   H a n d l e P t r ,   u i n t 3 2   S i z e ) ; 
 i n t 3 2   C F E _ E S _ G e t P o o l B u f H o o k ( u i n t 3 2   * * B u f P t r ,   C F E _ E S _ M e m H a n d l e _ t   H a n d l e P t r ,   u i n t 3 2   S i z e ) 
 { 
         u i n t 3 2     O f f s e t ; 
         u i n t 8       * B y t e P t r ; 
         
         O f f s e t   =   ( C F E _ E S _ G e t P o o l B u f H o o k C a l l C n t   *   s i z e o f   ( C F _ Q u e u e E n t r y _ t ) ) ; 
         
         B y t e P t r   =   ( u i n t 8   * ) & C F _ A p p D a t a . M e m . P a r t i t i o n ; 
     
         B y t e P t r   + =   O f f s e t ; 
         
         * B u f P t r   =   ( u i n t 3 2   * ) B y t e P t r ; 
 
         C F E _ E S _ G e t P o o l B u f H o o k C a l l C n t + + ; 
 
         r e t u r n   S i z e ; 
 } 
 
 
 i n t 3 2   O S _ s t a t H o o k ( c o n s t   c h a r   * p a t h ,   o s _ f s t a t _ t   * f i l e s t a t s ) ; 
 i n t 3 2   O S _ s t a t H o o k ( c o n s t   c h a r   * p a t h ,   o s _ f s t a t _ t   * f i l e s t a t s ) 
 { 
 
         f i l e s t a t s - > s t _ s i z e   =   1 2 3 ; 
         
         r e t u r n   O S _ F S _ S U C C E S S ; 
 } 
 
 i n t 3 2   O S _ F D G e t I n f o H o o k   ( i n t 3 2   f i l e d e s ,   O S _ F D T a b l e E n t r y   * f d _ p r o p ) ; 
 i n t 3 2   O S _ F D G e t I n f o H o o k   ( i n t 3 2   f i l e d e s ,   O S _ F D T a b l e E n t r y   * f d _ p r o p ) 
 { 
         
         s t r c p y ( f d _ p r o p - > P a t h , " / c f / t e s t f i l e . t x t " ) ; 
 
         r e t u r n   O S _ F S _ S U C C E S S ; 
 } 
 
 
 s t a t i c   u i n t 3 2   R e a d d i r H o o k C a l l C n t ; 
 o s _ d i r e n t _ t       R e a d d i r H o o k D i r E n t r y ; 
 
 o s _ d i r e n t _ t   *     O S _ r e a d d i r H o o k   ( o s _ d i r p _ t   d i r e c t o r y ) ; 
 o s _ d i r e n t _ t   *     O S _ r e a d d i r H o o k   ( o s _ d i r p _ t   d i r e c t o r y ) 
 { 
               
         R e a d d i r H o o k C a l l C n t + + ; 
         
         i f ( R e a d d i r H o o k C a l l C n t   = =   1 ) { 
         
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e , " . " ) ; 
         
         } e l s e   i f ( R e a d d i r H o o k C a l l C n t   = =   2 ) { 
         
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e , " . . " ) ; 
         
         } e l s e   i f ( R e a d d i r H o o k C a l l C n t   = =   3 ) { 
             
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e , " f i l e n a m e 1 . t x t " ) ; 
             R e a d d i r H o o k D i r E n t r y . d _ t y p e   =   O S _ D T _ D I R ; 
         
         } e l s e   i f ( R e a d d i r H o o k C a l l C n t   = =   4 ) { 
             
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e , " T h i s F i l e n a m e I s T o o L o n g I t E x c e e d s 6 4 T h i s F i l e n a m e I s T o o L o n g I t I s 6 5 c h a r a s " ) ; 
             R e a d d i r H o o k D i r E n t r y . d _ t y p e   =   5 ; 
         
         } e l s e   i f ( R e a d d i r H o o k C a l l C n t   = =   5 ) { 
             
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e ,   " T h i s F i l e n a m e I s T o o L o n g W h e n T h e P a t h I s A t t a c h e d T o I t . I t I s 6 3 C h a r a c t e r s " ) ; 
                                                                                     
         } e l s e   i f ( R e a d d i r H o o k C a l l C n t   = =   6 ) { 
             
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e , " t e s t f i l e . t x t " ) ; 
         
         } e l s e   i f ( R e a d d i r H o o k C a l l C n t   = =   7 ) { 
             
             s t r c p y ( R e a d d i r H o o k D i r E n t r y . d _ n a m e , " f i l e n a m e 5 . t x t " ) ; 
         
         } e l s e { 
             r e t u r n   N U L L ; 
         } 
         
         r e t u r n   & R e a d d i r H o o k D i r E n t r y ; 
 } 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *     C F   T e s t   U t i l i t i e s   n e e d e d   f o r   u n i t   t e s t 
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 i n t 3 2   C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( c h a r   * O r d e r G i v e n ) ; 
 i n t 3 2   C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( c h a r   * O r d e r G i v e n ) 
 { 
         C F _ Q u e u e E n t r y _ t       * P t r T o E n t r y ; 
         c h a r                             B u f [ 6 4 ] ; 
         u i n t 3 2                         i = 0 ; 
 
         P t r T o E n t r y   =   C F _ A p p D a t a . C h a n [ 0 ] . P b Q [ C F _ P B _ P E N D I N G Q ] . H e a d P t r ;                         
         w h i l e ( P t r T o E n t r y   ! =   N U L L ) 
         {                                 
                 s p r i n t f ( & B u f [ i ] , " % d " , ( i n t ) P t r T o E n t r y - > T r a n s N u m ) ; 
                 P t r T o E n t r y   =   P t r T o E n t r y - > N e x t ; 
                 i + + ; 
         } 
         
         i f ( s t r n c m p ( O r d e r G i v e n , B u f , 6 4 ) = = 0 ) 
                 r e t u r n   C F _ S U C C E S S ; 
         e l s e { 
             p r i n t f ( " V e r f i y L i s t   i s   c o m p a r i n g   g i v e n   % s   w i t h   % s \ n " , O r d e r G i v e n , B u f ) ; 
             r e t u r n   C F _ E R R O R ; 
         } 
 } 
 
 
 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( v o i d ) ; 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( v o i d ) 
 { 
     C F _ P l a y b a c k F i l e C m d _ t             P b F i l e C m d M s g ; 
 
         / *   r e s e t   C F   g l o b a l s   e t c   * / 
     C F _ A p p I n i t ( ) ;     
     
     / *   r e s e t   t h e   t r a n s a c t i o n s   s e q   n u m b e r   u s e d   b y   t h e   e n g i n e   * / 
     m i s c _ _ s e t _ t r a n s _ s e q _ n u m ( 1 ) ; 
     
     / *   E x e c u t e   a   p l a y b a c k   f i l e   c o m m a n d   s o   t h a t   o n e   q u e u e   e n t r y   i s   a d d e d   t o   t h e   p e n d i n g   q u e u e   * / 
     C F E _ S B _ I n i t M s g ( & P b F i l e C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   0 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 2 . 2 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   e x e c u t e   t h e   p l a y b a c k   f i l e   c m d   t o   g e t   a   q u e u e   e n t r y   o n   t h e   p e n d i n g   q u e u e   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
     
 } / *   C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y   * / 
 
 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( v o i d ) ; 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( v o i d ) 
 { 
     
     C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( ) ; 
     
     / *   F o r c e   O S _ s t a t   t o   r e t u r n   a   v a l i d   s i z e   a n d   s u c c e s s   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ S T A T _ I N D E X ,   & O S _ s t a t H o o k ) ; 
     
     C F _ S t a r t N e x t F i l e ( 0 ) ; 
     
 } / *   C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y   * / 
 
 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e P b H i s t o r y Q u e u e E n t r y ( v o i d ) ; 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e P b H i s t o r y Q u e u e E n t r y ( v o i d ) 
 { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ A B A N D O N _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " A l l " ) ; 
 
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
     
     c f d p _ c y c l e _ e a c h _ t r a n s a c t i o n ( ) ; 
     
     / *   S e n d   A b a n d o n   C m d   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
         
     c f d p _ c y c l e _ e a c h _ t r a n s a c t i o n ( ) ; 
     
     c f d p _ c y c l e _ e a c h _ t r a n s a c t i o n ( ) ; 
 
     
 } / *   C F _ T s t U t i l _ C r e a t e O n e P b H i s t o r y Q u e u e E n t r y   * / 
 
 
 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e U p A c t i v e Q u e u e E n t r y ( v o i d ) ; 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e U p A c t i v e Q u e u e E n t r y ( v o i d ) 
 { 
     I N D I C A T I O N _ T Y P E   I n d T y p e   =   I N D _ M A C H I N E _ A L L O C A T E D ; 
     T R A N S _ S T A T U S   T r a n s I n f o ; 
     
         / *   r e s e t   C F   g l o b a l s   e t c   * / 
     C F _ A p p I n i t ( ) ;     
     
     T r a n s I n f o . r o l e   =     C L A S S _ 1 _ R E C E I V E R ; 
     T r a n s I n f o . t r a n s . n u m b e r   =   5 0 0 ; 
     T r a n s I n f o . t r a n s . s o u r c e _ i d . v a l u e [ 0 ]   =   0 ; 
     T r a n s I n f o . t r a n s . s o u r c e _ i d . v a l u e [ 1 ]   =   2 3 ; 
     
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     C F _ I n d i c a t i o n   ( I n d T y p e , T r a n s I n f o ) ; 
     
 } / *   C F _ T s t U t i l _ C r e a t e O n e U p A c t i v e Q u e u e E n t r y   * / 
 
 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e U p H i s t o r y Q u e u e E n t r y ( v o i d ) ; 
 v o i d   C F _ T s t U t i l _ C r e a t e O n e U p H i s t o r y Q u e u e E n t r y ( v o i d ) 
 { 
     I N D I C A T I O N _ T Y P E   I n d T y p e   =   I N D _ M A C H I N E _ D E A L L O C A T E D ; 
     T R A N S _ S T A T U S   T r a n s I n f o ; 
     
     C F _ T s t U t i l _ C r e a t e O n e U p A c t i v e Q u e u e E n t r y ( ) ; 
     
     T r a n s I n f o . r o l e   =     C L A S S _ 1 _ R E C E I V E R ; 
     T r a n s I n f o . t r a n s . n u m b e r   =   5 0 0 ; 
     T r a n s I n f o . t r a n s . s o u r c e _ i d . v a l u e [ 0 ]   =   0 ; 
     T r a n s I n f o . t r a n s . s o u r c e _ i d . v a l u e [ 1 ]   =   2 3 ; 
     T r a n s I n f o . f i n a l _ s t a t u s   =   F I N A L _ S T A T U S _ S U C C E S S F U L ;     
     s t r c p y ( T r a n s I n f o . m d . d e s t _ f i l e _ n a m e , " / r a m / u p l o a d e d f i l e . t x t " ) ; 
     
     C F _ I n d i c a t i o n   ( I n d T y p e , T r a n s I n f o ) ; 
     
 } / *   C F _ T s t U t i l _ C r e a t e O n e U p H i s t o r y Q u e u e E n t r y   * / 
 
 
 v o i d   C F _ R e s e t E n g i n e ( v o i d ) ; 
 v o i d   C F _ R e s e t E n g i n e ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ A B A N D O N _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " A l l " ) ; 
 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     c f d p _ c y c l e _ e a c h _ t r a n s a c t i o n ( ) ; 
     
     m i s c _ _ s e t _ t r a n s _ s e q _ n u m   ( 1 ) ; 
     
 } 
 
 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *     C F   A p p I n i t   T e s t s 
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ E V S R e g F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ E V S _ S e t R e t u r n C o d e ( U T _ C F E _ E V S _ R E G I S T E R _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
     U t P r i n t f ( " \ n " ) ; / *   n e e d e d   b e c a u s e   s y s   l o g   m s g   h a s   n o   E O L   * / 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ N o E v e n t S e n t ( " V e r i f y i n g   N o   E v e n t   w a s   s e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ E V S R e g F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ C r P i p e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ S B _ S e t R e t u r n C o d e ( U T _ C F E _ S B _ C R E A T E P I P E _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C R _ P I P E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ C r P i p e F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ S u b 1 F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ S B _ S e t R e t u r n C o d e ( U T _ C F E _ S B _ S U B S C R I B E _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S U B _ R E Q _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ S u b 1 F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ S u b 2 F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ S B _ S e t R e t u r n C o d e ( U T _ C F E _ S B _ S U B S C R I B E _ I N D E X ,   - 1 ,   2 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S U B _ C M D _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ S u b 2 F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ S u b 3 F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ S B _ S e t R e t u r n C o d e ( U T _ C F E _ S B _ S U B S C R I B E _ I N D E X ,   - 1 ,   3 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S U B _ W A K E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ S u b 3 F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ S u b 4 F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ S B _ S e t R e t u r n C o d e ( U T _ C F E _ S B _ S U B S C R I B E _ I N D E X ,   - 1 ,   4 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S U B _ P D U S _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ S u b 4 F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ T b l R e g F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ R E G I S T E R _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F G T B L _ R E G _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ T b l R e g F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ T b l L o a d F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ L O A D _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F G T B L _ L D _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ T b l L o a d F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ T b l M a n a g e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ M A N A G E _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F G T B L _ M N G _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ T b l M a n a g e F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ T b l G e t A d r F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ G E T A D D R E S S _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F G T B L _ G A D R _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ T b l G e t A d r F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ P o o l C r e a t e E x F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P O O L C R E A T E E X _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 0 , " E v e n t   C o u n t   =   0 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ N o E v e n t S e n t ( " V e r i f y i n g   N o   E v e n t   w a s   s e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ P o o l C r e a t e E x F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ S e n d E v e n t F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n , N u m E v e n t s E x p ; 
 
     / *   S e t u p   I n p u t s   * / 
     N u m E v e n t s E x p   =   8 ; / *   c f d p   d e b u g   e v e n t s   f o r   s e t   m i b   c a l l s   * / 
     E x p R t n   =   - 1 ; 
     U t _ C F E _ E V S _ S e t R e t u r n C o d e ( U T _ C F E _ E V S _ S E N D E V E N T _ I N D E X ,   - 1 ,   ( N u m E v e n t s E x p   +   1 ) ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 8 , " E v e n t   C o u n t   =   8 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ S e n d E v e n t F a i l   * / 
 
 
 v o i d   T e s t _ C F _ A p p I n i t _ N o E r r o r s ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F E _ S U C C E S S ; 
   
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 9 , " E v e n t   C o u n t   =   9 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p I n i t _ N o E r r o r s   * / 
 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *     C F   T a b l e   V a l i d a t i o n   T e s t s 
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 v o i d   T e s t _ C F _ T b l V a l F l i g h t E n t i t y I d F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     c h a r     V a l i d F l i g h t E n t i t y I d [ C F _ M A X _ C F G _ V A L U E _ C H A R S ] ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     / *   s t o r e   t h e   v a l i d   f l i g h t   e n t i t y   I D   t o   b e   r e s t o r e d   l a t e r   * / 
     s t r n c p y ( V a l i d F l i g h t E n t i t y I d , C F _ A p p D a t a . T b l - > F l i g h t E n t i t y I d , C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     s t r n c p y ( C F _ A p p D a t a . T b l - > F l i g h t E n t i t y I d , " 1 2 3 4 5 6 7 8 9 0 " ,   C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     s t r n c p y ( C F _ A p p D a t a . T b l - > F l i g h t E n t i t y I d , V a l i d F l i g h t E n t i t y I d , C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l F l i g h t E n t i t y I d F a i l   * / 
 
 
 v o i d   T e s t _ C F _ T b l V a l I n c o m i n g M s g I d F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     C F E _ S B _ M s g I d _ t   V a l i d I n c o m i n g P D U M s g I d ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d I n c o m i n g P D U M s g I d   =   C F _ A p p D a t a . T b l - > I n C h [ 0 ] . I n c o m i n g P D U M s g I d ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > I n C h [ 0 ] . I n c o m i n g P D U M s g I d   =   0 x F F F F ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > I n C h [ 0 ] . I n c o m i n g P D U M s g I d   =   V a l i d I n c o m i n g P D U M s g I d ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l I n c o m i n g M s g I d F a i l   * / 
 
 
 v o i d   T e s t _ C F _ T b l V a l O u t g o i n g F i l e C h u n k F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     c h a r     V a l i d O u t g o i n g F i l e C h u n k S i z e [ C F _ M A X _ C F G _ V A L U E _ C H A R S ] ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     / *   s t o r e   t h e   v a l i d   f l i g h t   e n t i t y   I D   t o   b e   r e s t o r e d   l a t e r   * / 
     s t r n c p y ( V a l i d O u t g o i n g F i l e C h u n k S i z e , C F _ A p p D a t a . T b l - > O u t g o i n g F i l e C h u n k S i z e , C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u t g o i n g F i l e C h u n k S i z e , " 1 2 3 4 5 6 7 8 9 0 " ,   C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u t g o i n g F i l e C h u n k S i z e , V a l i d O u t g o i n g F i l e C h u n k S i z e , C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l O u t g o i n g F i l e C h u n k F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l C h a n I n U s e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     u i n t 3 2   V a l i d C h a n I n U s e ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d C h a n I n U s e   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . E n t r y I n U s e ; 
     
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . E n t r y I n U s e   =   3 ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . E n t r y I n U s e   =   V a l i d C h a n I n U s e ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l C h a n I n U s e F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l D e q u e E n a b l e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     u i n t 3 2   V a l i d D e q u e u e E n a b l e ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d D e q u e u e E n a b l e   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . D e q u e u e E n a b l e ; 
     
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . D e q u e u e E n a b l e   =   2 ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 5 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . D e q u e u e E n a b l e   =   V a l i d D e q u e u e E n a b l e ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l D e q u e E n a b l e F a i l   * / 
 
 
 v o i d   T e s t _ C F _ T b l V a l O u t g o i n g M s g I d F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     C F E _ S B _ M s g I d _ t   V a l i d O u t g o i n g P D U M s g I d ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d O u t g o i n g P D U M s g I d   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . O u t g o i n g P d u M s g I d ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . O u t g o i n g P d u M s g I d   =   0 x F F F F ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 7 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . O u t g o i n g P d u M s g I d   =   V a l i d O u t g o i n g P D U M s g I d ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l O u t g o i n g M s g I d F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l P o l l D i r I n U s e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     u i n t 3 2   V a l i d P o l l D i r I n U s e ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d P o l l D i r I n U s e   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n t r y I n U s e ; 
     
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n t r y I n U s e   =   3 ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 8 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n t r y I n U s e   =   V a l i d P o l l D i r I n U s e ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P o l l D i r I n U s e F a i l   * / 
 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l P o l l E n a b l e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     u i n t 3 2   V a l i d P o l l E n a b l e ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d P o l l E n a b l e   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n a b l e S t a t e ; 
     
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n a b l e S t a t e   =   2 ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 9 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n a b l e S t a t e   =   V a l i d P o l l E n a b l e ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P o l l E n a b l e F a i l   * / 
 
 
 v o i d   T e s t _ C F _ T b l V a l P o l l C l a s s F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     u i n t 3 2   V a l i d P o l l C l a s s ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d P o l l C l a s s   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . C l a s s ; 
     
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . C l a s s   =   0 ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 0 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . C l a s s   =   V a l i d P o l l C l a s s ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P o l l C l a s s F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l P o l l P r e s e r v e F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     u i n t 3 2   V a l i d P o l l P r e s e r v e ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     
     / *   s t o r e   t h e   v a l i d   p a r a m e t e r   t o   b e   r e s t o r e d   l a t e r   * / 
     V a l i d P o l l P r e s e r v e   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . P r e s e r v e ; 
     
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . P r e s e r v e   =   3 ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . P r e s e r v e   =   V a l i d P o l l P r e s e r v e ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P o l l P r e s e r v e F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l P o l l S r c P a t h F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     c h a r     V a l i d S r c P a t h [ O S _ M A X _ P A T H _ L E N ] ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     / *   s t o r e   t h e   v a l i d   f l i g h t   e n t i t y   I D   t o   b e   r e s t o r e d   l a t e r   * / 
     s t r n c p y ( V a l i d S r c P a t h , C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . S r c P a t h , O S _ M A X _ P A T H _ L E N ) ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . S r c P a t h , " / N o S l a s h A t E n d " ,   O S _ M A X _ P A T H _ L E N ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . S r c P a t h , V a l i d S r c P a t h , O S _ M A X _ P A T H _ L E N ) ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P o l l S r c P a t h F a i l   * / 
 
 
 v o i d   T e s t _ C F _ T b l V a l P o l l D s t P a t h F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     c h a r     V a l i d D s t P a t h [ O S _ M A X _ P A T H _ L E N ] ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     / *   s t o r e   t h e   v a l i d   f l i g h t   e n t i t y   I D   t o   b e   r e s t o r e d   l a t e r   * / 
     s t r n c p y ( V a l i d D s t P a t h , C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . D s t P a t h , O S _ M A X _ P A T H _ L E N ) ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . D s t P a t h , " / N o   S p a c e s   A l l o w e d " ,   O S _ M A X _ P A T H _ L E N ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . D s t P a t h , V a l i d D s t P a t h , O S _ M A X _ P A T H _ L E N ) ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P o l l D s t P a t h F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ T b l V a l P e e r E n t i t y I d F a i l ( v o i d ) { 
 
     i n t 3 2   E x p R t n , A c t R t n ; 
     c h a r     V a l i d P e e r E n t i t y I d [ C F _ M A X _ C F G _ V A L U E _ C H A R S ] ; 
 
     / *   S e t u p   I n p u t s   * / 
     E x p R t n   =   C F _ E R R O R ; 
     C F _ A p p I n i t ( ) ; 
     / *   s t o r e   t h e   v a l i d   f l i g h t   e n t i t y   I D   t o   b e   r e s t o r e d   l a t e r   * / 
     s t r n c p y ( V a l i d P e e r E n t i t y I d , C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . P e e r E n t i t y I d , C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     / *   C o r r u p t   t a b l e   p a r a m   * /     
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . P e e r E n t i t y I d , " 1 2 " ,   C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     A c t R t n   =   C F _ V a l i d a t e C F C o n f i g T a b l e   ( C F _ A p p D a t a . T b l ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 6 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ V A L _ E R R 1 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     / *   r e s t o r e   t b l   p a r a m   * / 
     s t r n c p y ( C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . P e e r E n t i t y I d , V a l i d P e e r E n t i t y I d , C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     
 } / *   e n d   T e s t _ C F _ T b l V a l P e e r E n t i t y I d F a i l   * / 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *     C F   A p p M a i n   T e s t s 
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 
 v o i d   T e s t _ C F _ A p p M a i n _ I n i t E r r o r s ( v o i d ) { 
 
     / *   S e t u p   I n p u t s   * / 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ G E T A D D R E S S _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     C F _ A p p M a i n ( ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F G T B L _ G A D R _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p M a i n _ I n i t E r r o r s   * / 
 
 
 v o i d   T e s t _ C F _ A p p M a i n _ S e m G e t I d B y N a m e F a i l ( v o i d ) { 
 
     / *   S e t u p   I n p u t s   * / 
     U t _ O S A P I _ S e t R e t u r n C o d e ( U T _ O S A P I _ C O U N T S E M G E T I D B Y N A M E _ I N D E X ,   - 1 ,   1 ) ; 
     / / T h e   l i n e   b e l o w   i s   n e e d e d   t o   s t o p   e n t r y   i n t o   w h i l e   l o o p 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ G E T A D D R E S S _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     C F _ A p p M a i n ( ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F G T B L _ G A D R _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ H A N D S H A K E _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ A p p M a i n _ S e m G e t I d B y N a m e F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ A p p M a i n _ R c v M s g E r r ( v o i d ) { 
 
     / *   S e t u p   I n p u t s   * / 
     U t _ C F E _ S B _ S e t R e t u r n C o d e ( U T _ C F E _ S B _ R C V M S G _ I N D E X ,   - 1 ,   1 ) ; 
   
     / *   E x e c u t e   T e s t   * / 
     C F _ A p p M a i n ( ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ E v e n t S e n t ( C F _ R C V _ M S G _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
         
 } / *   e n d   T e s t _ C F _ A p p M a i n _ R c v M s g E r r   * / 
 
 
 / *   T O _ A p p M a i n _ T e s t 0 2   -   S u c c e s s f u l   ( P r o c e s s   C o m m a n d )   * / 
 v o i d   T e s t _ C F _ A p p M a i n _ R c v M s g O k O n F i r s t ( v o i d ) 
 { 
         C F _ N o A r g s C m d _ t         N o o p C m d ; 
         i n t 3 2                       C m d P i p e ; 
 
         / *   S e t u p   I n p u t s   * / 
         U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ R U N L O O P _ I N D E X ,   F A L S E ,   2 ) ; 
 
         C m d P i p e   =   U t _ C F E _ S B _ C r e a t e P i p e ( " C F _ C M D _ P I P E " ) ; 
         C F E _ S B _ I n i t M s g ( & N o o p C m d ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
         C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d ,   C F _ N O O P _ C C ) ; 
         U t _ C F E _ S B _ A d d M s g T o P i p e ( & N o o p C m d ,   C m d P i p e ) ; 
 
         / *   E x e c u t e   T e s t   * / 
         C F _ A p p M a i n ( ) ; 
 
         / *   V e r i f y   O u t p u t s   * / 
         U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
         U t A s s e r t _ E v e n t S e n t ( C F _ N O O P _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
         U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
         
 
 } / *   e n d   T e s t _ C F _ A p p M a i n _ R c v M s g O k O n F i r s t   * / 
 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *     C F   C o m m a n d   T e s t s 
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 
 v o i d   T e s t _ C F _ H o u s e k e e p i n g C m d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     H o u s e k e e p i n g C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & H o u s e k e e p i n g C m d M s g ,   C F _ S E N D _ H K _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 0 , " E v e n t   C o u n t   =   0 " ) ; 
     U t A s s e r t _ P a c k e t S e n t ( C F _ H K _ T L M _ M I D ,   " H o u s e k e e p i n g   P a c k e t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ H o u s e k e e p i n g C m d   * / 
 
 
 v o i d   T e s t _ C F _ H k C m d I n v L e n ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     H o u s e k e e p i n g C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & H o u s e k e e p i n g C m d M s g ,   C F _ S E N D _ H K _ M I D ,   8 8 ,   F A L S E ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ H k C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ H k C m d T b l U p d a t e d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     H o u s e k e e p i n g C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & H o u s e k e e p i n g C m d M s g ,   C F _ S E N D _ H K _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
     / / T h e   l i n e   b e l o w   i s   n e e d e d   t o   g e t   c o v e r a g e   i n   C F _ C h e c k F o r T b l R e q u e s t s 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ G E T S T A T U S _ I N D E X ,   C F E _ T B L _ I N F O _ U P D A T E _ P E N D I N G ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * /     
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T B L _ L D _ A T T E M P T _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ H k C m d T b l U p d a t e d   * / 
 
 
 v o i d   T e s t _ C F _ H k C m d V a l P e n d i n g ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     H o u s e k e e p i n g C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & H o u s e k e e p i n g C m d M s g ,   C F _ S E N D _ H K _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
         / / T h e   l i n e   b e l o w   i s   n e e d e d   t o   g e t   c o v e r a g e   i n   C F _ C h e c k F o r T b l R e q u e s t s 
     U t _ C F E _ T B L _ S e t R e t u r n C o d e ( U T _ C F E _ T B L _ G E T S T A T U S _ I N D E X ,   C F E _ T B L _ I N F O _ V A L I D A T I O N _ P E N D I N G ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & H o u s e k e e p i n g C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 0 , " E v e n t   C o u n t   =   0 " ) ; 
 
 } / *   e n d   T e s t _ C F _ H k C m d V a l P e n d i n g   * / 
 
 
 v o i d   T e s t _ C F _ N o o p C m d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     N o o p C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & N o o p C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d M s g ,   C F _ N O O P _ C C ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O O P _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ N o o p C m d   * / 
 
 
 v o i d   T e s t _ C F _ N o o p C m d I n v L e n ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     N o o p C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & N o o p C m d M s g ,   C F _ C M D _ M I D ,   1 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d M s g ,   C F _ N O O P _ C C ) ;   
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & N o o p C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ N o o p C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ W a k e u p C m d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     W a k e u p C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & W a k e u p C m d M s g ,   C F _ W A K E _ U P _ R E Q _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ;     
     / /   n e e d   t o   c a l l   a t   l e a s t   4   t i m e s   t o   g e t   c o v e r a g e   o f   
     / /   c h e c k i n g   p o l l i n g   d i r e c t o r i e s   a n d   p e n d i n g   q u e u e s 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     / / f i x m e   -   a d d   t e s t   t o   v e r i f y   w a k e u p   c t r   i n c r e m e n t s 
 
 } / *   e n d   T e s t _ C F _ W a k e u p C m d   * / 
 
 
 v o i d   T e s t _ C F _ W a k e u p C m d P o l l i n g E n a b l e d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     W a k e u p C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & W a k e u p C m d M s g ,   C F _ W A K E _ U P _ R E Q _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
 
     / *   f o r c e   p o l l i n g   t o   b e   e n a b l e d   t o   g e t   m o r e   c o v e r a g e   i n   p l a y b a c k . c   * / 
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n a b l e S t a t e   =   C F _ E N A B L E D ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ;     
     / /   n e e d   t o   c a l l   a t   l e a s t   4   t i m e s   t o   g e t   c o v e r a g e   o f   
     / /   c h e c k i n g   p o l l i n g   d i r e c t o r i e s   a n d   p e n d i n g   q u e u e s 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ O P E N _ D I R _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     / / f i x m e   -   a d d   t e s t   t o   v e r i f y   w a k e u p   c t r   i n c r e m e n t s 
     
     C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P o l l D i r [ 0 ] . E n a b l e S t a t e   =   C F _ D I S A B L E D ; 
 
 } / *   e n d   T e s t _ C F _ W a k e u p C m d P o l l i n g E n a b l e d   * / 
 
 
 v o i d   T e s t _ C F _ W a k e u p C m d I n v L e n ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     W a k e u p C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & W a k e u p C m d M s g ,   C F _ W A K E _ U P _ R E Q _ C M D _ M I D ,   5 ,   T R U E ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W a k e u p C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ W a k e u p C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ I n P D U N o E r r C m d ( v o i d ) { 
 
     / *   I n c o m i n g   P D U   t e s t s   c a n n o t   g e t   a   n o   e r r o r s   c o n d i t i o n   b e c a u s e   t h e   e n g i n e   s t a t e s 
 s i z e   m u s t   b e   > = 5 2 4   a n d   C F   s t a t e s   s i z e   m u s t   b e   l e s s   t h a n   5 1 2 .   M u s t   b e   s o m e   p d u   
 h e a d e r   f i e l d   w r o n g l y   s e t .   S t i l l   g e t   1 0 0 %   c o v e r a g e   o n   C F _ S e n d P D U T o E n g i n e   * /   
     
     C F _ P D U _ M s g _ t         I n c o m i n g P d u M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & I n c o m i n g P d u M s g ,   C F _ I N C O M I N G _ P D U _ M I D ,   s i z e o f ( C F _ P D U _ M s g _ t ) ,   T R U E ) ; 
     I n c o m i n g P d u M s g . P H d r . O c t e t 1   =   0 x 0 4 ; / / f i l e   d i r e c t i v e , t o w a r d   r c v r , c l a s s 1 , c r c   n o t   p r e s e n t 
     I n c o m i n g P d u M s g . P H d r . P D a t a L e n   =   0 x C 8 ; / / p d u   d a t a   f i e l d   s i z e 
     I n c o m i n g P d u M s g . P H d r . O c t e t 4   =   0 x 1 3 ; / /   h e x   1   -   l e n g t h   e n t i t y I D   i s   2 ,   h e x   3   -   l e n g t h   x a c t   s e q   i s   4 
     I n c o m i n g P d u M s g . P H d r . S r c E n t i t y I d   =   0 x 0 0 1 7 ; / / 0 . 2 3 
     I n c o m i n g P d u M s g . P H d r . T r a n s S e q N u m   =   1 ; 
     I n c o m i n g P d u M s g . P H d r . D s t E n t i t y I d   =   0 x 0 0 1 8 ; / / 0 . 2 4 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ) ; 
 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P D U _ R C V _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ I n P D U N o E r r C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ I n P D U T l m P k t C m d ( v o i d ) { 
 
     C F _ P D U _ M s g _ t         I n c o m i n g P d u M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & I n c o m i n g P d u M s g ,   0 x 0 8 F D ,   s i z e o f ( C F _ P D U _ M s g _ t ) ,   T R U E ) ; 
     I n c o m i n g P d u M s g . P H d r . O c t e t 1   =   0 x 0 4 ; / / f i l e   d i r e c t i v e , t o w a r d   r c v r , c l a s s 1 , c r c   n o t   p r e s e n t 
     I n c o m i n g P d u M s g . P H d r . P D a t a L e n   =   0 x C 8 ; / / p d u   d a t a   f i e l d   s i z e 
     I n c o m i n g P d u M s g . P H d r . O c t e t 4   =   0 x 1 3 ; / /   h e x   1   -   l e n g t h   e n t i t y I D   i s   2 ,   h e x   3   -   l e n g t h   x a c t   s e q   i s   4 
     I n c o m i n g P d u M s g . P H d r . S r c E n t i t y I d   =   0 x 0 0 1 7 ; / / 0 . 2 3 
     I n c o m i n g P d u M s g . P H d r . T r a n s S e q N u m   =   1 ; 
     I n c o m i n g P d u M s g . P H d r . D s t E n t i t y I d   =   0 x 0 0 1 8 ; / / 0 . 2 4 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ;       
     C F _ S e n d P D U T o E n g i n e ( ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P D U _ R C V _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ I n P D U T l m P k t C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ I n P D U H d r S i z e E r r C m d ( v o i d ) { 
 
     i n t 3 2                       E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P D U _ M s g _ t         I n c o m i n g P d u M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & I n c o m i n g P d u M s g ,   C F _ I N C O M I N G _ P D U _ M I D ,   s i z e o f ( C F _ P D U _ M s g _ t ) ,   T R U E ) ; 
     I n c o m i n g P d u M s g . P H d r . O c t e t 1   =   0 x 0 4 ; / / f i l e   d i r e c t i v e , t o w a r d   r c v r , c l a s s 1 , c r c   n o t   p r e s e n t 
     I n c o m i n g P d u M s g . P H d r . P D a t a L e n   =   0 x 0 0 ; / / p d u   d a t a   f i e l d 
     I n c o m i n g P d u M s g . P H d r . O c t e t 4   =   0 x 2 4 ; / /   h e x   2   -   l e n g t h   e n t i t y I D   i s   3 ,   h e x   4   -   l e n g t h   x a c t   s e q   i s   5 
     I n c o m i n g P d u M s g . P H d r . S r c E n t i t y I d   =   0 x 0 0 1 7 ; / / 0 . 2 3 
     I n c o m i n g P d u M s g . P H d r . T r a n s S e q N u m   =   1 ; 
     I n c o m i n g P d u M s g . P H d r . D s t E n t i t y I d   =   0 x 0 0 1 8 ; / / 0 . 2 4 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ) ; 
 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P D U _ R C V _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ I n P D U H d r S i z e E r r C m d   * / 
 
 
 
 
 v o i d   T e s t _ C F _ I n P D U T o o B i g C m d ( v o i d ) { 
 
     i n t 3 2                       E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P D U _ M s g _ t         I n c o m i n g P d u M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & I n c o m i n g P d u M s g ,   C F _ I N C O M I N G _ P D U _ M I D ,   s i z e o f ( C F _ P D U _ M s g _ t ) ,   T R U E ) ; 
     I n c o m i n g P d u M s g . P H d r . O c t e t 1   =   0 x 0 4 ; / / f i l e   d i r e c t i v e , t o w a r d   r c v r , c l a s s 1 , c r c   n o t   p r e s e n t 
     I n c o m i n g P d u M s g . P H d r . P D a t a L e n   =   0 x F A 0 ; / / p d u   d a t a   f i e l d   -   4 0 0 0   b y t e s 
     I n c o m i n g P d u M s g . P H d r . O c t e t 4   =   0 x 1 3 ; / / 1   -   l e n g t h   e n t i t y I D   i s   2 ,   3   -   l e n g t h   x a c t   s e q   i s   4 
     I n c o m i n g P d u M s g . P H d r . S r c E n t i t y I d   =   0 x 0 0 1 7 ; / / 0 . 2 3 
     I n c o m i n g P d u M s g . P H d r . T r a n s S e q N u m   =   1 ; 
     I n c o m i n g P d u M s g . P H d r . D s t E n t i t y I d   =   0 x 0 0 1 8 ; / / 0 . 2 4 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ) ; 
 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P D U _ R C V _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ I n P D U T o o B i g C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ I n P D U T o o S m a l l C m d ( v o i d ) { 
 
     i n t 3 2                       E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P D U _ M s g _ t         I n c o m i n g P d u M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & I n c o m i n g P d u M s g ,   C F _ I N C O M I N G _ P D U _ M I D ,   s i z e o f ( C F _ P D U _ M s g _ t ) ,   T R U E ) ; 
     I n c o m i n g P d u M s g . P H d r . O c t e t 1   =   0 x 0 4 ; / / f i l e   d i r e c t i v e , t o w a r d   r c v r , c l a s s 1 , c r c   n o t   p r e s e n t 
     I n c o m i n g P d u M s g . P H d r . P D a t a L e n   =   0 x 0 0 ; / / p d u   d a t a   f i e l d 
     I n c o m i n g P d u M s g . P H d r . O c t e t 4   =   0 x 1 3 ; / / 1   -   l e n g t h   e n t i t y I D   i s   2 ,   3   -   l e n g t h   x a c t   s e q   i s   4 
     I n c o m i n g P d u M s g . P H d r . S r c E n t i t y I d   =   0 x 0 0 1 7 ; / / 0 . 2 3 
     I n c o m i n g P d u M s g . P H d r . T r a n s S e q N u m   =   1 ; 
     I n c o m i n g P d u M s g . P H d r . D s t E n t i t y I d   =   0 x 0 0 1 8 ; / / 0 . 2 4 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & I n c o m i n g P d u M s g ) ; 
 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P D U _ R C V _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     
     
 } / *   e n d   T e s t _ C F _ I n P D U T o o S m a l l C m d   * / 
 
 
 v o i d   T e s t _ C F _ R s t C t r s C m d ( v o i d ) { 
 
     C F _ R e s e t C t r s C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ R e s e t C t r s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ R E S E T _ C C ) ; 
     C m d M s g . V a l u e   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ R E S E T _ C M D _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D E B U G   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ R s t C t r s C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ R s t C t r s C m d I n v L e n ( v o i d ) { 
 
     C F _ R e s e t C t r s C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   8 8 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ R E S E T _ C C ) ; 
     C m d M s g . V a l u e   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ R s t C t r s C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ F r e e z e C m d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ F R E E Z E _ C C ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ F R E E Z E _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ F r e e z e C m d   * / 
 
 
 v o i d   T e s t _ C F _ F r e e z e C m d I n v L e n ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   3 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ F R E E Z E _ C C ) ;   
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ F r e e z e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ T h a w C m d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ T H A W _ C C ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ T H A W _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ T h a w C m d   * / 
 
 
 v o i d   T e s t _ C F _ T h a w C m d I n v L e n ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   1 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ T H A W _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ T h a w C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d T r a n s I d C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 5 6 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d T r a n s I d C m d * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d T r a n s I d C m d I n v L e n ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   6 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 5 6 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d T r a n s I d C m d I n v L e n * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d F i l e n a m e C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / f i l e . t x t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C A R S _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d F i l e n a m e C m d   * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d I n v F i l e n a m e C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f /         f i l e . t x t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d I n v F i l e n a m e C m d   * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d C m d I n v L e n ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   5 5 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / f i l e . t x t " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d U n t e r m S t r g C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ;     
     C F E _ P S P _ M e m S e t     ( C m d M s g . T r a n s , 0 x F F ,   O S _ M A X _ P A T H _ L E N ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d U n t e r m S t r g C m d   * / 
 
 
 v o i d   T e s t _ C F _ S u s p e n d A l l C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " A l l " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C A R S _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S u s p e n d A l l C m d * / 
 
 
 v o i d   T e s t _ C F _ R e s u m e C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ R E S U M E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 5 6 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ R e s u m e C m d   * / 
 
 
 v o i d   T e s t _ C F _ R e s u m e A l l C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ R E S U M E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " A l l " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C A R S _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ R e s u m e A l l C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ C a n c e l C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ C A N C E L _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 5 6 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ C a n c e l C m d   * / 
 
 
 v o i d   T e s t _ C F _ C a n c e l A l l C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ C A N C E L _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " A l l " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C A R S _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ C a n c e l A l l C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ A b a n d o n C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ A B A N D O N _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 5 6 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ A b a n d o n C m d   * / 
 
 
 v o i d   T e s t _ C F _ A b a n d o n A l l C m d ( v o i d ) { 
 
     C F _ C A R S C m d _ t         C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ A B A N D O N _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " A l l " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C A R S _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ A b a n d o n A l l C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ S e t M i b P a r a m C m d ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " s a v e _ i n c o m p l e t e _ f i l e s " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " y e s " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b P a r a m C m d   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b P a r a m C m d I n v L e n ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   8 8 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " s a v e _ i n c o m p l e t e _ f i l e s " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " y e s " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b P a r a m C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d U n t e r m P a r a m ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     C F E _ P S P _ M e m S e t     ( C m d M s g . P a r a m , 0 x F F ,   C F _ M A X _ C F G _ P A R A M _ C H A R S ) ; 
     s t r c p y ( C m d M s g . V a l u e , " y e s " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t \ n " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d U n t e r m P a r a m   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d U n t e r m V a l u e ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     s t r c p y ( C m d M s g . P a r a m , " s a v e _ i n c o m p l e t e _ f i l e s " ) ; 
     C F E _ P S P _ M e m S e t ( C m d M s g . V a l u e , 0 x F F ,   C F _ M A X _ C F G _ V A L U E _ C H A R S ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d U n t e r m V a l u e   * / 
 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d F i l e C h u n k O v e r L i m i t ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     s t r c p y ( C m d M s g . P a r a m , " O U T G O I N G _ F I L E _ C H U N K _ S I Z E " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 1 0 0 0 0 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t \ n " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d F i l e C h u n k O v e r L i m i t   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d M y I d I n v a l i d ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     s t r c p y ( C m d M s g . P a r a m , " M Y _ I D " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 1 0 0 0 0 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t \ n " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d M y I d I n v a l i d   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d A c k L i m i t ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " A C K _ L I M I T " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 2 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d A c k L i m i t   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d A c k T i m e o u t ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " A C K _ T I M E O U T " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 2 5 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d A c k T i m e o u t   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d I n a c t T i m e o u t ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " I N A C T I V I T Y _ T I M E O U T " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 2 0 0 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d I n a c t T i m e o u t   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d N a k L i m i t ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " N A K _ L I M I T " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 4 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d N a k L i m i t   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b C m d N a k T i m e o u t ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " N A K _ T I M E O U T " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 1 5 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b C m d N a k T i m e o u t   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b F i l e C h u n k S i z e ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " O U T G O I N G _ F I L E _ C H U N K _ S I Z E " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 2 5 0 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b F i l e C h u n k S i z e   * / 
 
 
 v o i d   T e s t _ C F _ S e t M i b M y I d ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ M I B _ P A R A M _ C C ) ; 
     
     s t r c p y ( C m d M s g . P a r a m , " M Y _ I D " ) ; 
     s t r c p y ( C m d M s g . V a l u e , " 2 5 . 2 9 " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 , " E v e n t   C o u n t   =   2 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t M i b M y I d   * / 
 
 
 v o i d   T e s t _ C F _ G e t M i b P a r a m C m d ( v o i d ) { 
 
     C F _ G e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ G e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ G E T _ M I B _ P A R A M _ C C ) ; 
     s t r c p y ( C m d M s g . P a r a m , " s a v e _ i n c o m p l e t e _ f i l e s " ) ;     
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ G E T _ M I B _ C M D _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t \ n " ) ; 
 
 } / *   e n d   T e s t _ C F _ G e t M i b P a r a m C m d   * / 
 
 
 v o i d   T e s t _ C F _ G e t M i b P a r a m C m d I n v L e n ( v o i d ) { 
 
     C F _ G e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   0 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ G E T _ M I B _ P A R A M _ C C ) ; 
     s t r c p y ( C m d M s g . P a r a m , " s a v e _ i n c o m p l e t e _ f i l e s " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ G e t M i b P a r a m C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ G e t M i b C m d U n t e r m P a r a m ( v o i d ) { 
 
     C F _ G e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ G e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ G E T _ M I B _ P A R A M _ C C ) ; 
     C F E _ P S P _ M e m S e t     ( C m d M s g . P a r a m , 0 x F F ,   C F _ M A X _ C F G _ P A R A M _ C H A R S ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t \ n " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ G e t M i b C m d U n t e r m P a r a m   * / 
 
 
 v o i d   T e s t _ C F _ G e t M i b P a r a m C m d I n v P a r a m ( v o i d ) { 
 
     C F _ G e t M i b P a r a m _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ G e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ G E T _ M I B _ P A R A M _ C C ) ; 
     s t r c p y ( C m d M s g . P a r a m , " s a v e _ t h e _ b a y " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C F D P _ E N G I N E _ W A R N _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o r m a t i o n   E v e n t   S e n t \ n " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ G e t M i b P a r a m C m d I n v P a r a m   * / 
 
 
 
 v o i d   T e s t _ C F _ S e n d C f g P a r a m s C m d ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ N o A r g s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ C F G _ P A R A M S _ C C ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ C F G _ C M D _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d C f g P a r a m s C m d   * / 
 
 
 v o i d   T e s t _ C F _ S e n d C f g P a r a m s C m d I n v L e n ( v o i d ) { 
 
     C F _ N o A r g s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   2 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ C F G _ P A R A M S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d C f g P a r a m s C m d I n v L e n   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e C m d C r e a t E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ Q U E _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e C m d C r e a t E r r   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e C m d I n v L e n ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   2 7 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e U p Q V a l u e E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   1 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R _ C M D _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e U p Q V a l u e E r r   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e U p D e f F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   1 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   1 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ;     
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ Q _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e U p D e f F i l e n a m e   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e U p C u s t o m F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   1 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   2 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / r a m / f i l e n a m e . d a t " ) ; 
     
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ Q _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e U p C u s t o m F i l e n a m e   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e O u t Q V a l u e E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   4 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R _ C M D _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e O u t Q V a l u e E r r   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e O u t Q T y p e E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   3 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R _ C M D _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e O u t Q T y p e E r r   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e O u t C h a n E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   1 6 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R _ C M D _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e O u t C h a n E r r   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e O u t D e f F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
     
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ Q _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e O u t D e f F i l e n a m e   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e O n e E n t r y ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t             W r Q C m d M s g ; 
     C F _ P l a y b a c k F i l e C m d _ t         P b F i l e C m d M s g ; 
 
     / *   r e s e t   C F   g l o b a l s   e t c   * / 
     C F _ A p p I n i t ( ) ; 
 
     / *   C r e a t e   o n e   q u e u e   e n t r y   * / 
     / *   E x e c u t e   a   p l a y b a c k   f i l e   c o m m a n d   s o   t h a t   o n e   q u e u e   e n t r y   i s   a d d e d   t o   t h e   p e n d i n g   q u e u e   * / 
     C F E _ S B _ I n i t M s g ( & P b F i l e C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   0 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
     / *   e n d   . . .   C r e a t e   o n e   q u e u e   e n t r y   * / 
 
     / *   f o r c e   t h e   f i l e   c r e a t e   t o   r e t u r n   a   v a l i d   f i l e   d e s c r i p t o r   ( 5 )   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     
     / *   f o r c e   t h e   C F E _ F S _ W r i t e H d r   f u n c t i o n   t o   r e t u r n   a   v a l i d   b y t e   c o u n t   ( 9 6 )   * / 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   a   W r i t e   Q u e u e   C o m m a n d   n o w   t h a t   w e   h a v e   o n e   q u e u e   e n t r y   * / 
     C F E _ S B _ I n i t M s g ( & W r Q C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & W r Q C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     W r Q C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     W r Q C m d M s g . C h a n   =   0 ; 
     W r Q C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( W r Q C m d M s g . F i l e n a m e , " " ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & W r Q C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W r Q C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ Q _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P L A Y B A C K _ F I L E _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ;     
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e O n e E n t r y   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e O u t C u s t o m F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / r a m / f i l e n a m e . d a t " ) ; 
     
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ Q _ I N F O _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e O u t C u s t o m F i l e n a m e   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e W r i t e H d r E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / r a m / f i l e n a m e . d a t " ) ; 
     
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ F I L E W R I T E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e W r i t e H d r E r r   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e E n t r y W r i t e E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t             W r Q C m d M s g ; 
     C F _ P l a y b a c k F i l e C m d _ t         P b F i l e C m d M s g ; 
 
     / *   r e s e t   C F   g l o b a l s   e t c   * / 
     C F _ A p p I n i t ( ) ; 
 
     / *   C r e a t e   o n e   q u e u e   e n t r y   * / 
     / *   E x e c u t e   a   p l a y b a c k   f i l e   c o m m a n d   s o   t h a t   o n e   q u e u e   e n t r y   i s   a d d e d   t o   t h e   p e n d i n g   q u e u e   * / 
     C F E _ S B _ I n i t M s g ( & P b F i l e C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   0 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
     / *   e n d   . . .   C r e a t e   o n e   q u e u e   e n t r y   * / 
 
     / *   f o r c e   t h e   f i l e   c r e a t e   t o   r e t u r n   a   v a l i d   f i l e   d e s c r i p t o r   ( 5 )   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     
     / *   f o r c e   t h e   C F E _ F S _ W r i t e H d r   f u n c t i o n   t o   r e t u r n   a   v a l i d   b y t e   c o u n t   ( 9 6 )   * / 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
     
     / *   f o r c e   a n   e r r o r   w h e n   w r i t i n g   t h e   e n t r y   t o   t h e   f i l e   * / 
     / *   C o i n c i d e n t a l l y ,   e n t r i e s   a r e   t h e   s a m e   s i z e   a s   c f e   f i l e   h d r   ( 9 6 ) ,   t h i s   r t n s   1 2   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ W R I T E _ I N D E X ,   1 2 ,   1 ) ; 
 
     / *   E x e c u t e   a   W r i t e   Q u e u e   C o m m a n d   n o w   t h a t   w e   h a v e   o n e   q u e u e   e n t r y   * / 
     C F E _ S B _ I n i t M s g ( & W r Q C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & W r Q C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     W r Q C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     W r Q C m d M s g . C h a n   =   0 ; 
     W r Q C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( W r Q C m d M s g . F i l e n a m e , " " ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & W r Q C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & W r Q C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P L A Y B A C K _ F I L E _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ;     
     U t A s s e r t _ E v e n t S e n t ( C F _ F I L E W R I T E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e E n t r y W r i t e E r r   * / 
 
 
 
 
 
 v o i d   T e s t _ C F _ W r i t e Q u e u e I n v F i l e n a m e E r r ( v o i d ) { 
 
     C F _ W r i t e Q u e u e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R I T E _ Q U E U E _ I N F O _ C C ) ;   
 
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / r a m / f i l   e n a m e . d a t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e Q u e u e I n v F i l e n a m e E r r   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s D e f a u l t F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
 
     / *   c r e a t e   o n e   p l a y b a c k   c h a n   0 ,   a c t i v e   q u e u e   e n t r y   * / 
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ;   
 
     / *   c r e a t e   o n e   u p l i n k   a c t i v e   q u e u e   e n t r y   * / 
     C F _ A d d F i l e T o U p Q u e u e ( C F _ U P _ A C T I V E Q ,   ( C F _ Q u e u e E n t r y _ t   * ) & C F _ A p p D a t a . M e m . P a r t i t i o n ) ; 
 
     / *   b u i l d   c m d   t o   w r i t e   a l l   a c t i v e   e n t r i e s   t o   a   f i l e   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ;   
     C m d M s g . T y p e   =   0 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " " ) ; 
 
     / *   f o r c e   t h e   f i l e   c r e a t e   t o   r e t u r n   a   v a l i d   f i l e   d e s c r i p t o r   ( 5 )   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     
     / *   f o r c e   t h e   C F E _ F S _ W r i t e H d r   f u n c t i o n   t o   r e t u r n   a   v a l i d   b y t e   c o u n t   ( 9 6 )   * / 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 4 , " E v e n t   C o u n t   =   1 4 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P L A Y B A C K _ F I L E _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R A C T _ T R A N S _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s D e f a u l t F i l e n a m e   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s C u s t F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
     
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
 
     / *   b u i l d   c m d   t o   w r i t e   a l l   a c t i v e   e n t r i e s   t o   a   f i l e   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / c f / A c t i v e T r a n s a c t i o n s . d a t " ) ; 
 
     / *   f o r c e   t h e   f i l e   c r e a t e   t o   r e t u r n   a   v a l i d   f i l e   d e s c r i p t o r   ( 5 )   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     
     / *   f o r c e   t h e   C F E _ F S _ W r i t e H d r   f u n c t i o n   t o   r e t u r n   a   v a l i d   b y t e   c o u n t   ( 9 6 )   * / 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 4 , " E v e n t   C o u n t   =   1 4 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P L A Y B A C K _ F I L E _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R A C T _ T R A N S _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s C u s t F i l e n a m e   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s C m d I n v L e n ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   1 0 2 4 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s C m d I n v F i l e n a m e ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     C m d M s g . T y p e   =   2 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / c f   T r a n s a c t i o n s . d a t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s C m d I n v F i l e n a m e   * / 
 
 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s C r e a t F a i l ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     C m d M s g . T y p e   =   2 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / c f / T r a n s a c t i o n s . d a t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R A C T _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s C r e a t F a i l   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s W r H d r F a i l ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     C m d M s g . T y p e   =   2 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / c f / T r a n s a c t i o n s . d a t " ) ; 
 
     / *   f o r c e   t h e   f i l e   c r e a t e   t o   r e t u r n   a   v a l i d   f i l e   d e s c r i p t o r   ( 5 )   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ F I L E W R I T E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s W r H d r F a i l   * / 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s I n v W h i c h Q s ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   N o t e :   T h e   c o d e   n a m e d   t h e   ' T y p e '   c m d   p a r a m   -   W h i c h Q u e u e s ,   h e n c e   t h e   t e s t   n a m e   * / 
     C m d M s g . T y p e   =   3 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / c f / T r a n s a c t i o n s . d a t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ W R A C T _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s I n v W h i c h Q s   * / 
 
 
 
 v o i d   T e s t _ C F _ W r i t e A c t T r a n s E n t r y W r i t e E r r ( v o i d ) { 
 
     C F _ W r i t e A c t i v e T r a n s C m d _ t     C m d M s g ; 
     
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
 
     / *   b u i l d   c m d   t o   w r i t e   a l l   a c t i v e   e n t r i e s   t o   a   f i l e   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ W r i t e A c t i v e T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ W R _ A C T I V E _ T R A N S _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( a l l = 0 / u p = 1 / d o w n = 2 ) * / 
     s t r c p y ( C m d M s g . F i l e n a m e , " / c f / A c t i v e T r a n s a c t i o n s . d a t " ) ; 
 
     / *   f o r c e   t h e   f i l e   c r e a t e   t o   r e t u r n   a   v a l i d   f i l e   d e s c r i p t o r   ( 5 )   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ C R E A T _ I N D E X ,   5 ,   1 ) ; 
     
     / *   f o r c e   t h e   C F E _ F S _ W r i t e H d r   f u n c t i o n   t o   r e t u r n   a   v a l i d   b y t e   c o u n t   ( 9 6 )   * / 
     U t _ C F E _ F S _ S e t R e t u r n C o d e ( U T _ C F E _ F S _ W R I T E H D R _ I N D E X ,   9 6 ,   1 ) ; 
     
     / *   f o r c e   a n   e r r o r   w h e n   w r i t i n g   t h e   e n t r y   t o   t h e   f i l e   * / 
     / *   C o i n c i d e n t a l l y ,   e n t r i e s   a r e   t h e   s a m e   s i z e   a s   c f e   f i l e   h d r   ( 9 6 ) ,   t h i s   r t n s   1 2   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ W R I T E _ I N D E X ,   1 2 ,   1 ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 4 , " E v e n t   C o u n t   =   1 4 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P L A Y B A C K _ F I L E _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ F I L E W R I T E _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ W r i t e A c t T r a n s E n t r y W r i t e E r r   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s F i l e n a m e C m d ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / r a m / f i l e 4 . d a t " ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q U I C K _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s F i l e n a m e C m d   * / 
 
 
 
 v o i d   T e s t _ C F _ I n v C m d C o d e C m d ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   5 5 ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C C _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ I n v C m d C o d e C m d   * / 
 
 
 v o i d   T e s t _ C F _ I n v M s g I d C m d ( v o i d ) { 
 
     C F _ S e t M i b P a r a m _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   0 x 2 0 F ,   s i z e o f ( C F _ S e t M i b P a r a m _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ F R E E Z E _ C C ) ;   
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ M I D _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ I n v M s g I d C m d   * / 
 
 
 
 
 
 
 / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * * 
 * *             T r a n s a c t i o n   D i a g n o s t i c   c o m m a n d   
 * * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * / 
 
 
 v o i d   T e s t _ C F _ S e n d T r a n s D i a g C m d S u c c e s s ( v o i d ) { 
 
     C F _ S e n d T r a n s C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e n d T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ T R A N S _ D I A G _ D A T A _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / t e s t f i l e . t x t " ) ; 
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 4 , " E v e n t   C o u n t   =   1 4 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ T R A N S _ C M D _ E I D , C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r = = 2 , " C F _ A p p D a t a . H k . C m d C o u n t e r   =   2 " ) ;     
 
 } / *   e n d   T e s t _ C F _ S e n d T r a n s D i a g C m d S u c c e s s   * / 
 
 
 v o i d   T e s t _ C F _ S e n d T r a n s D i a g F i l e N o t F o u n d ( v o i d ) { 
 
     C F _ S e n d T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e n d T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ T R A N S _ D I A G _ D A T A _ C C ) ;       
     s t r c p y ( C m d M s g . T r a n s , " / g n d / f i l e . t x t " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ T R A N S _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d T r a n s D i a g F i l e N o t F o u n d   * / 
 
 
 v o i d   T e s t _ C F _ S e n d T r a n s D i a g T r a n s N o t F o u n d ( v o i d ) { 
 
     C F _ S e n d T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e n d T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ T R A N S _ D I A G _ D A T A _ C C ) ;   
     s t r c p y ( C m d M s g . T r a n s , " 2 . 3 5 _ 5 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S N D _ T R A N S _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d T r a n s D i a g T r a n s N o t F o u n d   * / 
 
 
 v o i d   T e s t _ C F _ S e n d T r a n s D i a g C m d I n v L e n ( v o i d ) { 
 
     C F _ S e n d T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   3 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ T R A N S _ D I A G _ D A T A _ C C ) ;       
     s t r c p y ( C m d M s g . T r a n s , " / g n d / f i l e . t x t " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d T r a n s D i a g C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ S e n d T r a n s D i a g U n t e r m S t r i n g ( v o i d ) { 
 
     C F _ S e n d T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e n d T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ T R A N S _ D I A G _ D A T A _ C C ) ;   
     C F E _ P S P _ M e m S e t     ( C m d M s g . T r a n s , 0 x F F ,   O S _ M A X _ P A T H _ L E N ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d T r a n s D i a g U n t e r m S t r i n g   * / 
 
 
 v o i d   T e s t _ C F _ S e n d T r a n s D i a g I n v F i l e n a m e ( v o i d ) { 
 
     C F _ S e n d T r a n s C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e n d T r a n s C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E N D _ T R A N S _ D I A G _ D A T A _ C C ) ;       
     s t r c p y ( C m d M s g . T r a n s , " / T h i s   s t r i n g   h a s   s p a c e s " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e n d T r a n s D i a g I n v F i l e n a m e   * / 
 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m C m d ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M 1 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m C m d   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m C m d I n v L e n ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   4 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v C h a n ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   1 3 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v D i r ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   8 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 )   * / 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v D i r   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v C l a s s ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   0 ; / *   1 = c l a s s   1 ,   2 =   c l a s s   2   * / 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v C l a s s   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v P r e s e r v e ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   2 ; / *   0 = d e l e t e ,   1 = k e e p   * / 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v P r e s e r v e   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v S r c ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f   / " ) ; / *   n o   s p a c e s   * / 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 5 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v S r c   * / 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v D s t ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 . 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n   d / p a t h " ) ; / *   s p a c e   i n   s t r i n g   * / 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 6 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v D s t   * / 
 
 
 
 v o i d   T e s t _ C F _ S e t P o l l P a r a m I n v I d ( v o i d ) { 
 
     C F _ S e t P o l l P a r a m C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ;     
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ S e t P o l l P a r a m C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ S E T _ P O L L _ P A R A M _ C C ) ;   
     
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . D i r   =   1 ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . P r i o r i t y   =   1 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d , " 2 3 4 2 0 0 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h , " / g n d / p a t h / " ) ; 
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ S E T _ P O L L _ P A R A M _ E R R 7 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ S e t P o l l P a r a m I n v I d   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e C m d I n v L e n ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   1 0 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 1 _ 2 0 9 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e T r a n s U n t e r m ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     C F E _ P S P _ M e m S e t ( C m d M s g . T r a n s , 0 x F F ,   O S _ M A X _ P A T H _ L E N ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e T r a n s U n t e r m   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e I n v F i l e n a m e ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / F i l e n a m e   w i t h   s p a c e " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e I n v F i l e n a m e   * / 
 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e F i l e N o t F o u n d ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / F i l e D o e s N o t E x i s t . t x t " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e F i l e N o t F o u n d   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e I d N o t F o u n d ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 1 _ 2 0 9 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e I d N o t F o u n d   * / 
 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e U p A c t i v e ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 3 _ 5 0 0 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     C F _ T s t U t i l _ C r e a t e O n e U p A c t i v e Q u e u e E n t r y ( ) ; 
 
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
     
     / *   T h i s   d e q u e u e   c o m m a n d   w i l l   p r o d u c e   t h e   w a r n i n g   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     / *   T h i s   c m d   w i l l   d e q u e u e   w i t h o u t   w a r n i n g   b e c a u s e   i t s   t h e   s e c o n d   i d e n t i c a l   c m d   * / 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     O S _ p r i n t f ( " C F _ A p p D a t a . H k . E r r C o u n t e r   =   % d \ n " , C F _ A p p D a t a . H k . E r r C o u n t e r ) ; 
       O S _ p r i n t f ( " C F _ A p p D a t a . H k . C m d C o u n t e r   =   % d \ n " , C F _ A p p D a t a . H k . C m d C o u n t e r ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E _ E R R 2 _ E I D ,   C F E _ E V S _ C R I T I C A L ,   " " ,   " C r i t i c a l   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E 1 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   2 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   2 " ) ; 
     
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e U p A c t i v e   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e U p H i s t ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 3 _ 5 0 0 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     C F _ T s t U t i l _ C r e a t e O n e U p H i s t o r y Q u e u e E n t r y ( ) ; 
 
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N _ T R A N S _ O K _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E 1 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 " ) ; 
     
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e U p H i s t   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e P b P e n d ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / t e s t f i l e . t x t " ) ; 
     C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( ) ; 
     
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ;     
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E 2 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   2 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   2 " ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e P b P e n d   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e P b A c t i v e ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 1 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
         
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
 
     / *   T h i s   d e q u e u e   c m d   w i l l   p r o d u c e   t h e   w a r n i n g   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     / *   T h i s   c m d   w i l l   d e q u e u e   w i t h o u t   w a r n i n g   b e c a u s e   i t s   t h e   s e c o n d   i d e n t i c a l   c m d   * / 
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 5 , " E v e n t   C o u n t   =   1 5 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E _ E R R 3 _ E I D ,   C F E _ E V S _ C R I T I C A L ,   " " ,   " C r i t i c a l   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   3 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   3 " ) ; 
         
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e P b A c t i v e   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e P b H i s t ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 1 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     C F _ T s t U t i l _ C r e a t e O n e P b H i s t o r y Q u e u e E n t r y ( ) ; 
 
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
         
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 5 , " E v e n t   C o u n t   =   2 5 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ O U T _ T R A N S _ F A I L E D _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   3 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 " ) ; 
     
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e P b H i s t   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e P u t F a i l ( v o i d ) { 
 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 1 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     C F _ T s t U t i l _ C r e a t e O n e P b H i s t o r y Q u e u e E n t r y ( ) ; 
 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
         
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 6 , " E v e n t   C o u n t   =   2 6 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ O U T _ T R A N S _ F A I L E D _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ M E M _ D E A L L O C _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   3 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 " ) ; 
     
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e P u t F a i l   * / 
 
 
 v o i d   T e s t _ C F _ D e l e t e Q u e u e N o d e I n v T y p e ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t           * P t r ; 
     C F _ D e q u e u e N o d e C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ D e q u e u e N o d e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D E L E T E _ Q U E U E _ N O D E _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / t e s t f i l e . t x t " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     P t r   =   ( C F _ Q u e u e E n t r y _ t   * ) C F _ A p p D a t a . M e m . P a r t i t i o n ;     
     C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( ) ; 
     P t r - > N o d e T y p e   =   5 5 ; 
     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
         
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D E Q _ N O D E _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ;     
 
 } / *   e n d   T e s t _ C F _ D e l e t e Q u e u e N o d e I n v T y p e   * / 
 
 
 
 v o i d   T e s t _ C F _ P u r g e Q u e u e C m d I n v L e n ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   2 8 0 0 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e Q u e u e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e U p l i n k A c t i v e ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   1 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   1 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e U p l i n k A c t i v e   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e U p H i s t o r y ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   1 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   2 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
     
     C F _ T s t U t i l _ C r e a t e O n e U p H i s t o r y Q u e u e E n t r y ( ) ; 
     
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q 1 _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
     
     C F _ R e s e t E n g i n e ( ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e U p H i s t o r y   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e I n v U p Q ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   1 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   3 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e I n v U p Q   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e O u t A c t i v e ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   1 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e O u t A c t i v e   * / 
 
 
 
 v o i d   T e s t _ C F _ P u r g e O u t P e n d ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( ) ; 
     
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q 2 _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   2 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   2 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e O u t P e n d   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e O u t H i s t ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   2 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
         
     C F _ T s t U t i l _ C r e a t e O n e P b H i s t o r y Q u e u e E n t r y ( ) ; 
 
     / *   H a v e   p u t   p o o l   r e t u r n   p o s i t i v e   n u m b e r ( i n d i c a t i n g   s u c c e s s )   i n s t e a d   o f   d e f a u l t   z e r o   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ P U T P O O L B U F _ I N D E X ,   1 6 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 2 5 , " E v e n t   C o u n t   =   2 5 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q 2 _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   3 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   3 " ) ; 
     
     C F _ R e s e t E n g i n e ( ) ; 
     
     
 } / *   e n d   T e s t _ C F _ P u r g e O u t H i s t   * / 
 
 
 
 v o i d   T e s t _ C F _ P u r g e I n v O u t Q ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   3 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e I n v O u t Q   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e I n v O u t C h a n ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   2 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   4 7 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q _ E R R 5 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e I n v O u t C h a n   * / 
 
 
 v o i d   T e s t _ C F _ P u r g e I n v T y p e ( v o i d ) { 
 
     C F _ P u r g e Q u e u e C m d _ t     C m d M s g ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P u r g e Q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P U R G E _ Q U E U E _ C C ) ;   
     C m d M s g . T y p e   =   3 ; / * ( u p = 1 / d o w n = 2 ) * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . Q u e u e   =   0 ; / *   0 = p e n d i n g , 1 = a c t i v e , 2 = h i s t o r y   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * /     
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P U R G E Q _ E R R 6 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P u r g e I n v T y p e   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e D e q u e u e C m d ( v o i d ) { 
 
     C F _ E n D i s D e q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s D e q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D E Q U E U E _ C C ) ; 
     C m d M s g . C h a n   = 0 ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ E N A _ D Q _ C M D _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ E n a b l e D e q u e u e C m d   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e D e q u e u e C m d I n v L e n ( v o i d ) { 
 
     C F _ E n D i s D e q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   9 7 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D E Q U E U E _ C C ) ; 
     C m d M s g . C h a n   = 0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ E n a b l e D e q u e u e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e D e q u e u e I n v C h a n ( v o i d ) { 
 
     C F _ E n D i s D e q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s D e q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D E Q U E U E _ C C ) ; 
     C m d M s g . C h a n   = 4 9 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D Q _ C M D _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ E n a b l e D e q u e u e I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e D e q u e u e C m d ( v o i d ) { 
 
     C F _ E n D i s D e q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s D e q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D E Q U E U E _ C C ) ; 
     C m d M s g . C h a n   =   0 ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D I S _ D Q _ C M D _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e D e q u e u e C m d   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e D e q u e u e C m d I n v L e n ( v o i d ) { 
 
     C F _ E n D i s D e q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   3 4 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D E Q U E U E _ C C ) ; 
     C m d M s g . C h a n   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e D e q u e u e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e D e q u e u e I n v C h a n ( v o i d ) { 
 
     C F _ E n D i s D e q u e u e C m d _ t     C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s D e q u e u e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D E Q U E U E _ C C ) ; 
     C m d M s g . C h a n   =   4 6 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D Q _ C M D _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e D e q u e u e I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e D i r P o l l i n g C m d ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ E N A _ P O L L _ C M D 2 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ E n a b l e D i r P o l l i n g C m d   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e D i r P o l l i n g C m d I n v L e n ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   6 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ E n a b l e D i r P o l l i n g C m d I n v L e n   * / 
 
 
 
 v o i d   T e s t _ C F _ E n a b l e P o l l i n g I n v C h a n ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 3 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ E N A _ P O L L _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ E n a b l e P o l l i n g I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e P o l l i n g I n v D i r ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 3 2 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ E N A _ P O L L _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ E n a b l e P o l l i n g I n v D i r   * / 
 
 
 v o i d   T e s t _ C F _ E n a b l e P o l l i n g A l l ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ E N A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 x F F ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ E N A _ P O L L _ C M D 1 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ E n a b l e P o l l i n g A l l   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D I R _ P O L L I N G _ C C ) ; 
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * /     
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D I S _ P O L L _ C M D 2 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d I n v L e n ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   3 0 0 0 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D I R _ P O L L I N G _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e P o l l i n g I n v C h a n ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 3 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D I S _ P O L L _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e P o l l i n g I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e P o l l i n g I n v D i r ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 3 2 ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D I S _ P O L L _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e P o l l i n g I n v D i r   * / 
 
 
 v o i d   T e s t _ C F _ D i s a b l e P o l l i n g A l l ( v o i d ) { 
 
     C F _ E n D i s P o l l C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ E n D i s P o l l C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ D I S A B L E _ D I R _ P O L L I N G _ C C ) ;   
     C m d M s g . C h a n   = 0 ; / *   0   t o   ( C F _ M A X _ P L A Y B A C K _ C H A N N E L S   -   1 )   * / 
     C m d M s g . D i r   = 0 x F F ; / *   0   t o   ( C F _ M A X _ P O L L I N G _ D I R S _ P E R _ C H A N   -   1 ) ,   o r   0 x F F   f o r   e n / d i s   a l l   * / 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ D I S _ P O L L _ C M D 1 _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     
 } / *   e n d   T e s t _ C F _ D i s a b l e P o l l i n g A l l   * / 
 
 
 v o i d   T e s t _ C F _ K i c k S t a r t C m d ( v o i d ) { 
 
     C F _ K i c k s t a r t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ K i c k s t a r t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ K I C K S T A R T _ C C ) ; 
     C m d M s g . C h a n   =   1 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ K I C K S T A R T _ C M D _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ K i c k S t a r t C m d   * / 
 
 
 v o i d   T e s t _ C F _ K i c k S t a r t C m d I n v L e n ( v o i d ) { 
 
     C F _ K i c k s t a r t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   6 5 0 ,   F A L S E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ K I C K S T A R T _ C C ) ; 
     C m d M s g . C h a n   =   1 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ K i c k S t a r t C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ K i c k S t a r t C m d I n v C h a n ( v o i d ) { 
 
     C F _ K i c k s t a r t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ K i c k s t a r t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ K I C K S T A R T _ C C ) ; 
     C m d M s g . C h a n   =   5 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ K I C K S T A R T _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ K i c k S t a r t C m d I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s T r a n s C m d ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 6 " ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q U I C K _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s T r a n s C m d   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s A c t i v e T r a n s ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " 0 . 2 4 _ 1 " ) ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 4 , " E v e n t   C o u n t   =   1 4 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q U I C K _ C M D _ E I D , C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   2 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   2 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 " ) ; 
         
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s A c t i v e T r a n s   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s A c t i v e N a m e ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / t e s t f i l e . t x t " ) ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 4 , " E v e n t   C o u n t   =   1 4 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q U I C K _ C M D _ E I D , C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   2 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   2 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 " ) ; 
         
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s A c t i v e N a m e   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s A c t i v e S u s p e n d e d ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t       C m d M s g ; 
     C F _ C A R S C m d _ t                 S u s p e n d C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / c f / t e s t f i l e . t x t " ) ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     C F _ T s t U t i l _ C r e a t e O n e P b A c t i v e Q u e u e E n t r y ( ) ; 
     
     / *   S u s p e n d   t h e   a c t i v e   t r a n s a c t i o n   * / 
     C F E _ S B _ I n i t M s g ( & S u s p e n d C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ C A R S C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & S u s p e n d C m d M s g ,   C F _ S U S P E N D _ C C ) ; 
     s t r c p y ( S u s p e n d C m d M s g . T r a n s , " / c f / t e s t f i l e . t x t " ) ;   
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & S u s p e n d C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & S u s p e n d C m d M s g ) ;     
         
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 7 , " E v e n t   C o u n t   =   1 7 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q U I C K _ C M D _ E I D , C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   3 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 " ) ; 
         
     C F _ R e s e t E n g i n e ( ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s A c t i v e S u s p e n d e d   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s C m d I n v L e n ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   0 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     p r i n t f ( " C F _ A p p D a t a . H k . E r r C o u n t e r   =   % d \ n " , C F _ A p p D a t a . H k . E r r C o u n t e r ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k P a c k e t . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s U n t e r m S t r i n g ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     C F E _ P S P _ M e m S e t     ( C m d M s g . T r a n s , 0 x F F ,   O S _ M A X _ P A T H _ L E N ) ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ N O _ T E R M _ E R R _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s U n t e r m S t r i n g   * / 
 
 
 v o i d   T e s t _ C F _ Q u i c k S t a t u s I n v F i l e n a m e ( v o i d ) { 
 
     C F _ Q u i c k S t a t C m d _ t     C m d M s g ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ Q u i c k S t a t C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ Q U I C K S T A T U S _ C C ) ; 
     s t r c p y ( C m d M s g . T r a n s , " / i n v a l i d   f i l e n a m e " ) ; 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ Q u i c k S t a t u s I n v F i l e n a m e   * / 
 
 
 
 v o i d   T e s t _ C F _ P b F i l e N o M e m ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ N O M E M 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b F i l e N o M e m   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e C m d I n v L e n ( v o i d ) { 
 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   1 6 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e C m d P a r a m E r r ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   3 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ F I L E _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e C m d P a r a m E r r   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e C h a n N o t I n U s e ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   1 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ F I L E _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e C h a n N o t I n U s e   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e I n v S r c F i l e n a m e ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f /   t e s t f i l e . t x t " ) ; / *   n o   s p a c e s   * / 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e I n v S r c F i l e n a m e   * / 
 
 
 
 v o i d   T e s t _ C F _ P b F i l e I n v D s t F i l e n a m e ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     C F E _ P S P _ M e m S e t     ( C m d M s g . D s t F i l e n a m e , 0 x F F ,   O S _ M A X _ P A T H _ L E N ) ; / *   d e s t   f i l e n a m e   n o t   t e r m i n a t e d   * / 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N V _ F I L E N A M E _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e I n v D s t F i l e n a m e   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e P e n d Q F u l l ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
     
     / *   s e t   t h e   p e n d i n g   q u e u e   d e p t h   t o   t h e   m a x   * / 
     C F _ A p p D a t a . C h a n [ 0 ] . P b Q [ 0 ] . E n t r y C n t   =   C F _ A p p D a t a . T b l - > O u C h [ 1 ] . P e n d i n g Q D e p t h ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ F I L E _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e P e n d Q F u l l   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e I n v P e e r I d ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 5 . 2 5 5 " ) ; / *   2 5 5   c h a n g e d   t o   2 5 5 5   * / 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ F I L E _ E R R 6 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 
 } / *   e n d   T e s t _ C F _ P b F i l e I n v P e e r I d   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e F i l e O p e n ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
     
     / *   F o r c e   O S _ F D G e t I n f o   t o   r e t u r n   ' f i l e   i s   o p e n '   a n d   s u c c e s s   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ F D G E T I N F O _ I N D E X ,   & O S _ F D G e t I n f o H o o k ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ F D G E T I N F O _ I N D E X ,   N U L L ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ F I L E _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e F i l e O p e n   * / 
 
 
 v o i d   T e s t _ C F _ P b F i l e F i l e O n Q ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k F i l e C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n n e l   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c F i l e n a m e ,   " / c f / t e s t f i l e . t x t " ) ; 
     s t r c p y ( C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
     
     / *   s e n d   c m d   a g a i n   t o   i n v o k e   t h e   e r r o r   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
         p r i n t f ( " Q d e p t h = % u ,   C m d C t r = % d ,   E r r C t r = % d \ n " , ( u n s i g n e d   i n t ) U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) , 
                     C F _ A p p D a t a . H k . C m d C o u n t e r , C F _ A p p D a t a . H k . E r r C o u n t e r ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ F I L E _ E R R 5 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   1 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b F i l e F i l e O n Q   * / 
 
 
 v o i d   T e s t _ C F _ P b D i r C m d ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h ,   " g n d p a t h / " ) ; 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   F o r c e   O S _ o p e n d i r   t o   r e t u r n   s u c c e s s ,   i n s t e a d   o f   d e f a u l t   N U L L   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ O P E N D I R _ I N D E X ,   5 ,   1 ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P L A Y B A C K _ D I R _ E I D ,   C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r C m d   * / 
 
 
 v o i d   T e s t _ C F _ P b D i r C m d O p e n E r r ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h ,   " g n d p a t h / " ) ; 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ O P E N _ D I R _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r C m d O p e n E r r   * / 
 
 
 
 v o i d   T e s t _ C F _ P b D i r C m d I n v L e n ( v o i d ) { 
 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   7 ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ C M D _ L E N _ E R R _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r C m d I n v L e n   * / 
 
 
 v o i d   T e s t _ C F _ P b D i r C m d P a r a m E r r ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   3 ; / *   i n v a l i d   c l a s s   * / 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ D I R _ E R R 1 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r C m d P a r a m E r r   * / 
 
 
 v o i d   T e s t _ C F _ P b D i r C h a n N o t I n U s e ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n   =   1 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ D I R _ E R R 2 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r C h a n N o t I n U s e   * / 
 
 
 v o i d   T e s t _ C F _ P b D i r I n v S r c P a t h ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f /   t e s t f i l e . t x t " ) ; / *   n o   s p a c e s ,   n o   s l a s h   a t   e n d   * / 
     s t r c p y ( C m d M s g . D s t P a t h ,   " g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ D I R _ E R R 3 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r I n v S r c P a t h   * / 
 
 
 
 v o i d   T e s t _ C F _ P b D i r I n v D s t P a t h ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 . 2 5 5 " ) ; 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f / " ) ; 
     C F E _ P S P _ M e m S e t     ( C m d M s g . D s t P a t h , 0 x F F ,   O S _ M A X _ P A T H _ L E N ) ; / *   d e s t   f i l e n a m e   n o t   t e r m i n a t e d   * / 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ D I R _ E R R 4 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r I n v D s t P a t h   * / 
 
 
 v o i d   T e s t _ C F _ P b D i r I n v P e e r I d ( v o i d ) { 
 
     i n t 3 2                               E x p A p p I n i t R t n , A c t A p p I n i t R t n ; 
     C F _ P l a y b a c k D i r C m d _ t       C m d M s g ; 
     
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k D i r C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ P L A Y B A C K _ D I R _ C C ) ; 
     C m d M s g . C l a s s   =   1 ; 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . P r i o r i t y   =   0 ; 
     C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( C m d M s g . P e e r E n t i t y I d ,   " 2 5 5 5 . 2 5 5 " ) ; / *   2 5 5   c h a n g e d   t o   2 5 5 5   * / 
     s t r c p y ( C m d M s g . S r c P a t h ,   " / c f / " ) ; 
     s t r c p y ( C m d M s g . D s t P a t h ,   " / g n d p a t h / " ) ; 
 
     E x p A p p I n i t R t n   =   C F E _ S U C C E S S ; 
     A c t A p p I n i t R t n   =   C F _ A p p I n i t ( ) ; 
 
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ T r u e ( A c t A p p I n i t R t n   = =   E x p A p p I n i t R t n ,   " A c t A p p I n i t R t n   =   E x p A p p I n i t R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ I N I T _ E I D ,   C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ P B _ D I R _ E R R 5 _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b D i r I n v P e e r I d   * / 
 
 
 v o i d   T e s t _ C F _ Q D i r F i l e s Q F u l l ( v o i d ) { 
 
     C F _ Q u e u e D i r F i l e s _ t     Q d f ; 
     u i n t 3 2                             A c t R t n , E x p R t n ; 
     
     / *   S e t u p   I n p u t s   * / 
     Q d f . C h a n   =   0 ; 
     Q d f . C l a s s   =   1 ; 
     Q d f . P r i o r i t y   =   1 ; 
     Q d f . P r e s e r v e   =   1 ; 
     Q d f . C m d O r P o l l   =   2 ; 
     s t r c p y ( Q d f . P e e r E n t i t y I d , " 0 . 2 3 " ) ; 
     s t r c p y ( Q d f . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( Q d f . D s t P a t h , " / g n d / " ) ; 
     
     / *   m a k e s   t h e   p e n d i n g   q u e u e   a p p e a r   f u l l   * / 
     C F _ A p p D a t a . C h a n [ 0 ] . P b Q [ 0 ] . E n t r y C n t   =   C F _ A p p D a t a . T b l - > O u C h [ 0 ] . P e n d i n g Q D e p t h ; 
         
         / *   F o r c e   O S _ o p e n d i r   t o   r e t u r n   s u c c e s s ,   i n s t e a d   o f   d e f a u l t   N U L L   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ O P E N D I R _ I N D E X ,   5 ,   1 ) ; 
 
     / *   
     * *   F o r c e   O S _ r e a d d i r   t o   f i r s t   r e t u r n   a   ' d o t   f i l e n a m e , t h e n   a   S u b   D i r ,   
     * *   t h e n   t h e   Q u e u e   F u l l   C h e c k   w i l l   f a i l   d u e   t o   l i n e   a b o v e   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ R E A D D I R _ I N D E X ,   & O S _ r e a d d i r H o o k ) ; 
     
     / *   E x e c u t e   T e s t   * /   
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ Q u e u e D i r e c t o r y F i l e s ( & Q d f ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 , " E v e n t   C o u n t   =   1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ P Q F U L _ E I D ,   C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
 
     R e a d d i r H o o k C a l l C n t   =   0 ; 
     C F _ A p p D a t a . C h a n [ 0 ] . P b Q [ 0 ] . E n t r y C n t   =   0 ; 
 
 } / *   e n d   T e s t _ C F _ Q D i r F i l e s Q F u l l   * / 
 
 
 
 v o i d   T e s t _ C F _ Q D i r F i l e s N o M e m ( v o i d ) { 
 
     C F _ Q u e u e D i r F i l e s _ t     Q d f ; 
     u i n t 3 2                             A c t R t n , E x p R t n ; 
     
     / *   S e t u p   I n p u t s   * / 
     Q d f . C h a n   =   0 ; 
     Q d f . C l a s s   =   1 ; 
     Q d f . P r i o r i t y   =   1 ; 
     Q d f . P r e s e r v e   =   1 ; 
     Q d f . C m d O r P o l l   =   2 ; 
     s t r c p y ( Q d f . P e e r E n t i t y I d , " 0 . 2 3 " ) ; 
     s t r c p y ( Q d f . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( Q d f . D s t P a t h , " / g n d / " ) ; 
   
     C F _ A p p I n i t ( ) ; 
     
     / *   F o r c e   O S _ o p e n d i r   t o   r e t u r n   s u c c e s s ,   i n s t e a d   o f   d e f a u l t   N U L L   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ O P E N D I R _ I N D E X ,   5 ,   1 ) ; 
     
     / *   m a k e   t h e   m e m   a l l o c a t i o n   f o r   t h e   n e w   q u e u e   n o d e   f a i l   * / 
     U t _ C F E _ E S _ S e t R e t u r n C o d e ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   - 1 ,   1 ) ; 
 
     / *   
     * *   F o r c e   O S _ r e a d d i r   t o   f i r s t   r e t u r n   a   ' d o t   f i l e n a m e , t h e n   a   S u b   D i r ,   
     * *   t h e n   t h e   Q u e u e   F u l l   C h e c k   w i l l   f a i l   d u e   t o   l i n e   a b o v e   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ R E A D D I R _ I N D E X ,   & O S _ r e a d d i r H o o k ) ; 
     
     / *   E x e c u t e   T e s t   * /   
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ Q u e u e D i r e c t o r y F i l e s ( & Q d f ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 3 , " E v e n t   C o u n t   =   1 3 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 1 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 2 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ N O M E M 2 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     
     R e a d d i r H o o k C a l l C n t   =   0 ; 
 
 } / *   e n d   T e s t _ C F _ Q D i r F i l e s N o M e m   * / 
 
 
 v o i d   T e s t _ C F _ Q D i r F i l e s F i l e O n Q ( v o i d ) { 
 
     C F _ Q u e u e D i r F i l e s _ t     Q d f ; 
     u i n t 3 2                             A c t R t n , E x p R t n ; 
     
     / *   S e t u p   I n p u t s   * / 
     Q d f . C h a n   =   0 ; 
     Q d f . C l a s s   =   1 ; 
     Q d f . P r i o r i t y   =   1 ; 
     Q d f . P r e s e r v e   =   1 ; 
     Q d f . C m d O r P o l l   =   2 ; 
     s t r c p y ( Q d f . P e e r E n t i t y I d , " 0 . 2 3 " ) ; 
     s t r c p y ( Q d f . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( Q d f . D s t P a t h , " / g n d / " ) ; 
                 
     / *   T h i s   c a l l   p u t s   o n   t h e   p e n d i n g   q u e u e ,   t h e   s a m e   f i l e n a m e   l i s t e d   i n   r e a d d i r H o o k   * / 
     C F _ T s t U t i l _ C r e a t e O n e P e n d i n g Q u e u e E n t r y ( ) ; 
     
     / *   F o r c e   O S _ o p e n d i r   t o   r e t u r n   s u c c e s s ,   i n s t e a d   o f   d e f a u l t   N U L L   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ O P E N D I R _ I N D E X ,   5 ,   1 ) ; 
 
     / *   
     * *   F o r c e   O S _ r e a d d i r   t o   f i r s t   r e t u r n   a   ' d o t   f i l e n a m e , t h e n   a   S u b   D i r ,   
     * *   t h e n   t h e   Q u e u e   F u l l   C h e c k   w i l l   f a i l   d u e   t o   l i n e   a b o v e   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ R E A D D I R _ I N D E X ,   & O S _ r e a d d i r H o o k ) ; 
     
     / *   E x e c u t e   T e s t   * /   
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ Q u e u e D i r e c t o r y F i l e s ( & Q d f ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 3 , " E v e n t   C o u n t   =   1 3 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 1 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 2 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ A C T I V E F I L E _ E I D , C F E _ E V S _ D E B U G ,   " " ,   " D e b u g   E v e n t   S e n t " ) ; 
     
     R e a d d i r H o o k C a l l C n t   =   0 ; 
     
 } / *   e n d   T e s t _ C F _ Q D i r F i l e s F i l e O n Q   * / 
 
 
 v o i d   T e s t _ C F _ Q D i r F i l e s F i l e O p e n ( v o i d ) { 
 
     C F _ Q u e u e D i r F i l e s _ t     Q d f ; 
     u i n t 3 2                             A c t R t n , E x p R t n ; 
     
     / *   S e t u p   I n p u t s   * / 
     Q d f . C h a n   =   0 ; 
     Q d f . C l a s s   =   1 ; 
     Q d f . P r i o r i t y   =   1 ; 
     Q d f . P r e s e r v e   =   1 ; 
     Q d f . C m d O r P o l l   =   2 ; 
     s t r c p y ( Q d f . P e e r E n t i t y I d , " 0 . 2 3 " ) ; 
     s t r c p y ( Q d f . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( Q d f . D s t P a t h , " / g n d / " ) ; 
   
     C F _ A p p I n i t ( ) ; 
             
     / *   F o r c e   O S _ F D G e t I n f o   t o   r e t u r n   ' f i l e   i s   o p e n '   a n d   s u c c e s s   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ F D G E T I N F O _ I N D E X ,   & O S _ F D G e t I n f o H o o k ) ; 
     
     / *   F o r c e   O S _ o p e n d i r   t o   r e t u r n   s u c c e s s ,   i n s t e a d   o f   d e f a u l t   N U L L   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ O P E N D I R _ I N D E X ,   5 ,   1 ) ; 
     
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   
     * *   F o r c e   O S _ r e a d d i r   t o   f i r s t   r e t u r n   a   ' d o t   f i l e n a m e , t h e n   a   S u b   D i r ,   
     * *   t h e n   t h e   Q u e u e   F u l l   C h e c k   w i l l   f a i l   d u e   t o   l i n e   a b o v e   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ R E A D D I R _ I N D E X ,   & O S _ r e a d d i r H o o k ) ; 
     
     / *   E x e c u t e   T e s t   * /   
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ Q u e u e D i r e c t o r y F i l e s ( & Q d f ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 2 , " E v e n t   C o u n t   =   1 2 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 1 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 2 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ O P E N F I L E _ E I D , C F E _ E V S _ I N F O R M A T I O N ,   " " ,   " I n f o   E v e n t   S e n t " ) ; 
 
     R e a d d i r H o o k C a l l C n t   =   0 ; 
     U t _ O S F I L E A P I _ H o o k T a b l e . O S _ F D G e t I n f o   =   N U L L ; 
 
 } / *   e n d   T e s t _ C F _ Q D i r F i l e s F i l e O p e n   * / 
 
 
 
 v o i d   T e s t _ C F _ Q D i r F i l e s A l l G o o d ( v o i d ) { 
 
     C F _ Q u e u e D i r F i l e s _ t     Q d f ; 
     u i n t 3 2                             A c t R t n , E x p R t n ; 
     
     / *   S e t u p   I n p u t s   * / 
     Q d f . C h a n   =   0 ; 
     Q d f . C l a s s   =   1 ; 
     Q d f . P r i o r i t y   =   1 ; 
     Q d f . P r e s e r v e   =   1 ; 
     Q d f . C m d O r P o l l   =   C F _ P O L L D I R E C T O R Y ; 
     s t r c p y ( Q d f . P e e r E n t i t y I d , " 0 . 2 3 " ) ; 
     s t r c p y ( Q d f . S r c P a t h , " / c f / " ) ; 
     s t r c p y ( Q d f . D s t P a t h , " / g n d / " ) ; 
     
     C F _ A p p I n i t ( ) ; 
 
     / *   F o r c e   O S _ o p e n d i r   t o   r e t u r n   s u c c e s s ,   i n s t e a d   o f   d e f a u l t   N U L L   * / 
     U t _ O S F I L E A P I _ S e t R e t u r n C o d e ( U T _ O S F I L E A P I _ O P E N D I R _ I N D E X ,   5 ,   1 ) ; 
       
     / *   F o r c e   O S _ r e a d d i r   t o   f i r s t   r e t u r n   a   ' d o t   f i l e n a m e , t h e n   a   S u b   D i r ,   
     * *   t h e n   t h e   Q u e u e   F u l l   C h e c k   w i l l   f a i l   d u e   t o   l i n e   a b o v e   * / 
     U t _ O S F I L E A P I _ S e t F u n c t i o n H o o k ( U T _ O S F I L E A P I _ R E A D D I R _ I N D E X ,   & O S _ r e a d d i r H o o k ) ; 
     
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
     
     / *   E x e c u t e   T e s t   * /   
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ Q u e u e D i r e c t o r y F i l e s ( & Q d f ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 1 , " E v e n t   C o u n t   =   1 1 " ) ; 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 1 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ Q D I R _ I N V _ N A M E 2 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E R R O R   E v e n t   S e n t " ) ; 
     
     R e a d d i r H o o k C a l l C n t   =   0 ; 
     
 } / *   e n d   T e s t _ C F _ Q D i r F i l e s A l l G o o d   * / 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e R e m o v e F i r s t ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 ,   N o d e 2 ,   N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 2 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 3 , " E n t r y C n t B e f o r e   =   3 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t A f t e r   =   2 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b Q u e u e R e m o v e F i r s t   * / 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e R e m o v e M i d d l e ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 ,   N o d e 2 ,   N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 2 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 3 , " E n t r y C n t B e f o r e   =   3 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 2 ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t A f t e r   =   2 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b Q u e u e R e m o v e M i d d l e   * / 
 
 v o i d   T e s t _ C F _ P b Q u e u e R e m o v e L a s t ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 ,   N o d e 2 ,   N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 2 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 3 , " E n t r y C n t B e f o r e   =   3 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t A f t e r   =   2 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b Q u e u e R e m o v e L a s t   * / 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e R e m o v e N u l l ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 ,   N o d e 2 ,   N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 2 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 3 , " E n t r y C n t B e f o r e   =   3 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m P b Q u e u e ( C h a n ,   Q u e u e ,   N U L L ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 3 , " E n t r y C n t A f t e r   =   3 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b Q u e u e R e m o v e N u l l   * / 
 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e I n s e r t I n v C h a n ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 , N o d e 2 , N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
     
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t B e f o r e   =   2 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ I n s e r t P b N o d e ( C h a n   +   5 , Q u e u e , & N o d e 2 ,   & N o d e 3 ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t A f t e r   =   2 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b Q u e u e I n s e r t I n v C h a n   * / 
 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e I n s e r t I n v Q ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 , N o d e 2 , N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
     
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t B e f o r e   =   2 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ I n s e r t P b N o d e ( C h a n , Q u e u e   +   7 , & N o d e 2 ,   & N o d e 3 ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t A f t e r   =   2 " ) ; 
 
 } / *   e n d   T e s t _ C F _ P b Q u e u e I n s e r t I n v Q   * / 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e I n s e r t G o o d ( v o i d ) { 
 
     C F _ Q u e u e E n t r y _ t       N o d e 1 , N o d e 2 , N o d e 3 ; 
     u i n t 3 2                         C h a n   =   0 ; 
     u i n t 3 2                         Q u e u e   =   0 ; 
     u i n t 3 2                         A c t R t n , E x p R t n ; 
         
     / *   S e t u p   I n p u t s   * / 
     C F _ A p p I n i t ( ) ; 
 
     N o d e 1 . P r e v   =   N U L L ; 
     N o d e 1 . N e x t   =   N U L L ; 
     N o d e 1 . T r a n s N u m   =   1 ; 
 
     N o d e 2 . P r e v   =   N U L L ; 
     N o d e 2 . N e x t   =   N U L L ; 
     N o d e 2 . T r a n s N u m   =   2 ;     
 
     N o d e 3 . P r e v   =   N U L L ; 
     N o d e 3 . N e x t   =   N U L L ; 
     N o d e 3 . T r a n s N u m   =   3 ; 
     
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 3 ) ; 
     C F _ A d d F i l e T o P b Q u e u e ( C h a n ,   Q u e u e ,   & N o d e 1 ) ; 
     
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 2 , " E n t r y C n t B e f o r e   =   2 " ) ; 
     
     / *   E x e c u t e   T e s t   * / 
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ I n s e r t P b N o d e ( C h a n , Q u e u e , & N o d e 2 ,   & N o d e 3 ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( A c t R t n   = =   E x p R t n ,   " A c t R t n   =   E x p R t n " ) ; 
     U t A s s e r t _ T r u e ( C F _ T s t U t i l _ V e r i f y L i s t O r d e r ( " 1 2 3 " ) = = C F _ S U C C E S S , " L i s t O r d e r   1 , 2 , 3 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ C h a n ] . P b Q [ Q u e u e ] . E n t r y C n t = = 3 , " E n t r y C n t A f t e r   =   3 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b Q u e u e I n s e r t G o o d   * / 
 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e F r o n t G o o d ( v o i d ) { 
 
     / *   q u e u e   o n e   l o w   p r i o r i t y   f i l e ,   t h e n   a   h i g h   p r i o r i t y   f i l e . 
     T h i s   s h o u l d   c a u s e   C F _ I n s e r t P b N o d e A t F r o n t   t o   b e   e x e c u t e d   * / 
 
     C F _ P l a y b a c k F i l e C m d _ t             P b F i l e C m d M s g ; 
 
         / *   r e s e t   C F   g l o b a l s   e t c   * / 
     C F _ A p p I n i t ( ) ;     
     
     / *   r e s e t   t h e   t r a n s a c t i o n s   s e q   n u m b e r   u s e d   b y   t h e   e n g i n e   * / 
     m i s c _ _ s e t _ t r a n s _ s e q _ n u m ( 1 ) ; 
     
     C F E _ S B _ I n i t M s g ( & P b F i l e C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ P l a y b a c k F i l e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ,   C F _ P L A Y B A C K _ F I L E _ C C ) ; 
 
     / *   l o w   p r i o r i t y   f i l e   * / 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   1 0 0 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 2 . 2 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / l o w e s t p r i o r i t y . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   e x e c u t e   t h e   p l a y b a c k   f i l e   c m d   t o   g e t   a   q u e u e   e n t r y   o n   t h e   p e n d i n g   q u e u e   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
     
     
         / *   n e x t   l o w   p r i o r i t y   f i l e   * / 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   7 0 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 2 . 2 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / l o w e r p r i o r i t y . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   e x e c u t e   t h e   p l a y b a c k   f i l e   c m d   t o   g e t   a   q u e u e   e n t r y   o n   t h e   p e n d i n g   q u e u e   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
     
     
     / *   n e x t   l o w   p r i o r i t y   f i l e   * / 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   5 0 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 2 . 2 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / l o w p r i o r i t y . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   e x e c u t e   t h e   p l a y b a c k   f i l e   c m d   t o   g e t   a   q u e u e   e n t r y   o n   t h e   p e n d i n g   q u e u e   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
     
     
         
     / *   h i g h   p r i o r i t y   f i l e   * / 
     P b F i l e C m d M s g . C l a s s   =   1 ; 
     P b F i l e C m d M s g . C h a n n e l   =   0 ; 
     P b F i l e C m d M s g . P r i o r i t y   =   5 ; 
     P b F i l e C m d M s g . P r e s e r v e   =   0 ; 
     s t r c p y ( P b F i l e C m d M s g . P e e r E n t i t y I d ,   " 1 . 2 5 " ) ; 
     s t r c p y ( P b F i l e C m d M s g . S r c F i l e n a m e ,   " / c f / h i p r i o r i t y . t x t " ) ; 
     s t r c p y ( P b F i l e C m d M s g . D s t F i l e n a m e ,   " g n d p a t h / " ) ; 
 
     / *   f o r c e   t h e   G e t P o o l B u f   c a l l   f o r   t h e   q u e u e   e n t r y   t o   r e t u r n   s o m e t h i n g   v a l i d   * / 
     U t _ C F E _ E S _ S e t F u n c t i o n H o o k ( U T _ C F E _ E S _ G E T P O O L B U F _ I N D E X ,   & C F E _ E S _ G e t P o o l B u f H o o k ) ; 
 
     / *   e x e c u t e   t h e   p l a y b a c k   f i l e   c m d   t o   g e t   a   q u e u e   e n t r y   o n   t h e   p e n d i n g   q u e u e   * / 
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & P b F i l e C m d M s g ) ; 
 
 
     / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . C h a n [ 0 ] . P b Q [ 0 ] . E n t r y C n t = = 4 , " E n t r y C n t B e f o r e   =   4 " ) ; 
     
 } / *   e n d   T e s t _ C F _ P b Q u e u e F r o n t G o o d   * / 
 
 
 v o i d   T e s t _ C F _ P b Q u e u e F r o n t I n v C h a n ( v o i d ) 
 { 
     i n t 3 2                       A c t R t n , E x p R t n ; 
     u i n t 8                       C h a n   =   1 0 ; 
     u i n t 3 2                     Q u e u e   =   0 ; 
     C F _ Q u e u e E n t r y _ t   * N o d e T o I n s e r t ; 
     
     C F _ A p p I n i t ( ) ; 
         
     N o d e T o I n s e r t   =   ( C F _ Q u e u e E n t r y _ t   * ) & C F _ A p p D a t a . M e m . P a r t i t i o n ; 
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ I n s e r t P b N o d e A t F r o n t ( C h a n , Q u e u e , N o d e T o I n s e r t ) ; 
     
         / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( A c t R t n = = E x p R t n , " A c t R t n = = E x p R t n " ) ; 
       
 } / *   e n d   T e s t _ C F _ P b Q u e u e F r o n t I n v C h a n   * / 
 
 
 v o i d   T e s t _ C F _ A d d U p Q u e u e I n v N e w N o d e ( v o i d ) 
 { 
     i n t 3 2                       A c t R t n , E x p R t n ; 
     u i n t 3 2                     Q u e u e   =   0 ; 
     
     C F _ A p p I n i t ( ) ; 
         
     E x p R t n   =   C F _ E R R O R ; 
     A c t R t n   =   C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   N U L L ) ; 
     
     / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( A c t R t n = = E x p R t n , " A c t R t n = = E x p R t n " ) ; 
       
 } / *   e n d   T e s t _ C F _ A d d U p Q u e u e I n v N e w N o d e   * / 
 
 
 v o i d   T e s t _ C F _ A d d U p Q u e u e S e c o n d N o d e ( v o i d ) 
 { 
     i n t 3 2                       A c t R t n , E x p R t n ; 
     u i n t 3 2                     Q u e u e   =   0 ; 
     C F _ Q u e u e E n t r y _ t   F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * F i r s t N o d e P t r   =   & F i r s t N o d e A d d e d ; 
     
     C F _ A p p I n i t ( ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   F i r s t N o d e P t r ) ; 
     
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   F i r s t N o d e P t r ) ; 
     
     / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( A c t R t n = = E x p R t n , " A c t R t n = = E x p R t n " ) ; 
       
 } / *   e n d   T e s t _ C F _ A d d U p Q u e u e S e c o n d N o d e   * / 
 
 
         
 v o i d   T e s t _ C F _ R e m o v e F i r s t U p N o d e ( v o i d ) 
 { 
     i n t 3 2                       A c t R t n , E x p R t n ; 
     u i n t 3 2                     Q u e u e   =   0 ; 
     C F _ Q u e u e E n t r y _ t   F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   S e c o n d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   T h i r d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * F i r s t N o d e P t r   =   & F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * S e c o n d N o d e P t r   =   & S e c o n d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * T h i r d N o d e P t r   =   & T h i r d N o d e A d d e d ;     
     
     C F _ A p p I n i t ( ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   F i r s t N o d e P t r ) ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   S e c o n d N o d e P t r ) ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   T h i r d N o d e P t r ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m U p Q u e u e ( Q u e u e ,   F i r s t N o d e P t r ) ; 
     
     / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( A c t R t n = = E x p R t n , " A c t R t n = = E x p R t n " ) ; 
       
 } / *   e n d   T e s t _ C F _ R e m o v e F i r s t U p N o d e   * / 
 
 
 v o i d   T e s t _ C F _ R e m o v e M i d d l e U p N o d e ( v o i d ) 
 { 
     i n t 3 2                       A c t R t n , E x p R t n ; 
     u i n t 3 2                     Q u e u e   =   0 ; 
     C F _ Q u e u e E n t r y _ t   F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   S e c o n d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   T h i r d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * F i r s t N o d e P t r   =   & F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * S e c o n d N o d e P t r   =   & S e c o n d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * T h i r d N o d e P t r   =   & T h i r d N o d e A d d e d ;     
     
     C F _ A p p I n i t ( ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   F i r s t N o d e P t r ) ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   S e c o n d N o d e P t r ) ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   T h i r d N o d e P t r ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m U p Q u e u e ( Q u e u e ,   S e c o n d N o d e P t r ) ; 
     
     / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( A c t R t n = = E x p R t n , " A c t R t n = = E x p R t n " ) ; 
       
 } / *   e n d   T e s t _ C F _ R e m o v e M i d d l e U p N o d e   * / 
 
 
 v o i d   T e s t _ C F _ R e m o v e L a s t U p N o d e ( v o i d ) 
 { 
     i n t 3 2                       A c t R t n , E x p R t n ; 
     u i n t 3 2                     Q u e u e   =   0 ; 
     C F _ Q u e u e E n t r y _ t   F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   S e c o n d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   T h i r d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * F i r s t N o d e P t r   =   & F i r s t N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * S e c o n d N o d e P t r   =   & S e c o n d N o d e A d d e d ; 
     C F _ Q u e u e E n t r y _ t   * T h i r d N o d e P t r   =   & T h i r d N o d e A d d e d ;     
         
     C F _ A p p I n i t ( ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   F i r s t N o d e P t r ) ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   S e c o n d N o d e P t r ) ; 
     C F _ A d d F i l e T o U p Q u e u e ( Q u e u e ,   T h i r d N o d e P t r ) ; 
         
     E x p R t n   =   C F _ S U C C E S S ; 
     A c t R t n   =   C F _ R e m o v e F i l e F r o m U p Q u e u e ( Q u e u e ,   T h i r d N o d e P t r ) ; 
     
     / *   v e r i f y   * / 
     U t A s s e r t _ T r u e ( A c t R t n = = E x p R t n , " A c t R t n = = E x p R t n " ) ; 
       
 } / *   e n d   T e s t _ C F _ R e m o v e L a s t U p N o d e   * / 
 
 
 
 v o i d   T e s t _ C F _ G i v e S e m I n v P a r a m C m d ( v o i d ) { 
 
     C F _ G i v e T a k e C m d _ t     C m d M s g ; 
     i n t 3 2                           C r e a t R t n ; 
     u i n t 3 2                         S e m I d ; 
     u i n t 3 2                         I n i t i a l V a l u e   =   5 ; 
     u i n t 3 2                         S e m O p t i o n s   =   0 ; 
 
     / *   S e t u p   I n p u t s   * / 
     C F E _ S B _ I n i t M s g ( & C m d M s g ,   C F _ C M D _ M I D ,   s i z e o f ( C F _ G i v e T a k e C m d _ t ) ,   T R U E ) ; 
     C F E _ S B _ S e t C m d C o d e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ,   C F _ G I V E T A K E _ C C ) ; 
     C m d M s g . C h a n   =   0 ; 
     C m d M s g . G i v e O r T a k e S e m a p h o r e   =   C F _ G I V E _ S E M A P H O R E ; / *   0   * / 
     C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 ; 
     C F _ A p p D a t a . H k . E r r C o u n t e r   =   0 ; 
     
     C r e a t R t n   =   O S _ C o u n t S e m C r e a t e   ( & S e m I d ,   " T e s t _ S e m a p h o r e " ,   I n i t i a l V a l u e ,   S e m O p t i o n s ) ; 
     
     C F _ A p p I n i t ( ) ; 
     
     / *   E x e c u t e   T e s t   * /     
     C F _ A p p D a t a . M s g P t r   =   ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ;     
     C F _ A p p P i p e ( ( C F E _ S B _ M s g P t r _ t ) & C m d M s g ) ; 
 
     / *   V e r i f y   O u t p u t s   * / 
     U t A s s e r t _ T r u e ( C r e a t R t n = = O S _ S U C C E S S , " C r e a t e   =   S U C C E S S " ) ; 
     U t A s s e r t _ T r u e ( U t _ C F E _ E V S _ G e t E v e n t Q u e u e D e p t h ( ) = = 1 0 , " E v e n t   C o u n t   =   1 0 " ) ; 
     U t A s s e r t _ E v e n t S e n t ( C F _ G I V E T A K E _ E R R 1 _ E I D , C F E _ E V S _ E R R O R ,   " " ,   " E r r o r   E v e n t   S e n t " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . C m d C o u n t e r   = =   0 ,   " C F _ A p p D a t a . H k . C m d C o u n t e r   =   0 " ) ; 
     U t A s s e r t _ T r u e ( C F _ A p p D a t a . H k . E r r C o u n t e r   = =   1 ,   " C F _ A p p D a t a . H k . E r r C o u n t e r   =   1 " ) ; 
 
 } / *   e n d   T e s t _ C F _ G i v e S e m a p h o r e C m d   * / 
 
 
 
 
 
 v o i d   C F _ S e t u p ( v o i d ) 
 {     
 
         C F E _ E S _ G e t P o o l B u f H o o k C a l l C n t   =   0 ; 
         U t _ O S A P I _ R e s e t ( ) ; 
         U t _ C F E _ S B _ R e s e t ( ) ; 
         U t _ C F E _ E S _ R e s e t ( ) ; 
         U t _ C F E _ E V S _ R e s e t ( ) ; 
         U t _ C F E _ T B L _ R e s e t ( ) ; 
         U t _ C F E _ T B L _ A d d T a b l e ( C F _ C O N F I G _ T A B L E _ F I L E N A M E ,   & C F _ C o n f i g T a b l e ) ; 
 
 } 
 
 v o i d   C F _ T e a r D o w n ( v o i d ) 
 { 
 
 } 
 
 / *   C F _ A d d T e s t C a s e   i s   l a s t   i n   t h e   f i l e   s o   I   d o n ' t   h a v e   t o   d e c l a r e   p r o t o t y p e s   f o r   e a c h   t e s t   m e t h o d   * / 
 v o i d   C F _ A d d T e s t C a s e ( v o i d ) 
 { 
 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ E V S R e g F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ E V S R e g F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ C r P i p e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ C r P i p e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ S u b 1 F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ S u b 1 F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ S u b 2 F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ S u b 2 F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ S u b 3 F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ S u b 3 F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ S u b 4 F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ S u b 3 F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ T b l R e g F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ T b l R e g F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ T b l L o a d F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ T b l L o a d F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ T b l M a n a g e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ T b l M a n a g e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ T b l G e t A d r F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ T b l G e t A d r F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ P o o l C r e a t e E x F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ P o o l C r e a t e E x F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ S e n d E v e n t F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ S e n d E v e n t F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p I n i t _ N o E r r o r s ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p I n i t _ N o E r r o r s " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l F l i g h t E n t i t y I d F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l F l i g h t E n t i t y I d F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l I n c o m i n g M s g I d F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l I n c o m i n g M s g I d F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l O u t g o i n g F i l e C h u n k F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l O u t g o i n g F i l e C h u n k F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l C h a n I n U s e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l C h a n I n U s e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l D e q u e E n a b l e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l D e q u e E n a b l e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l O u t g o i n g M s g I d F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l O u t g o i n g M s g I d F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P o l l D i r I n U s e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P o l l D i r I n U s e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P o l l E n a b l e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P o l l E n a b l e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P o l l C l a s s F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P o l l C l a s s F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P o l l P r e s e r v e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P o l l P r e s e r v e F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P o l l S r c P a t h F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P o l l S r c P a t h F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P o l l D s t P a t h F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P o l l D s t P a t h F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T b l V a l P e e r E n t i t y I d F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T b l V a l P e e r E n t i t y I d F a i l " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ A p p M a i n _ I n i t E r r o r s ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p M a i n _ A p p I n i t E r r o r s " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p M a i n _ S e m G e t I d B y N a m e F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p M a i n _ S e m G e t I d B y N a m e F a i l " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ A p p M a i n _ R c v M s g E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p M a i n _ R c v M s g E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A p p M a i n _ R c v M s g O k O n F i r s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A p p M a i n _ R c v M s g O k O n F i r s t " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ H o u s e k e e p i n g C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ H o u s e k e e p i n g C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ H k C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ H k C m d I n v L e n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ H k C m d T b l U p d a t e d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ H k C m d T b l U p d a t e d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ H k C m d V a l P e n d i n g ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ H k C m d V a l P e n d i n g " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ N o o p C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ N o o p C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ N o o p C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ N o o p C m d I n v L e n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ W a k e u p C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W a k e u p C m d " ) ; 
 / /         U t T e s t _ A d d ( T e s t _ C F _ W a k e u p C m d P o l l i n g E n a b l e d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W a k e u p C m d P o l l i n g E n a b l e d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W a k e u p C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W a k e u p C m d I n v L e n " ) ; 
 
 / /         U t T e s t _ A d d ( T e s t _ C F _ I n P D U N o E r r C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n P D U N o E r r C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ I n P D U T l m P k t C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n P D U T l m P k t C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ I n P D U H d r S i z e E r r C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n P D U H d r S i z e E r r C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ I n P D U T o o B i g C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n P D U T o o B i g C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ I n P D U T o o S m a l l C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n P D U T o o S m a l l C m d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ R s t C t r s C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R s t C t r s C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ R s t C t r s C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R s t C t r s C m d I n v L e n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ F r e e z e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ F r e e z e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ F r e e z e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ F r e e z e C m d I n v L e n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ T h a w C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T h a w C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ T h a w C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T h a w C m d I n v L e n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ S u s p e n d T r a n s I d C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S u s p e n d T r a n s I d C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S u s p e n d F i l e n a m e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S u s p e n d F i l e n a m e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S u s p e n d I n v F i l e n a m e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S u s p e n d I n v F i l e n a m e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S u s p e n d C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S u s p e n d C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S u s p e n d U n t e r m S t r g C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S u s p e n d U n t e r m S t r g C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S u s p e n d A l l C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S u s p e n d A l l C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ R e s u m e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R e s u m e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ R e s u m e A l l C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R e s u m e A l l C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ C a n c e l C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ C a n c e l C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ C a n c e l A l l C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ C a n c e l A l l C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A b a n d o n C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A b a n d o n C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A b a n d o n A l l C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A b a n d o n A l l C m d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b P a r a m C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b P a r a m C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b P a r a m C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b P a r a m C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d U n t e r m P a r a m ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d U n t e r m P a r a m " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d U n t e r m V a l u e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d U n t e r m V a l u e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d F i l e C h u n k O v e r L i m i t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d F i l e C h u n k O v e r L i m i t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d M y I d I n v a l i d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d M y I d I n v a l i d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d A c k L i m i t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d A c k L i m i t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d A c k T i m e o u t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d A c k T i m e o u t " ) ; 
 / /         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d I n a c t T i m e o u t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d I n a c t T i m e o u t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d N a k L i m i t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d N a k L i m i t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b C m d N a k T i m e o u t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b C m d N a k T i m e o u t " ) ; 
 / /         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b F i l e C h u n k S i z e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b F i l e C h u n k S i z e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t M i b M y I d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t M i b M y I d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ G e t M i b P a r a m C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G e t M i b P a r a m C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G e t M i b P a r a m C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G e t M i b P a r a m C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G e t M i b C m d U n t e r m P a r a m ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G e t M i b C m d U n t e r m P a r a m " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G e t M i b P a r a m C m d I n v P a r a m ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G e t M i b P a r a m C m d I n v P a r a m " ) ; 
                 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d C f g P a r a m s C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d C f g P a r a m s C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d C f g P a r a m s C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d C f g P a r a m s C m d I n v L e n " ) ; 
               
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e C m d C r e a t E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e C m d C r e a t E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e U p Q V a l u e E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e U p Q V a l u e E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e U p D e f F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e U p D e f F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e U p C u s t o m F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e U p C u s t o m F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e O u t Q V a l u e E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e O u t Q V a l u e E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e O u t Q T y p e E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e O u t Q T y p e E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e O u t C h a n E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e O u t C h a n E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e O u t D e f F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e O u t D e f F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e O n e E n t r y ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e O n e E n t r y " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e O u t C u s t o m F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e O u t C u s t o m F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e W r i t e H d r E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e W r i t e H d r E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e E n t r y W r i t e E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e E n t r y W r i t e E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e Q u e u e I n v F i l e n a m e E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e Q u e u e I n v F i l e n a m e E r r " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s D e f a u l t F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s D e f a u l t F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s C u s t F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s C u s t F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s C m d I n v F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s C m d I n v F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s C r e a t F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s C r e a t F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s W r H d r F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s W r H d r F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s I n v W h i c h Q s ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s I n v W h i c h Q s " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ W r i t e A c t T r a n s E n t r y W r i t e E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ W r i t e A c t T r a n s E n t r y W r i t e E r r " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ I n v C m d C o d e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n v C m d C o d e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ I n v M s g I d C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ I n v M s g I d C m d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d T r a n s D i a g C m d S u c c e s s ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d T r a n s D i a g C m d S u c c e s s " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d T r a n s D i a g F i l e N o t F o u n d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d T r a n s D i a g F i l e N o t F o u n d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d T r a n s D i a g T r a n s N o t F o u n d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d T r a n s D i a g T r a n s N o t F o u n d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d T r a n s D i a g C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d T r a n s D i a g C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d T r a n s D i a g U n t e r m S t r i n g ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d T r a n s D i a g U n t e r m S t r i n g " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e n d T r a n s D i a g I n v F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e n d T r a n s D i a g I n v F i l e n a m e " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v C h a n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v D i r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v D i r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v C l a s s ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v C l a s s " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v P r e s e r v e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v P r e s e r v e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v S r c ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v S r c " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v D s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v D s t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ S e t P o l l P a r a m I n v I d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ S e t P o l l P a r a m I n v I d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e T r a n s U n t e r m ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e T r a n s U n t e r m " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e I n v F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e I n v F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e F i l e N o t F o u n d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e F i l e N o t F o u n d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e I d N o t F o u n d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e I d N o t F o u n d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e U p A c t i v e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e U p A c t i v e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e U p H i s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e U p H i s t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e P b P e n d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e P b P e n d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e P b A c t i v e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e P b A c t i v e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e P b H i s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e P b H i s t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e P u t F a i l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e P u t F a i l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D e l e t e Q u e u e N o d e I n v T y p e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D e l e t e Q u e u e N o d e I n v T y p e " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e Q u e u e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e Q u e u e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e U p l i n k A c t i v e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e U p l i n k A c t i v e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e U p H i s t o r y ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e U p H i s t o r y " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e I n v U p Q ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e I n v U p Q " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e O u t A c t i v e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e O u t A c t i v e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e O u t P e n d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e O u t P e n d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e O u t H i s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e O u t H i s t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e I n v O u t Q ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e I n v O u t Q " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e I n v O u t C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e I n v O u t C h a n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P u r g e I n v T y p e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P u r g e I n v T y p e " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e D e q u e u e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e D e q u e u e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e D e q u e u e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e D e q u e u e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e D e q u e u e I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e D e q u e u e I n v C h a n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e D e q u e u e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e D e q u e u e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e D e q u e u e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e D e q u e u e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e D e q u e u e I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e D e q u e u e I n v C h a n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e D i r P o l l i n g C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e D i r P o l l i n g C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e D i r P o l l i n g C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e D i r P o l l i n g C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e P o l l i n g I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e P o l l i n g I n v C h a n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e P o l l i n g I n v D i r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e P o l l i n g I n v D i r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ E n a b l e P o l l i n g A l l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ E n a b l e P o l l i n g A l l " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e D i r P o l l i n g C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e P o l l i n g I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e P o l l i n g I n v C h a n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e P o l l i n g I n v D i r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e P o l l i n g I n v D i r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ D i s a b l e P o l l i n g A l l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ D i s a b l e P o l l i n g A l l " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ K i c k S t a r t C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ K i c k S t a r t C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ K i c k S t a r t C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ K i c k S t a r t C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ K i c k S t a r t C m d I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ K i c k S t a r t C m d I n v C h a n " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s F i l e n a m e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s F i l e n a m e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s T r a n s C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s T r a n s C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s A c t i v e T r a n s ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s A c t i v e T r a n s " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s A c t i v e N a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s A c t i v e N a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s A c t i v e S u s p e n d e d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s A c t i v e S u s p e n d e d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s U n t e r m S t r i n g ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s U n t e r m S t r i n g " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q u i c k S t a t u s I n v F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q u i c k S t a t u s I n v F i l e n a m e " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e N o M e m ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e N o M e m " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e C m d P a r a m E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e C m d P a r a m E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e C h a n N o t I n U s e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e C h a n N o t I n U s e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e I n v S r c F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e I n v S r c F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e I n v D s t F i l e n a m e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e I n v D s t F i l e n a m e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e P e n d Q F u l l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e P e n d Q F u l l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e I n v P e e r I d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e I n v P e e r I d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e F i l e O p e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e F i l e O p e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b F i l e F i l e O n Q ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b F i l e F i l e O n Q " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r C m d O p e n E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r C m d O p e n E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r C m d P a r a m E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r C m d P a r a m E r r " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r C h a n N o t I n U s e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r C h a n N o t I n U s e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r I n v S r c P a t h ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r I n v S r c P a t h " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r I n v D s t P a t h ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r I n v D s t P a t h " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b D i r I n v P e e r I d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b D i r I n v P e e r I d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ Q D i r F i l e s Q F u l l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q D i r F i l e s Q F u l l " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q D i r F i l e s N o M e m ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q D i r F i l e s N o M e m " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q D i r F i l e s F i l e O n Q ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q D i r F i l e s F i l e O n Q " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q D i r F i l e s F i l e O p e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q D i r F i l e s F i l e O p e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ Q D i r F i l e s A l l G o o d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ Q D i r F i l e s A l l G o o d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e R e m o v e F i r s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e R e m o v e F i r s t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e R e m o v e M i d d l e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e R e m o v e M i d d l e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e R e m o v e L a s t ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e R e m o v e L a s t " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e R e m o v e N u l l ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e R e m o v e N u l l " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e I n s e r t I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e I n s e r t I n v C h a n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e I n s e r t I n v Q ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e I n s e r t I n v Q " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e I n s e r t G o o d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e I n s e r t G o o d " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e F r o n t G o o d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e F r o n t G o o d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ P b Q u e u e F r o n t I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ P b Q u e u e F r o n t I n v C h a n " ) ; 
 
         / / i n t 3 2   C F _ A d d F i l e T o U p Q u e u e ( u i n t 3 2   Q u e u e ,   C F _ Q u e u e E n t r y _ t   * N e w N o d e ) 
         U t T e s t _ A d d ( T e s t _ C F _ A d d U p Q u e u e I n v N e w N o d e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A d d U p Q u e u e I n v N e w N o d e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ A d d U p Q u e u e S e c o n d N o d e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ A d d U p Q u e u e S e c o n d N o d e " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ R e m o v e F i r s t U p N o d e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R e m o v e F i r s t U p N o d e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ R e m o v e M i d d l e U p N o d e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R e m o v e M i d d l e U p N o d e " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ R e m o v e L a s t U p N o d e ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ R e m o v e L a s t U p N o d e " ) ; 
 
         U t T e s t _ A d d ( T e s t _ C F _ G i v e S e m I n v P a r a m C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G i v e S e m a p h o r e C m d " ) ; 
 # i f   0 
         U t T e s t _ A d d ( T e s t _ C F _ T a k e S e m a p h o r e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ T a k e S e m a p h o r e C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G i v e S e m a p h o r e C m d ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G i v e S e m I n v P a r a m C m d " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G i v e T a k e C m d I n v L e n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G i v e T a k e C m d I n v L e n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G i v e T a k e C m d I n v C h a n ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G i v e T a k e C m d I n v C h a n " ) ; 
         U t T e s t _ A d d ( T e s t _ C F _ G i v e T a k e C m d S e m E r r ,   C F _ S e t u p ,   C F _ T e a r D o w n ,   " T e s t _ C F _ G i v e T a k e C m d S e m E r r " ) ; 
 # e n d i f 
 
 
                 
 } 
 
 
     
 
 