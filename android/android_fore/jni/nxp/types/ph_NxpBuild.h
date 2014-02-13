/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Build System Definitions for Reader Library Framework.
* $Author: jayaprakash.desai $
* $Revision: 1.6 $
* $Date: Wed May 25 14:06:15 2011 $
*
* History:
*  MK: Generated 15. October 2009
*
*/

#ifndef PH_NXPBUILD_H
#define PH_NXPBUILD_H

/** \defgroup ph_NxpBuild NXP Build
* \brief Controls build behaviour of components.
* @{
*/

/* NXPBUILD_DELETE: included code lines should be always removed from code */

/* NXP Build defines                            */
/* use #define to include components            */
/* comment out #define to exclude components    */

/* DEBUG build mode */


/* BAL components */

#define NXPBUILD__PHBAL_REG_STUB                /**< Stub BAL definition */
//#define NXPBUILD__PHBAL_REG_SERIALWIN           /**< SerialWin BAL definition */
//#define NXPBUILD__PHBAL_REG_PCSCWIN             /**< PcscWin BAL definition */
//#define NXPBUILD__PHBAL_REG_RD70XUSBWIN         /**< Rd70X_UsbWin BAL definition */
//#define NXPBUILD__PHBAL_REG_RD710SAM            /**< Rd710Sam BAL definition */

#if defined (NXPBUILD__PHBAL_REG_STUB)          || \
    defined (NXPBUILD__PHBAL_REG_SERIALWIN)     || \
    defined (NXPBUILD__PHBAL_REG_PCSCWIN)       || \
    defined (NXPBUILD__PHBAL_REG_RD70XUSBWIN)   || \
    defined (NXPBUILD__PHBAL_REG_RD710SAM)
    #define NXPBUILD__PHBAL_REG                 /**< Generic BAL definition */
#endif

/* HAL components */

//#define NXPBUILD__PHHAL_HW_RC523                /**< Rc523 HAL definition */
//#define NXPBUILD__PHHAL_HW_SAMAV2               /**< SamAV2 HAL definition */
//#define NXPBUILD__PHHAL_HW_RD70X                /**< Rd70X HAL definition */
//#define NXPBUILD__PHHAL_HW_RC632                /**< Rc632 HAL definition */
//#define NXPBUILD__PHHAL_HW_CALLBACK             /**< Callback HAL definition */
#define NXPBUILD__PHHAL_HW_RC663                /**< Rc663 HAL definition */
//#define NXPBUILD__PHHAL_HW_RD710                /**< Rd710 HAL definition */

#if defined (NXPBUILD__PHHAL_HW_RC523)        || \
    defined (NXPBUILD__PHHAL_HW_SAMAV2)       || \
    defined (NXPBUILD__PHHAL_HW_RD70X)        || \
    defined (NXPBUILD__PHHAL_HW_CALLBACK)     || \
    defined (NXPBUILD__PHHAL_HW_RC663)        || \
    defined (NXPBUILD__PHHAL_HW_RC632)        || \
    defined (NXPBUILD__PHHAL_HW_RD710)
    #define NXPBUILD__PHHAL_HW                  /**< Generic HAL definition */
#endif

/* PAL ISO 14443-3A components */

#define NXPBUILD__PHPAL_I14443P3A_SW            /**< Software PAL ISO 14443-3A definition */
//#define NXPBUILD__PHPAL_I14443P3A_SAMAV2_X      /**< SamAV2_X PAL ISO 14443-3A definition */
//#define NXPBUILD__PHPAL_I14443P3A_RD70X         /**< Rd70X PAL ISO 14443-3A definition */
//#define NXPBUILD__PHPAL_I14443P3A_RD710         /**< Rd710 PAL ISO 14443-3A definition */

