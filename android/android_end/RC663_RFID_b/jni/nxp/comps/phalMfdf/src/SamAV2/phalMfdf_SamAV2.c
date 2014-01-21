/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/**
* \file
* Software MIFARE DESFire Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:18 2011 $
*
* History:
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>
#include <phCryptoSym.h>
#include <phCryptoRng.h>
#include <phKeyStore.h>
#include <ph_RefDefs.h>
#include <string.h>
#include <ph_TypeDefs.h>

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2

#include "../phalMfdf_Int.h"
#include "phalMfdf_SamAV2.h"
#include "phalMfdf_SamAV2_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

/**
* \name Security related Commands
*/
phStatus_t phalMfdf_SamAV2_Init(
                                phalMfdf_SamAV2_DataParams_t * pDataParams,
                                uint16_t wSizeOfDataParams,
                                phhalHw_SamAV2_DataParams_t * pHalSamDataParams,
                                void * pHalDataParams,
                                void * pPalMifareDataParams
                                )
{
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pPalMifareDataParams);
    PH_ASSERT_NULL (pHalSamDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* data param check */
    if (sizeof(phalMfdf_SamAV2_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFDF | PHAL_MFDF_SAMAV2_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pHalSamDataParams      = pHalSamDataParams;
    pDataParams->pHalDataParams         = pHalDataParams;
    pDataParams->bKeyNo                 = 0xFF; /* Set to invalid */
    pDataParams->bAuthMode              = PHAL_MFDF_NOT_AUTHENTICATED; /* Set to invalid */
    pDataParams->bWrappedMode           = 0x00; /* Set to false */
    pDataParams->wAdditionalInfo        = 0x0000;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_Authenticate(
                                        phalMfdf_SamAV2_DataParams_t *pDataParams,
                                        uint16_t wOption,
                                        uint16_t wKeyNo,
                                        uint16_t wKeyVer,
                                        uint8_t bKeyNoCard,
                                        uint8_t * pDivInput,
                                        uint8_t bDivLen
                                        )
{
    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATE;
    return phalMfdf_SamAV2_Int_Authenticate_General(
        pDataParams,
        wOption,
        wKeyNo,
        wKeyVer,
        bKeyNoCard,
        pDivInput,
        bDivLen
        );
}

phStatus_t phalMfdf_SamAV2_AuthenticateISO(
    phalMfdf_SamAV2_DataParams_t *pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    )
{
    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
    return phalMfdf_SamAV2_Int_Authenticate_General(
        pDataParams,
        wOption,
        wKeyNo,
        wKeyVer,
        bKeyNoCard,
        pDivInput,
        bDivLen
        );
}

phStatus_t phalMfdf_SamAV2_AuthenticateAES(
    phalMfdf_SamAV2_DataParams_t *pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    )
{
    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES;
    return phalMfdf_SamAV2_Int_Authenticate_General(
        pDataParams,
        wOption,
        wKeyNo,
        wKeyVer,
        bKeyNoCard,
        pDivInput,
        bDivLen
        );
}

phStatus_t phalMfdf_SamAV2_ChangeKeySettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeySettings
    )
{
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CHANGE_KEY_SETTINGS;
    bCmdBuff[wCmdLen++] = bKeySettings;

    /* COMMUNICATION IS Encrypted */
    return phalMfdf_SamAV2_Int_Write_Enc(pDataParams,
        bCmdBuff,
        0x0001,
        &bCmdBuff[1],
        0x0001
        );
}

phStatus_t phalMfdf_SamAV2_GetKeySettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pResponse
    )
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_KEY_SETTINGS;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        0x01,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 0x02)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_ChangeKey(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint16_t wOldKeyNo,
                                     uint16_t wOldKeyVer,
                                     uint16_t wNewKeyNo,
                                     uint16_t wNewKeyVer,
                                     uint8_t bKeyNoCard,
                                     uint8_t * pDivInput,
                                     uint8_t bDivLen
                                     )
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCmdBuff[42];
    uint8_t     PH_MEMLOC_REM bEncStream[42]; /* SAM can return upto 42 bytes */
    uint8_t     PH_MEMLOC_REM bEncStreamLen;
    uint8_t     PH_MEMLOC_REM bKeyCompMethod;
    uint8_t     PH_MEMLOC_REM bCfg;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bAppId[3] = { 0x00, 0x00, 0x00 };

    if ((memcmp(pDataParams->pAid, bAppId, 3) == 0x00) && ((bKeyNoCard & 0x3FU) == 0x00)) /* Card Master key */
    {
        /* Only if seleted Aid is 0x000000, and card key number is X0, then 
        it is likely to be the PICC master key that has to be changed. */
        if ((bKeyNoCard != 0x80) && (bKeyNoCard != 0x40) && (bKeyNoCard != 0x00))
        {
            /* Invalid card key number supplied */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    else
    {
        if (bKeyNoCard > 0x0D)
        {
            /* Invalid application key specified */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    if ((wOption == 0x0000) || (bDivLen > 31))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOldKeyNo > 0x7F) || (wNewKeyNo > 0x7F) || (wNewKeyVer > 0xFF) || (wOldKeyVer > 0xFF))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    memset(bEncStream, 0x00, 42); /* PRQA S 3200 */
    memset(bCmdBuff, 0x00, 42); /* PRQA S 3200 */
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CHANGE_KEY;
    bCmdBuff[wCmdLen++] = bKeyNoCard;

    /* build the bKeyCompMethod */
    /* Initialize */
    bKeyCompMethod = 0x00;

    /* Setting bit 0 */
    if ((pDataParams->bKeyNo & 0x3FU) != (bKeyNoCard & 0x3FU))
    {
        /* Key number to change is different than the key authenticated with */
        bKeyCompMethod = 0x00;
    }
    else
    {
        bKeyCompMethod = 0x01;
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && (bDivLen != 0x00))
    {
        if (wOption & PHAL_MFDF_CHGKEY_DIV_NEW_KEY )
        {
            /* Set bit1 to 1 */
            bKeyCompMethod |= 0x02; 

            if (wOption & PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC)
            {
                /* Set bit 5 */
                bKeyCompMethod |= 0x20;
            }
            else
            {
                /* bit 5 is zero by default */
                if (wOption & PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND)
                {
                    /* Set bit 3. For Two rounds, it is zero and default */
                    bKeyCompMethod |= 0x08;
                }
            }
        }

        if (wOption & PHAL_MFDF_CHGKEY_DIV_OLD_KEY )
        {
            /* Set bit 2 */
            bKeyCompMethod |= 0x04;

            if (wOption & PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC)
            {
                /* Set bit 5 */
                bKeyCompMethod |= 0x20;
            }
            else
            {
                /* bit 5 is zero by default */;
                if (wOption & PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND)
                {
                    /* Set bit 4. For Two rounds, it is zero and default */
                    bKeyCompMethod |= 0x10;
                }
            } 
        }
    }
    else
    {
        bDivLen = 0x00;
    }

    /* Now build the bCfg */
    /* Initialize */
    bCfg = 0x00; 

    bCfg = bKeyNoCard & 0x0FU; /* Bits 0 to 3 has the desfire key number to be changed */

    if ((memcmp(pDataParams->pAid, bAppId, 3) == 0x00) && ((bKeyNoCard & 0x3FU) == 0x00)) /* Card Master key */
    {
        /* Set bit 4 in case of card master key being changed */
        bCfg |= 0x10;
    }

    /* Ask SAM to generate the cryptogram */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_ChangeKeyPICC(
        pDataParams->pHalSamDataParams,  
        bKeyCompMethod,  
        bCfg,  
        (uint8_t)wOldKeyNo,  
        (uint8_t)wOldKeyVer,  
        (uint8_t)wNewKeyNo,  
        (uint8_t)wNewKeyVer,  
        pDivInput,  
        bDivLen,  
        bEncStream,  
        &bEncStreamLen   
        ));

    memcpy(&bCmdBuff[wCmdLen], bEncStream, bEncStreamLen); /* PRQA S 3200 */

    wCmdLen = wCmdLen + bEncStreamLen;

    /* Send the bEncStream to PICC */
    status = phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,                          
        bCmdBuff,
        wCmdLen,
        bCmdBuff,
        &wRxlen
        );
    if (status != PH_ERR_SUCCESS)
    {
        if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
        {
            phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
            statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                pDataParams->pHalSamDataParams,
                0x00); 
        }             
    }
    return status;
}

