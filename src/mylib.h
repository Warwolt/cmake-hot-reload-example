#pragma once

#ifdef _DEBUG
    #define MYLIB_API __declspec(dllexport)
#else
    #define MYLIB_API
#endif

extern "C" MYLIB_API void hello(int num);
extern "C" MYLIB_API int next_number(int num);
