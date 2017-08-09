//--------------------------------------------------------------------------//
/// Copyright (c) 2017 by Milos Tosic. All Rights Reserved.                ///
/// License: http://www.opensource.org/licenses/BSD-2-Clause               ///
//--------------------------------------------------------------------------//

#ifndef __RTM_RBASE_ATOMIC_H__
#define __RTM_RBASE_ATOMIC_H__

#include <rbase/inc/platform.h>

#if RTM_PLATFORM_WINDOWS
extern "C"
{
	__declspec(dllimport) long		_InterlockedIncrement(long volatile*);
	__declspec(dllimport) long		_InterlockedDecrement(long volatile*);
	__declspec(dllimport) long		_InterlockedCompareExchange(long volatile*, long, long);
	__declspec(dllimport) long		_InterlockedExchange(long volatile*, long);
	__declspec(dllimport) long		_InterlockedExchangeAdd(long volatile*, long);
	__declspec(dllimport) long long	_InterlockedCompareExchange64(long long volatile*, long long, long long);
	__declspec(dllimport) long long	_InterlockedIncrement64(long long volatile*);
	__declspec(dllimport) long long	_InterlockedDecrement64(long long volatile*);
	__declspec(dllimport) long long	_InterlockedExchange64(long long volatile*, long long);
	__declspec(dllimport) long long	_InterlockedExchangeAdd64(long long volatile*, long long);
	__declspec(dllimport) void 		_ReadBarrier(void);
	__declspec(dllimport) void 		_WriteBarrier(void);
	__declspec(dllimport) void 		_ReadWriteBarrier(void);
	__declspec(dllimport) void		_mm_mfence(void);
}
#if RTM_COMPILER_MSVC
	#pragma intrinsic (_InterlockedIncrement)
	#pragma intrinsic (_InterlockedDecrement)
	#pragma intrinsic (_InterlockedCompareExchange)
	#pragma intrinsic (_InterlockedExchange)
	#pragma intrinsic (_InterlockedExchangeAdd)
	#pragma intrinsic (_InterlockedCompareExchange64)
#if RTM_64BIT
	#pragma intrinsic (_InterlockedIncrement64)
	#pragma intrinsic (_InterlockedDecrement64)
	#pragma intrinsic (_InterlockedExchange64)
	#pragma intrinsic (_InterlockedExchangeAdd64)
#endif // RTM_64BIT
	#pragma intrinsic (_ReadBarrier)
	#pragma intrinsic (_WriteBarrier)
	#pragma intrinsic (_ReadWriteBarrier)
	#pragma intrinsic (_mm_mfence)
#endif // RTM_COMPILER_MSVC
#endif // RTM_PLATFORM_WINDOWS

#if RTM_PLATFORM_NACL || RTM_PLATFORM_ANDROID || RTM_PLATFORM_LINUX || RTM_PLATFORM_OSX
#include <sched.h> // sched_yield 
#endif

namespace rtm {

	static inline void readBarrier()
	{
#if RTM_COMPILER_MSVC
		_ReadBarrier();
#elif RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		asm volatile("":::"memory");
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline void writeBarrier()
	{
#if RTM_COMPILER_MSVC
		_WriteBarrier();
#elif RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		asm volatile("":::"memory");
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline void readWriteBarrier()
	{
#if RTM_COMPILER_MSVC
		_ReadWriteBarrier();
#elif RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		asm volatile("":::"memory");
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline void memoryBarrier()
	{
#if RTM_PLATFORM_WINDOWS
		_mm_mfence();
#elif RTM_PLATFORM_XBOX360 || RTM_PLATFORM_PS3
		__lwsync();
#elif RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		__sync_synchronize();
#else
		#error "Unsupported platform/compiler!"
#endif
	}

	static inline int32_t interlockedInc(int32_t volatile* _value)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_add(_value, 1);
#elif RTM_COMPILER_MSVC
		return _InterlockedIncrement((long volatile*)_value);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int64_t interlockedInc(int64_t volatile* _value)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_add(_value, 1);
#elif RTM_COMPILER_MSVC
		return _InterlockedIncrement64((long long volatile*)_value);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int32_t interlockedDec(int32_t volatile* _value)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_add(_value, -1);
#elif RTM_COMPILER_MSVC
		return _InterlockedDecrement((volatile long*)_value);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int64_t interlockedDec(int64_t volatile* _value)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_add(_value, -1);
#elif RTM_COMPILER_MSVC
		return _InterlockedDecrement64((volatile long long*)_value);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int32_t interlockedAdd(int32_t volatile* _value, int32_t _add)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_add(_value, _add);
#elif RTM_COMPILER_MSVC
		return _InterlockedExchangeAdd((volatile long*)_value, _add);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int64_t interlockedAdd(int64_t volatile* _value, int64_t _add)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_add(_value, _add);
#elif RTM_COMPILER_MSVC
		return _InterlockedExchangeAdd64((volatile long long*)_value, _add);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int32_t interlockedSub(int32_t volatile* _value, int32_t _sub)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_sub(_value, _sub);
#elif RTM_COMPILER_MSVC
		return _InterlockedExchangeAdd((volatile long*)_value, -_sub);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int64_t interlockedSub(int64_t volatile* _value, int64_t _sub)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_fetch_and_sub( _value, _sub );
#elif RTM_COMPILER_MSVC
		return _InterlockedExchangeAdd64((volatile long long*)_value, -_sub);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int32_t interlockedSet(int32_t volatile* _value, int32_t _newValue)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		__sync_synchronize();
		return __sync_lock_test_and_set(_value, _newValue);
#elif RTM_COMPILER_MSVC
		return _InterlockedExchange((volatile long*)_value, _newValue);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int64_t interlockedSet(int64_t volatile* _value, int64_t _newValue)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		__sync_synchronize();
		return __sync_lock_test_and_set(_value, _newValue);
#elif RTM_COMPILER_MSVC
		return _InterlockedExchange64((volatile long long*)_value, _newValue);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int32_t interlockedCAS(int32_t volatile* _value, int32_t _oldValue, int32_t _newValue)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_val_compare_and_swap(_value, _oldValue, _newValue);
#elif RTM_COMPILER_MSVC
		return _InterlockedCompareExchange((volatile long*)_value, _newValue, _oldValue);
#else
		#error "Unsupported compiler!"
#endif
	}

	static inline int64_t interlockedCAS(int64_t volatile* _value, int64_t _oldValue, int64_t _newValue)
	{
#if RTM_COMPILER_GCC || RTM_COMPILER_CLANG
		return __sync_val_compare_and_swap(_value, _oldValue, _newValue);
#elif RTM_COMPILER_MSVC
		return _InterlockedCompareExchange64((volatile long long*)_value, _newValue, _oldValue);
#else
		#error "Unsupported compiler!"
#endif
	}

} // namespace rtm

#endif // __RTM_RBASE_ATOMIC_H__