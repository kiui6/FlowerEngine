#pragma once
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    #include <immintrin.h>

    using simd128i = __m128i;

    static inline simd128i simdLoadu128i(simd128i const *ptr) {
        return _mm_loadu_si128(ptr);
    }

    static inline simd128i simdLoad128i(simd128i const *ptr) {
        return _mm_load_si128(ptr);
    }

    static inline void simdStore128i(simd128i *ptr, simd128i val) {
        _mm_store_si128(ptr, val);
    }

    static inline simd128i simdSet128i(const char val) {
        return _mm_set1_epi8(val);
    }

    static inline int simdMovemask8(simd128i val) {
        return _mm_movemask_epi8(val);
    }

    static inline simd128i simdCmpEq8(simd128i vec1, simd128i vec2) {
        return _mm_cmpeq_epi8(vec1, vec2);
    }

#elif defined(__ARM_NEON) || defined(_M_ARM64)
    #include <arm_neon.h>

    using simd128i = uint8x16_t;

    static inline simd128i simdLoadu128i(simd128i const *ptr) {
        return vld1q_u8(reinterpret_cast<const uint8_t*>(ptr));
    }

    static inline simd128i simdSet128i(const char val) {
        return vdupq_n_u8(ptr);
    }

    static inline void simdStore128i(simd128i *ptr, simd128i val) {
        vst1q_u8((uint8_t)ptr, val);
    }

    static inline int simdMovemask8(simd128i val) {
    #if defined(__aarch64__)
        uint8x8_t weight = vcreate_u8(UINT64_C(0x8040201008040201));
        uint16x8_t lo = vmovl_u8(
            vand_u8(vshr_n_u8(vget_low_u8(val), 7), weight));
        uint16x8_t hi = vmovl_u8(
            vand_u8(vshr_n_u8(vget_high_u8(val), 7), weight));

        uint16_t low_sum  = vaddvq_u16(lo);
        uint16_t high_sum = vaddvq_u16(vshlq_n_u16(hi, 8));

        return (int)(low_sum | high_sum);
    #else
        uint8x8_t weight = vcreate_u8(UINT64_C(0x8040201008040201));

        uint8x8_t lo = vget_low_u8(val);
        uint8x8_t hi = vget_high_u8(val);

        lo = vand_u8(vshr_n_u8(lo, 7), weight);
        hi = vand_u8(vshr_n_u8(hi, 7), weight);

        lo = vpadd_u8(lo, lo);
        lo = vpadd_u8(lo, lo);
        lo = vpadd_u8(lo, lo);
        uint8_t low_mask  = vget_lane_u8(lo, 0);

        hi = vpadd_u8(hi, hi);
        hi = vpadd_u8(hi, hi);
        hi = vpadd_u8(hi, hi);
        uint8_t high_mask = vget_lane_u8(hi, 0);

        return (high_mask << 8) | low_mask;
    #endif
    }

    static inline simd128i simdCmpEq8(simd128i vec1, simd128i vec2) {
        return vceqq_u8(vec1, vec2);
    }
#else
    #include <cstdint>
    typedef struct simd128i {
        uint8_t i8[16];
    } simd128i;

    static inline simd128i simdLoadu128i(simd128i const *ptr) {
        simd128i result;
        memcpy(result.data, ptr->data, 16);
        return result;
    }

    static inline void simdStore128i(simd128i *ptr, simd128i val) {
        memcpy(ptr, &val, sizeof(simd128i));
    }

    static inline simd128i simdSet128i(const char val) {
        return {{val, val, val, val, val, val, val, val, val, val, val, val, val, val, val, val}};
    }

    static inline simd128i simdCmpEq8(simd128i vec1, simd128i vec2) {
        simd128i result;
        for (int i = 0; i < 16; ++i) {
            result.data[i] = (a.data[i] == b.data[i]) ? 0xFF : 0x00;
        }
        return result;
    }
#endif