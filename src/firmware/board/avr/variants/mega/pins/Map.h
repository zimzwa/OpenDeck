/*
    OpenDeck MIDI platform firmware
    Copyright (C) 2015-2017 Igor Petrovic

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../Hardware.h"
#include "Pins.h"

const uint8_t aInPinMap[MAX_NUMBER_OF_ANALOG] =
{
    AI_1_PIN,
    AI_2_PIN,
    AI_3_PIN,
    AI_4_PIN,
    AI_5_PIN,
    AI_6_PIN
};