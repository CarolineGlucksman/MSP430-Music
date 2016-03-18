asm(" .length 10000");
asm(" .width 132");
// TimerTone0 == produces a 1 Khz tone using TimerA, Channel 0
// Toggled on and off with a button
// Using the timer in up mode with NO INTERRUPT
//
// Sound is turned on and off by directly manipulating
// the TACCTL0 register.  The half period is not dynamically
// updated (though it can be changed in the debugger by
// changing TACCR0.


#include "msp430g2553.h"
//-----------------------
// The following definitions allow us to adjust some of the port properties
// for the example:

// define the bit mask (within P1) corresponding to output TA0
#define TA0_BIT 0x02

// define the location for the buttons
// specific bit for the button
#define CHANGE 	0x04
#define PLAY	0x08
#define	SLOW	BIT4
#define	FAST	BIT5
#define RESET	BIT7

//define LED
#define RED 0x01
#define GREEN 0x40
//----------------------------------

//define notes
#define B3 	   2024
#define C4     1911
#define C4s    1803
#define D4     1703
#define E4     1517
#define F4     1432
#define G4     1276
#define A4     1136
#define B4     1012
#define C5     956
#define C5s    902
#define D5     851
#define E5     758
#define F5     716
#define G5     638
#define A5     568
#define B5     506
#define C6     478
#define R		1

//define durations
#define PAUSE .005
#define QUARTER 1
#define DOT_EIGTH .75
#define SIXTEENTH .25
#define DOT_QUARTER 1.5
#define EIGTH .5
#define HALF 2



//----------------------------------
#define JOY_LENGTH 72

//define Joy to the World
unsigned const int joy_notes[JOY_LENGTH] = {
		C5, B4, A4, G4, F4, E4, D4, C4, G4, A4, R, A4, B4, R, B4, C5, R, C5, R, C5, B4,
		A4, G4, R, G4, F4, E4, C5, R, C5, B4, A4, G4, R, G4, F4, E4, R, E4, R, E4, R, E4,
		R, E4, R, E4, F4, G4, F4, E4, D4, R, D4, R, D4, R, D4, E4, F4, E4, D4, E4, C5, A4,
		G4, F4, E4, F4, E4, D4, C4
};

const double joy_delay[JOY_LENGTH] = {
		QUARTER, DOT_EIGTH, SIXTEENTH, DOT_QUARTER, EIGTH, QUARTER, QUARTER, DOT_QUARTER,
		EIGTH, DOT_QUARTER, PAUSE, EIGTH, DOT_QUARTER, PAUSE, EIGTH, DOT_QUARTER, PAUSE,
		EIGTH, PAUSE, EIGTH, EIGTH, EIGTH, EIGTH, PAUSE, DOT_EIGTH, SIXTEENTH, EIGTH, EIGTH,
		PAUSE, EIGTH, EIGTH, EIGTH, EIGTH, PAUSE, DOT_EIGTH, SIXTEENTH, EIGTH, PAUSE, EIGTH,
		PAUSE, EIGTH, PAUSE, EIGTH, PAUSE, EIGTH, PAUSE, SIXTEENTH, SIXTEENTH, DOT_QUARTER,
		SIXTEENTH, SIXTEENTH, EIGTH, PAUSE, EIGTH, PAUSE, EIGTH, PAUSE, SIXTEENTH, SIXTEENTH,
		DOT_QUARTER, SIXTEENTH, SIXTEENTH, EIGTH, QUARTER, EIGTH, DOT_EIGTH, SIXTEENTH,
		EIGTH, EIGTH, QUARTER, QUARTER, HALF
};

//----------------------------------

#define TWO_LENGTH 246

//define Song Two - HOTLINE BLING by DRAKE
unsigned const int two_notes[TWO_LENGTH] = {
		A4, A4, A4, D5, C5, B4, A4, //7
		C5, A4, R,
		R, D5, C5, B4, A4,
		C5, A4,
		F4, R, D5, C5, B4, A4,
		C5, A4, R,
		R, D5, C5, B4, A4,
		C5, A4,
		E4, R, E4, A4, A4, F4, F4,
		A4, A4, B4, R,
		R, A4, C5, D5, A4,
		B4, B4, C5, R,
		R, A4, A4, F4, F4,
		A4, A4, B4, R,
		R, A4, C5, B4, A4,
		B4, B4, C5, R,
		A4, F4, A4, F4, A4, F4, A4, F4,
		D5, R, R,
		F4, F4, F4, F4, G4, F4, E4, D4,
		F4, B3, R,
		F4, F4, F4, F4, G4, F4, E4, D4,
		F4, B3, R,
		F4, F4, F4, F4, G4, F4, E4, D4,
		F4, B3, R, R, F4,
		A4, F4, A4, F4, A4, F4, A4, F4,
		B4, C5s, A4, R,
		F4, F4, F4, F4, G4, F4, E4, D4,
		F4, B3, R,
		F4, F4, F4, F4, G4, F4, E4, D4,
		F4, B3, R,
		F4, F4, F4, F4, G4, F4, E4, D4,
		F4, B3, R,
		A4, A4, A4, D5, C5, B4, A4, //7
		C5, A4, R,
		R, D5, C5, B4, A4,
		C5, A4,
		F4, R, D5, C5, B4, A4,
		C5, A4, R,
		R, D5, C5, B4, A4,
		C5, A4,
		E4, R, E4, A4, A4, F4, F4,
		A4, A4, B4, R,
		R, A4, C5, D5, A4,
		B4, B4, C5, R,
		R, A4, A4, F4, F4,
		A4, A4, B4, R,
		R, A4, C5, B4, A4,
		B4, B4, C5, R,
		A4, F4, A4, F4, A4, F4, A4, F4,
		D5, R, R
};

