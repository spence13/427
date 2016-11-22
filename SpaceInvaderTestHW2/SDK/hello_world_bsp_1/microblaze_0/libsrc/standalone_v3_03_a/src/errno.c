<<<<<<< HEAD
/* The errno variable is stored in the reentrancy structure.  This
   function returns its address for use by the macro errno defined in
   errno.h.  */

#include <errno.h>
#include <reent.h>

int *
__errno ()
{
  return &_REENT->_errno;
}
=======
/* The errno variable is stored in the reentrancy structure.  This
   function returns its address for use by the macro errno defined in
   errno.h.  */

#include <errno.h>
#include <reent.h>

int *
__errno ()
{
  return &_REENT->_errno;
}
>>>>>>> 46133ae2d6a022a717e89ee90e8959352f12048c
