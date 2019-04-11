/*
  Gingerbread Man

  by Chris Ritchie - 30/03/2019


  ATTiny @ 1 MHz
*/

#include <rtttl.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Define pins for speaker, LEDs and button
const byte rightLED = 1;
const byte leftLED = 0;
const byte speakerPin = 4;
const byte playBttn = 2;

// State of button
boolean playState = 0;

int playSong = 0;
const int octave = 0;

const char song_0[] PROGMEM =
"It'sBegi:d=16,o=5,b=160:8c,32p,c#.,d#.,f,d#.,d.,p,d#.,4f,4g#,4c6,2d#.,4c.6,32p,c.6,4a#,4g#,2f,4p,f.,p,g.,g#.,a#,g#.,g.,p,g#.,8g.,p,f.,p,e.,8d#,32p,f.,g#.,p,c.6,4d#6,8e,32p,f.,2c#6,2g,2g#";
const char song_1[] PROGMEM =
"JoyToThe:d=4,o=5,b=112:d6,8c#.6,16b,a.,8g,f#,e,d,8p,8a,b,8p,8b,c#6,8p,8c#6,2d.6,8p,8d6,8d6,8c#6,8b,8a,8a.,16g,8f#,8d6,8d6,8c#6,8b,8a,8a.,16g,8f#,8f#,8f#,8f#,8f#,16f#,16g,a.,16g,16f#,8e,8e,8e,16e,16f#,g,8p,16f#,16e,8d,8d6,8p,8b,8a.,16g,8f#,8g,f#,e,2d";
const char song_2[] PROGMEM =
"Auld L S:d=4,o=5,b=100:g,c.6,8c6,c6,e6,d.6,8c6,d6,8e6,8d6,c.6,8c6,e6,g6,2a.6,a6,g.6,8e6,e6,c6,d.6,8c6,d6,8e6,8d6,c.6,8a,a,g,2c.6";
const char song_3[] PROGMEM =
"Greensle:d=4,o=5,b=140:32p,g,2a#,c6,d.6,8d#6,d6,2c6,a,f.,8g,a,2a#,g,g.,8f,g,2a,f,2d,g,2a#,c6,d.6,8e6,d6,2c6,a,f.,8g,a,a#.,8a,g,f#.,8e,f#,2g";
const char song_4[] PROGMEM =
"OhChristtree:d=4,o=5,b=100:c,8f.,16f,f.,8g,8a.,16a,a.,8a,8g,8a,a#,e,g,f,8p,8c6,8c6,8a,d.6,8c6,8c6,8a#,a#.,8a#,8a#,8g,c.6,8a#,8a#,8a,a,8p,8c,8f.,8f,f,8g,8a.,16a,a.,8a,8g,8a,a#,e,g,f";
const char song_5[] PROGMEM =
"SoThisIs:d=8,o=5,b=80:32p,a,b,c#6,a,e,4p,e,a,b,c#6,4b,4p,16f#,b,c#6,d6,c#6,4b,16e,16e,c#6,e6,16c#6,16b,4a";
const char song_6[] PROGMEM =
"We Wish you a Merry Christmas:d=8,o=5,b=140:4d,4g,g,a,g,f#,4e,4c,4e,4a,a,b,a,g,4f#,4d,4f#,4b,b,c6,b,a,4g,4e,4d,4e,4a,4f#,2g";
const char song_7[] PROGMEM =
"LittleDr:d=8,o=5,b=140:2c,4p,d,p,e,10p,e,p,e,p,e,p,f,e,16f,5p,2e,4p,d,p,e,p,f,p,g,p,g,p,g,p,a,p,g,f,16e,5p,4d,5p,e,32p,d,e,10p,4c";
const char song_8[] PROGMEM =
"FrostyTh:d=4,o=5,b=140:2g,e.,8f,g,2c6,8b,8c6,d6,c6,b,a,2g,p,8b,8c6,d6,c6,b,8a,8a,g,c6,e,8f,8a,g,f,e,f,2g";
const char song_9[] PROGMEM =
"TheFirst:d=4,o=6,b=100:8e5,8d5,c.5,8d5,8e5,8f5,2g5,8a5,8b5,c,b5,a5,2g5,8a5,8b5,c,b5,a5,g5,a5,b5,c,g5,f5,2e5,8e5,8d5,c.5,8d5,8e5,8f5,2g5,8c,8b5,2a5,a5,2g.5,c,b5,a5,g5,a5,b5,c,g5,f5,2e5";
const char song_10[] PROGMEM =
"O Come:d=4,o=5,b=160:g,2g,d,g,2a,2d,b,a,b,c6,2b,a,g,2g,f#,e,f#,g,a,b,2f#,e.,8d,2d.";
const char song_11[] PROGMEM =
"DeckTheH:d=4,o=5,b=200:c.,8a#,a,g,f,g,a,f,8g,8a,8a#,8g,a.,8g,f,e,2f,c.,8a#,a,g,f,g,a,f,8g,8a,8a#,8g,a.,8g,f,e,2f,g.,8a,a#,g,a.,8a#,c6,g,8a,8b,c6,8d6,8e6,f6,e6,d6,2c6,c.,8a#,a,g,f,g,a,f,8g,8a,8a#,8g,a.,8g,f,e,2f";
const char song_12[] PROGMEM =
"Shepherd:d=4,o=6,b=25:32g5,32g5,16b.5,32b5,16a5,16g5,16c6,16c6,16b5,16a5,16b5,16d6,16d6,16c#6,8d.6,16b5,16e.6,32d6,16c6,16b5,16a5,16g5,16f#5,16b5,16a5,16g5,16g5,16f#5,4g5,";
const char song_13[] PROGMEM =
"silent:d=4,o=5,b=160:g.,8a,g,2e.,g.,8a,g,2e.,2d6,d6,2b.,2c6,c6,2g.";
const char song_14[] PROGMEM =
"Winter:d=4,o=5,b=125:8c.6,16c6,2c.6,8c.6,16c6,a,2c6,8c.6,16c6,2c.6,8c.6,16c6,a#,2c6,8p,8c6,8e6,8e6,8e6,2d6,8d6,8c6,8c6,8c6,2a#,16p,8a.,16a,8a.,16a,8g.,16g,8g.,16g,2f.";
const char song_15[] PROGMEM =
"JingleBellRock:d=8,o=6,b=125:g5,e,d,c,2g5,g5,e,d,c,2a5,a5,f,e,d,b5,g5,b5,d,g.,16g,f,d,2e,g5,e,d,c,2g5,16f#5,g5,e,d,c,2a5,a5,f,e,d,g,16g,16f#,16g,16f#,16g,16g#,a.,16g,e,d,4c,4g,e,e,e.,16d#,e,e,e.,16d#,e,g,c.,16d,2e,f,f,f.,16f,f,e,e,16e,16e,e,d,d,e,2d";
const char song_16[] PROGMEM =
"WhiteChr:d=8,o=5,b=70:4f#,g,f#,f,f#,4g,g#,a,p,b,c#6,d6,e6,d6,c#6,b,a,p,d,e,4f#,4f#,b,4a,a,4d6,p,d,e,4f#,4f#,b,b,c#,16c#.,4d";
const char song_17[] PROGMEM =
"LastChri:d=4,o=5,b=112:16d6,e6,e6,8d6,a,8e6,8e6,8f#6,d.6,8b,8b,8e6,8e6,f#6,d.6,8b,8c#6,8d6,8c#6,2b,16e6,f#6,e.6,8b,8f#6,8g6,8f#6,2e6,8d6,8c#6,8d6,8c#6,c#6,8d6,8c#6,2a,16d6,e6,e6,8d6,8a,8e6,8e6,8f#6,d.6,8b,8b,8e6,8e6,f#6,d.6,8b,8c#6,8d6,8c#6,b.,16e6,f#6,e.6,8b,8f#6,8g6,8f#6,2e6,8d6,8c#6,8d6,8c#6,c#6,8d6,8c#6,a";

