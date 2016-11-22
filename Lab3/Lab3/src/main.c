<<<<<<< HEAD
/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "videoController.h"
#include "tank.h"
#include "bullet.h"
#include "bunker.h"
#include "stats.h"
#include "aliens.h"
#include "saucer.h"
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.

#define DEBUG
void print(char *str);

#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 10000000;

#define MAX_SECOND 100 // 100 x 10 ms = 1 second
#define MAX_HOLD MAX_SECOND/2 // (100 x 10 ms)/2 = 1/2 second
#define MAX_DEBOUNCE 10 // 5 x 10 ms = 50 ms debounce time
#define MAX_SEC_MIN 60 // Max number for seconds and minutes
#define MAX_VAL_SEC_MIN 59 // The max value minutes and seconds can have
#define MAX_VAL_HOUR 23 // The max value hours can have
#define MAX_HOUR 24 // Max number of hours
#define DECREMENT_TOO_FAR 255 // When we decrement an unsigned 8-bit past 0 it goes here to 255
#define MIDDLE_BTN_MASK 1 // The bit for the middle button
#define RIGHT_BTN_MASK 2 // The bit for the right button
#define BOTTOM_BTN_MASK 4 // The bit for the bottom button
#define LEFT_BTN_MASK 8 // The bit for the left button
#define TOP_BTN_MASK 16 // The bit for the top button

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

uint32_t currentButtonState = 0; // This holds the most current value of the buttons being pressed

uint8_t second_counter = 0; // This counts the ms until one second
uint8_t top_counter = 0; // This counts the ms that the top button is pressed
uint8_t bottom_counter = 0; // This counts the ms that the bottom button is pressed
uint8_t left_counter = 0; // This counts the ms that the left button is pressed
uint8_t right_counter = 0; // This counts the ms that the right button is pressed
uint8_t middle_counter = 0; // This counts the ms that the middle button is pressed
uint8_t top_hold_counter = 0; // This counts the ms while increment is being held
uint8_t bottom_hold_counter = 0; // This counts the ms while decrement is being held

uint8_t hours = 0; // The current hours on the timer
uint8_t minutes = 0; // The current minutes on the timer
uint8_t seconds = 0; // The current seconds on the timer

uint8_t hours_button = 0; // If the hours button is pressed, this = 1
uint8_t minutes_button = 0; // If the minutes button is pressed, this = 1
uint8_t seconds_button = 0; // If the seconds button is pressed, this = 1
uint8_t top_button = 0; // If the increment button has been held for a second, this = 1
uint8_t bottom_button = 0; // If the decrement button has been held for a second, this = 1

unsigned int * framePointer;

// This function will increment the time and print the time
void update_clock()
{
	if(seconds == MAX_SEC_MIN){ // Reset seconds and rollover the minutes if 60 seconds
		seconds = 0;
		if (!seconds_button) // Don't rollover time if the seconds are being incremented by the buttons
			minutes++;
	}
	else if (seconds == DECREMENT_TOO_FAR){ // If the seconds have decremented past 0 all the way to 255, reset seconds to 59
		seconds = MAX_VAL_SEC_MIN;
	}

	if(minutes == MAX_SEC_MIN){ // Reset minutes and rollover the hours if 60 minutes
		minutes = 0;
		if (!minutes_button) // Don't rollover time if the minutes are being incremented by the buttons
			hours++;
	}
	else if (minutes == DECREMENT_TOO_FAR){ // If the minutes have decremented past 0 all the way to 255, reset seconds to 59
		minutes = MAX_VAL_SEC_MIN;
	}

	if(hours == MAX_HOUR) // Reset hours if 24 hours
		hours = 0;
	else if (hours == DECREMENT_TOO_FAR){ // If the hours have decremented past 0 all the way to 255, reset seconds to 23
		hours = MAX_VAL_HOUR;
	}
	xil_printf("\r%02d:%02d:%02d", hours, minutes, seconds); // \r clears the line on the display, and %02 makes sure we pad with a 0 if needed
}

// This will increment the time depending on which buttons are being pressed
void increment_time()
{
	if(hours_button)
		hours++;
	if(minutes_button)
		minutes++;
	if(seconds_button)
		seconds++;
}

