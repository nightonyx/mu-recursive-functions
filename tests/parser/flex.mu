//@author RossKRE:
//../tests/parser/flex.mu

fun
getTwo    ()
=      2.0    ; fun
sum
(a    , b) =     a
 +
 b;

 fun
 sqr(x)=x *x;

fun
sqr( x)= x*x;fun biEq(x,
a,     b, c
)= a*sqr(       x      )+
 b* x
  + c

 ; fun
getDiscriminant(     a,      b,   c
) = sqr(b) -
4.0 *
 a * c
 ;fun
   getPositiveRoot    (a    , b,     c)
   = (  sqrt (
   sqr(b) - 4.0 * a * c
   ) - b) / (getTwo(     )
    * a)
   ;
