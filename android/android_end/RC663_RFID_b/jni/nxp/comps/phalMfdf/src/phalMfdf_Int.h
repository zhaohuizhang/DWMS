/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Internal functions of Software implementation of MIFARE DESFIRE EV1 application layer.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:18 2011 $
*
* History:
*  SA: Generated 09. Mar 2010
*
*/

#ifndef PHALMFDF_INT_H
#define PHALMFDF_INT_H

#include <ph_Status.h>
#include <phalMfdf.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \name MIFARE Desfire Native Commands
*/

/*@{*/
#define PHAL_MFDF_CMD_AUTHENTICATE              0x0A    /**< MF DF Authenticate command. */
#define PHAL_MFDF_CMD_AUTHENTICATE_ISO          0x1A    /**< MF DF Authenticate ISO command. */
#define PHAL_MFDF_CMD_AUTHENTICATE_AES          0xAA    /**< MF DF Authenticate AES command. */
#define PHAL_MFDF_CMD_CHANGE_KEY_SETTINGS       0x54    /**< MF DF Change key settings cmd. */
#define PHAL_MFDF_CMD_GET_KEY_SETTINGS          0x45    /**< MF DF Get Key Settings Cmd.  */
#define PHAL_MFDF_CMD_CHANGE_KEY                0xC4    /**< MF DF Change key cmd. */
#define PHAL_MFDF_CMD_GET_KEY_VERSION           0x64    /**< MF DF Get Key Version. */
#define PHAL_MFDF_CMD_CREATE_APPLN              0xCA    /**< MF DF Create Application cmd. */
#define PHAL_MFDF_CMD_DELETE_APPLN              0xDA    /**< MF DF Delete Application cmd. */
#define PHAL_MFDF_CMD_GET_APPLN_IDS             0x6A    /**< MF DF Get Application Ids cmd. */
#define PHAL_MFDF_CMD_GET_DF_NAMES              0x6D    /**< MF DF Get Dedicated Fine names cmd. */
#define PHAL_MFDF_CMD_SELECT_APPLN              0x5A    /**< MF DF Select Application Cmd. */
#define PHAL_MFDF_CMD_FORMAT_PICC               0xFC    /**< MF DF Format PICC Cmd. */
#define PHAL_MFDF_CMD_GET_VERSION               0x60    /**< MF DF Get Version cmd. */
#define PHAL_MFDF_CMD_FREE_MEM                  0x6E    /**< MF DF Free Memory cmd. */
#define PHAL_MFDF_CMD_SET_CONFIG                0x5C    /**< MF DF Set Configuration Cmd. */
#define PHAL_MFDF_CMD_GET_CARD_UID              0x51    /**< MF DF Get Card UID cmd. */
#define PHAL_MFDF_CMD_GET_FILE_IDS              0x6F    /**< MF DF Get File IDs cmd. */
#define PHAL_MFDF_CMD_GET_ISO_FILE_IDS          0x61    /**< MF DF Get ISO File IDs cmd. */
#define PHAL_MFDF_CMD_GET_FILE_SETTINGS         0xF5    /**< MF DF Get File settings cmd. */
#define PHAL_MFDF_CMD_CHANGE_FILE_SETTINGS      0x5F    /**< MF DF Change file settings cmd. */
#define PHAL_MFDF_CMD_CREATE_STD_DATAFILE       0xCD    /**< MF DF Create Standard data file cmd. */
#define PHAL_MFDF_CMD_CREATE_BKUP_DATAFILE      0xCB    /**< MF DF Create Backup data file cmd. */
#define PHAL_MFDF_CMD_CREATE_VALUE_FILE         0xCC    /**< MF DF Create Value File cmd. */
#define PHAL_MFDF_CMD_CREATE_LINEAR_RECFILE     0xC1    /**< MF DF Create Linear record file cmd. */
#define PHAL_MFDF_CMD_CREATE_CYCLIC_RECFILE     0xC0    /**< MF DF Create Cyclic record file cmd. */
#define PHAL_MFDF_CMD_DELETE_FILE               0xDF    /**< MF DF Delete File cmd. */
#define PHAL_MFDF_CMD_READ_DATA                 0xBD    /**< MF DF Read Data cmd. */
#define PHAL_MFDF_CMD_WRITE_DATA                0x3D    /**< MF DF Write data cmd. */
#define PHAL_MFDF_CMD_GET_VALUE                 0x6C    /**< MF DF Get Value cmd. */
#define PHAL_MFDF_CMD_CREDIT                    0x0C    /**< MF DF Credit cmd. */
#define PHAL_MFDF_CMD_DEBIT                     0xDC    /**< MF DF Debit cmd. */
#define PHAL_MFDF_CMD_LIMITED_CREDIT            0x1C    /**< MF DF Limited Credit cmd. */
#define PHAL_MFDF_CMD_WRITE_RECORD              0x3B    /**< MF DF Write Record cmd. */
#define PHAL_MFDF_CMD_READ_RECORDS              0xBB    /**< MF DF Read Records cmd. */
#define PHAL_MFDF_CMD_CLEAR_RECORDS_FILE        0xEB    /**< MF DF Clear records file cmd. */
#define PHAL_MFDF_CMD_COMMIT_TXN                0xC7    /**< MF DF Commit transaction cmd. */
#define PHAL_MFDF_CMD_ABORT_TXN                 0xA7    /**< MF DF Abort transaction cmd. */
/*@}*/

