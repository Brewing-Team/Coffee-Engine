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

/**
 * \brief Wwise Authoring Plug-ins - Plug-in API for property sets.
 * \file AK/Wwise/Plugin/HostPropertySet.h
 */

#pragma once

#include "PluginInfoGenerator.h"
#include "./V1/HostPropertySet.h"

/// \copydoc ak_wwise_plugin_host_property_set_v1
struct ak_wwise_plugin_host_property_set_v2
#ifdef __cplusplus
	: public ak_wwise_plugin_host_property_set_v1
#endif
{
#ifndef __cplusplus
	struct ak_wwise_plugin_host_property_set_v1;
#endif

#ifdef __cplusplus
	/// This interface is compatible with the v1 instance.
	using Instance = ak_wwise_plugin_host_property_set_instance_v1;
	ak_wwise_plugin_host_property_set_v2() :
		ak_wwise_plugin_host_property_set_v1(/* in_version =*/ 2)
	{}
#endif

	/// \brief Open one or more brace that denotes the start of a change
	/// \param[in] in_bracesToOpen A bit field composed of flags defined by \c ak_wwise_plugin_property_set_braces_values
	void(*OpenBraces)(
		struct ak_wwise_plugin_host_property_set_instance_v1* in_this,
		ak_wwise_plugin_property_set_braces in_bracesToOpen
	);

	/// \brief Close one or more brace that denotes the end of a change
	/// \param[in] in_bracesToClose A bit field composed of flags defined by \c ak_wwise_plugin_property_set_braces_values
	void(*CloseBraces)(
		struct ak_wwise_plugin_host_property_set_instance_v1* in_this,
		ak_wwise_plugin_property_set_braces in_bracesToClose
	);

	/// \brief Retrieve the currently open braces
	/// \return A bit field composed of flags defined by \c ak_wwise_plugin_property_set_braces_values
	ak_wwise_plugin_property_set_braces(*GetOpenedBraces)(
		const struct ak_wwise_plugin_host_property_set_instance_v1* in_this
	);
};

#define AK_WWISE_PLUGIN_HOST_PROPERTY_SET_V2_ID() \
	AK_WWISE_PLUGIN_BASE_INTERFACE_FROM_ID(AK_WWISE_PLUGIN_INTERFACE_TYPE_HOST_PROPERTY_SET, 2)
#define AK_WWISE_PLUGIN_HOST_PROPERTY_SET_V2_CTOR() \
{ \
	.m_baseInterface = AK_WWISE_PLUGIN_HOST_PROPERTY_SET_V2_ID() \
}

#ifdef __cplusplus
namespace AK::Wwise::Plugin
{
	namespace V2
	{
		using CHostPropertySet = ak_wwise_plugin_host_property_set_v2;

		/// \copydoc ak_wwise_plugin_host_property_set_v2
		class PropertySet : public AK::Wwise::Plugin::V1::PropertySetBase<CHostPropertySet, 2>
		{
		public:
			using Interface = CHostPropertySet;
			using Instance = CHostPropertySet::Instance;

			/// \brief Open one or more brace that denotes the start of a change
			/// \param[in] in_bracesToOpen A bit field composed of flags defined by ak_wwise_plugin_property_set_braces
			inline void OpenBraces(
				ak_wwise_plugin_property_set_braces in_bracesToOpen
			)
			{
				g_cinterface->OpenBraces(this, in_bracesToOpen);
			}

			/// \brief Close one or more brace that denotes the end of a change
			/// \param[in] in_bracesToClose A bit field composed of flags defined by ak_wwise_plugin_property_set_braces
			inline void CloseBraces(
				ak_wwise_plugin_property_set_braces in_bracesToClose
			)
			{
				g_cinterface->CloseBraces(this, in_bracesToClose);
			}

			/// \brief Retrieve the currently open braces
			/// \return A bit field composed of flags defined by \c ak_wwise_plugin_property_set_braces_values
			inline ak_wwise_plugin_property_set_braces GetOpenedBraces() const
			{
				return g_cinterface->GetOpenedBraces(this);
			}

