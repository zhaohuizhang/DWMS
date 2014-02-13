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
* Internal definitions for SAMAV2 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.6 $
* $Date: Fri Apr 15 09:27:18 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_SAMAV2_CMD_H
#define PHHALHW_SAMAV2_CMD_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef  NXPBUILD__PHHAL_HW_SAMAV2

/** \addtogroup ph_Error
* phHalHw_SamAV2 Custom Errors
* @{
*/

/** \name phHalHw_SamAV2 Error Codes
*/
/*@{*/
#define PHHAL_HW_SAMAV2_ERR_ISO_UID_BCC_INCORRECT           (PH_ERR_CUSTOM_BEGIN)
#define PHHAL_HW_SAMAV2_ERR_ISO_WRONG_BNR                   (PH_ERR_CUSTOM_BEGIN+1)
#define PHHAL_HW_SAMAV2_ERR_ISO_INVALID_FORMAT              (PH_ERR_CUSTOM_BEGIN+2)
#define PHHAL_HW_SAMAV2_ERR_ISO_INVALID_PARAMETER           (PH_ERR_CUSTOM_BEGIN+3)
#define PHHAL_HW_SAMAV2_ERR_ISO_UID_INCOMPLETE              (PH_ERR_CUSTOM_BEGIN+4)     /**< Incomplete UID Error. */
#define PHHAL_HW_SAMAV2_ERR_MIFARE_GEN                      (PH_ERR_CUSTOM_BEGIN+5)     /**< Generic MIFARE Error. */
#define PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_CRYPTO              (PH_ERR_CUSTOM_BEGIN+6)     /**< Crypto MIFARE Plus Error. */
#define PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN                 (PH_ERR_CUSTOM_BEGIN+7)     /**< Generic MIFARE Plus Error. */
#define PHHAL_HW_SAMAV2_ERR_DESFIRE_GEN                     (PH_ERR_CUSTOM_BEGIN+8)     /**< Generic DESFIRE Error. */
#define PHHAL_HW_SAMAV2_ERR_CRYPTO                          (PH_ERR_CUSTOM_BEGIN+9)     /**< MAC verification failed. */
#define PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_LENGTH_LC         (PH_ERR_CUSTOM_BEGIN+10)
#define PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_P1P2              (PH_ERR_CUSTOM_BEGIN+11)
#define PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_LE                (PH_ERR_CUSTOM_BEGIN+12)
#define PHHAL_HW_SAMAV2_ERR_ISO7816_UNKNOWN_INS             (PH_ERR_CUSTOM_BEGIN+13)
#define PHHAL_HW_SAMAV2_ERR_ISO7816_UNKNOWN_CLASS           (PH_ERR_CUSTOM_BEGIN+14)
#define PHHAL_HW_SAMAV2_ERR_ISO7816_COMMAND_NOT_ALLOWED     (PH_ERR_CUSTOM_BEGIN+15)
#define PHHAL_HW_SAMAV2_ERR_ISO7816_WRONG_PARAMS_FOR_INS    (PH_ERR_CUSTOM_BEGIN+16)
#define PHHAL_HW_SAMAV2_ERR_HW_EEPROM                       (PH_ERR_CUSTOM_BEGIN+17)
#define PHHAL_HW_SAMAV2_ERR_HW_RC5XX                        (PH_ERR_CUSTOM_BEGIN+18)
#define PHHAL_HW_SAMAV2_ERR_HW_EE_HIGH_VOLTAGE              (PH_ERR_CUSTOM_BEGIN+19)
#define PHHAL_HW_SAMAV2_ERR_KEY_CREATE_FAILED               (PH_ERR_CUSTOM_BEGIN+20)
#define PHHAL_HW_SAMAV2_ERR_KEY_REF_NO_INVALID              (PH_ERR_CUSTOM_BEGIN+21)
#define PHHAL_HW_SAMAV2_ERR_KEY_KUC_NO_INVALID              (PH_ERR_CUSTOM_BEGIN+22)
#define PHHAL_HW_SAMAV2_ERR_KEY_INTEGRITY_ERROR             (PH_ERR_CUSTOM_BEGIN+23)
#define PHHAL_HW_SAMAV2_ERR_KEY_VERSION_INVALID             (PH_ERR_CUSTOM_BEGIN+24)
#define PHHAL_HW_SAMAV2_ERR_COND_USE_NOT_SATISFIED          (PH_ERR_CUSTOM_BEGIN+25)
#define PHHAL_HW_SAMAV2_ERR_INTEGRITY_ERROR                 (PH_ERR_CUSTOM_BEGIN+26)
#define PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE              (PH_ERR_CUSTOM_BEGIN+27) 
#define PHHAL_HW_SAMAV2_ERR_INCOMPLETE_CHAINING             (PH_ERR_CUSTOM_BEGIN+28)
#define PHHAL_HW_SAMAV2_ERR_MIFARE_NAK0                     (PH_ERR_CUSTOM_BEGIN+29)
#define PHHAL_HW_SAMAV2_ERR_MIFARE_NAK1                     (PH_ERR_CUSTOM_BEGIN+30)
#define PHHAL_HW_SAMAV2_ERR_MIFARE_NAK4                     (PH_ERR_CUSTOM_BEGIN+31)
#define PHHAL_HW_SAMAV2_ERR_MIFARE_NAK5                     (PH_ERR_CUSTOM_BEGIN+32)
/*@}*/

/** @} */

/** \defgroup phhalHw_SamAV2_Cmd_Defines SAM Defines
* \brief Defines used accross the whole SAM implementation.
* @{
*/

/** \name ISO7816 Application protocol data unit structure (APDU)
*/
/*@{*/
#define PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH           0x05U   /**< Length of ISO7816 header */
#define PHHAL_HW_SAMAV2_ISO7816_HEADER_LE_LENGTH        0x06U   /**< Length of ISO7816 header including LE byte*/
#define PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH     0x04U   /**< Length of ISO7816 header excluding LC byte*/
#define PHHAL_HW_SAMAV2_ISO7816_CLA_POS                 0x00U   /**< Pos of CLA byte in ISO7816 header */
#define PHHAL_HW_SAMAV2_ISO7816_INS_POS                 0x01U   /**< Pos of INS byte in ISO7816 header */
#define PHHAL_HW_SAMAV2_ISO7816_P1_POS                  0x02U   /**< Pos of P1 byte in ISO7816 header */
#define PHHAL_HW_SAMAV2_ISO7816_P2_POS                  0x03U   /**< Pos of P2 byte in ISO7816 header */
#define PHHAL_HW_SAMAV2_ISO7816_LC_POS                  0x04U   /**< Pos of LC byte in ISO7816 header */
#define PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS            0x04U   /**< Pos of LE byte in ISO7816 header and no LC */
#define PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH           0x02U   /**< Length of SW1SW2 in ISO7816 */
#define PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_APP_DATA_MAX   0xFFU   /**< Max Length of Transparent Exchange Response */
#define PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_RESPONSE_MAX   0xF8U   /**< Max Length of Transparent Exchange Response */

#define PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE                0x80U   /**< Cla Byte of SamAV2 Commands */
#define PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE         0x00U   /**< Default Ins Byte of SamAV2 Commands */
#define PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE         0x00U   /**< Default Ins Byte of SamAV2 Commands */
#define PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE         0x00U   /**< Default Le Byte of SamAV2 Commands */

#define PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME              0x00U   /**< Indication last frame in a cipher sequence */
#define PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME           0xAFU   /**< Indication lnon ast frame in a cipher sequence */

#define PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK    0xF0    /**< Maximal data field length, which is still a multiple of AES block length. */
/*@}*/

/** \name Error codes returned by the SAM. 
*/
/*@{*/

/** Success status codes */

#define PHHAL_HW_SAMAV2_RET_CODE_OK                     0x9000  /**< Correct execution */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_1BIT                0x9001  /**< Correct execution, 1 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_2BIT                0x9002  /**< Correct execution, 2 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_3BIT                0x9003  /**< Correct execution, 3 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_4BIT                0x9004  /**< Correct execution, 4 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_5BIT                0x9005  /**< Correct execution, 5 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_6BIT                0x9006  /**< Correct execution, 7 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_7BIT                0x9007  /**< Correct execution, 7 bits received */
#define PHHAL_HW_SAMAV2_RET_CODE_OK_CHAINING_ACTIVE     0x90AF  /**< Correct execution - 2 Part Command */

/** Communication Error Codes codes - can be used from generic HAL layer */

#define PHHAL_HW_SAMAV2_RET_CODE_COMM_IO_TIMEOUT                0x90E0  /**< no response from card (no card in field) */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_BITCNT_PROTOCOL           0x90E1  /**< bit count error or protocol error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_PARITY                    0x90E2  /**< parity error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_FIFO_BUF_OVERFLOW         0x90E3  /**< MFRC52X FIFO buffer overflow */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_CRC_FAILURE               0x90E4  /**< CRC error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_RF_FAILURE                0x90E5  /**< MFRC52X RF error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_TEMP_FAILURE              0x90E6  /**< MFRC52X temperature error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_FIFO_WRITE                0x90E7  /**< MFRC52X FIFO write error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_COLLISION                 0x90E8  /**< collision error */
#define PHHAL_HW_SAMAV2_RET_CODE_COMM_INTERNAL_BUF_OVERFLOW     0x90E9  /**< internal transaction buffer overflow (more than 256 bytes received from the card) */

/** ISO Error Codes codes */

#define PHHAL_HW_SAMAV2_RET_CODE_ISO_UID_BCC_INCORRECT  0x90EA  /**< UID check byte (BCC) incorrect */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO_WRONG_BNR          0x90EB  /**< Invalid Layer 4 Block number in PCB */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO_INVALID_FORMAT     0x90EC  /**< invalid format received in any ISO command */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO_INVALID_PARAMETER  0x90ED  /**< invalid parameter received in any ISO command*/
#define PHHAL_HW_SAMAV2_RET_CODE_ISO_UID_INCOMPLETE     0x90C0  /**< UID not yet complete in card selection */

/** Application protocol error codes */

#define PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_ERROR          0x90EF  /**< MIFARE (R) protocol error */
#define PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_0         0x90F0  /**< MIFARE (R) NACK 0 received */
#define PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_1         0x90F1  /**< MIFARE (R) NACK 1 received */
#define PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_4         0x90F4  /**< MIFARE (R) NACK 4 received */
#define PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_NACK_5         0x90F5  /**< MIFARE (R) NACK 5 received */
#define PHHAL_HW_SAMAV2_RET_CODE_PROT_MIFARE_PLUS_ERROR     0x90BE  /**< MIFARE PLUS (R) protocol error */
#define PHHAL_HW_SAMAV2_RET_CODE_INS_MIFARE_PLUS_ERROR      0x90BF  /**< MIFARE PLUS (R) return code error */
#define PHHAL_HW_SAMAV2_RET_CODE_PROT_DESFIRE_ERROR         0x90DF  /**< MIFARE DESFIRE (R) protocol error */

/** Crypto error codes */

#define PHHAL_HW_SAMAV2_RET_CODE_CRYPTO_FAILURE     0x901E  /**< MAC verification failed, CRC/Padding failed, auth error, integrity error */

/** ISO7816 Error Codes codes */

#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_LENGTH_LC        0x6700  /**< Wrong Length or LC byte */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_P1P2             0x6A86  /**< Wrong P1 P2 */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_LE               0x6C00  /**< Wrong LE */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_UNKNOWN_INS            0x6D00  /**< Wrong Instruction */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_CLASS            0x6E00  /**< Wrong Class */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_COMMAND_NOT_ALLOWED    0x6986  /**< Command not allowed */
#define PHHAL_HW_SAMAV2_RET_CODE_ISO7816_WRONG_PARAMS_FOR_INS   0x6A80  /**< Wrong parameters for current instruction */

/** SAM Hardware related Error Codes codes */

#define PHHAL_HW_SAMAV2_RET_CODE_HW_EEPROM              0x6400  /**< Memory Range, EEProm busy or access collision in EEPROM module */
#define PHHAL_HW_SAMAV2_RET_CODE_HW_RC5XX               0x6401  /**< MFRC52X interface error */
#define PHHAL_HW_SAMAV2_RET_CODE_HW_EE_HIGH_VOLTAGE     0x6581  /**< High voltage error in SAM EEprom module*/

/** Key management error codes */

#define PHHAL_HW_SAMAV2_RET_CODE_KEY_CREATE_FAILED      0x6501  /**< Creation of key entry failed */
#define PHHAL_HW_SAMAV2_RET_CODE_KEY_REF_NO_INVALID     0x6502  /**< Invalid key reference number */
#define PHHAL_HW_SAMAV2_RET_CODE_KEY_KUC_NO_INVALID     0x6503  /**< Invalid key usage counter reference number */
#define PHHAL_HW_SAMAV2_RET_CODE_KEY_INTEGRITY_ERROR    0x6984  /**< Key integrity error, wrong key reference */
#define PHHAL_HW_SAMAV2_RET_CODE_KEY_VERSION_INVALID    0x6A82  /**< Invalid key version */

/** Other error codes */

#define PHHAL_HW_SAMAV2_RET_CODE_COND_USE_NOT_SATISFIED     0x6985  /**< conditions of use not satisfied, invalid key type, invalid CID, key limit reached */
#define PHHAL_HW_SAMAV2_RET_CODE_INTEGRITY_ERROR            0x6982  /**< Integrity error */
#define PHHAL_HW_SAMAV2_RET_CODE_INCOMPLETE_CHAINING        0x6883  /**< Invalid chaining sequence */

/*@}*/

/** \name APDU CMD bytes supported by the SAM
*/
/*@{*/
#define PHHAL_HW_SAMAV2_CMD_GET_CHALLENGE_INS                               0x84    /**< CMD Byte for SAM_IsoGetChallenge command */