phStatus_t phalMfdf_SamAV2_GetKeyVersion(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t * pResponse
    )
{
    /**
    * This command can be issued without valid authentication
    */
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    if (bKeyNo > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_KEY_VERSION;
    bCmdBuff[wCmdLen++] = bKeyNo;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 0x01)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_CreateApplication(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pAid, 
    uint8_t bKeySettings1,
    uint8_t bKeySettings2,
    uint8_t * pISOFileId,
    uint8_t * pISODFName,
    uint8_t bISODFNameLen
    )
{
    /*
    If (bKeySettings2 & 0x03)== 00 [DES, 3DES] then pDataParams->bAuthMode can be either
    0x0A or 0x1A.
    If (bKeySettings2 & 0x03)== 01 [3K3DES] then pDataParams->bAuthMode can only be 0x1A.
    If (bKeySettings2 & 0x03)== 10 [AES] then pDataParams->bAuthMode can only be 0xAA.
    */
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bISODFNameLen > 16) || (bOption > 0x03) || (bOption == 0x02))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_APPLN;

    memcpy(&bCmdBuff[wCmdLen], pAid, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    bCmdBuff[wCmdLen++] = bKeySettings1;
    bCmdBuff[wCmdLen++] = bKeySettings2;

    if (bOption & 0x01)
    {
        /* wISOFileId is present */
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }
    if (bOption & 0x02)
    {
        /* pISODFName is present */
        memcpy(&bCmdBuff[wCmdLen], pISODFName, bISODFNameLen); /* PRQA S 3200 */
        wCmdLen = wCmdLen + bISODFNameLen;
    }

    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_DeleteApplication(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pAid
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[10];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_DELETE_APPLN;
    memcpy(&bCmdBuff[1], pAid, PHAL_MFDF_DFAPPID_SIZE); /* PRQA S 3200 */
    wCmdLen += PHAL_MFDF_DFAPPID_SIZE;

    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_GetApplicationIDs(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pResponse,
    uint8_t * pNumAIDs
    )
{
    /**
    A PICC can store upto 28 applications. PICC will return all
    AIDs (3 byte/aid) in single response if the number of applications <= 20
    else, it will send the  remaining AIDs in the second transmission. The
    first response sent by PICC will have 0xAF in the status byte.
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* Initialization */
    *pNumAIDs = 0;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_APPLN_IDS;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        0x0001,
        &pRecv,
        &wRxlen
        ));
    /* Length should be a multiple of 3. Else return error */
    if (wRxlen % 3)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pNumAids and return  */
    *pNumAIDs = (uint8_t)(wRxlen / 3);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_GetDFNames(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bOption,
                                      uint8_t * pDFBuffer,
                                      uint8_t * pDFInfoLen
                                      )
{
    /**
    Returns AID(3B), FID(2B), DF-Name(1..16B) in one frame.

    Will return PH_EXCHANGE_RXCHAINING if more DF names are present.
    The caller has to call the function with option PH_EXCHANGE_RXCHAINING

    Will not work if authenticated in standard TDES or AES modes as per the
    Implementation Hints document.
    */
    uint16_t    PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint8_t     PH_MEMLOC_REM bWorkBuffer[24];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen;


    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
    {
        /* Should return, invalid scenario error. Card will be disabled 
        in case this command is sent in these modes */
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
    }

    /* form the command */
    if (bOption == PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[0] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if (bOption == PH_EXCHANGE_DEFAULT)
    {
        bCmdBuff[0] = PHAL_MFDF_CMD_GET_DF_NAMES;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    wCmdLen = 1;

    /* Send the command */
    status = phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,                          
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        );
    /* check for protocol errors */
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) &&
        (wRxlen == 0))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* Should return either zero bytes or more than 4 bytes. Anything inbetween
      is an error */
    if ((status == PH_ERR_SUCCESS) && (wRxlen != 0) && (wRxlen < 5))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        && (wRxlen < 5))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) || (status == PH_ERR_SUCCESS))
    {
        memcpy(pDFBuffer, bWorkBuffer, wRxlen); /* PRQA S 3200 */
        *pDFInfoLen = (uint8_t)wRxlen;
    }
    return status;
}

