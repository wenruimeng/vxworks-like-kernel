/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CONSOL.c                                                *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : MIPS 4Kc (32 bit RISC) - ATLAS board                    *
*                                                                         *
*   TOOL-CHAIN  : SDE & Cygnus                                            *
*                                                                         *
*   DESCRIPTION :                                                         *
*   This is the CONSOL Driver module. Supports tty0 RS232 console         *
*   interfaces.                                                           *
*                                                                         *
**************************************************************************/


#include "sysdefs.h"
#include "atlas.h"
#include "consol.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


/* ********************************************************************* */
/* Global definitions */


/* ********************************************************************* */
/* File local definitions */

#define __isalpha(c) (c >'9')
#define __isupper(c) !(c & 0x20)

static U8 __bFifoFlag;
static U8 __bFifoCnt;


/* ********************************************************************* */
/* Local functions */


/* ********************************************************************* */
/* Global functions */

/*
*********************************************************************************************
*                                       CONSOL_Init
*
* Description: This routine initializes the consol port by setting its baud rate & FIFO
*              buffer.
*
* Arguments  : wBaud     - Baud Rate in bit/sec.
*              bFifoFlag - FIFO flag.
*                          True  - FIFO enabled.
*                          False - FIFO disabled.
*
* Return     : none.
*
* Note(s)    : Before calling this, CONSOL_Select needs to be called for selecting the consol
*              port.
*********************************************************************************************
*/

void CONSOL_Init(U32 wBaud, U8 bFifoFlag)
{
    tunSU16 unBaud;

    __bFifoFlag = bFifoFlag;
    __bFifoCnt = 0;

    unBaud.hwHW = 230400/wBaud;
    
    /*  Init baudrate : 
     *    1) enable DL-registers
     *    2) set DLL + DLM divisor registers
     *    3) disable DL-registers                                
     */
    BIT_SET(r16550LCTRL, 0x80);
    r16550DLL = unBaud.st2U8.bB0;
    r16550DLM = unBaud.st2U8.bB1;
    BIT_CLR(r16550LCTRL, 0x80);

    /*  Init LCR: 
     *    1) 8-bit data
     *    2) 1 stop bit
     *    3) no parity                                           
     */
    r16550LCTRL = 0x03;

    if(bFifoFlag)
    {
        /*  Init FCR: 
         *    1) enable FIFO
         *    2) reset receiver FIFO
         *    3) reset transmitter FIFO
         *    4) set receive FIFO threshold to 4                     
         */
        r16550IIFIFO = 0x47;
    }
    else
    {
        r16550IIFIFO = 0x00;
    }

    /*  Init MCR: 
     *    1) set DTR
     *    2) set RTS
     *    3) enable auto-flow control (bit 5 - both RX and TX)
     *          (Texas only;  SuperIO and Philips: No OPeration)
     *    4) enable interrupt mask (OUT2)
     *          (SuperIO and Philips only;  Texas: NC on Atlas,SEAD,Malta)
     */
    r16550MCTRL = 0x2b;

    /* Clear interrupt bits */
    r16550INTEN = 0x00;
}

/*
*********************************************************************************************
*                                       CONSOL_GetCh
*
* Description: This routine waits for a character from the CONSOL port & returns it.
*
* Arguments  : none.
*
* Return     : Returns the character read from the consol port.
*
* Note(s)    : 
*********************************************************************************************
*/

char CONSOL_GetCh(void)
{
    while(!(r16550LSTAT & (0x01<<0)));
    return r16550RXTX;
}

/*
*********************************************************************************************
*                                       CONSOL_GetChar
*
* Description: This routine reads a character from the consol port if available.
*
* Arguments  : pbData - Pointer to return the received data.
*
* Return     : The status of the RX buffer.
*              True  - Data returned is valid.
*              False - Data returned is invalid.
*
* Note(s)    : 
*********************************************************************************************
*/

char CONSOL_GetChar(char * pbData)
{
    if((r16550LSTAT & (0x01<<0)) != 0x00)
    {
        *pbData = r16550RXTX;
        return True;
    }
    return False;
}

/*
*********************************************************************************************
*                                       CONSOL_GetString
*
* Description: This routine waits for a string from the CONSOL port & returns it.
*
* Arguments  : pbString - Pointer to return the received string.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_GetString(char *pbString)
{
    char *pbString2=pbString;
    char bC;

    while((bC=CONSOL_GetCh())!='\r')
    {
        if(bC=='\b')
        {
            if((int)pbString2<(int)pbString){CONSOL_SendString("\b \b");pbString--;}
        }
        else
        {
            *pbString++=bC;CONSOL_SendCh(bC);
        }
    }
    *pbString='\0';
    CONSOL_SendCh('\n');
}

/*
*********************************************************************************************
*                                       CONSOL_GetIntNum
*
* Description: This routine waits for a Integer from the CONSOL port & returns it.
*
* Arguments  : none.
*
* Return     : Returns the received Integer value.
*
* Note(s)    : 
*********************************************************************************************
*/

