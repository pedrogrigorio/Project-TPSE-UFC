#ifndef _BBB_REGS_H_
#define _BBB_REGS_H_

/*
 * =====================================================================================
 *
 *       Filename:  bb_regs
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/05/2018 17:47:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */




/* Interrupt */
#define INTC_BASE       						0x48200000
#define INTC_SIR_IRQ    						0x48200040
#define INTC_CONTROL    						0x48200048
#define INTC_ILR        						0x48200100
#define INTC_MIR_CLEAR1 						0x482000A8
#define INTC_MIR_CLEAR2 						0x482000C8
#define INTC_SYSCONFIG							0x48200010
#define INTC_SYSSTATUS							0x48200014
#define INTC_THRESHOLD							0x48200068
	
#define INTC_MIR_CLEAR3 						0x482000E8

/* UART0 */
#define UART0_BASE 								0x44E09000
#define UART0_RHR  								0x44E09000
#define UART0_THR  								0x44E09000
#define UART0_LSR  								0x44E09014

/* Watch Dog Timer */
#define  WDT_BASE 								0x44E35000
#define  WDT_WSPR 								0x44E35048
#define  WDT_WWPS 								0x44E35034

/* Clock */
#define CM_PER_GPIO1_CLKCTRL 					0x44E000AC
#define CM_PER_GPIO2_CLKCTRL 					0x44E000B0
#define CM_PER_GPIO3_CLKCTRL 					0x44E000B4
#define CM_PER_RTC_RTC_CLKCTRL   				0x44E00800
#define CM_PER_RTC_CLKSTCTRL     				0x44E00804
#define CM_PER_TIMER7_CLKCTRL					0x44E0007C
#define SOC_CM_DPLL_CLKSEL_TIMER7_CLK          	0x44E00504

/* LED PINS - EXPANSION HEADER P8 */
// PLAYER 1 
#define CM_PER_GPMCAD6_REGS					        0x44E10818 //GPIO1_6  - PIN 3
#define CM_PER_GPMCAD2_REGS					        0x44E10808 //GPIO1_2  - PIN 5
#define CM_PER_GPMCAD13_REGS					    0x44E10834 //GPIO1_13 - PIN 11
#define CM_PER_GPMCAD15_REGS					    0x44E1083C //GPIO1_15 - PIN 15
#define CM_PER_GPMCCSN1_REGS					    0x44E10880 //GPIO1_30 - PIN 21
#define CM_PER_GPMCAD4_REGS					        0x44E10810 //GPIO1_4  - PIN 23
#define CM_PER_LCD_DATA4_REGS					    0x44E108B0 //GPIO2_10 - PIN 41
#define CM_PER_LCD_DATA2_REGS					    0x44E108A8 //GPIO2_8  - PIN 43
#define CM_PER_LCD_DATA0_REGS					    0x44E108A0 //GPIO2_6  - PIN 45

// PLAYER 2 
#define CM_PER_GPMCAD7_REGS					        0x44E1081C //GPIO1_7  - PIN 4
#define CM_PER_GPMCAD3_REGS					        0x44E1080C //GPIO1_3  - PIN 6
#define CM_PER_GPMCAD12_REGS					    0x44E10830 //GPIO1_12 - PIN 12
#define CM_PER_GPMCAD14_REGS					    0x44E10838 //GPIO1_14 - PIN 16
#define CM_PER_GPMCCSN2_REGS					    0x44E10884 //GPIO1_31 - PIN 20
#define CM_PER_GPMCAD5_REGS					        0x44E10814 //GPIO1_5  - PIN 22
#define CM_PER_LCD_DATA5_REGS					    0x44E108B4 //GPIO2_11 - PIN 42
#define CM_PER_LCD_DATA3_REGS					    0x44E108AC //GPIO2_9  - PIN 44
#define CM_PER_LCD_DATA1_REGS					    0x44E108A4 //GPIO2_7  - PIN 46


/* BUTTON PINS - EXPANSION HEADER P9 */
#define CM_PER_GPMCA0_REGS					        0x44E10840 // GPIO1_16 PIN 15 RESET 
#define CM_PER_BEN1_REGS					        0x44E10878 // GPIO1_28 PIN 12 WALK 
#define CM_PER_MCASP0_FSR_REGS                      0x44E109A4 // GPIO3_19 PIN 27 PLAYER 1
#define CM_PER_MCASP0_AXR1_REGS                     0x44E109A8 // GPIO3_20 PIN 41 PLAYER 2

/* GPIO */\

