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

// AkAudioObjectDefs.h

/// Definition of data structures for AkAudioObject

#pragma once

#include <AK/SoundEngine/Common/AkCommonDefs.h>
#include <AK/SoundEngine/Common/IAkPluginMemAlloc.h>
#include <AK/Tools/Common/AkArray.h>
#include <AK/Tools/Common/AkString.h>

namespace AK
{
	class IAkPluginParam;
}

/// An audio object refers to an audio signal with some attached metadata going through the sound engine pipeline.
/// The AkAudioObject struct encapsulates the metadata part. The signal itself is contained in a separate AkAudioBuffer instance.
struct AkAudioObject
{
	/// Constructor
	AkAudioObject()
		:key(AK_INVALID_AUDIO_OBJECT_ID)
		,cumulativeGain(1.f, 1.f)
		,instigatorID(AK_INVALID_PIPELINE_ID)
		,priority(AK_DEFAULT_PRIORITY)
	{}

	/// Destructor
	~AkAudioObject()
	{
		arCustomMetadata.Term();
		objectName.Term();
	}

	static const AkUInt64 kObjectKeyNumBits = 56;
	static const AkUInt64 kObjectKeyMask = (((AkUInt64)1 << kObjectKeyNumBits) - 1);

	AkAudioObjectID key;			///< Unique ID, local to a given bus. Only the lower 56 of 64 bits are used for the object itself. The highest 8 bits are available for channel indexing.

	AkPositioningData positioning;	///< Positioning data for deferred 3D rendering.
	AkRamp cumulativeGain;			///< Cumulative ramping gain to apply when mixing down to speaker bed or final endpoint
	AkPipelineID instigatorID;		///< Profiling ID of the node from which the object stems (typically the voice, instance of an actor-mixer).
	AkPriority priority;			///< Audio object playback priority. Object with a higher priority will be rendered using the hardware's object functionality on platforms that supports it, whereas objects with a lower priority will be downmixed to a lower resolution 3D bed. Audio object priorities should be retrieved, or set through IAkPluginServiceAudioObjectPriority to retain compatibility with future Wwise releases.

	/// Custom object metadata.
	struct CustomMetadata
	{
		AkPluginID pluginID;		///< Full plugin ID, including company ID and plugin type. See AKMAKECLASSID macro.
		AkUniqueID contextID;		///< (Profiling) ID of the sound or bus from which the custom metadata was fetched.
		AK::IAkPluginParam* pParam;	///< Custom, pluggable medata.  Note: any custom metadata is expected to exist for only the current sound engine render tick, and persistent references to it should not be stored.
	};

	/// Array type for carrying custom metadata.
	class ArrayCustomMetadata : public AkArray<CustomMetadata, const CustomMetadata&, AkPluginArrayAllocator>
	{
	public:
		using ArrayType = AkArray<CustomMetadata, const CustomMetadata&, AkPluginArrayAllocator>;

		ArrayType::Iterator FindByPluginID(AkPluginID pluginID) const
		{
			for (auto it = Begin(); it != End(); ++it)
			{
				if ((*it).pluginID == pluginID)
					return it;
			}
			return End();
		}
	};

	ArrayCustomMetadata arCustomMetadata;	///< Array of custom metadata, gathered from visited objects. Note: any custom metadata is expected to exist for only the current sound engine render tick, and persistent references to it should not be stored.

	typedef AkString<AkPluginArrayAllocator, char> String;	///< String type for use in 3D audio objects.
	String objectName;				///< Name string of the object, to appear in the object profiler. This is normally used by out-of-place object processors for naming their output objects. Built-in sound engine structures don't use it.

	/// Copies object metadata (everything but the key) from another object.
	void CopyContents(
		const AkAudioObject& in_src	///< Object from which metadata is copied.
	)
	{
		positioning = in_src.positioning;
		cumulativeGain = in_src.cumulativeGain;
		instigatorID = in_src.instigatorID;
		priority = in_src.priority;
		arCustomMetadata.Copy(in_src.arCustomMetadata);
		objectName = in_src.objectName;	// AkString performs a shallow copy when it can, like here.
	}
	
	/// Moves object metadata (everything but the key) from another object.
	void TransferContents(
		AkAudioObject& in_src	///< Object from which metadata is moved.
	)
	{
		positioning = in_src.positioning;
		cumulativeGain = in_src.cumulativeGain;
		instigatorID = in_src.instigatorID;
		priority = in_src.priority;
		arCustomMetadata.Transfer(in_src.arCustomMetadata);
		objectName.Transfer(in_src.objectName);
	}

	void SetCustomMetadata(CustomMetadata* in_aCustomMetadata, AkUInt32 in_uLength)
	{
		if (arCustomMetadata.Resize(in_uLength))
		{
			for (int i = 0; i < (int)in_uLength; ++i)
			{
				arCustomMetadata[i] = in_aCustomMetadata[i];
			}
		}
	}

	/// Transfer function for transfer move policies.
	void Transfer(
		AkAudioObject& in_from	///< Object from which data is transferred.
	)
	{
		key = in_from.key;
		TransferContents(in_from);
	}

	/// Object processors may give an explicit name to objects. 
	/// \return AK_Success if the string was allocated successfully, AK_InsufficientMemory otherwise.
	AKRESULT SetName(
		AK::IAkPluginMemAlloc* in_pAllocator,	///< Memory allocator.
		const char* in_szName					///< Null-terminated string to allocate and store on this object.
	)
	{
		objectName.Init(in_pAllocator);
		objectName = in_szName;
		return objectName.AllocCopy();
	}

	/// Reset object state in preparation for next frame.
	void ResetState()
	{
		arCustomMetadata.Term(); // Reset custom metadata in preparation for next frame.
		objectName.ClearReference(); // Clear reference to string in preparation for next frame.
	}
};

/// A collection of audio objects. Encapsulates the audio data and metadata of each audio object in separate arrays.
struct AkAudioObjects
{
	AkAudioObjects(AkUInt32 in_uNumObjects = 0, AkAudioBuffer** in_ppObjectBuffers = nullptr, AkAudioObject** in_ppObjects = nullptr)
		: uNumObjects(in_uNumObjects)
		, ppObjectBuffers(in_ppObjectBuffers)
		, ppObjects(in_ppObjects)
	{}

	AkUInt32 uNumObjects;				///< Number of audio objects.
	AkAudioBuffer** ppObjectBuffers;	///< Array of pointers to audio object buffers.
	AkAudioObject** ppObjects;			///< Array of pointers to audio objects.
};