/**
* \name MIFARE Desfire Response Codes
*/

/*@{*/

#define PHAL_MFDF_RESP_OPERATION_OK                 0x00U    /**< MF DF Response - Successful operation. */
#define PHAL_MFDF_RESP_NO_CHANGES                   0x0CU    /**< MF DF Response - No changes done to backup files. */
#define PHAL_MFDF_RESP_ERR_OUT_OF_EEPROM_ERROR      0x0EU    /**< MF DF Response - Insufficient NV-Memory. */
#define PHAL_MFDF_RESP_ILLEGAL_COMMAND_CODE         0x1CU    /**< MF DF command code not supported. */
#define PHAL_MFDF_RESP_ERR_INTEGRITY_ERROR          0x1EU    /**< MF DF CRC or MAC does not match data padding bytes not valid. */
#define PHAL_MFDF_RESP_NO_SUCH_KEY                  0x40U    /**< MF DF Invalid key number specified. */
#define PHAL_MFDF_RESP_ERR_LENGTH_ERROR             0x7EU    /**< MF DF Length of command string invalid. */
#define PHAL_MFDF_RESP_PERMISSION_DENIED            0x9DU    /**< MF DF Current configuration/status does not allow the requested command. */
#define PHAL_MFDF_RESP_ERR_PARAMETER_ERROR          0x9EU    /**< MF DF Value of params invalid. */
#define PHAL_MFDF_RESP_APPLICATION_NOT_FOUND        0xA0U    /**< MF DF Requested AID not found on PICC. */
#define PHAL_MFDF_RESP_ERR_APPL_INTEGRITY_ERROR     0xA1U    /**< MF DF Unrecoverable error within application, appln will be disabled. */
#define PHAL_MFDF_RESP_ERR_AUTHENTICATION_ERROR     0xAEU    /**< MF DF Current authentication status does not allow the requested cmd. */
#define PHAL_MFDF_RESP_ADDITIONAL_FRAME             0xAFU    /**< MF DF Additional data frame is expected to be sent. */
#define PHAL_MFDF_RESP_ERR_BOUNDARY_ERROR           0xBEU    /**< MF DF Attempt to read/write data from/to beyond the files/record's limits. */
#define PHAL_MFDF_RESP_ERR_PICC_INTEGRITY           0xC1U    /**< MF DF Unrecoverable error within PICC. PICC will be disabled. */
#define PHAL_MFDF_RESP_ERR_COMMAND_ABORTED          0xCAU    /**< MF DF Previous cmd not fully completed. Not all frames were requested or provided by the PCD. */
#define PHAL_MFDF_RESP_ERR_PIC_DISABLED             0xCDU    /**< MF DF PICC was disabled by an unrecoverable error. */
#define PHAL_MFDF_RESP_ERR_COUNT                    0xCEU    /**< MF DF Num. of applns limited to 28. No additional applications possible. */
#define PHAL_MFDF_RESP_ERR_DUPLICATE                0xDEU    /**< MF DF File/Application with same number already exists. */
#define PHAL_MFDF_RESP_ERR_EEPROM                   0xEEU    /**< MF DF Could not complete NV-Write operation due to loss of power. */
#define PHAL_MFDF_RESP_ERR_FILE_NOT_FOUND           0xF0U    /**< MF DF Specified file number does not exist. */
#define PHAL_MFDF_RESP_ERR_FILE_INTEGRITY           0xF1U    /**< MF DF Unrecoverable error within file. File will be disabled. */

/*@}*/

/**
* \name ISO 7816 Instructions
*/

/*@{*/
#define PHAL_MFDF_CMD_ISO7816_SELECT_FILE       0xA4    /**< ISO Select File. */
#define PHAL_MFDF_CMD_ISO7816_READ_RECORDS      0xB2    /**< ISO Read records. */
#define PHAL_MFDF_CMD_ISO7816_READ_BINARY       0xB0    /**< ISO Read Binary. */
#define PHAL_MFDF_CMD_ISO7816_UPDATE_BINARY     0xD6    /**< ISO UPDATE Binary. */
#define PHAL_MFDF_CMD_ISO7816_APPEND_RECORD     0xE2    /**< ISO Append record. */
#define PHAL_MFDF_CMD_ISO7816_UPDATE_RECORD     0xD2    /**< ISO Update record. */
#define PHAL_MFDF_CMD_ISO7816_GET_CHALLENGE     0x84    /**< ISO Get challenge. */
#define PHAL_MFDF_CMD_ISO7816_EXT_AUTHENTICATE  0x82    /**< ISO Ext. Authenticate. */
#define PHAL_MFDF_CMD_ISO7816_INT_AUTHENTICATE  0x88    /**< ISO Int. Authenticate. */
/*@}*/