#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_INS                       0xC5    /**< CMD Byte for SAM_ChangeKeyMIFARE command */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_CHANGING_MIN_DATA_LENGTH  0x07    /**< Minimum data length of SAM_ChangeKeyMIFARE_Changing command. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_CHANGING_MAX_DATA_LENGTH  0x0C    /**< Maximum data length of SAM_ChangeKeyMIFARE_Changing command. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_RETRIEVE_MIN_DATA_LENGTH  0x03    /**< Minimum data length of SAM_ChangeKeyMIFARE_Retrieve command. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_RETRIEVE_MAX_DATA_LENGTH  0x08    /**< Maximum data length of SAM_ChangeKeyMIFARE_Retrieve command. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_CHANGING_RESPONSE_LENGTH  0x15    /**< Response length of the SAM_ChangeKeyMIFARE_Changing command. */

#define PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_LENGTH                       0x0D    /**< Data length of MF_ChangeKey command (with Diversification)*/
#define PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_NO_DIV_LENGTH                0x08    /**< Data length of MF_ChangeKey command (without Diversification)*/

#define PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_INS                      0x1C    /**< CMD Byte for AuthenticateMifare command */
#define PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_DATA_LENGTH_PART1        0x0E    /**< Length of data of the Auth Mifare command Part1 */
#define PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_DATA_LENGTH_PART2        0x05    /**< Length of data of the Auth Mifare command Part2*/
#define PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_RESPONSE_LENGTH          0x09

#define PHHAL_HW_SAMAV2_CMD_MF_READ_INS                     0x30    /**< CMD Byte for MF_Read command */
#define PHHAL_HW_SAMAV2_CMD_MF_AUTH_READ_INS                0x3A    /**< CMD Byte for MF_AuthenticateRead command */
#define PHHAL_HW_SAMAV2_CMD_MF_WRITE_INS                    0xA0    /**< CMD Byte for MF_Write command */
#define PHHAL_HW_SAMAV2_CMD_MF_AUTH_WRITE_INS               0xAA    /**< CMD Byte for MF_AuthenticateWrite command */
#define PHHAL_HW_SAMAV2_CMD_MF_WRITE_VALUE_INS              0xA2    /**< CMD Byte for MF_WriteValue command */
#define PHHAL_HW_SAMAV2_CMD_MF_INCREMENT_INS                0xC3    /**< CMD Byte for MF_Increment command */
#define PHHAL_HW_SAMAV2_CMD_MF_DECREMENT_INS                0xC0    /**< CMD Byte for MF_Decrement command */
#define PHHAL_HW_SAMAV2_CMD_MF_RESTORE_INS                  0xC2    /**< CMD Byte for MF_Restore command */
#define PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_EXCHANGE_INS    0x0C    /** < Mifare Classic Authenticate command */
#define PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_INFO_LENGTH     0x05    /** < Mifare Classic Authenticate Length of info except for UID */
#define PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_INS               0xA1    /**< CMD Byte for MF_ChangeKey command */

#define PHHAL_HW_SAMAV2_CMD_MF_AUTH_READ_DATAHEADER_LENGTH  0x07    /**< Data header length of MF_AuthenticateRead command (without UID)*/
#define PHHAL_HW_SAMAV2_CMD_MF_AUTH_WRITE_DATAHEADER_LENGTH 0x08    /**< Data header length of MF_AuthenticateWrite command (without UID)*/

#define PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_MFP                0xA3    /**< CMD Byte for SAM_AuthenticateMFP command */
#define PHHAL_HW_SAMAV2_CMD_WRITE_MFP                       0x34    /**< CMD Byte for SAM_CombinedWriteMFP command */
#define PHHAL_HW_SAMAV2_CMD_READ_MFP                        0x33    /**< CMD Byte for SAM_CombinedReadMFP command */
#define PHHAL_HW_SAMAV2_CMD_VERIFY_PROXIMITY_CHECK          0xFE    /**< CMD Byte for SAM_ProximityCheck command */                 
#define PHHAL_HW_SAMAV2_CMD_VIRTUAL_CARD_SUPPORT            0x41    /**< CMD Byte for SAM_VirtualCardSupport command */   
#define PHHAL_HW_SAMAV2_CMD_SELECT_VIRTUAL_CARD             0x43    /**< CMD Byte for SAM_SelectVirtualCard command */   
#define PHHAL_HW_SAMAV2_CMD_READ_MFP_COMMAND                0x00    /**< data parameter is a command */
#define PHHAL_HW_SAMAV2_CMD_READ_MFP_RESPONSE               0x01    /**< data parameter is a response */
#define PHHAL_HW_SAMAV2_CMD_READ_MFP_BOTH                   0x02    /**< data parameter is a command and a response */

#define PHHAL_HW_SAMAV2_CMD_MFP_WRITE_PERSO_INS             0xA8    /**< CMD Byte for WritePerso command */
#define PHHAL_HW_SAMAV2_CMD_MFP_WRITE                       0x32    /**< CMD Byte for MFP_CombinedWrite command */
#define PHHAL_HW_SAMAV2_CMD_MFP_READ                        0x31    /**< CMD Byte for MFP_CombinedRead command */
#define PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE                0x70    /**< CMD Byte for MFP_Authenticate command */
#define PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK             0xFD    /**< CMD Byte for MFP_Proximity Check command */
#define PHHAL_HW_SAMAV2_CMD_MFP_VIRTUAL_CARD_SUPPORT        0x40    /**< CMD Byte for MFP_VirtualCardSupport command */
#define PHHAL_HW_SAMAV2_CMD_MFP_SELECT_VIRTUAL_CARD         0x42    /**< CMD Byte for MFP_SelectVirtualCard command */
#define PHHAL_HW_SAMAV2_CMD_MFP_CHANGE_KEY_INS              0xA5    /**< CMD Byte for MFPChangeKey command */

#define PHHAL_HW_SAMAV2_CMD_DESFIRE_AUTH_PICC_INS               0xDA    /**< CMD Byte for DESFIRE_AuthenticatePICC command */
#define PHHAL_HW_SAMAV2_CMD_DESFIRE_AUTH_PICC_MIN_DATA_LENGTH   0x03    /**< Minimum CMD length of DESFIRE_AuythenticatePICC command */

#define PHHAL_HW_SAMAV2_CMD_DESFIRE_CHANGE_KEY_INS              0xDE    /**< CMD Byte for DESFIRE_ChangeKey command */
#define PHHAL_HW_SAMAV2_CMD_DESFIRE_CHANGE_KEY_MIN_DATA_LENGTH  0x04    /**< Minimum CMD length of DESFIRE_ChangeKey command */

#define PHHAL_HW_SAMAV2_CMD_DESFIRE_WRITE_X_INS                   0xD3    /**< CMD Byte for DESFIRE_WriteX command */
#define PHHAL_HW_SAMAV2_CMD_DESFIRE_READ_X_INS                    0xD2    /**< CMD Byte for DESFIRE_ReadX command */
#define PHHAL_HW_SAMAV2_CMD_DESFIRE_READ_X_MIN_DATA_LENGTH        0x03    /**< Minimum CMD length of DESFIRE_ReadX command */

#define PHHAL_HW_SAMAV2_CMD_ULC_AUTH_PICC_INS                     0x2C    /**< CMD Byte for ULC_AuthenticatePICC command */
#define PHHAL_HW_SAMAV2_CMD_ULC_AUTH_PICC_MIN_DATA_LENGTH         0x02    /**< Minimum CMD length of ULC_AuthenticatePICC command */

#define PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_INS                 0x8E    /**< CMD Byte for IsoAuthenticatePICCPartOne command */
#define PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_MIN_DATA_LENGTH     0x02    /**< Minimum CMD length of IsoAuthenticatePICCPartOne command */

#define PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_INS                     0x0A    /**< CMD Byte for AuthenticatePICCPartOne command */
#define PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_MIN_DATA_LENGTH         0x02    /**< Minimum CMD length of AuthenticatePICCPartOne command */

#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_PICC_INS                       0xC4    /**< CMD Byte for AuthenticatePICCPartOne command */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MIN_DATA_LENGTH                0x04    /**< Minimum CMD length of AuthenticatePICCPartOne command */

#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_INS                        0x35    /**< CMD Byte for ChangeKeyMFP command */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_CMD_MIN_DATA_LENGTH        0x05    /**< Minimum CMD length of ChangeKeyMFP command */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_COMMAND                    0x00    /**< data parameter is a command */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_RESPONSE                   0x01    /**< data parameter is a response */

/*************************************************************************************************************************/
/**************************************** SAM SECURITY *******************************************************************/
/*************************************************************************************************************************/

#define PHHAL_HW_SAMAV2_CMD_DISABLE_CRYPTO                    0xDC
#define PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_HOST_INS             0xA4    /**< CMD Byte for SAM_AuthenticateHost command */
#define PHHAL_HW_SAMAV2_AUTH_FIRST_PART_MASK                  0x10    /**< Mask to specify first part of Authenticate Host */
#define PHHAL_HW_SAMAV2_AUTH_SECOND_PART_MASK                 0x20    /**< Mask to specify second part of Authenticate Host */
#define PHHAL_HW_SAMAV2_AUTH_THIRD_PART_MASK                  0x40    /**< Mask to specify third part of Authenticate Host */

#define PHHAL_HW_SAMAV2_CMD_SELECT_APPLICATION_INS            0x5A    /**< CMD Byte for SAM_SelectApplication command */
#define PHHAL_HW_SAMAV2_CMD_SELECT_APPLICATION_DATA_LENGTH    0x03    /**< data length for SAM_KillAuthentication command */
#define PHHAL_HW_SAMAV2_CMD_LOAD_INIT_VECTOR_INS                0x71    /**< CMD Byte for SAM_SelectApplication command */
#define PHHAL_HW_SAMAV2_CMD_LOAD_INIT_VECTOR_DATA_LENGTH_MIN  0x08    /**< Minimum data length for SAM_KillAuthentication command */
#define PHHAL_HW_SAMAV2_CMD_LOAD_INIT_VECTOR_DATA_LENGTH_MAX  0x10    /**< Maximum data  length for SAM_KillAuthentication command */
#define PHHAL_HW_SAMAV2_CMD_KILL_AUTHENTICATION_INS           0xCA    /**< CMD Byte for SAM_KillAuthentication command */
#define PHHAL_HW_SAMAV2_CMD_GET_VERSION_INS                   0x60    /**< CMD Byte for GetVersion command */
#define PHHAL_HW_SAMAV2_CMD_GET_VERSION_DATA_LENGTH           0x00    /**< CMD length of GetVersion command */
#define PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_LENGTH       0x1F    /**< Response length for GetVersion command */
#define PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_UID_OFFSET   0x0E    /**< Offset of first UID byte field in the Version field */
#define PHHAL_HW_SAMAV2_CMD_GET_VERSION_RESPONSE_HOSTMODE_POS 0x1E    /**< Offset of first UID byte field in the Version field */

#define PHHAL_HW_SAMAV2_CMD_SLEEP_INS                 0x51    /**< CMD Byte for Sleep command */
#define PHHAL_HW_SAMAV2_CMD_SLEEP_DATA_LENGTH         0x00    /**< CMD length of Sleep command */
#define PHHAL_HW_SAMAV2_CMD_SLEEP_RESPONSE_LENGTH     0x00    /**< Response length for Sleep command */

#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_INS                               0x10    /**< CMD Byte for LockUnlock command */
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_UNLOCK                         0x00    /**< P1 Byte for Unlock*/
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_LOCK_WO_SPECIFYING_UNLOCK_KEY  0x01    /**< P1 Byte for Lock command without specifying unlock key*/
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_LOCK_SPECIFYING_UNLOCK_KEY     0x02    /**< P1 Byte for Lock command with specifying unlock key*/
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_AV2_MODE_SWITCH                0x03    /**< P1 Byte for AV2 Mode Switch*/
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_MIN_DATA_LENGTH                   0x02    /**< Minimum CMD length of LockUnlock command */
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_MAX_DATA_LENGTH                   0x05    /**< Maximum CMD length of LockUnlock command */
#define PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_RESPONSE1_LENGTH                  0x0C    /**< Response length for LockUnlock command */
#define PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_INS                    0x82    /**< CMD Byte for SAM_IsoExternalAuthenticate command */
#define PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_MIN_DATA_LENGTH        0x02    /**< Minimum CMD length for SAM_IsoExternalAuthenticate command */
#define PHHAL_HW_SAMAV2_CMD_ISO_INTERNAL_AUTHENTICATE_INS                    0x88    /**< CMD Byte for SAM_IsoInternalAuthenticate command */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_OFFLINE_KEY_INS                        0x01    /**< CMD Byte for SAM_ActivateOfflineKey command */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_OFFLINE_KEY_MIN_DATA_LENGTH            0x02    /**< Minimum CMD length for SAM_ActivateOfflineKey command */

/*************************************************************************************************************************/
/**************************************** SAM KEY HANDLING ***************************************************************/
/*************************************************************************************************************************/

#define PHHAL_HW_SAMAV2_CMD_GET_KEYENTRY_INS          0x64U   /**< CMD Byte for GetKeyEntry command */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_INS       0xC1U   /**< CMD Byte for ChangeKeyEntry command */
#define PHHAL_HW_SAMAV2_CMD_UPDATE_KEYA_MASK          0x80U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_KEYB_MASK          0x40U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_KEYC_MASK          0x20U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_DFAID_MASK         0x10U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_KEYCEK_MASK        0x08U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_REFNOKUC_MASK      0x04U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_SET_MASK           0x02U
#define PHHAL_HW_SAMAV2_CMD_SENT_KEY_VERSIONS_MASK    0x01U

