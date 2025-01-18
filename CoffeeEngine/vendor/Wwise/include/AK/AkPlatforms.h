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

/// \file 
/// Audiokinetic platform checks. This is where we detect which platform
/// is being compiled, and where we define the corresponding AK-specific
/// symbols.

#pragma once

#if defined( NN_NINTENDO_SDK )
	#include "nn/nn_Platform.h"
#endif

#if defined( AK_NULL_PLATFORM )
	// AK_NULL_PLATFORM can be defined as a part of standing up a new platform in client code,
	// which may not be fully supported by Wwise.
	#include <AK/SoundEngine/Platforms/Generic/AkTypes.h>

#elif defined( NN_BUILD_TARGET_PLATFORM_NX )

	#include <AK/SoundEngine/Platforms/NX/AkTypes.h>

#elif defined( _GAMING_XBOX )

	#include <AK/SoundEngine/Platforms/XboxGC/AkTypes.h>

#elif defined( _WIN32 ) || defined ( _WIN64 ) || defined( WINAPI_FAMILY )

	#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>

#elif defined( __APPLE__ )

	#include <AK/SoundEngine/Platforms/Mac/AkTypes.h>
	
#elif defined( __ORBIS__ )

	#include <AK/SoundEngine/Platforms/PS4/AkTypes.h>

#elif defined(__HARMONY__)

	#include <AK/SoundEngine/Platforms/OpenHarmony/AkTypes.h>

#elif defined( __PROSPERO__ )

	#include "AK/SoundEngine/Platforms/PS5/AkTypes.h"
	
#elif defined( __ANDROID__ ) && !defined (AK_LINUX_AOSP)

	#include <AK/SoundEngine/Platforms/Android/AkTypes.h>

#elif defined( __linux__ )

	#include <AK/SoundEngine/Platforms/Linux/AkTypes.h>

#elif defined( __EMSCRIPTEN__ )

	#include <AK/SoundEngine/Platforms/Emscripten/AkTypes.h>
	
#elif defined( __QNX__ )

	#include <AK/SoundEngine/Platforms/QNX/AkTypes.h>

#elif defined(__has_include)

// AkTypes included here should have internal guardrails to prevent processing if not applicable for current platform

#else

	#error Unsupported platform, or platform-specific symbols not defined

#endif

#ifndef AK_ALIGN
#if defined(SWIG)
#define AK_ALIGN( _declaration_, _alignment_ ) _declaration_
#else
#if defined(_MSC_VER)
#define AK_ALIGN( _declaration_, _alignment_ ) __declspec( align( _alignment_ ) ) _declaration_
#else
#define AK_ALIGN( _declaration_, _alignment_ ) _declaration_ __attribute__( ( aligned( _alignment_ ) ) )
#endif // _MSC_VER
#endif // SWIG
#endif // AK_ALIGN

#define AK_ALIGN_TO_NEXT_BOUNDARY( __num__, __boundary__ ) (((__num__) + ((__boundary__)-1)) & ~((__boundary__)-1))
#define AK_IS_POWER_OF_TWO(__num__) (((__num__) & ((__num__) - 1)) == 0)

#if !defined(AK_ENDIANNESS_LITTLE) && !defined(AK_ENDIANNESS_BIG)
#define AK_ENDIANNESS_LITTLE
#endif

/// AK_UNALIGNED refers to the __unaligned compilation flag available on some platforms. Note that so far, on the tested platform this should always be placed before the pointer symbol *.
#ifndef AK_UNALIGNED
#if defined(__GNUC__)
#define AK_UNALIGNED __attribute__((aligned(1)))
#elif defined(_MSC_VER) && !defined(AK_CPU_X86) // __unaligned not supported on 32-bit x86
#define AK_UNALIGNED __unaligned
#else
#define AK_UNALIGNED
#endif
#endif // AK_UNALIGNED

#ifndef AK_SELECTANY
#if defined(__GNUC__)
#define AK_SELECTANY __attribute__((weak))
#elif defined(_MSC_VER)
#define AK_SELECTANY __declspec(selectany)
#else
#define AK_SELECTANY
#endif
#endif // AK_SELECTANY

#if (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define AK_CPP17
#endif

#if (defined(__cplusplus) && __cplusplus >= 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#define AK_CPP20
#endif

#define AK_NODISCARD    [[nodiscard]]

#if defined(AK_CPU_X86_64) || defined(AK_CPU_ARM_64)
#define AK_POINTER_64
#endif // #if defined(AK_CPU_X86_64) || defined(AK_CPU_ARM_64)

#if !defined(AK_UNUSEDVAR)
// use to mark variables as unused to avoid warnings
#define AK_UNUSEDVAR(x)	((void)(x))
#endif

#if defined(AK_SUPPORT_THREADS)
#define AK_THREAD_LOCAL thread_local
#else
#define AK_THREAD_LOCAL
#endif

// Helper macro to disable optimizations in a specific file (or part of a file)
// (no code using these macros should be checked in)
#if defined(__clang__)
#define AK_DISABLE_OPTIMIZATIONS    _Pragma("clang optimize off")
#define AK_ENABLE_OPTIMIZATIONS     _Pragma("clang optimize on")
#elif defined(__GNUC__)
#define AK_DISABLE_OPTIMIZATIONS    _Pragma("GCC optimize (\"O0\")")
#define AK_ENABLE_OPTIMIZATIONS     _Pragma("GCC optimize (\"O2\")")
#elif defined(_MSC_VER)
#define AK_DISABLE_OPTIMIZATIONS    __pragma(optimize("", off))
#define AK_ENABLE_OPTIMIZATIONS     __pragma(optimize("", on))
#endif