// This will decrement the time depending on which buttons are being pressed
void decrement_time()
{
	if(hours_button)
		hours--;
	if(minutes_button)
		minutes--;
	if(seconds_button)
		seconds--;
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce buttons, and 2) advances the time.
void timer_interrupt_handler() {
	////////////////////////////////////////////
	second_counter++; // Increment the ms counter for seconds
	if(second_counter == MAX_SECOND){ // If we have reached 100 x 10 ms = 1 sec
		seconds++; // Increment the seconds by one
		update_alien_pos(framePointer); // Move the aliens every second
		second_counter = 0; // Reset the counter for seconds
	}
	////////////////////////////////////////////
	////////////////////////////////////////////
	////////////////////////////////////////////
	if(currentButtonState & LEFT_BTN_MASK){ // If the left button has been pressed, increment the left_counter
		left_counter++;
		if (left_counter == MAX_DEBOUNCE){ // If the left counter has reached 50 ms, reset the counter and increment hours
			left_counter = 0;
			tank_shift_left(framePointer); //shift tank to the left
		}
	}
	else{ // Reset the counter and button value if button is released
		left_counter = 0;
		hours_button = 0;
	}
	////////////////////////////////////////////
	if(currentButtonState & MIDDLE_BTN_MASK){ // If the middle button has been pressed, increment the middle_counter
		middle_counter++;
		if (middle_counter == MAX_DEBOUNCE){ // If the middle counter has reached 50 ms, reset the counter and increment minutes
			middle_counter = 0;
			if(!bullet_get_is_fired())//only fire if bullet doesnt already exist
				bullet_draw(framePointer);//fire_tank_bullet();
		}
	}
	else{ // Reset the counter and button value if button is released
		middle_counter = 0;
		minutes_button = 0;
	}
	////////////////////////////////////////////
	if(currentButtonState & RIGHT_BTN_MASK){ // If the right button has been pressed, increment the right_counter
		right_counter++;
		if (right_counter == MAX_DEBOUNCE){ // If the right counter has reached 50 ms, reset the counter and increment seconds
			right_counter = 0;
			tank_shift_right(framePointer);//shift tank to the right
		}
	}
	else{ // Reset the counter and button value if button is released
		right_counter = 0;
		seconds_button = 0;
	}
	////////////////////////////////////////////
	////////////////////////////////////////////
	////////////////////////////////////////////
	if(currentButtonState & TOP_BTN_MASK){ // If the top button is pressed, increment counters
		top_counter++;
		top_hold_counter++;
		second_counter = 0; // Keep resetting the second_counter so it doesn't increment while changing the time

		if (top_counter == MAX_DEBOUNCE){ // If the top button has been pressed for 100 ms
			increment_time();
			update_clock();
		}
		if (top_counter == MAX_SECOND) // If the top button has been held for a second
			top_button++; // Increment the boolean value for top button
	}
	else{ // Reset counters if button is not pressed
		top_counter = 0;
		top_button = 0;
		top_hold_counter = 0;
	}

	if (top_button && top_hold_counter == MAX_HOLD){ // If the top button has been held for a second initially, this runs every half second
		top_hold_counter = 0; // Reset the hold_counter
		increment_time();
		update_clock();
	}
	////////////////////////////////////////////
	if(currentButtonState & BOTTOM_BTN_MASK){ // If the bottom button is pressed, increment counters
		bottom_counter++;
		bottom_hold_counter++;
		second_counter = 0; // Keep resetting the second_counter so it doesn't increment while changing the time

		if (bottom_counter == MAX_DEBOUNCE){ // If the bottom button has been pressed for 100 ms
			decrement_time();
			update_clock();
		}
		if (bottom_counter == MAX_SECOND) // If the bottom button has been held for a second
			bottom_button++; // Increment the boolean value for bottom button
	}
	else { // Reset counters if button is not pressed
		bottom_counter = 0;
		bottom_button = 0;
		bottom_hold_counter = 0;
	}

	if (bottom_button && bottom_hold_counter == MAX_HOLD){ // If the bottom button has been held for a second initially, this runs every half second
		bottom_hold_counter = 0; // Reset the hold_counter
		decrement_time();
		update_clock();
	}
	////////////////////////////////////////////
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
  // Everything else will be done in the timer_interrupt_handler()
  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is the timer_interrupt_handler() called after ack'ing the interrupt controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}

int main()
{
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
    XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
    // Step 2: Initialize the memory structure and the hardware.
    if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,	XPAR_AXI_VDMA_0_BASEADDR)) {
    	xil_printf("VideoDMA Did not initialize.\r\n");
    }
    // Step 3: (optional) set the frame store number.
    if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
    	xil_printf("Set Frame Store Failed.");
    }
    // Initialization is complete at this point.

    // Setup the frame counter. We want two read frames. We don't need any write frames but the
    // function generates an error if you set the write frame count to 0. We set it to 2
    // but ignore it because we don't need a write channel at all.
    XAxiVdma_FrameCounter myFrameConfig;
    myFrameConfig.ReadFrameCount = 2;
    myFrameConfig.ReadDelayTimerCount = 10;
    myFrameConfig.WriteFrameCount =2;
    myFrameConfig.WriteDelayTimerCount = 10;
    Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
    if (Status != XST_SUCCESS) {
	   xil_printf("Set frame counter failed %d\r\n", Status);
	   if(Status == XST_VDMA_MISMATCH_ERROR)
		   xil_printf("DMA Mismatch Error\r\n");
    }
    // Now we tell the driver about the geometry of our frame buffer and a few other things.
    // Our image is 480 x 640.
    XAxiVdma_DmaSetup myFrameBuffer;
    myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
    myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
    myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
    myFrameBuffer.FrameDelay = 0;
    myFrameBuffer.EnableCircularBuf=1;
    myFrameBuffer.EnableSync = 0;
    myFrameBuffer.PointNum = 0;
    myFrameBuffer.EnableFrameCounter = 0;
    myFrameBuffer.FixedFrameStoreAddr = 0;
    if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
    	xil_printf("DMA Config Failed\r\n");
     }
    // We need to give the frame buffer pointers to the memory that it will use. This memory
    // is where you will write your video data. The vdma IP/driver then streams it to the HDMI
    // IP.
     myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
     myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

     if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
    		               myFrameBuffer.FrameStoreStartAddr)) {
    	 xil_printf("DMA Set Address Failed Failed\r\n");
     }
     // Now, let's get ready to start displaying some stuff on the screen.
     // The variables framePointer and framePoi8nter1 are just pointers to the base address
     // of frame 0 and frame 1.
     framePointer = (unsigned int *) FRAME_BUFFER_0_ADDR;
     int row=0, col=0;
     for( row=0; row<480; row++)
     {
    	 for(col=0; col<640; col++)
    		 framePointer[row*640 + col] = 0; //Clear screen for the first and only time
     }

     // This tells the HDMI controller the resolution of your display (there must be a better way to do this).
     XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

     // Start the DMA for the read channel only.
     if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
    	 xil_printf("DMA START FAILED\r\n");
     }
     int frameIndex = 0;
     // We have two frames, let's park on frame 0. Use frameIndex to index them.
     // Note that you have to start the DMA process before parking on a frame.
     if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
    	 xil_printf("vdma parking failed\n\r");
     }

     update_alien_pos(framePointer); // Draw aliens for the first time
     tank_draw(framePointer); // Draw tank for the first time
     tank_draw_bottom_line(framePointer); // Draw the green line below the tank
     bunker_draw(framePointer); // Draw the bunkers
     stats_draw_score_word(framePointer); // Draw initial score
     stats_draw_lives_word(framePointer); // Draw lives word
     stats_draw_lives_tank(framePointer,1); // Draw first tank life
     stats_draw_lives_tank(framePointer,2); // Draw second tank life
     stats_draw_lives_tank(framePointer,3); // Draw third tank life
     srand(time(NULL));
 	////////////////////////////////////FROM LAB 2//////////////////////////////
     int success;
     //print("hello world\n\r");
     success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
     // Set the push button peripheral to be inputs.
     XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
     // Enable the global GPIO interrupt for push buttons.
     XGpio_InterruptGlobalEnable(&gpPB);
     // Enable all interrupts in the push button peripheral.
     XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

     microblaze_register_handler(interrupt_handler_dispatcher, NULL);
     XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
     		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
     XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
     microblaze_enable_interrupts();
     //////////////////////////////////////////////////////////////////////////////
     //Print out a menu on the UART for convenience
     xil_printf("2  to kill alien, enter 2-digit alien number (00-54)\n\r");
     xil_printf("3  to fire random alien missile\n\r");
     xil_printf("4  to move tank left\n\r");
     xil_printf("5  to fire tank bullet\n\r");
     xil_printf("6  to move tank right\n\r");
     xil_printf("7  to erode bunker, enter bunker number (0-3)\n\r");
     xil_printf("8  to update alien position\n\r");
     xil_printf("9  to update all bullets\n\r");
     while (1) {
    	 //update_display(framePointer); // Just keep calling update_display and wait for a char on the UART

         if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
        	 xil_printf("vdma parking failed\n\r");
         }
     }
     cleanup_platform();

    return 0;
}
=======
/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xio.h"
#include "time.h"
#include "unistd.h"
#include "videoController.h"
#include "tank.h"
#include "bullet.h"
#include "bunker.h"
#include "stats.h"
#include "aliens.h"
#include "saucer.h"
#include "xgpio.h"          // Provides access to PB GPIO driver.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.

