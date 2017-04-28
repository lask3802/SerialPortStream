////////////////////////////////////////////////////////////////////////////////
// PROJECT : libnserial
//  (C) Jason Curl, 2016.
//
// FILE : modem.c
//
// Published under the MIT license.
//
// AUTHOR : Jason Curl
//
// DESCRIPTION : Get and set modem signals
//
////////////////////////////////////////////////////////////////////////////////

#include "config.h"

#include <pthread.h>
#include <semaphore.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#ifdef HAVE_LINUX_SERIAL_ICOUNTER_STRUCT
#include <linux/serial.h>
#endif

#define NSERIAL_EXPORTS
#include "nserial.h"
#include "errmsg.h"
#include "log.h"
#include "serialhandle.h"

static int getmodemsignal(int fd, int signal, int *outsignal)
{
  int serial;
  if (ioctl(fd, TIOCMGET, &serial) == -1) {
    return -1;
  }
  *outsignal = (serial & signal) ? 1 : 0;
  return 0;
}

NSERIAL_EXPORT int WINAPI serial_getdcd(struct serialhandle *handle, int *dcd)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  serial_seterror(handle, ERRMSG_OK);
  if (dcd == NULL) {
    serial_seterror(handle, ERRMSG_INVALIDPARAMETER);
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  if (getmodemsignal(handle->fd, TIOCM_CAR, dcd) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }
  return 0;
}

NSERIAL_EXPORT int WINAPI serial_getri(struct serialhandle *handle, int *ri)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  serial_seterror(handle, ERRMSG_OK);
  if (ri == NULL) {
    serial_seterror(handle, ERRMSG_INVALIDPARAMETER);
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  if (getmodemsignal(handle->fd, TIOCM_RI, ri) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }
  return 0;
}

NSERIAL_EXPORT int WINAPI serial_getdsr(struct serialhandle *handle, int *dsr)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  serial_seterror(handle, ERRMSG_OK);
  if (dsr == NULL) {
    serial_seterror(handle, ERRMSG_INVALIDPARAMETER);
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  if (getmodemsignal(handle->fd, TIOCM_DSR, dsr) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }
  return 0;
}

NSERIAL_EXPORT int WINAPI serial_getcts(struct serialhandle *handle, int *cts)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  serial_seterror(handle, ERRMSG_OK);
  if (cts == NULL) {
    serial_seterror(handle, ERRMSG_INVALIDPARAMETER);
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  if (getmodemsignal(handle->fd, TIOCM_CTS, cts) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }
  return 0;
}

NSERIAL_EXPORT int WINAPI serial_setdtr(struct serialhandle *handle, int dtr)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  int serial = TIOCM_DTR;
  int cmd = dtr ? TIOCMBIS : TIOCMBIC;
  if (ioctl(handle->fd, cmd, &serial) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }

  return 0;
}

NSERIAL_EXPORT int WINAPI serial_getdtr(struct serialhandle *handle, int *dtr)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  serial_seterror(handle, ERRMSG_OK);
  if (dtr == NULL) {
    serial_seterror(handle, ERRMSG_INVALIDPARAMETER);
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  int serial;
  if (ioctl(handle->fd, TIOCMGET, &serial) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }
  *dtr = (serial & TIOCM_DTR) ? 1 : 0;

  return 0;
}

NSERIAL_EXPORT int WINAPI serial_setrts(struct serialhandle *handle, int rts)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  int serial = TIOCM_RTS;
  int cmd = rts ? TIOCMBIS : TIOCMBIC;
  if (ioctl(handle->fd, cmd, &serial) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }

  return 0;
}

NSERIAL_EXPORT int WINAPI serial_getrts(struct serialhandle *handle, int *rts)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  serial_seterror(handle, ERRMSG_OK);
  if (rts == NULL) {
    serial_seterror(handle, ERRMSG_INVALIDPARAMETER);
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  int serial;
  if (ioctl(handle->fd, TIOCMGET, &serial) == -1) {
    serial_seterror(handle, ERRMSG_IOCTL);
    return -1;
  }
  *rts = (serial & TIOCM_RTS) ? 1 : 0;

  return 0;
}

