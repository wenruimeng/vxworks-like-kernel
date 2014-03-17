/*! Copyright(c) Janson(http://www.gandancing.com) 2011-2012
 *
 *\file		frmwrk.c
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
 *	modify from MIPS port for uC/OS-II by Michael Anburaj
 *  Legal Declaration: it is for studying only.
 */


#include "includes.h"
#include "frmwrk.h"
#include "consol.h"
#include <stdio.h>

#include "mips.h"
#include "atlas.h"

/* Level 2 cache configuration */
U8  L2CACHE_bEnabled = False;

/* CPU attributes */
U8  CPU_b64Bit = False;


void __vLogo(void)
{
    CONSOL_Printf("\n\n%s Ver %s for %s :%s\n", __mLogo, __mVer, __mProcessor, __mDate);
    CONSOL_Printf("Built using %s on %s <%s>\n", "mips-elf-gcc",__DATE__,__TIME__);
    CONSOL_Printf("Developed by %s\n\n", __mAuthor);
}

/*
*********************************************************************************************
*                                       C_vMain
*
* Description: This is the main C entry function.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

extern U32 _freemem;
extern void kernelStart(char* pMemPoolStart, unsigned memPoolSize);
void usrClock();

void C_vMain(void)
{
	classLibInit();
	taskLibInit();
	kernelPreInit();
	
	semBLibInit();
	semMLibInit();
	semCLibInit();
	msgQLibInit();
	
	kernelInit(kernelStart, 100*1024, (char*)((U32)&_freemem+_SYS_STKSIZE)
		, 64*1024*1024+KSEG0BASE, 512*1024);
}

void kernelStart(char* pMemPoolStart, unsigned memPoolSize)
{
	extern void sysClkConnect(FUNCTION routine, int arg);
	extern void sysClkDisable();
	extern void sysClkEnable();
	extern STATUS sysClkRateSet(int ticksPerSecond);
	extern void kLogChar(char a);
	
	int taskId;
	int i, j = 0;

	taskId = 0;
	
	CONSOL_Init(__nConsolBaud, 0);

    CONSOL_Printf("test print\n");

	memPartLibInit(pMemPoolStart, memPoolSize);

	excVecInit();

	sysClkConnect((FUNCTION)usrClock, 0);
	sysClkRateSet(60);
	sysClkDisable();
	sysClkEnable();
	
	__vLogo();

    taskDelay(1);

	CONSOL_Printf("kernelStart pMemPoolStart:0x%x memPoolSize:0x%x\n", pMemPoolStart, memPoolSize);

	{
		demo(4);
	}

	/*{
		s3_sem1();
	}*/

	/*{
		s3_sem2();
	}*/

	/*{
		s4_msgq2();
	}*/

	/*{
		s7_inversion();
	}*/

	/*{
		s7_inversion2();
	}*/

	/* TODO for shell
       TODO for taskShow semShow msgQShow and so on.
	*/
	CONSOL_Printf("---------SHELL START---------\n--> ");
	{
		char key;
		for(;;)
		{
			if(CONSOL_GetChar(&key) == True)
	        {
	            if(key == 0x1B)
	            {
					while(1);
	            }
				if(key == 13)
				{
					CONSOL_Printf("\n--> ");
				}
				/*else if( ((key >= 'A') && 
					(key <= 'z')) )
				{
					CONSOL_SendChar(key);
				}*/
				else
				{
					CONSOL_SendChar(key);
				}
			}
			taskDelay(1);
		}
	}

}

void usrClock()
{
	tickAnnounce();
}

