/*

OpenDeck platform firmware v2.0
Last revision date: 2015-05-08
Author: Igor Petrovic

*/

//disable default serial object
#define HardwareSerial_h

#include "OpenDeck.h"

void setup()    {

    //setup
    openDeck.init();

}

void loop() {

    //check incoming MIDI messages
    openDeck.checkMIDIIn();

    //check buttons
    openDeck.readButtons();

    //check analog inputs
    openDeck.readAnalog();

    //check encoders
    openDeck.readEncoders();

    #ifdef SERIAL_MOD
        uart.releaseTX();
    #endif

}