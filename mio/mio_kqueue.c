/*
   MIO -- Managed Input/Output
   ---------------------------
*/

#include "mio.h"

#ifdef MIO_KQUEUE
#include "mio_kqueue.h"
#include "mio_impl.h"

mio_t mio_kqueue_new(int maxfd)
{
  return _mio_new(maxfd);
}
#endif
