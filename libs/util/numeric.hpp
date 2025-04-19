#pragma once

#include "types.hpp"

//#define PLATFORM_RPI

#ifdef PLATFORM_RPI
#ifndef NUMERIC_CMATH
#define NUMERIC_CMATH
#endif
#endif


/* constexpr */

namespace numeric
{
namespace cxpr
{
    template <typename T>
    inline constexpr T min(T a, T b)
    {
        return a < b ? a : b;
    }


    template <typename T>
    inline constexpr T max(T a, T b)
    {
        return a > b ? a : b;
    }


    template <typename T>
    inline constexpr bool is_unsigned()
    {
        return (T)((T)0 - (T)1) > (T)0;
    }


    template <typename T>
    inline constexpr T clamp(T value, T min, T max)
    {
        const T t = value < min ? min : value;
        return t > max ? max : t;
    }


    template <typename T, typename U>
    inline constexpr T clamp(T value, U min, U max)
    {
        const T t = value < min ? (T)min : value;
        return t > max ? (T)max : t;
    }


    template <typename R, typename T>
    inline constexpr R sign(T value)
    {
        static_assert(!is_unsigned<T>());

        constexpr R P = (R)1;
        constexpr R N = (R)-1;
        constexpr R Z = (R)0;

        return value ? ((f32)value < 0.0f ? N : P) : Z;
    }


    template <typename T>
    inline constexpr f32 sign_f32(T value)
    {           
        return sign<f32, T>(value);
    }


    template <typename T>
    inline constexpr i8 sign_i8(T value)
    {
        return sign<i8, T>(value);
    }


    template <typename T>
    inline constexpr T round_to_unsigned(f32 value)
    {
        static_assert(is_unsigned<T>());

        return (T)(value + 0.5f);
    }


    template <typename T>
    inline constexpr T round_to_signed(f32 value)
    {
        static_assert(!is_unsigned<T>());

        return (T)(value + sign<f32, f32>(value) * 0.5f);
    }


    template <typename T>
    inline constexpr T round_to_signed(f64 value)
    {
        static_assert(!is_unsigned<T>());

        return (T)(value + sign<f64, f64>(value) * 0.5);
    }


    inline constexpr f32 pow(f32 base, u32 exp)
    {
        f32 val = 1.0f;
        for (u32 i = 0; i < exp; i++)
        {
            val *= base;
        }

        return val;
    }


    template <u32 N>
    inline constexpr f32 round(f32 value)
    {
        constexpr auto f = pow(10.0f, N);
        constexpr auto i_f = 1.0f / f;

        return round_to_signed<i32>(value * f) * i_f;
    }


    template <typename T>
    inline constexpr T floor(T value)
    { 
        return (T)round_to_signed<i64>(value - 0.5f);
    }


    template <typename T>
    inline constexpr T ceil(T value)
    { 
        auto f = floor(value);
        return (T)(f + (f != value));
    }


    template <typename T>
    inline constexpr T abs(T value)
    {
        return sign<T, T>(value) * value;
    }


    template <typename uT>
    inline constexpr uT unsigned_max()
    {
        static_assert(is_unsigned<uT>());

        return (uT)((uT)0 - (uT)1);
    }


    template <typename uT>
    inline constexpr uT scale_to_unsigned(f32 value)
    {
        static_assert(is_unsigned<uT>());

        constexpr f32 max = (f32)unsigned_max<uT>() + 1.0f;

        auto s = sign_f32(value);

        value -= s * (u64)value;
        value = s > 0.0f ? value : (1.0f - value);

        return round_to_unsigned<uT>(max * value);
    }


    template <typename uSrc, typename uDst>
    inline constexpr uDst scale_unsigned(uSrc value)
    {
        static_assert(is_unsigned<uSrc>());
        static_assert(is_unsigned<uDst>());

        constexpr f32 i_smax = 1.0f / ((f32)unsigned_max<uSrc>() + 1.0f);
        constexpr f32 dmax = (f32)unsigned_max<uDst>() + 1.0f;

        return round_to_unsigned<uDst>(dmax * value * i_smax);
    }


    template <typename T>
    inline constexpr T inc_wrap(T value, T min, T max)
    {
        ++value;
        return value < min || value > max ? min : value;
    }


