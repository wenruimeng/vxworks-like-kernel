/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : ATLAS.h                                                 *
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
*   Atlas 4Kc board address-MAP, IO register definition header file.      *
*                                                                         *
**************************************************************************/


#ifndef __ATLAS_H__
#define __ATLAS_H__

#include "sysdefs.h"
#include "mips.h"


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */
#define IVEC_TO_INUM(intVec)	((int) (intVec) >> 2)
#define INUM_TO_IVEC(intNum)	((VOIDFUNCPTR *) ((intNum) << 2))


/************************************************************************
 *  Assembly & C address-Maps & Bit fields
*************************************************************************/

/* EEPROM devices,  IIC-slaveadresses */

#define  ATLAS_EEPROM_IICADR_SN       0x54  /* NM24C09, 1024 bytes      */
#define  ATLAS_EEPROM_IICADR_SPD000   0x50  /* PC-SDRAM, 256 bytes      */



/* IIC-bus frequencies for the SAA9730 controller */

#define  ATLAS_IICFREQ_5156HZ         0x05
#define  ATLAS_IICFREQ_10312HZ        0x01
#define  ATLAS_IICFREQ_68750HZ        0x04
#define  ATLAS_IICFREQ_103125HZ       0x06
#define  ATLAS_IICFREQ_275000HZ       0x00
#define  ATLAS_IICFREQ_412500HZ       0x02



/* Physical address MAP (512 MByte) */

#define  ATLAS_SYSTEMRAM_BASE         0x00000000 /* System RAM:     */
#define  ATLAS_SYSTEMRAM_SIZE         0x10000000 /*   256 MByte     */

#define  ATLAS_PCIMEM1_BASE           0x10000000 /* PCI 1 memory:   */
#define  ATLAS_PCIMEM1_SIZE           0x0be00000 /*  128 + 60 MByte */

#define  ATLAS_PCIMEM2_BASE           0x18000000 /* PCI 2 memory    */
#define  ATLAS_PCIMEM2_SIZE           0x00200000 /*     2 MByte     */

#define  ATLAS_CORECTRL_BASE          0x1BE00000 /* Core control:   */
#define  ATLAS_CORECTRL_SIZE          0x00200000 /*     2 MByte     */

#define  ATLAS_SYSTEMFLASH_BASE       0x1C000000 /* System Flash:   */
#define  ATLAS_SYSTEMFLASH_SIZE       0x02000000 /*    32 MByte     */

#define  ATLAS_SYSTEMFLASH_SECTORSIZE 0x00020000 /* Sect. = 128 KB  */
#define  ATLAS_SYSTEMFLASH_BANKCOUNT           2 /*                 */
#define  ATLAS_SYSTEMFLASH_BLOCKCOUNT         64 /*                 */

#define  ATLAS_MONITORFLASH_BASE      0x1E000000 /* Monitor Flash:  */
#define  ATLAS_MONITORFLASH_SIZE      0x003E0000 /*     4 MByte     */
#define  ATLAS_MONITORFLASH_SECTORSIZE 0x00010000 /* Sect. = 64 KB  */

#define  ATLAS_RESERVED_BASE          0x1E400000 /* Reserved:       */
#define  ATLAS_RESERVED_SIZE          0x00C00000 /*    12 MByte     */

#define  ATLAS_FPGA_BASE              0x1F000000 /* FPGA:           */
#define  ATLAS_FPGA_SIZE              0x00C00000 /*    12 MByte     */

#define  ATLAS_BOOTROM_BASE           0x1FC00000 /* Boot ROM:       */
#define  ATLAS_BOOTROM_SIZE           0x00400000 /*     4 MByte     */



/* Use of PCI ranges (Memory vs I/O) */

#define  ATLAS_PCI_MEM_BASE           ATLAS_PCIMEM1_BASE
#define  ATLAS_PCI_MEM_SIZE           ATLAS_PCIMEM1_SIZE

#define  ATLAS_PCI_IO_BASE            ATLAS_PCIMEM2_BASE
#define  ATLAS_PCI_IO_SIZE            ATLAS_PCIMEM2_SIZE



/* FILEFLASH: upper 128 KByte (2x64 KByte sectors) of MONITORFLASH */

#define  ATLAS_FILEFLASH_BASE         0x1E3E0000 /* File Flash:     */
#define  ATLAS_FILEFLASH_SIZE         0x00020000 /*   128 KByte     */

#define  ATLAS_FILEFLASH_SECTORSIZE   0x00010000 /* Sect. = 64 KB   */



/* Devices, base adresses */

/*#define  ATLAS_ICTA_BASE              0x1F000000*/ /* FPGA, Int. ctrl,
                                                    ref. icta defs      */
/*#define  ATLAS_TMRA_BASE              0x1F000300*/ /* FPGA, Timer device,
                                                    ref. 'tmra.h'       */
/*#define  ATLAS_TI16C550_BASE          0x1F000900*/ /* TI16C550 UART,
                                                    upper connector     */

#define  ATLAS_ICTA_BASE              0x14000020

#define ATLAS_TMRA_BASE           	0x14000040
/* #define PIT_CLOCK 					1193182 */

#define ATLAS_TI16C550_BASE			0x140003f8



                                                    
/* FPGA, register address map */
/* REVISION: MUST BE FIXED TO  0x1FC00010 on any baseboard */

/******** reg: REVISION ********/
#define ATLAS_REVISION                0x1FC00010 /* REVISION              */

/* field: FPGRV */
#define ATLAS_REVISION_FPGRV_SHF      16
#define ATLAS_REVISION_FPGRV_MSK      (MSK(8) << ATLAS_REVISION_FPGRV_SHF)

/* field: CORID */
#define ATLAS_REVISION_CORID_SHF      10
#define ATLAS_REVISION_CORID_MSK      (MSK(6) << ATLAS_REVISION_CORID_SHF)

/* field: CORRV */
#define ATLAS_REVISION_CORRV_SHF      8
#define ATLAS_REVISION_CORRV_MSK      (MSK(2) << ATLAS_REVISION_CORRV_SHF)

/* field: PROID */
#define ATLAS_REVISION_PROID_SHF      4
#define ATLAS_REVISION_PROID_MSK      (MSK(4) << ATLAS_REVISION_PROID_SHF)

/* field: PRORV */
#define ATLAS_REVISION_PRORV_SHF      0
#define ATLAS_REVISION_PRORV_MSK      (MSK(4) << ATLAS_REVISION_PRORV_SHF)



/******** reg: HWINTACK ********/
#define ATLAS_HWINTACK                0x1F000100 /* HWINTACK              */

/* bit 1: ACKDEG */
#define ATLAS_HWINTACK_ACKDEG_SHF     1
#define ATLAS_HWINTACK_ACKDEG_MSK     (MSK(1) << ATLAS_HWINTACK_ACKDEG_SHF)
#define ATLAS_HWINTACK_ACKDEG_SET     ATLAS_HWINTACK_ACKDEG_MSK



/******** reg: NMISTATUS ********/
#define ATLAS_NMISTATUS               0x1F000024 /* NMISTATUS    */

/* bit 0: NMISTATUS */
#define ATLAS_NMISTATUS_ONNMI_SHF     0
#define ATLAS_NMISTATUS_ONNMI_MSK     (MSK(1) << ATLAS_NMISTATUS_ONNMI_SHF)
#define ATLAS_NMISTATUS_ONNMI_BIT     ATLAS_NMISTATUS_ONNMI_MSK    



/******** reg: NMIACK ********/
#define ATLAS_NMIACK                  0x1F000104 /* NMIACK        */

/* bit 0: NMI */
#define ATLAS_NMIACK_ONNMI_SHF        0
#define ATLAS_NMIACK_ONNMI_MSK        (MSK(1) << ATLAS_NMIACK_ONNMI_SHF)
#define ATLAS_NMIACK_ONNMI_BIT        ATLAS_NMIACK_ONNMI_MSK



/******** reg: SWVALUE ********/
#define ATLAS_SWVALUE                 0x1F000200 /* SWVALUE               */

/* bit 11: S14 */
#define ATLAS_SWVALUE_S14_SHF         11
#define ATLAS_SWVALUE_S14_MSK         (MSK(1) << ATLAS_SWVALUE_S14_SHF)
#define ATLAS_SWVALUE_S14_SET         ATLAS_SWVALUE_S14_MSK

/* bit 10: S13 */
#define ATLAS_SWVALUE_S13_SHF         10
#define ATLAS_SWVALUE_S13_MSK         (MSK(1) << ATLAS_SWVALUE_S13_SHF)
#define ATLAS_SWVALUE_S13_SET         ATLAS_SWVALUE_S13_MSK

/* bit 9: S12 */
#define ATLAS_SWVALUE_S12_SHF         9
#define ATLAS_SWVALUE_S12_MSK         (MSK(1) << ATLAS_SWVALUE_S12_SHF)
#define ATLAS_SWVALUE_S12_SET         ATLAS_SWVALUE_S12_MSK

/* bit 8: S11 */
#define ATLAS_SWVALUE_S11_SHF         8
#define ATLAS_SWVALUE_S11_MSK         (MSK(1) << ATLAS_SWVALUE_S11_SHF)
#define ATLAS_SWVALUE_S11_SET         ATLAS_SWVALUE_S11_MSK

/* bits 7:0: S2 */
#define ATLAS_SWVALUE_S2_SHF          0
#define ATLAS_SWVALUE_S2_MSK          (MSK(8) << ATLAS_SWVALUE_S2_SHF)