/**
* \name MIFARE Desfire specific defines
*/

/*@{*/
#define PHAL_MFDF_MAXWRAPPEDAPDU_SIZE   0x37u   /**< Maximum size of wrapped APDU 55 bytes. */
#define PHAL_MFDF_MAXDFAPDU_SIZE        0x3Cu   /**< Maximum size of DESFire APDU 60 bytes. */
#define PHAL_MFDF_DFAPPID_SIZE          0x03u   /**< Size of MF DF application Id. */
/*@}*/

/** 
* \name ISO 7816 DF return Codes
*/
/*@{*/
#define PHAL_MFDF_ISO7816_SUCCESS                       0x9000U /**< Correct execution. */
#define PHAL_MFDF_ISO7816_ERR_WRONG_LENGTH              0x6700U /**< Wrong length. */
#define PHAL_MFDF_ISO7816_ERR_INVALID_APPLN             0x6A82U /**< Application / file not found. */
#define PHAL_MFDF_ISO7816_ERR_WRONG_PARAMS              0x6A86U /**< Wrong parameters P1 and/or P2. */
#define PHAL_MFDF_ISO7816_ERR_WRONG_LC			        0x6A87U /**< Lc inconsistent with P1/p2. */
#define PHAL_MFDF_ISO7816_ERR_WRONG_LE                  0x6C00U /**< Wrong Le. */
#define PHAL_MFDF_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS    0x6F00U /**< No precise diagnostics. */
#define PHAL_MFDF_ISO7816_ERR_EOF_REACHED		        0x6282U /**< End of File reached. */
#define PHAL_MFDF_ISO7816_ERR_FILE_ACCESS		        0x6982U	/**< File access not allowed. */
#define PHAL_MFDF_ISO7816_ERR_FILE_EMPTY		        0x6985U	/**< File empty or access conditions not satisfied. */
#define PHAL_MFDF_ISO7816_ERR_FILE_NOT_FOUND		    0x6A82U /**< File not found. */
#define PHAL_MFDF_ISO7816_ERR_MEMORY_FAILURE		    0x6581U /**< Memory failure (unsuccessful update). */
#define PHAL_MFDF_ISO7816_ERR_INCORRECT_PARAMS		    0x6B00U /**< Wrong parameter p1 or p2. READ RECORDS. */
#define PHAL_MFDF_ISO7816_ERR_WRONG_CLA			        0x6E00U /**< Wrong Class byte. */
#define PHAL_MFDF_ISO7816_ERR_UNSUPPORTED_INS		    0x6D00U /**< Instruction not supported. */
/*@}*/

#define PHAL_MFDF_WRAP_HDR_LEN      0x05U   /* Wrapped APDU header length */
#define PHAL_MFDF_WRAPPEDAPDU_CLA   0x90U   /* Wrapped APDU default class. */
#define PHAL_MFDF_WRAPPEDAPDU_P1    0x00U   /* Wrapped APDU default P1. */
#define PHAL_MFDF_WRAPPEDAPDU_P2    0x00U   /* Wrapped APDU default P2. */
#define PHAL_MFDF_WRAPPEDAPDU_LE    0x00U   /* Wrapped APDU default LE. */

/* To avoid unused variable warnings. */
#define PHAL_MFDF_UNUSED_VARIABLE(x)  {for( ( x ) = ( x ) ; ( x ) != ( x ) ; );}
#define PHAL_MFDF_UNUSED_ARRAY(x)  {for( ( (x)[0] ) = ( (x)[0] ) ; ( (x)[0] ) != ( (x)[0] ) ; );}

phStatus_t phalMfdf_ExchangeCmd(
                                void * pDataParams,         /* Pointer to params data structure */
                                void * pPalMifareDataParams,  /*  Pointer to mifare protocol layer */
                                uint8_t bWrappedMode,
                                uint8_t * pCmd,
                                uint16_t wCmdLen,
                                uint8_t * pResponse,
                                uint16_t * wRxlen
                                );

phStatus_t phalMfdf_Int_ComputeErrorResponse(
    void * pDataParams,
    uint16_t wStatus
    );

phStatus_t phalMfdf_Int_Send7816Apdu(
                                     void * pDataParams,
                                     void * pPalMifareDataParams,
                                     uint8_t bOption,
                                     uint8_t bIns,
                                     uint8_t p1,
                                     uint8_t p2,
                                     uint8_t Lc,   /*  Length of Data sent with this cmd. */
                                     uint8_t * pDataIn,  /*  Data In of Lc bytes. */
                                     uint8_t bLe,  /*  Length of expected response */
                                     uint8_t ** pDataOut,  /*  Data received from the card. */
                                     uint16_t *pDataLen  /*  Length of data returned. */
                                     );
/*@}*/

#endif /* PHALMFDF_INT_H */