    inline constexpr bool is_power_of_2(u64 num)
    {
        return (num && !(num & (num - 1)));
    }

}


/* trig */

static constexpr f64 PI = 3.14159265358979323846;

namespace cxpr
{
    template <typename T>
    inline constexpr T deg_to_rad(T deg)
    {
        constexpr auto scale = PI / 180.0;
        return (T)(deg * scale);
    }


    template <typename T>
    inline constexpr T rad_to_deg(T rad)
    {
        constexpr auto scale = 180.0 / PI;
        return (T)(rad * scale);
    }


    template <typename uT>
    inline constexpr f32 max_angle_f32()
    {
        static_assert(is_unsigned<uT>());

        return (f32)unsigned_max<uT>() + 1.0f;
    }


    template <typename uT>
    inline constexpr u64 max_angle_u64()
    {
        static_assert(is_unsigned<uT>());

        return (u64)unsigned_max<uT>() + 1u;
    }


    template <typename uT>
    inline constexpr f32 unsigned_to_rad(uT a)
    {
        static_assert(is_unsigned<uT>());

        constexpr f32 max = max_angle_f32<uT>();
        constexpr auto scale = 2 * PI / max;

        return (f32)(a * scale);
    }


    template <typename uT>
    inline constexpr uT rad_to_unsigned(f32 rad)
    {
        static_assert(is_unsigned<uT>());

        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 max = max_angle_f32<uT>();

        auto n = rad / TP;
        auto f = n - (i64)n;
        f = f < 0.0f ? f + 1.0f : f;

        return round_to_unsigned<uT>(max * f);
    }


    inline constexpr f32 u16_to_rad(u16 a)
    {
        return unsigned_to_rad(a);
    }


    inline constexpr f32 u8_to_rad(u8 a)
    {
        return unsigned_to_rad(a);
    }


    template <typename T>
    inline constexpr T sin_approx(T rad)
    {
        // best for small angles e.g. 0 - 45deg

        constexpr T B = (T)(4.0) / (T)(PI);
        constexpr T C = (T)(-4.0) / ((T)(PI * PI));
        constexpr T P = (T)(0.225);

        T y = B * rad + C * rad * abs(rad);
        y = P * (y * abs(y) - y) + y;

        return y;
    }


    template <typename T>
    inline constexpr T cos_approx(T rad)
    {
        // best for small angles e.g. 0 - 45deg

        constexpr T tp = (T)(1.0) / (T)(2 * PI);

        T x = rad * tp;

        x -= (T)(0.25) + floor(x + T(0.25));
        x *= (T)(16.0) * (abs(x) - (T)(0.5));
        x += (T)(0.225) * x * (abs(x) - (T)(1.0));

        return x;
    }


    inline constexpr f32 atan_approx(f32 tan)
    {
        f32 sq = tan * tan;

        constexpr f32 a1  =  0.99997726f;
        constexpr f32 a3  = -0.33262347f;
        constexpr f32 a5  =  0.19354346f;
        constexpr f32 a7  = -0.11643287f;
        constexpr f32 a9  =  0.05265332f;
        constexpr f32 a11 = -0.01172120f;        

        return tan * (a1 + sq * (a3 + sq * (a5 + sq * (a7 + sq * (a9 + sq * a11)))));
    }


    inline constexpr f32 sin(uangle a)
    {
        static_assert(sizeof(uangle) <= sizeof(u32));

        constexpr f32 P = (f32)PI;
        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 HP = (f32)(PI / 2);

        // split full rotation into 8 x 45deg sections
        constexpr u64 max = max_angle_u64<uangle>();
        constexpr u64 oct = max / 8;
        
        auto rad = unsigned_to_rad(a);

        switch (a / oct)
        {
            case 0: return sin_approx(rad);
            case 1: return cos_approx(HP - rad);
            case 2: return cos_approx(rad - HP);
            case 3: return sin_approx(P - rad);
            case 4: return -sin_approx(rad - P);
            case 5: return -cos_approx(P + HP - rad);
            case 6: return -cos_approx(rad - (P + HP));
            case 7: return -sin_approx(TP - rad);
            default: return 0.0f;
        }
    } 