/******** reg: STATUS ********/
#define ATLAS_STATUS                  0x1F000208 /* STATUS                */

/* bit 4 : MFWR */
#define ATLAS_STATUS_MFWR_SHF         4
#define ATLAS_STATUS_MFWR_MSK         (MSK(1) << ATLAS_STATUS_MFWR_SHF)
#define ATLAS_STATUS_MFWR_SET         ATLAS_STATUS_MFWR_MSK

/* bit 3 : S54 */
#define ATLAS_STATUS_S54_SHF          3
#define ATLAS_STATUS_S54_MSK          (MSK(1) << ATLAS_STATUS_S54_SHF)
#define ATLAS_STATUS_S54_SET          ATLAS_STATUS_S54_MSK

/* bit 2 : S53 */
#define ATLAS_STATUS_S53_SHF          2
#define ATLAS_STATUS_S53_MSK          (MSK(1) << ATLAS_STATUS_S53_SHF)
#define ATLAS_STATUS_S53_SET          ATLAS_STATUS_S53_MSK

/* bit 1: BIGEND */
#define ATLAS_STATUS_BIGEND_SHF       1
#define ATLAS_STATUS_BIGEND_MSK       (MSK(1) << ATLAS_STATUS_BIGEND_SHF)
#define ATLAS_STATUS_BIGEND_SET       ATLAS_STATUS_BIGEND_MSK

/* bit 0: PCISYS */
#define ATLAS_STATUS_PCISYS_SHF       0
#define ATLAS_STATUS_PCISYS_MSK       (MSK(1) << ATLAS_STATUS_PCISYS_SHF)
#define ATLAS_STATUS_PCISYS_SET       ATLAS_STATUS_PCISYS_MSK



/******** reg: JMPRS ********/
#define ATLAS_JMPRS                   0x1F000210 /* JMPRS                 */

/* bit 1: EELOCK */
#define ATLAS_JMPRS_EELOCK_SHF        1
#define ATLAS_JMPRS_EELOCK_MSK        (MSK(1) << ATLAS_JMPRS_EELOCK_SHF)
#define ATLAS_JMPRS_EELOCK_SET        ATLAS_JMPRS_EELOCK_MSK

/* bit 0: PCI33M */
#define ATLAS_JMPRS_PCI33M_SHF        0
#define ATLAS_JMPRS_PCI33M_MSK        (MSK(1) << ATLAS_JMPRS_PCI33M_SHF)
#define ATLAS_JMPRS_PCI33M_SET        ATLAS_JMPRS_PCI33M_MSK



/******** reg: LEDGREEN ********/
#define ATLAS_LEDGREEN                0x1F000400 /* LEDGREEN              */

/* bits 0: ON */
#define ATLAS_LEDGREEN_ON_SHF         0
#define ATLAS_LEDGREEN_ON_MSK         (MSK(1) << ATLAS_LEDGREEN_ON_SHF)
#define ATLAS_LEDGREEN_ON_SET         ATLAS_LEDGREEN_ON_MSK



/******** reg: LEDBAR ********/
#define ATLAS_LEDBAR                  0x1F000408 /* LEDBAR                */



/******** reg: ASCII LED ********/
#define ATLAS_ASCIIWORD               0x1F000410 /* ASCIIWORD             */
#define ATLAS_ASCIIPOS0               0x1F000418 /* ASCIIPOS0             */
#define ATLAS_ASCIIPOS1               0x1F000420 /* ASCIIPOS1             */
#define ATLAS_ASCIIPOS2               0x1F000428 /* ASCIIPOS2             */
#define ATLAS_ASCIIPOS3               0x1F000430 /* ASCIIPOS3             */
#define ATLAS_ASCIIPOS4               0x1F000438 /* ASCIIPOS4             */
#define ATLAS_ASCIIPOS5               0x1F000440 /* ASCIIPOS5             */
#define ATLAS_ASCIIPOS6               0x1F000448 /* ASCIIPOS6             */
#define ATLAS_ASCIIPOS7               0x1F000450 /* ASCIIPOS7             */



/******** reg: SOFTRES ********/
#define ATLAS_SOFTRES                 0x1F000500 /* SOFTRES               */

/* bits 7:0: RESET */
#define ATLAS_SOFTRES_RESET_SHF       0
#define ATLAS_SOFTRES_RESET_MSK       (MSK(8) << ATLAS_SOFTRES_RESET_SHF)
#define ATLAS_SOFTRES_RESET_GORESET   0x42     /* magic value to reset    */    



/******** reg: BRKRES ********/
#define ATLAS_BRKRES                  0x1F000508 /* BRKRES                */

/* bits 7:0: WIDTH */
#define ATLAS_BRKRES_WIDTH_SHF        0
#define ATLAS_BRKRES_WIDTH_MSK        (MSK(8) << ATLAS_BRKRES_WIDTH_SHF)



/******** reg: PSUSTBY ********/
#define ATLAS_PSUSTBY                 0x1F000600 /* PSUSTBY               */

/* bits 7:0: STBY */
#define ATLAS_PSUSTBY_STBY_SHF        0
#define ATLAS_PSUSTBY_STBY_MSK        (MSK(8) << ATLAS_PSUSTBY_STBY_SHF)
#define ATLAS_PSUSTBY_STBY_GOSTBY     0x4D     /* magic value to go stby  */    



/******** reg: SFWCTRL ********/
#define ATLAS_SFWCTRL                 0x1F000700 /* SFWCTRL               */

/* bits 7:0: WRENA */
#define ATLAS_SFWCTRL_WRENA_SHF       0
#define ATLAS_SFWCTRL_WRENA_MSK       (MSK(8) << ATLAS_SFWCTRL_WRENA_SHF)
#define ATLAS_SFWCTRL_WRENA_ENSFWRITE 0xC7   /* magic value to enable 
                                                write protect           */    


/******** reg: RTCADR ********/
#define ATLAS_RTCADR                  0x1F000800 /* RTCADR                */

/* bits 7:0: ADR */
#define ATLAS_RTCADR_ADR_SHF          0
#define ATLAS_RTCADR_ADR_MSK          (MSK(8) << ATLAS_RTCADR_ADR_SHF)



/******** reg: RTCDAT ********/
#define ATLAS_RTCDAT                  0x1F000808 /* RTCDAT                */

/* bits 7:0: DATA */
#define ATLAS_RTCDAT_DATA_SHF         0
#define ATLAS_RTCDAT_DATA_MSK         (MSK(8) << ATLAS_RTCDAT_DATA_SHF)



/******** reg: GPOUT ********/
#define ATLAS_GPOUT                   0x1F000A00 /* GPOUT                 */

/* bits 7:0: OUTVAL */
#define ATLAS_GPOUT_OUTVAL_SHF        0
#define ATLAS_GPOUT_OUTVAL_MSK        (MSK(8) << ATLAS_GPOUT_OUTVAL_SHF)



/******** reg: GPINP ********/
#define ATLAS_GPINP                   0x1F000A08 /* GPINP                 */

/* bits 7:0: INPVAL */
#define ATLAS_GPINP_INPVAL_SHF        0
#define ATLAS_GPINP_INPVAL_MSK        (MSK(8) << ATLAS_GPINP_INPVAL_SHF)




/* PCI definitions */

/* ADP bit used as IDSEL during configuration cycles */
#define ATLAS_IDSEL_21150             25
#define ATLAS_IDSEL_SYM53C810A        26
#define ATLAS_IDSEL_CORE              27
#define ATLAS_IDSEL_CONNECTOR         28
#define ATLAS_IDSEL_SAA9730           29


/**** Interrupt lines for ATLAS devices (on interrupt controller) ****/

/* PCI INTA..D       */
#define ATLAS_INTLINE_PCIA            15
#define ATLAS_INTLINE_PCIB            16
#define ATLAS_INTLINE_PCIC            17
#define ATLAS_INTLINE_PCID            18
/* Compact PCI A..D  */
#define ATLAS_INTLINE_CPCIA           8
#define ATLAS_INTLINE_CPCIB           9
#define ATLAS_INTLINE_CPCIC           10
#define ATLAS_INTLINE_CPCID           11
/* Local PCI devices */
#define ATLAS_INTLINE_9730            ATLAS_INTLINE_PCIB
#define ATLAS_INTLINE_SCSI            ATLAS_INTLINE_PCIC
/* Other devices     */
#define ATLAS_INTLINE_COREHI          5

/**** CPU interrupt lines used by devices ****/
#define ATLAS_CPUINT_ICTA                C0_STATUS_IM_HW0



/* MISC definitions */

/* Lowest possible frequency for cpu (used during init for conservative
 * setup of timing (e.g. SDRAM refresh)
 */
#define ATLAS_CPUFREQ_LOWEST_MHZ      2

/* PCI device numbers */
#define ATLAS_DEVNUM_SAA9730          PCI_IDSEL2DEVNUM(ATLAS_IDSEL_SAA9730)
#define ATLAS_DEVNUM_PCI_SLOT         PCI_IDSEL2DEVNUM(ATLAS_IDSEL_CONNECTOR)



/************************************************************************
 *  Interrupt Controller definition
*************************************************************************/

/**** Number of interrupt lines ****/
#define ICTA_IC_COUNT                 20



/* Relative Register Addresses & bit fields */

/******** reg: INTRAW ********/
#define ICTA_INTRAW_OFS               0x00 /* RAW value on external int lines */

