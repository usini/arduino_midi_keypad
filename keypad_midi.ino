#include "MIDI.h"

int noteON = 144;   //144 = 10010000 in binary, note on command
int noteOFF = 128;  //128 = 10000000 in binary, note off command

/* Settings for MIDI preset */
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //four columns
const int NB_BUTTONS = ROWS * COLS;

int notes[NB_BUTTONS] = { 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74 };
int notes2[NB_BUTTONS] = { 76, 77, 79, 81, 83, 84, 86, 88, 89, 91, 93, 94, 96, 98, 100, 101 };

/* Keypad Init */
//https://github.com/Nullkraft/Keypad
#include <Keypad.h>
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'F' },
  { '4', '5', '6', 'A' },
  { '7', '8', '9', 'C' },
  { 'U', '0', 'D', 'V' }
};

//Keypad pins
byte rowPins[ROWS] = { 2, 3, 4, 5 };       //connect to the row pinouts of the kpd
byte colPins[COLS] = { 9, 8, 7, 6 };       //connect to the column pinouts of the kpd
byte rowPins2[ROWS] = { 10, 11, 12, 13 };  //connect to the row pinouts of the kpd
byte colPins2[COLS] = { A0, A1, A2, A3 };  //connect to the column pinouts of the kpd

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Keypad kpd2 = Keypad(makeKeymap(keys), rowPins2, colPins2, ROWS, COLS);

//Keypad state : 1 = DOWN 3 = UP (there are other state which are unused here)
unsigned int buttons_state[NB_BUTTONS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned int buttons_state2[NB_BUTTONS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(115200);
}

void loop() {

  if (kpd.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++)  // Scan the whole key list.
    {
      if (kpd.key[i].stateChanged)  // Only find keys that have changed state.
      {
        buttons_state[kpd.key[i].kcode] = kpd.key[i].kstate;

        if (kpd.key[i].kstate == 1) {  //DOWN
          key_down(kpd.key[i].kcode);
        }
        if (kpd.key[i].kstate == 3) {  //UP
          // If key is up we send a midi note off
          key_up(kpd.key[i].kcode);
        }
      }
    }
  }

  if (kpd2.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++)  // Scan the whole key list.
    {
      if (kpd2.key[i].stateChanged)  // Only find keys that have changed state.
      {
        buttons_state2[kpd2.key[i].kcode] = kpd2.key[i].kstate;

        if (kpd2.key[i].kstate == 1) {  //DOWN
          MIDImessage(noteON, notes2[kpd2.key[i].kcode], 100);
        }
        if (kpd2.key[i].kstate == 3) {  //UP
          // If key is up we send a midi note off
          MIDImessage(noteOFF, notes2[kpd2.key[i].kcode], 127);
        }
      }
    }
  }
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);       //send note on or note off command
  Serial.write(MIDInote);      //send pitch data
  Serial.write(MIDIvelocity);  //send velocity data
}

//Key down (pressed)
void key_down(int key) {
  int current_velocity = 100;
  //Serial.print(buttons_state[velocity_button]);
  MIDImessage(noteON, notes[key], current_velocity);
}

//Key up (unpressed)
void key_up(int key) {
  MIDImessage(noteOFF, notes[key], 127);
}