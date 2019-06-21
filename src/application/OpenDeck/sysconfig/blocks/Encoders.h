/*

Copyright 2015-2019 Igor Petrovic

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

#pragma once

#include "database/blocks/Encoders.h"

typedef enum
{
    sysExSection_encoders_enable,
    sysExSection_encoders_invert,
    sysExSection_encoders_mode,
    sysExSection_encoders_midiID_lsb,
    sysExSection_encoders_midiChannel,
    sysExSection_encoders_pulsesPerStep,
    sysExSection_encoders_acceleration,
    sysExSection_encoders_midiID_msb,
    sysExSection_encoders_remoteSync,
    SYSEX_SECTIONS_ENCODERS
} sysExSection_encoders_t;

//map sysex sections to sections in db
const uint8_t sysEx2DB_encoders[SYSEX_SECTIONS_ENCODERS] = {
    dbSection_encoders_enable,
    dbSection_encoders_invert,
    dbSection_encoders_mode,
    dbSection_encoders_midiID,
    dbSection_encoders_midiChannel,
    dbSection_encoders_pulsesPerStep,
    dbSection_encoders_acceleration,
    dbSection_encoders_midiID,
    dbSection_encoders_remoteSync
};