/* bit 19: PCISERRN */
#define ICTA_INTRAW_PCISERRN_SHF      19
#define ICTA_INTRAW_PCISERRN_MSK      (MSK(1) << ICTA_INTRAW_PCISERRN_SHF)
#define ICTA_INTRAW_PCISERRN_SET      ICTA_INTRAW_PCISERRN_MSK

/* bit 18: CONINTDN */
#define ICTA_INTRAW_CONINTDN_SHF      18
#define ICTA_INTRAW_CONINTDN_MSK      (MSK(1) << ICTA_INTRAW_CONINTDN_SHF)
#define ICTA_INTRAW_CONINTDN_SET      ICTA_INTRAW_CONINTDN_MSK

/* bit 17: CONINTCN */
#define ICTA_INTRAW_CONINTCN_SHF      17
#define ICTA_INTRAW_CONINTCN_MSK      (MSK(1) << ICTA_INTRAW_CONINTCN_SHF)
#define ICTA_INTRAW_CONINTCN_SET      ICTA_INTRAW_CONINTCN_MSK

/* bit 16: CONINTBN */
#define ICTA_INTRAW_CONINTBN_SHF      16
#define ICTA_INTRAW_CONINTBN_MSK      (MSK(1) << ICTA_INTRAW_CONINTBN_SHF)
#define ICTA_INTRAW_CONINTBN_SET      ICTA_INTRAW_CONINTBN_MSK

/* bit 15: CONINTAN */
#define ICTA_INTRAW_CONINTAN_SHF      15
#define ICTA_INTRAW_CONINTAN_MSK      (MSK(1) << ICTA_INTRAW_CONINTAN_SHF)
#define ICTA_INTRAW_CONINTAN_SET      ICTA_INTRAW_CONINTAN_MSK

/* bit 14: ATXOKN */
#define ICTA_INTRAW_ATXOKN_SHF        14
#define ICTA_INTRAW_ATXOKN_MSK        (MSK(1) << ICTA_INTRAW_ATXOKN_SHF)
#define ICTA_INTRAW_ATXOKN_SET        ICTA_INTRAW_ATXOKN_MSK

/* bit 13: DEG */
#define ICTA_INTRAW_DEG_SHF           13
#define ICTA_INTRAW_DEG_MSK           (MSK(1) << ICTA_INTRAW_DEG_SHF)
#define ICTA_INTRAW_DEG_SET           ICTA_INTRAW_DEG_MSK

/* bit 12: ENUM */
#define ICTA_INTRAW_ENUM_SHF          12
#define ICTA_INTRAW_ENUM_MSK          (MSK(1) << ICTA_INTRAW_ENUM_SHF)
#define ICTA_INTRAW_ENUM_SET          ICTA_INTRAW_ENUM_MSK

/* bit 11: PCID */
#define ICTA_INTRAW_PCID_SHF          11
#define ICTA_INTRAW_PCID_MSK          (MSK(1) << ICTA_INTRAW_PCID_SHF)
#define ICTA_INTRAW_PCID_SET          ICTA_INTRAW_PCID_MSK

/* bit 10: PCIC */
#define ICTA_INTRAW_PCIC_SHF          10
#define ICTA_INTRAW_PCIC_MSK          (MSK(1) << ICTA_INTRAW_PCIC_SHF)
#define ICTA_INTRAW_PCIC_SET          ICTA_INTRAW_PCIC_MSK

/* bit 9: PCIB */
#define ICTA_INTRAW_PCIB_SHF          9
#define ICTA_INTRAW_PCIB_MSK          (MSK(1) << ICTA_INTRAW_PCIB_SHF)
#define ICTA_INTRAW_PCIB_SET          ICTA_INTRAW_PCIB_MSK

/* bit 8: PCIA */
#define ICTA_INTRAW_PCIA_SHF          8
#define ICTA_INTRAW_PCIA_MSK          (MSK(1) << ICTA_INTRAW_PCIA_SHF)
#define ICTA_INTRAW_PCIA_SET          ICTA_INTRAW_PCIA_MSK

/* bit 7: NMI */
#define ICTA_INTRAW_NMI_SHF           7
#define ICTA_INTRAW_NMI_MSK           (MSK(1) << ICTA_INTRAW_NMI_SHF)
#define ICTA_INTRAW_NMI_SET           ICTA_INTRAW_NMI_MSK

/* bit 6: CORELO */
#define ICTA_INTRAW_CORELO_SHF        6
#define ICTA_INTRAW_CORELO_MSK        (MSK(1) << ICTA_INTRAW_CORELO_SHF)
#define ICTA_INTRAW_CORELO_SET        ICTA_INTRAW_CORELO_MSK

/* bit 5: COREHI */
#define ICTA_INTRAW_COREHI_SHF        5
#define ICTA_INTRAW_COREHI_MSK        (MSK(1) << ICTA_INTRAW_COREHI_SHF)
#define ICTA_INTRAW_COREHI_SET        ICTA_INTRAW_COREHI_MSK

/* bit 4: RTC */
#define ICTA_INTRAW_RTC_SHF           4
#define ICTA_INTRAW_RTC_MSK           (MSK(1) << ICTA_INTRAW_RTC_SHF)
#define ICTA_INTRAW_RTC_SET           ICTA_INTRAW_RTC_MSK

/* bit 2: TIM1 */
#define ICTA_INTRAW_TIM1_SHF          2
#define ICTA_INTRAW_TIM1_MSK          (MSK(1) << ICTA_INTRAW_TIM1_SHF)
#define ICTA_INTRAW_TIM1_SET          ICTA_INTRAW_TIM1_MSK

/* bit 1: TIM0 */
#define ICTA_INTRAW_TIM0_SHF          1
#define ICTA_INTRAW_TIM0_MSK          (MSK(1) << ICTA_INTRAW_TIM0_SHF)
#define ICTA_INTRAW_TIM0_SET          ICTA_INTRAW_TIM0_MSK

/* bit 0: SER */
#define ICTA_INTRAW_SER_SHF           0
#define ICTA_INTRAW_SER_MSK           (MSK(1) << ICTA_INTRAW_SER_SHF)
#define ICTA_INTRAW_SER_SET           ICTA_INTRAW_SER_MSK



/******** reg: INTSETEN ********/
#define ICTA_INTSETEN_OFS             0x08 /* SET enable int ("1") per bit    */

/* bit 19: PCISERRN */
#define ICTA_INTSETEN_PCISERRN_SHF    19
#define ICTA_INTSETEN_PCISERRN_MSK    (MSK(1) << ICTA_INTSETEN_PCISERRN_SHF)
#define ICTA_INTSETEN_PCISERRN_SET    ICTA_INTSETEN_PCISERRN_MSK

/* bit 18: CONINTDN */
#define ICTA_INTSETEN_CONINTDN_SHF    18
#define ICTA_INTSETEN_CONINTDN_MSK    (MSK(1) << ICTA_INTSETEN_CONINTDN_SHF)
#define ICTA_INTSETEN_CONINTDN_SET    ICTA_INTSETEN_CONINTDN_MSK

/* bit 17: CONINTCN */
#define ICTA_INTSETEN_CONINTCN_SHF    17
#define ICTA_INTSETEN_CONINTCN_MSK    (MSK(1) << ICTA_INTSETEN_CONINTCN_SHF)
#define ICTA_INTSETEN_CONINTCN_SET    ICTA_INTSETEN_CONINTCN_MSK

/* bit 16: CONINTBN */
#define ICTA_INTSETEN_CONINTBN_SHF    16
#define ICTA_INTSETEN_CONINTBN_MSK    (MSK(1) << ICTA_INTSETEN_CONINTBN_SHF)
#define ICTA_INTSETEN_CONINTBN_SET    ICTA_INTSETEN_CONINTBN_MSK

/* bit 15: CONINTAN */
#define ICTA_INTSETEN_CONINTAN_SHF    15
#define ICTA_INTSETEN_CONINTAN_MSK    (MSK(1) << ICTA_INTSETEN_CONINTAN_SHF)
#define ICTA_INTSETEN_CONINTAN_SET    ICTA_INTSETEN_CONINTAN_MSK

/* bit 14: ATXOKN */
#define ICTA_INTSETEN_ATXOKN_SHF      14
#define ICTA_INTSETEN_ATXOKN_MSK      (MSK(1) << ICTA_INTSETEN_ATXOKN_SHF)
#define ICTA_INTSETEN_ATXOKN_SET      ICTA_INTSETEN_ATXOKN_MSK

/* bit 13: DEG */
#define ICTA_INTSETEN_DEG_SHF         13
#define ICTA_INTSETEN_DEG_MSK         (MSK(1) << ICTA_INTSETEN_DEG_SHF)
#define ICTA_INTSETEN_DEG_SET         ICTA_INTSETEN_DEG_MSK

/* bit 12: ENUM */
#define ICTA_INTSETEN_ENUM_SHF        12
#define ICTA_INTSETEN_ENUM_MSK        (MSK(1) << ICTA_INTSETEN_ENUM_SHF)
#define ICTA_INTSETEN_ENUM_SET        ICTA_INTSETEN_ENUM_MSK

/* bit 11: PCID */
#define ICTA_INTSETEN_PCID_SHF        11
#define ICTA_INTSETEN_PCID_MSK        (MSK(1) << ICTA_INTSETEN_PCID_SHF)
#define ICTA_INTSETEN_PCID_SET        ICTA_INTSETEN_PCID_MSK

/* bit 10: PCIC */
#define ICTA_INTSETEN_PCIC_SHF        10
#define ICTA_INTSETEN_PCIC_MSK        (MSK(1) << ICTA_INTSETEN_PCIC_SHF)
#define ICTA_INTSETEN_PCIC_SET        ICTA_INTSETEN_PCIC_MSK

