/*
 * Cache.h
 *
 *  Created on: 22 Nov 2019
 *      Author: David
 */

#ifndef SRC_CACHE_H_
#define SRC_CACHE_H_

#include <CoreIO.h>

namespace Cache
{
	void Init() noexcept;
	void Enable() noexcept;
	void Disable() noexcept;
	void Flush(const volatile void *start, size_t length) noexcept;
	void Invalidate(const volatile void *start, size_t length) noexcept;

	inline void FlushBeforeDMAReceive(const volatile void *start, size_t length) noexcept { Flush(start, length); }
	inline void InvalidateAfterDMAReceive(const volatile void *start, size_t length) noexcept { Invalidate(start, length); }
	inline void FlushBeforeDMASend(const volatile void *start, size_t length) noexcept { Flush(start, length); }

#if SAM4E
	uint32_t GetHitCount() noexcept;
#endif
};

// Entry points that can be called from ASF C code
extern "C" void CacheFlushBeforeDMAReceive(const volatile void *start, size_t length) noexcept;
extern "C" void CacheInvalidateAfterDMAReceive(const volatile void *start, size_t length) noexcept;
extern "C" void CacheFlushBeforeDMASend(const volatile void *start, size_t length) noexcept;

#if SAMC21

// SAMC21 has no cache
inline void Cache::Init() noexcept { __DSB(); }
inline void Cache::Enable() noexcept { }
inline void Cache::Disable() noexcept { }
inline void Cache::Flush(const volatile void *start, size_t length) noexcept { __DSB(); }
inline void Cache::Invalidate(const volatile void *start, size_t length) noexcept { __DSB(); }

#endif

#if SAM4E || SAME5x

// The SAM4E and SAME5x cache is write-through, so no need to flush it
inline void Cache::Flush(const volatile void *start, size_t length) noexcept { __DSB(); }

#endif

#endif /* SRC_CACHE_H_ */