#define PHHAL_HW_SAMAV2_CMD_AV1_KEY_ENTRY_RESPONSE_NO_VC_LENGTH   0x0BU   /**< Response in AV1 Mode length if VC is not available */
#define PHHAL_HW_SAMAV2_CMD_AV1_KEY_ENTRY_RESPONSE_VC_LENGTH      0x0CU   /**< Response in AV1 Mode length if VC is available */

#define PHHAL_HW_SAMAV2_CMD_AV2_KEY_ENTRY_RESPONSE_NO_VC_LENGTH   0x0CU   /**< Response in AV2 Mode length if VC is not available */
#define PHHAL_HW_SAMAV2_CMD_AV2_KEY_ENTRY_RESPONSE_VC_LENGTH      0x0DU   /**< Response in AV2 Mode length if VC is available */

#define PHHAL_HW_SAMAV2_CMD_AV2_KEY_ENTRY_SIZE      0x0DU

#define PHHAL_HW_SAMAV2_CMD_KEY_C_VALID_OFFSET        0x04U
#define PHHAL_HW_SAMAV2_CMD_DF_AID_OFFSET            0x05U

#define PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_INS       0xCCU
#define PHHAL_HW_SAMAV2_CMD_GET_KUCENTRY_INS          0x6CU
#define PHHAL_HW_SAMAV2_CMD_UPDATE_LIMIT_MASK         0x80U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_KEYNO_CKUC_MASK    0x40U
#define PHHAL_HW_SAMAV2_CMD_UPDATE_KEY_VCKUC_MASK     0x20U

#define PHHAL_HW_SAMAV2_CMD_DUMP_SESSIONKEY_INS     0xD5U   /**< CMD Byte for DumpSessionKey command */
#define PHHAL_HW_SAMAV2_CMD_DUMP_SECRETKEY_INS        0xD6U   /**< CMD Byte for DumpSecretKey command */
#define PHHAL_HW_SAMAV2_CMD_DISABLE_KEYENTRY_INS    0xD8U   /**< CMD Byte for DisableKeyEntry command */

/*************************************************************************************************************************/
/**************************************** SAM DATA PROCESS ***************************************************************/
/*************************************************************************************************************************/

#define PHHAL_HW_SAMAV2_CMD_VERIFY_MAC_INS                0x5C    /**< CMD Byte for SAM_VerifyMac command */
#define PHHAL_HW_SAMAV2_CMD_GENERATE_MAC_INS            0x7C    /**< CMD Byte for SAM_GenerateMac command */
#define PHHAL_HW_SAMAV2_CMD_ENCIPHER_DATA_INS            0xED    /**< CMD Byte for SAM_EncipherData command */
#define PHHAL_HW_SAMAV2_CMD_DECIPHER_DATA_INS            0xDD    /**< CMD Byte for SAM_DecipherData command */
#define PHHAL_HW_SAMAV2_CMD_ENCIPHER_OFFLINE_DATA_INS    0x0E    /**< CMD Byte for SAM_EncipherOfflineData command */
#define PHHAL_HW_SAMAV2_CMD_DECIPHER_OFFLINE_DATA_INS   0x0D    /**< CMD Byte for SAM_DecipherOfflineData command */


/*************************************************************************************************************************/
/****************************************NON X MODE: PKI *****************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/
#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_INS                    0x15    /**< CMD Byte for PKI_GenerateKeyPair command */
#define PHHAL_HW_SAMAV2_CMD_PKI_EXPORT_PRIVATE_KEY_INS                0x1F    /**< CMD Byte for PKI_ExportPrivateKey command */
#define PHHAL_HW_SAMAV2_CMD_PKI_EXPORT_PUBLIC_KEY_INS                    0x18    /**< CMD Byte for PKI_ExportPublicKey command */
#define PHHAL_HW_SAMAV2_CMD_PKI_SEND_SIGNATURE_INS                    0x1A    /**< CMD Byte for PKI_SendSignature command */
#define PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_INS                0x1D    /**< CMD Byte for PKI_UpdateKeyEntries command */
#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_SIGNATURE_INS                0x16    /**< CMD Byte for PKI_GenerateSignature command */
#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_INS                        0x17    /**< CMD Byte for PKI_GenerateHash command */
#define PHHAL_HW_SAMAV2_CMD_PKI_VERIFY_SIGNATURE_INS                    0x1B    /**< CMD Byte for PKI_VerifySignature command */
#define PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_INS                        0x19    /**< CMD Byte for PKI_ImportKey command */

#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_LC_MIN                0x0A    /**< Minimun Length of data of PKI_GenerateKeyPair */
#define PHHAL_HW_SAMAV2_CMD_PKI_SEND_SIGNATURE_LE_VALUE                0x00    /**< Length of data(LE) for the response of PKI_SendSignature */
#define PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_LC_MIN                0x02    /**< Minimun Length of data of PKI_UpdateKeyEntries */
#define PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_LC_MAX                0xC0    /**< Maximun Length of data of PKI_UpdateKeyEntries */
#define PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_LC_MIN                        0x0E    /**< Minimun Length of data of PKI_ImportKey */

#define PHHAL_HW_SAMAV2_CMD_PKI_ONE_KEY_ENTRY                            0x01    /**< One key entry */
#define PHHAL_HW_SAMAV2_CMD_PKI_TWO_KEY_ENTRIES                        0x02    /**< Two key entry */
#define PHHAL_HW_SAMAV2_CMD_PKI_THREE_KEY_ENTRIES                        0x03    /**< Three key entry */

#define PHHAL_HW_SAMAV2_CMD_HASH_ALGO_SHA_1                            0x00    /**< SHA-1 hashing algorithm */
#define PHHAL_HW_SAMAV2_CMD_HASH_ALGO_SHA_224                            0x01    /**< SHA-224 hashing algorithm */
#define PHHAL_HW_SAMAV2_CMD_HASH_ALGO_SHA_256                            0x02    /**< SHA-256 hashing algorithm */
#define PHHAL_HW_SAMAV2_CMD_BIG_ENDIAN                                0x00    /**< Big Endian */
#define PHHAL_HW_SAMAV2_CMD_LITTLE_ENDIAN                                0x01    /**< Little Endian */
#define PHHAL_HW_SAMAV2_CMD_HASH_MESSAGE_LENGTH_20                    0x14    /**< 20-bytes long hashed message */
#define PHHAL_HW_SAMAV2_CMD_HASH_MESSAGE_LENGTH_28                    0x1C    /**< 28-bytes long hashed message */
#define PHHAL_HW_SAMAV2_CMD_HASH_MESSAGE_LENGTH_32                    0x20    /**< 32-bytes long hashed message */

/*************************************************************************************************************************/
/****************************************NON X MODE: MIFARE PLUS **********************************************************/
/*************************************************************************************************************************/

#define PHHAL_HW_SAMAV2_NO_KDF 0x00
#define PHHAL_HW_SAMAV2_KDFSL2 0x01
#define PHHAL_HW_SAMAV2_KDFSL3 0x03

/*************************************************************************************************************************/
/****************************************X MODE: MFRC52 ******************************************************************/
/*************************************************************************************************************************/

#define PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL                          0xCF    /**< CMD Byte for RC_RFControl command */
#define PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL_DATA_LENGTH              0x02    /**< CMD length of RC_RFControl command */
#define PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL_RESPONSE_LENGTH          0x00    /**< Response length for RC_RFControl command */

#define PHHAL_HW_SAMAV2_CMD_RC_READREGISTER                       0xEE    /**< CMD Byte for RC_ReadRegister command */
#define PHHAL_HW_SAMAV2_CMD_RC_READREGISTER_MAX_DATA_LENGTH       0xFF    /**< CMD length of RC_ReadRegister command */
#define PHHAL_HW_SAMAV2_CMD_RC_READREGISTER_RESPONSE_LENGTH       0xFF    /**< Response length for RC_ReadRegister command */

#define PHHAL_HW_SAMAV2_CMD_RC_WRITEREGISTER                      0x1E    /**< CMD Byte for RC_WriteRegister command */
#define PHHAL_HW_SAMAV2_CMD_RC_WRITEREGISTER_MAX_DATA_LENGTH      0xFE    /**< CMD length of RC_WriteRegister command */
#define PHHAL_HW_SAMAV2_CMD_RC_WRITEREGISTER_RESPONSE_LENGTH      0x00    /**< Response length for RC_WriteRegister command */

#define PHHAL_HW_SAMAV2_CMD_RC_INIT                               0xE5    /**< CMD Byte for RC_Init command */
#define PHHAL_HW_SAMAV2_CMD_RC_INIT_RESPONSE_LENGTH               0x00    /**< Response length for RC_Init command */

#define PHHAL_HW_SAMAV2_CMD_RC_LOAD_REG_VALUE_SET                 0x2E    /**< CMD Byte for RC_LoadRegisterVlaueSet command */
#define PHHAL_HW_SAMAV2_CMD_RC_LOAD_REG_VALUE_SET_MAX_DATA_LENGTH 0x3E    /**< CMD length of RC_WriteRegister command */
#define PHHAL_HW_SAMAV2_CMD_RC_LOAD_REG_VALUE_SET_RESPONSE_LENGTH 0x00    /**< Response length for RC_WriteRegister command */

/*************************************************************************************************************************/
/******************************************** ISO3A LAYER ****************************************************************/
/*************************************************************************************************************************/
#define PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS          0x7E   /** < ISO 14443-3 Transparent Exchange command */

#define PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_INS                0x25    /**< ISO 14443-3 Request_Wakeup command */
#define PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_RESPONSE_LENGTH    0x02    /**< 2 byte ATQA */
#define PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_CMD_LC             0x01    /**< length of data; fixed to 01h */
#define PHHAL_HW_SAMAV2_CMD_HALTA_INS                         0x50    /** < ISO 14443-3 HaltA command */
#define PHHAL_HW_SAMAV2_CMD_AKTIVATE_WAKEUP_INS               0x52    /** < ISO 14443-3 Aktivate WakeUp command */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_IDLE_INS                 0x26    /** < ISO14443-3_ActivateIdle command */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_IDLE_CMD_LENGTH          0x0A    /** < Max length of the activate idle command */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_IDLE_ATQA_FILTER_MASK    0x01    /** < Flag in the filter settings which enables the ATQA filter */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_IDLE_SAK_FILTER_MASK     0x02    /** < Flag in the filter settings which enables the SAK filter */
#define PHHAL_HW_SAMAV2_CMD_ACTIVATE_RESPONSE_MAX_LENGTH      0xC5    /** < Max length of the activate idle command without SW1SW2*/
#define PHHAL_HW_SAMAV2_CMD_MAX_UID_LENGTH                    0x0A    /** < Max length of the UID */

#define PHHAL_HW_SAMAV2_CMD_ANTICOLLSION_SELECT_INS           0x93    /**< ISO 14443-3 Anticollision_Select command */
#define PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_RESPONSE_LENGTH    0x02    /**< 2 byte ATQA */
#define PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_CMD_LC             0x01    /**< length of data; fixed to 01h */

#define PHHAL_HW_SAMAV2_CMD_INIT_DATA_LENGTH_MAX              0x05    /**< length of init data; fixed to 05h */
#define PHHAL_HW_SAMAV2_CMD_INIT_INS                          0x11    /** < ISO14443-4_Init command */
#define PHHAL_HW_SAMAV2_CMD_DESELECT_INS                      0xD4    /** < ISO14443-4_Init command */
#define PHHAL_HW_SAMAV2_CMD_AUTO_CID                          0xFF    /** < the SAM-X will automatically assign a CID to the card */
#define PHHAL_HW_SAMAV2_CMD_CIDS_TO_FREE_MAX                  0x0E    /** < Max Number CID(s) to free */
#define PHHAL_HW_SAMAV2_CMD_FREE_CID_INS                      0xFC    /**<  ISO14443-4_FreeCID command */
#define PHHAL_HW_SAMAV2_CMD_EMPTY_CID                         0x00    /**<  ISO14443-4_ do not FreeCID  */
#define PHHAL_HW_SAMAV2_CMD_RATS_PPS_DATA_LENGTH              0x03    /** < RATS_PPS command data CID, DRI, DSI */
#define PHHAL_HW_SAMAV2_CMD_RATS_PPS_RESPONSE_DATA            0x04    /** < RATS_PPS response data CID, DRI, DSI, ATS */
#define PHHAL_HW_SAMAV2_CMD_RATS_PPS_INS                      0xE0    /**<  ISO14443-4_RATS_PPS command */
#define PHHAL_HW_SAMAV2_CMD_EXCHANGE_INS                      0xEC    /**<  ISO14443-4_Exchange command */
#define PHHAL_HW_SAMAV2_CMD_PRESENCE_CHECK_INS                0x4C    /**<  ISO14443-4_Exchange command */
#define PHHAL_HW_SAMAV2_CMD_DRI_DSI_SPEED_106                 0x00    /**<  DRI, DSI coding and resulting bit rate 106kbit/s */
#define PHHAL_HW_SAMAV2_CMD_DRI_DSI_SPEED_212                 0x01    /**<  DRI, DSI coding and resulting bit rate 212kbit/s */
#define PHHAL_HW_SAMAV2_CMD_DRI_DSI_SPEED_424                 0x02    /**<  DRI, DSI coding and resulting bit rate 424kbit/s */
#define PHHAL_HW_SAMAV2_CMD_DRI_DSI_SPEED_848                 0x03    /**<  DRI, DSI coding and resulting bit rate 848kbit/s */
#define PHHAL_HW_SAMAV2_CMD_ATS_MAX_LENGTH                    0xFF    /**<  Max length of the ATS array */
#define PHHAL_HW_SAMAV2_CMD_FREE_CID                          0xFF    /**<  Take next free CID */