    inline constexpr f32 cos(uangle a)
    {
        static_assert(sizeof(uangle) <= sizeof(u32));

        constexpr f32 P = (f32)PI;
        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 HP = (f32)(PI / 2);

        // split full rotation into 8 x 45deg sections
        constexpr u64 max = max_angle_u64<uangle>();
        constexpr u64 oct = max / 8;
        
        auto rad = unsigned_to_rad(a);

        switch (a / oct)
        {
            case 0: return cos_approx(rad);
            case 1: return sin_approx(HP - rad);
            case 2: return -sin_approx(rad - HP);
            case 3: return -cos_approx(P - rad);
            case 4: return -cos_approx(rad - P);
            case 5: return -sin_approx(P + HP - rad);
            case 6: return sin_approx(rad - (P + HP));
            case 7: return cos_approx(TP - rad);
            default: return 0.0f;
        }
    }   


    inline constexpr f32 sin(f32 rad)
    {
        return sin(rad_to_unsigned<uangle>(rad));
    }


    inline constexpr f32 cos(f32 rad)
    {
        return cos(rad_to_unsigned<uangle>(rad));
    }


    inline constexpr uangle atan2(f32 sin, f32 cos)
    { 
        constexpr f32 P = (f32)PI;
        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 HP = (f32)(PI / 2);
        constexpr f32 QP = (f32)(PI / 4);

        assert(abs((cos * cos + sin * sin) - 1.0f) < 0.001f);

        auto pcos = abs(cos);
        auto psin = abs(sin);

        auto flip_45 = pcos < psin;
        auto flip_y = cos < 0.0f;
        auto flip_x = sin < 0.0f;

        auto key_y = (int)flip_y << 2;
        auto key_x = (int)flip_x << 1;
        auto key_45 = (int)flip_45;

        int oct_key = key_y | key_x | key_45;

        auto tan = flip_45 ? pcos / psin : psin / pcos;
        auto rad = atan_approx(tan);
        
        assert(rad >= 0.0f);
        assert(rad <= QP);

        switch (oct_key)
        {
        case 0b000:
            // octant 0
            break;

        case 0b001:
            rad = HP - rad; // octant 1
            break;

        case 0b101:
            rad = HP + rad; // octant 2
            break;

        case 0b100:
            rad = P - rad; // octant 3
            break;

        case 0b110:
            rad = P + rad; // octant 4
            break;

        case 0b111:
            rad = 3 * HP - rad; // octant 5
            break;

        case 0b011:
            rad = 3 * HP + rad; // octant 6
            break;

        case 0b010:
            rad = TP - rad; // octant 7
            break;
        
        default:
            break;
        }

        return rad_to_unsigned<uangle>(rad);
    }
}
}


#ifdef __AVX__
#define NUMERIC_SIMD_128
// -mavx -mavx2 -mfma

#include <immintrin.h>


namespace numeric
{
    static inline __m128 to_128(f32 val32)
    {
        return _mm_set_ss(val32);
    }


    static inline f32 to_f32(__m128 val128)
    {
        return _mm_cvtss_f32(val128);
    }


    static inline __m128d to_128(f64 val64)
    {
        return _mm_set_sd(val64);
    }


    static inline f64 to_f64(__m128d val128)
    {
        return _mm_cvtsd_f64(val128);
    }
}

#else

#ifndef NUMERIC_CMATH
#define NUMERIC_CMATH
#endif

#endif


#ifdef NUMERIC_CMATH
#include <cmath>
#endif


/* fma */

namespace numeric
{

    inline f64 fma(f64 a, f64 b, f64 c)
    {
#ifdef NUMERIC_SIMD_128
        auto a128 = to_128(a);
        auto b128 = to_128(b);
        auto c128 = to_128(c);

        auto res = _mm_fmadd_sd(a128, b128, c128);

        return to_f64(res);
#else
        return a * b + c;
#endif
    }


    inline f32 fmaf(f32 a, f32 b, f32 c)
    {
#ifdef NUMERIC_SIMD_128
        auto a128 = to_128(a);
        auto b128 = to_128(b);
        auto c128 = to_128(c);

        auto res = _mm_fmadd_ss(a128, b128, c128);

        return to_f32(res);
#else
        return a * b + c;
#endif
    }

}



namespace numeric
{
    template <typename T>
    inline constexpr bool is_unsigned()
    {
        return cxpr::is_unsigned<T>();
    }


    template <typename T>
    inline constexpr T min(T a, T b)
    {
        return cxpr::min(a, b);
    }


    template <typename T>
    inline constexpr T max(T a, T b)
    {
        return cxpr::max(a, b);
    }


