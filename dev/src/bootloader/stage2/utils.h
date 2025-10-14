#pragma once
#if defined(__32BIT__)
    #define uint_t uint32_t
    #define size_t uint_t
#else
    #define uint_t uint64_t
    #define size_t uint_t
#endif