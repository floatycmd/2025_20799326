#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

#if defined(_WIN32) || defined(_WIN64)

  // If building/using STATIC library, no import/export decoration needed
  #ifdef maths_STATIC
    #define MATHSLIB_API
  #else
    #ifdef maths_EXPORTS
      #define MATHSLIB_API __declspec(dllexport)
    #else
      #define MATHSLIB_API __declspec(dllimport)
    #endif
  #endif

#else
  #define MATHSLIB_API
#endif

MATHSLIB_API int add(int a, int b);

#endif
