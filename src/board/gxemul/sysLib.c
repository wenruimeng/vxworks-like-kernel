#include "includes.h"
#include "atlas.h"

#define IV_SWTRAP0_VEC          32      /* software trap 0              */
#define IV_SWTRAP1_VEC          33      /* software trap 1              */

#define INT_VEC_IORQ0           65
#define INT_VEC_IORQ1           66
#define INT_VEC_IORQ2           67
#define INT_VEC_IORQ3           68
#define INT_VEC_IORQ4           69
#define INT_VEC_IORQ5           70

/* shared mips int 0 */
#define IV_LAST_VEC             77
#define IV_IORQ0_BIT6_VEC		77	   
#define IV_IORQ0_BIT5_VEC		76	   
#define IV_IORQ0_BIT4_VEC		75	   
#define IV_IORQ0_BIT3_VEC		74	   
#define IV_IORQ0_BIT2_VEC		73	   
#define IV_IORQ0_BIT1_VEC		72	   
#define IV_IORQ0_BIT0_VEC		71

#define SYS_CLK_RATE_MIN        1           /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        3600        /* maximum system clock rate */

typedef struct 
{
    ULONG       intCause;               /* cause of interrupt   */
    ULONG       bsrTableOffset;         /* index to BSR table   */
    ULONG       statusReg;              /* interrupt level      */
    ULONG       pad;                    /* pad for ease of use  */
} PRIO_TABLE;

U8 ffsMsbTbl [256] =			/* lookup table for ffsMsb() */
    {
    0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    };

U8* sysHashOrder = ffsMsbTbl;


PRIO_TABLE intPrioTable[8] =
{
    {M_CauseIP0,(ULONG) IV_SWTRAP0_VEC, 0x0100, 0},      /* sw trap 0      */
    {M_CauseIP1,(ULONG) IV_SWTRAP1_VEC, 0x0200, 0},      /* sw trap 1      */
    {M_CauseIP2,(ULONG) INT_VEC_IORQ0 , 0x0400, 0},      /*share interrupt*/        
    {M_CauseIP3,(ULONG) INT_VEC_IORQ1 , 0x0800, 0},      /* */
    {M_CauseIP4,(ULONG) INT_VEC_IORQ2 , 0x1000, 0},      /* */
    {M_CauseIP5,(ULONG) INT_VEC_IORQ3 , 0x2000, 0},      /* */
    {M_CauseIP6,(ULONG) INT_VEC_IORQ4 , 0x4000, 0},      /* */
    {M_CauseIP7,(ULONG) INT_VEC_IORQ5 , 0x8000, 0}       /* */
};

static FUNCTION sysClkRoutine	= NULL; /* routine to call on clock interrupt */
static int sysClkArg		= 0; /* its argument */
static int sysClkRunning		= FALSE;
static int sysClkConnected	= FALSE;
static int sysClkTicksPerSecond	= 60;


static void dummyISR(int vec)
{
    return;
}

void sysClkInt()
{
	(*sysClkRoutine)(0);
}

/*extern int bTest;
extern SEM_ID bSemTest;*/

static void sharedInt0()
{
#if 0	
	extern ULONG intSRGet();
	extern void intSRSet(ULONG status);

	int i, j;
	ULONG sr;
	static int n = 0;

	if(n == 0)
	{
		n++;
		sr = intSRGet();
		sr |= 0x400;
		intSRSet(sr);
		
		i = 1;
		while(i > 0)
		{
			i--;
			j = 900;
			while(j > 0)
			{
				j--;
			}
		}
	}
	else
	{
		n++;
	}
#endif
/*
	extern int bTest;
	extern SEM_ID bSemTest;
	
	bTest++;
	semGive(bSemTest); */

	FUNCTION  intVec;
    int      idx;
    int irq;
	int ikey;
	unsigned char imr;
	

	ikey = intLock();
	sysoutbyte(PIC_MASTER_CMD, 0x0C); 	/* prepare for poll */
	irq = sysinbyte(PIC_MASTER_CMD) & 7;
	if (irq == PIC_CASCADE_IR) {
		/*
		 * Interrupt is cascaded so perform interrupt
		 * acknowledge on controller 2.
		 */
		sysoutbyte(PIC_SLAVE_CMD, 0x0C);		/* prepare for poll */
		irq = (sysinbyte(PIC_SLAVE_CMD) & 7) + 8;
	}

	if (irq == 7) {
		/*
		 * This may be a spurious interrupt.
		 *
		 * Read the interrupt status register (ISR). If the most
		 * significant bit is not set then there is no valid
		 * interrupt.
		 */
		sysoutbyte(PIC_MASTER_ISR, 0x0B); 	/* ISR register */
		if(sysinbyte(PIC_MASTER_ISR) & 0x80)
			irq = -1;
	}
	intUnlock(ikey);

	if(irq >= 0)
	{
		idx = irq+IV_IORQ0_BIT0_VEC;
		intVec = intVecGet((FUNCTION*)INUM_TO_IVEC(idx));
	    if(intVec)
		{
			(intVec)(idx);
	    }

		/*bTest++;
		semGive(bSemTest);*/

		/* reset interrupt */
		{
			ikey = intLock();
			imr = sysinbyte(PIC_MASTER_IMR);
			sysoutbyte(PIC_MASTER_IMR, imr | (1 << irq));
			sysoutbyte(PIC_MASTER_CMD, 0x60 | irq);
			sysoutbyte(PIC_MASTER_IMR, imr & (~(1 << irq)) );
			intUnlock(ikey);
		}
	}

}

