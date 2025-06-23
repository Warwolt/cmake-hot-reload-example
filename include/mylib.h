#pragma once

#ifdef _DEBUG
    #ifdef EXPORT_MYLIB
        #define MYLIB_API __declspec(dllexport)
    #else
        #define MYLIB_API __declspec(dllimport)
    #endif
#else
    #define MYLIB_API
#endif

extern "C" MYLIB_API void hello();
