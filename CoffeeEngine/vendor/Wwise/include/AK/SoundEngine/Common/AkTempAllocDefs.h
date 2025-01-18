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
/// namespace for handling temp-allocations (incl. stack allocs)

#pragma once

#include <AK/SoundEngine/Common/AkSoundEngineExport.h>
#include <AK/SoundEngine/Common/AkTypes.h>

namespace AK
{
	/// TempAlloc namespace.
	/// \remarks The functions in this namespace are thread-safe, unless stated otherwise.
	/// \sa
	/// - \ref goingfurther_optimizingmempools_tempallocs
	namespace TempAlloc
	{
		////////////////////////////////////////////////////////////////////////
		/// @name TempAlloc systems
		//@{

		/// Temp-alloc memory statistics. Whenever these are fetched, they represent the last completed temp-alloc "tick".
		/// \remarks These statistics are not collected in the Release configuration of the memory mgr.
		struct Stats
		{
			AkUInt32 uMemUsed;          ///< Used memory (in bytes).
			AkUInt32 uMemAllocated;     ///< Allocated memory (in bytes).
			AkUInt32 uBlocksUsed;       ///< Number of individual blocks used.

			AkUInt32 uPeakMemUsed;      ///< The peak value for uMemUsed since initialization.
			AkUInt32 uPeakMemAllocated; ///< The peak value for uMemAllocated since initialization.
			AkUInt32 uPeakBlocksUsed;   ///< The peak value for uBlocksUsed since initialization.
			AkUInt32 uPeakBlockUsed;    ///< The peak amount of used memory in any single block since initialization.
		};

		/// IDs of temporary memory pools used by the sound engine.
		enum Type
		{
			Type_AudioRender,
			Type_NUM,
		};

		/// Lifetimes of temporary allocations available.
		enum Lifetime
		{
			Lifetime_CurrentTick = 0,
			Lifetime_DoubleTick = 1,
			Lifetime_NUM,
		};

		/// Initialization settings for temporary-memory pools. Separate settings are specified for each temporary-memory pool.
		/// \remarks The debug options are intended for monitoring and analyzing potential issues in usage of the TempAlloc system during development. Their functionality is specifically removed in Release configurations of the AkMemoryMgr.
		struct InitSettings
		{
			AkUInt32 uMinimumBlockCount;    ///< The number of blocks of memory the system is initialized with and is the minimum kept around forever. Defaults to 1. Higher values increase upfront memory use, but can reduce, or eliminate, the creation and destruction of memory blocks over time. 
			AkUInt32 uMinimumBlockSize;     ///< The minimum size of each block. If a new allocation requests a new block of memory, then the new block is the size of the requested allocation times four, and then rounded up to the next multiple of this value. Defaults to 512 KiB.
			AkUInt32 uMaximumUnusedBlocks;  ///< The maximum number of blocks that the system keeps in an unused state, and avoids freeing. Defaults to 1. Higher values do not increase the peak memory use, but do prevent unused memory from being freed, in order to reduce creation and destruction of memory blocks.

			bool bDebugDetailedStats;       ///< Enable to track detailed stats and include them in the detailed stat dump. Detailed stats include the size and quantity of each type of allocation from the system. Disabled by default.
			bool bDebugClearMemory;         ///< Enable to clear any allocation to a deterministic garbage value. Useful to make sure memory is initialized properly. Disabled by default.
			bool bDebugEnableSentinels;     ///< Enable to write out sentinels between most allocations to help detect memory overwrites, verified at the end of a tick. Enabled by default. Increases memory usage of blocks slightly.
			bool bDebugFlushBlocks;         ///< Enable to forcefully release all blocks at the end of a tick and recreate them from scratch every tick. Useful to ensure stale memory is not being accessed. Disabled by default. This might interfere with some stats reporting due to blocks being released between ticks.
			bool bDebugStandaloneAllocs;    ///< Enable to force the block size to be as small as possible for each allocation (smaller than can be achieved by just setting uMinimumBlockSize to very low values). Useful to investigate memory overruns in-depth, especially in conjunction with other options like bDebugFlushBlocks and the MemoryMgr's stomp allocator. If enabled, bDebugDetailedStats and bDebugEnableSentinels will be disabled. Greatly increases CPU and memory usage.
		};

		/// Get simple statistics for a given temporary-memory pool
		AK_EXTERNAPIFUNC(void, GetStats)(
			Type in_eType,      ///< Temporary-memory pool type.
			Stats& out_stats    ///< Returned statistics.
			);

