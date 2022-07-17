#include "bbb_regs.h"
#include "hw_types.h"

#define DELAY_USE_INTERRUPT			1

/**
 * \brief   This macro will check for write POSTED status
 *
 * \param   reg          Register whose status has to be checked
 *
 *    'reg' can take the following values \n
 *    DMTIMER_WRITE_POST_TCLR - Timer Control register \n
 *    DMTIMER_WRITE_POST_TCRR - Timer Counter register \n
 *    DMTIMER_WRITE_POST_TLDR - Timer Load register \n
 *    DMTIMER_WRITE_POST_TTGR - Timer Trigger register \n
 *    DMTIMER_WRITE_POST_TMAR - Timer Match register \n
 *
 **/
#define DMTimerWaitForWrite(reg)   \
            if(HWREG(DMTIMER_TSICR) & 0x4)\
            while((reg & HWREG(DMTIMER_TWPS)));


int flag_timer = false;
int reset = true;
int led = 0;

/*typedef enum _pinNum{
	PIN1_1,
	PIN1_2,
	PIN2_1,
	PIN2_2,
	PIN3_1,
	PIN3_2,
	PIN4_1,
	PIN4_2,
	PIN5_1,
	PIN5_2,
	PIN6_1,
	PIN6_2,
	PIN7_1,
	PIN7_2,
	PIN8_1,
	PIN8_2,
	PIN9_1,
	PIN9_2
}pinNum;*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  disableWdt
 *  Description:  
 * =====================================================================================
 */
void disableWdt(void){
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));
	
	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  putCh
 *  Description:  
 * =====================================================================================
 */