		#if defined( DOXYGEN_INCLUDE )
			bool GetValue(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				AK::WwiseAuthoringAPI::AkVariantBase& out_varProperty
			) const;
			bool GetValue(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				AK::WwiseAuthoringAPI::AkVariantBase& out_varProperty
			) const;
			bool SetValue(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				const AK::WwiseAuthoringAPI::AkVariantBase& in_varProperty
			);
			bool HasPropertyValue(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			bool PropertyHasRTPC(
				const char * in_pszPropertyName
			) const;
			bool PropertyHasState(
				const char* in_pszPropertyName
			) const;
			bool PropertyHasLinked(
				const char* in_pszPropertyName
			) const;
			bool PropertyHasUnlinked(
				const char* in_pszPropertyName
			) const;
			bool PropertyPlatformIsLinked(
				const GUID& in_guidPlatform,
				const char* in_pszPropertyName
			) const;
			const GUID* GetID() const;
			int GetType(
				const GUID& in_guidPlatform,
				const char* in_pszPropertyName
			) const;
			bool ClearValue(
				const GUID& in_guidPlatform,
				const char* in_pszPropertyName
			);
			bool SetValueString(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				const char * in_propertyValue
			);
			bool SetValueInt64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int64_t in_propertyValue
			);
			bool SetValueInt32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int32_t in_propertyValue
			);
			bool SetValueInt16(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int16_t in_propertyValue
			);
			bool SetValueInt8(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int8_t in_propertyValue
			);
			bool SetValueUInt64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint64_t in_propertyValue
			);
			bool SetValueUInt32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint32_t in_propertyValue
			);
			bool SetValueUInt16(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint16_t in_propertyValue
			);
			bool SetValueUInt8(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint8_t in_propertyValue
			);
			bool SetValueReal64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				double in_propertyValue
			);
			bool SetValueReal32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				float in_propertyValue
			);
			bool SetValueBool(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				bool in_propertyValue
			);
			bool GetValueString(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				const char*& out_propertyValue
			) const;
			bool GetValueInt64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int64_t& out_propertyValue
			) const;
			bool GetValueInt32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int32_t& out_propertyValue
			) const;
			bool GetValueInt16(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int16_t& out_propertyValue
			) const;
			bool GetValueInt8(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				int8_t& out_propertyValue
			) const;
			bool GetValueUInt64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint64_t& out_propertyValue
			) const;
			bool GetValueUInt32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint32_t& out_propertyValue
			) const;
			bool GetValueUInt16(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint16_t& out_propertyValue
			) const;
			bool GetValueUInt8(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				uint8_t& out_propertyValue
			) const;
			bool GetValueReal64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				double& out_propertyValue
			) const;
			bool GetValueReal32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				float& out_propertyValue
			) const;
			bool GetValueBool(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName,
				bool& out_propertyValue
			) const;
			const char* GetString(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			int64_t GetInt64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			int32_t GetInt32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			int16_t GetInt16(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			int8_t GetInt8(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			uint64_t GetUInt64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			uint32_t GetUInt32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			uint16_t GetUInt16(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			uint8_t GetUInt8(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			double GetReal64(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			float GetReal32(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
			bool GetBool(
				const GUID & in_guidPlatform,
				const char * in_pszPropertyName
			) const;
		#endif
		};

		/**
		 * \brief Requests a PropertySet interface, provided as m_propertySet variable.
		 * 
		 * Deriving your plug-in class from RequestPropertySet will automatically request both PropertySet and
		 * Notifications::PropertySet_ interfaces. From this point, you will be able to derive from the virtual
		 * functions as defined in Notifications::PropertySet_, and access the host-provided functions in the
		 * `m_propertySet` variable.
		 * 
		 * This is also useful when you want to access inner property sets from the ObjectStore interface.
		 * In that case, you can ignore the m_propertySet variable and the notifications interface.
		 * 
		 * \aknote The AK::Wwise::Plugin::RequestPropertySet and AK::Wwise::Plugin::RequestWrite classes are automatically
		 * derived when providing AudioPlugin in C++. \endaknote
		 */
		using RequestPropertySet = RequestedHostInterface<PropertySet>;
	} // of namespace V2

	/// Latest version of the C PropertySet interface.
	using CHostPropertySet = V2::CHostPropertySet;
	/// Latest version of the C++ PropertySet interface.
	using PropertySet = V2::PropertySet;
	/// Latest version of the requested C++ PropertySet interface.
	using RequestPropertySet = V2::RequestPropertySet;

	AK_WWISE_PLUGIN_SPECIALIZE_HOST_INTERFACE(PropertySet, propertySet,, public Notifications::PropertySet);
	AK_WWISE_PLUGIN_SPECIALIZE_INTERFACE_CLASS(PropertySet);
	AK_WWISE_PLUGIN_SPECIALIZE_INTERFACE_VERSION(PropertySet);
} // of namespace AK::Wwise::Plugin

#endif