const double two_delay[TWO_LENGTH] = {
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, //7
		QUARTER, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		HALF, HALF,
		QUARTER, QUARTER, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		HALF, HALF,
		QUARTER, QUARTER, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, QUARTER, EIGTH, EIGTH,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		DOT_EIGTH, DOT_EIGTH, EIGTH, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, //7
		QUARTER, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		HALF, HALF,
		QUARTER, QUARTER, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		HALF, HALF,
		QUARTER, QUARTER, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		HALF, EIGTH, EIGTH, EIGTH, EIGTH,
		EIGTH, EIGTH, QUARTER, HALF,
		EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH, EIGTH,
		QUARTER, QUARTER, HALF

};

//----------------------------------

//use counter to keep track of note duration
unsigned int counter_joy = 0;
unsigned int counter_two = 0;
//change tempo by increasing or decreasing the unit
unsigned int unit = 53;
// keep track of what note we are one
unsigned int joy_curr = 0;
unsigned int two_curr = 0;
// keep track of what song we are playing, default to JOY
char* song_curr = "JOY";
// keep track of whether we are paused
unsigned int is_play = 1;
//use this to debounce
unsigned int button_count;

void init_timer(void); // routine to setup the timer
void init_button(void); // routine to setup the button
void init_WDT(void);	// routine to set up WDT


// ++++++++++++++++++++++++++

void main(){

	BCSCTL1 = CALBC1_1MHZ;    // 1Mhz calibration for clock
	DCOCTL  = CALDCO_1MHZ;
	init_WDT();    // initialize WDT
	init_timer();  // initialize timer
	init_button(); // initialize the button
	_bis_SR_register(GIE+LPM0_bits);// enable general interrupts and power down CPU
}

// +++++++++++++++++++++++++++

void init_WDT(void){
	//WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
		// setup the watchdog timer as an interval timer
		WDTCTL =(WDTPW + // (bits 15-8) password
			                   // bit 7=0 => watchdog timer on
			                   // bit 6=0 => NMI on rising edge (not used here)
			                   // bit 5=0 => RST/NMI pin does a reset (not used here)
				WDTTMSEL + // (bit 4) select interval timer mode
			    WDTCNTCL +  // (bit 3) clear watchdog timer counter
			  		          0 // bit 2=0 => SMCLK is the source
			  		          +1 // bits 1-0 = 01 => source/8K
			  		   );
		IE1 |= WDTIE;		// enable the WDT interrupt (in the system interrupt register IE1)
}

// +++++++++++++++++++++++++++
// Sound Production System
void init_timer(){              // initialization and start of timer
	TA0CTL |= TACLR;            // reset clock
	TA0CTL = TASSEL_2+ID_0+MC_1;// clock source = SMCLK
	                            // clock divider=1
	                            // UP mode
	                            // timer A interrupt off
	TA0CCTL0 ^= OUTMOD_4; // compare mode, output mode 0, no interrupt enabled
	P1SEL|=TA0_BIT; // connect timer output to pin
	P1DIR|=TA0_BIT;
}

// +++++++++++++++++++++++++++
// Button input System
// Button toggles the state of the sound (on or off)
// action will be interrupt driven on a downgoing signal on the pin