phStatus_t phalMfdf_SamAV2_SelectApplication(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pAppId
    )
{
    uint16_t  PH_MEMLOC_REM statusTmp;
    uint8_t   PH_MEMLOC_REM bCmdBuff[10];
    uint8_t   PH_MEMLOC_REM bWorkBuffer[8];
    uint16_t  PH_MEMLOC_REM wRxlen;
    uint16_t  PH_MEMLOC_REM wCmdLen;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_SELECT_APPLN;
    memcpy(&bCmdBuff[1], pAppId, PHAL_MFDF_DFAPPID_SIZE); /* PRQA S 3200 */
    wCmdLen = PHAL_MFDF_DFAPPID_SIZE + 1;

    /* Reset Authentication Status here */
    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);

    /* Kill existing authentications with SAM */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
        pDataParams->pHalSamDataParams,
        0x00));
    /* Send the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_ExchangeCmd(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        pDataParams->bWrappedMode,                                        
        bCmdBuff,
        wCmdLen,
        bWorkBuffer,
        &wRxlen
        ));

    /* Store the currently selected application Id */
    memcpy(pDataParams->pAid, pAppId, 3);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_FormatPICC(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams
                                      )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_FORMAT_PICC;
    return phalMfdf_SamAV2_Int_Write_Plain(pDataParams, bCmdBuff, 0x0001, PHAL_MFDF_COMMUNICATION_PLAIN, NULL, 0x0000);
}

phStatus_t phalMfdf_SamAV2_GetVersion(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t * pResponse
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_VERSION;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        0x0001,
        &pRecv,
        &wRxlen
        ));

    if (wRxlen != 28)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_FreeMem(
                                   phalMfdf_SamAV2_DataParams_t * pDataParams,
                                   uint8_t * pResponse
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_FREE_MEM;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        0x0001,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 3)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_SetConfiguration(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pData,
    uint8_t bDataLen
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bOption == PHAL_MFDF_SET_CONFIG_OPTION3)
    {
        /* This command is not supported in Sam Non X mode since
        it requires Padding method 2 before encryption */
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption != PHAL_MFDF_SET_CONFIG_OPTION1) && 
        (bOption != PHAL_MFDF_SET_CONFIG_OPTION2))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if((bOption == PHAL_MFDF_SET_CONFIG_OPTION2) && (bDataLen != 25))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_SET_CONFIG;
    bCmdBuff[wCmdLen++] = bOption;

    return phalMfdf_SamAV2_Int_Write_Enc(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        pData,
        (uint16_t)bDataLen
        );
}

phStatus_t phalMfdf_SamAV2_GetCardUID(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t * pResponse
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bLen[3] = { 0x07, 0x00, 0x00 };

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_CARD_UID;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Enc(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_ENC | PH_CRYPTOSYM_PADDING_MODE_1,
        bCmdBuff,
        0x0001,
        bLen,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 7)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_GetFileIDs(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t * pResponse,
                                      uint8_t * bNumFIDs
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM * pRecv;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_FILE_IDS;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        0x0001,
        &pRecv,
        &wRxlen
        ));

    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */
    *bNumFIDs = (uint8_t)wRxlen;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_GetISOFileIDs(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pResponse,
    uint8_t * pNumFIDs
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM * pRecv;

    /* Initialization */
    *pNumFIDs = 0;

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_GET_ISO_FILE_IDS;

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        0x0001,
        &pRecv,
        &wRxlen
        ));

    /* Length should be multiple of 2 */
    if ((wRxlen != 0) && (wRxlen % 2))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pNumAids and return  */
    *pNumFIDs = (uint8_t)(wRxlen / 2);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_GetFileSettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bFileNo,
    uint8_t * pResponse,
    uint8_t * pBufferLen
    )
{
    uint16_t    PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_FILE_SETTINGS;
    bCmdBuff[wCmdLen++] = bFileNo;    

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
        pDataParams,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        bCmdBuff,
        wCmdLen,
        &pRecv,
        &wRxlen
        ));

    /* 7 => Data files, 17 => value files, 13 => record files */
    if ((wRxlen != 7) && (wRxlen != 17) && (wRxlen != 13))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRecv, wRxlen); /* PRQA S 3200 */

    /* Update pBufferLen and return  */
    *pBufferLen = (uint8_t)wRxlen;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_ChangeFileSettings(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t * pAccessRights
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CHANGE_FILE_SETTINGS;
    bCmdBuff[wCmdLen++] = bFileNo;

    bCmdBuff[wCmdLen++] = bCommSett >> 4; /* Settings stored in MS Nibble moved to LS nibble */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    if ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_SamAV2_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            0x0002,
            &bCmdBuff[2],
            wCmdLen - 2
            );
    }
    else if (((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_PLAIN) ||
        ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD))    
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_SamAV2_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            PHAL_MFDF_COMMUNICATION_PLAIN,
            NULL,
            0x0000
            );
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
}