#define PHHAL_HW_SAMAV2_RATS_PPS_DATA_LENGTH                  0x03    /** < RATS_PPS command data CID, DRI, DSI */
#define PHHAL_HW_SAMAV2_RATS_PPS_RESPONSE_DATA                0x03    /** < RATS_PPS response data CID, DRI, DSI */
#define PHHAL_HW_SAMAV2_RATS_PPS_INS                          0xE0    /**<  ISO14443-4_RATS_PPS command */
#define PHHAL_HW_SAMAV2_ATS_MAX_LENGTH                        0xFF
/*@}*/

/** @} */

/** \defgroup phhalHw_SamAV2_Cmd_Security SAM Security
* \brief SAM commands used for host communication and security related configuration. 
* @{
*/

/**
* \brief Disable cryto-related features of the SAM PERMANENTLY AND IRREVERSIBLY
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_DisableCrypto(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wProMas                            /**< [In] Two byte mask to specify the desired settings for cryptography-related features. */
    );

/**
* \brief Lock or Unlock the SAM
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_LockUnlock(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bMode,                              /**< [In] Mode for lock/unlock.*/
    uint16_t wRdKeyNo,                          /**< [In] Key reference number for readerLib key storage. */
    uint16_t wRdKeyV,                           /**< [In] Key version for readerLib key storage. */
    uint8_t bSamKeyNo,                          /**< [In] Key reference number to be used. */
    uint8_t bSamKeyV,                           /**< [In] Key version to be used. */
    uint8_t bUnlockKeyNo,                       /**< [In] Unlock Key Number to be used (only used when bMode = 0x01).*/
    uint8_t bUnlockKeyV,                        /**< [In] Unlock Key Version to be used (only used when bMode = 0x01).*/
    uint32_t bMaxChainBlocks                    /**< [In] Maximal message size under command chaining in MAC or Full Protection (only used during switch from AV1 to AV2).*/
    );

/**
* \brief Mutual 2-pass-AV1 or 3-pass-AV2 authentication between Host and SAM.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateHost(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bAuthType_HostMode,                 /**< [In] Authentication mode in AV1 or host mode AV2. */
    uint16_t wRdKeyNo,                          /**< [In] Key reference number for readerLib key storage. */
    uint16_t wRdKeyV,                           /**< [In] Key version for readerLib key storage. */
    uint8_t bSamKeyNo,                          /**< [In] Key reference number to be used. */
    uint8_t bSamKeyV,                           /**< [In] Key version to be used. */
    uint8_t * pDivInput,                        /**< [In] Diversification input data (only used in AV1). */
    uint8_t bDivInputLength                     /**< [In] Diversification input data length (only used in AV1). */
    );

/**
* \brief Activation of an OfflineCrypto or an OfflineChange Key. This command is only available in AV2 mode.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ActivateOfflineKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bKeyNo,                             /**< [In] Key reference number to be used. */
    uint8_t bKeyV,                              /**< [In] Key version to be used. */
    uint8_t * pDivInput,                        /**< [In] Diversification input data. */
    uint8_t bDivInputLength                      /**< [In] Diversification input data length. If set to 00h no diversification is done. */
    );

/**
* \brief Load an Init Vector for the next cryptographic operation into the SAM.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_LoadInitVector(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pInitVector,                      /**< [In] Init Vector. */
    uint8_t bInitVectorLength                   /**< [In] Init Vector length. */
    );

#define PHHAL_HW_SAMAV2_CMD_CHANGE_SAMKILLAUTHENTICATION_DEFAULT    0x00    /**<  Default option mask for SAM_KillAuthentication - kill all valid authentication in the LC. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_SAMKILLAUTHENTICATION_PICC_ONLY  0x01    /**<  Option mask for kill only PICC authentication. Only valid in AV2. */

/**
* \brief Kill all active authentications in this logical channel.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption                             /**< [In] Only valid in SamAV2 mode. If set to 01 only an active PICC Authentication or OfflineKey Activation is killed. */
    );

/**
* \brief Select an application by the DF_AID
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_SelectApplication(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pDF_Aid                           /**< [In] DESFire application ID. This buffer has to be 3 bytes long. */
    );

/**
* \brief Executes the first part of an ISO compliant authentication procedure (AV1) or fetches a random number from the SAM (AV2).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_GetChallenge(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t    bExpL,                           /**< [In] Expected length of data bytes to be retrieved. */
    uint8_t  * pRnd                             /**< [Out] Buffer containing the challenge. It has to be bExpL long. */
    );

/**
* \brief Executes the second part of an ISO compliant authentication procedure.
*
* \b Note: This command is only available in AV1 mode.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoExternalAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bKeyNo,                             /**< [In] Key reference number to be used. */
    uint8_t bKeyV,                              /**< [In] Key version to be used. */
    uint8_t * pEncRnd,                          /**< [In] Encrypted (Random number PCD1 || Random number PICC1). */
    uint8_t bEncRndLength                       /**< [In] Encrypted data length. */
    );

/**
* \brief Executes the third part of an ISO compliant authentication procedure.
*
* After successfull execution of this command, the Session Key may be generated
* as described in the SAMAV2 specification.\n
* The Session Key may then be loaded using the command \ref phCryptoSym_LoadKeyDirect
* together with \ref phhalHw_SamAV2_DataParams_t::pENCCryptoDataParams.\n
* \b Note: This command is only available in AV1 mode.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoInternalAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pRPcd2,                           /**< [In] Random number PCD2. */
    uint8_t bRPcd2Length,                       /**< [In] Length of RPCD2. */
    uint8_t ** pEncResp,                        /**< [Out] Encrypted (Random number PICC2 || Random number PCD2). */
    uint16_t * pEncRespLength                   /**< [Out] Length of encrypted response. */
    );

/**
* \brief Get version information from the SAM.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_GetVersion(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pVersion,                         /**< [Out] Buffer containing the read version. It has to be 31 bytes long. */
    uint8_t * pVersionLength                    /**< [Out] Amount of valid bytes in the version buffer. */
    );

/**
* \brief Set the SAM into power down mode.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_Sleep(
                                        phhalHw_SamAV2_DataParams_t * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                                        );

/** @} */

/** \defgroup phhalHw_SamAV2_Cmd_KeyMng Key Management
* \brief SAM commands used for managing all symmetric keys.
* @{
*/

#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_DEFAULT     0x00    /**<  Default option mask for ChangeKeyEntry. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN       0x01    /**<  Option mask for a plain ChangeKeyEntry in AV1. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_ENCRYPTED   0x00    /**<  Option mask for a encrypted ChangeKeyEntry in AV1. */

/**
* \brief Change a symmetric key entry in the key table of the SAM.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption,                            /**< [In] Option mask for this command - only used in AV1. \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN is set, the key data is sent in plain to the SAM. */
                                                uint8_t bKeyNo,     /**< [In] Reference number of the key entry to be changed (00h to 7Fh). */
                                                uint8_t bProMas,    /**< [In] Program mask indicating the fields that should be changed. */
                                                uint8_t * pKeyData, /**< [In] Buffer containing the key data:
                                                                    *          \arg AV1 and host authentication with session key: key data = (plain key entry); Flag #PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN in option byte has to be cleared. 
                                                                    *          \arg AV1 and host authentication with secret key: key data = (plain key entry | UID | CRC | padding) encrypted with secret key; option byte is not used. 
                                                                    *          \arg AV1 and KeyNoCEK is set to FEh: key data = (plain key entry | UID); Flag #PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN in option byte has to be set. 
                                                                    *          \arg AV2 and host authentication: key data = (plain key entry). 
                                                                    *          \arg AV2 and offline key activation: key data = (offline key data). 
                                                                    *          \arg AV2 and KeyNoCEK is set to FEh: key data = (plain key entry). */
                                                                    uint8_t bKeyDataLength  /**< [In] Length of the key data buffer. */
                                                                    );

/**
* \brief Get information about a key entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */                                    
    uint8_t bKeyNo,                             /**< [In] Reference number of the key entry to be returned (00h to 7Fh). */
    uint8_t * pKeyEntry,                        /**< [Out] Buffer containing the information about the key entry. This buffer has to be 13 bytes long. */
    uint8_t * bKeyEntryLength                   /**< [Out] Amount of valid bytes in pKeyEntry. */
    );

#define PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_DEFAULT     0x00    /**<  Default option mask for ChangeKUCEntry. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_PLAIN       0x01    /**<  Option mask for a plain ChangeKUCEntry in AV1. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_ENCRYPTED   0x00    /**<  Option mask for a encrypted ChangeKUCEntry in AV1. */

/**
* \brief Change the key usage counter (KUC). Selection is done by its reference number.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKUCEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption,                            /**< [In] Option mask for this command - only used in AV1. \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_PLAIN is set, the KUC data is sent in plain to the SAM. */
                                                uint8_t bKucNo,     /**< [In] Reference number of the key usage counter to be updated (00h to 0Fh). */
                                                uint8_t bProMas,    /**< [In] Program mask indicating the fields that should be changed. */
                                                uint8_t * pKucData, /**< [In] Buffer containing the KUC data:
                                                                    *          \arg AV1 and host authentication with session key: KUC data = (plain KUC entry); Flag #PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_PLAIN in option byte has to be cleared. 
                                                                    *          \arg AV1 and host authentication with secret key: KUC data = (plain KUC entry | UID | CRC | padding) encrypted with secret key; bSkipEncryption is not used. 
                                                                    *          \arg AV1 and KeyNoCKUC is set to FEh: KUC data = (plain KUC entry | UID); Flag #PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_PLAIN in option byte has to be set. 
                                                                    *          \arg AV2 and host authentication: KUC data = (plain KUC entry). 
                                                                    *          \arg AV2 and offline key activation: KUC data = (offline KUC data). 
                                                                    *          \arg AV2 and KeyNoCKUC is set to FEh: KUC data = (plain KUC entry). */
                                                                    uint8_t KucDataLength   /**< [In] Length of the KUC data. */
                                                                    );

/**
* \brief Get information about a key usage counter (KUC).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_GetKUCEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bKucNo,                             /**< [In] Reference number of the key usage counter to be returned (00h to 0Fh). */ 
    uint8_t * pKucEntry                         /**< [Out] Buffer containing the KUC entry. This buffer has to be 10 bytes long. */
    );

/**
* \brief Retrive a PICC key stored in the key table. This command is only available in AV2.
* The pSecretKey buffer contains always the plain session key even if bUseCrypto is set.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_DumpSecretKey (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bCrypto,                            /**< [In] Bit mask containing the crypto options for the key dump. */
    uint8_t bKeyNo,                             /**< [In] Reference number of the key entry to be dumped. */
    uint8_t bKeyV,                              /**< [In] Reference number of the key entry to be dumped. */
    uint8_t * pDivInput,                        /**< [In] Diversification input data. */
    uint8_t bDivInputLength,                    /**< [In] Diversification input data length. If set to 00h no diversification is used. */
    uint8_t * pSecretKey,                       /**< [Out] Buffer containing the plain secret key. It has to be 24 bytes long. */
    uint8_t * pSecretKeyLength                  /**< [Out] Amount of valid bytes in secret key buffer. */
    );

/**
* \brief Dump the current session key.
* In AV2 the pSessionKey buffer contains always the plain session key.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_DumpSessionKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
    phhalHw_SamAV2_DataParams_t * pDataParamsEnc,   /**< [In] Pointer to the layer's parameter structure whose session key shall be used for response encryption. Only used in AV1 whenever encryption is selected. */    
    uint8_t bCrypto,                                /**< [In] Crypto settings. 
                                                    *        \arg AV1: bit 0 indicates if the key shall be encrypted or not.
                                                    *        \arg AV1: bit 1-2 indicates the LC number whose current session key shall be used for encryption.
                                                    *        \arg AV2: bit 0 indicates if the key shall be encrypted under MAC protection. */
                                                    uint8_t * pSessionKey,      /**< [Out] Buffer containig the session key. It has to be 24 bytes long. */
                                                    uint8_t * pSessionKeyLength /**< [Out] Amount of valid bytes in session key buffer. */
                                                    );

/**
* \brief Disable a key entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_DisableKeyEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bKeyNo,                             /**< [In] Number of the key entry to be disabled (00h to 7Fh). */
    uint8_t * pOfflineCryptogram,               /**< [Out] Buffer containing the cryptogram for offline key deactivation. This parameter is only used in AV2. */
    uint8_t bCryptogramLength                   /**< [Out] Length of the offline cryptogram. If set to 00h no offline cryptogram is sent. */
    );

/** @} */

/** \defgroup phhalHw_SamAV2_Cmd_DataProcessing Data Processing
* \brief SAM commands used for cryptographic data processing. This includes encryption, decryption, MAC generation and MAC verification.
* @{
*/

/**
* \brief Check the validity of the MAC over given data.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *            Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                *            On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. \n
                                                *            The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. */
                                                uint8_t bNum,           /**< [In] Number of MAC bytes to check in the plain data buffer. */
                                                uint8_t * pTxBuffer,    /**< [In] Plain data including the MAC to be checked. */
                                                uint8_t bTxLength       /**< [In] Length of input data. */
                                                );

/**
* \brief Generate a MAC over given data.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *            Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *            On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *            The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. */
                                                uint8_t bNum,           /**< [In] Number of MAC bytes to generate. */
                                                uint8_t * pTxBuffer,    /**< [In] Plain data to be MACed. */
                                                uint8_t bTxLength,      /**< [In] Length of input data. */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to plain MACed data. */
                                                uint16_t * pRxLength    /**< [Out] Length of plain MACed data. */
                                                );


#define PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH  0x04    /**< Option mask for a decipher with given input length. */