#if defined (NXPBUILD__PHPAL_I14443P3A_SW)          || \
    defined (NXPBUILD__PHPAL_I14443P3A_SAMAV2_X)    || \
    defined (NXPBUILD__PHPAL_I14443P3A_RD70X)       || \
    defined (NXPBUILD__PHPAL_I14443P3A_RD710)
    #define NXPBUILD__PHPAL_I14443P3A           /**< Generic PAL ISO 14443-3A definition */
#endif

/* PAL ISO 14443-3B components */

#define NXPBUILD__PHPAL_I14443P3B_SW            /**< Software PAL ISO 14443-3B definition */

#if defined (NXPBUILD__PHPAL_I14443P3B_SW)
    #define NXPBUILD__PHPAL_I14443P3B           /**< Generic PAL ISO 14443-3B definition */
#endif

/* PAL ISO 14443-4A components */

#define NXPBUILD__PHPAL_I14443P4A_SW            /**< Software PAL ISO 14443-4A definition */
//#define NXPBUILD__PHPAL_I14443P4A_SAMAV2_X      /**< SamAV2_X PAL ISO 14443-4A definition */
//#define NXPBUILD__PHPAL_I14443P4A_RD710         /**< Rd710 PAL ISO 14443-4A definition */
        
#if defined (NXPBUILD__PHPAL_I14443P4A_SW)       || \
    defined (NXPBUILD__PHPAL_I14443P4A_SAMAV2_X) || \
    defined (NXPBUILD__PHPAL_I14443P4A_RD710)
    #define NXPBUILD__PHPAL_I14443P4A           /**< Generic PAL ISO 14443-4A definition */
#endif

/* PAL ISO 14443-4 components */

#define NXPBUILD__PHPAL_I14443P4_SW             /**< Software PAL ISO 14443-4 definition */
//#define NXPBUILD__PHPAL_I14443P4_SAMAV2_X       /**< SamAV2_X PAL ISO 14443-4 definition */
//#define NXPBUILD__PHPAL_I14443P4_RD710          /**< Rd710 PAL ISO 14443-4 definition */

#if defined (NXPBUILD__PHPAL_I14443P4_SW)       || \
    defined (NXPBUILD__PHPAL_I14443P4_SAMAV2_X) || \
    defined (NXPBUILD__PHPAL_I14443P4_RD710)
    #define NXPBUILD__PHPAL_I14443P4            /**< Generic PAL ISO 14443-4 definition */
#endif

/* PAL MIFARE components */

//#define NXPBUILD__PHPAL_MIFARE_STUB             /**< Stub PAL MIFARE */
#define NXPBUILD__PHPAL_MIFARE_SW               /**< Software PAL MIFARE */
//#define NXPBUILD__PHPAL_MIFARE_SAMAV2_X         /**< SamAV2_X PAL MIFARE */
//#define NXPBUILD__PHPAL_MIFARE_RD710            /**< Rd710 PAL MIFARE */

#if defined (NXPBUILD__PHPAL_MIFARE_STUB)       || \
    defined (NXPBUILD__PHPAL_MIFARE_SW)         || \
    defined (NXPBUILD__PHPAL_MIFARE_SAMAV2_X)   || \
    defined (NXPBUILD__PHPAL_MIFARE_RD710)
    #define NXPBUILD__PHPAL_MIFARE              /**< Generic PAL MIFARE definition */
#endif

/* PAL ISO15693 ICode SLI components */

#define NXPBUILD__PHPAL_SLI15693_SW             /**< Software PAL Sli 15693 */

#if defined (NXPBUILD__PHPAL_SLI15693_SW)
    #define NXPBUILD__PHPAL_SLI15693            /**< Generic PAL Sli 15693 definition */
#endif 

/* PAL Felica components */

#define NXPBUILD__PHPAL_FELICA_SW               /**< Software PAL Felica */

#if defined (NXPBUILD__PHPAL_FELICA_SW)
    #define NXPBUILD__PHPAL_FELICA              /**< Generic PAL Felica definition */
#endif 

/* PAL ICode EPC/UID components */

//#define NXPBUILD__PHPAL_EPCUID_SW               /**< Software ICode EPC/UID */