phStatus_t phalMfdf_SamAV2_CreateStdDataFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_STD_DATAFILE;

    /* File Number */
    bCmdBuff[wCmdLen++] = bFileNo;

    /* Copy ISO Filed ID if present */
    if (bOption == 0x01)
    {
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }

    /* Copy communication settings. communication settings in the first nibble so right shifting */
    bCmdBuff[wCmdLen++] = bCommSett >> 4;

    /* Copy Access rights */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    /* Copy File size supplied by the user */
    memcpy(&bCmdBuff[wCmdLen], pFileSize, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_CreateBackupDataFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_BKUP_DATAFILE;

    /* File Number */
    bCmdBuff[wCmdLen++] = bFileNo;

    /* Copy ISO Filed ID if present */
    if (bOption == 0x01)
    {
        bCmdBuff[wCmdLen++] = pISOFileId[0];
        bCmdBuff[wCmdLen++] = pISOFileId[1];
    }

    /* Copy communication settings */
    bCmdBuff[wCmdLen++] = bCommSett >> 4;

    /* Copy Access rights */
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    /* Copy File size supplied by the user */
    memcpy(&bCmdBuff[wCmdLen], pFileSize, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_CreateValueFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pLowerLmit,
    uint8_t * pUpperLmit,
    uint8_t * pValue,
    uint8_t bLimitedCredit
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_VALUE_FILE;
    bCmdBuff[wCmdLen++] = bFileNo;
    bCmdBuff[wCmdLen++] = bCommSett >> 4;

    memcpy(bCmdBuff+wCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 2;

    memcpy(bCmdBuff+wCmdLen, pLowerLmit, 4); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 4;

    memcpy(bCmdBuff+wCmdLen, pUpperLmit, 4); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 4;

    memcpy(bCmdBuff+wCmdLen, pValue, 4); /* PRQA S 3200 */
    wCmdLen = wCmdLen + 4;

    bCmdBuff[wCmdLen++] = bLimitedCredit;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_CreateLinearRecordFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_LINEAR_RECFILE;

    /* Copy the value information supplied by the user */
    bCmdBuff[wCmdLen++] = bFileNo;

    if (bOption == 0x01)
    {
        bCmdBuff[wCmdLen++] = pIsoFileId[0];
        bCmdBuff[wCmdLen++] = pIsoFileId[1];
    }
    bCmdBuff[wCmdLen++] = bCommSett>>4;
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    bCmdBuff[wCmdLen++] = pRecordSize[0];
    bCmdBuff[wCmdLen++] = pRecordSize[1];
    bCmdBuff[wCmdLen++] = pRecordSize[2];

    bCmdBuff[wCmdLen++] = pMaxNoOfRec[0];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[1];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[2];

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_CreateCyclicRecordFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if ((bFileNo > 0x1f) || (bOption > 0x01))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommSett != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommSett != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREATE_CYCLIC_RECFILE;

    /* Copy the value information supplied by the user */
    bCmdBuff[wCmdLen++] = bFileNo;

    if (bOption == 0x01)
    {
        bCmdBuff[wCmdLen++] = pIsoFileId[0];
        bCmdBuff[wCmdLen++] = pIsoFileId[1];
    }
    bCmdBuff[wCmdLen++] = bCommSett >> 4;
    bCmdBuff[wCmdLen++] = pAccessRights[0];
    bCmdBuff[wCmdLen++] = pAccessRights[1];

    bCmdBuff[wCmdLen++] = pRecordSize[0];
    bCmdBuff[wCmdLen++] = pRecordSize[1];
    bCmdBuff[wCmdLen++] = pRecordSize[2];

    bCmdBuff[wCmdLen++] = pMaxNoOfRec[0];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[1];
    bCmdBuff[wCmdLen++] = pMaxNoOfRec[2];

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_DeleteFile(
                                      phalMfdf_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bFileNo
                                      )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_DELETE_FILE;
    bCmdBuff[wCmdLen++] = bFileNo;    

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_ReadData(
                                    phalMfdf_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t bFileNo,
                                    uint8_t * pOffset,
                                    uint8_t * pLength,
                                    uint8_t ** ppRxdata,
                                    uint16_t * pRxdataLen
                                    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wDataLen = 0;
    phStatus_t  PH_MEMLOC_REM status;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption & 0x0FU) == PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if ((bOption & 0x0FU) == PH_EXCHANGE_DEFAULT)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_READ_DATA;
        bCmdBuff[wCmdLen++] = bFileNo;
        memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
        wCmdLen += 3;
        memcpy(&bCmdBuff[wCmdLen], pLength, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        wDataLen = (uint16_t)pLength[1]; /* MSB */
        wDataLen <<= 8;
        wDataLen |= pLength[0]; /* LSB */
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        if ((pLength[0] == 0x00) &&
            (pLength[1] == 0x00) &&
            (pLength[2] == 0x00))
        {
            /* Unknown number of bytes will be read */
            bOption = PH_CRYPTOSYM_PADDING_MODE_2;
        }
        else
        {
            bOption = PH_CRYPTOSYM_PADDING_MODE_1;
        }

        status = phalMfdf_SamAV2_Int_ReadData_Enc(
            pDataParams,
            bOption, /* padding option based on pLength */
            bCmdBuff,
            wCmdLen,
            pLength,  /* Sending pLength. If pLength == 00, then whole file is read */
            ppRxdata,
            pRxdataLen
            );
    }
    else if (((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD) ||
        ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_PLAIN))
    {
        status = phalMfdf_SamAV2_Int_ReadData_Plain(
            pDataParams,
            bOption, /* Is either plain or MAC'd. No need for padding options here */
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((status == PH_ERR_SUCCESS) && (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
    {
        /* Cannot check for exact length if all data is read by specifying
            pLength = 0x00 00 00 */
        if ((wDataLen != *pRxdataLen) && (wDataLen != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                status = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00); 
            }
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
    }
    return status;
}

phStatus_t phalMfdf_SamAV2_WriteData(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint8_t bCommOption,
                                     uint8_t bFileNo,
                                     uint8_t * pOffset,
                                     uint8_t * pData,
                                     uint8_t * pDataLen
                                     )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wDataLen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_WRITE_DATA;
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* Assuming here that the size can never go beyond FFFF. 
    In fact it can never go beyond 8092 (1F9C) bytes */
    wDataLen = (uint16_t)pDataLen[1];
    wDataLen = wDataLen << 8;
    wDataLen |= pDataLen[0];

    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDF_COMMUNICATION_PLAIN;
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_SamAV2_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            pData,
            wDataLen
            );
    }
    else
    {
        return phalMfdf_SamAV2_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            bCommOption,
            pData,
            wDataLen
            );
    }
}

phStatus_t phalMfdf_SamAV2_GetValue(
                                    phalMfdf_SamAV2_DataParams_t * pDataParams,
                                    uint8_t bCommOption,
                                    uint8_t bFileNo,
                                    uint8_t * pValue
                                    )
{
    /* If not authenticated, send the data and get the value in plain.
    Else use the mode dictated by the caller of this API
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM pLen[3] = { 0x04, 0x00, 0x00 }; /* 4 byte value to be read */

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_GET_VALUE;
    bCmdBuff[wCmdLen++] = bFileNo;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Enc(
            pDataParams,
            PH_CRYPTOSYM_PADDING_MODE_1,
            bCmdBuff,
            wCmdLen,
            pLen,
            &pRecv,
            &wRxlen
            ));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_Int_ReadData_Plain(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            &pRecv,
            &wRxlen
            ));
    }
    if (wRxlen != 4)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pValue, pRecv, wRxlen); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_Credit(
                                  phalMfdf_SamAV2_DataParams_t * pDataParams,
                                  uint8_t bCommOption,
                                  uint8_t bFileNo,
                                  uint8_t * pValue
                                  )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CREDIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_SamAV2_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            0x0002,
            &bCmdBuff[2],
            0x0004
            );
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_SamAV2_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }    
}

phStatus_t phalMfdf_SamAV2_Debit(
                                 phalMfdf_SamAV2_DataParams_t * pDataParams,
                                 uint8_t bCommOption,
                                 uint8_t bFileNo,
                                 uint8_t * pValue
                                 )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_DEBIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_SamAV2_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            0x0002,
            &bCmdBuff[2],
            0x0004
            );
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_SamAV2_Int_Write_Plain(pDataParams,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }
}

phStatus_t phalMfdf_SamAV2_LimitedCredit(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bCommOption,
    uint8_t bFileNo,
    uint8_t * pValue
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[32];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_LIMITED_CREDIT;
    bCmdBuff[wCmdLen++] = bFileNo;

    memcpy(&bCmdBuff[wCmdLen], pValue, 4); /* PRQA S 3200 */
    wCmdLen += 4;

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        return phalMfdf_SamAV2_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            0x0002,
            &bCmdBuff[2],
            0x0004
            );
    }
    else
    {
        /* COMMUNICATION IS PLAIN */
        return phalMfdf_SamAV2_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            0x0002,
            bCommOption,
            &bCmdBuff[2],
            0x0004
            );
    }

}

