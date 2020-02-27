#include<avr/io.h>

const int strobePin = 8;  // STB ... brwn ... 8 (PB0)
const int clockPin = 9;   // CLK ... wht  ... 9 (PB1)
const int dataPin = 10;   // DIO ... grey .. 10 (PB2)
const uint8_t nrState = 8; // how many LEDs are part of the StateEngine

uint8_t numberLED[nrState];
uint16_t counter[nrState];
uint16_t LedOnTime[] = {111, 222, 333, 444, 555, 666, 777, 888};
uint16_t LedOffTime[] = {233, 344, 455, 566, 677, 788, 899, 654};
bool LedOnOff[nrState];

// Text:
uint8_t stateEng[] = {109, 120, 119, 120, 249, 121, 84, 111}; // "StAtE.Eng" on sevenSegment

void LedState()
{
  for (uint8_t i = 0; i < nrState; i++)
  {
    if ((LedOnOff[i] == true) && (counter[i] > LedOnTime[i]))
    {
      LedOnOff[i] = false;    // state engine info
      counter[i] = 0;         // reset counter of LED
      TM1638(i, LedOnOff[i]); // LED off
    }
    else if ((LedOnOff[i] == false) && (counter[i] > LedOffTime[i]))
    {
      LedOnOff[i] = true;     // state engine info
      counter[i] = 0;         // reset counter
      TM1638(i, LedOnOff[i]); // LED on
    }
  }
}

void shiftOL(uint8_t value)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(dataPin, (value & (1 << i)));
    digitalWrite(clockPin, 1);
    digitalWrite(clockPin, 0);
  }
}

uint8_t shiftIL(void)
{
  uint8_t  value = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(clockPin, HIGH);
    value |= digitalRead(dataPin) << i;
    digitalWrite(clockPin, LOW);
  }
  return value;
}


void sendCommand(uint8_t value)     // to do: integrate this Func. with ShiftOL (8/16 Bit)
{
  digitalWrite(strobePin, LOW);
  shiftOL(value);
  digitalWrite(strobePin, HIGH);
}

void TM1638 (uint8_t digitPos, uint8_t digit)           // User_Interface: digitPos 0 - 7 are the 8 LEDs
{ // digitPos 8 - 15 are the 8 7Segments
  if (digitPos > 7) digitPos = 176 + (digitPos << 1);   // Data is for a Segment
  else  digitPos = 193 + (digitPos << 1);               // data is for one of the LEDs
  digitalWrite(strobePin, LOW);
  shiftOL(digitPos);
  shiftOL(digit);
  digitalWrite(strobePin, HIGH);
}

uint8_t TM1638Button(void)
{
  uint8_t buttons = 0;
  digitalWrite(strobePin, LOW);
  shiftOL(0x42);
  pinMode(dataPin, INPUT);
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t value = ((shiftIL()) << i);
    buttons |= value;
  }
  pinMode(dataPin, OUTPUT);
  digitalWrite(strobePin, HIGH);
  return buttons;
}

int main (void) {
  cli();                    // turn Interrupts off
  TCCR1A = 0;               // Timer1 | mode selection CTC
  OCR1A = 50;               // Timer1 | compare match register 16MHz/256/ ~1000Hz
  TCCR1B = 12;              // Timer1 | 256 prescaler and CTC-Mode
  TIMSK1 = 2;               // Timer1 | enable timer compare interrupt
  sei();                    // turn Interrupts on
  DDRB = 7;                 // PB 0 - 2 = OUTPUTs
  sendCommand(138);         // activate and set brightness (Max: 0x8f = 143)
  sendCommand(0x44);        // set single address
  for (uint8_t i = 0; i < 8; i++)
  {
    TM1638(i, 0);           // blank all LEDs
    TM1638((i + 8), stateEng[i]); // write Text on 7Segs
  }

  while (1) {
    LedState();
    uint8_t buttons = TM1638Button();
    if (buttons == 2)
    {
      cli();
    }
    if (buttons == 4)
    {
      sei();
    }
  }
}


ISR(TIMER1_COMPA_vect)
{
  for (uint8_t i = 0; i < nrState; i++)
  {
    counter[i]++;
  }
}