    inline f32 min(f32 a, f32 b)
    {
#ifdef NUMERIC_SIMD_128
        auto a128 = to_128(a);
        auto b128 = to_128(b);

        auto res = _mm_min_ss(a128, b128);

        return to_f32(res);
#else
        return cxpr::min(a, b);
#endif
    }


    inline f64 min(f64 a, f64 b)
    {
#ifdef NUMERIC_SIMD_128
        auto a128 = to_128(a);
        auto b128 = to_128(b);

        auto res = _mm_min_sd(a128, b128);

        return to_f64(res);
#else
        return cxpr::min(a, b);
#endif
    }


    template <typename T>
    inline constexpr T clamp(T value, T min, T max)
    {
        return cxpr::clamp(value, min, max);
    }


    template <typename T, typename U>
    inline constexpr T clamp(T value, U min, U max)
    {
        return cxpr::clamp(value, min, max);
    }


    template <typename R, typename T>
    inline constexpr R sign(T value)
    {
        return cxpr::sign<R>(value);
    }


    template <typename T>
    inline constexpr T round_to_unsigned(f32 value)
    {
        static_assert(is_unsigned<T>());

    #ifdef NUMERIC_CMATH

        return (T)((u64)std::roundf(value));

    #else

        return cxpr::round_to_unsigned<T>(value);

    #endif        
    }


    template <typename T>
    inline constexpr T round_to_unsigned(f64 value)
    {
        static_assert(is_unsigned<T>());

    #ifdef NUMERIC_CMATH

        return (T)((u64)std::round(value));

    #else

        return cxpr::round_to_unsigned<T>(value);

    #endif
    }


    template <typename T>
    inline T round_to_signed(f32 value)
    {
        static_assert(!is_unsigned<T>());

    #ifdef NUMERIC_CMATH

        return (T)std::llround(value);

    #else

        return (T)fmaf(sign<f32, f32>(value), 0.5f, value);

    #endif
    }


    template <typename T>
    inline T round_to_signed(f64 value)
    {
        static_assert(!is_unsigned<T>());

    #ifdef NUMERIC_CMATH

        return (T)std::llround(value);

    #else

        return (T)fma(sign<f64, f64>(value), 0.5, value);

    #endif        
    }


    template <typename T>
    inline constexpr f32 sign_f32(T value)
    {           
        return cxpr::sign_f32(value);
    }


    template <typename T>
    inline constexpr i8 sign_i8(T value)
    {
        return cxpr::sign_i8(value);
    }


    template <typename T>
    inline T floor(T value)
    {         
    #ifdef NUMERIC_CMATH

        return (T)std::floor((f64)value);

    #else

        return (T)round_to_signed<i64>(value - 0.5f);

    #endif
    }


    inline f32 floor(f32 value)
    {
    #ifdef NUMERIC_CMATH

        return std::floor(value);

    #else

        return floor<f32>(value);

    #endif
    }


    template <typename T>
    inline T ceil(T value)
    { 
    #ifdef NUMERIC_CMATH

        return (T)std::ceil((f64)value);

    #else

        auto f = floor(value);
        return (T)(f + (f != value));

    #endif
    }


    inline f32 ceil(f32 value)
    { 
    #ifdef NUMERIC_CMATH

        return std::ceil(value);

    #else

        return ceil<f32>(value);

    #endif
    }


    template <typename T>
    inline constexpr T abs(T value)
    {
        return cxpr::abs(value);
    }


    template <typename uT>
    inline constexpr uT unsigned_max()
    {
        static_assert(is_unsigned<uT>());

        return cxpr::unsigned_max<uT>();
    }


    template <typename uT>
    inline uT scale_to_unsigned(f32 value)
    {
        static_assert(is_unsigned<uT>());

        constexpr f32 max = (f32)unsigned_max<uT>() + 1.0f;

        auto s = sign_f32(value);

        value -= s * (u64)value;
        value = s > 0.0f ? value : (1.0f - value);

        return round_to_unsigned<uT>(max * value);
    }


    template <typename uSrc, typename uDst>
    inline constexpr uDst scale_unsigned(uSrc value)
    {
        static_assert(is_unsigned<uSrc>());
        static_assert(is_unsigned<uDst>());

        return cxpr::scale_unsigned<uDst>(value);
    }


    template <typename T>
    inline constexpr T inc_wrap(T value, T min, T max)
    {
        return cxpr::inc_wrap(value, min, max);
    }


