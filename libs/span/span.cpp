#pragma once

#include "span.hpp"

#ifdef __AVX__
#define SPAN_SIMD_AVX_128
#endif

#ifdef __AVX2__
#define SPAN_SIMD_AVX_128
#define SPAN_SIMD_AVX_256
#endif

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#define SPAN_SIMD_NEON_128
#endif

#ifdef SPAN_SIMD_AVX_128
#include <immintrin.h>

/* defines */

namespace span
{
    using i128 = __m128i;

#ifdef SPAN_SIMD_AVX_256
    using i256 = __m256i;
#endif
}

#endif


#ifdef SPAN_SIMD_NEON_128
#include <arm_neon.h>
#endif

/* defines */

namespace span
{
    constexpr auto size32 = sizeof(u32);
    constexpr auto size64 = sizeof(u64);
    constexpr auto size128 = 2 * size64;
    constexpr auto size256 = 2 * size128;
    constexpr auto size512 = 2 * size256;
    constexpr auto size1024 = 2 * size512;
    
}


/* bit_copy simd */

namespace span
{
    static inline void bit_copy_u8_64(u8* src, u8* dst)
    {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
    }


    static inline void bit_copy_128(u8* src, u8* dst)
    {
        #ifdef SPAN_SIMD_AVX_128
        _mm_storeu_si128((i128*)dst, _mm_loadu_si128((i128*)src));

        #elif defined(SPAN_SIMD_NEON_128)
        vst1q_u8(dst, vld1q_u8(src));

        #else
        bit_copy_u8_64(src, dst);
        bit_copy_u8_64(src + size64, dst + size64);

        #endif
    }


    static inline void bit_copy_256(u8* src, u8* dst)
    {
        #ifdef SPAN_SIMD_AVX_256
        _mm256_storeu_si256((i256*)dst, _mm256_loadu_si256((i256*)src));

        #else
        bit_copy_128(src, dst);
        bit_copy_128(src + size128, dst + size128);

        #endif
    }
}


/* bit_fill simd */

namespace span
{
    static inline void bit_fill_u8_64(u8* dst, u8 value)
    {
        dst[0] = value;
        dst[1] = value;
        dst[2] = value;
        dst[3] = value;
        dst[4] = value;
        dst[5] = value;
        dst[6] = value;
        dst[7] = value;
    }


    static inline void bit_fill_u8_128(u8* dst, u8 value)
    {
        #ifdef SPAN_SIMD_AVX_128
        _mm_storeu_si128((i128*)dst, _mm_set1_epi8(value));

        #elif defined(SPAN_SIMD_NEON_128)
        vst1q_u8(dst, vdupq_n_u8(value));

        #else
        bit_fill_u8_64(dst, value);
        bit_fill_u8_64(dst + size64, value);

        #endif
    }


    static inline void bit_fill_u8_256(u8* dst, u8 value)
    {
        #ifdef SPAN_SIMD_AVX_256
        _mm256_storeu_si256((i256*)dst, _mm256_set1_epi8(value));
        #else
        bit_fill_u8_128(dst, value);
        bit_fill_u8_128(dst + size128, value);
        #endif
    }


    static inline void bit_fill_u32_128(u8* dst, u32 value)
    {
        #ifdef SPAN_SIMD_AVX_128
        _mm_storeu_si128((i128*)dst, _mm_set1_epi32(value));

        #elif defined(SPAN_SIMD_NEON_128)
        vst1q_u32((u32*)dst, vdupq_n_u32(value));

        #else
        ((i32*)dst)[0] = value;
        ((i32*)dst)[1] = value;
        ((i32*)dst)[2] = value;
        ((i32*)dst)[3] = value;
        #endif
    }


    static inline void bit_fill_u32_256(u8* dst, u32 value)
    {
        #ifdef SPAN_SIMD_AVX_256
        _mm256_storeu_si256((i256*)dst, _mm256_set1_epi32(value));
        #else
        bit_fill_u32_128(dst, value);
        bit_fill_u32_128(dst + size128, value);
        #endif
    }
    
}


/* bit_copy_64 */

