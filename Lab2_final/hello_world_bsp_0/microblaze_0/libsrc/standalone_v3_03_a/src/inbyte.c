<<<<<<< HEAD
#include "xparameters.h"
#include "xuartlite_l.h"

#ifdef __cplusplus
extern "C" {
#endif
char inbyte(void);
#ifdef __cplusplus
}
#endif 

char inbyte(void) {
	 return XUartLite_RecvByte(STDIN_BASEADDRESS);
}
=======
#include "xparameters.h"
#include "xuartlite_l.h"

#ifdef __cplusplus
extern "C" {
#endif
char inbyte(void);
#ifdef __cplusplus
}
#endif 

char inbyte(void) {
	 return XUartLite_RecvByte(STDIN_BASEADDRESS);
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
