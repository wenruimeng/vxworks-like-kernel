/*! Janson(http://www.gandancing.com) 2011-2012
 *
 *\file
 *\brief		
 *\details	
 *
 *\author	Janson
 *\version	
 *\date		04Jan12
 *
 *\warning	
 *
 *\history \arg	30Jan12, Janson, Create the file
 *	modify from VxWorks source
 *  Legal Declaration: it is for studying VxWorks only.
 */
#ifndef __KERNEL_Q_LIB_H__
#define __KERNEL_Q_LIB_H__

/* typedefs */

typedef struct		/* JOB */
    {
    FUNCTION function;		/* 00: function to invoke */
    int	arg1;				/* 04: argument 1 */
    int arg2;				/* 08: argument 2 */
    int arg3;				/* 12: argument 3 (unused) */
    } JOB;

#define WIND_JOBS_MAX	64				/* max q'ed jobs must be 64 */

/* variable declarations */

extern volatile U8	workQReadIx;		/* circular work queue read index */
extern volatile U8	workQWriteIx;		/* circular work queue read index */
extern volatile BOOL	workQIsEmpty;	/* TRUE if work queue is empty */
extern int	pJobPool[];					/* pool of memory for jobs */

void workQInit();
void workQAdd0(FUNCTION func);
void workQAdd1(FUNCTION func, int arg1);
void workQAdd2(FUNCTION func, int arg1, int arg2);
void workQDoWork(void);
void workQPanic(void);
#endif
