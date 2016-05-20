/*********************************************************************
 *               analog example for Versa1.0
 *	Analog capture on connectors K1, K2, K3 and K5. 
 *********************************************************************/

#define BOARD 8X2A

#include <fruit.h>
//#include <analog.h>
#include <dimmer.h>
#include <dcmotor.h>

t_delay mainDelay;
DCMOTOR_DECLARE(C);

void setup(void) {	
//----------- Setup ----------------
	fruitInit();
			
	//pinModeDigitalOut(LED); 	// set the LED pin mode to digital out
	//digitalClear(LED);		// clear the LED
	delayStart(mainDelay, 5000); 	// init the mainDelay to 5 ms

//----------- Analog setup ----------------
	//analogInit();		// init analog module
	/*analogSelect(0,K2);	// assign connector K1 to analog channel 0
	analogSelect(1,K3);
	analogSelect(2,K4);
	analogSelect(3,K5);*/
	
	dimmerInit();        // init dimmer module

	digitalClear(MD1);
	digitalClear(MD2);
	digitalSet(MDEN);
	pinModeDigitalOut(MD1);
	pinModeDigitalOut(MD2);
	pinModeDigitalOut(MDEN);

	digitalClear(DANSEUSE);
	pinModeDigitalOut(DANSEUSE);

    dcmotorInit(C);
}


void loop() {
// ---------- Main loop ------------
	fraiseService();	// listen to Fraise events
	//analogService();	// analog management routine
	dimmerService();	// dimmer management routine

	if(delayFinished(mainDelay)) // when mainDelay triggers :
	{
		delayStart(mainDelay, 5000); 	// re-init mainDelay
		//analogSend();		// send analog channels that changed
		DCMOTOR_COMPUTE(C,SYM);
	}
}

// Receiving

void fraiseReceiveChar() // receive text
{
	unsigned char c;
	
	c=fraiseGetChar();
	/*if(c=='L'){		//switch LED on/off 
		c=fraiseGetChar();
		digitalWrite(LED, c!='0');		
	}
	else*/ if(c=='E') { 	// echo text (send it back to host)
		printf("C");
		c = fraiseGetLen(); 			// get length of current packet
		while(c--) printf("%c",fraiseGetChar());// send each received byte
		putchar('\n');				// end of line
	}	
}

void bitoutReceive()
{
	unsigned char n,v;
	n=fraiseGetChar();
	v=fraiseGetChar();
	
	switch(n) {
		case 0 : 
			if(v==0) digitalClear(MD1);
			else digitalSet(MD1);
			break;
		case 1 : 
			if(v==0) digitalClear(MD2);
			else digitalSet(MD2);
			break;
		case 2 : 
			if(v==0) digitalClear(DANSEUSE);
			else digitalSet(DANSEUSE);
			break;
	}	
}

void fraiseReceive() // receive raw
{
	unsigned char c;
	c=fraiseGetChar();
	
	switch(c) {
		case 10 : bitoutReceive() ; break;
		case 40 : dimmerReceive() ; break;
	    case 120 : DCMOTOR_INPUT(C) ; break;
	    //case 121 : DCMOTOR_INPUT(D) ; break;
	}
}

// ---------- Interrupts ------------
void highInterrupts()
{
	dimmerHighInterrupt();
}