#define DEBUG
void print(char *str);

#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.
#define MAX_SILLY_TIMER 10000000;

#define MAX_SECOND 100 // 100 x 10 ms = 1 second
#define MAX_HOLD MAX_SECOND/2 // (100 x 10 ms)/2 = 1/2 second
#define MAX_DEBOUNCE 10 // 5 x 10 ms = 50 ms debounce time
#define MAX_SEC_MIN 60 // Max number for seconds and minutes
#define MAX_VAL_SEC_MIN 59 // The max value minutes and seconds can have
#define MAX_VAL_HOUR 23 // The max value hours can have
#define MAX_HOUR 24 // Max number of hours
#define DECREMENT_TOO_FAR 255 // When we decrement an unsigned 8-bit past 0 it goes here to 255
#define MIDDLE_BTN_MASK 1 // The bit for the middle button
#define RIGHT_BTN_MASK 2 // The bit for the right button
#define BOTTOM_BTN_MASK 4 // The bit for the bottom button
#define LEFT_BTN_MASK 8 // The bit for the left button
#define TOP_BTN_MASK 16 // The bit for the top button

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.

uint32_t currentButtonState = 0; // This holds the most current value of the buttons being pressed

uint8_t second_counter = 0; // This counts the ms until one second
uint8_t top_counter = 0; // This counts the ms that the top button is pressed
uint8_t bottom_counter = 0; // This counts the ms that the bottom button is pressed
uint8_t left_counter = 0; // This counts the ms that the left button is pressed
uint8_t right_counter = 0; // This counts the ms that the right button is pressed
uint8_t middle_counter = 0; // This counts the ms that the middle button is pressed
uint8_t top_hold_counter = 0; // This counts the ms while increment is being held
uint8_t bottom_hold_counter = 0; // This counts the ms while decrement is being held