/**
* \brief Decipher a data stream.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_DecipherData(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *            Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *            On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *            The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. \n 
                                                *            If flag #PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH is set, 
                                                *            the length in pLenEncInputData is sent in front of the data in pTxBuffer. */
                                                uint8_t * pTxBuffer,        /**< [In] Data to be deciphered. */
                                                uint8_t bTxLength,          /**< [In] Length of input data. */
                                                uint8_t * pLenEncInputData, /**< [In] Overall length of encrypted input data. This 3 byte value is only used if indicated by wOption. */
                                                uint8_t ** ppRxBuffer,      /**< [Out] Pointer to the deciphered data. */
                                                uint16_t * pRxLength        /**< [Out] Length of deciphered data. */
                                                );

/**
* \brief Encipher a data stream.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_EncipherData(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *          Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *          On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *          The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. */
                                                uint8_t * pTxBuffer,    /**< [In] Data to be enciphered. */
                                                uint8_t bTxLength,      /**< [In] Length of input data. */
                                                uint8_t bOffset,        /**< [In] Offset into the input data indicating the first data byte to be enciphered. */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to enciphered data. */
                                                uint16_t * pRxLength    /**< [Out] Length of enciphered data. */
                                                );

/**
* \brief Decipher a data stream using an OfflineCrypto key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_DecipherOfflineData(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *            Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *            On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *            The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. */
                                                uint8_t * pTxBuffer,    /**< [In] Data to be deciphered. */
                                                uint8_t bTxLength,      /**< [In] Length of input data. */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to deciphered data. */
                                                uint16_t * pRxLength    /**< [Out] Length of deciphered data. */
                                                );

/**
* \brief Encipher a data stream using an OfflineCrypto key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_EncipherOfflineData(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *          Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *          On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *          The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. */
                                                uint8_t * pTxBuffer,    /**< [In] Data to be enciphered. */
                                                uint8_t bTxLength,      /**< [In] Length of input data */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to enciphered data */
                                                uint16_t * pRxLength    /**< [Out] Length of enciphered data. */
                                                );

/** @} */

/** \defgroup phhalHw_SamAV2_Cmd_PKI Public Key Infrastructure
* \brief SAM commands used for asymmetric key management, signature handling and symmetric key updates based on PKI.
* @{
*/

#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_DEFAULT   0x00    /**< Default option mask for PKI_GenerateKeyPair. */
#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_FIXED_E   0x01    /**< Option mask for a key generation with a given exponent e. */
#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_RANDOM_E  0x00    /**< Option mask for a key generation with a randomly selected exponent e. */

/**
* \brief Create an RSA key pair. This command is only available in AV2.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_GenerateKeyPair(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bOption,                            /**< [In] Option for key generation: \n
                                                *            if flag #PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_FIXED_E the value in pPKI_e is used for the exponent e,
                                                *            otherwhise e is randomly choosen. */
                                                uint8_t bPKI_KeyNo,     /**< [In] Reference number of the key entry where the created key should be stored (00h to 01h). */
                                                uint16_t wPKI_Set,      /**< [In] Configuration settings of the created key entry. */
                                                uint8_t bPKI_KeyNoCEK,  /**< [In] Reference number to the change key of the created key entry. */
                                                uint8_t bPKI_KeyVCEK,   /**< [In] Version of the change key of the created key entry. */
                                                uint8_t bPKI_RefNoKUC,  /**< [In] Reference number to the KUC of the created key entry. */
                                                uint16_t wPKI_NLen,     /**< [In] Length of the modulus N (multiple of 8 and in [32;256]). */
                                                uint16_t wPKI_eLen,     /**< [In] Length of the exponent e (multiple of 4 and in [4;256]). */
                                                uint8_t * pPKI_e        /**< [In] Buffer containing the fixed exponent e. */
                                                );

/**
* \brief Export the public and the private part of an RSA key pair. This command is only available in AV2.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful, command completed.
* \retval #PH_ERR_SUCCESS_CHAINING Operation successful, chaining ongoing.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_ExportPrivateKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *         If set to #PH_EXCHANGE_RXCHAINING only dummy commands are sent to fetch the remaining key data. */
                                                uint8_t bPKI_KeyNo,     /**< [In] Reference number of the key entry to be exported (00h to 01h). */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data */
                                                uint16_t * pRxLength    /**< [Out] Length of received data. */
                                                );

/**
* \brief Export the public part of an RSA key pair. This command is only available in AV2.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful, command completed.
* \retval #PH_ERR_SUCCESS_CHAINING Operation successful, chaining ongoing.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_ExportPublicKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *         If set to #PH_EXCHANGE_RXCHAINING only dummy commands are sent to fetch the remaining key data. */
                                                uint8_t bPKI_KeyNo,     /**< [In] Reference number of the key entry to be exported (00h to 02h). */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data */
                                                uint16_t * pRxLength    /**< [Out] Length of received data. */
                                                );

/**
* \brief Get a previously generated signature. This command is only available in AV2.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_SendSignature(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t ** ppRxBuffer,                      /**< [Out] Pointer to received data */
    uint16_t * pRxLength                        /**< [Out] Length of received data. */
    );

/**
* \brief Change up to 3 symmetric key entries by using PKI.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_UpdateKeyEntries(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bNoOfKeyEntries,                    /**< [In] Number of symmetric key entries to update. */
    uint8_t bHashingAlg,                        /**< [In] Hashing algorithm to be used for signature verification (00:SHA-1, 01:SHA-224, 11: SHA-256, 10:RFU). */
    uint8_t bPKI_KeyNo_Enc,                     /**< [In] Reference number of the private RSA key entry to be used for decryption (00h to 01h). */
    uint8_t bPKI_KeyNo_Sign,                    /**< [In] Reference number of the public key entry to be used for signature verification (00h to 02h). */
    uint8_t * pKeyFrame,                        /**< [In] Buffer containing the RSA encrypted key entries and the signature. */
    uint16_t wKeyFrameLength                    /**< [In] Key frame length. */
    );

/**
* \brief Generate a signature with a given RSA key entry.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_GenerateSignature(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bHashingAlg,                        /**< [In] Hashing algorithm to be used for signature generation and padding (00:SHA-1, 01:SHA-224, 11: SHA-256, 10:RFU). */
    uint8_t bPKI_KeyNo,                         /**< [In] Number of the key entry to be used for signing (00h to 01h). */
    uint8_t * pHashMsgData,                     /**< [In] Hash message to be signed. */
    uint8_t    bHashMsgLength                   /**< [In] Hash message length. */
    );

#define PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_OPTION_FIRST_FRAME 0x04    /**< Option mask for a decipher with given input length. */

/**
* \brief Generate Hash dataframe from Data
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful, command completed.
* \retval #PH_ERR_SUCCESS_CHAINING Operation successful, chaining ongoing.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_GenerateHash(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *         Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *         On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *         The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. \n
                                                *         On the first frame of the command chain the flag #PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_OPTION_FIRST_FRAME has to be set \n
                                                *         to force the appending of the length of the overall message. */
                                                uint8_t bHashingAlg,    /**< [In] Hashing algorithm selection (00:SHA-1, 01:SHA-224, 11: SHA-256, 10:RFU). */
                                                uint32_t dwMsgLength,   /**< [In] Overall message length. */
                                                uint8_t * pTxBuffer,    /**< [In] Message chunk to be hashed. */
                                                uint16_t wTxLength,     /**< [In] Length of message chunk. */
                                                uint8_t * pHash,        /**< [Out] Buffer containing the hash after sending the last message chunk. The buffer has to be 32 bytes long. */
                                                uint8_t * pHashLength   /**< [Out] Amount of valid data in hash buffer. */
                                                );

/**
* \brief Verify a hash / signature pair with a given RSA key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_VerifySignature(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bPKI_KeyNo,                         /**< [In] Number of the key entry to be used for verification (00h to 02h). */
    uint8_t bHashingAlg,                        /**< [In] Hashing algorithm selection (00:SHA-1, 01:SHA-224, 11: SHA-256, 10:RFU). */
    uint8_t * pHashData,                        /**< [In] Hash data. */
    uint8_t bHashDataLength,                    /**< [In] Hash data length. */
    uint8_t * pSignature,                       /**< [In] RSA digital signature. */
    uint16_t wSignatureLength                   /**< [In] RSA digital signature length. */
    );

#define PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_DEFAULT          0x00    /**<  Default option mask for PKI_ImportKey. */
#define PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_SETTINGS_ONLY    0x01    /**<  Option mask for importing the key settings only. */
#define PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_KEY_ENTRY        0x00    /**<  Option mask for importing a complete key entry. */

/**
* \brief Import a public or private RSA key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_PKI_ImportKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption,                            /**< [In] Option parameter: \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_SETTINGS_ONLY only the key settings are imported. */
                                                uint8_t bPKI_KeyNo,     /**< [In] Number of the key entry to be imported (00h to 02h). */
                                                uint16_t wPKI_Set,      /**< [In] Configuration settings of the imported key entry. It indicates wheter a private of public key shall be imported.  */
                                                uint8_t bPKI_KeyNoCEK,  /**< [In] Reference number to the change key of the imported key entry. */
                                                uint8_t bPKI_KeyVCEK,   /**< [In] Version of the change key of the imported key entry. */
                                                uint8_t bPKI_RefNoKUC,  /**< [In] Reference number to the KUC of the created key entry. */
                                                uint16_t wPKI_NLen,     /**< [In] Length of Modulus N (multiple of 8 and in [32;256]). */
                                                uint16_t wPKI_eLen,     /**< [In] Length of exponent e (multiple of 4 and in [4;256]). */
                                                uint16_t wPKI_PLen,     /**< [In] Length of prime P. Only used if bPublicOnly = 00h. */
                                                uint16_t wPKI_QLen,     /**< [In] Length of prime Q. Only used if bPublicOnly = 00h. */
                                                uint8_t * pPKI_N,       /**< [In] Modulus N. */
                                                uint8_t * pPKI_e,       /**< [In] Exponent e. */
                                                uint8_t * pPKI_P,       /**< [In] Prime P. */
                                                uint8_t * pPKI_Q,       /**< [In] Prime Q. */
                                                uint8_t * pPKI_dP,      /**< [In] Parameter dP padded up to a length of wPKI_PLen. */
                                                uint8_t * pPKI_dQ,      /**< [In] Parameter dQ padded up to a length of wPKI_QLen. */
                                                uint8_t * pPKI_iPQ      /**< [In] Inverse P(-1) mod Q padded up to a length of wPKI_QLen. */
                                                );

/** @} */

/** \defgroup phhalHw_SamAV2_NonX_MFP MFP in Non-X-Mode
* \see phhalHw_SamAV2_Cmd_DataProcessing
* \brief SAM commands used for MIFARE Plus card communication in Non-X-Mode.
* @{
*/

#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_AUTH_FIRST     0x01    /**<  Option mask for a first MFP Authentication. */
#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_MF_ENCIPHERED  0x02    /**<  Option mask for a MFP Authentication after a MIFARE Authentication. */
#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_NO_KDF         0x00    /**<  Option mask for a MFP Authentication with no KDF. */
#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL2_KDF        0x04    /**<  Option mask for a MFP Authentication with SL2 KDF. */
#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL3_KDF        0x08    /**<  Option mask for a MFP Authentication with SL3 KDF. */

/**
* \brief First part of MFP Authenticate.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMfp_Part1 (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bOption,                            /**< [In] Option for Authenticate MFP: \n
                                                *          If flag #PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_AUTH_FIRST a first authentication is done.
                                                *          Flag #PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_MF_ENCIPHERED has to be set on an active MIFARE Authentication. \n
                                                *          The key derivation function can be selected by setting the flags to #PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_NO_KDF,
                                                *          #PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL2_KDF or #PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL3_KDF. */
                                                uint8_t bKeyNo,             /**< [In] Key number to be used in authentication. */
                                                uint8_t bKeyV,              /**< [In] Key version to be used in authentication. */
                                                uint8_t * pRndB,            /**< [In] Encrypted RND B received from the card. This buffer has to be 22 bytes long in case of an active MFC authentication; in all other cases 16 bytes. */
                                                uint8_t * pDivInput,        /**< [In] Diversification input. */
                                                uint8_t  bDivInputLength,   /**< [In] Length of diversification input. If it is set to 00h no diversification is used. */    
                                                uint8_t * pRndARndB,        /**< [Out] Buffer containing encrypted RndA || RndB to be sent to the card. This buffer has to be 40 bytes long. */
                                                uint8_t * pRndARndBLength   /**< [Out] Amount of valid bytes in encrypted RNDA || RndB buffer. */
                                                );

/**
* \brief Second part of MFP Authenticate.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMfp_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */                                    
    uint8_t * pEncData,                         /**< [In] Encrypted RND A' including TI. */
    uint8_t bEncDataLength,                     /**< [In] Length of pEncData (16 or 32 bytes). */
    uint8_t * pPdCap,                           /**< [Out] Buffer containing PD Caps received from card. The buffer has to be 6 bytes long. */
    uint8_t * pPcdCap                           /**< [Out] Buffer containing PCD Caps received from card. The buffer has to be 6 bytes long. */                                                               
    );


#define PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND           0x0000  /**< Option mask for a combined read - command. */
#define PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_RESPONSE          0x0004  /**< Option mask for a combined read - response. */
#define PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND_RESPONSE  0x0008  /**< Option mask for a combined read - command + response. */


/**
* \brief MFP CombinedRead command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_CombinedReadMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */   
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *         Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *         On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. 
                                                *         The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. \n
                                                *         The flags #PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND, #PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_RESPONSE and
                                                *         #PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND_RESPONSE are used to switch between command, response or command+response. */
                                                uint8_t * pData,            /**< [In] Input Data Stream. */
                                                uint8_t bDataLength,        /**< [In] Length of input data stream. */
                                                uint8_t ** ppOutput,        /**< [Out] Encrypted command or decrypted response. */ 
                                                uint16_t * pOutputLength    /**< [Out] Length of encrypted command or decrypted response. */ 
                                                );

