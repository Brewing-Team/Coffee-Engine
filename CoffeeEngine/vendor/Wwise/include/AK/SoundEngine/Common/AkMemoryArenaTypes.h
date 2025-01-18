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
/// namespace for handling Wwise internal memory arena.

#pragma once

#include <AK/SoundEngine/Common/AkSoundEngineExport.h>
#include <AK/SoundEngine/Common/AkTypes.h>

typedef void* (AKSOUNDENGINE_CALL* AkAllocSpan) (
	size_t in_uSize,
	size_t* out_userData
	);
typedef void (AKSOUNDENGINE_CALL* AkFreeSpan) (
	void* in_pAddress,
	size_t in_uSize,
	size_t in_userData
	);

namespace AK
{
	namespace MemoryArena
	{
		// the maximum size of the memory arena (provided externally for static code allocation, or other dynamically allocated heaps)
		const AkUInt32 kMemoryArenaSize = 8192;

		// The overhead attached to 'huge' allocs, or spans allocated by the AkMemoryArena.
		// Typically useful to slightly lower the size of ordinarily 'huge' allocations, to more precisely fit the page size, e.g.
		//     Malloc(pArena, 16 * 1024 * 1024 - kMemoryArenaSpanOverhead);
		// will return an allocation that is exactly 16MiB in size, whereas,
		//     Malloc(pArena, 16 * 1024 * 1024);
		// will return an allocation that is 18MiB in size, if MemoryArenaSettings::uPageSize is 2MiB
		const AkUInt32 kMemoryArenaSpanOverhead = 64;

		struct AkMemoryArenaSettings
		{
			bool bEnableSba; // Used to determine if the SbaHeap should be initialized and utilized at all. If disabled, all small allocs (size < kAllocSizeMedium) will go into the TlsfHeap and be treated as "Medium" allocs.
			AkUInt32 uSbaInitSize; // the size of the initial allocation of memory for the small block allocator (SBA). This allocation will be made inside the main "tlsf" heap, and will always persist. Sub-allocations that fit in this range will have better fragmentation characteristics, and an overall reduction in memory overhead. This does not have to be a power-of-two.
			AkUInt32 uSbaSpanSize; // the size of each span of memory for the SBA. Each span has a unique size class. Lower values can slightly increase the overhead of initializing SBA allocations, but can reduce overall memory reservation. This must be a power-of-two.
			AkUInt32 uSbaMaximumUnusedSpans; // The maximum number of SBA spans that the system keeps in an unused state, and avoids freeing. Defaults to 1. Higher values do not increase the peak memory use, but do prevent unused memory from being freed, in order to reduce creation and destruction of SBA spans.

			AkUInt32 uTlsfInitSize; // the size of the initial span of memory requested for the main tlsf heap. This span will always persist. (does not have to be a power-of-two)
			AkUInt32 uTlsfSpanSize; // when a memory allocation cannot fit in the main tlsf heap, and is a medium-sized allocation, new spans requested will be a multiple of this size (does not have to be a power-of-two)
			AkUInt32 uTlsfLargeSpanSize; // when a memory allocation cannot fit in the main tlsf heap, and is a large-sized allocation, new spans requested will be a multiple of this size (does not have to be a power-of-two)
			AkUInt32 uTlsfSpanOverhead; // the amount of allocation 'overhead' assumed for each span. When new spans for Tlsf are requested, this is subtracted from the size of the requested span, after multiplying the requested size up by uPageSize. Defaults to 128.
			AkUInt32 uTlsfMaximumUnusedMediumSpans; // The maximum number of "medium spans" that the system keeps in an unused state, and avoids freeing. Defaults to 1. Higher values do not increase the peak memory use, but do prevent unused memory from being freed, in order to reduce creation and destruction of medium spans.
			AkUInt32 uTlsfMaximumUnusedLargeSpans; // The maximum number of "large spans" that the system keeps in an unused state, and avoids freeing. Defaults to 1. Higher values do not increase the peak memory use, but do prevent unused memory from being freed, in order to reduce creation and destruction of large spans.

			AkUInt32 uAllocSizeLarge; // defines the minimum size for an allocation to qualify as "Large". Default to UINT_MAX, so that all large allocs are treated as Medium allocs. Large allocs go into best-fit searches, same as medium allocations, but will go to a separate list of secondary spans, distinct from "Medium" allocations
			AkUInt32 uAllocSizeHuge; // defines the minimum size for an allocation to qualify as "Huge". Huge allocs always go into standalone spans even if there is space available in an existing span
			
			AkUInt32 uMemReservedLimit; // the limit on how much memory will be reserved by this arena. If a request for to reserve more memory is made that would go over this limit, a nullptr is returned. If set to zero, no limit is in place.

			AkAllocSpan fnMemAllocSpan; // called when the arena needs a new span of memory, including the initial one requested. Set to nullptr to disable initialization of the arena.
			AkFreeSpan fnMemFreeSpan; // called when the arena is releasing a span of memory
		};
	}
}