#if defined (NXPBUILD__PHPAL_EPCUID_SW)
    #define NXPBUILD__PHPAL_EPCUID              /**< Generic PAL ICode EPC/UID definition */
#endif

/* PAL ISO 18092 components */

//#define NXPBUILD__PHPAL_I18092MPI_SW            /**< Software PAL ISO 18092 definition */

#if defined (NXPBUILD__PHPAL_I18092MPI_SW)
    #define NXPBUILD__PHPAL_I18092MPI           /**< Generic PAL ISO 18092 definition */
#endif

/* AL Felica components */

#define NXPBUILD__PHAL_FELICA_SW                /**< Software AL Felica */

#if defined (NXPBUILD__PHAL_FELICA_SW)
    #define NXPBUILD__PHAL_FELICA               /**< Generic AL Felica definition */
#endif 

/* AL MIFARE Classic components */

#define NXPBUILD__PHAL_MFC_SW                   /**< Software MIFARE Classic */
//#define NXPBUILD__PHAL_MFC_SAMAV2_X             /**< SamAV2_X MIFARE Classic */

#if defined (NXPBUILD__PHAL_MFC_SW)         || \
    defined (NXPBUILD__PHAL_MFC_SAMAV2_X)
    #define NXPBUILD__PHAL_MFC                  /**< Generic AL MIFARE Classic definition */
#endif

/* AL MIFARE Plus components */

//#define NXPBUILD__PHAL_MFP_SW                   /**< Software MIFARE Plus */
//#define NXPBUILD__PHAL_MFP_SAMAV2               /**< SamAV2 MIFARE Plus */
//#define NXPBUILD__PHAL_MFP_SAMAV2_X             /**< SamAV2_X MIFARE Plus */

#if defined (NXPBUILD__PHAL_MFP_SW)         || \
    defined (NXPBUILD__PHAL_MFP_SAMAV2)     || \
    defined (NXPBUILD__PHAL_MFP_SAMAV2_X)
    #define NXPBUILD__PHAL_MFP                  /**< Generic AL MIFARE Plus definition */
#endif

//#define NXPBUILD__PHAL_MFDF_SW                  /**< Software MIFARE DESFire */
//#define NXPBUILD__PHAL_MFDF_SAMAV2              /**< SamAV2 MIFARE DESFire */
//#define NXPBUILD__PHAL_MFDF_SAMAV2_X            /**< SamAV2_X MIFARE DESFire */

#if defined (NXPBUILD__PHAL_MFDF_SW)         || \
    defined (NXPBUILD__PHAL_MFDF_SAMAV2)     || \
    defined (NXPBUILD__PHAL_MFDF_SAMAV2_X)
    #define NXPBUILD__PHAL_MFDF                  /**< Generic AL MIFARE Desfire definition */
#endif

/* AL MIFARE Ultralight components */

#define NXPBUILD__PHAL_MFUL_SW                  /**< Software MIFARE Ultralight */
//#define NXPBUILD__PHAL_MFUL_SAMAV2              /**< SamAV2 MIFARE Ultralight */
//#define NXPBUILD__PHAL_MFUL_SAMAV2_X            /**< SamAV2_X MIFARE Ultralight */

#if defined (NXPBUILD__PHAL_MFUL_SW)        || \
    defined (NXPBUILD__PHAL_MFUL_SAMAV2)    || \
    defined (NXPBUILD__PHAL_MFUL_SAMAV2_X)
    #define NXPBUILD__PHAL_MFUL                 /**< Generic AL MIFARE Ultralight definition */
#endif

/* AL Virtual Card Architecture components */

//#define NXPBUILD__PHAL_VCA_SW                   /**< Software Virtual Card Architecture */
//#define NXPBUILD__PHAL_VCA_SAMAV2               /**< SamAV2 Virtual Card Architecture */
//#define NXPBUILD__PHAL_VCA_SAMAV2_X             /**< SamAV2_X Virtual Card Architecture */