void putCh(char c){
	while(!(HWREG(UART0_LSR) & (1<<5)));

	HWREG(UART0_THR) = c;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getCh
 *  Description:  
 * =====================================================================================
 */
char getCh(){
	while(!(HWREG(UART0_LSR) & (1<<0)));

	return(HWREG(UART0_RHR));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  putString
 *  Description:  
 * =====================================================================================
 */
int putString(char *str, unsigned int length){
	for(int i = 0; i < length; i++){
    	putCh(str[i]);
	}
	return(length);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getString
 *  Description:  
 * =====================================================================================
 */
int getString(char *buf, unsigned int length){
	for(int i = 0; i < length; i ++){
    	buf[i] = getCh();
   	}
	return(length);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerEnable
 *  Description:  
 * =====================================================================================
 */
void timerEnable(){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
    HWREG(DMTIMER_TCLR) |= 0x1;
}/* -----  end of function timerEnable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerDisable
 *  Description:  
 * =====================================================================================
 */
void timerDisable(){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
    HWREG(DMTIMER_TCLR) &= ~(0x1);
}/* -----  end of function timerEnable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delay
 *  Description:  
 * =====================================================================================
 */
void delay(unsigned int mSec){
#ifdef DELAY_USE_INTERRUPT
    unsigned int countVal = TIMER_OVERFLOW - (mSec * TIMER_1MS_COUNT);

   	/* Wait for previous write to complete */
	DMTimerWaitForWrite(0x2);

    /* Load the register with the re-load value */
	HWREG(DMTIMER_TCRR) = countVal;
	
	flag_timer = false;

    /* Enable the DMTimer interrupts */
	HWREG(DMTIMER_IRQENABLE_SET) = 0x2; 

    /* Start the DMTimer */
	timerEnable();

    while(flag_timer == false);

    /* Disable the DMTimer interrupts */
	HWREG(DMTIMER_IRQENABLE_CLR) = 0x2; 
#else
    while(mSec != 0){
        /* Wait for previous write to complete */
        DMTimerWaitForWrite(0x2);

        /* Set the counter value. */
        HWREG(DMTIMER_TCRR) = 0x0;

        timerEnable();

        while(HWREG(DMTIMER_TCRR) < TIMER_1MS_COUNT);

        /* Stop the timer */
        HWREG(DMTIMER_TCLR) &= ~(0x00000001u);

        mSec--;
    }
#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerSetup
 *  Description:  
 * =====================================================================================
 */
void timerSetup(void){
     /*  Clock enable for DMTIMER7 TRM 8.1.12.1.25 */
    HWREG(CM_PER_TIMER7_CLKCTRL) |= 0x2;

	/*  Check clock enable for DMTIMER7 TRM 8.1.12.1.25 */    
    while((HWREG(CM_PER_TIMER7_CLKCTRL) & 0x3) != 0x2);

#ifdef DELAY_USE_INTERRUPT
    /* Interrupt mask */
    HWREG(INTC_MIR_CLEAR2) |= (1<<31);//(95 --> Bit 31 do 3º registrador (MIR CLEAR2))
#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerIrqHandler
 *  Description:  
 * =====================================================================================
 */
void timerIrqHandler(void){

    /* Clear the status of the interrupt flags */
	HWREG(DMTIMER_IRQSTATUS) = 0x2; 

	flag_timer = true;

    /* Stop the DMTimer */
	timerDisable();

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  gpioSetup
 *  Description:  
 * =====================================================================================
 */
void gpioSetup(){
  /* set clock for GPIO1, TRM 8.1.12.1.31 */
   HWREG(CM_PER_GPIO1_CLKCTRL) = 0x40002;
   HWREG(CM_PER_GPIO2_CLKCTRL) = 0x40002;
   HWREG(CM_PER_GPIO3_CLKCTRL) = 0x40002;

  /* clear pin 21 for output, led USR0, TRM 25.3.4.3 */
	HWREG(INTC_MIR_CLEAR3) |= (1<<2);	//(98 --> Bit 2 do 4º registrador (MIR CLEAR3) - GPIO1_A)
	HWREG(INTC_MIR_CLEAR3) |= (1<<3);	//(99 --> Bit 3 do 4º registrador (MIR CLEAR3) - GPIO1_B)
	HWREG(INTC_MIR_CLEAR1) |= (1<<30);	//(62 --> Bit 2 do 4º registrador (MIR CLEAR3) - GPIO3_A)
	HWREG(INTC_MIR_CLEAR1) |= (1<<31);	//(63 --> Bit 2 do 4º registrador (MIR CLEAR3) - GPIO3_B)
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOff
 *  Description:  
 * =====================================================================================
 */
void ledOff(int pin){
	switch(pin){
		case 1:
			HWREG(GPIO1_DATAOUT) &= ~(1<<6);
			putString("1\n\r",3);
			break;
		case 2:
			HWREG(GPIO1_DATAOUT) &= ~(1<<2);
			putString("2\n\r",3);
			break;
		case 3:
			HWREG(GPIO1_DATAOUT) &= ~(1<<13);
			putString("3\n\r",3);
			break;
		case 4:
			HWREG(GPIO1_DATAOUT) &= ~(1<<15);
			putString("4\n\r",3);
			break;
		case 5:
			HWREG(GPIO1_DATAOUT) &= ~(1<<30);
			break;
		case 6:
			HWREG(GPIO1_DATAOUT) &= ~(1<<4);
			putString("6\n\r",3);
			break;
		case 7:
			HWREG(GPIO2_DATAOUT) &= ~(1<<10);
			break;
		case 8:
			HWREG(GPIO2_DATAOUT) &= ~(1<<8);
			break;
		case 9:
			HWREG(GPIO2_DATAOUT) &= ~(1<<6);
			putString("9\n\r",3);
			break;

		case 11:
			HWREG(GPIO1_DATAOUT) &= ~(1<<7);
			break;
		case 22:
			HWREG(GPIO1_DATAOUT) &= ~(1<<3);
			break;
		case 33:
			HWREG(GPIO1_DATAOUT) &= ~(1<<12);
			break;
		case 44:
			HWREG(GPIO1_DATAOUT) &= ~(1<<14);
			break;
		case 55:
			HWREG(GPIO1_DATAOUT) &= ~(1<<31);
			break;
		case 66:
			HWREG(GPIO1_DATAOUT) &= ~(1<<5);
			break;
		case 77:
			HWREG(GPIO2_DATAOUT) &= ~(1<<11);
			break;
		case 88:
			HWREG(GPIO2_DATAOUT) &= ~(1<<9);
			break;
		case 99:
			HWREG(GPIO2_DATAOUT) &= ~(1<<7);
			break;
		default:
			break;
	}/* -----  end switch  ----- */
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOn
 *  Description:  
 * =====================================================================================
 */
void ledOn(int pin){
	
	switch(pin) {
		case 1:
			//HWREG(GPIO1_SETDATAOUT) |= (1<<6);
			HWREG(GPIO1_DATAOUT) |= (1<<6);
			break;
		case 2:
			HWREG(GPIO1_DATAOUT) |= (1<<2);
			break;
		case 3:
			HWREG(GPIO1_DATAOUT) |= (1<<13);
			break;
		case 4:
			HWREG(GPIO1_DATAOUT) |= (1<<15);
			break;
		case 5:
			HWREG(GPIO1_DATAOUT) |= (1<<30);
			break;
		case 6:
			HWREG(GPIO1_DATAOUT) |= (1<<4);
			break;
		case 7:
			HWREG(GPIO2_DATAOUT) |= (1<<10);
			break;
		case 8:
			HWREG(GPIO2_DATAOUT) |= (1<<8);
			break;
		case 9:
			HWREG(GPIO2_DATAOUT) |= (1<<6);
			break;

		case 11:
			HWREG(GPIO1_DATAOUT) |= (1<<7);
			break;
		case 22:
			HWREG(GPIO1_DATAOUT) |= (1<<3);
			break;
		case 33:
			HWREG(GPIO1_DATAOUT) |= (1<<12);
			break;
		case 44:
			HWREG(GPIO1_DATAOUT) |= (1<<14);
			break;
		case 55:
			HWREG(GPIO1_DATAOUT) |= (1<<31);
			break;
		case 66:
			HWREG(GPIO1_DATAOUT) |= (1<<5);
			break;
		case 77:
			HWREG(GPIO2_DATAOUT) |= (1<<11);
			break;
		case 88:
			HWREG(GPIO2_DATAOUT) |= (1<<9);
			break;
		case 99:
			HWREG(GPIO2_DATAOUT) |= (1<<7);
			break;
		default:
			break;
	}/* -----  end switch  ----- */
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledConfig
 *  Description:  
 * =====================================================================================
 */
void ledConfig ( ){
    /*  configure pin mux for output GPIO */
	// player 1 
	HWREG(CM_PER_GPMCAD6_REGS)   |= 7;
	HWREG(CM_PER_GPMCAD2_REGS)   |= 7;
	HWREG(CM_PER_GPMCAD13_REGS)  |= 7;
	HWREG(CM_PER_GPMCAD15_REGS)  |= 7;
	HWREG(CM_PER_GPMCCSN1_REGS)  |= 7;
	HWREG(CM_PER_GPMCAD4_REGS)   |= 7;
	HWREG(CM_PER_LCD_DATA4_REGS) |= 7;
	HWREG(CM_PER_LCD_DATA2_REGS) |= 7;
	HWREG(CM_PER_LCD_DATA0_REGS) |= 7;

	// player 2 
	HWREG(CM_PER_GPMCAD7_REGS)   |= 7;
	HWREG(CM_PER_GPMCAD3_REGS)   |= 7;
	HWREG(CM_PER_GPMCAD12_REGS)  |= 7;	
	HWREG(CM_PER_GPMCAD14_REGS)  |= 7;
	HWREG(CM_PER_GPMCCSN2_REGS)  |= 7;
	HWREG(CM_PER_GPMCAD5_REGS)   |= 7;
	HWREG(CM_PER_LCD_DATA5_REGS) |= 7;
	HWREG(CM_PER_LCD_DATA3_REGS) |= 7;
	HWREG(CM_PER_LCD_DATA1_REGS) |= 7;
    
    /* clear pin 23 and 24 for output, leds USR3 and USR4, TRM 25.3.4.3 */
	//player 1
    HWREG(GPIO1_OE) &= ~(1<<6);
    HWREG(GPIO1_OE) &= ~(1<<2);
	HWREG(GPIO1_OE) &= ~(1<<13);
	HWREG(GPIO1_OE) &= ~(1<<15);
	HWREG(GPIO1_OE) &= ~(1<<30);
	HWREG(GPIO1_OE) &= ~(1<<4);
	HWREG(GPIO2_OE) &= ~(1<<10);
	HWREG(GPIO2_OE) &= ~(1<<8);
	HWREG(GPIO2_OE) &= ~(1<<6);

	//player 2 
    HWREG(GPIO1_OE) &= ~(1<<7);
	HWREG(GPIO1_OE) &= ~(1<<3);
	HWREG(GPIO1_OE) &= ~(1<<12);
	HWREG(GPIO1_OE) &= ~(1<<14);
	HWREG(GPIO1_OE) &= ~(1<<31);
	HWREG(GPIO1_OE) &= ~(1<<5);
	HWREG(GPIO2_OE) &= ~(1<<11);
	HWREG(GPIO2_OE) &= ~(1<<9);
	HWREG(GPIO2_OE) &= ~(1<<7);
	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  butConfig
 *  Description:  
 * =====================================================================================
 */
void butConfig ( ){
    /*  configure pin mux for input GPIO */
    HWREG(CM_PER_BEN1_REGS)       			|= 0x2F;
    HWREG(CM_PER_GPMCA0_REGS)				|= 0x2F;
	HWREG(CM_PER_MCASP0_FSR_REGS)    		|= 0x2F;
	HWREG(CM_PER_MCASP0_AXR1_REGS) 			|= 0x2F;
	
    /* clear pin 23 for input, led USR0, TRM 25.3.4.3 */
    HWREG(GPIO1_OE) |= 1<<16;
    HWREG(GPIO1_OE) |= 1<<28;
	HWREG(GPIO3_OE) |= 1<<19;
	HWREG(GPIO3_OE) |= 1<<20;


    /* Setting interrupt GPIO pin. */ 
	HWREG(GPIO1_IRQSTATUS_SET_0) |= 1<<16; // Reset
	HWREG(GPIO1_IRQSTATUS_SET_1) |= 1<<28; // Walk
	HWREG(GPIO3_IRQSTATUS_SET_0) |= 1<<19; // Botão player 1
	HWREG(GPIO3_IRQSTATUS_SET_1) |= 1<<20; // Botão player 2

  	/* Enable interrupt generation on detection of a rising edge.*/
	HWREG(GPIO1_RISINGDETECT) |= 1<<16;	//Reset
	HWREG(GPIO1_RISINGDETECT) |= 1<<28;	//Walk
	HWREG(GPIO3_RISINGDETECT) |= 1<<19;	//Botão player 1
	HWREG(GPIO3_RISINGDETECT) |= 1<<20;	//Botão player 2

	/* Deboucing */

	HWREG(GPIO1_DEBOUNCENABLE) |= 1<<16;
	HWREG(GPIO1_DEBOUNCENABLE) |= 1<<28;
	HWREG(GPIO3_DEBOUNCENABLE) |= 1<<19;
	HWREG(GPIO3_DEBOUNCENABLE) |= 1<<20;

	HWREG(GPIO1_DEBOUNCINGTIME) |= 0xFF;
	HWREG(GPIO3_DEBOUNCINGTIME) |= 0xFF;

}/* -----  end of function butConfig  ----- */