#define PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_COMMAND  0x0000  /**<  Option mask for a combined write - command. */
#define PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_RESPONSE 0x0004  /**<  Option mask for a combined write - response. */

/**
* \brief MFP CombinedWrite command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_CombinedWriteMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *         Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT.
                                                *         On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. \n
                                                *         The flags #PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_COMMAND and #PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_RESPONSE
                                                *         are used to switch between command or response. */
                                                uint8_t * pData,            /**< [In] Input data stream. */
                                                uint8_t bDataLength,        /**< [In] Length of input data stream. */
                                                uint8_t ** ppOutput,        /**< [Out] Encrypted command or decrypted response. */ 
                                                uint16_t * pOutputLength    /**< [Out] Length of encrypted command or decrypted response. */ 
                                                );

/**
* \brief Prepare the MFP command for key change.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyMfp_Part1 (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bCmd,                               /**< [In] Command used to perform the change. */
    uint16_t wBlockNo,                          /**< [In] Block Number. */
    uint8_t bKeyNo,                             /**< [In] Key number to be used. */
    uint8_t bKeyV,                              /**< [In] Key version to be used. */
    uint8_t * pDivInput,                        /**< [In] Pointer to diversification input data. */
    uint8_t bDivInputLength,                    /**< [In] Diversification input length. If it is 00h no diversification is used. */
    uint8_t ** ppOutput,                        /**< [Out] Output data from SAM. */
    uint16_t * pOutputLength                    /**< [Out] Output data length. */
    );

/**
* \brief Process the response to a key change command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyMfp_Part2 (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t * pDataIn,                          /**< [In] Pointer to input data (response from PICC). */
    uint8_t bDataInLength                       /**< [In] Input data length. */
    );

/**
* \brief MFP Virtual Card Support command.
* 
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *         The data in pData is buffered during #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                *         On a #PH_EXCHANGE_BUFFER_LAST the buffered data is sent togheter with the pData buffer and the PCD Capabilities to the card. */
                                                uint8_t* pData,                     /**< [In] Buffer containing the data set field. */
                                                uint8_t bDataLength,                /**< [In] Length of the data set buffer. */   
                                                uint8_t * pPcdCapabilities,         /**< [In] PcdCapabilities sent to the card. This buffer has to be bPcdCapabilitiesLength bytes long. */
                                                uint8_t  bPcdCapabilitiesLength,    /**< [In] Length of PcdCapabilities. */
                                                uint8_t ** pSupportedCardInfo,      /**< [Out] Pointer to buffer containing the card info of supported cards */
                                                uint16_t * pSupportedCardInfoLength /**< [Out] Amount of valid bytes in pSupportedCardInfo. */
                                                );

/**
* \brief MFP Select Virtual Card Select command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_SelectVirtualCardMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bKeyNo,                             /**< [In] Key number to be used in SVC command. */
    uint8_t bKeyV,                              /**< [In] Key version to be used in SVC command. */     
    uint8_t * pRndQ,                            /**< [In] RndQ sent to the card. This buffer has to be 12 bytes long. */
    uint8_t * pDivInput,                        /**< [In] Diversification input. */
    uint8_t  bDivInputLength,                   /**< [In] Length of diversification input. If 00h no diversification is used. */   
    uint8_t * pMac                              /**< [Out] MAC to be sent to the card. This buffer has to be 8 bytes long. */
    );

/**
* \brief First part of the MFP Proximity Check command. 
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bKeyNo,                             /**< [In] Key number to be used in authentication. */
    uint8_t bKeyV,                              /**< [In] Key version to be used in authentication. */
    uint8_t bPpse,                              /**< [In] PPSE byte used. */
    uint8_t * pRndRC,                           /**< [In] RND number in correct format for MAC verification. */
    uint8_t * pDivInput,                        /**< [In] Diversification input. */
    uint8_t  bDivInputLength,                   /**< [In] Length of diversification input. If 00h no diversification is used. */   
    uint8_t * pMac                              /**< [Out] Input in case of response MAC verification, output in case of command MAC generation. The buffer has to be 8 bytes long. */
    );

/**
* \brief Second part of the MFP Proximity Check command. 
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t * pMac                              /**< [In] Input in case of response MAC verification, output in case of command MAC generation. The buffer has to be 8 bytes long. */
    );

/** @} */

/*************************************************************************************************************************/
/****************************************NON X MODE: MIFARE CLASSIC ******************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

/** \defgroup phhalHw_SamAV2_NonX_MFC MIFARE Classic in Non-X-Mode
* \see phhalHw_SamAV2_Cmd_DataProcessing
* \brief SAM commands used for MIFARE Classic card communication in Non-X-Mode.
* @{
*/

#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MIFARE_DEFAULT     0x00    /**<  Default option mask for SAM_AuthenticateMifare. */
#define PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MIFARE_DIVERSIFY   0x01    /**<  Option mask for authentication with key diversification. */


/**
* \brief First part of MF Authenticate.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMifare_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption,                            /**< [In] Option parameter: \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MIFARE_DIVERSIFY is set the key is diversified. */
                                                uint8_t * pUid,         /**< [In] Pointer to the Uid of the Card. */
                                                uint8_t   bKeyNoM,      /**< [In] Key Number. */
                                                uint8_t   bKeyVM,       /**< [In] Key Version. */
                                                uint8_t   bKeyType,     /**< [In] Key type A (0Ah) or B (0Bh). */
                                                uint8_t   bBlockNo,     /**< [In] Block Number. */
                                                uint8_t * pRndB,        /**< [In] Random number received from card (5 bytes).*/
                                                uint8_t   bDivBlockNo,  /**< [In] Block number used for diversification. */
                                                uint8_t * pTokenAB      /**< [Out] Buffer containing the token to be sent to the card. The buffer has to be 9 bytes long. */
                                                );

/**
* \brief Second part of MF Authenticate.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMifare_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */                                    
    uint8_t * pTokenAB                          /**< [In] Token received from the card (5 bytes).*/ 
    );

#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_RETRIEVE_KEY       0x01    /**<  Option flag for key retrieval for transactions in host system. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_DIVERSIFY_KEY_A    0x02    /**<  Option flag for diversification of key A. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_DIVERSIFY_KEY_B    0x04    /**<  Option flag for diversification of key B. */
#define PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_ENCRYPT            0x08    /**<  Option flag for encrypted key retrival. Not used in case of key change for MIFARE 1k or 4k. */

/**
* \brief Change key MIFARE command used to prepare an encrypted stream to be written to a MIFARE Classic card.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyMifare(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */                                    
    uint8_t bOption,                            /**< [In] Option parameter:
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_RETRIEVE_KEY is set the command returns a single MIFARE key
                                                *         for transactions in host system. Otherwhise a cryptogram for key change on a MIFARE 1k or 4k card is generated.
                                                *         Key diversification can be enabled by setting flag #PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_DIVERSIFY_KEY_A and/or
                                                *         flag #PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_DIVERSIFY_KEY_B. In case of key retrieval for transactions in host system
                                                *         the encryption with the actual session key can be enabled by setting flag #PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_ENCRYPT. */
                                                uint8_t bKeyNoM,                /**< [In] Reference number of MIFARE key (00h to 7Fh). */
                                                uint8_t bKeyVM_KeyVMA,          /**< [In] Key version of MIFARE key in case of key retrieval or key version of MIFARE key A in case of key change. */
                                                uint8_t bKeyAorB_KeyVMB,        /**< [In] Key 0Ah or 0Bh in case of key retrieval or key version of MIFARE key B in case of key change.. */
                                                uint8_t * pAccessConditions,    /**< [In] Access conditions to be stored within MIFARE card. Only used for key change. This buffer has to be 4 bytes long. */
                                                uint8_t * pUid,                 /**< [In] Uid of the card. Only used if diversification is needed. This buffer has to be 4 bytes long. */
                                                uint8_t bDivBlockNo,            /**< [In] Block number to be used for diversification. */
                                                uint8_t * pKeyData,             /**< [Out] Buffer containing the key data. The buffer has to be 16 bytes long in case of key retrieval and 21 bytes long in case of key change. */
                                                uint8_t * pKeyDataLength        /**< [Out] Amount of valid data bytes in pKeyData. */
                                                );

/** @} */

/*************************************************************************************************************************/
/****************************************NON X MODE: DESFIRE ULC *********************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

/** \defgroup phhalHw_SamAV2_NonX_DF DESFire and ULC in Non-X-Mode
* \see phhalHw_SamAV2_Cmd_DataProcessing
* \brief SAM commands used for DESFire and ULC card communication in Non-X-Mode.
* @{
*/

/**
* \brief First part of an authentication procedure between SAM and DESFire or ULC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bAuthMode,                          /**< [In] Authentication mode used. */
    uint8_t bKeyNo,                             /**< [In] DESFire key number or key reference number (depends on SAM_SelectApplication done or not). */
    uint8_t bKeyV,                              /**< [In] Key version. */
    uint8_t * pEncRndB,                         /**< [In] Encoded random B (received from PICC). */
    uint8_t bEncRndBLength,                     /**< [In] Encoded random B length. */
    uint8_t * pDivInput,                        /**< [In] Diversification input data. */
    uint8_t bDivInputLength,                    /**< [In] Diversification input data length (00h in case of no diversification).  */
    uint8_t * pEncRndAB,                        /**< [Out] Buffer containing the encoded random numbers to be sent to the PICC. This buffer has to be 32 bytes long. */
    uint8_t * pEncRndABLength                   /**< [Out] Amount of valid data bytes in encoded pEncRndAB. */
    );

/**
* \brief Second part of an authentication procedure between SAM and DESFire.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t * pEncRndA,                         /**< [In] Encoded random A (received from PICC - 08h or 10h bytes). */
    uint8_t bEncRndALength                      /**< [In] Encoded random A length. */
    );

/**
* \brief First part of an ISO authentication procedure between SAM and DESFire.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bAuthMode,                          /**< [In] Authentication mode used. */
    uint8_t bKeyNo,                             /**< [In] DESFire key number or key reference number (depends on SAM_SelectApplication done or not). */
    uint8_t bKeyV,                              /**< [In] Key version. */
    uint8_t * pRndCard,                         /**< [In] Random number received from card. */
    uint8_t pRndCardLength,                     /**< [In] Random number length (8 bytes for (3)DES or 16 bytes for 3K3DES and AES). */
    uint8_t * pDivInput,                        /**< [In] Diversification input data. */
    uint8_t bDivInputLength,                    /**< [In] Diversification input data length. */
    uint8_t * pEncRnd,                          /**< [Out] Buffer containing two encoded and one plain random number. This buffer has to be 24 or 48 bytes long. */
    uint8_t * pEncRndLength                     /**< [Out] Amount of valid data bytes in encoded pEncRnd. */
    );

/**
* \brief Second part of an ISO authentication procedure between SAM and DESFire.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t * pRnd,                             /**< [In] RPCD2 received from card. */
    uint8_t bRndLength                          /**< [In] RPCD2 length. */
    );

/**
* \brief Change a key of a DESFire PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyPICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bKeyCompMeth,                       /**< [In] Bit mask defining the key compilation method. */
    uint8_t bCfg,                               /**< [In] Bit mask defining the change key configuration. */
    uint8_t bKeyNoCurrent,                      /**< [In] Current Key Number. */
    uint8_t bKeyVCurrent,                       /**< [In] Current Key Version. */
    uint8_t bKeyNoNew,                          /**< [In] New Key Number. */
    uint8_t bKeyVNew,                           /**< [In] New Key Version. */
    uint8_t * pDivInput,                        /**< [In] Diversification input data. */
    uint8_t bDivInputLength,                    /**< [In] Diversification input data length. No diversification in case of 00h. */
    uint8_t * pEncStream,                       /**< [Out] Buffer containing the stream to be sent to the PICC. The buffer has to be 32 bytes long. */
    uint8_t * pEncStreamLength                  /**< [Out] Amount of valid data bytes in pEncStream. */
    );

/** @} */

/** \defgroup phhalHw_SamAV2_RC RC522 Configuration
* \brief SAM commands used reader IC configuration.
* @{
*/

/**
* \brief Read one or more registers of the RC522.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_Cmd_RC_ReadRegister (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t *pRegAddress,                       /**< [In] Buffer containing the addresses of registers to be read. */
    uint8_t bRegAddressLength,                  /**< [In] Length of pRegAddress. */
    uint8_t *pValue                             /**< [Out] Buffer containing the read values. The buffer has to be bRegAddressLength long. */                                        
    );

/** 
* \brief Write one or more registers of the RC522.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_Cmd_RC_WriteRegister(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t *pData,                             /**< [In] Buffer containing address - value duos. */
    uint8_t bDataLength                         /**< [In] Amount of valid data bytes in the pData buffer. It has to be a multiple of 2. */                                       
    );

/**
* \brief Turn the RF-field of the RC522 on and/or off.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_Cmd_RC_RFControl(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wTime                              /**< [In] Shut down time for HF Field*/
    );

/**
* \brief Initialize the RC522 with a given register value set.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_Cmd_RC_Init(
                                      phhalHw_SamAV2_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                      uint8_t bLoadReg                              /**< [In] Number of register value set to be used for initialization (00h to 07h). */
                                      );

/**
* \brief Update a register value set in the SAM.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phhalHw_SamAV2_Cmd_RC_LoadRegisterValueSet(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */                                     
    uint8_t bStoreReg,                          /**< [In] Number of register value set to be used for storing (00h to 07h). */
    uint8_t *pData,                             /**< [In] Buffer containing address - value duos. */
    uint8_t bDataLength                         /**< [In] Amount of valid data bytes in the pData buffer. It has to be a multiple of 2. */
    );

/** @} */

/*************************************************************************************************************************/
/****************************************X MODE: ISO TYPEA ***************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

/** \defgroup phhalHw_SamAV2_X_ISO14443_3 ISO14443-3
* \brief SAM commands used for ISO14443 layer 3 communication in X-Mode.
* @{
*/