uint8_t hours = 0; // The current hours on the timer
uint8_t minutes = 0; // The current minutes on the timer
uint8_t seconds = 0; // The current seconds on the timer

uint8_t hours_button = 0; // If the hours button is pressed, this = 1
uint8_t minutes_button = 0; // If the minutes button is pressed, this = 1
uint8_t seconds_button = 0; // If the seconds button is pressed, this = 1
uint8_t top_button = 0; // If the increment button has been held for a second, this = 1
uint8_t bottom_button = 0; // If the decrement button has been held for a second, this = 1

unsigned int * framePointer;

// This function will increment the time and print the time
void update_clock()
{
	if(seconds == MAX_SEC_MIN){ // Reset seconds and rollover the minutes if 60 seconds
		seconds = 0;
		if (!seconds_button) // Don't rollover time if the seconds are being incremented by the buttons
			minutes++;
	}
	else if (seconds == DECREMENT_TOO_FAR){ // If the seconds have decremented past 0 all the way to 255, reset seconds to 59
		seconds = MAX_VAL_SEC_MIN;
	}

	if(minutes == MAX_SEC_MIN){ // Reset minutes and rollover the hours if 60 minutes
		minutes = 0;
		if (!minutes_button) // Don't rollover time if the minutes are being incremented by the buttons
			hours++;
	}
	else if (minutes == DECREMENT_TOO_FAR){ // If the minutes have decremented past 0 all the way to 255, reset seconds to 59
		minutes = MAX_VAL_SEC_MIN;
	}

	if(hours == MAX_HOUR) // Reset hours if 24 hours
		hours = 0;
	else if (hours == DECREMENT_TOO_FAR){ // If the hours have decremented past 0 all the way to 255, reset seconds to 23
		hours = MAX_VAL_HOUR;
	}
	xil_printf("\r%02d:%02d:%02d", hours, minutes, seconds); // \r clears the line on the display, and %02 makes sure we pad with a 0 if needed
}