    inline constexpr bool is_power_of_2(u64 num)
    {
        return cxpr::is_power_of_2(num);
    }
}


namespace numeric
{
    inline f32 log(f32 x) 
    {
        u32 bx = *(u32*)(&x);
        u32 ex = bx >> 23;
        i32 t = (i32)ex-(i32)127;
        i32 s = (t < 0) ? (-t) : t;
        bx = 1065353216 | (bx & 8388607);
        x = *(f32*)(&bx);

        return -1.49278 + (2.11263 + (-0.729104+0.10969 * x) * x) * x + 0.6931471806 * t;
    }
    
    
    inline f32 rsqrt(f32 number)
    {
    #ifdef NUMERIC_SIMD_128

        auto num128 = to_128(number);
        auto res = _mm_rsqrt_ss(num128);
        return to_f32(res);

    #elif defined(NUMERIC_CMATH)

        return 1.0f / std::sqrt(number);
    
    #else
        static_assert(false && " *** rsqrt not defined *** ");
        return 0.0f;
    #endif
    }


    inline f32 sqrt(f32 number)
    {
        if (number <= 0.0f)
        {
            return 0.0f;
        }

    #ifdef NUMERIC_SIMD_128

        auto num = to_128(number);
        auto sqrt = _mm_sqrt_ss(num);

        return to_f32(sqrt);        
    #elif defined(NUMERIC_CMATH)
    
        return std::sqrt(number);

    #else
        static_assert(false && " *** sqrt not defined *** ");
        return 0.0f;
    #endif
    }


    inline f32 hypot(f32 a, f32 b)
    {
    #ifdef NUMERIC_CMATH

        return std::hypotf(a, b);

    #else
        
        return sqrt(fmaf(a, a, b * b));

    #endif
    }


    inline f32 rhypot(f32 a, f32 b)
    {
        return 1.0f / hypot(a, b); // div 0?
    }
}


/* vector */

namespace numeric
{
    template <typename T>
    inline f32 magnitude(Vec2D<T> const& vec)
    {
        auto const x = (f32)vec.x;
        auto const y = (f32)vec.y;

        return hypot(x, y);
    }
}


/* trig */

namespace numeric
{
    template <typename T>
    inline constexpr T deg_to_rad(T deg)
    {
        return cxpr::deg_to_rad(deg);
    }


    template <typename T>
    inline constexpr T rad_to_deg(T rad)
    {
        return cxpr::rad_to_deg(rad);
    }


    template <typename uT>
    inline constexpr f32 max_angle_f32()
    {
        static_assert(is_unsigned<uT>());

        return cxpr::max_angle_f32<uT>();
    }


    template <typename uT>
    inline constexpr u64 max_angle_u64()
    {
        static_assert(is_unsigned<uT>());

        return cxpr::max_angle_u64<uT>();
    }


    template <typename uT>
    inline constexpr f32 unsigned_to_rad(uT a)
    {
        static_assert(is_unsigned<uT>());

        return cxpr::unsigned_to_rad(a);
    }


    template <typename uT>
    inline constexpr uT rad_to_unsigned(f32 rad)
    {
        static_assert(is_unsigned<uT>());

        return cxpr::rad_to_unsigned<uT>(rad);
    }


    inline constexpr f32 u16_to_rad(u16 a)
    {
        return unsigned_to_rad(a);
    }


    inline constexpr f32 u8_to_rad(u8 a)
    {
        return unsigned_to_rad(a);
    }
    

    inline f32 sin_approx(f32 rad)
    {
        // best for small angles e.g. 0 - 45deg

        constexpr f32 B = 4.0f / (f32)PI;
        constexpr f32 C = -4.0f / ((f32)(PI * PI));
        constexpr f32 P = 0.225f;

        f32 y = fmaf(B, rad, C * rad * abs(rad));
        y = fmaf(P, fmaf(y, abs(y), -y), y);

        return y;
    }
    

    inline f32 cos_approx(f32 rad)
    {
        // best for small angles e.g. 0 - 45deg

        constexpr f32 tp = 1.0f / (f32)(2 * PI);

        f32 x = rad * tp;

        x -= 0.25f + floor(x + 0.25f);
        x *= 16.0f * (abs(x) - 0.5f);
        x += 0.225f * x * (abs(x) - 1.0f);

        return x;
    }


