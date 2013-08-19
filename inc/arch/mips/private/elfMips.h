/* elfMips.h - Mips specific Extended Load Format header */

/* Copyright 1984-2001 Wind River Systems, Inc. */

/*
 * This file has been developed or significantly modified by the
 * MIPS Center of Excellence Dedicated Engineering Staff.
 * This notice is as per the MIPS Center of Excellence Master Partner
 * Agreement, do not remove this notice without checking first with
 * WR/Platforms MIPS Center of Excellence engineering management.
 */

/*
modification history
--------------------
01e,11dec01,pad  Removed all unnecessary types and definitions.
01d,16jul01,ros  add CofE comment
01c,14sep99,myz added a new relocation type R_MIPS16_26 for mips16
01b,17nov95,kkk added two new special section numbers
01a,28sep93,cd  derived from elfSparc.h.
*/

#ifndef __INCelfMipsh
#define __INCelfMipsh

#ifdef	__cplusplus
extern "C" {
#endif

/* MIPS ABI relocation types */

#define R_MIPS_NONE		0
#define R_MIPS_16		1
#define R_MIPS_32		2
#define R_MIPS_REL32		3	/* unsupported in VxWorks */
#define R_MIPS_26		4
#define R_MIPS_HI16		5
#define R_MIPS_LO16		6
#define R_MIPS_GPREL16		7	/* unsupported in VxWorks */
#define R_MIPS_LITERAL		8	/* unsupported in VxWorks */
#define R_MIPS_GOT16		9	/* unsupported in VxWorks */
#define R_MIPS_PC16		10	/* unsupported in VxWorks */
#define R_MIPS_CALL16		11	/* unsupported in VxWorks */
#define R_MIPS_GPREL32		12	/* unsupported in VxWorks */

#define EM_ARCH_MACHINE		EM_MIPS
#define EI_ARCH_CLASS		ELFCLASS32
#define EI_ARCH_DATA		ELFDATE2MSB

#ifdef	__cplusplus
}
#endif

#endif	/* __INCelfMipsh */