void sysHwInit2()
{
	int i;
	
    /* connect dummy isr's to shared int vectors */
    for(i = INT_VEC_IORQ0; i <= IV_LAST_VEC; i++)
    {
       intConnect(INUM_TO_IVEC(i), dummyISR, i);
    }

	(void) intConnect (INUM_TO_IVEC(INT_VEC_IORQ0), sharedInt0, 0);

	intConnect (INUM_TO_IVEC(IV_IORQ0_BIT0_VEC), sysClkInt, 0);
	
}

STATUS sysClkConnect
(
    FUNCTION routine,	/* routine to be called at each clock interrupt */
    int arg		/* argument with which to call routine */
)
{
    if (sysClkConnected == FALSE)
    {
		sysHwInit2 ();
    }

    sysClkRoutine   = routine;
    sysClkArg	    = arg;
    sysClkConnected = TRUE;

    return (OK);
}


void sysClkDisable (void)
{
    int oldLevel;

    if (sysClkRunning)
	{
        oldLevel = intLock ();				/* LOCK INTERRUPT */
		sysoutbyte (PIT_CMD (PIT_BASE_ADR), 0x38);
		sysoutbyte (PIT_CNT0 (PIT_BASE_ADR), LSB(0));
		sysoutbyte (PIT_CNT0 (PIT_BASE_ADR), MSB(0));

		/*{
			unsigned char c = sysinbyte(PIC_MASTER_IMR) | 1;
			sysoutbyte(PIC_MASTER_IMR, c);
		}*/
		
        intUnlock (oldLevel);				/* UNLOCK INTERRUPT */

		sysClkRunning = FALSE;
	}
}

void sysClkEnable (void)
{
    UINT tc0;
    UINT tc2;
    int oldLevel;

    if (!sysClkRunning)
	{
	
		tc0 = PIT_CLOCK / sysClkTicksPerSecond;
        tc2 = PIT_CLOCK / sysClkTicksPerSecond * 2;

        oldLevel = intLock ();				/* LOCK INTERRUPT */

		sysoutbyte (PIT_CMD (PIT_BASE_ADR), 0x36);
		sysoutbyte (PIT_CNT0 (PIT_BASE_ADR), LSB(tc0));
		sysoutbyte (PIT_CNT0 (PIT_BASE_ADR), MSB(tc0));  

		/*{
			unsigned char c = sysinbyte(PIC_MASTER_IMR) & 0xfe;
			sysoutbyte(PIC_MASTER_IMR, c);
		}*/
		
	    intUnlock (oldLevel);				/* UNLOCK INTERRUPT */

		
		sysClkRunning = TRUE;
	}
}


int sysClkRateGet (void)
{
    return (sysClkTicksPerSecond);
}


STATUS sysClkRateSet
    (
    int ticksPerSecond	    /* number of clock interrupts per second */
    )
    {
    if (ticksPerSecond < SYS_CLK_RATE_MIN || ticksPerSecond > SYS_CLK_RATE_MAX)
		return (ERROR);

    sysClkTicksPerSecond = ticksPerSecond;

    if (sysClkRunning)
	{
		sysClkDisable ();
		sysClkEnable ();
	}

    return (OK);
    }

