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

#include "board/Internal.h"
#include "board/common/io/Helpers.h"
#include "Pins.h"
#include "core/src/general/IO.h"
#include "core/src/general/ADC.h"

namespace Board
{
    namespace detail
    {
        namespace setup
        {
            void io()
            {
                CORE_IO_CONFIG(DI_1_PORT, DI_1_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_2_PORT, DI_2_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_3_PORT, DI_3_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_4_PORT, DI_4_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_5_PORT, DI_5_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_6_PORT, DI_6_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_7_PORT, DI_7_PIN, core::io::pinMode_t::input);
                CORE_IO_CONFIG(DI_8_PORT, DI_8_PIN, core::io::pinMode_t::input);

                CORE_IO_CONFIG(AI_1_PORT, AI_1_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_1_PORT, AI_1_PIN);

                CORE_IO_CONFIG(AI_2_PORT, AI_2_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_2_PORT, AI_2_PIN);

                CORE_IO_CONFIG(AI_3_PORT, AI_3_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_3_PORT, AI_3_PIN);

                CORE_IO_CONFIG(AI_4_PORT, AI_4_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_4_PORT, AI_4_PIN);

                CORE_IO_CONFIG(AI_5_PORT, AI_5_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_5_PORT, AI_5_PIN);

                CORE_IO_CONFIG(AI_6_PORT, AI_6_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_6_PORT, AI_6_PIN);

                CORE_IO_CONFIG(AI_7_PORT, AI_7_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_7_PORT, AI_7_PIN);

                CORE_IO_CONFIG(AI_8_PORT, AI_8_PIN, core::io::pinMode_t::input);
                CORE_IO_SET_LOW(AI_8_PORT, AI_8_PIN);

                CORE_IO_CONFIG(BTLDR_BUTTON_PORT, BTLDR_BUTTON_PIN, core::io::pinMode_t::input);
            }

            void adc()
            {
                core::adc::conf_t adcConfiguration;

                adcConfiguration.prescaler = core::adc::prescaler_t::p128;
                adcConfiguration.vref      = core::adc::vRef_t::aref;

                core::adc::setup(adcConfiguration);
                core::adc::setChannel(AI_1_PIN);

                for (int i = 0; i < 3; i++)
                    core::adc::read();    //few dummy reads to init ADC

                core::adc::enableInterrupt();
                core::adc::startConversion();
            }

            void timers()
            {
                //clear timer0 conf
                TCCR0A = 0;
                TCCR0B = 0;
                TIMSK0 = 0;

                //clear timer1 conf
                TCCR1A = 0;
                TCCR1B = 0;

                //clear timer3 conf
                TCCR3A = 0;
                TCCR3B = 0;

                //clear timer4 conf
                TCCR4A = 0;
                TCCR4B = 0;
                TCCR4C = 0;
                TCCR4D = 0;
                TCCR4E = 0;

                //set timer0 to ctc, used for millis/led matrix
                TCCR0A |= (1 << WGM01);                 //CTC mode
                TCCR0B |= (1 << CS01) | (1 << CS00);    //prescaler 64
                OCR0A = 124;                            //500us
                TIMSK0 |= (1 << OCIE0A);                //compare match interrupt
            }
        }    // namespace setup
    }        // namespace detail
}    // namespace Board