/* bit 9: PCIB */
#define ICTA_INTSETEN_PCIB_SHF        9
#define ICTA_INTSETEN_PCIB_MSK        (MSK(1) << ICTA_INTSETEN_PCIB_SHF)
#define ICTA_INTSETEN_PCIB_SET        ICTA_INTSETEN_PCIB_MSK

/* bit 8: PCIA */
#define ICTA_INTSETEN_PCIA_SHF        8
#define ICTA_INTSETEN_PCIA_MSK        (MSK(1) << ICTA_INTSETEN_PCIA_SHF)
#define ICTA_INTSETEN_PCIA_SET        ICTA_INTSETEN_PCIA_MSK

/* bit 7: NMI */
#define ICTA_INTSETEN_NMI_SHF         7
#define ICTA_INTSETEN_NMI_MSK         (MSK(1) << ICTA_INTSETEN_NMI_SHF)
#define ICTA_INTSETEN_NMI_SET         ICTA_INTSETEN_NMI_MSK

/* bit 6: CORELO */
#define ICTA_INTSETEN_CORELO_SHF      6
#define ICTA_INTSETEN_CORELO_MSK      (MSK(1) << ICTA_INTSETEN_CORELO_SHF)
#define ICTA_INTSETEN_CORELO_SET      ICTA_INTSETEN_CORELO_MSK

/* bit 5: COREHI */
#define ICTA_INTSETEN_COREHI_SHF      5
#define ICTA_INTSETEN_COREHI_MSK      (MSK(1) << ICTA_INTSETEN_COREHI_SHF)
#define ICTA_INTSETEN_COREHI_SET      ICTA_INTSETEN_COREHI_MSK

/* bit 4: RTC */
#define ICTA_INTSETEN_RTC_SHF         4
#define ICTA_INTSETEN_RTC_MSK         (MSK(1) << ICTA_INTSETEN_RTC_SHF)
#define ICTA_INTSETEN_RTC_SET         ICTA_INTSETEN_RTC_MSK

/* bit 2: TIM1 */
#define ICTA_INTSETEN_TIM1_SHF        2
#define ICTA_INTSETEN_TIM1_MSK        (MSK(1) << ICTA_INTSETEN_TIM1_SHF)
#define ICTA_INTSETEN_TIM1_SET        ICTA_INTSETEN_TIM1_MSK

/* bit 1: TIM0 */
#define ICTA_INTSETEN_TIM0_SHF        1
#define ICTA_INTSETEN_TIM0_MSK        (MSK(1) << ICTA_INTSETEN_TIM0_SHF)
#define ICTA_INTSETEN_TIM0_SET        ICTA_INTSETEN_TIM0_MSK

/* bit 0: SER */
#define ICTA_INTSETEN_SER_SHF         0
#define ICTA_INTSETEN_SER_MSK         (MSK(1) << ICTA_INTSETEN_SER_SHF)
#define ICTA_INTSETEN_SER_SET         ICTA_INTSETEN_SER_MSK



/******** reg: INTRSTEN ********/
#define ICTA_INTRSTEN_OFS             0x10 /* RESET enable int ("1") per bit  */

/* bit 19: PCISERRN */
#define ICTA_INTRSTEN_PCISERRN_SHF    19
#define ICTA_INTRSTEN_PCISERRN_MSK    (MSK(1) << ICTA_INTRSTEN_PCISERRN_SHF)
#define ICTA_INTRSTEN_PCISERRN_SET    ICTA_INTRSTEN_PCISERRN_MSK

/* bit 18: CONINTDN */
#define ICTA_INTRSTEN_CONINTDN_SHF    18
#define ICTA_INTRSTEN_CONINTDN_MSK    (MSK(1) << ICTA_INTRSTEN_CONINTDN_SHF)
#define ICTA_INTRSTEN_CONINTDN_SET    ICTA_INTRSTEN_CONINTDN_MSK

/* bit 17: CONINTCN */
#define ICTA_INTRSTEN_CONINTCN_SHF    17
#define ICTA_INTRSTEN_CONINTCN_MSK    (MSK(1) << ICTA_INTRSTEN_CONINTCN_SHF)
#define ICTA_INTRSTEN_CONINTCN_SET    ICTA_INTRSTEN_CONINTCN_MSK

/* bit 16: CONINTBN */
#define ICTA_INTRSTEN_CONINTBN_SHF    16
#define ICTA_INTRSTEN_CONINTBN_MSK    (MSK(1) << ICTA_INTRSTEN_CONINTBN_SHF)
#define ICTA_INTRSTEN_CONINTBN_SET    ICTA_INTRSTEN_CONINTBN_MSK

/* bit 15: CONINTAN */
#define ICTA_INTRSTEN_CONINTAN_SHF    15
#define ICTA_INTRSTEN_CONINTAN_MSK    (MSK(1) << ICTA_INTRSTEN_CONINTAN_SHF)
#define ICTA_INTRSTEN_CONINTAN_SET    ICTA_INTRSTEN_CONINTAN_MSK

/* bit 14: ATXOKN */
#define ICTA_INTRSTEN_ATXOKN_SHF      14
#define ICTA_INTRSTEN_ATXOKN_MSK      (MSK(1) << ICTA_INTRSTEN_ATXOKN_SHF)
#define ICTA_INTRSTEN_ATXOKN_SET      ICTA_INTRSTEN_ATXOKN_MSK

/* bit 13: DEG */
#define ICTA_INTRSTEN_DEG_SHF         13
#define ICTA_INTRSTEN_DEG_MSK         (MSK(1) << ICTA_INTRSTEN_DEG_SHF)
#define ICTA_INTRSTEN_DEG_SET         ICTA_INTRSTEN_DEG_MSK

/* bit 12: ENUM */
#define ICTA_INTRSTEN_ENUM_SHF        12
#define ICTA_INTRSTEN_ENUM_MSK        (MSK(1) << ICTA_INTRSTEN_ENUM_SHF)
#define ICTA_INTRSTEN_ENUM_SET        ICTA_INTRSTEN_ENUM_MSK

/* bit 11: PCID */
#define ICTA_INTRSTEN_PCID_SHF        11
#define ICTA_INTRSTEN_PCID_MSK        (MSK(1) << ICTA_INTRSTEN_PCID_SHF)
#define ICTA_INTRSTEN_PCID_SET        ICTA_INTRSTEN_PCID_MSK

/* bit 10: PCIC */
#define ICTA_INTRSTEN_PCIC_SHF        10
#define ICTA_INTRSTEN_PCIC_MSK        (MSK(1) << ICTA_INTRSTEN_PCIC_SHF)
#define ICTA_INTRSTEN_PCIC_SET        ICTA_INTRSTEN_PCIC_MSK

/* bit 9: PCIB */
#define ICTA_INTRSTEN_PCIB_SHF        9
#define ICTA_INTRSTEN_PCIB_MSK        (MSK(1) << ICTA_INTRSTEN_PCIB_SHF)
#define ICTA_INTRSTEN_PCIB_SET        ICTA_INTRSTEN_PCIB_MSK

/* bit 8: PCIA */
#define ICTA_INTRSTEN_PCIA_SHF        8
#define ICTA_INTRSTEN_PCIA_MSK        (MSK(1) << ICTA_INTRSTEN_PCIA_SHF)
#define ICTA_INTRSTEN_PCIA_SET        ICTA_INTRSTEN_PCIA_MSK

/* bit 7: NMI */
#define ICTA_INTRSTEN_NMI_SHF         7
#define ICTA_INTRSTEN_NMI_MSK         (MSK(1) << ICTA_INTRSTEN_NMI_SHF)
#define ICTA_INTRSTEN_NMI_SET         ICTA_INTRSTEN_NMI_MSK

/* bit 6: CORELO */
#define ICTA_INTRSTEN_CORELO_SHF      6
#define ICTA_INTRSTEN_CORELO_MSK      (MSK(1) << ICTA_INTRSTEN_CORELO_SHF)
#define ICTA_INTRSTEN_CORELO_SET      ICTA_INTRSTEN_CORELO_MSK

/* bit 5: COREHI */
#define ICTA_INTRSTEN_COREHI_SHF      5
#define ICTA_INTRSTEN_COREHI_MSK      (MSK(1) << ICTA_INTRSTEN_COREHI_SHF)
#define ICTA_INTRSTEN_COREHI_SET      ICTA_INTRSTEN_COREHI_MSK

/* bit 4: RTC */
#define ICTA_INTRSTEN_RTC_SHF         4
#define ICTA_INTRSTEN_RTC_MSK         (MSK(1) << ICTA_INTRSTEN_RTC_SHF)
#define ICTA_INTRSTEN_RTC_SET         ICTA_INTRSTEN_RTC_MSK

/* bit 2: TIM1 */
#define ICTA_INTRSTEN_TIM1_SHF        2
#define ICTA_INTRSTEN_TIM1_MSK        (MSK(1) << ICTA_INTRSTEN_TIM1_SHF)
#define ICTA_INTRSTEN_TIM1_SET        ICTA_INTRSTEN_TIM1_MSK

/* bit 1: TIM0 */
#define ICTA_INTRSTEN_TIM0_SHF        1
#define ICTA_INTRSTEN_TIM0_MSK        (MSK(1) << ICTA_INTRSTEN_TIM0_SHF)
#define ICTA_INTRSTEN_TIM0_SET        ICTA_INTRSTEN_TIM0_MSK

