#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* =============== Macro definitions =============== */

#define do_error_exit(msg) do {perror("[-] " msg); exit(EXIT_FAILURE); } while(0)

/* ================ Type definitions =============== */

/* ============== Function definitions ============= */

#endif /* _LOG_H_ */

