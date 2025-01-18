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

// AkAtomicSpinFuncs.h

// Helper functions for some simple spin-locks using atomics.
 
#pragma once

#include <AK/SoundEngine/Common/AkAtomic.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

#if defined(AK_MONITORX_SUPPORTED)

#if (defined __clang__ || defined __GNUC__)
#include <x86intrin.h>
#elif defined(_MSC_VER)
#include <intrin.h>
#endif

#endif // defined(AK_MONITORX_SUPPORTED)

#if !defined(AK_NULL_PLATFORM)
/// Platform-dependent helpers
namespace AKPLATFORM
{
#if defined(AK_MONITORX_SUPPORTED)
	// monitorx and waitx are available on certain AMD CPUs,
	//  so we can have a custom impl of AkLimitedSpinForZero, and AkLimitedSpinToAcquire,
	// which can slightly improve performance when spinlock is tripped

	// Waits for a limited amount of time for in_pVal to hit zero (without yielding the thread)
	inline void AkLimitedSpinForZero(AkAtomic32* in_pVal)
	{
		AkInt64 endSpinTime = 0;
		AkInt64 currentTime = 0;
		PerformanceCounter(&endSpinTime);
		endSpinTime += AkInt64(AK::g_fFreqRatio * 0.01); // only spin for about 10us
		while (true)
		{
			// set up monitorx on pVal
			_mm_monitorx((void*)in_pVal, 0U, 0U);
			// if pval is zero, skip out
			if (AkAtomicLoad32(in_pVal) == 0)
			{
				break;
			}
			// wait until a store to pVal occurs (or ~1us passes)
			_mm_mwaitx(2U, 0U, 1000U);

			// Check if we've hit the deadline for the timeout
			PerformanceCounter(&currentTime);
			if (currentTime > endSpinTime)
			{
				break;
			}
		}
	}

	// Waits for a limited amount of time for in_pVal to get atomically shift from the expected value to the proposed one
	// returns true if acquisition succeeded
	inline bool AkLimitedSpinToAcquire(AkAtomic32* in_pVal, AkInt32 in_proposed, AkInt32 in_expected)
	{
		if (AkAtomicCas32(in_pVal, in_proposed, in_expected))
		{
			return true;
		}

		// Cas failed, start the slower evaluation

		AkInt64 endSpinTime = 0;
		AkInt64 currentTime = 0;
		PerformanceCounter(&endSpinTime);
		endSpinTime += AkInt64(AK::g_fFreqRatio * 0.01); // only spin for about 10us
		while (true)
		{
			// set up monitorx on pVal
			_mm_monitorx((void*)in_pVal, 0U, 0U);
			// attempt cas to acquire and if successful, skip out
			if (AkAtomicCas32(in_pVal, in_proposed, in_expected))
			{
				return true;
			}
			// wait until a store to pVal occurs (or ~1us passes)
			_mm_mwaitx(2U, 0U, 1000U);

			// Check if we've hit the deadline for the timeout
			PerformanceCounter(&currentTime);
			if (currentTime > endSpinTime)
			{
				return false;
			}
		}
	}
#else
	// Waits for a limited amount of time for in_pVal to hit zero (without yielding the thread)
	inline void AkLimitedSpinForZero(AkAtomic32* in_pVal)
	{
		AkInt64 endSpinTime = 0;
		AkInt64 currentTime = 0;
		PerformanceCounter(&endSpinTime);
		endSpinTime += AkInt64(AK::g_fFreqRatio * 0.01); // only spin for about 10us
		while (true)
		{
			// if pval is zero, skip out
			if (AkAtomicLoad32(in_pVal) == 0)
			{
				break;
			}
			AkSpinHint();

			// Check if we've hit the deadline for the timeout
			PerformanceCounter(&currentTime);
			if (currentTime > endSpinTime)
			{
				break;
			}
		}
	}

	// Waits for a limited amount of time for in_pVal to get atomically shift from the expected value to the proposed one
	// returns true if acquisition succeeded
	inline bool AkLimitedSpinToAcquire(AkAtomic32* in_pVal, AkInt32 in_proposed, AkInt32 in_expected)
	{
		if (AkAtomicCas32(in_pVal, in_proposed, in_expected))
		{
			return true;
		}

		// Cas failed, start the slower evaluation
		AkInt64 endSpinTime = 0;
		AkInt64 currentTime = 0;
		PerformanceCounter(&endSpinTime);
		endSpinTime += AkInt64(AK::g_fFreqRatio * 0.01); // only spin for about 10us
		while (true)
		{
			// attempt cas to acquire and if successful, skip out
			if (AkAtomicCas32(in_pVal, in_proposed, in_expected))
			{
				return true;
			}
			AkSpinHint();

			// Check if we've hit the deadline for the timeout
			PerformanceCounter(&currentTime);
			if (currentTime > endSpinTime)
			{
				return false;
			}
		}
	}
#endif // !defined(AK_MONITORX_SUPPORTED)

	inline void AkSpinWaitForZero(AkAtomic32* in_pVal)
	{
		if (AkAtomicLoad32(in_pVal) == 0)
		{
			return;
		}

		// do a limited spin on-the-spot until in_pVal hits zero
		AkLimitedSpinForZero(in_pVal);

		// if in_pVal is still non-zero, then the other thread is either blocked or waiting for us.  Yield for real.
		while (AkAtomicLoad32(in_pVal))
			AkThreadYield();
	}

	// Waits for a limited amount of time for in_pVal to get atomically shift from 0 to 1
	inline void AkSpinToAcquire(AkAtomic32* in_pVal, AkInt32 in_proposed, AkInt32 in_expected)
	{
		// do a limited spin on-the-spot until in_pVal can successfully hit 1
		// or if it fails, then the other thread is either blocked or waiting for us.  Yield for real.
		while (!AkLimitedSpinToAcquire(in_pVal, in_proposed, in_expected))
		{
			AkThreadYield();
		}
	}
}
#endif