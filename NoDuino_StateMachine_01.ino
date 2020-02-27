#include<avr/io.h>

int counter1 = 0;
int Led1OnTime = 240;
int Led1OffTime = 900;
bool Led1OnOff = false;

int counter2 = 0;
int Led2OnTime = 220;
int Led2OffTime = 1200;
bool Led2OnOff = false;

int counter3 = 0;
int Led3OnTime = 520;
int Led3OffTime = 430;
bool Led3OnOff = false;

int main (void) {
  cli();                    // turn Interrupts off
  DDRD = 56;                // LEDs on PinD 3.4.5 as OUTPUT
  TCCR1A = 0;               // Timer1 | mode selection CTC
  OCR1A = 50;               // Timer1 | compare match register 16MHz/256/ ~2Hz
  TCCR1B = 12;              // Timer1 | 256 prescaler and CTC-Mode
  TIMSK1 = 2;               // Timer1 | enable timer compare interrupt
  sei();                    // turn Interrupts on

  while (1) {
    if ((Led1OnOff == true) && (counter1 >= Led1OnTime))
    {
      Led1OnOff = false;    // stete engine info
      counter1 = 0;         // reset counter1
      PORTD &= ~8;           // LED1 off
    }
    else if ((Led1OnOff == false) && (counter1 >= Led1OffTime))
    {
      Led1OnOff = true;     // state engine info
      counter1 = 0;         // reset counter1
      PORTD |= 8;           // LED1 on
    }

    if ((Led2OnOff == true) && (counter2 >= Led2OnTime))
    {
      Led2OnOff = false;
      counter2 = 0;
      PORTD &=~16;
    }
    else if ((Led2OnOff == false) && (counter2 >= Led2OffTime))
    {
      Led2OnOff = true;
      counter2 = 0;
      PORTD |= 16;
    }

    if ((Led3OnOff == true) && (counter3 >= Led2OnTime))
    {
      Led3OnOff = false;
      counter3 = 0;
      PORTD &=~32;  //
    }
    else if ((Led3OnOff == false) && (counter3 >= Led3OffTime))
    {
      Led3OnOff = true;
      counter3 = 0;
      PORTD |= 32;
    }
  }
}
  ISR(TIMER1_COMPA_vect)
  {
    counter1++;
    counter2++;
    counter3++;
  }
