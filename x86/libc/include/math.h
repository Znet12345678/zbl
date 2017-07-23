#ifndef __MATH_H
#define __MATH_H
#define M_E 2.71828
#define M_LOG10E 0.4342944819
#define M_LOG2E 1.44269504089
#define M_LN2 0.69314718056
#define M_LN10 2.30258509299
#define M_PI 3.14159265358979323
#define M_PI_2 1.57079632679
#define M_PI_4 0.78539816339
#define M_1_PI 0.31830988618
#define M_2_PI 0.63661977236
#define M_2_SQRTPI 1.1283791671
#define M_SQRT2 1.41421356237
#define M_SQRT1_2 0.70710678118
double acos(double);
double asin(double);
double atan(double);
double atan2(double,double);
double ceil(double);
double cos(double);
double cosh(double);
double exp(double);
double fabs(double);
double floor(double);
double fmod(double, double);
double frexp(double, int *);
double ldexp(double, int);
double log(double);
double log10(double);
double modf(double, double *);
double pow(double, double);
double sin(double);
double sinh(double);
double sqrt(double);
double tan(double);
double tanh(double);
double erf(double);
double erfc(double);
double gamma(double);
double hypot(double, double);
double j0(double);
double j1(double);
double jn(int, double);
double lgamma(double);
double y0(double);
double y1(double);
double yn(int, double);
int isnan(double);
double acosh(double);
double asinh(double);
double atanh(double);
double cbrt(double);
double expm1(double);
int ilogb(double);
double log1p(double);
double logb(double);
double nextafter(double, double);
double remainder(double, double);
double rint(double);
double scalb(double, double);
double ipow(double,int);
int fact(int n);
double sin(double);
#ifdef __TBUILD
int factT(int n);
double sinT(double n);
#endif
#endif
