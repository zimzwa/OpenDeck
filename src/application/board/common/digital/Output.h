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

namespace Board
{
    namespace interface
    {
        namespace digital
        {
            namespace output
            {
                namespace detail
                {
                    ///
                    /// \brief Checks if digital outputs need to be updated (state and PWM control).
                    ///
                    void checkDigitalOutputs();

                    ///
                    /// \brief Checks if indicator LEDs need to be turned on or off.
                    ///
                    void checkIndicators();
                }    // namespace detail
            }        // namespace output
        }            // namespace digital
    }                // namespace interface
}    // namespace Board