// This will increment the time depending on which buttons are being pressed
void increment_time()
{
	if(hours_button)
		hours++;
	if(minutes_button)
		minutes++;
	if(seconds_button)
		seconds++;
}

// This will decrement the time depending on which buttons are being pressed
void decrement_time()
{
	if(hours_button)
		hours--;
	if(minutes_button)
		minutes--;
	if(seconds_button)
		seconds--;
}

// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce buttons, and 2) advances the time.
void timer_interrupt_handler() {
	////////////////////////////////////////////
	second_counter++; // Increment the ms counter for seconds
	if(second_counter == MAX_SECOND){ // If we have reached 100 x 10 ms = 1 sec
		seconds++; // Increment the seconds by one
		update_alien_pos(framePointer); // Move the aliens every second
		second_counter = 0; // Reset the counter for seconds
	}
	////////////////////////////////////////////
	////////////////////////////////////////////
	////////////////////////////////////////////
	if(currentButtonState & LEFT_BTN_MASK){ // If the left button has been pressed, increment the left_counter
		left_counter++;
		if (left_counter == MAX_DEBOUNCE){ // If the left counter has reached 50 ms, reset the counter and increment hours
			left_counter = 0;
			tank_shift_left(framePointer); //shift tank to the left
		}
	}
	else{ // Reset the counter and button value if button is released
		left_counter = 0;
		hours_button = 0;
	}
	////////////////////////////////////////////
	if(currentButtonState & MIDDLE_BTN_MASK){ // If the middle button has been pressed, increment the middle_counter
		middle_counter++;
		if (middle_counter == MAX_DEBOUNCE){ // If the middle counter has reached 50 ms, reset the counter and increment minutes
			middle_counter = 0;
			if(!bullet_get_is_fired())//only fire if bullet doesnt already exist
				bullet_draw(framePointer);//fire_tank_bullet();
		}
	}
	else{ // Reset the counter and button value if button is released
		middle_counter = 0;
		minutes_button = 0;
	}
	////////////////////////////////////////////
	if(currentButtonState & RIGHT_BTN_MASK){ // If the right button has been pressed, increment the right_counter
		right_counter++;
		if (right_counter == MAX_DEBOUNCE){ // If the right counter has reached 50 ms, reset the counter and increment seconds
			right_counter = 0;
			tank_shift_right(framePointer);//shift tank to the right
		}
	}
	else{ // Reset the counter and button value if button is released
		right_counter = 0;
		seconds_button = 0;
	}
	////////////////////////////////////////////
	////////////////////////////////////////////
	////////////////////////////////////////////
	if(currentButtonState & TOP_BTN_MASK){ // If the top button is pressed, increment counters
		top_counter++;
		top_hold_counter++;
		second_counter = 0; // Keep resetting the second_counter so it doesn't increment while changing the time

		if (top_counter == MAX_DEBOUNCE){ // If the top button has been pressed for 100 ms
			increment_time();
			update_clock();
		}
		if (top_counter == MAX_SECOND) // If the top button has been held for a second
			top_button++; // Increment the boolean value for top button
	}
	else{ // Reset counters if button is not pressed
		top_counter = 0;
		top_button = 0;
		top_hold_counter = 0;
	}

	if (top_button && top_hold_counter == MAX_HOLD){ // If the top button has been held for a second initially, this runs every half second
		top_hold_counter = 0; // Reset the hold_counter
		increment_time();
		update_clock();
	}
	////////////////////////////////////////////
	if(currentButtonState & BOTTOM_BTN_MASK){ // If the bottom button is pressed, increment counters
		bottom_counter++;
		bottom_hold_counter++;
		second_counter = 0; // Keep resetting the second_counter so it doesn't increment while changing the time

		if (bottom_counter == MAX_DEBOUNCE){ // If the bottom button has been pressed for 100 ms
			decrement_time();
			update_clock();
		}
		if (bottom_counter == MAX_SECOND) // If the bottom button has been held for a second
			bottom_button++; // Increment the boolean value for bottom button
	}
	else { // Reset counters if button is not pressed
		bottom_counter = 0;
		bottom_button = 0;
		bottom_hold_counter = 0;
	}

	if (bottom_button && bottom_hold_counter == MAX_HOLD){ // If the bottom button has been held for a second initially, this runs every half second
		bottom_hold_counter = 0; // Reset the hold_counter
		decrement_time();
		update_clock();
	}
	////////////////////////////////////////////
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
  // Everything else will be done in the timer_interrupt_handler()
  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
// Question: Why is the timer_interrupt_handler() called after ack'ing the interrupt controller
// but pb_interrupt_handler() is called before ack'ing the interrupt controller?
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		pb_interrupt_handler();
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
	}
}

int main()
{
	init_platform();                   // Necessary for all programs.
	int Status;                        // Keep track of success/failure of system function calls.
	XAxiVdma videoDMAController;
	// There are 3 steps to initializing the vdma driver and IP.
	// Step 1: lookup the memory structure that is used to access the vdma driver.
    XAxiVdma_Config * VideoDMAConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
    // Step 2: Initialize the memory structure and the hardware.
    if(XST_FAILURE == XAxiVdma_CfgInitialize(&videoDMAController, VideoDMAConfig,	XPAR_AXI_VDMA_0_BASEADDR)) {
    	xil_printf("VideoDMA Did not initialize.\r\n");
    }
    // Step 3: (optional) set the frame store number.
    if(XST_FAILURE ==  XAxiVdma_SetFrmStore(&videoDMAController, 2, XAXIVDMA_READ)) {
    	xil_printf("Set Frame Store Failed.");
    }
    // Initialization is complete at this point.

    // Setup the frame counter. We want two read frames. We don't need any write frames but the
    // function generates an error if you set the write frame count to 0. We set it to 2
    // but ignore it because we don't need a write channel at all.
    XAxiVdma_FrameCounter myFrameConfig;
    myFrameConfig.ReadFrameCount = 2;
    myFrameConfig.ReadDelayTimerCount = 10;
    myFrameConfig.WriteFrameCount =2;
    myFrameConfig.WriteDelayTimerCount = 10;
    Status = XAxiVdma_SetFrameCounter(&videoDMAController, &myFrameConfig);
    if (Status != XST_SUCCESS) {
	   xil_printf("Set frame counter failed %d\r\n", Status);
	   if(Status == XST_VDMA_MISMATCH_ERROR)
		   xil_printf("DMA Mismatch Error\r\n");
    }
    // Now we tell the driver about the geometry of our frame buffer and a few other things.
    // Our image is 480 x 640.
    XAxiVdma_DmaSetup myFrameBuffer;
    myFrameBuffer.VertSizeInput = 480;    // 480 vertical pixels.
    myFrameBuffer.HoriSizeInput = 640*4;  // 640 horizontal (32-bit pixels).
    myFrameBuffer.Stride = 640*4;         // Dont' worry about the rest of the values.
    myFrameBuffer.FrameDelay = 0;
    myFrameBuffer.EnableCircularBuf=1;
    myFrameBuffer.EnableSync = 0;
    myFrameBuffer.PointNum = 0;
    myFrameBuffer.EnableFrameCounter = 0;
    myFrameBuffer.FixedFrameStoreAddr = 0;
    if(XST_FAILURE == XAxiVdma_DmaConfig(&videoDMAController, XAXIVDMA_READ, &myFrameBuffer)) {
    	xil_printf("DMA Config Failed\r\n");
     }
    // We need to give the frame buffer pointers to the memory that it will use. This memory
    // is where you will write your video data. The vdma IP/driver then streams it to the HDMI
    // IP.
     myFrameBuffer.FrameStoreStartAddr[0] = FRAME_BUFFER_0_ADDR;
     myFrameBuffer.FrameStoreStartAddr[1] = FRAME_BUFFER_0_ADDR + 4*640*480;

     if(XST_FAILURE == XAxiVdma_DmaSetBufferAddr(&videoDMAController, XAXIVDMA_READ,
    		               myFrameBuffer.FrameStoreStartAddr)) {
    	 xil_printf("DMA Set Address Failed Failed\r\n");
     }
     // Now, let's get ready to start displaying some stuff on the screen.
     // The variables framePointer and framePoi8nter1 are just pointers to the base address
     // of frame 0 and frame 1.
     framePointer = (unsigned int *) FRAME_BUFFER_0_ADDR;
     int row=0, col=0;
     for( row=0; row<480; row++)
     {
    	 for(col=0; col<640; col++)
    		 framePointer[row*640 + col] = 0; //Clear screen for the first and only time
     }

     // This tells the HDMI controller the resolution of your display (there must be a better way to do this).
     XIo_Out32(XPAR_AXI_HDMI_0_BASEADDR, 640*480);

     // Start the DMA for the read channel only.
     if(XST_FAILURE == XAxiVdma_DmaStart(&videoDMAController, XAXIVDMA_READ)){
    	 xil_printf("DMA START FAILED\r\n");
     }
     int frameIndex = 0;
     // We have two frames, let's park on frame 0. Use frameIndex to index them.
     // Note that you have to start the DMA process before parking on a frame.
     if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
    	 xil_printf("vdma parking failed\n\r");
     }

     update_alien_pos(framePointer); // Draw aliens for the first time
     tank_draw(framePointer); // Draw tank for the first time
     tank_draw_bottom_line(framePointer); // Draw the green line below the tank
     bunker_draw(framePointer); // Draw the bunkers
     stats_draw_score_word(framePointer); // Draw initial score
     stats_draw_lives_word(framePointer); // Draw lives word
     stats_draw_lives_tank(framePointer,1); // Draw first tank life
     stats_draw_lives_tank(framePointer,2); // Draw second tank life
     stats_draw_lives_tank(framePointer,3); // Draw third tank life
     srand(time(NULL));
 	////////////////////////////////////FROM LAB 2//////////////////////////////
     int success;
     //print("hello world\n\r");
     success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
     // Set the push button peripheral to be inputs.
     XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
     // Enable the global GPIO interrupt for push buttons.
     XGpio_InterruptGlobalEnable(&gpPB);
     // Enable all interrupts in the push button peripheral.
     XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

     microblaze_register_handler(interrupt_handler_dispatcher, NULL);
     XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
     		(XPAR_FIT_TIMER_0_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
     XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
     microblaze_enable_interrupts();
     //////////////////////////////////////////////////////////////////////////////
     //Print out a menu on the UART for convenience
     xil_printf("2  to kill alien, enter 2-digit alien number (00-54)\n\r");
     xil_printf("3  to fire random alien missile\n\r");
     xil_printf("4  to move tank left\n\r");
     xil_printf("5  to fire tank bullet\n\r");
     xil_printf("6  to move tank right\n\r");
     xil_printf("7  to erode bunker, enter bunker number (0-3)\n\r");
     xil_printf("8  to update alien position\n\r");
     xil_printf("9  to update all bullets\n\r");
     while (1) {
    	 //update_display(framePointer); // Just keep calling update_display and wait for a char on the UART

         if (XST_FAILURE == XAxiVdma_StartParking(&videoDMAController, frameIndex,  XAXIVDMA_READ)) {
        	 xil_printf("vdma parking failed\n\r");
         }
     }
     cleanup_platform();

    return 0;
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
