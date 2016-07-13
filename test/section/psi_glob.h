#ifndef __PSI_HEADER__
#define __PSI_HEADER__

#include <stdio.h>
#include <iostream>
#include <memory>

#include "types.h"

/* -------------------------------------------------------------------------------------------------------------------
	declare define
------------------------------------------------------------------------------------------------------------------- */
#define PMT_HEADER_SIZE 	12
#define GET_2BYTE(x) 		(((*(x)) << 8)|((*(x+1))))
#define GET_PID(x) 			((GET_2BYTE(x))&0x1FFF)

/* -------------------------------------------------------------------------------------------------------------------
	declare struct 
------------------------------------------------------------------------------------------------------------------- */
typedef enum {
	MSG_PAT_DONE,
	MSG_PMT_DONE,
	MSG_SDTA_DONE,
	MSG_SDTO_DONE,
	MSG_NITA_DONE,
	MSG_NITO_DONE,
	MSG_EITA_DONE,
	MSG_EITO_DONE,
	MSG_MESSAGE_MAX
} MESSAGE_T;

#endif