namespace span
{
    static void bit_copy_64(u8* src, u8* dst, u32 n_u8)
    {
        switch (n_u8)
        {
        case 1:
            *dst = *src;
            return;

        case 2:
            *(u16*)dst = *(u16*)src;
            return;

        case 3:
            *(u16*)dst = *(u16*)src;
            dst[2] = src[2];
            return;

        case 4:
            *(u32*)dst = *(u32*)src;
            return;
        
        case 5:
            *(u32*)dst = *(u32*)src;
            dst[4] = src[4];
            return;
        
        case 6:
            *(u32*)dst = *(u32*)src;
            *(u16*)(dst + 4) = *(u16*)(src + 4);
            return;
        
        case 7:
            *(u32*)dst = *(u32*)src;
            *(u16*)(dst + 4) = *(u16*)(src + 4);
            dst[6] = src[6];
            return;

        case 8:
            *(u64*)dst = *(u64*)src;
            return;

        default:
            break;
        }
    }
}


/* bit_copy */

namespace span
{
    static inline void bit_copy_512(u8* src, u8* dst)
    {
        bit_copy_256(src, dst);
        bit_copy_256(src + size256, dst + size256);
    }


    static inline void bit_copy_1024(u8* src, u8* dst)
    {
        bit_copy_512(src, dst);
        bit_copy_512(src + size512, dst + size512);
    }
}


/* bit_fill */

namespace span
{

    static inline void bit_fill_u8_512(u8* dst, u8 value)
    {
        bit_fill_u8_256(dst, value);
        bit_fill_u8_256(dst + size256, value);
    }


    static inline void bit_fill_u8_1024(u8* dst, u8 value)
    {
        bit_fill_u8_512(dst, value);
        bit_fill_u8_512(dst + size512, value);
    }


    static inline void bit_fill_u32_512(u8* dst, u32 value)
    {              
        bit_fill_u32_256(dst, value);
        bit_fill_u32_256(dst + size256, value);
    }


    static inline void bit_fill_u32_1024(u8* dst, u32 value)
    {
        bit_fill_u32_512(dst, value);
        bit_fill_u32_512(dst + size512, value);
    }
}



/* copy */

namespace span
{
    static void copy_64(u8* src, u8* dst, u64 len_u8)
    {
        auto const len64 = len_u8 / size64;
        auto const src64 = (u64*)src;
        auto const dst64 = (u64*)dst;

        for (u64 i = 0; i < len64; ++i)
        {
            dst64[i] = src64[i];
        }

        auto const len8 = len_u8 - len64 * size64;
        auto const src8 = (u8*)(src64 + len64);
        auto const dst8 = (u8*)(dst64 + len64);

        bit_copy_64(src8, dst8, len8);
    }


    static void copy_128(u8* src, u8* dst, u64 len_u8)
    {
        auto const n128 = len_u8 / size128;
        auto const end128 = n128 * size128;

        u64 i = 0;

        for (; i < end128; i += size128)
        {
            bit_copy_128(src + i, dst + i);
        }

        i = len_u8 - size128;
        bit_copy_128(src + i, dst + i);
    }


    static void copy_256(u8* src, u8* dst, u64 len_u8)
    {
        auto const n256 = len_u8 / size256;
        auto const end256 = n256 * size256;

        u64 i = 0;

        for (; i < end256; i += size256)
        {            
            bit_copy_256(src + i, dst + i);
        }

        i = len_u8 - size256;
        bit_copy_256(src + i, dst + i);
    }


    static void copy_512(u8* src, u8* dst, u64 len_u8)
    {
        auto const n512 = len_u8 / size512;
        auto const end512 = n512 * size512;

        u64 i = 0;

        for(; i < end512; i += size512)
        {
            bit_copy_512(src + i, dst + i);
        }

        i = len_u8 - size512;
        bit_copy_512(src + i, dst + i);
    }


    static void copy_1024(u8* src, u8* dst, u64 len_u8)
    {
        auto const n1024 = len_u8 / size1024;
        auto const end1024 = n1024 * size1024;

        u64 i = 0;

        for(; i < end1024; i += size1024)
        {
            bit_copy_1024(src + i, dst + i);
        }

        i = len_u8 - size1024;
        bit_copy_1024(src + i, dst + i);
    }