/* bit 0: SER */
#define ICTA_INTRSTEN_SER_SHF         0
#define ICTA_INTRSTEN_SER_MSK         (MSK(1) << ICTA_INTRSTEN_SER_SHF)
#define ICTA_INTRSTEN_SER_SET         ICTA_INTRSTEN_SER_MSK



/******** reg: INTENABLE ********/
#define ICTA_INTENABLE_OFS            0x18 /* INT enable mask status          */

/* bit 19: PCISERRN */
#define ICTA_INTENABLE_PCISERRN_SHF   19
#define ICTA_INTENABLE_PCISERRN_MSK   (MSK(1) << ICTA_INTENABLE_PCISERRN_SHF)
#define ICTA_INTENABLE_PCISERRN_SET   ICTA_INTENABLE_PCISERRN_MSK

/* bit 18: CONINTDN */
#define ICTA_INTENABLE_CONINTDN_SHF   18
#define ICTA_INTENABLE_CONINTDN_MSK   (MSK(1) << ICTA_INTENABLE_CONINTDN_SHF)
#define ICTA_INTENABLE_CONINTDN_SET   ICTA_INTENABLE_CONINTDN_MSK

/* bit 17: CONINTCN */
#define ICTA_INTENABLE_CONINTCN_SHF   17
#define ICTA_INTENABLE_CONINTCN_MSK   (MSK(1) << ICTA_INTENABLE_CONINTCN_SHF)
#define ICTA_INTENABLE_CONINTCN_SET   ICTA_INTENABLE_CONINTCN_MSK

/* bit 16: CONINTBN */
#define ICTA_INTENABLE_CONINTBN_SHF   16
#define ICTA_INTENABLE_CONINTBN_MSK   (MSK(1) << ICTA_INTENABLE_CONINTBN_SHF)
#define ICTA_INTENABLE_CONINTBN_SET   ICTA_INTENABLE_CONINTBN_MSK

/* bit 15: CONINTAN */
#define ICTA_INTENABLE_CONINTAN_SHF   15
#define ICTA_INTENABLE_CONINTAN_MSK   (MSK(1) << ICTA_INTENABLE_CONINTAN_SHF)
#define ICTA_INTENABLE_CONINTAN_SET   ICTA_INTENABLE_CONINTAN_MSK

/* bit 14: ATXOKN */
#define ICTA_INTENABLE_ATXOKN_SHF     14
#define ICTA_INTENABLE_ATXOKN_MSK     (MSK(1) << ICTA_INTENABLE_ATXOKN_SHF)
#define ICTA_INTENABLE_ATXOKN_SET     ICTA_INTENABLE_ATXOKN_MSK

/* bit 13: DEG */
#define ICTA_INTENABLE_DEG_SHF        13
#define ICTA_INTENABLE_DEG_MSK        (MSK(1) << ICTA_INTENABLE_DEG_SHF)
#define ICTA_INTENABLE_DEG_SET        ICTA_INTENABLE_DEG_MSK

/* bit 12: ENUM */
#define ICTA_INTENABLE_ENUM_SHF       12
#define ICTA_INTENABLE_ENUM_MSK       (MSK(1) << ICTA_INTENABLE_ENUM_SHF)
#define ICTA_INTENABLE_ENUM_SET       ICTA_INTENABLE_ENUM_MSK

/* bit 11: PCID */
#define ICTA_INTENABLE_PCID_SHF       11
#define ICTA_INTENABLE_PCID_MSK       (MSK(1) << ICTA_INTENABLE_PCID_SHF)
#define ICTA_INTENABLE_PCID_SET       ICTA_INTENABLE_PCID_MSK

/* bit 10: PCIC */
#define ICTA_INTENABLE_PCIC_SHF       10
#define ICTA_INTENABLE_PCIC_MSK       (MSK(1) << ICTA_INTENABLE_PCIC_SHF)
#define ICTA_INTENABLE_PCIC_SET       ICTA_INTENABLE_PCIC_MSK

/* bit 9: PCIB */
#define ICTA_INTENABLE_PCIB_SHF       9
#define ICTA_INTENABLE_PCIB_MSK       (MSK(1) << ICTA_INTENABLE_PCIB_SHF)
#define ICTA_INTENABLE_PCIB_SET       ICTA_INTENABLE_PCIB_MSK

/* bit 8: PCIA */
#define ICTA_INTENABLE_PCIA_SHF       8
#define ICTA_INTENABLE_PCIA_MSK       (MSK(1) << ICTA_INTENABLE_PCIA_SHF)
#define ICTA_INTENABLE_PCIA_SET       ICTA_INTENABLE_PCIA_MSK

/* bit 7: NMI */
#define ICTA_INTENABLE_NMI_SHF        7
#define ICTA_INTENABLE_NMI_MSK        (MSK(1) << ICTA_INTENABLE_NMI_SHF)
#define ICTA_INTENABLE_NMI_SET        ICTA_INTENABLE_NMI_MSK

/* bit 6: CORELO */
#define ICTA_INTENABLE_CORELO_SHF     6
#define ICTA_INTENABLE_CORELO_MSK     (MSK(1) << ICTA_INTENABLE_CORELO_SHF)
#define ICTA_INTENABLE_CORELO_SET     ICTA_INTENABLE_CORELO_MSK

/* bit 5: COREHI */
#define ICTA_INTENABLE_COREHI_SHF     5
#define ICTA_INTENABLE_COREHI_MSK     (MSK(1) << ICTA_INTENABLE_COREHI_SHF)
#define ICTA_INTENABLE_COREHI_SET     ICTA_INTENABLE_COREHI_MSK

/* bit 4: RTC */
#define ICTA_INTENABLE_RTC_SHF        4
#define ICTA_INTENABLE_RTC_MSK        (MSK(1) << ICTA_INTENABLE_RTC_SHF)
#define ICTA_INTENABLE_RTC_SET        ICTA_INTENABLE_RTC_MSK

/* bit 2: TIM1 */
#define ICTA_INTENABLE_TIM1_SHF       2
#define ICTA_INTENABLE_TIM1_MSK       (MSK(1) << ICTA_INTENABLE_TIM1_SHF)
#define ICTA_INTENABLE_TIM1_SET       ICTA_INTENABLE_TIM1_MSK

/* bit 1: TIM0 */
#define ICTA_INTENABLE_TIM0_SHF       1
#define ICTA_INTENABLE_TIM0_MSK       (MSK(1) << ICTA_INTENABLE_TIM0_SHF)
#define ICTA_INTENABLE_TIM0_SET       ICTA_INTENABLE_TIM0_MSK

/* bit 0: SER */
#define ICTA_INTENABLE_SER_SHF        0
#define ICTA_INTENABLE_SER_MSK        (MSK(1) << ICTA_INTENABLE_SER_SHF)
#define ICTA_INTENABLE_SER_SET        ICTA_INTENABLE_SER_MSK



/******** reg: INTSTATUS ********/
#define ICTA_INTSTATUS_OFS            0x20 /* value = INTRAW & INTENABLE      */

/* bit 19: PCISERRN */
#define ICTA_INTSTATUS_PCISERRN_SHF   19
#define ICTA_INTSTATUS_PCISERRN_MSK   (MSK(1) << ICTA_INTSTATUS_PCISERRN_SHF)
#define ICTA_INTSTATUS_PCISERRN_SET   ICTA_INTSTATUS_PCISERRN_MSK

/* bit 18: CONINTDN */
#define ICTA_INTSTATUS_CONINTDN_SHF   18
#define ICTA_INTSTATUS_CONINTDN_MSK   (MSK(1) << ICTA_INTSTATUS_CONINTDN_SHF)
#define ICTA_INTSTATUS_CONINTDN_SET   ICTA_INTSTATUS_CONINTDN_MSK

/* bit 17: CONINTCN */
#define ICTA_INTSTATUS_CONINTCN_SHF   17
#define ICTA_INTSTATUS_CONINTCN_MSK   (MSK(1) << ICTA_INTSTATUS_CONINTCN_SHF)
#define ICTA_INTSTATUS_CONINTCN_SET   ICTA_INTSTATUS_CONINTCN_MSK

/* bit 16: CONINTBN */
#define ICTA_INTSTATUS_CONINTBN_SHF   16
#define ICTA_INTSTATUS_CONINTBN_MSK   (MSK(1) << ICTA_INTSTATUS_CONINTBN_SHF)
#define ICTA_INTSTATUS_CONINTBN_SET   ICTA_INTSTATUS_CONINTBN_MSK

/* bit 15: CONINTAN */
#define ICTA_INTSTATUS_CONINTAN_SHF   15
#define ICTA_INTSTATUS_CONINTAN_MSK   (MSK(1) << ICTA_INTSTATUS_CONINTAN_SHF)
#define ICTA_INTSTATUS_CONINTAN_SET   ICTA_INTSTATUS_CONINTAN_MSK

/* bit 14: ATXOKN */
#define ICTA_INTSTATUS_ATXOKN_SHF     14
#define ICTA_INTSTATUS_ATXOKN_MSK     (MSK(1) << ICTA_INTSTATUS_ATXOKN_SHF)
#define ICTA_INTSTATUS_ATXOKN_SET     ICTA_INTSTATUS_ATXOKN_MSK

/* bit 13: DEG */
#define ICTA_INTSTATUS_DEG_SHF        13
#define ICTA_INTSTATUS_DEG_MSK        (MSK(1) << ICTA_INTSTATUS_DEG_SHF)
#define ICTA_INTSTATUS_DEG_SET        ICTA_INTSTATUS_DEG_MSK