int CONSOL_GetIntNum(void)
{
    char abStr[30];
    char *pbString=abStr;
    int wBase=10;
    int wMinus=0;
    int wLastIndex;
    int wResult=0;
    int wI;
        
    CONSOL_GetString(pbString);
        
    if(pbString[0]=='-'){wMinus=1;pbString++;}

    if(pbString[0]=='0' && (pbString[1]=='x' || pbString[1]=='X'))
    {
        wBase=16;
        pbString+=2;
    }
        
    wLastIndex=strlen(pbString)-1;
    if( pbString[wLastIndex]=='h' || pbString[wLastIndex]=='H' )
    {
        wBase=16;
        pbString[wLastIndex]=0;
        wLastIndex--;
    }

    if(wBase==10)
    {
        wResult=atoi(pbString);
        wResult=wMinus ? (-1*wResult):wResult;
    }
    else
    {
        for(wI=0; wI<=wLastIndex; wI++)
        {
            if(__isalpha(pbString[wI]))
            {
                if(__isupper(pbString[wI]))
                    wResult=(wResult<<4)+pbString[wI]-'A'+10;
                else
                    wResult=(wResult<<4)+pbString[wI]-'a'+10;
            }
            else
            {
                wResult=(wResult<<4)+pbString[wI]-'0';
            }
        }
        wResult=wMinus ? (-1*wResult):wResult;
    }
    return wResult;
}

/*
*********************************************************************************************
*                                       CONSOL_SendChar
*
* Description: This routine waits till the character is sent.
*
* Arguments  : bData - Data to be sent.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_SendChar(char bData)
{
    if(__bFifoFlag)
    {
        if(r16550LSTAT & (0x01<<5))
        {
            r16550RXTX = bData;
            __bFifoCnt = 1;
        }
        else
        {
            if(__bFifoCnt < 16)
            {
                r16550RXTX = bData;
                __bFifoCnt++;
            }
            else
            {
                while(!(r16550LSTAT & (0x01<<5))); //Wait until THR is empty
                r16550RXTX = bData;
                __bFifoCnt = 1;
            }
        }
    }
    else
    {
        while(!(r16550LSTAT & (0x01<<5))); //Wait until THR is empty
        r16550RXTX = bData;
    }
}

/*
*********************************************************************************************
*                                       CONSOL_SendCh
*
* Description: This routine waits till the character is sent. It also sends an extra carriage
*              return character when sending a new line character
*
* Arguments  : bData - Data to be sent.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_SendCh(char bData)
{
    if(bData == '\n')
    {
        CONSOL_SendChar('\r');
    }

    CONSOL_SendChar(bData);
}

/*
*********************************************************************************************
*                                       CONSOL_SendString
*
* Description: This routine waits till the string is sent.
*
* Arguments  : pbString - String to be sent.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_SendString(char *pbString)
{
    while(*pbString)CONSOL_SendCh(*pbString++);
}

/*
*********************************************************************************************
*                                       CONSOL_Scanf
*
* Description: Reads input from the consol stream, under control of the string pointed to by
*              format that specifies the admissible input sequences and how they are to be
*              converted for assignment, using subsequent arguments as pointers to the
*              objects to receive the converted input. If there are insufficient arguments
*              for the format, the behavior is undefined. If the format is exhausted while
*              arguments remain, the excess arguments are ignored.
*
* Arguments  : pcFmt - Format string. It can contain only the following format specifiers:
*                      %s - String.
*                      %c - character.
*                      %i - Integer.
*              ...   - Are the passed parameters (pointers to the objects to receive the
*                      converted input).
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_Scanf(char *pcFmt,...)
{
    va_list pArg;
    char cChar;
    int *pwInt;
    char *pbChar;
        
    va_start(pArg, pcFmt);
    while((cChar=*pcFmt++) != '\0')
    {
        if(cChar != '%')continue;
        switch(*pcFmt)
        {
            case 's':
            case 'S':
                pbChar = va_arg (pArg, char *);
                CONSOL_GetString(pbChar);
                break;
            case 'i':
            case 'I':
                pwInt = va_arg (pArg, int *);
                *pwInt = CONSOL_GetIntNum();
                break;
            case 'c':
            case 'C':
                pbChar = va_arg (pArg, char *);
                *pbChar = CONSOL_GetCh();
                break;
        }
    }
    va_end(pArg);
}

/*
*********************************************************************************************
*                                       CONSOL_Printf
*
* Description: Writes output to the consol stream, under control of the string pointed to by
*              format that specifies how subsequent arguments are converted for output. If 
*              there are insufficient arguments for the format, the behavior is undefined.
*              If the format is exhausted while arguments remain, the excess arguments are
*              ignored.
*
* Arguments  : pcFmt - Format string. It can contain all the format specifies.
*              ...   - Are the passed parameters (pointers to the objects to receive the
*                      converted input).
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/

void CONSOL_Printf(char *pcFmt,...)
{
    va_list ap;
    char pbString[256];

    va_start(ap,pcFmt);
    vsprintf(pbString,pcFmt,ap);
    CONSOL_SendString(pbString);
    va_end(ap);
}


/* ********************************************************************* */