/**
* \brief Perform a request or wake-up command (type A).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHHAL_HW_SAMAV2_ERR_ISO_UID_INCOMPLETE Operation successful, UID not complete.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_RequestA_Wakeup(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bReqaWakeupCode,                    /**< [In] Command code (either REQA - 26h or WUPA - 52h). */
    uint8_t * pAtqa                             /**< [Out] AtqA returned by the card. The buffer has to be 2 bytes long. */
    );

/**
* \brief Perform a bit-wise anticollision and select. (type A).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_AnticollisionSelect(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pSelCodes,                        /**< [In] Buffer containing the SEL sequence bytes (93h, 95h and 97h). */
    uint8_t bSelCodesLength,                    /**< [In] Length of the SEL sequence buffer (01h to 03h). */
    uint8_t * pSak,                             /**< [Out] Pointer to the 1 byte Select Acknowledge reveived from card. */
    uint8_t * pUid,                             /**< [Out] Buffer containing the received UID. This buffer has to be 10 bytes long. */
    uint8_t * pUidLength                        /**< [Out] Amount of valid bytes in UID buffer. */
    );

#define PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_DEFAULT     0x00    /**<  Default option mask for ActivateIdle. */
#define PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_ATQA  0x01    /**<  Option flag for applying the ATQA filter. */
#define PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_SAK   0x02    /**<  Option flag for applying the SAK filter. */
#define PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_TIME  0x04    /**<  Option flag for setting the time to wait. */

/**
* \brief Perform one or several request - anticollision - select sequences (type A).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_ActivateIdle(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption,                            /**< [In] Option parameter: \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_ATQA the ATQA filter is applied. \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_SAK the SAK filter is applied. \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_TIME the waiting time is set to wTime. */
                                                uint8_t bNumCards,      /**< [In] Holds the number of cards to activate. */
                                                uint16_t wTime,         /**< [In] Time to wait for a card. */
                                                uint8_t * pAtqaIn,      /**< [In] Buffer containing the AtqA filter. This buffer has to be 4 bytes long. */
                                                uint8_t * pSakIn,       /**< [In] Buffer containing the Sak filter. This buffer has to be 2 bytes long. */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to the buffer containing the received data. */
                                                uint16_t * pRxLength    /**< [Out] Amount of valid bytes in ppRxBuffer. */
                                                );

/**
* \brief Perform a reactivate and select for a card in halt state (type A).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_ActivateWakeUp(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pUid,                             /**< [In] UID of the card to reactivate. */
    uint8_t bUidLength                          /**< [In] Length of UID (4, 7 or 10 bytes). */
    );

/**
* \brief Perform a halt command (type A).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_HaltA(
    phhalHw_SamAV2_DataParams_t * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Perform a transparent exchange command (type A).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_TransparentExchange(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pTxBuffer,                        /**< [In] Buffer containing the data to be sent. The buffer has to be bTxLength bytes long. */
    uint8_t bTxLength,                          /**< [In] Number of bytes to be sent to the card. */
    uint8_t bTxBitLength,                       /**< [In] Number valid bits in the last byte of the bTxBuffer. If set to 00h all bits are valid. */
    uint8_t ** ppRxBuffer,                      /**< [Out] Pointer to the buffer containing the received data. */
    uint16_t * pRxLength,                       /**< [Out] Amount of valid bytes in ppRxBuffer. */
    uint8_t * pRxBitLength                      /**< [Out] Amount of valid bits in the last byte in case of an incomplete byte. */
    );

/** @} */

/*************************************************************************************************************************/
/*********************************************** ISO4A LAYER *************************************************************/
/*************************************************************************************************************************/

/** \defgroup phhalHw_SamAV2_X_ISO14443_4 ISO14443-4
* \brief SAM commands used for ISO14443 layer 4 communication in X-Mode.
* @{
*/

/**
* \brief Perform a combined RATS and PPS to prepare a card for T=CL data exchange.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_RATS_PPS (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */ 
    uint8_t bCidIn,                             /**< [In] CID to be sent. */
    uint8_t bDsiIn,                             /**< [In] DSI to be sent. */
    uint8_t bDriIn,                             /**< [In] DRI to be sent. */
    uint8_t * pCidOut,                          /**< [Out] CID used (1 byte). */
    uint8_t * pDsiOut,                          /**< [Out] DSI used (1 byte). */
    uint8_t * pDriOut,                          /**< [Out] DRI used (1 byte). */
    uint8_t * pAts                              /**< [Out] Pointer to a buffer containing the received ATS. The length of the ATS can be found on the first position. */
    );

/**
* \brief Perform a init of ISO-14443-4 layer (init T=CL protocol).
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_Init(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bCid,                               /**< [In] CID to apply. */
    uint8_t bDri,                               /**< [In] Bit rate PCD -> PICC. */
    uint8_t bDsi,                               /**< [In] Bit rate PICC -> PCD */
    uint8_t bFwi,                               /**< [In] Frame waiting time indicator. */
    uint8_t bFsci                               /**< [In] Frame size card indicator. */
    );

/**
* \brief Perform an exchange bytes according ISO14443-4 T=CL protocol.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange (
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter: \n.
                                                *         Can be #PH_EXCHANGE_DEFAULT or #PH_EXCHANGE_TXCHAINING. */
                                                uint8_t * pAppDataIn,           /**< [In] Buffer containing application data to sent. */
                                                uint8_t bLenAppData,            /**< [In] Length of application data to sent. */
                                                uint8_t ** ppAppDataOut,        /**< [Out] Pointer to buffer containing the data returned by the PICC. */
                                                uint16_t * pAppDataOutLength    /**< [Out] Amount of valid bytes in ppAppDataOut. */    
                                                );

/**
* \brief Check if an activated card is still in the field.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_PresenceCheck(
    phhalHw_SamAV2_DataParams_t * pDataParams   /**< [In] Pointer to this layer's parameter structure. */                           
    );

#define PHHAL_HW_SAMAV2_CMD_ISO14443_4_DESELECT_FREECID_DEFAULT         0x00    /**< Default bFreeCid mask for Deselect. */
#define PHHAL_HW_SAMAV2_CMD_ISO14443_3_DESELECT_FREECID_FORCE_FREEING   0x01    /**< Option flag for forcing CID to be freed in any case. */

/**
* \brief Perform a Deselect command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_Deselect(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bFreeCid                            /**< [In] Bitmask for deselect option. */
    );

/**
* \brief Free one or more currently assigned CIDs.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_FreeCid(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pCid,                             /**< [In] Buffer containing all CIDs to be freed. */
    uint8_t bCidLength                          /**< [In] Length of the CID buffer (01h to 0Eh). */
    );

/** @} */

/*************************************************************************************************************************/
/****************************************X MODE: MIFARE CLASSIC ***********************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

/** \defgroup phhalHw_SamAV2_X_MFC MIFARE Classic in X-Mode
* \brief SAM commands used for MIFARE Classic card communication in X-Mode.
* @{
*/

#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFAUTHENTICATE_DEFAULT           0x00    /**<  Default option mask for MFAuthenticate - no diversification. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFAUTHENTICATE_DIVERSIFICATION   0x01    /**<  Option mask for key diversification. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFAUTHENTICATE_NODIVERSIFICATION 0x00    /**<  Option mask for no key diversification. */

/**
* \brief Perform a MIFARE Authenticate command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bOption,                            /**< [In] Option mask for this command. \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_CHANGE_MFAUTHENTICATE_DIVERSIFICATION is set, key diversification is used. */
                                                uint8_t * pUid,         /**< [In] MIFARE standard UID. This buffer has to be 4 bytes long. */
                                                uint8_t bKeyNoM,        /**< [In] Key reference number of MIFARE key. */        
                                                uint8_t bKeyVM,         /**< [In] Key version of MIFARE key. */        
                                                uint8_t bKeyType,       /**< [In] Type of key : MF key A (0x0A) or MF key B (0xB). */
                                                uint8_t bAuthBlockNo,   /**< [In] Block number to be authenticated within MIFARE PICC. */
                                                uint8_t bDivBlockNo     /**< [In] Block number to be used for diversification. Only used in case of diversification. */
                                                );

#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_DEFAULT       0x00    /**<  Default option mask for MFRead - single block read. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_MULTIBLOCK    0x01    /**<  Option mask for multiblock MFRead. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_SINGLEBLOCK   0x00    /**<  Option mask for singleblock MFRead. */

/**
* \brief Perform a MIFARE Read command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfRead (
                                      phhalHw_SamAV2_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */    
                                      uint8_t bOption,                              /**< [In] Option mask for this command. \n
                                                                                    *         If flag #PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_MULTIBLOCK is set, multi block read is used. */
                                                                                    uint8_t * pBlocks,  /**< [In] Pointer to the block numbers to be read. \n
                                                                                                        *         In case of single block read -> BNR_1 | BNR_2 | BNR_3 ... \n
                                                                                                        *         In case of multi block read -> BNR_1 | NumBlocks_1 | BNR_2 | NumBlocks_2 | BNR_3 | NumBlocks_3 ... */
                                                                                                        uint8_t bBlocksLength,  /**< [In] Length of the pBlocks buffer. */
                                                                                                        uint8_t ** ppData,      /**< [Out] Pointer to buffer containing the data read from the PICC. */   
                                                                                                        uint16_t * pDataLength  /**< [Out] Amount of valid bytes in the ppData buffer. */ 
                                                                                                        );

#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_DEFAULT         0x00    /**< Default option mask for MFWrite - MIFARE Classic single block write. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MFULTRALIGTH    0x01    /**< Option mask for multiblock MIFARE Plus write. */
#define PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MULTIBLOCK      0x02    /**< Option mask for singleblock MIFARE Ultraligth write. */

/**
* \brief Perform a MIFARE Write command.
* 
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfWrite(
                                      phhalHw_SamAV2_DataParams_t * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */   
                                      uint16_t wOption,                             /**< [In] Option mask for this command. \n
                                                                                    *          Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                                                    *          On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. \n
                                                                                    *          If flag #PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MFULTRALIGTH is set 4 byte data blocks are written (MIFARE Ultralight). \n
                                                                                    *          If flag #PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MULTIBLOCK is set multiblock write with 16 bytes data block length is used. */
                                                                                    uint8_t * pData,    /**< [In] Pointer to the MF_Write data field. \n
                                                                                                        *         In case of single block write -> BNR_1 | DATA_1 | BNR_2 | DATA_2 ... \n
                                                                                                        *         In case of multi block write -> BNR_1 | NumBlocks_1 | DATA_1 | BNR_2 | NumBlocks_2 | DATA_2 ... */
                                                                                                        uint8_t bDataLength /**< [In] Amount of valid bytes in the pData buffer. */
                                                                                                        );

/**
* \brief Perform a MIFARE Write Value command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfValueWrite(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t * pBlocks,                          /**< [In] Pointer to the block numbers and data to be written. BNR || value[4] || address[4] ... */
    uint8_t bBlocksLength                       /**< [In] Length of the pBlocks buffer. */    
    );

/**
* \brief Perform a MIFARE Increment command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfIncrement(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t * pBlocks,                          /**< [In] Pointer to the block numbers and data to be written. src_BNR || dest_BNR || value[4] ||  ... */
    uint8_t bBlocksLength                       /**< [In] Length of the pBlocks buffer. */    
    );

/**
* \brief Perform a MIFARE Decrement command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfDecrement(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t * pBlocks,                          /**< [In] Pointer to the block numbers and data to be written. src_BNR || dest_BNR || value[4] ||  ... */
    uint8_t bBlocksLength                       /**< [In] Length of the pBlocks buffer. */    
    );

/**
* \brief Perform a MIFARE Restore command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfRestore(
                                        phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
                                        uint8_t * pBlocks,                          /**< [In] Pointer to the block numbers of source and destination. src_BNR || dest_BNR || ... */
                                        uint8_t bBlocksLength                       /**< [In] Length of the pBlocks buffer. */    
                                        );

/**
* \brief Perform a MIFARE Authenticate followed by a MIFARE Read command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfAuthenticateRead(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option mask for this command. \n
                                                *         On every call a single data cycle is buffered. A cycle is one sequence of \n
                                                *         CmdSettings | KeyNoM | KeyVM | KeyType | AuthBlockNo | ... | NumBlocks | Blocks. \n
                                                *         If wOption is set to #PH_EXCHANGE_BUFFER_LAST the buffered cycles are send to the PICC. \n */
                                                uint8_t * pUid,         /**< [In] MIFARE standard UID. This buffer has to be 4 bytes long. This buffer is only used if wOption is set to #PH_EXCHANGE_BUFFER_FIRST. */
                                                uint8_t bCmdSettings,   /**< [In] Command settings of the data cycle. */        
                                                uint8_t bKeyNoM,        /**< [In] Key reference number of MIFARE key. */        
                                                uint8_t bKeyVM,         /**< [In] Key version of MIFARE key. */ 
                                                uint8_t bKeyType,       /**< [In] Type of key : MF key A (0x0A) or MF key B (0xB). */
                                                uint8_t bAuthBlockNo,   /**< [In] Block number to be authenticated within MIFARE PICC. */
                                                uint8_t bDivBlockNo,    /**< [In] Block number to be used for diversification. */
                                                uint8_t * pBlocks,      /**< [In] Pointer to the block numbers to be read. */
                                                uint8_t bBlocksLength,  /**< [In] Length of the pBlocks buffer. */
                                                uint8_t ** ppData,      /**< [Out] Pointer to buffer containing the data read from the PICC. */   
                                                uint16_t * pDataLength  /**< [Out] Amount of valid bytes in the ppData buffer. */ 
                                                );

