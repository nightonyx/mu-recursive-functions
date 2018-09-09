//@author RossKRE:
//../tests/performance/functions.mu

fun sum(a, b) = a + b;

fun mul(a, b) = a * b;

fun sqr(a) = mul(a, a);

fun sqrSum(a, b) = sqr(b) + sqr(a);

fun countHypotenuse(a, b) = sqrt(sum(sqr(a), sqr(b)));

fun smthExpr(alpha, beta, gamma) = 2.0 * 7.0 / alpha + 6.0 + sqrt(beta) * gamma;

fun smthExpr2(a, b, c, x) = (a * sqr(x)) + b * x + c;

//Native functions:
//Sqrt passed!
//Sin passed!
//Cos passed!
//Log passed!