#define GPIO0_IRQSTATUS_0					        0x44E0702C
#define GPIO0_IRQSTATUS_1                           0x44E07030
#define GPIO0_IRQSTATUS_SET_0                       0x44E07034
#define GPIO0_IRQSTATUS_SET_1                       0x44E07038
#define GPIO0_OE                                    0x44E07134
#define GPIO0_RISINGDETECT                          0x44E07148
#define GPIO0_CLEARDATAOUT                          0x44E07190
#define GPIO0_SETDATAOUT                            0x44E07194
#define GPIO1_DATAOUT   					        0x4804C13C
#define GPIO1_IRQSTATUS_0					        0x4804C02C
#define GPIO1_IRQSTATUS_1					        0x4804C030
#define GPIO1_IRQSTATUS_SET_0  				        0x4804C034
#define GPIO1_IRQSTATUS_SET_1 				        0x4804C038
#define GPIO1_OE            					    0x4804C134
#define GPIO1_RISINGDETECT  					    0x4804C148
#define GPIO1_CLEARDATAOUT  					    0x4804C190
#define GPIO1_SETDATAOUT   				 	        0x4804C194
#define GPIO1_DEBOUNCENABLE					        0x4804C150
#define GPIO1_DEBOUNCINGTIME					    0x4804C154
#define GPIO2_DATAOUT                               0x481AC13C
#define GPIO2_IRQSTATUS_0					        0x481AC02C
#define GPIO2_IRQSTATUS_1                           0x481AC030
#define GPIO2_IRQSTATUS_SET_0                       0x481AC034
#define GPIO2_IRQSTATUS_SET_1                       0x481AC038
#define GPIO2_OE                                    0x481AC134
#define GPIO2_RISINGDETECT                          0x481AC148
#define GPIO2_CLEARDATAOUT                          0x481AC190
#define GPIO2_SETDATAOUT                            0x481AC194
#define GPIO3_IRQSTATUS_0					        0x481AE02C
#define GPIO3_IRQSTATUS_1                           0x481AE030
#define GPIO3_IRQSTATUS_SET_0                       0x481AE034
#define GPIO3_IRQSTATUS_SET_1                       0x481AE038
#define GPIO3_OE                                    0x481AE134
#define GPIO3_RISINGDETECT                          0x481AE148
#define GPIO3_CLEARDATAOUT                          0x481AE190
#define GPIO3_SETDATAOUT                            0x481AE194
#define GPIO3_DEBOUNCENABLE					        0x481AE150
#define GPIO3_DEBOUNCINGTIME					    0x481AE154


/* The Input clock is selected as 24MHz. So for 1ms set the count to 0x5DC0. 
 *If the input clock is changed to different source this value has to be updated 
 *accordingly.	
*/
#define TIMER_1MS_COUNT                 		0x5DC0u
#define TIMER_OVERFLOW                  		0xFFFFFFFFu


/* DMTIMER */
#define SOC_DMTIMER_7_REGS                   	0x4804A000
#define DMTIMER_TIDR   							0x4804A000
#define DMTIMER_TIOCP_CFG   					0x4804A010
#define DMTIMER_IRQ_EOI   						0x4804A020
#define DMTIMER_IRQSTATUS_RAW   				0x4804A024
#define DMTIMER_IRQSTATUS   					0x4804A028
#define DMTIMER_IRQENABLE_SET   				0x4804A02C
#define DMTIMER_IRQENABLE_CLR   				0x4804A030
#define DMTIMER_IRQWAKEEN  	 					0x4804A034
#define DMTIMER_TCLR   							0x4804A038
#define DMTIMER_TCRR   							0x4804A03C
#define DMTIMER_TLDR   							0x4804A040
#define DMTIMER_TTGR   							0x4804A044
#define DMTIMER_TWPS   							0x4804A048
#define DMTIMER_TMAR   							0x4804A04C
#define DMTIMER_TCAR(n)   						(0x50 + (((n) - 1) * 8))
#define DMTIMER_TSICR  		 					0x4804A054



/* RTC */
#define RTC_BASE           						0x44E3E000
#define SECONDS_REG        						0x44E3E000
#define MINUTES_REG        						0x44E3E004
#define HOURS_REG          						0x44E3E008
#define RTC_CTRL_REG       						0x44E3E040
#define RTC_STATUS_REG     						0x44E3E044
#define RTC_INTERRUPTS_REG 						0x44E3E048
#define RTC_OSC_REG        						0x44E3E054
#define KICK0R             						0x44E3E06C
#define KICK1R             						0x44E3E070


#endif //__BB_REGS_H__