Rtttl player;

void setup(void)
{
    analogReference(INTERNAL);
    pinMode(playBttn, INPUT_PULLUP);
    pinMode(rightLED, OUTPUT);
    pinMode(leftLED, OUTPUT);
    digitalWrite(rightLED, 150);
    digitalWrite(leftLED, 150);

    player.begin(speakerPin);
}

void loop(void)
{
	sleep(); // Stay in low power mode until button is pressed
    playState = digitalRead(playBttn); // Check state of button

    // Play song if button is pressed
    if (playState == LOW)
    {
        // Reset playSong variable if all songs have been played
        if (playSong > 17) playSong = 0;

        switch (playSong)
        {
        case 0:
            player.play_P(song_0, octave);
            break;
        case 1:
            player.play_P(song_1, octave);
            break;
        case 2:
            player.play_P(song_2, octave);
            break;
        case 3:
            player.play_P(song_3, octave);
            break;
        case 4:
            player.play_P(song_4, octave);
            break;
        case 5:
            player.play_P(song_5, octave);
            break;
        case 6:
            player.play_P(song_6, octave);
            break;
        case 7:
            player.play_P(song_7, octave);
            break;
        case 8:
            player.play_P(song_8, octave);
            break;
        case 9:
            player.play_P(song_9, octave);
            break;
        case 10:
            player.play_P(song_10, octave);
            break;
        case 11:
            player.play_P(song_11, octave);
            break;
        case 12:
            player.play_P(song_12, octave);
            break;
        case 13:
            player.play_P(song_13, octave);
            break;
        case 14:
            player.play_P(song_14, octave);
            break;
        case 15:
			player.play_P(song_15, octave);
			break;
		case 16:
			player.play_P(song_16, octave);
			break;
		case 17:
			player.play_P(song_17, octave);
			break;
        }
		digitalWrite(rightLED, 150);
		digitalWrite(leftLED, 150);
        playSong++;
    }

}

void sleep() {

  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);                   // Use PB2 as interrupt pin
  PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
  PCMSK |= _BV(PCINT4);                   // Use PB4 as interrupt pin
  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep

  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT2);                  // Turn off PB2 as interrupt pin
  PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
  PCMSK &= ~_BV(PCINT4);                  // Turn off PB4 as interrupt pin
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on

  sei();                                  // Enable interrupts
} // sleep

ISR(PCINT0_vect) {
  // This is called when the interrupt occurs, but I don't need to do anything in it
}

