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

// AkSimd.h

/// \file 
/// Simd definitions.

#pragma once

#include <AK/SoundEngine/Common/AkTypes.h>

/// Get the element at index __num__ in vector __vName
#define AKSIMD_GETELEMENT_V4F32( __vName, __num__ )			((AkReal32*)&(__vName))[(__num__)]
#define AKSIMD_GETELEMENT_V2F32( __vName, __num__ )			((AkReal32*)&(__vName))[(__num__)]
#define AKSIMD_GETELEMENT_V2F64( __vName, __num__ )			((AkReal64*)&(__vName))[(__num__)]
#define AKSIMD_GETELEMENT_V4I32( __vName, __num__ )			((AkInt32*)&(__vName))[(__num__)]
#define AKSIMD_GETELEMENT_V2I64( __vName, __num__ )			((AkInt64*)&(__vName))[(__num__)]

//////////////////////////////////////////////////////////////////////////
// Platform-specific section.

#if defined AK_CPU_X86 || defined AK_CPU_X86_64 || defined AK_CPU_WASM
	#include <AK/SoundEngine/Platforms/SSE/AkSimd.h>
#elif defined AK_CPU_ARM_NEON
	#include <AK/SoundEngine/Platforms/arm_neon/AkSimd.h>
#else
	#include <AK/SoundEngine/Platforms/Generic/AkSimd.h>
#endif


//////////////////////////////////////////////////////////////////////////
// Other helpers

#ifndef AKSIMD_ASSERTFLUSHZEROMODE
	#define AKSIMD_ASSERTFLUSHZEROMODE
#endif

// Helper macros and structs to declare vectors in an order that matches memory layout
struct AKSIMD_DECLARE_V4F32_TYPE
{
	AK_ALIGN_SIMD(AkReal32 f[4]);
	inline operator const AkReal32* () const noexcept { return f; }
	inline operator AKSIMD_V4F32() const noexcept { return AKSIMD_LOAD_V4F32(f); }
};
struct AKSIMD_DECLARE_V4I32_TYPE
{
	AK_ALIGN_SIMD(AkInt32 i[4]);
	inline operator const AkInt32* () const noexcept { return i; }
	inline operator AKSIMD_V4I32() const noexcept { return AKSIMD_LOAD_V4I32((AKSIMD_V4I32*)i); }
};
#define AKSIMD_SETVR_V2F64( _a, _b )         AKSIMD_SETV_V2F64( (_b), (_a) )
#define AKSIMD_SETVR_V4F32( _a, _b, _c, _d ) AKSIMD_SETV_V4F32( (_d), (_c), (_b), (_a) )
#define AKSIMD_SETVR_V2I64( _a, _b)          AKSIMD_SETV_V2I64( (_b), (_a) )
#define AKSIMD_SETVR_V4I32( _a, _b, _c, _d ) AKSIMD_SETV_V4I32( (_d), (_c), (_b), (_a) )
#define AKSIMD_DECLARE_V4F32( _x, _a, _b, _c, _d ) AKSIMD_DECLARE_V4F32_TYPE _x = { _a, _b, _c, _d };
#define AKSIMD_DECLARE_V4I32( _x, _a, _b, _c, _d ) AKSIMD_DECLARE_V4I32_TYPE _x = { _a, _b, _c, _d };

#define AKSIMD_SETELEMENT_V4F32( __vName__, __num__, __value__ ) ( AKSIMD_GETELEMENT_V4F32( __vName__, __num__ ) = (__value__) )

/// Rotate four vectors. After rotation:
///  A[3:0] = D[0] C[0] B[0] A[0]
///  B[3:0] = D[1] C[1] B[1] A[1]
///  C[3:0] = D[2] C[2] B[2] A[2]
///  D[3:0] = D[3] C[3] B[3] A[3]
AkForceInline void AKSIMD_TRANSPOSE4X4_V4F32(AKSIMD_V4F32 &A, AKSIMD_V4F32 &B, AKSIMD_V4F32 &C, AKSIMD_V4F32 &D)
{
	AKSIMD_V4F32 tmp1, tmp2, tmp3, tmp4;		
	tmp1 = AKSIMD_MOVELH_V4F32(A, B);
	tmp2 = AKSIMD_MOVEHL_V4F32(B, A);
	tmp3 = AKSIMD_MOVELH_V4F32(C, D);
	tmp4 = AKSIMD_MOVEHL_V4F32(D, C);

	A = AKSIMD_SHUFFLE_V4F32(tmp1, tmp3, AKSIMD_SHUFFLE(2, 0, 2, 0));
	B = AKSIMD_SHUFFLE_V4F32(tmp1, tmp3, AKSIMD_SHUFFLE(3, 1, 3, 1));
	C = AKSIMD_SHUFFLE_V4F32(tmp2, tmp4, AKSIMD_SHUFFLE(2, 0, 2, 0));
	D = AKSIMD_SHUFFLE_V4F32(tmp2, tmp4, AKSIMD_SHUFFLE(3, 1, 3, 1));
}

