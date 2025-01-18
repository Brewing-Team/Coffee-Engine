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
/// Plug-in interface for temp-alloc systems

#pragma once

#include <AK/SoundEngine/Common/IAkPlugin.h>
#include <AK/SoundEngine/Common/IAkPluginMemAlloc.h>

namespace AK
{

/// Interface for performing extremely-fast zero-fragmentation temporary allocations,
/// e.g. allocations that exist for one or two ticks, or allocations that exist for a single stack frame.
/// These use the existing TempAlloc infrastructure in the core soundengine, whose behaviour and allocation
/// characteristics are defined in the MemoryMgr initialization settings.
///
/// For temporary allocations, types and lifetimes can be specified for mallocations -- or reallocs.
/// These allocations will only persist for the given soundengine tick (Lifetime_CurrentTick) and be destroyed
/// at the beginning of the next tick, or will persist for two soundengine ticks.
/// 
/// For bookmark allocations, you need to mark the start of a bookmark-allocation Region and perform allocations 
/// that persist until the bookmark-allocation region is closed. A struct, "PluginBookmarkAllocRegion" has
/// been provided to help with that.
/// 
/// Because these allocations are guaranteed to be destroyed collectively and simultaneously as a part
/// of their respective system, there is no fragmentation incurred by this, and no 'free' function is available.
/// Also, these allocations are also extremely fast, with no multi-threading contention in most cases, and
/// the total overhead of each alloc measured on the order of dozens of cycles.
/// 
/// \sa
/// - \ref goingfurther_optimizingmempools_bookmarkalloc

class IAkPluginServiceTempAlloc : public IAkPluginService
{
protected:
	virtual ~IAkPluginServiceTempAlloc() {}
public:

	// Returns an IAkPluginMemAlloc interface to perform temp allocations for the current Audio Render tick
	// This is static for the duration of the process, so it can be cached once during init.
	// Note that this interface is provided only for convenience with other systems; the "Free" operation in
	// this interface is a no-op, and instead all memory is released at the end of the current audioRender tick.
	virtual IAkPluginMemAlloc* GetTempAllocAudioRenderCurrent() = 0;

	// Returns an IAkPluginMemAlloc interface to perform temp allocations for the current and next Audio Render tick
	// This is static for the duration of the process, so it can be cached once during init.
	// Note that this interface is provided only for convenience with other systems; the "Free" operation in
	// this interface is a no-op, and instead all memory is released at the end of the next audioRender tick.
	virtual IAkPluginMemAlloc* GetTempAllocAudioRenderDouble() = 0;

	// Returns an IAkPluginMemAlloc interface to perform 'bookmark' allocations.
	// This is static for the duration of the process, so it can be cached once during init.
	// Note that this interface is provided only for convenience with other systems; the "Free" operation in
	// this interface is a no-op, and instead all memory is released at the end of the BookmarkAlloc Region
	virtual IAkPluginMemAlloc* GetBookmarkAlloc() = 0;

private:
	friend struct PluginBookmarkAllocRegion;

	// Call to mark the start of a bookmark alloc region. This must be called before any usage of the BookmarkAlloc memalloc interface.
	// The bookmark pointer returned must be used in the equivalent call to EndBookmarkAllocRegion.
	// This should only be used by the PluginBookmarkAllocRegion helper struct, below.
	virtual void* BeginBookmarkAllocRegion() = 0;

	// Call to mark the end of a bookmark alloc region. Rewinds the stack usage back to where BeginBookmarkAllocRegion started
	// This must be called for each call to BeginBookmarkAllocRegion.
	// This should only be used by the PluginBookmarkAllocRegion helper struct, below.
	virtual void EndBookmarkAllocRegion(void* in_pBookmark) = 0;
};

// Helper struct to be used for performing any bookmark allocations.
// One of these must be instantiated before any usage of the BookmarkAlloc IAkPluginMemAlloc interface.
// Automatically rewinds the bookmarkAlloc memory back to the point where this was instantiated, when this leaves the scope.
struct PluginBookmarkAllocRegion
{
	PluginBookmarkAllocRegion(IAkPluginServiceTempAlloc* in_pTempAlloc)
	{
		m_pTempAlloc = in_pTempAlloc;
		m_pBookmark = in_pTempAlloc->BeginBookmarkAllocRegion();
	}
	~PluginBookmarkAllocRegion()
	{
		EndRegion();
	}
	void EndRegion()
	{
		m_pTempAlloc->EndBookmarkAllocRegion(m_pBookmark);
		m_pBookmark = nullptr;
	}
private:
	void* m_pBookmark;
	IAkPluginServiceTempAlloc* m_pTempAlloc;
};

#define AK_GET_PLUGIN_SERVICE_TEMPALLOC(plugin_ctx) static_cast<AK::IAkPluginServiceTempAlloc*>(plugin_ctx->GetPluginService(AK::PluginServiceType_TempAlloc))

} // namespace AK
