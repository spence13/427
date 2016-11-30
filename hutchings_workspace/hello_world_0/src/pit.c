#include "pit.h"
#include "xio.h"
#include <stdio.h>
#include <stdlib.h>
#include <xparameters.h>

#define PIT_CONTROL_INIT 7
#define PIT_DELAY_INIT 1000000
#define PIT_DELAY_CHARS 8
#define PIT0 0
#define PIT1 1
#define PIT2 2
#define PIT3 3
#define PIT4 4
#define PIT5 5
#define PIT6 6
#define PIT7 7

//Initialize delay value, and enable interrupts
void pit_init()
{
	XIo_Out32(/*XPAR_PIT_TIMER_0_BASEADDR*/XPAR_PIT_TIMER_0_BASEADDR+PIT_DELAY_VAL, PIT_DELAY_INIT); //Set delay value
	XIo_Out32(/*XPAR_PIT_TIMER_0_BASEADDR*/XPAR_PIT_TIMER_0_BASEADDR+PIT_CONTROL_REG, PIT_CONTROL_INIT); //Enable interrupts
}

//Wait for 8 characters on the UART to set number of clock cycles between interrupts
void pit_set_delay_value()
{
	char delay[PIT_DELAY_CHARS] = "";
	delay[PIT0] = getchar();
	delay[PIT1] = getchar();
	delay[PIT2] = getchar();
	delay[PIT3] = getchar();
	delay[PIT4] = getchar();
	delay[PIT5] = getchar();
	delay[PIT6] = getchar();
	delay[PIT7] = getchar();

	XIo_Out32(/*XPAR_PIT_TIMER_0_BASEADDR*/XPAR_PIT_TIMER_0_BASEADDR+PIT_DELAY_VAL, atoi(delay)); //Write new delay value

	xil_printf("Delay set to: %d\n\r", atoi(delay));
}
