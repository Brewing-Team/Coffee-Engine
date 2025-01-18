/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2024 Audiokinetic Inc.
*******************************************************************************/

// AkAtomic.h

#pragma once

#if !defined(WIN32_LEAN_AND_MEAN)
	#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <AK/SoundEngine/Common/AkAtomicTypes.h>

// Sleep of 1 is as close as we can get on Microsoft platforms (SwitchToThread()/sleep(0) is not reliable enough to get a real yield)
#define AkThreadYield() Sleep(1);

#ifdef __cplusplus
extern "C" {
#endif

#if defined( _M_ARM )
#define						AK_ATOMIC_FENCE_FULL_BARRIER()												__dmb( _ARM_BARRIER_ISH )
#elif defined( _M_ARM64 )
#define						AK_ATOMIC_FENCE_FULL_BARRIER()												__dmb( _ARM64_BARRIER_ISH )
#else
#define						AK_ATOMIC_FENCE_FULL_BARRIER()												MemoryBarrier();
#endif

#if defined( _M_ARM ) || defined( _M_ARM64 )
__forceinline	long		AkAtomicLoad32( AkAtomic32* pSrc )											{ long tmp; tmp = *pSrc; AK_ATOMIC_FENCE_FULL_BARRIER(); return tmp; }
#else
__forceinline	long		AkAtomicLoad32( AkAtomic32* pSrc )											{ return *pSrc; }
#endif
__forceinline	void		AkAtomicStore32( AkAtomic32* pDest, long value )							{ InterlockedExchange((volatile long*)pDest, value); }
__forceinline	long		AkAtomicInc32( AkAtomic32* pValue )											{ return InterlockedExchangeAdd((volatile long*)pValue, 1 ) + 1; }
__forceinline	long		AkAtomicDec32( AkAtomic32* pValue )											{ return InterlockedExchangeAdd((volatile long*)pValue, -1 ) - 1; }
__forceinline	long		AkAtomicExchange32( AkAtomic32* pDest, long value )							{ return InterlockedExchange((volatile long*)pDest, value ); }
__forceinline	long		AkAtomicAdd32( AkAtomic32* pDest, long value )								{ return InterlockedExchangeAdd((volatile long*)pDest, value ) + value; }
__forceinline	long		AkAtomicSub32( AkAtomic32* pDest, long value )								{ return InterlockedExchangeAdd((volatile long*)pDest, -value ) - value; }
__forceinline	long		AkAtomicAnd32( AkAtomic32* pDest, long value )								{ return InterlockedAnd((volatile long*)pDest, value) & value; }
__forceinline	long		AkAtomicOr32(  AkAtomic32* pDest, long value )								{ return InterlockedOr((volatile long*)pDest, value) | value; }
__forceinline	int			AkAtomicCas32( AkAtomic32* pDest, long proposed, long expected )			{ return InterlockedCompareExchange((volatile long*)pDest, proposed, expected ) == expected ? 1 : 0; }

#ifdef _WIN64
#if defined( _M_ARM ) || defined( _M_ARM64 )
__forceinline	long long	AkAtomicLoad64( AkAtomic64* pSrc )											{ long long tmp; tmp = *pSrc; AK_ATOMIC_FENCE_FULL_BARRIER(); return tmp; }
#else
__forceinline	long long	AkAtomicLoad64( AkAtomic64* pSrc )											{ return *pSrc; }
#endif
#else
__forceinline	long long	AkAtomicLoad64( AkAtomic64* pSrc )											{ return InterlockedCompareExchange64( pSrc, 0, 0 ); }
#endif

__forceinline	void		AkAtomicStore64( AkAtomic64* pDest, long long value )						{ InterlockedExchange64((volatile long long*)pDest, value); }
__forceinline	long long	AkAtomicInc64( AkAtomic64* pValue )											{ return InterlockedExchangeAdd64((volatile long long*)pValue, 1 ) + 1; }
__forceinline	long long	AkAtomicDec64( AkAtomic64* pValue )											{ return InterlockedExchangeAdd64((volatile long long*)pValue, - 1 ) - 1; }
__forceinline	long long	AkAtomicExchange64( AkAtomic64* pDest, long long value )					{ return InterlockedExchange64((volatile long long*)pDest, value ); }
__forceinline	long long	AkAtomicAdd64( AkAtomic64* pDest, long long value )							{ return InterlockedExchangeAdd64((volatile long long*)pDest, value ) + value; }
__forceinline	long long	AkAtomicSub64( AkAtomic64* pDest, long long value )							{ return InterlockedExchangeAdd64((volatile long long*)pDest, -value ) - value; }
__forceinline	long long	AkAtomicAnd64( AkAtomic64* pDest, long long value )							{ return InterlockedAnd64((volatile long long*)pDest, value) & value; }
__forceinline	long long	AkAtomicOr64(  AkAtomic64* pDest, long long value )							{ return InterlockedOr64((volatile long long*)pDest, value) | value; }
__forceinline	int			AkAtomicCas64( AkAtomic64* pDest, long long proposed, long long expected )	{ return InterlockedCompareExchange64((volatile long long*)pDest, proposed, expected ) == expected ? 1 : 0; }

#if defined( _M_ARM ) || defined( _M_ARM64 )
__forceinline	void*		AkAtomicLoadPtr( AkAtomicPtr* pSrc )										{ void* tmp; tmp = ( void* )*pSrc; AK_ATOMIC_FENCE_FULL_BARRIER(); return tmp; }
#else
__forceinline	void*		AkAtomicLoadPtr( AkAtomicPtr* pSrc )										{ return ( void* )*pSrc; }
#endif

#ifdef _WIN64
__forceinline	void		AkAtomicStorePtr( AkAtomicPtr* pDest, void* value )							{ InterlockedExchangePointer( ( volatile PVOID* )pDest, value ); }
__forceinline	void*		AkAtomicExchangePtr( AkAtomicPtr* pDest, void* value )						{ return InterlockedExchangePointer( ( volatile PVOID* )pDest, value ); }
__forceinline	int			AkAtomicCasPtr( AkAtomicPtr* pDest, void* proposed, void* expected )		{ return InterlockedCompareExchangePointer( ( volatile PVOID* )pDest, proposed, expected ) == expected ? 1 : 0; }
#else
__forceinline	void		AkAtomicStorePtr( AkAtomicPtr* pDest, void* value )							{ InterlockedExchangePointer( ( void** )pDest, value );  }
__forceinline	void*		AkAtomicExchangePtr(AkAtomicPtr* pDest, void* value)						{ return InterlockedExchangePointer( ( void** )pDest, value ); }
__forceinline	int			AkAtomicCasPtr(AkAtomicPtr* pDest, void* proposed, void* expected)			{ return InterlockedCompareExchangePointer( ( void** )pDest, proposed, expected ) == expected ? 1 : 0; }
#endif

#if defined(_MSC_VER)
__forceinline void AkSpinHint(void)
{
#if defined(_M_IX86) || defined(_M_X64)
	_mm_pause();
#elif defined( _M_ARM ) || defined( _M_ARM64 )
	__yield();
#else
	#error Unsupported platform for AkSpinHint
#endif
}
#endif

#ifdef __cplusplus
}
#endif