/* bit 12: ENUM */
#define ICTA_INTSTATUS_ENUM_SHF       12
#define ICTA_INTSTATUS_ENUM_MSK       (MSK(1) << ICTA_INTSTATUS_ENUM_SHF)
#define ICTA_INTSTATUS_ENUM_SET       ICTA_INTSTATUS_ENUM_MSK

/* bit 11: PCID */
#define ICTA_INTSTATUS_PCID_SHF       11
#define ICTA_INTSTATUS_PCID_MSK       (MSK(1) << ICTA_INTSTATUS_PCID_SHF)
#define ICTA_INTSTATUS_PCID_SET       ICTA_INTSTATUS_PCID_MSK

/* bit 10: PCIC */
#define ICTA_INTSTATUS_PCIC_SHF       10
#define ICTA_INTSTATUS_PCIC_MSK       (MSK(1) << ICTA_INTSTATUS_PCIC_SHF)
#define ICTA_INTSTATUS_PCIC_SET       ICTA_INTSTATUS_PCIC_MSK

/* bit 9: PCIB */
#define ICTA_INTSTATUS_PCIB_SHF       9
#define ICTA_INTSTATUS_PCIB_MSK       (MSK(1) << ICTA_INTSTATUS_PCIB_SHF)
#define ICTA_INTSTATUS_PCIB_SET       ICTA_INTSTATUS_PCIB_MSK

/* bit 8: PCIA */
#define ICTA_INTSTATUS_PCIA_SHF       8
#define ICTA_INTSTATUS_PCIA_MSK       (MSK(1) << ICTA_INTSTATUS_PCIA_SHF)
#define ICTA_INTSTATUS_PCIA_SET       ICTA_INTSTATUS_PCIA_MSK

/* bit 7: NMI */
#define ICTA_INTSTATUS_NMI_SHF        7
#define ICTA_INTSTATUS_NMI_MSK        (MSK(1) << ICTA_INTSTATUS_NMI_SHF)
#define ICTA_INTSTATUS_NMI_SET        ICTA_INTSTATUS_NMI_MSK

/* bit 6: CORELO */
#define ICTA_INTSTATUS_CORELO_SHF     6
#define ICTA_INTSTATUS_CORELO_MSK     (MSK(1) << ICTA_INTSTATUS_CORELO_SHF)
#define ICTA_INTSTATUS_CORELO_SET     ICTA_INTSTATUS_CORELO_MSK

/* bit 5: COREHI */
#define ICTA_INTSTATUS_COREHI_SHF     5
#define ICTA_INTSTATUS_COREHI_MSK     (MSK(1) << ICTA_INTSTATUS_COREHI_SHF)
#define ICTA_INTSTATUS_COREHI_SET     ICTA_INTSTATUS_COREHI_MSK

/* bit 4: RTC */
#define ICTA_INTSTATUS_RTC_SHF        4
#define ICTA_INTSTATUS_RTC_MSK        (MSK(1) << ICTA_INTSTATUS_RTC_SHF)
#define ICTA_INTSTATUS_RTC_SET        ICTA_INTSTATUS_RTC_MSK

/* bit 2: TIM1 */
#define ICTA_INTSTATUS_TIM1_SHF       2
#define ICTA_INTSTATUS_TIM1_MSK       (MSK(1) << ICTA_INTSTATUS_TIM1_SHF)
#define ICTA_INTSTATUS_TIM1_SET       ICTA_INTSTATUS_TIM1_MSK

/* bit 1: TIM0 */
#define ICTA_INTSTATUS_TIM0_SHF       1
#define ICTA_INTSTATUS_TIM0_MSK       (MSK(1) << ICTA_INTSTATUS_TIM0_SHF)
#define ICTA_INTSTATUS_TIM0_SET       ICTA_INTSTATUS_TIM0_MSK

/* bit 0: SER */
#define ICTA_INTSTATUS_SER_SHF        0
#define ICTA_INTSTATUS_SER_MSK        (MSK(1) << ICTA_INTSTATUS_SER_SHF)
#define ICTA_INTSTATUS_SER_SET        ICTA_INTSTATUS_SER_MSK



/************************************************************************
 *  RTC device: Relative Register Addresses
 ************************************************************************/

#define RTC_SEC_OFS                   0x00    /* seconds register        */
#define RTC_SECALARM_OFS              0x01    /* seconds alarm register  */
#define RTC_MIN_OFS                   0x02    /* minutes register        */
#define RTC_MINALARM_OFS              0x03    /* minutes alarm register  */
#define RTC_HOUR_OFS                  0x04    /* hours register          */
#define RTC_HOURALARM_OFS             0x05    /* hours alarm register    */
#define RTC_DAYOFWEEK_OFS             0x06    /* day of week register    */
#define RTC_DAYOFMONTH_OFS            0x07    /* day of month register   */
#define RTC_MONTH_OFS                 0x08    /* month register          */
#define RTC_YEAR_OFS                  0x09    /* year register           */
#define RTC_REGA_OFS                  0x0A    /* register A              */
#define RTC_REGB_OFS                  0x0B    /* register B              */
#define RTC_REGC_OFS                  0x0C    /* register C              */
#define RTC_REGD_OFS                  0x0D    /* register D              */

/* RTC Register A bit map definitions */
#define RTC_REGA_UIP_SHF              7
#define RTC_REGA_UIP_MSK              (MSK(1) << RTC_REGA_UIP_SHF)
#define RTC_REGA_UIP_BIT              RTC_REGA_UIP_MSK

#define RTC_REGA_DCS_SHF              4
#define RTC_REGA_DCS_MSK              (MSK(3) << RTC_REGA_DCS_SHF)
#define RTC_REGA_DCS_NORMAL           2       /* Normal operation        */
#define RTC_REGA_DCS_RESET            6       /* Divider reset           */

#define RTC_REGA_RSB_SHF              0
#define RTC_REGA_RSB_MSK              (MSK(4) << RTC_REGA_RSB_SHF)
#define RTC_REGA_RSB_NONE             0       /* No periodic interrupt   */
#define RTC_REGA_RSB_3_9              1       /* 3.90625 ms              */
#define RTC_REGA_RSB_7_8              2       /* 7.8125 ms               */
#define RTC_REGA_RSB_122              3       /* 122.070 us              */
#define RTC_REGA_RSB_244              4       /* 244.141 us              */
#define RTC_REGA_RSB_488              5       /* 488.281 us              */
#define RTC_REGA_RSB_976              6       /* 976.5625 us             */
#define RTC_REGA_RSB_1_9              7       /* 1.953125 ms             */
#define RTC_REGA_RSB_3_9A             8       /* 3.90625 ms              */
#define RTC_REGA_RSB_7_8A             9       /* 7.8125 ms               */
#define RTC_REGA_RSB_15               10      /* 15.625 ms               */
#define RTC_REGA_RSB_31               11      /* 31.25 ms                */
#define RTC_REGA_RSB_62               12      /* 62.5 ms                 */
#define RTC_REGA_RSB_125              13      /* 125 ms                  */
#define RTC_REGA_RSB_250              14      /* 250 ms                  */
#define RTC_REGA_RSB_500              15      /* 500 ms                  */


/* RTC Register B bit map definitions */
#define RTC_REGB_SET_SHF              7 
#define RTC_REGB_SET_MSK              (MSK(1) << RTC_REGB_SET_SHF)
#define RTC_REGB_SET_BIT              RTC_REGB_SET_MSK

#define RTC_REGB_PIE_SHF              6
#define RTC_REGB_PIE_MSK              (MSK(1) << RTC_REGB_PIE_SHF)
#define RTC_REGB_PIE_BIT              RTC_REGB_PIE_MSK

#define RTC_REGB_AIE_SHF              5
#define RTC_REGB_AIE_MSK              (MSK(1) << RTC_REGB_AIE_SHF)
#define RTC_REGB_AIE_BIT              RTC_REGB_AIE_MSK

#define RTC_REGB_UIE_SHF              4
#define RTC_REGB_UIE_MSK              (MSK(1) << RTC_REGB_UIE_SHF)
#define RTC_REGB_UIE_BIT              RTC_REGB_UIE_MSK

#define RTC_REGB_SQWE_SHF             3
#define RTC_REGB_SQWE_MSK             (MSK(1) << RTC_REGB_SQWE_SHF)
#define RTC_REGB_SQWE_BIT             RTC_REGB_SQWE_MSK

#define RTC_REGB_DM_SHF               2
#define RTC_REGB_DM_MSK               (MSK(1) << RTC_REGB_DM_SHF)
#define RTC_REGB_DM_BIT               RTC_REGB_DM_MSK
#define RTC_REGB_DM_BIN               1
#define RTC_REGB_DM_BCD               0

#define RTC_REGB_HF_SHF               1
#define RTC_REGB_HF_MSK               (MSK(1) << RTC_REGB_HF_SHF)
#define RTC_REGB_HF_BIT               RTC_REGB_HF_MSK
#define RTC_REGB_HF_24                1
#define RTC_REGB_HF_12                0

#define RTC_REGB_DSE_SHF              0    
#define RTC_REGB_DSE_MSK              (MSK(1) << RTC_REGB_DSE_SHF)
#define RTC_REGB_DSE_BIT              RTC_REGB_DSE_MSK


/* RTC Register C bit map definitions */
#define RTC_REGC_IRQF_SHF             7
#define RTC_REGC_IRQF_MSK             (MSK(1) << RTC_REGC_IRQF_SHF)
#define RTC_REGC_IRQF_BIT             RTC_REGC_IRQF_MSK

