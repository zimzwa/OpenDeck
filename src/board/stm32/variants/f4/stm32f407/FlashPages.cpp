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

#include "board/Board.h"
#include "board/Internal.h"

#define TOTAL_FLASH_PAGES 12

namespace
{
    Board::detail::map::flashPage_t pageDescriptor[TOTAL_FLASH_PAGES] = {
        {
            .address = 0x8000000,
            .size    = 16384,
        },

        {
            .address = 0x8004000,
            .size    = 16384,
        },

        {
            .address = 0x8008000,
            .size    = 16384,
        },

        {
            .address = 0x800C000,
            .size    = 16384,
        },

        {
            .address = 0x8010000,
            .size    = 65536,
        },

        {
            .address = 0x8020000,
            .size    = 131072,
        },

        {
            .address = 0x8040000,
            .size    = 131072,
        },

        {
            .address = 0x8060000,
            .size    = 131072,
        },

        {
            .address = 0x8080000,
            .size    = 131072,
        },

        {
            .address = 0x80A0000,
            .size    = 131072,
        },

        {
            .address = 0x80C0000,
            .size    = 131072,
        },

        {
            .address = 0x80E0000,
            .size    = 131072,
        },
    };
}

namespace Board
{
    namespace detail
    {
        namespace map
        {
            Board::detail::map::flashPage_t& flashPageDescriptor(size_t pageIndex)
            {
                if (pageIndex >= TOTAL_FLASH_PAGES)
                    return pageDescriptor[TOTAL_FLASH_PAGES - 1];

                return pageDescriptor[pageIndex];
            }

            size_t eepromFlashPageFactory()
            {
                return 5;
            }

            size_t eepromFlashPage1()
            {
                return 6;
            }

            size_t eepromFlashPage2()
            {
                return 7;
            }
        }    // namespace map
    }        // namespace detail
}    // namespace Board