phStatus_t phalMfdf_SamAV2_WriteRecord(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bCommOption,
                                       uint8_t bFileNo,
                                       uint8_t * pOffset,
                                       uint8_t * pData,
                                       uint8_t * pDataLen
                                       )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wDataLen;
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bCommOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bCommOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_WRITE_RECORD;
    bCmdBuff[wCmdLen++] = bFileNo;
    memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
    wCmdLen += 3;
    memcpy(&bCmdBuff[wCmdLen], pDataLen, 3); /* PRQA S 3200 */
    wCmdLen += 3;

    /* Assuming here that the size can never go beyond FFFF. 
    In fact it can never go beyond 8092 (1F9C) bytes */

    wDataLen = (uint16_t)pDataLen[1];
    wDataLen = wDataLen << 8;
    wDataLen |= pDataLen[0];

    if (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        bCommOption = PHAL_MFDF_COMMUNICATION_PLAIN;
    }

    if ( (pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)||
        (bCommOption == PHAL_MFDF_COMMUNICATION_MACD) ||
        (bCommOption == PHAL_MFDF_COMMUNICATION_PLAIN))
    {
        return phalMfdf_SamAV2_Int_Write_Plain(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            bCommOption,
            pData,
            wDataLen
            );
    }
    else /* (bCommOption == PHAL_MFDF_COMMUNICATION_ENC) */
    {
        return phalMfdf_SamAV2_Int_Write_Enc(
            pDataParams,
            bCmdBuff,
            wCmdLen,
            pData,
            wDataLen
            );
    }
}