    static void copy_64(u8* src, u8* dst1, u8* dst2, u64 len_u8)
    {
        auto const len64 = len_u8 / size64;
        auto const src64 = (u64*)src;
        auto const dst164 = (u64*)dst1;
        auto const dst264 = (u64*)dst2;

        for (u64 i = 0; i < len64; ++i)
        {
            dst164[i] = src64[i];
            dst264[i] = src64[i];
        }

        auto const len8 = len_u8 - len64 * size64;
        auto const src8 = (u8*)(src64 + len64);
        auto const dst18 = (u8*)(dst164 + len64);
        auto const dst28 = (u8*)(dst264 + len64);

        bit_copy_64(src8, dst18, len8);
        bit_copy_64(src8, dst28, len8);
    }


    static void copy_128(u8* src, u8* dst1, u8* dst2, u64 len_u8)
    {
        auto const n128 = len_u8 / size128;
        auto const end128 = n128 * size128;

        u64 i = 0;

        for (; i < end128; i += size128)
        {
            bit_copy_128(src + i, dst1 + i);
            bit_copy_128(src + i, dst2 + i);
        }

        i = len_u8 - size128;
        bit_copy_128(src + i, dst1 + i);
        bit_copy_128(src + i, dst2 + i);
    }


    static void copy_256(u8* src, u8* dst1, u8* dst2, u64 len_u8)
    {
        auto const n256 = len_u8 / size256;
        auto const end256 = n256 * size256;

        u64 i = 0;

        for (; i < end256; i += size256)
        {            
            bit_copy_256(src + i, dst1 + i);
            bit_copy_256(src + i, dst2 + i);
        }

        i = len_u8 - size256;
        bit_copy_256(src + i, dst1 + i);
        bit_copy_256(src + i, dst2 + i);
    }


    static void copy_512(u8* src, u8* dst1, u8* dst2, u64 len_u8)
    {
        auto const n512 = len_u8 / size512;
        auto const end512 = n512 * size512;

        u64 i = 0;

        for(; i < end512; i += size512)
        {
            bit_copy_512(src + i, dst1 + i);
            bit_copy_512(src + i, dst2 + i);
        }

        i = len_u8 - size512;
        bit_copy_512(src + i, dst1 + i);
        bit_copy_512(src + i, dst2 + i);
    }


    static void copy_1024(u8* src, u8* dst1, u8* dst2, u64 len_u8)
    {
        auto const n1024 = len_u8 / size1024;
        auto const end1024 = n1024 * size1024;

        u64 i = 0;

        for(; i < end1024; i += size1024)
        {
            bit_copy_1024(src + i, dst1 + i);
            bit_copy_1024(src + i, dst2 + i);
        }

        i = len_u8 - size1024;
        bit_copy_1024(src + i, dst1 + i);
        bit_copy_1024(src + i, dst2 + i);
    }
}


/* fill_u8 */

namespace span
{
    static void fill_u8_64(u8* dst, u8 value, u64 len_u8)
    {
        for (u32 i = 0; i < len_u8; i++)
        {
            dst[i] = value;
        }
    }


    static void fill_u8_128(u8* dst, u8 value, u64 len_u8)
    {
        auto const n128 = len_u8 / size128;
        auto const end128 = n128 * size128;

        u64 i = 0;

        for (; i < end128; i += size128)
        {
            bit_fill_u8_128(dst + i, value);
        }

        i = len_u8 - size128;
        bit_fill_u8_128(dst + i, value);
    }    


    static void fill_u8_256(u8* dst, u8 value, u64 len_u8)
    {
        auto const n256 = len_u8 / size256;
        auto const end256 = n256 * size256;

        u64 i = 0;

        for (; i < end256; i += size256)
        {
            bit_fill_u8_256(dst + i, value);
        }

        i = len_u8 - size256;
        bit_fill_u8_256(dst + i, value);
    }


    static void fill_u8_512(u8* dst, u8 value, u64 len_u8)
    {
        auto const n512 = len_u8 / size512;
        auto const end512 = n512 * size512;

        u64 i = 0;

        for (; i < end512; i += size512)
        {
            bit_fill_u8_512(dst + i, value);
        }

        i = len_u8 - size512;
        bit_fill_u8_512(dst + i, value);
    }


    static void fill_u8_1024(u8* dst, u8 value, u64 len_u8)
    {
        auto const n1024 = len_u8 / size1024;
        auto const end1024 = n1024 * size1024;

        u64 i = 0;

        for (; i < end1024; i += size1024)
        {
            bit_fill_u8_1024(dst + i, value);
        }

        i = len_u8 - size1024;
        bit_fill_u8_1024(dst + i, value);
    }
}


