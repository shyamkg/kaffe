/*
 * mips/common.h
 * Common MIPS configuration information.
 *
 * Copyright (c) 1996, 1997, 1998, 1999
 *	Transvirtual Technologies, Inc.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution 
 * of this file. 
 *
 * by Alexandre Oliva <oliva@dcc.unicamp.br>
 * and Edouard G. Parmelan <egp@free.fr>
 */

#ifndef __mips_common_h
#define __mips_common_h

#if NEED_sysdepCallMethod

#if !defined (_MIPS_SIM) || (_MIPS_SIM == _MIPS_SIM_ABI32)

#define LONG_SYSDEP 1
#include "o32-sysdepCallMethod.h"
#undef LONG_SYSDEP
#include "o32-sysdepCallMethod.h"

#elif (_MIPS_SIM == _MIPS_SIM_NABI32)

#define LONG_SYSDEP 1
#include "n32-sysdepCallMethod.h"
#undef LONG_SYSDEP
#include "n32-sysdepCallMethod.h"

#else

#error "Calling convention not specified"

#endif

#endif /* NEED_sysdepCallMethod */


#if defined(HAVE_MIPSII_INSTRUCTIONS)
/*
 * Do an atomic compare and exchange.  The address 'A' is checked against
 * value 'O' and if they match it's exchanged with value 'N'.
 * We return '1' if the exchange is sucessful, otherwise 0.
 */
#define COMPARE_AND_EXCHANGE(A,O,N)		\
({						\
	unsigned int tmp, ret;			\
 						\
	asm volatile(				\
	"	.set	noreorder\n"		\
	"	.set	mips2\n"		\
	"1:	ll	%0, %2\n"		\
	"	bne	%0, %3,2f\n"		\
	"	move	%0, $0\n"		\
	"	move	%0, %4\n"		\
	"	sc	%0, %1\n"		\
	"	beqz	%0, 1b\n"		\
	"	nop\n"				\
	"2:\n"					\
	"	.set	mips0\n"		\
	"	.set	reorder\n"		\
	: "=&r" (ret), "=m" (*(A))		\
	: "m" (*(A)), "r" (O), "r" (N)		\
	: "memory");				\
	ret;					\
})

#else

/*
 * Do an atomic compare and exchange.  The address 'A' is checked against
 * value 'O' and if they match it's exchanged with value 'N'.
 * We return '1' if the exchange is sucessful, otherwise 0.
 */
#define COMPARE_AND_EXCHANGE(A,O,N)		\
({						\
    int ret = 0;				\
    jthread_suspendall();			\
						\
    if (*(A) == (O)) {				\
	*(A) = (N);				\
	ret = 1;				\
    }						\
    jthread_unsuspendall();			\
    ret;					\
})

#endif
    
#endif
