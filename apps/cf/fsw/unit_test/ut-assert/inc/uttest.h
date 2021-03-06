
 / * 
   *   F i l e n a m e :   u t t e s t . h 
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
   *   P u r p o s e :   T h i s   f i l e   c o n t a i n s   f u n c t i o n s   t o   i m p l e m e n t   a   s t a n d a r d   w a y   t o   e x e c u t e   u n i t   t e s t s . 
   * 
   *   D e s i g n   N o t e s :   
   *         B y   d e f a u l t   t h e   o n l y   o u t p u t   t h a t   i s   p r i n t e d   t o   t h e   c o n s o l e   i s   a s s e r t   f a i l u r e s 
   *         a n d   a   s u m m a r y   o f   t h e   t e s t   r e s u l t s   a f t e r   a l l   t e s t s   h a v e   e x e c u t e d .     T o   e n a b l e   a d d i t i o n a l   
   *         t e s t   o u t p u t   d e f i n e   t h e   m a c r o   U T _ V E R B O S E . 
   * 
   *   R e f e r e n c e s : 
   * 
   * / 
 
 # i f n d e f   _ u t t e s t _ 
 # d e f i n e 	 _ u t t e s t _ 
 
 / * 
   *   E x p o r t e d   F u n c t i o n s 
   * / 
 
 / *   A d d s   a   n e w   u n i t   t e s t   t o   t h e   t e s t   d a t a b a s e .   * / 
 v o i d         U t T e s t _ A d d ( v o i d   ( * T e s t ) ( v o i d ) ,   v o i d   ( * S e t u p ) ( v o i d ) ,   v o i d   ( * T e a r d o w n ) ( v o i d ) ,   c o n s t   c h a r   * T e s t N a m e ) ; 
 
 / *   E x e c u t e s   a l l   u n i t   t e s t s   c o n t a i n e d   i n   t h e   t e s t   d a t a b a s e .     O n c e   a l l   t e s t s   h a v e   f i n i s h e d   e x e c u t i n g   
   *   a   r e s u l t s   s u m m a r y   i s   p r i n t e d   t o   t h e   c o n s o l e   a n d   t h e   t e s t   d a t a b a s e   i s   d e l e t e d .     T h i s   f u n c t i o n   a l s o   
   *   r e t u r n s   a   b o o l e a n   s t a t u s   i n d i c a t i n g   i f   a n y   o f   t h e   t e s t s   f a i l e d .   ( T R U E   =   a t   l e a s t   o n e   t e s t   f a i l u r e   
   *   h a s   o c c u r r e d ,   F A L S E   =   a l l   t e s t s   p a s s e d )   * / 
 i n t           U t T e s t _ R u n ( v o i d ) ; 
 
 # e n d i f 
 
 