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

// AkPlatformFuncs.h

/// \file 
/// Platform-dependent functions definition.

#ifndef _AK_TOOLS_COMMON_AKPLATFORMFUNCS_H
#define _AK_TOOLS_COMMON_AKPLATFORMFUNCS_H

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkAtomicTypes.h>

// Uncomment the following to enable built-in platform profiler markers in the sound engine
//#define AK_ENABLE_INSTRUMENT

#if defined(AK_NULL_PLATFORM)
// null platform has no funcs
struct AkThreadProperties {};
#elif defined(AK_WIN)
#include <AK/Tools/Win32/AkPlatformFuncs.h>

#elif defined (AK_XBOX)
#include <AK/Tools/XboxGC/AkPlatformFuncs.h>

#elif defined (AK_APPLE)
#include <AK/Tools/Mac/AkPlatformFuncs.h>
#include <AK/Tools/POSIX/AkPlatformFuncs.h>

#elif defined( AK_ANDROID ) || defined ( AK_LINUX_AOSP ) 
#include <AK/Tools/Android/AkPlatformFuncs.h>

#elif defined ( AK_HARMONY ) 
#include <AK/Tools/OpenHarmony/AkPlatformFuncs.h>

#elif defined (AK_PS4)
#include <AK/Tools/PS4/AkPlatformFuncs.h>

#elif defined (AK_PS5)
#include <AK/Tools/PS5/AkPlatformFuncs.h>

#elif defined (AK_EMSCRIPTEN)
#include <AK/Tools/Emscripten/AkPlatformFuncs.h>

#elif defined (AK_LINUX)
#include <AK/Tools/Linux/AkPlatformFuncs.h>
#include <AK/Tools/POSIX/AkPlatformFuncs.h>

#elif defined (AK_QNX)
#include <AK/Tools/QNX/AkPlatformFuncs.h>
#include <AK/Tools/POSIX/AkPlatformFuncs.h>

#elif defined (AK_NX)
#include <AK/Tools/NX/AkPlatformFuncs.h>

#else
#error AkPlatformFuncs.h: Undefined platform
#endif

#ifndef AkPrefetchZero
#define AkPrefetchZero(___Dest, ___Size)
#endif

#ifndef AkPrefetchZeroAligned
#define AkPrefetchZeroAligned(___Dest, ___Size)
#endif

#ifndef AkZeroMemAligned
#define AkZeroMemAligned(___Dest, ___Size) AKPLATFORM::AkMemSet(___Dest, 0, ___Size);
#endif
#ifndef AkZeroMemLarge
#define AkZeroMemLarge(___Dest, ___Size) AKPLATFORM::AkMemSet(___Dest, 0, ___Size);
#endif
#ifndef AkZeroMemSmall
#define AkZeroMemSmall(___Dest, ___Size) AKPLATFORM::AkMemSet(___Dest, 0, ___Size);
#endif

#ifndef AkAllocaSIMD
#if defined (__clang__)
#if __has_builtin( __builtin_alloca_with_align )
#define AkAllocaSIMD( _size_ ) __builtin_alloca_with_align( _size_, 128 )
#else
// work around alloca alignment issues in versions of clang before 4.0
#define AkAllocaSIMD( _size_ ) (void*)( ( ( uintptr_t )AkAlloca( _size_ + 16 ) + 0xF ) & ~0xF )
#endif
#else
#define AkAllocaSIMD( _size_ ) AkAlloca( _size_ )
#endif
#endif

#ifndef AkAllocaTypedArray
#define AkAllocaTypedArray(_type_, _count_) ( (_type_*)AkAlloca(sizeof(_type_) * _count_) )
#endif

#ifndef AK_THREAD_INIT_CODE
#define AK_THREAD_INIT_CODE(_threadProperties)
#endif

#ifndef AK_PLATFORM_MEMCPY
namespace AKPLATFORM
{
	/// Platform Independent Helper for memcpy/memmove/memset
	AkForceInline void AkMemCpy(void* pDest, const void* pSrc, AkUInt32 uSize)
	{
		memcpy(pDest, pSrc, uSize);
	}

	AkForceInline void AkMemMove(void* pDest, const void* pSrc, AkUInt32 uSize)
	{
		memmove(pDest, pSrc, uSize);
	}

	AkForceInline void AkMemSet(void* pDest, AkInt32 iVal, AkUInt32 uSize)
	{
		memset(pDest, iVal, uSize);
	}
}
#endif // AK_PLATFORM_MEMCPY

#if !defined(AK_NULL_PLATFORM)
/// Platform-dependent helpers
namespace AKPLATFORM
{
	inline void AkGetDefaultHighPriorityThreadProperties(AkThreadProperties& out_threadProperties)
	{
		AkGetDefaultThreadProperties(out_threadProperties);
		out_threadProperties.nPriority = AK_THREAD_PRIORITY_ABOVE_NORMAL;
	}

}
#endif

namespace AKPLATFORM
{
	// to be used to unequivocally cause a crash for scenarios that are critical failures that we cannot hope to recover from
	AkForceInline void AkForceCrash()
	{
		// We don't want to simply abort() the program; we want a real SIGSEGV happening on *this* thread
		// This convoluted way of crashing is necessary to avoid compilers optimizing out invalid code
		// By calling PerformanceCounter, which is inherently non-deterministic, the compiler must generate code as-is
		AkInt64 one, two;
		PerformanceCounter(&one);
		PerformanceCounter(&two);
		AkUIntPtr ptr = (AkUIntPtr)two - (AkUIntPtr)one;
		((void(*)())ptr)();
	}
}

#ifndef AK_FORCE_CRASH
#define AK_FORCE_CRASH AKPLATFORM::AkForceCrash()
#endif

#endif // _AK_TOOLS_COMMON_AKPLATFORMFUNCS_H