void init_button(){
// All GPIO's are already inputs if we are coming in after a reset

	P1DIR |= RED+GREEN; //initialize LED
	P1OUT |= RED;		// turn Red on for JOY
	P1OUT &= ~GREEN;	//turn off green

	//change button
	P1OUT |= CHANGE; // pullup
	P1REN |= CHANGE; // enable resistor
	P1IES |= CHANGE; // set for 1->0 transition
	P1IFG &= ~CHANGE;// clear interrupt flag
	P1IE  |= CHANGE; // enable interrupt

	//RESET button
	P1OUT |= RESET; // pullup
	P1REN |= RESET; // enable resistor
	P1IES |= RESET; // set for 1->0 transition
	P1IFG &= ~RESET;// clear interrupt flag
	P1IE  |= RESET; // enable interrupt

	//PLAY button
	P1OUT |= PLAY; // pullup
	P1REN |= PLAY; // enable resistor
	P1IES |= PLAY; // set for 1->0 transition
	P1IFG &= ~PLAY;// clear interrupt flag
	P1IE  |= PLAY; // enable interrupt

	//SLOW button
	P1OUT |= SLOW; // pullup
	P1REN |= SLOW; // enable resistor
	P1IES |= SLOW; // set for 1->0 transition
	P1IFG &= ~SLOW;// clear interrupt flag
	P1IE  |= SLOW; // enable interrupt

	//FAST button
	P1OUT |= FAST; // pullup
	P1REN |= FAST; // enable resistor
	P1IES |= FAST; // set for 1->0 transition
	P1IFG &= ~FAST;// clear interrupt flag
	P1IE  |= FAST; // enable interrupt
}

void interrupt button_handler(){
// check interrupt comes from the desired bit...
// (if not, just ignore -- cannot happen in this case)
	if (button_count >= 20){
		// play or pause
		if (P1IFG & PLAY){
				TACCTL0 ^= OUTMOD_4; // toggle outmod between 0 and 4 (toggle)
				is_play ^= 1; // toggle the play and pause
				P1IFG &= ~PLAY;
		}
		// reset the CURRENT SONG
		else if (P1IFG & RESET){
			unit = 53;
			if (song_curr == "JOY"){
				joy_curr = 0;
				counter_joy = 0;
			}
			else{
				two_curr = 0;
				counter_two = 0;
			}
			P1IFG &= ~RESET;
		}
		// decrease the tempo
		else if (P1IFG & SLOW){
			if (unit < 200){
				unit += 10;
			}
			P1IFG &= ~SLOW;
		}
		// increase the temp
		else if (P1IFG & FAST){
			if (unit > 10){
				unit -= 10;
			}
			P1IFG &= ~FAST;
		}
		// switch songs, but do not alter where we are in the current song (dont start over, just
		// pick up where we left off)
		else if (P1IFG & CHANGE){
			if (song_curr == "JOY"){
				song_curr = "TWO";
				P1OUT &= ~RED; //red led off
				P1OUT |= GREEN; //green led on
			}
			else{
				song_curr = "JOY";
				P1OUT &= ~GREEN; //green led off
				P1OUT |= RED; //red led on
			}
			P1IFG &= ~CHANGE;
		}
		//reset button
		button_count = 0;
	}
}

ISR_VECTOR(button_handler,".int02") // declare interrupt vector
// +++++++++++++++++++++++++++
interrupt void WDT_interval_handler(){
	// play the current song
	if (song_curr == "JOY"){
		// set the frequency for the speaker by choosing the current note from the note array
		TA0CCR0 = joy_notes[joy_curr];
		// if we have reached the end of the note duration, advance to the next note
		if (counter_joy >= (joy_delay[joy_curr] * unit) && is_play){
			counter_joy = 0;
			joy_curr++;
			TA0CCR0 = 1;
			// blink the LED slightly in time with the notes
			P1OUT ^= RED;
		}
		else if (joy_curr > JOY_LENGTH){	//replay current song if we are at the end
			joy_curr = 0;
		}
		// increase the counter
		else if (is_play){
			counter_joy++;
			P1OUT |= RED;
		}
	}
	else{ //we are playing song 2
		// set the frequency for the speaker by choosing the current note from the note array
		TA0CCR0 = two_notes[two_curr];
		// if we have reached the end of the note duration, advance to the next note
		if (counter_two >= (two_delay[two_curr] * unit) && is_play){
			counter_two = 0;
			two_curr++;
			TA0CCR0 = 1;
			// blink the LED slightly in time with the notes
			P1OUT ^= GREEN;
		}
		else if (two_curr > TWO_LENGTH){	//replay current song if we are at the end
			two_curr = 0;
		}
		else if (is_play){
			// increase the counter
			counter_two++;
			P1OUT |= GREEN;
		}
	}

	// to debounce buttons check for interrupt but wait to reset it
	// reset the count and the interrupt flag
	if (P1IFG & PLAY || P1IFG & SLOW || P1IFG & FAST || P1IFG & CHANGE || P1IFG & RESET){
		button_count++;
	}

}

// DECLARE function WDT_interval_handler as handler for interrupt 10

// using a macro defined in the msp430g2553.h include file

ISR_VECTOR(WDT_interval_handler, ".int10")