#define RTC_REGC_PF_SHF               6
#define RTC_REGC_PF_MSK               (MSK(1) << RTC_REGC_PF_SHF)
#define RTC_REGC_PF_BIT               RTC_REGC_PF_MSK

#define RTC_REGC_AF_SHF               5
#define RTC_REGC_AF_MSK               (MSK(1) << RTC_REGC_AF_SHF)
#define RTC_REGC_AF_BIT               RTC_REGC_AF_MSK

#define RTC_REGC_UF_SHF               4
#define RTC_REGC_UF_MSK               (MSK(1) << RTC_REGC_UF_SHF)
#define RTC_REGC_UF_BIT               RTC_REGC_UF_MSK


/* RTC Register D bit map definitions */
#define RTC_REGD_VRT_SHF              7
#define RTC_REGD_VRT_MSK              (MSK(1) << RTC_REGD_VRT_SHF)
#define RTC_REGD_VRT_BIT              RTC_REGD_VRT_MSK

#define RTC_REGD_DA_SHF               0
#define RTC_REGD_DA_MSK               (MSK(6) << RTC_REGD_DA_SHF)



/************************************************************************
 *  C code I/O Register access definitions
*************************************************************************/

#ifndef _ASSEMBLER_

#include "sysdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* REVISION */
#define aREVISION       (KSEG1BASE+ATLAS_REVISION)
#define rREVISION       REG32(aREVISION)

/* Green LED */
#define rLEDGREEN       REG32(KSEG1(ATLAS_LEDGREEN))

/* LED BAR */
#define rLEDBAR         REG32(KSEG1(ATLAS_LEDBAR))

/* ASCII LEDs */
#define rASCIIWORD      REG32(KSEG1(ATLAS_ASCIIWORD))
#define rASCIIPOS0      REG32(KSEG1(ATLAS_ASCIIPOS0))
#define rASCIIPOS1      REG32(KSEG1(ATLAS_ASCIIPOS1))
#define rASCIIPOS2      REG32(KSEG1(ATLAS_ASCIIPOS2))
#define rASCIIPOS3      REG32(KSEG1(ATLAS_ASCIIPOS3))
#define rASCIIPOS4      REG32(KSEG1(ATLAS_ASCIIPOS4))
#define rASCIIPOS5      REG32(KSEG1(ATLAS_ASCIIPOS5))
#define rASCIIPOS6      REG32(KSEG1(ATLAS_ASCIIPOS6))
#define rASCIIPOS7      REG32(KSEG1(ATLAS_ASCIIPOS7))

/* Interrupt Controller Registers. BASE = 0x1F00.0000 */
/*#define rINTRAW         REG32(KSEG1(ATLAS_ICTA_BASE+0x00))
#define rINTSETEN       REG32(KSEG1(ATLAS_ICTA_BASE+0x08))
#define rINTRSTEN       REG32(KSEG1(ATLAS_ICTA_BASE+0x10))
#define rINTENABLE      REG32(KSEG1(ATLAS_ICTA_BASE+0x18))
#define rINTSTATUS      REG32(KSEG1(ATLAS_ICTA_BASE+0x20))*/

#define rPIC_MASTER_CMD	REG8(KSEG1(ATLAS_ICTA_BASE+0x00))
#define rPIC_SLAVE_CMD	REG8(KSEG1(ATLAS_ICTA_BASE+0x80))
#define rPIC_MASTER_ISR REG8(KSEG1(ATLAS_ICTA_BASE+0x00))
#define rPIC_MASTER_IMR	REG8(KSEG1(ATLAS_ICTA_BASE+0x01))

/* UART TI16C550 */
/*#define r16550RXTX      REG32(KSEG1(ATLAS_TI16C550_BASE+0x00))
#define r16550INTEN     REG32(KSEG1(ATLAS_TI16C550_BASE+0x08))
#define r16550IIFIFO    REG32(KSEG1(ATLAS_TI16C550_BASE+0x10))
#define r16550LCTRL     REG32(KSEG1(ATLAS_TI16C550_BASE+0x18))
#define r16550MCTRL     REG32(KSEG1(ATLAS_TI16C550_BASE+0x20))
#define r16550LSTAT     REG32(KSEG1(ATLAS_TI16C550_BASE+0x28))
#define r16550MSTAT     REG32(KSEG1(ATLAS_TI16C550_BASE+0x30))
#define r16550SCRATCH   REG32(KSEG1(ATLAS_TI16C550_BASE+0x38))
#define r16550DLL       REG32(KSEG1(ATLAS_TI16C550_BASE+0x00))
#define r16550DLM       REG32(KSEG1(ATLAS_TI16C550_BASE+0x08))*/

#define r16550RXTX      REG8(KSEG1(ATLAS_TI16C550_BASE+0x00))
#define r16550INTEN     REG8(KSEG1(ATLAS_TI16C550_BASE+0x01))
#define r16550IIFIFO    REG8(KSEG1(ATLAS_TI16C550_BASE+0x2))
#define r16550LCTRL     REG8(KSEG1(ATLAS_TI16C550_BASE+0x3))
#define r16550MCTRL     REG8(KSEG1(ATLAS_TI16C550_BASE+0x4))
#define r16550LSTAT     REG8(KSEG1(ATLAS_TI16C550_BASE+0x5))
#define r16550MSTAT     REG8(KSEG1(ATLAS_TI16C550_BASE+0x6))
#define r16550SCRATCH   REG8(KSEG1(ATLAS_TI16C550_BASE+0x7))
#define r16550DLL       REG8(KSEG1(ATLAS_TI16C550_BASE+0x00))
#define r16550DLM       REG8(KSEG1(ATLAS_TI16C550_BASE+0x01))


/* 24bit TIMER */
/*#define rTM0CNT         REG32(KSEG1(ATLAS_TMRA_BASE+0x00))
#define rTM0CMP         REG32(KSEG1(ATLAS_TMRA_BASE+0x08))
#define rTMINTACK       REG32(KSEG1(ATLAS_TMRA_BASE+0x20))*/

#define rPIT_CMD		REG8(KSEG1(ATLAS_TMRA_BASE+0x3))
#define rPIT_CNT0		REG8(KSEG1(ATLAS_TMRA_BASE+0x00))


/* RTC */
#define rRTCADR         REG32(KSEG1(ATLAS_RTCADR))
#define rRTCDAT         REG32(KSEG1(ATLAS_RTCDAT))



/* Bit defintions */

/* rINTSETEN */
#define BIT_ICINT_SER                 (1<<0)  /* bit 0: SER */
#define BIT_ICINT_TIM0                (1<<1)  /* bit 1: TIM0 */
#define BIT_ICINT_TIM1                (1<<2)  /* bit 2: TIM1 */
#define BIT_ICINT_RTC                 (1<<4)  /* bit 4: RTC */
#define BIT_ICINT_COREHI              (1<<5)  /* bit 5: COREHI */
#define BIT_ICINT_CORELO              (1<<6)  /* bit 6: CORELO */
#define BIT_ICINT_NMI                 (1<<7)  /* bit 7: NMI */
#define BIT_ICINT_PCIA                (1<<8)  /* bit 8: PCIA */
#define BIT_ICINT_PCIB                (1<<9)  /* bit 9: PCIB */
#define BIT_ICINT_PCIC                (1<<10) /* bit 10: PCIC */
#define BIT_ICINT_PCID                (1<<11) /* bit 11: PCID */
#define BIT_ICINT_ENUM                (1<<12) /* bit 12: ENUM */
#define BIT_ICINT_DEG                 (1<<13) /* bit 13: DEG */
#define BIT_ICINT_ATXOKN              (1<<14) /* bit 14: ATXOKN */
#define BIT_ICINT_CONINTAN            (1<<15) /* bit 15: CONINTAN */
#define BIT_ICINT_CONINTBN            (1<<16) /* bit 16: CONINTBN */
#define BIT_ICINT_CONINTCN            (1<<17) /* bit 17: CONINTCN */
#define BIT_ICINT_CONINTDN            (1<<18) /* bit 18: CONINTDN */
#define BIT_ICINT_PCISERRN            (1<<19) /* bit 19: PCISERRN */



/* r16550INTEN */
#define BIT_16550RXINT                (1<<0)
#define BIT_16550TXINT                (1<<1)
#define BIT_16550LSINT                (1<<2)
#define BIT_16550MSINT                (1<<3)

#ifdef __cplusplus
}
#endif

#endif /* _ASSEMBLER_ */



/************************************************************************
 *  Assembly & C code soft vectors definition
*************************************************************************/

/* Core Exception handler vectors */
#define _ESR_STARTADDRESS 0x00000500+KSEG0BASE

#define aESR_INT            (_ESR_STARTADDRESS+0x8*EX_INT)
#define aESR_MOD            (_ESR_STARTADDRESS+0x8*EX_MOD)
#define aESR_TLBL           (_ESR_STARTADDRESS+0x8*EX_TLBL)
#define aESR_TLBS           (_ESR_STARTADDRESS+0x8*EX_TLBS)
#define aESR_ADEL           (_ESR_STARTADDRESS+0x8*EX_ADEL)
#define aESR_ADES           (_ESR_STARTADDRESS+0x8*EX_ADES)
#define aESR_IBE            (_ESR_STARTADDRESS+0x8*EX_IBE)
#define aESR_DBE            (_ESR_STARTADDRESS+0x8*EX_DBE)
#define aESR_SYS            (_ESR_STARTADDRESS+0x8*EX_SYS)
#define aESR_BP             (_ESR_STARTADDRESS+0x8*EX_BP)
#define aESR_RI             (_ESR_STARTADDRESS+0x8*EX_RI)
#define aESR_CPU            (_ESR_STARTADDRESS+0x8*EX_CPU)
#define aESR_OV             (_ESR_STARTADDRESS+0x8*EX_OV)
#define aESR_TR             (_ESR_STARTADDRESS+0x8*EX_TR)
#define aESR_FPE            (_ESR_STARTADDRESS+0x8*EX_FPE)
#define aESR_WATCH          (_ESR_STARTADDRESS+0x8*EX_WATCH)
#define aESR_MCHECK         (_ESR_STARTADDRESS+0x8*EX_MCHECK)

