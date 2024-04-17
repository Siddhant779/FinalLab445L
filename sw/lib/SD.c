#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../inc/tm4c123gh6pm.h"
#include "SD.h"
#include "../inc/Timer5A.h"


#define SDC_CS           (*((volatile uint32_t *)0x40005004))
#define SDC_CS_LOW       0           // CS controlled by software
#define SDC_CS_HIGH      0x01

void Timer5_Init(void);
void SD_initSDCard(void){
    // for init chip select 
  SYSCTL_RCGCGPIO_R |= 0x02;            // activate clock for Port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){}; // allow time for clock to stabilize
  GPIO_PORTB_DIR_R |= 0x01;             // make PB0 out
  GPIO_PORTB_AFSEL_R &= ~0x01;          // disable alt funct on PB0
  GPIO_PORTB_DR4R_R |= 0x01;            // 4mA drive on outputs
  GPIO_PORTB_PUR_R |= 0x01;             // enable weak pullup on PB0
  GPIO_PORTB_DEN_R |= 0x01;             // enable digital I/O on PB0
                                        // configure PB0 as GPIO
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFFF0)+0x00000000;
  GPIO_PORTB_AMSEL_R &= ~0x01;          // disable analog functionality on PB0
  SDC_CS = SDC_CS_HIGH;
  /*
  *  SD_SCK: PB4           T_CS: -
 * SD_MISO: PB6          T_CLK: -
 * SD_MOSI: PB7           MISO: -
 *   SD_CS: PB0            LED: 3.3V
 * */
//MOSI for display is TM4C sending to display 
//mapping 7 6 5 4 3 2 1 0
//        1 1 0 1 0 0 0 0 - D- 13
//PA2 to PB4
//PA5 to PB7
//PA4 to PB6 -- can you check if this mapping makes sense 
  //not sure if i need to activate port b again but will do it again 
  //may need to add all the display code in here 
  SYSCTL_RCGCGPIO_R |= 0x02;            // activate clock for Port B
  while((SYSCTL_PRGPIO_R&0x02) == 0){}; // allow time for clock to stabilize
  GPIO_PORTB_AFSEL_R |= 0xD0;           // enable alt funct on PB4,7,6
  GPIO_PORTA_PUR_R |= 0xD0;             // enable weak pullup on PB4,7,6
  GPIO_PORTA_DEN_R |= 0xD0;             // enable digital I/O on PB4,7,6
  GPIO_PORTA_AMSEL_R &= ~0xD0;          // disable analog functionality on PA2,3,4,5

  SYSCTL_RCGCSSI_R |= 0x04;             // activate clock for SSI2
  while((SYSCTL_PRSSI_R&0x4) == 0){};  // allow time for clock to stabilize

  SSI2_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
  SSI2_CR1_R &= ~SSI_CR1_MS;            // master mode
                                        // configure for clock from source PIOSC for baud clock source
  SSI2_CC_R = (SSI2_CC_R&~SSI_CC_CS_M)+SSI_CC_CS_PIOSC;
                                        // clock divider for SSIClk (16 MHz PIOSC/CPSDVSR)
  SSI2_CPSR_R = (SSI2_CPSR_R&~SSI_CPSR_CPSDVSR_M)+4; // NEED TO CHECK THIS - IDK WHAT TO SET THE CLOCK DIVIDER AT SO JUST LEFT IT AT THIS 
  // CPSDVSR must be even from 2 to 254
  SSI2_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (16/CPSDVSR Mbps data rate)
                  SSI_CR0_SPH |         // SPH = 0
                  SSI_CR0_SPO);         // SPO = 0
                                        // FRF = Freescale format
  SSI2_CR0_R = (SSI2_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
                                        // DSS = 8-bit data
  SSI2_CR0_R = (SSI2_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_8;
  SSI2_CR1_R |= SSI_CR1_SSE;            // enable SSI



}