#pragma once

#include <emmintrin.h>
#include "AlignedType.h"

/**
 * A stereo sample.
 */
struct alignas(16) Sample : public AlignedType
{
	/**
	 * Left and right volume two doubles packed in a SSE register.
	 */
	Sample() {}
	explicit Sample(const double singleSample) {v = _mm_set1_pd(singleSample);}
	Sample(__m128d in) { v = in; }
	Sample(const Sample& x) { v = x.v; }
	Sample(double sampleLeft, double sampleRight) {	v = _mm_set_pd(sampleLeft, sampleRight);}
	alignas(16) __m128d v;
	inline __m128d& get(void)
	{
		return v;
	}

	Sample& operator=(const Sample& other)
	{
		v = other.v;
		return *this;
	}

	Sample& operator+=(const Sample& other)
	{
		v = _mm_add_pd(v, other.v);
		return *this;
	}

	Sample& operator-=(const Sample& other)
	{
		v = _mm_sub_pd(v, other.v);
		return *this;
	}

	Sample& operator*=(const Sample& other)
	{
		v = _mm_mul_pd(v, other.v); return *this;
	}

	Sample& operator/=(const Sample& other)
	{
		v = _mm_div_pd(v, other.v); return *this;
	}

	/**
	 * Creates a sample value from two double values at the given address.
	 * Note that the address must be aligned to 16-byte boundaries.
	 * @param ptr The location of the sample values.
	 * @return The values as a Sample.
	 */
	//static
	Sample load_aligned(const double* ptr)
	{
		return Sample(_mm_load_pd(ptr));
	}

	/**
	 * Write the sample values as two doubles to the given address.
	 * Note that the address must be aligned to 16-byte boundaries.
	 * @param The location to which the values should be written.
	 */
	void store_aligned(double* ptr)
	{
		_mm_store_pd(ptr, v);
	}
};

inline Sample operator+(const Sample& a, const Sample& b) {
	return Sample(_mm_add_pd(a.v, b.v));
}

inline Sample operator-(const Sample& a, const Sample& b) {
	return Sample(_mm_sub_pd(a.v, b.v));
}

inline Sample operator*(const Sample& a, const Sample& b) {
	return Sample(_mm_mul_pd(a.v, b.v));
}

inline Sample operator/(const Sample& a, const Sample& b) {
	return Sample(_mm_div_pd(a.v, b.v));
}
