/*

Copyright 2015-2020 Igor Petrovic

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include <inttypes.h>
#include "core/src/general/Helpers.h"
#include "Layout.h"

///
/// \brief Helper macro for easier entry and exit from system block.
/// Important: ::init must called before trying to use this macro.
///
#define SYSTEM_BLOCK_ENTER(code)                                                    \
    {                                                                               \
        setStartAddress(0);                                                         \
        LESSDB::setLayout(dbLayout, static_cast<uint8_t>(block_t::AMOUNT) + 1);     \
        code                                                                        \
            LESSDB::setLayout(&dbLayout[1], static_cast<uint8_t>(block_t::AMOUNT)); \
        setStartAddress(userDataStartAddress + (lastPresetAddress * activePreset)); \
    }

///
/// \brief Initializes database.
///
bool Database::init()
{
    if (!LESSDB::init())
        return false;

    setStartAddress(0);

    uint32_t systemBlockUsage = 0;

    //set only system block for now
    if (!LESSDB::setLayout(dbLayout, 1))
    {
        return false;
    }
    else
    {
        systemBlockUsage     = LESSDB::currentDBsize();
        userDataStartAddress = LESSDB::nextParameterAddress();

        //now set the entire layout
        if (!LESSDB::setLayout(dbLayout, static_cast<uint8_t>(block_t::AMOUNT) + 1))
            return false;
    }

    lastPresetAddress = LESSDB::nextParameterAddress() - userDataStartAddress;

    //limit the address space to 0xFFFF - 1
    const uint32_t maxAddress = 0xFFFF - 1;
    uint16_t       maxPresets = (maxAddress / LESSDB::lastParameterAddress()) - userDataStartAddress;

    //get theoretical maximum of presets
    supportedPresets = (LESSDB::dbSize() - systemBlockUsage) / (LESSDB::currentDBsize() - systemBlockUsage);

    //limit by address space
    supportedPresets = CONSTRAIN(supportedPresets, 0, maxPresets);

    //limit by hardcoded limit
    supportedPresets = CONSTRAIN(supportedPresets, 0, MAX_PRESETS);

    bool returnValue = true;

    if (!isSignatureValid())
    {
        returnValue = factoryReset();
    }
    else
    {
        SYSTEM_BLOCK_ENTER(
            activePreset = read(0,
                                static_cast<uint8_t>(SectionPrivate::system_t::presets),
                                static_cast<size_t>(System::presetSetting_t::activePreset));)

        if (getPresetPreserveState())
        {
            //don't write anything to database in this case - setup preset only internally
            setPresetInternal(activePreset);
        }
        else
        {
            if (activePreset != 0)
            {
                //preset preservation is not set which means preset must be 0
                //in this case it is not so overwrite it with 0
                setPreset(0);
            }
            else
            {
                setPresetInternal(0);
            }
        }
    }

    if (returnValue)
    {
        initialized = true;
        handlers.initialized();
    }

    return returnValue;
}

bool Database::isInitialized()
{
    return initialized;
}

///
/// \brief Performs full factory reset of data in database.
///
bool Database::factoryReset()
{
    handlers.factoryResetStart();

    if (!clear())
        return false;

    if (initializeData)
    {
        for (int i = supportedPresets - 1; i >= 0; i--)
        {
            if (!setPresetInternal(i))
                return false;

            if (!initData(LESSDB::factoryResetType_t::full))
                return false;

            //perform custom init as well
            customInitGlobal();
            customInitButtons();
            customInitEncoders();
            customInitAnalog();
            customInitLEDs();
            customInitDisplay();
            customInitTouchscreen();
        }

        if (!setPresetPreserveState(false))
            return false;

        if (!setDbUID(getDbUID()))
            return false;

        if (!setPreset(0))
            return false;
    }
    else
    {
        if (!setPresetInternal(0))
            return false;
    }

    handlers.factoryResetDone();

    return true;
}

///
/// \brief Used to set new database layout (preset).
/// @param [in] preset  New preset to set.
/// \returns False if specified preset isn't supported, true otherwise.
///
bool Database::setPreset(uint8_t preset)
{
    if (preset >= supportedPresets)
        return false;

    activePreset = preset;

    bool returnValue;

    SYSTEM_BLOCK_ENTER(
        returnValue = update(0,
                             static_cast<uint8_t>(SectionPrivate::system_t::presets),
                             static_cast<size_t>(System::presetSetting_t::activePreset),
                             preset);)

    if (returnValue)
        handlers.presetChange(preset);

    return returnValue;
}

///
/// \brief Used to set new database layout (preset) without writing to database.
/// For internal use only.
/// @param [in] preset  New preset to set.
/// \returns False if specified preset isn't supported, true otherwise.
///
bool Database::setPresetInternal(uint8_t preset)
{
    if (preset >= supportedPresets)
        return false;

    activePreset = preset;
    setStartAddress(userDataStartAddress + (lastPresetAddress * activePreset));

    return true;
}

///
/// \brief Retrieves currently active preset.
///
uint8_t Database::getPreset()
{
    return activePreset;
}

///
/// \brief Retrieves number of presets possible to store in database.
/// Preset is simply another database layout copy.
///
uint8_t Database::getSupportedPresets()
{
    return supportedPresets;
}

///
/// \brief Enables or disables preservation of preset setting.
/// If preservation is enabled, configured preset will be loaded on board power on.
/// Otherwise, first preset will be loaded instead.
///
bool Database::setPresetPreserveState(bool state)
{
    bool returnValue;

    SYSTEM_BLOCK_ENTER(
        returnValue = update(0,
                             static_cast<uint8_t>(SectionPrivate::system_t::presets),
                             static_cast<size_t>(System::presetSetting_t::presetPreserve),
                             state);)

    return returnValue;
}

///
/// \brief Checks if preset preservation setting is enabled or disabled.
/// \returns True if preset preservation is enabled, false otherwise.
///
bool Database::getPresetPreserveState()
{
    bool returnValue;

    SYSTEM_BLOCK_ENTER(
        returnValue = read(0,
                           static_cast<uint8_t>(SectionPrivate::system_t::presets),
                           static_cast<size_t>(System::presetSetting_t::presetPreserve));)

    return returnValue;
}

///
/// \brief Checks if database has been already initialized by checking DB_BLOCK_ID.
/// \returns True if valid, false otherwise.
///
bool Database::isSignatureValid()
{
    uint16_t signature;

    SYSTEM_BLOCK_ENTER(
        signature = read(0,
                         static_cast<uint8_t>(SectionPrivate::system_t::uid),
                         0);)

    return getDbUID() == signature;
}

///
/// \brief Calculates unique database ID.
/// UID is calculated by appending number of parameters and their types for all
/// sections and all blocks.
///
uint16_t Database::getDbUID()
{
    ///
    /// \brief Magic value with which calculated signature is XORed.
    ///
    const uint16_t uidBase = 0x1701;

    uint16_t signature = 0;

    //get unique database signature based on its blocks/sections
    for (int i = 0; i < static_cast<uint8_t>(block_t::AMOUNT) + 1; i++)
    {
        for (int j = 0; j < dbLayout[i].numberOfSections; j++)
        {
            signature += static_cast<uint16_t>(dbLayout[i].section[j].numberOfParameters);
            signature += static_cast<uint16_t>(dbLayout[i].section[j].parameterType);
        }
    }

    signature += supportedPresets;

    return signature ^ uidBase;
}

///
/// \brief Updates unique database UID.
/// UID is written to first two database locations.
/// @param [in] uid Database UID to set.
///
bool Database::setDbUID(uint16_t uid)
{
    bool returnValue;

    SYSTEM_BLOCK_ENTER(
        returnValue = update(0, static_cast<uint8_t>(SectionPrivate::system_t::uid), 0, uid);)

    return returnValue;
}

__attribute__((weak)) void Database::customInitGlobal()
{
}

__attribute__((weak)) void Database::customInitButtons()
{
}

__attribute__((weak)) void Database::customInitEncoders()
{
}

__attribute__((weak)) void Database::customInitAnalog()
{
}

__attribute__((weak)) void Database::customInitLEDs()
{
}

__attribute__((weak)) void Database::customInitDisplay()
{
}

__attribute__((weak)) void Database::customInitTouchscreen()
{
}