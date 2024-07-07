/*

Pin connections:

- GP3: solenoid 1
- GP4: solenoid 2
- GP12: solenoid 3
- GP13: solenoid 4


*/
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <MIDI.h>

#include "AutoSleep.h"

// Solenoid pin assignments
const int solenoid1Pin = 3;
const int solenoid2Pin = 4;
const int solenoid3Pin = 12;
const int solenoid4Pin = 13;

// Set up sleep functionality
std::function<void()> enableSleepFunc = []() {
  Serial.println("Zzz going to sleep");
  // Set all solenoids to off, just to make sure nothing burns out.
  digitalWrite(solenoid1Pin, LOW);
  digitalWrite(solenoid2Pin, LOW);
  digitalWrite(solenoid3Pin, LOW);
  digitalWrite(solenoid4Pin, LOW);
};

std::function<void()> disableSleepFunc = []() {
  // Not doing anything
  Serial.println("WAKING from sleep!");
};

AutoSleep autoSleep1(2000, enableSleepFunc, disableSleepFunc);

// USB MIDI object
Adafruit_USBD_MIDI usbMidi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbMidi, MIDI);

// Handle MIDI note on
void handle_midi_note_on(byte channel, byte note, byte velocity) {
  Serial.println("Received note on: ");
  Serial.println(note);

  autoSleep1.disableSleep();
  autoSleep1.updateEventTime();

  if (note == 60) {
    analogWrite(solenoid1Pin, velocity * 2); // map 0-127 to 0-255
  } else if (note == 61) {
    analogWrite(solenoid2Pin, velocity * 2);
  } else if (note == 62) {
    analogWrite(solenoid3Pin, velocity * 2);
  } else if (note == 63) {
    analogWrite(solenoid4Pin, velocity * 2);
  }
}

// Handle MIDI note off
void handle_midi_note_off(byte channel, byte note, byte velocity) {

  autoSleep1.disableSleep();
  autoSleep1.updateEventTime();

  if (note == 60) {
    analogWrite(solenoid1Pin, 0);
  } else if (note == 61) {
    analogWrite(solenoid2Pin, 0);
  } else if (note == 62) {
    analogWrite(solenoid3Pin, 0);
  } else if (note == 63) {
    analogWrite(solenoid4Pin, 0);
  }
}
//
// Initialize solenoid pins
void setup() {
  pinMode(solenoid1Pin, OUTPUT);
  pinMode(solenoid2Pin, OUTPUT);
  pinMode(solenoid3Pin, OUTPUT);
  pinMode(solenoid4Pin, OUTPUT);

  autoSleep1.disableSleep();
  autoSleep1.updateEventTime();

  usbMidi.begin();
  MIDI.begin();

  // Turn midi thru off
  MIDI.turnThruOff();

  // Set the MIDI note on handling function
  MIDI.setHandleNoteOn(handle_midi_note_on);
  MIDI.setHandleNoteOff(handle_midi_note_off);
}

void loop() {
  MIDI.read();
  autoSleep1.checkSleep();
}