    inline f32 atan_approx(f32 tan)
    {
        f32 sq = tan * tan;

        constexpr f32 a1  =  0.99997726f;
        constexpr f32 a3  = -0.33262347f;
        constexpr f32 a5  =  0.19354346f;
        constexpr f32 a7  = -0.11643287f;
        constexpr f32 a9  =  0.05265332f;
        constexpr f32 a11 = -0.01172120f;        

        //return tan * (a1 + sq * (a3 + sq * (a5 + sq * (a7 + sq * (a9 + sq * a11)))));
        return tan * fmaf(sq, fmaf(sq, fmaf(sq, fmaf(sq, fmaf(sq, a11, a9), a7), a5), a3), a1);
    }


    
}


/* sin cos uangle */

namespace numeric
{
    inline f32 sin(uangle a)
    {
        static_assert(sizeof(uangle) <= sizeof(u32));

        constexpr f32 P = (f32)PI;
        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 HP = (f32)(PI / 2);

        // split full rotation into 8 x 45deg sections
        constexpr u64 max = max_angle_u64<uangle>();
        constexpr u64 oct = max / 8;
        
        auto rad = unsigned_to_rad(a);

        switch (a / oct)
        {
            case 0: return sin_approx(rad);
            case 1: return cos_approx(HP - rad);
            case 2: return cos_approx(rad - HP);
            case 3: return sin_approx(P - rad);
            case 4: return -sin_approx(rad - P);
            case 5: return -cos_approx(P + HP - rad);
            case 6: return -cos_approx(rad - (P + HP));
            case 7: return -sin_approx(TP - rad);
            default: return 0.0f;
        }
    }


    inline f32 cos(uangle a)
    {
        static_assert(sizeof(uangle) <= sizeof(u32));

        constexpr f32 P = (f32)PI;
        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 HP = (f32)(PI / 2);

        // split full rotation into 8 x 45deg sections
        constexpr u64 max = max_angle_u64<uangle>();
        constexpr u64 oct = max / 8;
        
        auto rad = unsigned_to_rad(a);

        switch (a / oct)
        {
            case 0: return cos_approx(rad);
            case 1: return sin_approx(HP - rad);
            case 2: return -sin_approx(rad - HP);
            case 3: return -cos_approx(P - rad);
            case 4: return -cos_approx(rad - P);
            case 5: return -sin_approx(P + HP - rad);
            case 6: return sin_approx(rad - (P + HP));
            case 7: return cos_approx(TP - rad);
            default: return 0.0f;
        }
    }


    inline f32 sin(f32 rad)
    {
        return sin(rad_to_unsigned<uangle>(rad));
    }


    inline f32 cos(f32 rad)
    {
        return cos(rad_to_unsigned<uangle>(rad));
    }


    inline uangle atan2(f32 sin, f32 cos)
    { 
        constexpr f32 P = (f32)PI;
        constexpr f32 TP = (f32)(2 * PI);
        constexpr f32 HP = (f32)(PI / 2);
        constexpr f32 QP = (f32)(PI / 4);

        assert(abs((cos * cos + sin * sin) - 1.0f) < 0.001f);

        auto pcos = abs(cos);
        auto psin = abs(sin);

        auto flip_45 = pcos < psin;
        auto flip_y = cos < 0.0f;
        auto flip_x = sin < 0.0f;

        auto key_y = (int)flip_y << 2;
        auto key_x = (int)flip_x << 1;
        auto key_45 = (int)flip_45;

        int oct_key = key_y | key_x | key_45;

        auto tan = flip_45 ? pcos / psin : psin / pcos;
        auto rad = atan_approx(tan);
        
        assert(rad >= 0.0f);
        assert(rad <= QP);

        switch (oct_key)
        {
        case 0b000:
            // octant 0
            break;

        case 0b001:
            rad = HP - rad; // octant 1
            break;

        case 0b101:
            rad = HP + rad; // octant 2
            break;

        case 0b100:
            rad = P - rad; // octant 3
            break;

        case 0b110:
            rad = P + rad; // octant 4
            break;

        case 0b111:
            rad = 3 * HP - rad; // octant 5
            break;

        case 0b011:
            rad = 3 * HP + rad; // octant 6
            break;

        case 0b010:
            rad = TP - rad; // octant 7
            break;
        
        default:
            break;
        }

        return rad_to_unsigned<uangle>(rad);
    }
}


