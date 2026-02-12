#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

#if defined(_WIN32) || defined(_WIN64)
  #ifdef maths_EXPORTS
    #define MATHSLIB_API __declspec(dllexport)
  #else
    #define MATHSLIB_API __declspec(dllimport)
  #endif
#else
  #define MATHSLIB_API
#endif

MATHSLIB_API int add(int a, int b);

#endif