/* fill_u32 */

namespace span
{
    static void fill_u32_64(u32* dst, u32 value, u64 len_u32)
    {
        for (u32 i = 0; i < len_u32; i++)
        {
            dst[i] = value;
        }
    }


    static void fill_u32_128(u32* dst, u32 value, u64 len_u32)
    {
        auto const len_u8 = len_u32 * size32;
        auto const n128 = len_u8 / size128;
        auto const end128 = n128 * size128;        

        u8* d8 = (u8*)dst;

        u64 i = 0;

        for (; i < end128; i += size128)
        {
            bit_fill_u32_128(d8 + i, value);
        }

        i = len_u8 - size128;
        bit_fill_u32_128(d8 + i, value);
    }


    static void fill_u32_256(u32* dst, u32 value, u64 len_u32)
    {
        auto const len_u8 = len_u32 * size32;
        auto const n256 = len_u8 / size256;
        auto const end256 = n256 * size256;

        u8* d8 = (u8*)dst;

        u64 i = 0;

        for (; i < end256; i += size256)
        {
            bit_fill_u32_256(d8 + i, value);
        }

        i = len_u8 - size256;
        bit_fill_u32_256(d8 + i, value);
    }


    static void fill_u32_512(u32* dst, u32 value, u64 len_u32)
    {
        auto const len_u8 = len_u32 * size32;
        auto const n512 = len_u8 / size512;
        auto const end512 = n512 * size512;

        u8* d8 = (u8*)dst;

        u64 i = 0;

        for (; i < end512; i += size512)
        {
            bit_fill_u32_512(d8 + i, value);
        }

        i = len_u8 - size512;
        bit_fill_u32_512(d8 + i, value);
    }


    static void fill_u32_1024(u32* dst, u32 value, u64 len_u32)
    {
        auto const len_u8 = len_u32 * size32;
        auto const n1024 = len_u8 / size1024;
        auto const end1024 = n1024 * size1024;

        u8* d8 = (u8*)dst;

        u64 i = 0;

        for (; i < end1024; i += size1024)
        {
            bit_fill_u32_1024(d8 + i, value);
        }

        i = len_u8 - size1024;
        bit_fill_u32_1024(d8 + i, value);
    }

}


/* api */

namespace span
{
    void copy_u8(u8* src, u8* dst, u64 len_u8)
    {
        auto const n_64 = len_u8 / 8;

        switch (n_64)
        {
        case 0:
        case 1:
            copy_64(src, dst, len_u8);
            break;
        case 2:
        case 3:
            copy_128(src, dst, len_u8);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            copy_256(src, dst, len_u8);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            copy_512(src, dst, len_u8);
            break;
        default:
            copy_1024(src, dst, len_u8);
        }
    }


    void copy_u8(u8* src, u8* dst1, u8* dst2, u64 len_u8)
    {
        auto const n_64 = len_u8 / 8;

        switch (n_64)
        {
        case 0:
        case 1:
            copy_64(src, dst1, dst2, len_u8);
            break;
        case 2:
        case 3:
            copy_128(src, dst1, dst2, len_u8);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            copy_256(src, dst1, dst2, len_u8);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            copy_512(src, dst1, dst2, len_u8);
            break;
        default:
            copy_1024(src, dst1, dst2, len_u8);
        }
    }


    void fill_u8(u8* dst, u8 value, u64 len_u8)
    {
        auto const n_64 = len_u8 / 8;

        switch (n_64)
        {
        case 0:
        case 1:
            fill_u8_64(dst, value, len_u8);
            break;
        case 2:
        case 3:
            fill_u8_128(dst, value, len_u8);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            fill_u8_256(dst, value, len_u8);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            fill_u8_512(dst, value, len_u8);
            break;
        default:
            fill_u8_1024(dst, value, len_u8);
        }
    }


    void fill_u32(u32* dst, u32 value, u64 len_u32)
    {
        auto const n_64 = len_u32 / 2;

        switch (n_64)
        {
        case 0:
        case 1:
            fill_u32_64(dst, value, len_u32);
            break;
        case 2:
        case 3:
            fill_u32_128(dst, value, len_u32);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            fill_u32_256(dst, value, len_u32);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            fill_u32_512(dst, value, len_u32);
            break;
        default:
            fill_u32_1024(dst, value, len_u32);
        }
    }
}