		/// Get a detailed listing of the allocations into the temp-alloc pool, and output them to a file.
		/// \note TempAllocInitSettings::bTrackDetailedStats must be enabled for the specified type to get detailed information about the underlying allocs. Otherwise, only the simple stats are listed.
		AK_EXTERNAPIFUNC(void, DumpTempAllocsToFile)(
			Type in_eType,      ///< Temporary-memory pool type.
			const AkOSChar* pszFilename    ///< Filename.
			);
	}

	/// BookmarkAlloc namespace.
	/// \remarks The functions in this namespace are thread-safe, unless stated otherwise.
	/// \sa
	/// - \ref goingfurther_optimizingmempools_bookmarkalloc
	namespace BookmarkAlloc
	{
		////////////////////////////////////////////////////////////////////////
		/// @name BookmarkAlloc systems
		//@{

		/// Bookmark-allocator memory statistics. Whenever these are fetched, they represent whatever happened since the last tick
		/// \remarks These statistics are not collected in the Release configuration of the memory mgr.
		struct Stats
		{
			AkUInt32 uRecentPeakMemUsed;     ///< Peak used memory in a single BookmarkAlloc region since the last tick (in bytes). 
			AkUInt32 uRecentBlocksFetched;   ///< Number of times a block was fetched from the cache, not including the base block. High values here may indicate that block sizes need to be larger.
			AkUInt32 uMemAllocated;          ///< Currently allocated memory (in bytes).
			AkUInt32 uBlocksAllocated;       ///< Number of individual blocks currently allocated.

			AkUInt32 uPeakMemUsed;           ///< The peak value for uRecentPeakMemUsed since initialization.
			AkUInt32 uPeakMemAllocated;      ///< The peak value for uMemAllocated since initialization.
			AkUInt32 uPeakBlocksFetched;     ///< The peak value for uRecentBlocksFetched since initialization.
			AkUInt32 uPeakBlocksAllocated;   ///< The peak value for uBlocksAllocated since initialization.
			AkUInt32 uPeakBlockSize;         ///< The peak size of any single block since initialization.
		};

		/// Initialization settings for Bookmark-allocator memory.
		/// \remarks The debug options are intended for monitoring and analyzing potential issues in usage of the BookmarkAlloc system during development. Their functionality is specifically removed in Release configurations of the AkMemoryMgr.
		struct InitSettings
		{
			AkUInt32 uMinimumBlockCount;    ///< The number of blocks of memory the system is initialized with and is the minimum kept around forever. Defaults to 1. Higher values increase upfront memory use, but can reduce, or eliminate, the creation and destruction of memory blocks over time. 
			AkUInt32 uMinimumBlockSize;     ///< The minimum size of each block. If a new allocation requests a new block of memory, then the new block is the size of the requested allocation times four, and then rounded up to the next multiple of this value. Defaults to 64 KiB.
			AkUInt32 uMaximumUnusedBlocks;  ///< The maximum number of blocks that the system keeps in an unused state, and avoids freeing. Defaults to 1. Higher values do not increase the peak memory use, but do prevent unused memory from being freed, in order to reduce creation and destruction of memory blocks.
						
			bool bDebugDetailedStats;       ///< Enable to track detailed stats, specifically collection of Stats::uRecentPeakMemUsed. Enabled by default.
			bool bDebugClearMemory;         ///< Enable to clear any allocation to a deterministic garbage value during allocs, and after the stack is rewound to a bookmark. Useful to make sure memory is initialized properly. Disabled by default.
			bool bDebugEnableSentinels;     ///< Enable to write out sentinels between most allocations to help detect memory overwrites, which are verified at the termination of a bookmark alloc region. Enabled by default. Increases memory usage of blocks slightly.
			bool bDebugStandaloneAllocs;    ///< Enable to force the block size to be as small as possible for each allocation (smaller than can be achieved by just setting uMinimumBlockSize to very low values). Useful to investigate memory overruns in-depth, especially in conjunction with the MemoryMgr's stomp allocator. If enabled, bDebugEnableSentinels will be disabled. Greatly increases CPU and memory usage.
		};

		/// Get simple statistics for the Bookmark allocator
		AK_EXTERNAPIFUNC(void, GetStats)(
			Stats& out_stats    ///< Returned statistics.
			);
	}
}

