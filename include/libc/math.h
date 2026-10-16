#pragma once

// Minimal <math.h> for building library code (Lua) for aarch64-none-elf without a C
// library. Declarations only. In C++ the float overloads mirror libc++'s <cmath>, which
// is what makes float lua_Number code call acosf/sqrtf and use single-precision ops.

#ifdef __cplusplus
extern "C" {
#endif

#define HUGE_VAL (__builtin_huge_val())
#define HUGE_VALF (__builtin_huge_valf())

double fabs(double);
double floor(double);
double ceil(double);
double sqrt(double);
double fmod(double, double);
double modf(double, double*);
double frexp(double, int*);
double ldexp(double, int);
double pow(double, double);
double exp(double);
double log(double);
double log10(double);
double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan(double);
double atan2(double, double);
double sinh(double);
double cosh(double);
double tanh(double);

float fabsf(float);
float floorf(float);
float ceilf(float);
float sqrtf(float);
float fmodf(float, float);
float modff(float, float*);
float frexpf(float, int*);
float ldexpf(float, int);
float powf(float, float);
float expf(float);
float logf(float);
float log10f(float);
float sinf(float);
float cosf(float);
float tanf(float);
float asinf(float);
float acosf(float);
float atanf(float);
float atan2f(float, float);
float sinhf(float);
float coshf(float);
float tanhf(float);

#ifdef __cplusplus
}

inline float fabs(float x) { return __builtin_fabsf(x); }
inline float floor(float x) { return __builtin_floorf(x); }
inline float ceil(float x) { return __builtin_ceilf(x); }
inline float sqrt(float x) { return __builtin_sqrtf(x); }
inline float fmod(float x, float y) { return fmodf(x, y); }
inline float modf(float x, float* i) { return modff(x, i); }
inline float frexp(float x, int* e) { return frexpf(x, e); }
inline float ldexp(float x, int e) { return ldexpf(x, e); }
inline float pow(float x, float y) { return powf(x, y); }
inline float exp(float x) { return expf(x); }
inline float log(float x) { return logf(x); }
inline float log10(float x) { return log10f(x); }
inline float sin(float x) { return sinf(x); }
inline float cos(float x) { return cosf(x); }
inline float tan(float x) { return tanf(x); }
inline float asin(float x) { return asinf(x); }
inline float acos(float x) { return acosf(x); }
inline float atan(float x) { return atanf(x); }
inline float atan2(float y, float x) { return atan2f(y, x); }
inline float sinh(float x) { return sinhf(x); }
inline float cosh(float x) { return coshf(x); }
inline float tanh(float x) { return tanhf(x); }
#endif