phStatus_t phalMfdf_SamAV2_ReadRecords(
                                       phalMfdf_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bCommOption,
                                       uint8_t bFileNo,
                                       uint8_t * pOffset,
                                       uint8_t * pNumRec,
                                       uint8_t * pRecSize,
                                       uint8_t ** ppRxdata,
                                       uint16_t * pRxdataLen
                                       )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[20];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;
    uint16_t    PH_MEMLOC_REM wNumBytes = 0;
    uint16_t    PH_MEMLOC_REM wNumRec = 0;
    phStatus_t  PH_MEMLOC_REM status;

    /* Should also handle the scenario where 0xAF is returned by
    the PICC */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if (!((pNumRec[0] == 0) && (pNumRec[1] == 0) && (pNumRec[2] == 0)))
    {
        if ((pRecSize[0] == 0) && (pRecSize[1] == 0))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }

    if ((bCommOption & 0x0FU) == PH_EXCHANGE_RXCHAINING)
    {
        bCmdBuff[wCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if ((bCommOption & 0x0FU) == PH_EXCHANGE_DEFAULT)
    {
        /* form the command */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_READ_RECORDS;
        bCmdBuff[wCmdLen++] = bFileNo;

        /* Offset */
        memcpy(&bCmdBuff[wCmdLen], pOffset, 3); /* PRQA S 3200 */
        wCmdLen += 3;

        /* Length */
        memcpy(&bCmdBuff[wCmdLen], pNumRec, 3); /* PRQA S 3200 */
        wCmdLen += 3;
        /* Calculate the number of bytes. numRecs * rec size */
        wNumBytes = pRecSize[1];
        wNumBytes <<= 8;
        wNumBytes |= pRecSize[0];

        wNumRec = pNumRec[1];
        wNumRec <<= 8;
        wNumRec |= pNumRec[0];

        wNumBytes = wNumBytes * wNumRec;
        pRecSize[0] = (uint8_t)wNumBytes;
        pRecSize[1] = (uint8_t)(wNumBytes>>8);
        pRecSize[2] = 0x00;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        if ((pNumRec[0] == 0x00) && (pNumRec[1] == 0x00) && (pNumRec[2] == 0x00))
        {
            bCommOption |= PH_CRYPTOSYM_PADDING_MODE_2;
        }
        else
        {
            bCommOption |= PH_CRYPTOSYM_PADDING_MODE_1;
        }
        status = phalMfdf_SamAV2_Int_ReadData_Enc(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            pRecSize, /* Number of bytes to be read */
            ppRxdata,
            pRxdataLen
            );
    }
    else if (((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_PLAIN) ||
        ((bCommOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_MACD))
    {
        status = phalMfdf_SamAV2_Int_ReadData_Plain(
            pDataParams,
            bCommOption,
            bCmdBuff,
            wCmdLen,
            ppRxdata,
            pRxdataLen
            );
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((status == PH_ERR_SUCCESS) && (bCmdBuff[0] != PHAL_MFDF_RESP_ADDITIONAL_FRAME))
    {
        /* Can check this case if user has not given num of records
        as 0x000000. If 0x000000, then all records are read */
        if ((wNumBytes != *pRxdataLen) && (wNumBytes != 0))
        {
            /* Reset authentication status */
            if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
                (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
            {
                phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
                status = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
                    pDataParams->pHalSamDataParams,
                    0x00); 
            }
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
    }
    return status;
}

phStatus_t phalMfdf_SamAV2_ClearRecordFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bFileNo
    )
{
    uint8_t PH_MEMLOC_REM bCmdBuff[8];
    uint16_t PH_MEMLOC_REM wCmdLen = 0;

    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* form the command */
    bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_CLEAR_RECORDS_FILE;
    bCmdBuff[wCmdLen++] = bFileNo;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        wCmdLen,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_CommitTransaction(
    phalMfdf_SamAV2_DataParams_t * pDataParams
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_COMMIT_TXN;    

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        0x0001,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_AbortTransaction(
    phalMfdf_SamAV2_DataParams_t * pDataParams
    )
{
    uint8_t     PH_MEMLOC_REM bCmdBuff[8];

    /* form the command */
    bCmdBuff[0] = PHAL_MFDF_CMD_ABORT_TXN;

    /* COMMUNICATION IS PLAIN */
    return phalMfdf_SamAV2_Int_Write_Plain(
        pDataParams,
        bCmdBuff,
        0x0001,
        PHAL_MFDF_COMMUNICATION_PLAIN,
        NULL,
        0x0000
        );
}

phStatus_t phalMfdf_SamAV2_IsoSelectFile(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bSelector,
    uint8_t *pFid,
    uint8_t * pDFname,
    uint8_t bDFnameLen,
    uint8_t **ppRecv,
    uint16_t * pwRxlen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bData[24];
    uint8_t     PH_MEMLOC_REM bLc = 0;

    if (bDFnameLen > 16)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bOption != 0x00) && (bOption != 0x0C))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    switch(bSelector)
    {
    case 0x00:
    case 0x02:
        /* Selection by EF Id*/
        /* Reverse before send */
        bData[0] = pFid[1];
        bData[1] = pFid[0];
        bLc = 2;
        break;

    case 0x04:
        /* Selection by DF Name */
        memcpy(bData, pDFname, bDFnameLen); /* PRQA S 3200 */
        bLc = bDFnameLen;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* Reset Authentication Status here */
    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x03,
        PHAL_MFDF_CMD_ISO7816_SELECT_FILE,
        bSelector,
        bOption,
        bLc,
        bData,
        0x00,
        ppRecv,
        pwRxlen));

    /* ISO wrapped mode is on */
    pDataParams->bWrappedMode = 1;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_IsoReadBinary(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bOffset,
    uint8_t bSfid,
    uint8_t bBytesToRead,
    uint8_t ** ppRxBuffer,
    uint16_t * pBytesRead
    )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLe = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (wOption == PH_EXCHANGE_DEFAULT)
    {
        if (bSfid & 0x80)
        {
            /* Short file id is supplied */
            if ((bSfid & 0x7FU) > 0x1F)
            {
                /* Error condition */
                return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
            }
        }

        bP1 = bSfid;
        bP2 = bOffset;

        bLe = bBytesToRead;
        bCmdBuff[wCmdLen++] = 0x00; /* Class */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_BINARY; /* Ins */
        bCmdBuff[wCmdLen++] = bP1;
        bCmdBuff[wCmdLen++] = bP2;
        bCmdBuff[wCmdLen++] = bBytesToRead; /* Le */
    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        wCmdLen = 0;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    return phalMfdf_SamAV2_Int_IsoRead(pDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        );
}

phStatus_t phalMfdf_SamAV2_IsoUpdateBinary(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bOffset,
    uint8_t bSfid,
    uint8_t * pData,
    uint8_t bDataLen
    )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;

    if (bSfid & 0x80)
    {
        /* Short file id is supplied */
        if ((bSfid & 0x7FU) > 0x1F)
        {
            /* Error condition */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }

    bP1 = bSfid;
    bP2 = bOffset;
    bLc = bDataLen;

    status = phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_UPDATE_BINARY,
        bP1,
        bP2,
        bLc,pData,0x00,NULL,NULL);
    if (status != PH_ERR_SUCCESS)
    {
        /* Reset authentication status */
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
            pDataParams->pHalSamDataParams,
            0x00); 
    }
    return status;
}

phStatus_t phalMfdf_SamAV2_IsoReadRecords(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bRecNo,
    uint8_t bReadAllFromP1,
    uint8_t bSfid,
    uint8_t bBytesToRead,
    uint8_t ** ppRxBuffer,
    uint16_t * pBytesRead
    )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bCmdBuff[16];
    uint16_t    PH_MEMLOC_REM wCmdLen = 0;

    if (wOption == PH_EXCHANGE_DEFAULT)
    {
        if (bSfid > 0x1F)
        {
            /* Invalid Short File Id */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        else
        {
            /* Valid Sfid */
            bP1 = bRecNo;
            bP2 = bSfid;
            bP2 <<= 3; /* left shift by 3 bits to move SFID to bits 7 to 3 */
            if (bReadAllFromP1)
            {
                bP2 |= 0x05; /* Last three bits of P2 = 101 */
            }
            else
            {
                bP2 |= 0x04; /* Last three bits of P2 = 100. Read only record P1 */
            }
        }

        bCmdBuff[wCmdLen++] = 0x00; /* Class */
        bCmdBuff[wCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_RECORDS; /* Ins */
        bCmdBuff[wCmdLen++] = bP1;
        bCmdBuff[wCmdLen++] = bP2;
        bCmdBuff[wCmdLen++] = bBytesToRead; /* Le */

    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        wCmdLen = 0;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    return phalMfdf_SamAV2_Int_IsoRead(pDataParams,
        wOption,
        bCmdBuff,
        wCmdLen,
        ppRxBuffer,
        pBytesRead
        ); 
}

phStatus_t phalMfdf_SamAV2_IsoAppendRecord(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t bSfid,
    uint8_t * pData,
    uint8_t bDataLen
    )
{
    uint8_t     PH_MEMLOC_REM bP1 = 0;
    uint8_t     PH_MEMLOC_REM bP2 = 0;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM statusTmp;

    if (bSfid > 0x1F)
    {
        /* Invalid Short File Id */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    bP2 = bSfid;
    bP2 <<= 3; /* left shift by 3 bits to move SFID to bits 7 to 3 */
    /* Last three bits of P2 = 000 */

    bLc = bDataLen;
    status = phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_APPEND_RECORD,
        bP1,
        bP2,
        bLc,
        pData,
        0x00,
        NULL,
        NULL);
    if (status != PH_ERR_SUCCESS)
    {
        /* Reset authentication status */
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
            pDataParams->pHalSamDataParams,
            0x00); 
    }
    return status;
}

phStatus_t phalMfdf_SamAV2_IsoGetChallenge(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bLe,
    uint8_t * pRPICC1
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bData[16];
    uint8_t     PH_MEMLOC_REM bTmp;
    uint16_t    PH_MEMLOC_REM wHostMode;

    PHAL_MFDF_UNUSED_VARIABLE(wKeyVer);

    if ((wKeyNo > 0x7F) || (wKeyVer > 0xFF) || ((bLe != 8) && (bLe != 16)))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* Getkey entry from SAM to switch the key type */
    PH_CHECK_SUCCESS_FCT(statusTmp,
        phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams->pHalSamDataParams,
        (uint8_t)wKeyNo,
        bData,
        &bTmp
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_DetectMode(
                                     pDataParams->pHalSamDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_HOSTMODE,
        &wHostMode));

    /* bWorkBuffer[keyentrylength - 2] should have the key type in bits 3 to 5 */
    if (wHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE )
    {
        bTmp = bData[bTmp - 3];
    }
    else
    {
        bTmp = bData[bTmp - 2];
    }

    bTmp &= 0x38;  /* 00011 1000 */
    bTmp >>= 3;

    switch(bTmp)
    {
    case 0x00:
    case 0x06:
        /* 2K3DES keys */
        bLe = 0x08;
        break;

    case 0x03:
        /* 3K3DES KEYS */
        bLe = 0x10;
        break;

    case 0x04:
        /* AES KEYS */
        bLe = 0x10;
        break;

    default:
        /* Wrong key type specified. Auth. will not work */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }   

    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x02,
        PHAL_MFDF_CMD_ISO7816_GET_CHALLENGE,
        0x00,
        0x00,
        0x00,
        NULL,
        bLe,
        &pRecv,
        &wRxlen));
    if (wRxlen != bLe)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }

    memcpy(pRPICC1, pRecv, wRxlen); /* PRQA S 3200 */

    /* Reset authentication status */
    phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_IsoExternalAuthenticate(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pInput,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    /* 
    Input is Rpicc1.
    Output is status from card and also Rpcd2.

    Call phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part1 with Rpicc1 as input.
    SAM returns eK(Rpcd1 || Rpicc1) || Rpcd2.
    Store Rpcd2.
    Send eK(Rpcd1 || Rpicc1) to PICC with ISOEXTERNAL_AUTHENTICATE command.
    If status is 0x9000, then return success and also Rpcd2.
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint16_t    PH_MEMLOC_REM wKeyNo;
    uint16_t    PH_MEMLOC_REM wKeyVer;
    uint8_t     PH_MEMLOC_REM bEncRndLen;
    uint8_t     PH_MEMLOC_REM bIndex = 0;
    uint8_t     PH_MEMLOC_REM bKeyNoCard;
    uint8_t     PH_MEMLOC_REM bIsDFkey;
    uint8_t     PH_MEMLOC_REM bAlgo;
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM *pRecv;
    uint8_t     PH_MEMLOC_REM bRpcd2[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bRpicc1[PH_CRYPTOSYM_AES128_KEY_SIZE];
    uint8_t     PH_MEMLOC_REM bEncRnd[3 * PH_CRYPTOSYM_AES128_KEY_SIZE]; /* maximum = 48B. To hold three random numbers */

    if ((bInputLen != 16) && (bInputLen != 24))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    bAlgo = pInput[bIndex++];
    bIsDFkey = pInput[bIndex++];
    bKeyNoCard = pInput[bIndex++];
    bRndLen = pInput[bIndex++];

    memcpy(bRpicc1, &pInput[bIndex], bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    memcpy(&wKeyNo, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;

    memcpy(&wKeyVer, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;

    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Returns a cryptogram consisting of ek(Rpcd1||Rpicc1) || Rpcd2 */
    status = phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part1(
        pDataParams->pHalSamDataParams,
        0x00, /* = 0x00. No key diversification. Key selection by entry number */
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        bRpicc1,
        bRndLen,
        NULL,
        0x00,
        bEncRnd, /* Rpcd2 is the last part of this data. Store this */
        &bEncRndLen
        );

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        statusTmp = phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
            pDataParams->pHalSamDataParams,
            0x00); 
        return status;
    }
    if (bEncRndLen != (3 * bRndLen))
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }

    /* Extract the Rpcd2 from bEncRnd. bEncRndLen should either be 16*3 = 48B or 8*3=24B */
    memcpy(bRpcd2, &bEncRnd[bEncRndLen - bRndLen], bRndLen);  /* PRQA S 3200 */


    /* Send the ek(Rpcd1||Rpicc1) to PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_EXT_AUTHENTICATE,
        bAlgo,
        (bIsDFkey << 7) | bKeyNoCard,
        bEncRndLen - bRndLen, /* Send only eK(Rpcd1 || Rpicc1) */
        bEncRnd,
        0x00,
        &pRecv,
        &wRxlen
        ));

    if (wRxlen != 0)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Return bRpcd2 */
    memcpy(pDataOut, bRpcd2, bRndLen); /* PRQA S 3200 */
    *pOutLen = bRndLen;

    /* External Authenticate done */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_IsoInternalAuthenticate(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint8_t * pInput,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    /* 
    Input is Rpcd2.
    Output is status of verification from SAM.

    Send Rpcd2 to PICC with command INTERNAL_AUTHENTICATE.
    PICC returns eK(Rpcd2 || Rpicc2)    

    Call phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part2 with eK(Rpcd1 || Rpcd1) as input.
    Map and return the status returned by PICC.
    */
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wRxlen = 0;
    uint16_t    PH_MEMLOC_REM wHostMode;
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint16_t    PH_MEMLOC_REM wKeyNo;
    uint16_t    PH_MEMLOC_REM wKeyVer;
    uint8_t     PH_MEMLOC_REM bIndex = 0;
    uint8_t     PH_MEMLOC_REM bKeyNoCard;
    uint8_t     PH_MEMLOC_REM bIsDFkey;
    uint8_t     PH_MEMLOC_REM bAlgo;
    uint8_t     PH_MEMLOC_REM bRndLen;
    uint8_t     PH_MEMLOC_REM bRpcd2[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t     PH_MEMLOC_REM bData[PH_CRYPTOSYM_AES_BLOCK_SIZE];

    if ((bInputLen != 16) && (bInputLen != 24))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if (pDataOut)
    {
        *pDataOut = 0; /* For Sam non x mode, no data is returned in pResp*/
    }
    *pOutLen = 0;

    bAlgo = pInput[bIndex++];
    bIsDFkey = pInput[bIndex++];
    bKeyNoCard = pInput[bIndex++];
    bRndLen = pInput[bIndex++];

    memcpy(bRpcd2, &pInput[bIndex], bRndLen); /* PRQA S 3200 */
    bIndex = bIndex + bRndLen;

    /* Key no. and version are also not required in sam non x mode */
    memcpy(&wKeyNo, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;

    memcpy(&wKeyVer, &pInput[bIndex], 2); /* PRQA S 3200 */
    bIndex += 2;


    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Send Rpcd2 to PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x03,
        PHAL_MFDF_CMD_ISO7816_INT_AUTHENTICATE,
        bAlgo,
        (bIsDFkey << 7) | bKeyNoCard,
        bRndLen, /* Length of random number */
        bRpcd2,
        bRndLen * 2, /* Expected length of response */
        &pRecv,
        &wRxlen));

    if (wRxlen != (2 * bRndLen))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* pRecv will have ek(Rpicc2 || Rpcd2). Send this to SAM to complete part2 of authentication */
    status = phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part2(
        pDataParams->pHalSamDataParams,
        pRecv,
        (uint8_t)wRxlen
        );

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return statusTmp;
    }
    /* Getkey entry from SAM to switch the key type */
    PH_CHECK_SUCCESS_FCT(statusTmp,
        phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams->pHalSamDataParams,
        (uint8_t)wKeyNo,
        bData,
        &bTmp
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_DetectMode(
                                     pDataParams->pHalSamDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_HOSTMODE,
        &wHostMode));

    /* bWorkBuffer[keyentrylength - 2] should have the key type in bits 3 to 5 */
    if (wHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE )
    {
        bTmp = bData[bTmp - 3];
    }
    else
    {
        bTmp = bData[bTmp - 2];
    }
    bTmp &= 0x38;  /* 00011 1000 */
    bTmp >>= 3;

    switch(bTmp)
    {
    case 0x06:
    case 0x03:
        /* 2K3DES keys or 3K3DES */
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
        break;

    case 0x04:
        /* AES KEYS */
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES;
        break;

    default:
        /* Wrong key type specified. Auth. will not work 
         * case 00, DF Native keys are also not supported 
         * in this mode.
         */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }

    pDataParams->bKeyNo = bKeyNoCard;
    pDataParams->bWrappedMode = 1;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_IsoAuthenticate(
    phalMfdf_SamAV2_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t isPICCkey
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bEncRndLen;
    uint8_t     PH_MEMLOC_REM bAlgo = 0x00;
    uint8_t     PH_MEMLOC_REM bAuthMode = 0x00;
    uint8_t     PH_MEMLOC_REM bTmp;
    uint8_t     PH_MEMLOC_REM bLe;
    uint8_t     PH_MEMLOC_REM bData[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint16_t    PH_MEMLOC_REM wHostMode;
    uint8_t     PH_MEMLOC_REM bRpcd2[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bRpicc1[PH_CRYPTOSYM_AES_BLOCK_SIZE];
    uint8_t     PH_MEMLOC_REM bEncRnd[PH_CRYPTOSYM_AES_BLOCK_SIZE * 3]; /* Should hold max of 3 rnd numbers of 16B */

    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wKeyNo > 0x7F) || (wKeyVer > 0xFF))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Getkey entry from SAM to switch the key type */
    PH_CHECK_SUCCESS_FCT(statusTmp,
        phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
        pDataParams->pHalSamDataParams,
        (uint8_t)wKeyNo,
        bData,
        &bTmp
        ));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_DetectMode(
                                     pDataParams->pHalSamDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalSamDataParams,
        PHHAL_HW_SAMAV2_CONFIG_HOSTMODE,
        &wHostMode));

    /* bWorkBuffer[keyentrylength - 2] should have the key type in bits 3 to 5 */
    if (wHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE )
    {
        bTmp = bData[bTmp - 3];
    }
    else
    {
        bTmp = bData[bTmp - 2];
    }
    bTmp &= 0x38;  /* 00011 1000 */
    bTmp >>= 3;

    switch(bTmp)
    {
    case 0x06:
        /* 2K3DES keys */
        bLe = 0x08;
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
        break;

    case 0x03:
        /* 3K3DES KEYS */
        bLe = 0x10;
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
        break;

    case 0x04:
        /* AES KEYS */
        bLe = 0x10;
        pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES;
        break;

    default:
        /* Wrong key type specified. Auth. will not work 
         * case 00, DF Native keys are also not supported 
         * in this mode.
         */
        return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_AL_MFDF);
    }   

    /* Get Challenge. Returns Rpicc1 from card */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x02,
        PHAL_MFDF_CMD_ISO7816_GET_CHALLENGE,
        0x00,
        0x00,
        0x00,
        NULL,
        bLe,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != bLe)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    memcpy(bRpicc1, pRecv, wRxlen); /* PRQA S 3200 */

    /* RPICC1 received from Card */

    /* Returns a cryptogram consisting of ek(Rpcd1||Rpicc1) || Rpcd2 */
    status = phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part1(
        pDataParams->pHalSamDataParams,
        bAuthMode, /* = 0x00. No key diversification. Key selection by entry number */
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        bRpicc1,
        (uint8_t)wRxlen,
        NULL,
        0x00,
        bEncRnd, /* Rpcd2 is the last part of this data. Store this */
        &bEncRndLen
        );

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return status;
    }
    if (bEncRndLen != (3 * bLe))
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    /* Extract the Rpcd2 from bEncRnd. bEncRndLen should either be 16*3 = 48B or 8*3=24B */
    if (bEncRndLen == PH_CRYPTOSYM_AES128_KEY_SIZE * 3)
    {
        memcpy(bRpcd2, &bEncRnd[bEncRndLen - PH_CRYPTOSYM_AES_BLOCK_SIZE], PH_CRYPTOSYM_AES_BLOCK_SIZE);  /* PRQA S 3200 */
        bEncRndLen -= PH_CRYPTOSYM_AES_BLOCK_SIZE;
    }
    else
    {
        memcpy(bRpcd2, &bEncRnd[bEncRndLen - PH_CRYPTOSYM_DES_BLOCK_SIZE], PH_CRYPTOSYM_DES_BLOCK_SIZE);  /* PRQA S 3200 */
        bEncRndLen -= PH_CRYPTOSYM_DES_BLOCK_SIZE;
    }    

    /* Send the ek(Rpcd1||Rpicc1) to PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x01,
        PHAL_MFDF_CMD_ISO7816_EXT_AUTHENTICATE,
        bAlgo,
        (!isPICCkey << 7) | bKeyNoCard,
        bEncRndLen,
        bEncRnd,
        0x00,
        &pRecv,
        &wRxlen
        ));
    if (wRxlen != 0)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* External Authenticate done */

    /* Begin Internal Authenticate */
    /* Send Rpcd2 to PICC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_Int_Send7816Apdu(
        pDataParams,
        pDataParams->pPalMifareDataParams,
        0x03,
        PHAL_MFDF_CMD_ISO7816_INT_AUTHENTICATE,
        bAlgo,
        (!isPICCkey << 7) | bKeyNoCard,
        bEncRndLen / 2, /* Length of random number */
        bRpcd2,
        bEncRndLen, /* Expected length of response */
        &pRecv,
        &wRxlen));

    if (wRxlen != bEncRndLen)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }
    /* pRecv will have ek(Rpicc2 || Rpcd2). Send this to SAM to complete part2 of authentication */
    status = phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part2(
        pDataParams->pHalSamDataParams,
        pRecv,
        (uint8_t)wRxlen
        );

    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        phalMfdf_SamAV2_Int_ResetAuthStatus(pDataParams);
        return status;
    }
    pDataParams->bKeyNo = bKeyNoCard;
    pDataParams->bWrappedMode = 1;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

/*
* Get config
*/
phStatus_t phalMfdf_SamAV2_GetConfig(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wConfig,
                                     uint16_t * pValue
                                     )
{
    switch (wConfig)
    {
    case PHAL_MFDF_ADDITIONAL_INFO:
        *pValue = pDataParams->wAdditionalInfo;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_SetConfig(
                                     phalMfdf_SamAV2_DataParams_t * pDataParams,
                                     uint16_t wConfig,
                                     uint16_t wValue
                                     )
{
    switch (wConfig)
    {
    case PHAL_MFDF_ADDITIONAL_INFO:
        pDataParams->wAdditionalInfo = wValue;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);    
}

#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */
