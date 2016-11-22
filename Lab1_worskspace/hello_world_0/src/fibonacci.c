<<<<<<< HEAD
/*
 * fibonacci.c
 *
 *  Created on: Sep 1, 2016
 *      Author: superman
 */


#include <stdio.h>
#include "platform.h"

void print(char *str);

int main()
{
    init_platform();

    int fib[3];
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    int i = 0;
    //for loop
    for(i = 0; i<10; i++)
    {
    	fib[2] = fib[1] + fib[0];
    	xil_printf("%d\n\r", fib[2]);
    	//fib[2] = 0;
    	fib[0] = fib[1];
    	fib[1] = fib[2];

    }



    cleanup_platform();

    return 0;
}
=======
/*
 * fibonacci.c
 *
 *  Created on: Sep 1, 2016
 *      Author: superman
 */


#include <stdio.h>
#include "platform.h"

void print(char *str);

int main()
{
    init_platform();

    int fib[3];
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    int i = 0;
    //for loop
    for(i = 0; i<10; i++)
    {
    	fib[2] = fib[1] + fib[0];
    	xil_printf("%d\n\r", fib[2]);
    	//fib[2] = 0;
    	fib[0] = fib[1];
    	fib[1] = fib[2];

    }



    cleanup_platform();

    return 0;
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
