/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CONSOL.h                                                *
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
*   External interface & Module configuration header for CONSOL (tty0).   *
*                                                                         *
**************************************************************************/


#ifndef __CONSOL_H__
#define __CONSOL_H__

#include "sysdefs.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */


/* ********************************************************************* */
/* Interface function definition */

/*
*********************************************************************************************
*                                       CONSOL_Init
*
* Description: This routine initializes the consol port by setting its baud rate & FIFO
*              buffer.
*
* Arguments  : wBaud     - Baud Rate in bit/sec.
*              bFifoFlag - FIFO 
*                          True  - FIFO enabled.
*                          False - FIFO disabled.
*
* Return     : none.
*
* Note(s)    : Before calling this, CONSOL_Select needs to be called for selecting the consol
*              port.
*********************************************************************************************
*/
void CONSOL_Init(U32 wBaud, U8 bFifoFlag);

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
char CONSOL_GetCh(void);

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
char CONSOL_GetChar(char * pbData);

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
void CONSOL_GetString(char *pbString);

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
int  CONSOL_GetIntNum(void);

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
void CONSOL_SendCh(char bData);

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
void CONSOL_SendChar(char bData);

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
void CONSOL_SendString(char *pbString);

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
void CONSOL_Scanf(char *pcFmt,...);

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
void CONSOL_Printf(char *pcFmt,...);


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__CONSOL_H__*/