/* General Exception Vector */
#define aESR_EXCE           (aESR_MCHECK+0x8*1)

/* CPU Interrupt handler vectors */
#define _CPUISR_STARTADDRESS _ESR_STARTADDRESS+0x100

#define aCPUISR_SW0         (_CPUISR_STARTADDRESS+0x00)
#define aCPUISR_SW1         (_CPUISR_STARTADDRESS+0x08)
#define aCPUISR_HW0         (_CPUISR_STARTADDRESS+0x10)
#define aCPUISR_HW1         (_CPUISR_STARTADDRESS+0x18)
#define aCPUISR_HW2         (_CPUISR_STARTADDRESS+0x20)
#define aCPUISR_HW3         (_CPUISR_STARTADDRESS+0x28)
#define aCPUISR_HW4         (_CPUISR_STARTADDRESS+0x30)
#define aCPUISR_HW5         (_CPUISR_STARTADDRESS+0x38)

/* Interrupt Controller Interrupt handler vectors */
#define _ICISR_STARTADDRESS aCPUISR_HW5+0x8

#define aICISR_SER          (_ICISR_STARTADDRESS+0x00)
#define aICISR_TIM0         (_ICISR_STARTADDRESS+0x08)
#define aICISR_TIM1         (_ICISR_STARTADDRESS+0x10)
/* bit 3: reserved */
#define aICISR_RTC          (_ICISR_STARTADDRESS+0x20)
#define aICISR_COREHI       (_ICISR_STARTADDRESS+0x28)
#define aICISR_CORELO       (_ICISR_STARTADDRESS+0x30)
#define aICISR_NMI          (_ICISR_STARTADDRESS+0x38)
#define aICISR_PCIA         (_ICISR_STARTADDRESS+0x40)
#define aICISR_PCIB         (_ICISR_STARTADDRESS+0x48)
#define aICISR_PCIC         (_ICISR_STARTADDRESS+0x50)
#define aICISR_PCID         (_ICISR_STARTADDRESS+0x58)
#define aICISR_ENUM         (_ICISR_STARTADDRESS+0x60)
#define aICISR_DEG          (_ICISR_STARTADDRESS+0x68)
#define aICISR_ATXOKN       (_ICISR_STARTADDRESS+0x70)
#define aICISR_CONINTAN     (_ICISR_STARTADDRESS+0x78)
#define aICISR_CONINTBN     (_ICISR_STARTADDRESS+0x80)
#define aICISR_CONINTCN     (_ICISR_STARTADDRESS+0x88)
#define aICISR_CONINTDN     (_ICISR_STARTADDRESS+0x90)
#define aICISR_PCISERRN     (_ICISR_STARTADDRESS+0x98)

/* Interrupt Controller Interrupt handler vectors */
#define _SAAISR_STARTADDRESS aICISR_PCISERRN+0x8

#define aSAAISR_ETH         (_SAAISR_STARTADDRESS+0x00)
#define aSAAISR_UART        (_SAAISR_STARTADDRESS+0x08)



#ifndef _ASSEMBLER_

#ifdef __cplusplus
extern "C" {
#endif

/* Pointer definition for C code */
/* Core Exception handler vectors */
#define pESR_INT            (*(U32 *)aESR_INT)
#define pESR_MOD            (*(U32 *)aESR_MOD)
#define pESR_TLBL           (*(U32 *)aESR_TLBL)
#define pESR_TLBS           (*(U32 *)aESR_TLBS)
#define pESR_ADEL           (*(U32 *)aESR_ADEL)
#define pESR_ADES           (*(U32 *)aESR_ADES)
#define pESR_IBE            (*(U32 *)aESR_IBE)
#define pESR_DBE            (*(U32 *)aESR_DBE)
#define pESR_SYS            (*(U32 *)aESR_SYS)
#define pESR_BP             (*(U32 *)aESR_BP)
#define pESR_RI             (*(U32 *)aESR_RI)
#define pESR_CPU            (*(U32 *)aESR_CPU)
#define pESR_OV             (*(U32 *)aESR_OV)
#define pESR_TR             (*(U32 *)aESR_TR)
#define pESR_FPE            (*(U32 *)aESR_FPE)
#define pESR_WATCH          (*(U32 *)aESR_WATCH)
#define pESR_MCHECK         (*(U32 *)aESR_MCHECK)

/* General Exception Vector */
#define pESR_EXCE           (*(U32 *)aESR_EXCE)

/* CPU Interrupt handler vectors */
#define pCPUISR_SW0         (*(U32 *)aCPUISR_SW0)
#define pCPUISR_SW1         (*(U32 *)aCPUISR_SW1)
#define pCPUISR_HW0         (*(U32 *)aCPUISR_HW0)
#define pCPUISR_HW1         (*(U32 *)aCPUISR_HW1)
#define pCPUISR_HW2         (*(U32 *)aCPUISR_HW2)
#define pCPUISR_HW3         (*(U32 *)aCPUISR_HW3)
#define pCPUISR_HW4         (*(U32 *)aCPUISR_HW4)
#define pCPUISR_HW5         (*(U32 *)aCPUISR_HW5)

/* Interrupt Controller Interrupt handler vectors */
#define pICISR_SER          (*(U32 *)aICISR_SER)
#define pICISR_TIM0         (*(U32 *)aICISR_TIM0)
#define pICISR_TIM1         (*(U32 *)aICISR_TIM1)
/* bit 3: reserved */
#define pICISR_RTC          (*(U32 *)aICISR_RTC)
#define pICISR_COREHI       (*(U32 *)aICISR_COREHI)
#define pICISR_CORELO       (*(U32 *)aICISR_CORELO)
#define pICISR_NMI          (*(U32 *)aICISR_NMI)
#define pICISR_PCIA         (*(U32 *)aICISR_PCIA)
#define pICISR_PCIB         (*(U32 *)aICISR_PCIB)
#define pICISR_PCIC         (*(U32 *)aICISR_PCIC)
#define pICISR_PCID         (*(U32 *)aICISR_PCID)
#define pICISR_ENUM         (*(U32 *)aICISR_ENUM)
#define pICISR_DEG          (*(U32 *)aICISR_DEG)
#define pICISR_ATXOKN       (*(U32 *)aICISR_ATXOKN)
#define pICISR_CONINTAN     (*(U32 *)aICISR_CONINTAN)
#define pICISR_CONINTBN     (*(U32 *)aICISR_CONINTBN)
#define pICISR_CONINTCN     (*(U32 *)aICISR_CONINTCN)
#define pICISR_CONINTDN     (*(U32 *)aICISR_CONINTDN)
#define pICISR_PCISERRN     (*(U32 *)aICISR_PCISERRN)

#define pSAAISR_ETH         (*(U32 *)aSAAISR_ETH)
#define pSAAISR_UART        (*(U32 *)aSAAISR_UART)

#define sysoutbyte(addr, value)	(*((volatile U8*)(addr)) = (U8)value)
#define sysinbyte(addr)			(*((volatile U8*)(addr)))
#define MSB(x)	(((x) >> 8) & 0xff)	  /* most signif byte of 2-byte integer */
#define LSB(x)	((x) & 0xff)		  /* least signif byte of 2-byte integer*/
#define MSW(x) (((x) >> 16) & 0xffff) /* most signif word of 2-word integer */
#define LSW(x) ((x) & 0xffff) 		  /* least signif byte of 2-word integer*/

#define	CAST

#define PIT_REG_ADDR_INTERVAL 1

#define PIT_ADRS(base,reg)   (CAST (base+(reg*PIT_REG_ADDR_INTERVAL)))

/* register definitions */

#define PIT_CNT0(base)	PIT_ADRS(base,0x00)	/* Counter 0. */
#define PIT_CNT1(base)	PIT_ADRS(base,0x01)	/* Counter 1. */
#define PIT_CNT2(base)	PIT_ADRS(base,0x02)	/* Counter 2. */
#define PIT_CMD(base)	PIT_ADRS(base,0x03)	/* Control word. */

#define PIT_BASE_ADR (0xb4000040)
#define PIT_CLOCK (1193182)
#define PIT0_INT_LVL (0)

/* i8259A PIC registers */
#define PIC_CASCADE_IR		2
#define PIC_MASTER_CMD		(0xb4000020)
#define PIC_MASTER_IMR		(0xb4000021)
#define PIC_MASTER_ISR		PIC_MASTER_CMD
#define PIC_MASTER_POLL		PIC_MASTER_ISR
#define PIC_MASTER_OCW3		PIC_MASTER_ISR
#define PIC_SLAVE_CMD		(0xb40000a0)
#define PIC_SLAVE_IMR		(0xb40000a1)

#ifdef __cplusplus
}
#endif

#endif /* _ASSEMBLER_ */


/* ********************************************************************* */
/* Interface function definition */


/* ********************************************************************* */

#endif /* __ATLAS_H__ */
