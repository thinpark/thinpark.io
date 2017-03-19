/*
 * LICENSE: GPL
 *
 * scm_cred.h
 *
 * Header file used by scm_cred_send.c and scm_cred_recv.c.
 */
#define _GNU_SOURCE             /* To get SCM_CREDENTIALS definition from
                                   <sys/sockets.h> */
#include <sys/socket.h>
#include <sys/un.h>
#include "unix_sockets.h"       /* Declares our socket functions */
#include "tlpi_hdr.h"

#define SOCK_PATH "scm_cred"
