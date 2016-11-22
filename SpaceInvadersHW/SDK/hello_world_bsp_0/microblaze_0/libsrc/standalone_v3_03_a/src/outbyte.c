<<<<<<< HEAD
#include "xparameters.h"
#include "xuartlite_l.h"

#ifdef __cplusplus
extern "C" {
#endif
void outbyte(char c); 

#ifdef __cplusplus
}
#endif 

void outbyte(char c) {
	 XUartLite_SendByte(STDOUT_BASEADDRESS, c);
}
=======
#include "xparameters.h"
#include "xuartlite_l.h"

#ifdef __cplusplus
extern "C" {
#endif
void outbyte(char c); 

#ifdef __cplusplus
}
#endif 

void outbyte(char c) {
	 XUartLite_SendByte(STDOUT_BASEADDRESS, c);
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
