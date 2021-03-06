#include "database/Database.h"

void Database::customInitButtons()
{
    //each new category of buttons should have their IDs start from 0
    for (int i = 0; i < MAX_NUMBER_OF_BUTTONS; i++)
        update(Database::Section::button_t::midiID, i, i);

    for (int i = 0; i < MAX_NUMBER_OF_ANALOG; i++)
        update(Database::Section::button_t::midiID, i + MAX_NUMBER_OF_BUTTONS, i);

    for (int i = 0; i < MAX_NUMBER_OF_TOUCHSCREEN_BUTTONS; i++)
        update(Database::Section::button_t::midiID, i + MAX_NUMBER_OF_BUTTONS + MAX_NUMBER_OF_ANALOG, i);
}

void Database::customInitLEDs()
{
    //same logic as for the buttons
    for (int i = 0; i < MAX_NUMBER_OF_LEDS; i++)
        update(Database::Section::leds_t::activationID, i, i);

    for (int i = 0; i < MAX_NUMBER_OF_TOUCHSCREEN_BUTTONS; i++)
        update(Database::Section::leds_t::activationID, i + MAX_NUMBER_OF_LEDS, i);
}