#if defined (NXPBUILD__PHAL_VCA_SW)         || \
    defined (NXPBUILD__PHAL_VCA_SAMAV2)     || \
    defined (NXPBUILD__PHAL_VCA_SAMAV2_X)
    #define NXPBUILD__PHAL_VCA                  /**< Generic AL Virtual Card Architecture definition */
#endif

/* DL FPGA Box components */


/* DL Amplifier components */


/* DL Thermostream components */


/* DL Oscilloscope components */


/* DL Master Amplifier Oscilloscope components */


/* DL Stepper components */


/* AL I15693 components */

#define NXPBUILD__PHAL_I15693_SW                /**< Software AL I15693 */

#if defined (NXPBUILD__PHAL_I15693_SW)
    #define NXPBUILD__PHAL_I15693               /**< Generic AL I15693 definition */
#endif

/* AL Sli components */

//#define NXPBUILD__PHAL_SLI_SW                   /**< Software AL Sli */

#if defined (NXPBUILD__PHAL_SLI_SW)
    #define NXPBUILD__PHAL_SLI                  /**< Generic AL Sli definition */
#endif

/* CID Manager components */

//#define NXPBUILD__PH_CIDMANAGER_SW              /**< Software CID Manager */
//#define NXPBUILD__PH_CIDMANAGER_RD710           /**< Rd710 CID Manager */

#if defined (NXPBUILD__PH_CIDMANAGER_SW)       || \
    defined (NXPBUILD__PH_CIDMANAGER_RD710)
    #define NXPBUILD__PH_CIDMANAGER             /**< Generic CID Manager definition */
#endif

/* Crypto components */

//#define NXPBUILD__PH_CRYPTOSYM_SW                  /**< Software Crypto Sym */
//#define NXPBUILD__PH_CRYPTOSYM_STUB                /**< Stub Crypto Sym */

#if defined (NXPBUILD__PH_CRYPTOSYM_SW)        || \
	defined (NXPBUILD__PH_CRYPTOSYM_STUB) 
    #define NXPBUILD__PH_CRYPTOSYM                 /**< Generic Crypto Sym definition */
#endif

/* CryptoRng components */

//#define NXPBUILD__PH_CRYPTORNG_SW               /**< Software CryptoRng */
//#define NXPBUILD__PH_CRYPTORNG_STUB             /**< Stub CryptoRng */

#if defined (NXPBUILD__PH_CRYPTORNG_SW)       || \
	defined (NXPBUILD__PH_CRYPTORNG_STUB)  
    #define NXPBUILD__PH_CRYPTORNG              /**< Generic CryptoRng definition */
#endif

/* KeyStore components */

//#define NXPBUILD__PH_KEYSTORE_SW                /**< Software KeyStore */
//#define NXPBUILD__PH_KEYSTORE_SAMAV2            /**< SamAV2 KeyStore */
//#define NXPBUILD__PH_KEYSTORE_RC632             /**< RC632 KeyStore */
#define NXPBUILD__PH_KEYSTORE_RC663             /**< RC663 KeyStore */
//#define NXPBUILD__PH_KEYSTORE_RD710             /**< Rd710 KeyStore */

#if defined (NXPBUILD__PH_KEYSTORE_SW)      || \
    defined (NXPBUILD__PH_KEYSTORE_SAMAV2)  || \
    defined (NXPBUILD__PH_KEYSTORE_RC663)   || \
    defined (NXPBUILD__PH_KEYSTORE_RC632)   || \
    defined (NXPBUILD__PH_KEYSTORE_RD710)
    #define NXPBUILD__PH_KEYSTORE               /**< Generic KeyStore definition */
#endif

/* Log component */

//#define NXPBUILD__PH_LOG

/* Add the local modifications added by myself */
#define JLL_MOD				1

#define DEBUG_MESSAGE		1

/** @}
* end of ph_NxpBuild
*/

#endif /* PH_NXPBUILD_H */
