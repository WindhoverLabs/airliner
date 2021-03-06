
 / * 
   *   F i l e n a m e :   u t t e s t . c 
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
   *   P u r p o s e :   T h i s   f i l e   c o n t a i n s   f u n c t i o n s   t o   i m p l e m e n t   a   s t a n d a r d   w a y   t o   e x e c u t e   u n i t   t e s t s . 
   * 
   * / 
 
 / * 
   *   I n c l u d e s 
   * / 
 
 # i n c l u d e   " c o m m o n _ t y p e s . h " 
 # i n c l u d e   " u t a s s e r t . h " 
 # i n c l u d e   " u t l i s t . h " 
 
 / * 
   *   T y p e   D e f i n i t i o n s 
   * / 
 
 t y p e d e f   s t r u c t   { 
         v o i d         ( * T e s t ) ( v o i d ) ; 
         v o i d         ( * S e t u p ) ( v o i d ) ; 
         v o i d         ( * T e a r d o w n ) ( v o i d ) ; 
         c h a r           * T e s t N a m e ; 
 }   U t T e s t D a t a B a s e E n t r y _ t ; 
 
 / * 
   *   L o c a l   D a t a 
   * / 
 
 U t L i s t H e a d _ t         U t T e s t D a t a B a s e ; 
 u i n t 3 2                     U t T e s t s E x e c u t e d C o u n t   =   0 ; 
 
 / * 
   *   F u n c t i o n   D e f i n i t i o n s 
   * / 
 
 v o i d   U t T e s t _ A d d ( v o i d   ( * T e s t ) ( v o i d ) ,   v o i d   ( * S e t u p ) ( v o i d ) ,   v o i d   ( * T e a r d o w n ) ( v o i d ) ,   c o n s t   c h a r   * T e s t N a m e ) 
 { 
         U t T e s t D a t a B a s e E n t r y _ t       U t T e s t D a t a B a s e E n t r y ; 
 
         U t T e s t D a t a B a s e E n t r y . T e s t   =   T e s t ; 
         U t T e s t D a t a B a s e E n t r y . S e t u p   =   S e t u p ; 
         U t T e s t D a t a B a s e E n t r y . T e a r d o w n   =   T e a r d o w n ; 
         U t T e s t D a t a B a s e E n t r y . T e s t N a m e   =   T e s t N a m e ; 
         U t L i s t _ A d d ( & U t T e s t D a t a B a s e ,   & U t T e s t D a t a B a s e E n t r y ,   s i z e o f ( U t T e s t D a t a B a s e E n t r y _ t ) ,   0 ) ; 
 } 
 
 i n t   U t T e s t _ R u n ( v o i d ) 
 { 
         u i n t 3 2                                       i ; 
         U t L i s t N o d e _ t                         * U t L i s t N o d e ; 
         U t T e s t D a t a B a s e E n t r y _ t       * U t T e s t D a t a B a s e E n t r y ; 
         
         i f   ( U t T e s t D a t a B a s e . N u m b e r O f E n t r i e s   >   0 )   { 
                 
                 U t L i s t N o d e   =   U t T e s t D a t a B a s e . F i r s t ; 
                 f o r   ( i = 0 ;   i   <   U t T e s t D a t a B a s e . N u m b e r O f E n t r i e s ;   i + + )   { 
                         
                         U t T e s t D a t a B a s e E n t r y   =   U t L i s t N o d e - > D a t a ; 
 
                         # i f d e f   U T _ V E R B O S E 
                         i f   ( s t r l e n ( U t T e s t D a t a B a s e E n t r y - > T e s t N a m e )   >   0 )   {   p r i n t f ( " \ n R u n n i n g   T e s t :   % s \ n " ,   U t T e s t D a t a B a s e E n t r y - > T e s t N a m e ) ;   } 
                         # e n d i f 
 
                         i f   ( U t T e s t D a t a B a s e E n t r y - > S e t u p )         {   U t T e s t D a t a B a s e E n t r y - > S e t u p ( ) ;   } 
                         i f   ( U t T e s t D a t a B a s e E n t r y - > T e s t )           {   U t T e s t D a t a B a s e E n t r y - > T e s t ( ) ;   U t T e s t s E x e c u t e d C o u n t + + ;   } 
                         i f   ( U t T e s t D a t a B a s e E n t r y - > T e a r d o w n )   {   U t T e s t D a t a B a s e E n t r y - > T e a r d o w n ( ) ;   } 
 
                         U t L i s t N o d e   =   U t L i s t N o d e - > N e x t ; 
                 } 
         } 
 
         p r i n t f ( " \ n " ) ; 
         p r i n t f ( " T e s t s   E x e c u t e d :         % l u \ n " ,   U t T e s t s E x e c u t e d C o u n t ) ; 
         p r i n t f ( " A s s e r t   P a s s   C o u n t :   % l u \ n " ,   U t A s s e r t _ G e t P a s s C o u n t ( ) ) ; 
         p r i n t f ( " A s s e r t   F a i l   C o u n t :   % l u \ n " ,   U t A s s e r t _ G e t F a i l C o u n t ( ) ) ; 
         
         U t L i s t _ R e s e t ( & U t T e s t D a t a B a s e ) ; 
 
         r e t u r n   ( U t A s s e r t _ G e t F a i l C o u n t ( )   >   0 ) ; 
 } 
 