#ifdef HAVE_LINUX_SERIAL_ICOUNTER_STRUCT
static serialmodemevent_t waitformodemevent(struct modemstate *mstate)
{
  int icount;
  int signals = 0;
  if (mstate->waitevent & MODEMEVENT_DCD) signals |= TIOCM_CAR;
  if (mstate->waitevent & MODEMEVENT_RI) signals |= TIOCM_RI;
  if (mstate->waitevent & MODEMEVENT_DSR) signals |= TIOCM_DSR;
  if (mstate->waitevent & MODEMEVENT_CTS) signals |= TIOCM_CTS;

  struct serial_icounter_struct icounter = {0, };
  if (ioctl(mstate->handle->fd, TIOCGICOUNT, &icounter) < 0) {
    // Not all serial port drivers support TIOCGICOUNT.
    //
    // If we get an error here, we assume that it's not supported and
    // we check the signals, which is a little less reliable.

    icount = FALSE;
  } else {
    icount = TRUE;
  }

  int ctssignal = -1;
  int dsrsignal = -1;
  int risignal = -1;
  int dcdsignal = -1;
  if ((mstate->waitevent & MODEMEVENT_CTS) &&
      getmodemsignal(mstate->handle->fd, TIOCM_CTS, &ctssignal) == -1)
    ctssignal = -1;
  if ((mstate->waitevent & MODEMEVENT_DSR) &&
      getmodemsignal(mstate->handle->fd, TIOCM_DSR, &dsrsignal) == -1)
    dsrsignal = -1;
  if ((mstate->waitevent & MODEMEVENT_DCD) &&
      getmodemsignal(mstate->handle->fd, TIOCM_CAR, &dcdsignal) == -1)
    dcdsignal = -1;
  if ((mstate->waitevent & MODEMEVENT_RI) &&
      getmodemsignal(mstate->handle->fd, TIOCM_RI, &risignal) == -1)
    risignal = -1;

  if (ioctl(mstate->handle->fd, TIOCMIWAIT, signals) < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
      return MODEMEVENT_NONE;
    }

    // Some USB drivers don't support modem signals.
    mstate->serialerror = ERRMSG_IOCTL;
    mstate->posixerrno = errno;
    return -1;
  }

  struct serial_icounter_struct ocounter = {0, };
  if (icount && ioctl(mstate->handle->fd, TIOCGICOUNT, &ocounter) < 0) {
    icount = FALSE;
  }

  // TODO: Do we just raise an event in case on from zero to one?
  //  CTS 0->1 and 1->0.
  //  DSR 0->1 and 1->0.
  //  DCD 0->1 and 1->0.
  //  RI  0->1 only.
  int rsignals = MODEMEVENT_NONE;
  if (icount) {
    if (ocounter.cts != icounter.cts) rsignals |= MODEMEVENT_CTS;
    if (ocounter.dsr != icounter.dsr) rsignals |= MODEMEVENT_DSR;
    if (ocounter.rng != icounter.rng) rsignals |= MODEMEVENT_RI;
    if (ocounter.dcd != icounter.dcd) rsignals |= MODEMEVENT_DCD;
  } else {
    int rctssignal = -1;
    int rdsrsignal = -1;
    int rrisignal = -1;
    int rdcdsignal = -1;
    if (ctssignal != -1 &&
	getmodemsignal(mstate->handle->fd, TIOCM_CTS, &rctssignal) == -1)
      rctssignal = -1;
    if (dsrsignal != -1 &&
	getmodemsignal(mstate->handle->fd, TIOCM_DSR, &rdsrsignal) == -1)
      rdsrsignal = -1;
    if (dcdsignal != -1 &&
	getmodemsignal(mstate->handle->fd, TIOCM_CAR, &rdcdsignal) == -1)
      rdcdsignal = -1;
    if (risignal != -1 &&
	getmodemsignal(mstate->handle->fd, TIOCM_RI, &rrisignal) == -1)
      rrisignal = -1;

    if (rctssignal != -1 && ctssignal != -1)
      rsignals |= (rctssignal != ctssignal) ? MODEMEVENT_CTS : MODEMEVENT_NONE;
    if (rdsrsignal != -1 && dsrsignal != -1)
      rsignals |= (rdsrsignal != dsrsignal) ? MODEMEVENT_DSR : MODEMEVENT_NONE;
    if (rdcdsignal != -1 && dcdsignal != -1)
      rsignals |= (rdcdsignal != dcdsignal) ? MODEMEVENT_DCD : MODEMEVENT_NONE;
    if (rrisignal != -1 && risignal != -1)
      rsignals |= (rrisignal != risignal) ? MODEMEVENT_RI : MODEMEVENT_NONE;
  }

  return rsignals & mstate->waitevent;
}

