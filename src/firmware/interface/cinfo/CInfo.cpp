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

#include "CInfo.h"

uint32_t lastCinfoMsgTime[DB_BLOCKS];

uint32_t getLastCinfoMsgTime(uint8_t block)
{
    return lastCinfoMsgTime[block];
}

void updateCinfoTime(uint8_t block)
{
    lastCinfoMsgTime[block] = rTimeMs();
}