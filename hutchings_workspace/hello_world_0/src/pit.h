#ifndef PIT_H_
#define PIT_H_

#include <stdint.h>

#define PIT_CONTROL_REG 0 //BASEADDRESS +0 = slv_reg0
#define PIT_DELAY_VAL 4 //BASEADDRESS +4 = slv_reg1

void pit_init(); //Initialize delay value, and enable interrupts
void pit_set_delay_value(); //Wait for 8 characters on the UART to set number of clock cycles between interrupts

#endif /* PIT_H_ */
