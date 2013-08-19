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

#ifndef __MSGQ_H_
#define __MSGQ_H_

/* message queue options */

#define MSG_Q_TYPE_MASK	0x01	/* mask for pend queue type in options */
#define MSG_Q_FIFO	0x00	/* tasks wait in FIFO order */
#define MSG_Q_PRIORITY	0x01	/* tasks wait in PRIORITY order */
#define MSG_Q_EVENTSEND_ERR_NOTIFY 0x02 /* notify when eventRsrcSend fails */


/* message send priorities */

#define MSG_PRI_NORMAL	0	/* normal priority message */
#define MSG_PRI_URGENT	1	/* urgent priority message */


#endif

