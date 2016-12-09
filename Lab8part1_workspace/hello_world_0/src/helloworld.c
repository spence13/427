#include <stdio.h>
#include <stdint.h>
#include "dma_controller.h"
#include "platform.h"
#include "xparameters.h"



#define SWITCHES_MASK 0xE0

void print(char *str);

int main()
{
	uint32_t source_word[] = {0xDEADBEEF, 0xBEEFDEAD, 0xDEADDEEF};
	uint32_t destination_word[] = {0x0, 0x0, 0x0};

    init_platform();

    print("Hello World\n\r");
    cleanup_platform();
    printf("Printing value before DMA transfer.\n\r");
    int i = 0;
    for (i = 0; i < 3; i++)
    {
    	xil_printf("%x\n\r", destination_word[i]);
    }

    //DMA_CONTROLLER_MasterRecvWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &source_word);
    //DMA_CONTROLLER_MasterSendWord(XPAR_DMA_CONTROLLER_0_BASEADDR, (Xuint32) &destination_word);


    Xil_Out32(XPAR_DMA_CONTROLLER_0_BASEADDR + 4, 3);xil_printf("1\n\r");
    Xil_Out32(XPAR_DMA_CONTROLLER_0_BASEADDR + 8, &source_word);xil_printf("2\n\r");
    Xil_Out32(XPAR_DMA_CONTROLLER_0_BASEADDR + 12, &destination_word);xil_printf("3\n\r");

    Xil_Out16(XPAR_DMA_CONTROLLER_0_BASEADDR+DMA_CONTROLLER_MST_BE_REG_OFFSET, 0xFFFF);xil_printf("4\n\r");
    Xil_Out8(XPAR_DMA_CONTROLLER_0_BASEADDR+DMA_CONTROLLER_MST_GO_PORT_OFFSET, MST_START);xil_printf("5\n\r");

    printf("Printing value after DMA transfer.\n\r");
    i = 0;
	for (i = 0; i < 3; i++)
	{
		xil_printf("%x\n\r", destination_word[i]);
	}

    while(1)
    {
    	unsigned readSW = Xil_In32(XPAR_SWITCHES_BASEADDR) & SWITCHES_MASK;
    	xil_printf("%d\r\n", readSW);
    }
    //    cleanup_platform();

    return 0;
}