static void *modemeventthread(void *ptr)
{
  struct modemstate *mstate = ptr;
  serialmodemevent_t result;

  // The IOCTL blocks until there is a change or a signal. There's no other
  // way to get out if you don't want a signal! So we have to ensure that we
  // can cancel the thread at any time and be careful about it.
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

  while (TRUE) {
    result = waitformodemevent(mstate);
    if (result == -1) {
      // We have a serious issue. We stop the thread. The error
      // was already set by waitformodemevent.
      mstate->eventresult = MODEMEVENT_NONE;
      pthread_exit(NULL);
    }
    if (result != MODEMEVENT_NONE) {
      mstate->eventresult = result;
      pthread_exit(NULL);
    }
    // else we just keep polling.
  }
}
#endif

static int entercritsection(struct serialhandle *handle)
{
  if (pthread_mutex_lock(&(handle->modemmutex)) == -1) {
    nslog(handle, NSLOG_CRIT,
	  "modem: lock mutex failed: errno=%d", errno);
    serial_seterror(handle, ERRMSG_MUTEXLOCK);
    return -1;
  }
  return 0;
}

static int exitcritsection(struct serialhandle *handle)
{
  if (pthread_mutex_unlock(&(handle->modemmutex)) == -1) {
    nslog(handle, NSLOG_CRIT,
	  "modem: unlock mutex failed: errno=%d", errno);
    serial_seterror(handle, ERRMSG_MUTEXUNLOCK);
    return -1;
  }
  return 0;
}

NSERIAL_EXPORT serialmodemevent_t WINAPI serial_waitformodemevent(struct serialhandle *handle, serialmodemevent_t event)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

#ifdef HAVE_TIOCMIWAIT
  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  if (entercritsection(handle)) return -1;

  if (handle->modemstate) {
    exitcritsection(handle);
    nslog(handle, NSLOG_WARNING, "waitformodemevent: already running");
    serial_seterror(handle, ERRMSG_MODEMEVENT_RUNNING);
    errno = EINVAL;
    return -1;
  }

  if ((event &
       (MODEMEVENT_DCD | MODEMEVENT_RI |
	MODEMEVENT_DSR | MODEMEVENT_CTS)) == 0) {
    if (exitcritsection(handle)) return -1;
    return MODEMEVENT_NONE;
  }

  int result;

  struct modemstate mstate = {0, };
  handle->modemstate = &mstate;
  mstate.handle = handle;
  mstate.waitevent = event;
  if (exitcritsection(handle)) return -1;

  // We create the thread to wait on it afterwards, as we can
  // make that thread cancellable.
  result = pthread_create(&(handle->modemthread), NULL,
			  modemeventthread, &mstate);
  if (result) {
    nslog(handle, NSLOG_CRIT,
	  "waitformodemevent: pthread_create: errno=%d",
	  errno);
  }

  if (!result) {
    result = pthread_join(handle->modemthread, NULL);
    if (result) {
      nslog(handle, NSLOG_CRIT,
	    "waitformodemevent: pthread_join: errno=%d",
	    errno);
    }
  }

  if (!result) {
    if (mstate.serialerror != 0) {
      errno = mstate.posixerrno;
      nslog(handle, NSLOG_CRIT,
	    "waitformodemevent: error in modemeventthread: errno=%d",
	    errno);
      serial_seterror(handle, mstate.serialerror);
      result = -1;
    }
  }

  if (entercritsection(handle)) result = -1;
  handle->modemstate = NULL;
  if (exitcritsection(handle)) result = -1;

  if (!result) {
    return mstate.eventresult & event;
  } else {
    return -1;
  }
#else
  serial_seterror(handle, ERRMSG_NOSYS);
  errno = ENOSYS;
  return -1;
#endif
}

NSERIAL_EXPORT int WINAPI serial_abortwaitformodemevent(struct serialhandle *handle)
{
  if (handle == NULL) {
    errno = EINVAL;
    return -1;
  }

  if (handle->fd == -1) {
    serial_seterror(handle, ERRMSG_SERIALPORTNOTOPEN);
    errno = EBADF;
    return -1;
  }

  int active;
  if (entercritsection(handle)) return -1;
  active = (handle->modemstate != NULL);
  exitcritsection(handle);

  if (!active) return 0;

  int result = pthread_cancel(handle->modemthread);
  if (result == -1) {
    serial_seterror(handle, ERRMSG_PTHREADCANCEL);
    return -1;
  }

  return 0;
}

