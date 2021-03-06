
 / * 
   *   F i l e n a m e :   u t a s s e r t . h 
   * 
   *     C o p y r i g h t   ��  2 0 0 7 - 2 0 1 4   U n i t e d   S t a t e s   G o v e r n m e n t   a s   r e p r e s e n t e d   b y   t h e   
   *     A d m i n i s t r a t o r   o f   t h e   N a t i o n a l   A e r o n a u t i c s   a n d   S p a c e   A d m i n i s t r a t i o n .   
   *     A l l   O t h e r   R i g h t s   R e s e r v e d .     
   * 
   *     T h i s   s o f t w a r e   w a s   c r e a t e d   a t   N A S A ' s   G o d d a r d   S p a c e   F l i g h t   C e n t e r . 
   *     T h i s   s o f t w a r e   i s   g o v e r n e d   b y   t h e   N A S A   O p e n   S o u r c e   A g r e e m e n t   a n d   m a y   b e   
   *     u s e d ,   d i s t r i b u t e d   a n d   m o d i f i e d   o n l y   p u r s u a n t   t o   t h e   t e r m s   o f   t h a t   
   *     a g r e e m e n t . 
   * 
   *   P u r p o s e :   T h i s   f i l e   c o n t a i n s   a   s t a n d a r d   s e t   o f   a s s e r t s   f o r   u s e   i n   u n i t   t e s t s . 
   * 
   *   D e s i g n   N o t e s :   
   *         -   A l l   a s s e r t s   e v a l u a t e   a   e x p r e s s i o n   a s   T R U E   o r   F A L S E   t o   d e t e r m i n e   i f   a   u n i t   t e s t   h a s   
   *             p a s s e d   o r   f a i l e d .     T R U E   m e a n s   t h e   t e s t   p a s s e d ,   F A L S E   m e a n s   t h e   t e s t   f a i l e d . 
   *         -   A l l   a s s e r t s   r e t u r n   a   b o o l e n   r e s u l t   t o   i n d i c a t e   t h e   p a s s   f a i l   s t a t u s . 
   *         -   A l l   a s s e r t s   a r e   i m p l e m e n t e d   a s   m a c r o s   t o   h i d e   t h e   _ _ L I N E _ _   a n d   _ _ F I L E _ _   m a c r o s . 
   *         -   A l l   a s s e r t s   m u s t   c a l l   t h e   f u n c t i o n   U t A s s e r t . 
   * 
   *   R e f e r e n c e s : 
   * 
   * / 
 
 # i f n d e f   _ u t a s s e r t _ 
 # d e f i n e 	 _ u t a s s e r t _ 
 
 / * 
   *   I n c l u d e s 
   * / 
 
 # i n c l u d e   " c o m m o n _ t y p e s . h " 
 # i n c l u d e   " u t t o o l s . h " 
 # i n c l u d e   < s t r i n g . h > 
 # i n c l u d e   < s t d i o . h > 
 # i n c l u d e   < m a t h . h > 
 
 / * 
   *   M a c r o   D e f i n i t i o n s 
   * / 
 
 / *   E v a l u a t e s   a   e x p r e s s i o n   a s   e i t h e r   T R U E   o r   F A L S E .     T R U E   m e a n s   t h e   t e s t   p a s s e d ,   F A L S E   m e a n s   t h e   t e s t   f a i l e d .   * / 
 # d e f i n e           U t A s s e r t _ T r u e ( E x p r e s s i o n ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( E x p r e s s i o n ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   E v a l u a t e s   a   e x p r e s s i o n   a s   e i t h e r   T R U E   o r   F A L S E .     T R U E   m e a n s   t h e   t e s t   p a s s e d ,   F A L S E   m e a n s   t h e   t e s t   f a i l e d .   * / 
 # d e f i n e           U t A s s e r t _ B o o l ( E x p r e s s i o n ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( E x p r e s s i o n ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   A s s e r t s   a   t e s t   f a i l u r e   * / 
 # d e f i n e           U t A s s e r t _ F a i l e d ( D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( F A L S E ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   t w o   f l o a t i n g   p o i n t   n u m b e r s   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l   w i t h i n   a   s p e c i f i e d   a b s o l u t e   t o l e r a n c e .   * / 
 # d e f i n e           U t A s s e r t _ D o u b l e C m p A b s ( x ,   y ,   T o l e r a n c e ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( f a b s ( ( x )   -   ( y ) )   < =   ( T o l e r a n c e ) ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   t w o   f l o a t i n g   p o i n t   n u m b e r s   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l   w i t h i n   a   s p e c i f i e d   r e l a t i v e   t o l e r a n c e .   * / 
 # d e f i n e           U t A s s e r t _ D o u b l e C m p R e l ( x ,   y ,   R a t i o ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( f a b s ( ( x )   -   ( y ) ) / ( x )   < =   ( R a t i o ) ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   t w o   s t r i n g s   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l .   * / 
 # d e f i n e           U t A s s e r t _ S t r C m p ( S t r i n g 1 ,   S t r i n g 2 ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( s t r c m p ( S t r i n g 1 ,   S t r i n g 2 )   = =   0 ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   a t   m o s t   L e n g t h   c h a r a c t e r s   o f   t w o   s t r i n g s   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l .   * / 
 # d e f i n e           U t A s s e r t _ S t r n C m p ( S t r i n g 1 ,   S t r i n g 2 ,   L e n g t h ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( s t r n c m p ( S t r i n g 1 ,   S t r i n g 2 ,   L e n g t h )   = =   0 ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   t w o   r e g i o n s   o f   m e m o r y   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l .   * / 
 # d e f i n e           U t A s s e r t _ M e m C m p ( M e m o r y 1 ,   M e m o r y 2 ,   L e n g t h ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( m e m c m p ( M e m o r y 1 ,   M e m o r y 2 ,   L e n g t h )   = =   0 ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   a   r e g i o n   o f   m e m o r y   t o   a   s t a t i c   p a t t e r n   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l .     N o t e :   U s e   U t M e m S e t   t o 
   *   f i l l   a   r e g i o n   o f   m e m o r y   w i t h   a   s t a t i c   p a t t e r n .   * / 
 # d e f i n e           U t A s s e r t _ M e m C m p V a l u e ( M e m o r y ,   V a l u e ,   L e n g t h ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( U t M e m C m p V a l u e ( M e m o r y ,   V a l u e ,   L e n g t h ) ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   a   r e g i o n   o f   m e m o r y   t o   a   b y t e   c o u n t   p a t t e r n   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l .     N o t e :   U s e   U t M e m F i l l   t o 
   *   f i l l   a   r e g i o n   o f   m e m o r y   w i t h   a   b y t e   c o u n t   p a t t e r n .   * / 
 # d e f i n e           U t A s s e r t _ M e m C m p C o u n t ( M e m o r y ,   L e n g t h ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( U t M e m C m p C o u n t ( M e m o r y ,   L e n g t h ) ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / *   C o m p a r e s   a   r e g i o n   o f   m e m o r y   w i t h   t h e   c o n t e n t s   o f   a   b i n a r y   f i l e   a n d   d e t e r m i n e s   i f   t h e y   a r e   e q u a l .     N o t e :   U s e 
   *   U t M e m 2 B i n F i l e   t o   c o p y   a   r e g i o n   o f   m e m o r y   t o   a   b i n a r y   f i l e .   * / 
 # d e f i n e           U t A s s e r t _ M e m 2 B i n F i l e C m p ( M e m o r y ,   F i l e n a m e ,   D e s c r i p t i o n )   \ 
                                 U t A s s e r t ( ( U t M e m 2 B i n F i l e C m p ( M e m o r y ,   F i l e n a m e ) ) ,   D e s c r i p t i o n ,   _ _ F I L E _ _ ,   _ _ L I N E _ _ ) 
 
 / * 
   *   E x p o r t e d   F u n c t i o n s 
   * / 
 
 / *   R e t u r n s   t h e   n u m b e r   o f   a s s e r t s   t h a t   h a v e   p a s s e d .   * / 
 u i n t 3 2             U t A s s e r t _ G e t P a s s C o u n t ( v o i d ) ; 
 
 / *   R e t u r n s   t h e   n u m b e r   o f   a s s e r t s   t h a t   h a v e   f a i l e d .   * / 
 u i n t 3 2             U t A s s e r t _ G e t F a i l C o u n t ( v o i d ) ; 
 
 / *   B a s e   a s s e r t   f u n c t i o n .     A l l   a s s e r t s   m u s t   c a l l   t h i s   f u n c t i o n .   * / 
 b o o l e a n           U t A s s e r t ( b o o l e a n   E x p r e s s i o n ,   c o n s t   c h a r   * D e s c r i p t i o n ,   c o n s t   c h a r   * F i l e ,   u i n t 3 2   L i n e ) ; 
 
 # e n d i f 
 