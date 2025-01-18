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
/// Defines the API of Audiokinetic's I/O streaming solution.

#ifndef _AK_FILE_SYSTEM_FLAGS_H_
#define _AK_FILE_SYSTEM_FLAGS_H_

/// File system flags for file descriptors mapping.
struct AkFileSystemFlags
{
	AkFileSystemFlags()
		: uCompanyID(0)
		, uCodecID(0)
		, uCustomParamSize(0)
		, pCustomParam(0)
		, bIsLanguageSpecific(false)
		, bIsAutomaticStream(false)
		, uCacheID(AK_INVALID_FILE_ID)
		, uNumBytesPrefetch(0)
		, uDirectoryHash(AK_INVALID_FILE_ID) {}

	AkFileSystemFlags( AkUInt32 in_uCompanyID, AkUInt32 in_uCodecID, AkUInt32 in_uCustomParamSize, void * in_pCustomParam, bool in_bIsLanguageSpecific, AkCacheID in_uCacheID )
		: uCompanyID( in_uCompanyID )
		, uCodecID( in_uCodecID )
		, uCustomParamSize( in_uCustomParamSize )
		, pCustomParam( in_pCustomParam )
		, bIsLanguageSpecific( in_bIsLanguageSpecific )
		, uCacheID( in_uCacheID )
		, uNumBytesPrefetch( 0 )
		, uDirectoryHash(AK_INVALID_FILE_ID) {}

    AkUInt32            uCompanyID;         ///< Company ID (Wwise uses AKCOMPANYID_AUDIOKINETIC, defined in AkTypes.h, for soundbanks and standard streaming files, and AKCOMPANYID_AUDIOKINETIC_EXTERNAL for streaming external sources).
    AkUInt32            uCodecID;           ///< File/codec type ID (defined in AkTypes.h)
    AkUInt32            uCustomParamSize;   ///< Size of the custom parameter
    void *              pCustomParam;       ///< Custom parameter
    bool                bIsLanguageSpecific;///< True when the file location depends on language
	bool                bIsAutomaticStream;	///< True when the file is opened to be used as an automatic stream. Note that you don't need to set it. 
											///< If you pass an AkFileSystemFlags to IAkStreamMgr CreateStd|Auto(), it will be set internally to the correct value.
    AkCacheID			uCacheID;			///< Cache ID for caching system used by automatic streams. The user is responsible for guaranteeing unicity of IDs. 
											///< When set, it supersedes the file ID passed to AK::IAkStreamMgr::CreateAuto() (ID version). Caching is optional and depends on the implementation.
	AkUInt32			uNumBytesPrefetch;	///< Indicates the number of bytes from the beginning of the file that should be streamed into cache via a caching stream. This field is only relevant when opening caching streams via 
											///< AK::IAkStreamMgr::PinFileInCache() and AK::SoundEngine::PinEventInStreamCache().  When using AK::SoundEngine::PinEventInStreamCache(), 
											///< it is initialized to the prefetch size stored in the sound bank, but may be changed by the file location resolver, or set to 0 to cancel caching.
	AkUInt32            uDirectoryHash;     ///< If the implementation uses a hashed directory structure, this is the hash value that should be employed for determining the directory structure
};

#endif //_AK_FILE_SYSTEM_FLAGS_H_