/**
* \brief Perform a MIFARE Authenticate followed by a MIFARE Write command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfAuthenticateWrite(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option mask for this command. \n
                                                *          On every call a single data cycle is buffered. A cycle is one sequence of \n
                                                *          CmdSettings | KeyNoM | KeyVM | KeyType | AuthBlockNo | ... | NumBlocks | Blocks. \n
                                                *         If wOption is set to #PH_EXCHANGE_BUFFER_LAST the buffered cycles are send to the PICC. \n */
                                                uint8_t * pUid,         /**< [In] MIFARE standard UID. This buffer has to be 4 bytes long. This buffer is only used if wOption is set to #PH_EXCHANGE_BUFFER_FIRST. */
                                                uint8_t bCmdSettings,   /**< [In] Command settings of the data cycle. */        
                                                uint8_t bKeyNoM,        /**< [In] Key reference number of MIFARE key. */        
                                                uint8_t bKeyVM,         /**< [In] Key version of MIFARE key. */ 
                                                uint8_t bKeyType,       /**< [In] Type of key : MF key A (0x0A) or MF key B (0xB). */
                                                uint8_t bAuthBlockNo,   /**< [In] Block number to be authenticated within MIFARE PICC. */
                                                uint8_t bDivBlockNo,    /**< [In] Block number to be used for diversification. */
                                                uint8_t * pBlocks,      /**< [In] Buffer containing the block number and the data to be written. The sequence is BNR_1 | Data_1 | BNR_2 | Data_2 ... */
                                                uint8_t bBlocksLength   /**< [In] Length of the pBlocks buffer. */
                                                );

/**
* \brief Perform a change of a key in a MIFARE Card.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/ 
phStatus_t phhalHw_SamAV2_Cmd_MfChangeKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bKeyCompMeth,                       /**< [In] Bit mask determining the key compilation method. */
    uint8_t bKeyNoM,                            /**< [In] Key reference number of MIFARE key. */
    uint8_t bKeyVMA,                            /**< [In] Key version of MIFARE key A. */
    uint8_t bKeyVMB,                            /**< [In] Key version of MIFARE key B. */    
    uint8_t bBlockNo,                           /**< [In] MIFARE block number (block to store the new key). */
    uint8_t *pAccessConditions,                 /**< [In] Access Conditions to be stored. This buffer has to be 4 bytes long. */
    uint8_t *pUid,                              /**< [In] UID of the card. Only needed if diversification is indicated by bKeyCompMeth. This buffer has to be 4 bytes long. */
    uint8_t bDivBlockNo                         /**< [In] Block number used for diversification. Only needed if diversification is indicated by bKeyCompMeth.  */
    );

/** @} */

/*************************************************************************************************************************/
/****************************************X MODE: MIFARE PLUS **************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

/** \defgroup phhalHw_SamAV2_X_MFP MIFARE Plus in X-Mode
* \brief SAM commands used for MIFARE Plus card communication in X-Mode.
* @{
*/

/**
* \brief Perform a Write Perso MFP command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpWritePerso(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t*  pValues,                          /**< [In] Buffer containing the block numbers and values. 
                                                *         The format is BNR_1 || DATA_1 || BNR_2 || DATA_2 ... */
                                                uint8_t bValuesLength,      /**< [In] Length of the values buffer. It has to be a multiple of 8. */ 
                                                uint8_t * pPiccReturnCode   /**< [Out] Return Code sent by the MFP card. */
                                                );

#define PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST     0x01    /**<  Option mask for a first MFP Authentication. */
#define PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_NO_KDF         0x00    /**<  Option mask for a MFP Authentication with no KDF. */
#define PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL2_KDF        0x06    /**<  Option mask for a MFP Authentication with SL2 KDF. */
#define PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL3_KDF        0x08    /**<  Option mask for a MFP Authentication with SL3 KDF. */

/**
* \brief Perform a MFP Authenticate command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */   
    uint8_t bOption,                            /**< [In] Option for Authenticate MFP: \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST a first authentication is done.
                                                *         The key derivation function can be selected by setting the flags to #PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_NO_KDF,
                                                *         #PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL2_KDF or #PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL3_KDF. */
                                                uint8_t bKeyNo,             /**< [In] Key reference number to be used in authentication. */
                                                uint8_t bKeyV,              /**< [In] Key version to be used in authentication. */
                                                uint16_t wBlockNo,          /**< [In] Block Number to be sent to the card. */
                                                uint8_t * pPcdCapsIn,       /**< [In] Buffer containing the input PcdCaps. It has to be bDivInputLength bytes long. */
                                                uint8_t bPcdCapsInLength,   /**< [In] Input PcdCaps length. */
                                                uint8_t * pDivInput,        /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
                                                uint8_t bDivInputLength,    /**< [In] Diversification data length. If set to 00h no diversification is used. */
                                                uint8_t * pPcdCapsOut,      /**< [Out] Buffer containing the PcdCaps Out. It has to be 3 bytes long. */
                                                uint8_t * pPdCaps,          /**< [Out] Buffer containing the PdCaps Out. It has to be 3 bytes long. */
                                                uint8_t * pPiccReturnCode   /**< [Out] Return Code sent by the MFP card. */
                                                );

/**
* \brief Perform a MFP Combined Read command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpCombinedRead(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */   
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *         If set to #PH_EXCHANGE_RXCHAINING only dummy commands are sent to fetch the remaining data. */
                                                uint8_t *pPlainCommand,         /**< [In] Plain command to be sent to the card. */
                                                uint8_t bPlainCommandLength,    /**< [In] Length of Plain Command. */
                                                uint8_t ** pData,               /**< [Out] Pointer to the buffer containing the data returned by the card. */
                                                uint16_t * pDataLength,         /**< [Out] Amount of valid bytes in pData. */
                                                uint8_t * pPiccReturnCode       /**< [Out] Return code sent by the MFP card. */
                                                );

/**
* \brief Perform a MFP Combined Write command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpCombinedWrite(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */   
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *         Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                *         On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. \n*/
                                                uint8_t * pData,            /**< [In] Input data stream. */
                                                uint8_t bDataLength,        /**< [In] Length of the input data stream. */
                                                uint8_t * pPiccReturnCode   /**< [Out] Return Code sent by the MFP card. */
                                                );

/**
* \brief Perform a MFP Change Key command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpChangeKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */   
    uint8_t bCmd,                               /**< [In] Command code for MIFARE Plus write (WRITE_M or WRITE). */
    uint16_t wBlockNo,                          /**< [In] Block number of the MIFARE Plus block. */
    uint8_t bKeyNo,                             /**< [In] Key reference number of the new MIFARE Plus key. */
    uint8_t bKeyV,                              /**< [In] Key version of the new MIFARE Plus key. */
    uint8_t * pDivInput,                        /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
    uint8_t bDivInputLength,                    /**< [In] Diversification data length. If set to 00h no diversification is used. */
    uint8_t * pPiccReturnCode                   /**< [Out] Return Code sent by the MFP card. */
    );

#define PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK_DEFAULT            0x00    /**<  Default option mask for a MFP_ProximityCheck. */
#define PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK_RAND_PROCESSING    0x01    /**<  Option mask for random VPC processing. */

/**
* \brief Perform a MFP Proximity Check command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpProximityCheck(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint8_t bOption,                            /**< [In] Option parameter: \n
                                                *         If flag #PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK_RAND_PROCESSING is set, random VPC processing is enabled. Otherwhise
                                                *         normal VPC is used. */
                                                uint8_t bKeyNo,             /**< [In] Key reference number to be used for the proximity check. */
                                                uint8_t bKeyV,              /**< [In] Key version to be used for the proximity check. */
                                                uint8_t bBlCnt,             /**< [In] Specifies the maximum amount of random bytes transmitted at once. */
                                                uint8_t * pDivInput,        /**< [In] Diversification data length. If set to 00h no diversification is used. */
                                                uint8_t bDivInputLength,    /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
                                                uint8_t * pPiccReturnCode   /**< [Out] Return Code sent by the MFP card. */
                                                );

/**
* \brief Perform a MFP Virtual Card Support command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint16_t wOption,                           /**< [In] Option parameter: \n 
                                                *         The data in pData is buffered during #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                *         On a #PH_EXCHANGE_BUFFER_LAST the buffered data is sent togheter with the pData buffer and the PCD Capabilities to the card. */
                                                uint8_t* pData,                         /**< [In] Buffer containing the data set field. */
                                                uint8_t bDataLength,                    /**< [In] Length of the data set buffer. */   
                                                uint8_t * pPcdCapabilities,             /**< [In] PcdCapabilities sent to the card. This buffer has to be bPcdCapabilitiesLength bytes long. */
                                                uint8_t  bPcdCapabilitiesLength,        /**< [In] Length of PcdCapabilities. */
                                                uint8_t ** pSupportedCardInfo,          /**< [Out] Pointer to buffer containing the card info of supported cards */
                                                uint16_t * pSupportedCardInfoLength,    /**< [Out] Amount of valid bytes in pSupportedCardInfo. */
                                                uint8_t * pPiccReturnCode               /**< [Out] Return Code sent by the MFP card. */
                                                );

/**
* \brief Perform a MFP Select Virtual Card command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_MfpSelectVirtualCard(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bKeyNo,                             /**< [In] Key reference number to be used in SVC command. */
    uint8_t bKeyV,                              /**< [In] Key version to be used in SVC command. */     
    uint8_t * pIid,                             /**< [In] desired Iid. This buffer has to be 16 bytes long. */
    uint8_t * pDivInput,                        /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
    uint8_t  bDivInputLength,                   /**< [In] Diversification data length. If set to 00h no diversification is used. */
    uint8_t * pPiccReturnCode                   /**< [Out] Return Code sent by the MFP card. */
    );

/** @} */

/*************************************************************************************************************************/
/****************************************X MODE: DESFIRE ULC **************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/


/** \defgroup phhalHw_SamAV2_X_DF DESFire and ULC in X-Mode
* \brief SAM commands used for DESFire and ULC card communication in X-Mode.
* @{
*/

/**
* \brief Perform an authentication procedure between SAM and DESFire.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_DESFireAuthenticatePICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */  
    uint8_t bAuthMode,                          /**< [In] Authentication mode to be used. */
    uint8_t bISOMode,                           /**< [In] ISO mode to be used. */
    uint8_t bDFKeyNo,                           /**< [In] DESFire key number. */
    uint8_t bKeyNo,                             /**< [In] Key reference number. */
    uint8_t bKeyV,                              /**< [In] Key version. */
    uint8_t * pDivInput,                        /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
    uint8_t  bDivInputLength                    /**< [In] Diversification data length. If set to 00h no diversification is used. */
    );

/**
* \brief Change a key of a DESFire PICC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_DESFireChangeKeyPICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bKeyCompMeth,                       /**< [In] Bit mask defining the key compilation method. */
    uint8_t bCfg,                               /**< [In] Bit mask defining the change key configuration. */
    uint8_t bKeyNoCurrent,                      /**< [In] Current Key Number. */
    uint8_t bKeyVCurrent,                       /**< [In] Current Key Version. */
    uint8_t bKeyNoNew,                          /**< [In] New Key Number. */
    uint8_t bKeyVNew,                           /**< [In] New Key Version. */
    uint8_t * pDivInput,                        /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
    uint8_t  bDivInputLength                    /**< [In] Diversification data length. If set to 00h no diversification is used. */
    );

/**
* \brief Write data to a DESFire encrypted or MACed.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_DESFireWriteX(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *         Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                *         On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. \n
                                                *         The LFI is set to AFh if the flag #PH_EXCHANGE_TXCHAINING is enabled. */
                                                uint8_t bCrypto,    /**< [In] Crypto mode. */
                                                uint8_t *pData,     /**< [In] Data to be written. This buffer has to be bDataLen bytes long. */
                                                uint8_t bDataLength /**< [In] Data length. */
                                                );

/**
* \brief Read data from a DESFire encrypted or MACed.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_DESFireReadX(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */ 
    uint16_t wOption,                           /**< [In] Option parameter: \n
                                                *         Local buffering is implemented by the flags #PH_EXCHANGE_BUFFER_FIRST and #PH_EXCHANGE_BUFFER_CONT. \n
                                                *         On #PH_EXCHANGE_BUFFER_LAST the command is sent to the SAM. \n
                                                *         If flag #PH_EXCHANGE_RXCHAINING set, the command continues data reception from the DESFire PICC. */
                                                uint8_t bCrypto,        /**< [In] Crypto mode. */
                                                uint8_t *pData,         /**< [In] Data to send to DESFire. */
                                                uint8_t bDataLength,    /**< [In] Data length. */
                                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to the received data. */
                                                uint16_t * pRxLength    /**< [Out] Amount of valid bytes in the received data buffer. */
                                                );

/**
* \brief Perform an authentication procedure between SAM and ULC.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_ULCAuthenticatePICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */    
    uint8_t bKeyNo,                             /**< [In] Key reference number to be used in authentication. */
    uint8_t bKeyV,                              /**< [In] Key version to be used in authentication. */
    uint8_t * pDivInput,                        /**< [In] Diversification data. This buffer has to be bDivInputLength bytes long. */
    uint8_t  bDivInputLength                    /**< [In] Diversification data length. If set to 00h no diversification is used. */
    );

/**
* \brief Exchange commands with the SAM
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Cmd_7816Exchange(
    phhalHw_SamAV2_DataParams_t * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,                           /**< [In] Option parameter. */
    uint8_t * pTxBuffer,                        /**< [In] Data to transmit. */
    uint16_t wTxLength,                         /**< [In] Length of input data. */
    uint8_t ** ppRxBuffer,                      /**< [Out] Pointer to received data. */
    uint16_t * pRxLength                        /**< [Out] Number of received data bytes. */
    );

/** @} */
#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHHALHW_SAMAV2_CMD_H */
