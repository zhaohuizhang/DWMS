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

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <ph_TypeDefs.h>
#include <string.h>
#include <phTools.h>

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X

#include "../phalMfdf_Int.h"
#include "phalMfdf_SamAV2_X.h"
#include "phalMfdf_SamAV2_X_Int.h"

phStatus_t phalMfdf_SamAV2_X_Init(
                                  phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                  uint16_t wSizeOfDataParams,
                                  phhalHw_SamAV2_DataParams_t * pHalSamDataParams
                                  )
{
    /* data param check */
    if (sizeof(phalMfdf_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalSamDataParams);

    /* init private data */
    pDataParams->wId                  = PH_COMP_AL_MFDF | PHAL_MFDF_SAMAV2_X_ID;
    pDataParams->pHalSamDataParams    = pHalSamDataParams;
    pDataParams->bAuthMode            = PHAL_MFDF_NOT_AUTHENTICATED; /* Set to invalid */
    pDataParams->bKeyNo               = 0xFF; /* Set to Invalid key */
    pDataParams->bWrappedMode         = 0x00; /* Set to false */
    pDataParams->wAdditionalInfo      = 0x0000;
    /* Default selected application */
    memset(pDataParams->pAid, 0x00, 3); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_Authenticate(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bAuthMode = 0;
    uint8_t     PH_MEMLOC_REM bISOMode = 0;

    /* Check for valid card key number. */
    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Check for valid SAM keystore number and version. */
    if ((wKeyNo > 0x7f) || (wKeyVer > 0xff))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) &&
        (wOption != (PH_CRYPTOSYM_DIV_MODE_DESFIRE | PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF)) &&
        (wOption != (PH_CRYPTOSYM_DIV_MODE_DESFIRE | PH_CRYPTOSYM_DIV_OPTION_2K3DES_FULL)) &&        
        (wOption != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && ((bDivLen > 31) ||(bDivLen < 1)))
    {
        /* Diversification input should be less than or equal to 31 bytes. */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Unset the current authentication status. */
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED;
    pDataParams->bKeyNo = 0xFF;

    if (bDivLen > 0)
    {
        /* Set bit 0 to one  for diversification input. */
        bAuthMode = (uint8_t)(bAuthMode | 0x01);
    }

    /* Key selection always by entry number, set bit one to zero. */
    bAuthMode = (uint8_t)(bAuthMode & 0xfd);

    /* Diversification option. */
    if (wOption == PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF)
    {
        bAuthMode = (uint8_t)(bAuthMode | 0x08);
    }

    if (wOption == PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS)
    {
        bAuthMode = (uint8_t)(bAuthMode | 0x10);
    }

    /* Set ISO mode to 0x40 for wrapped APDU. */
    pDataParams->bWrappedMode?(bISOMode = 0x40):(bISOMode = 0x00);

    status = phhalHw_SamAV2_Cmd_DESFireAuthenticatePICC(
        pDataParams->pHalSamDataParams,
        bAuthMode,
        bISOMode,
        bKeyNoCard,
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        pDivInput,
        bDivLen);

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATE;
    pDataParams->bKeyNo = bKeyNoCard;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_AuthenticateISO(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bAuthMode = 0;
    uint8_t     PH_MEMLOC_REM bISOMode = 0;

    /* Check for valid card key number. */
    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Check for valid SAM keystore number and version. */
    if ((wKeyNo > 0x7f) || (wKeyVer > 0xff))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) &&
        (wOption != (PH_CRYPTOSYM_DIV_MODE_DESFIRE | PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF)) &&
        (wOption != (PH_CRYPTOSYM_DIV_MODE_DESFIRE | PH_CRYPTOSYM_DIV_OPTION_2K3DES_FULL)) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && ((bDivLen > 31) ||(bDivLen < 1)))
    {
        /* Diversification input should be less than or equal to 31 bytes. */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Unset the current authentication status. */
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED;
    pDataParams->bKeyNo = 0xFF;

    if (bDivLen > 1)
    {
        /* Set bit 0 to one  for Diversification input. */
        bAuthMode = (uint8_t)(bAuthMode | 0x01);
    }

    /* Key selection always by entry number, set bit one to zero. */
    bAuthMode = (uint8_t)(bAuthMode & 0xfd);

    /* Diversification option. */
    if (wOption == PH_CRYPTOSYM_DIV_OPTION_2K3DES_HALF)
    {
        bAuthMode = (uint8_t)(bAuthMode | 0x08);
    }

    if (wOption == PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS)
    {
        bAuthMode = (uint8_t)(bAuthMode | 0x10);
    }

    /* Set ISO mode to 0x40 for wrapped APDU. */
    pDataParams->bWrappedMode ? (bISOMode = 0x40) : (bISOMode = 0x00);

    status = phhalHw_SamAV2_Cmd_DESFireAuthenticatePICC(
        pDataParams->pHalSamDataParams,
        bAuthMode,
        bISOMode,
        bKeyNoCard,
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        pDivInput,
        bDivLen);

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;
    pDataParams->bKeyNo = bKeyNoCard;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_AuthenticateAES(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t * pDivInput,
    uint8_t bDivLen
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp = 0;
    phStatus_t PH_MEMLOC_REM status = 0;
    uint8_t    PH_MEMLOC_REM bAuthMode = 0;
    uint8_t    PH_MEMLOC_REM bISOMode = 0;

    /* Check for valid card key number. */
    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Check for valid SAM keystore number and version. */
    if ((wKeyNo > 0x7f ) || (wKeyVer > 0xff))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_DESFIRE) &&
        (wOption != PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((wOption != PHAL_MFDF_NO_DIVERSIFICATION) && ((bDivLen > 31) ||(bDivLen < 1)))
    {
        /* Diversification input should be less than or equal to 31 bytes. */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Unset the current authentication status */
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED;
    pDataParams->bKeyNo = 0xFF;

    if (bDivLen > 1)
    {
        /* Set bit 0 to one  for Diversification input. */
        bAuthMode = (uint8_t)(bAuthMode | 0x01);
    }

     /* Key selection always by entry number, set bit one to zero. */
    bAuthMode = (uint8_t)(bAuthMode & 0xfd);

    /* Diversification option. */
    if (wOption == PH_CRYPTOSYM_DIV_MODE_MIFARE_PLUS)
    {
        bAuthMode = (uint8_t)(bAuthMode | 0x10);
    }

    /* Set ISO mode to 0x40 for wrapped APDU. */
    pDataParams->bWrappedMode?(bISOMode = 0x40):(bISOMode = 0x00);

    status = phhalHw_SamAV2_Cmd_DESFireAuthenticatePICC(
        pDataParams->pHalSamDataParams,
        bAuthMode,
        bISOMode,
        bKeyNoCard,
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        pDivInput,
        bDivLen);

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEAES;
    pDataParams->bKeyNo = bKeyNoCard;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_ChangeKeySettings(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bKeySettings
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    if(pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
     {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
     }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer */
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CHANGE_KEY_SETTINGS; /* Encode changekey settings command. */
    bCrypto = PHAL_MFDF_ENC_FLAG | bCmdLen ; /* Encrypt key settings. */
    bFrame[bCmdLen++] = bKeySettings; /* Encode the change key settings. */

    if (pDataParams->bWrappedMode)
    {
        /* Wrap  APDU into 7816 APDU*/
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_GetKeySettings(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pResponse
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the Get key settings command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_KEY_SETTINGS;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap  APDU into 7816 APDU*/
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    if (wRxFrameLen != 0x02)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRxFrame, wRxFrameLen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_ChangeKey(
                                       phalMfdf_SamAV2_X_DataParams_t * pDataParams,
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
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bKeyCompMeth = 0;
    uint8_t     PH_MEMLOC_REM bCfg = 0;

    if ((pDataParams->pAid[0] == 0x00) && (pDataParams->pAid[1] == 0x00) &&
        (pDataParams->pAid[2] == 0x00) && ((bKeyNoCard & 0x3FU) == 0x00))
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

    /* Check for valid SAM key number and version. */
    if ((wOldKeyNo > 0x7f) || (wOldKeyVer > 0xff) ||
        (wNewKeyNo > 0x7f) || (wNewKeyVer > 0xff))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Set the key compilation method. */
    switch(wOption)
    {
    case PHAL_MFDF_NO_DIVERSIFICATION:
        bKeyCompMeth = 0x00;
        break;

    case (PHAL_MFDF_CHGKEY_DIV_NEW_KEY | PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC):
        bKeyCompMeth = bKeyCompMeth | 0x02;
        bKeyCompMeth = bKeyCompMeth | 0x20;
        break;

    case (PHAL_MFDF_CHGKEY_DIV_NEW_KEY | PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND):
        bKeyCompMeth = bKeyCompMeth | 0x02;
        bKeyCompMeth = bKeyCompMeth | 0x08;
        break;
    case PHAL_MFDF_CHGKEY_DIV_NEW_KEY:
        bKeyCompMeth = bKeyCompMeth | 0x02;
        break;
    case (PHAL_MFDF_CHGKEY_DIV_OLD_KEY | PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC):
        bKeyCompMeth = bKeyCompMeth | 0x04;
        bKeyCompMeth = bKeyCompMeth | 0x20;
        break;

    case PHAL_MFDF_CHGKEY_DIV_OLD_KEY :
        bKeyCompMeth = bKeyCompMeth | 0x04;
        break;

    case (PHAL_MFDF_CHGKEY_DIV_OLD_KEY | PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND) :
        bKeyCompMeth = bKeyCompMeth | 0x04;
        bKeyCompMeth = bKeyCompMeth | 0x10;
        break;

    case (PHAL_MFDF_CHGKEY_DIV_OLD_KEY | PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND|
        PHAL_MFDF_CHGKEY_DIV_NEW_KEY) :
        bKeyCompMeth = bKeyCompMeth | 0x04;
        bKeyCompMeth = bKeyCompMeth | 0x10;
        bKeyCompMeth = bKeyCompMeth | 0x02;
        break;

    case (PHAL_MFDF_CHGKEY_DIV_NEW_KEY | PHAL_MFDF_CHGKEY_DIV_OLD_KEY | 
          PHAL_MFDF_CHGKEY_DIV_METHOD_CMAC):
        bKeyCompMeth = bKeyCompMeth | 0x02;
        bKeyCompMeth = bKeyCompMeth | 0x04;
        bKeyCompMeth = bKeyCompMeth | 0x20;
        break;

    case (PHAL_MFDF_CHGKEY_DIV_NEW_KEY | PHAL_MFDF_CHGKEY_DIV_OLD_KEY | 
          PHAL_MFDF_CHGKEY_DIV_NEW_KEY_ONERND | PHAL_MFDF_CHGKEY_DIV_OLD_KEY_ONERND):
        bKeyCompMeth = bKeyCompMeth | 0x02;
        bKeyCompMeth = bKeyCompMeth | 0x04;
        bKeyCompMeth = bKeyCompMeth | 0x10;
        bKeyCompMeth = bKeyCompMeth | 0x08;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ( bDivLen > 31 )
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Updating the SAM key store is already done by the Application */
    /* Desfire keynumber to be changed. */
    bCfg = bCfg | (0x0f & bKeyNoCard);

    /* SEt bit one if it is master key. */
    if ((bKeyNoCard & 0x3f)== 0)
    {
        bKeyCompMeth = bKeyCompMeth | 0x01;
    }

    /* Check for PICC master key */
    if ((pDataParams->pAid[0] == 0x00) &&
        (pDataParams->pAid[1] == 0x00) &&
        (pDataParams->pAid[2] == 0x00) )
    {
        bCfg = bCfg | 0x10;
    }

    if (pDataParams->bWrappedMode)
    {
        /* Set to send APDU using ISO7816-4 APDU */
        bCfg = (uint8_t)(bCfg | 0x40);
    }

    status = phhalHw_SamAV2_Cmd_DESFireChangeKeyPICC(
        pDataParams->pHalSamDataParams,
        bKeyCompMeth,
        bCfg,
        (uint8_t)wOldKeyNo,
        (uint8_t)wOldKeyVer,
        (uint8_t)wNewKeyNo,
        (uint8_t)wNewKeyVer,
        pDivInput,
        bDivLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_GetKeyVersion(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t * pResponse
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid card key number. */
    if (bKeyNo > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the change key settings command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_KEY_VERSION;
    bFrame[bCmdLen++] = bKeyNo;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
        bFrame[bCmdLen++] = 0x00;
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    if (wRxFrameLen != 0x01)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pResponse, pRxFrame, wRxFrameLen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

/*
* PICC level commands
*/
phStatus_t phalMfdf_SamAV2_X_CreateApplication(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pAid,
    uint8_t bKeySettings1,
    uint8_t bKeySettings2,
    uint8_t * pISOFileId,
    uint8_t * pISODFName,
    uint8_t bISODFNameLen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid ISO DFName */
    if ((bISODFNameLen > 16) || (bOption == 0x02) || (bOption > 0x03))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN);  /* PRQA S 3200 */

    /* Encode the create application command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREATE_APPLN;
    memcpy(bFrame + bCmdLen, pAid, 3); /* PRQA S 3200 */
    bCmdLen += 3;
    bFrame[bCmdLen++] = bKeySettings1;
    bFrame[bCmdLen++] = bKeySettings2;

    /* Encode ISO file Id*/
    if (((bOption & 0x01 ) == 0x01) &&
        (pISOFileId!=NULL))
    {
        bFrame[bCmdLen++] = pISOFileId[0];
        bFrame[bCmdLen++] = pISOFileId[1];
    }

    /* Encode ISODF Name*/
    if (((bOption & 0x02) == 0x02) &&
        (bISODFNameLen!= 0))
    {
        memcpy(bFrame + bCmdLen, pISODFName, bISODFNameLen); /* PRQA S 3200 */
        bCmdLen = bCmdLen + bISODFNameLen;
    }

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_DeleteApplication(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pAid
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the delete application command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_DELETE_APPLN;
    memcpy(bFrame + bCmdLen, pAid, 3); /* PRQA S 3200 */
    bCmdLen += 3;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_GetApplicationIDs(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pAidBuff,
    uint8_t * pNumAIDs
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint8_t     PH_MEMLOC_REM bOption = PH_EXCHANGE_DEFAULT;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_APPLN_IDS;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        bOption,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) && 
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        /* Map Sam error code to MFDF error codes. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    }

    /* Copy the appliation ID's*/
    memcpy(pAidBuff, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
    pAidBuff += wRxFrameLen;
    *pNumAIDs = (uint8_t)wRxFrameLen / 3;

    /* Check if more application ID's to be received*/
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        bOption = PH_EXCHANGE_RXCHAINING;
        /* Init wrapped header length for wrapped mode. */
        if (pDataParams->bWrappedMode)
        {
            bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
        }

        /* Initialize the buffer*/
        memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

        /* Encode the additional frame command*/
        bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

        if (pDataParams->bWrappedMode)
        {
            /* Wrap APDU using ISO7816-4 APDU */
            bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
            bOption = PH_EXCHANGE_DEFAULT;
        }

        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            bOption,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRxFrame,
            &wRxFrameLen);

        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
        {
            /* Map Sam error code to MFDF error codes. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
        }

        memcpy(pAidBuff, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
        *pNumAIDs += (uint8_t)wRxFrameLen / 3;

    }/*End of if status == PHAL_MFDF_RESP_ADDITIONAL_FRAME*/

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetDFNames(
                                        phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t bOption,
                                        uint8_t * pDFBuffer,
                                        uint8_t * bSize
                                        )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    if ((pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEISO) ||
        (pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES))
    {
        /* Should return, invalid scenario error */
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    if (bOption == PH_EXCHANGE_RXCHAINING)
    {
        bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else if (bOption == PH_EXCHANGE_DEFAULT)
    {
        /* Encode the get DF NAMES command*/
        bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_DF_NAMES;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
        bOption = PH_EXCHANGE_DEFAULT;
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        bOption,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );
    /* check for protocol errors */
    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) &&
        (wRxFrameLen == 0))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
    }

    if (((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ||
         ((status & PH_ERR_MASK) == PH_ERR_SUCCESS))
    {
        /* Copy the DF Names*/
        memcpy(pDFBuffer, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
        *bSize = (uint8_t) wRxFrameLen;
    }

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_SelectApplication(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pAppId
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /*Reset authentication status*/
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN);  /* PRQA S 3200 */

    /* Encode the select application command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_SELECT_APPLN;
    memcpy(bFrame + bCmdLen, pAppId, 3); /* PRQA S 3200 */
    bCmdLen += 3;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    /* Store the selected application */
    memcpy(pDataParams->pAid, pAppId, 3); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_FormatPICC(
                                        phalMfdf_SamAV2_X_DataParams_t * pDataParams
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM * pDataOut;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmd = PHAL_MFDF_CMD_FORMAT_PICC;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint8_t     PH_MEMLOC_REM * pCMAC;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];

     /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the format PICC command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_FORMAT_PICC;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
    {
        /* Tell SAM to generate CMAC on the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            0x00,
            &bCmd,
            0x01,
            &pCMAC,
            &wRxlen
            ));
    }

    /* Do a transperent exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bFrame,
        bCmdLen,
        &pDataOut,
        &wRxlen
        ));

    if (pDataParams->bWrappedMode)
    {
        if ((wRxlen < 2) || (pDataOut[wRxlen -2] != 0x91))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        bStatusByte = pDataOut[wRxlen - 1];
        wRxlen -= 1;
    }
    else
    {
        bStatusByte = pDataOut[0];
    }

    if (bStatusByte != PH_ERR_SUCCESS)
    {
        /* 
            In case of failure in authmodes 0x1A and 0xAA,
            SAM should have already reset its authentication 
         */
        return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
    }

    if (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE)
    {
        if (wRxlen != 9)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if (pDataParams->bWrappedMode)
        {
            memcpy(bCMacCard, pDataOut, 8);  /* PRQA S 3200 */
        }        
        else
        {
            memcpy(bCMacCard, &pDataOut[1], 8);  /* PRQA S 3200 */
        }

        /* Verify the CMAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            0x00,
            &bStatusByte,
            0x01
            ));

        /* Lastly send it along with the MAC */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            0x00,
            bCMacCard,
            8
            );
        if (statusTmp != PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFDF);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetVersion(
                                        phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t * pVerInfo
                                        )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN);  /* PRQA S 3200 */

    /* Encode the Get version command*/
    bFrame[bCmdLen++]= PHAL_MFDF_CMD_GET_VERSION;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        /* Map Sam error code to MFDF error codes. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    }

    if (wRxFrameLen != 7)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    /* Copy the partial version info*/
    memcpy(pVerInfo, pRxFrame, wRxFrameLen);  /* PRQA S 3200 */
    pVerInfo += wRxFrameLen;

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */
    bCmdLen = 0;

    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Encode the Additional frame command*/
    bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
         status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRxFrame,
            &wRxFrameLen
            );
    }
    else
    {
        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_RXCHAINING,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRxFrame,
            &wRxFrameLen
            );
    }
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        /* Map Sam error code to MFDF error codes. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    }
    if (wRxFrameLen != 7)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    /* Copy the partial version info*/
    memcpy(pVerInfo, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
    pVerInfo += wRxFrameLen;

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */
    bCmdLen = 0;

    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Encode the Additional frame command*/
    bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
           status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRxFrame,
            &wRxFrameLen
            );
    }
    else
    {

        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_RXCHAINING,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRxFrame,
            &wRxFrameLen
            );
    }
    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    if (wRxFrameLen != 14)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    /* Copy the final part of version info*/
    memcpy(pVerInfo, pRxFrame, wRxFrameLen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_FreeMem(
                                     phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                     uint8_t * pMemInfo
                                     )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the FREE mem command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_FREE_MEM;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    if (wRxFrameLen != 3)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    memcpy(pMemInfo, pRxFrame, wRxFrameLen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_SetConfiguration(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pData,
    uint8_t bDataSize
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    switch(bOption)
    {
    case 0x00:
        if (bDataSize > 1)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        break;

    case 0x01:
        /*  check for data length 24 byte key and 1 byte key version*/
        if (bDataSize > 25)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
        break;

    case 0x02:
        /* This command is not supported in Sam X mode since
        it requires Padding method 2 before encryption */
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_AL_MFDF);
        break;

    default:
        /* Invalid configuration option*/
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if(pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the configuration setting command. */
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_SET_CONFIG;
    bFrame[bCmdLen++] = bOption;
    /* Set communication mode encrypted */
    bCrypto = PHAL_MFDF_ENC_FLAG | bCmdLen ;

    memcpy(bFrame + bCmdLen, pData, bDataSize); /* PRQA S 3200 */
    bCmdLen = bCmdLen + bDataSize;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_GetCardUID(
                                        phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t * pUid
                                        )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0x30; /* Encrypted communication*/

     if(pDataParams->bAuthMode == PHAL_MFDF_NOT_AUTHENTICATED)
     {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_AL_MFDF);
     }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

        /* Length of Expected bytes for card */
    bFrame[bCmdLen++] = 7; 
    bFrame[bCmdLen++] = 0;
    bFrame[bCmdLen++] = 0;


    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = bCmdLen + (PHAL_MFDF_WRAP_HDR_LEN - 1);
    }

    /* Encode get card UID command. */
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_CARD_UID;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame+3, bCmdLen-3);
        bCmdLen +=3;
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    if (wRxFrameLen != 7)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    /* Copy the UID */
    memcpy(pUid, pRxFrame, wRxFrameLen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetFileIDs(
                                        phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t * pFid,
                                        uint8_t * bNumFIDs
                                        )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM  bCrypto = 0;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_FILE_IDS;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }
    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    /* Copy the file id's. */
    memcpy(pFid, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
    *bNumFIDs = (uint8_t)wRxFrameLen;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetISOFileIDs(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pFidBuffer,
    uint8_t * bNumFIDs
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint8_t     PH_MEMLOC_REM bOption = PH_EXCHANGE_DEFAULT;

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_ISO_FILE_IDS;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        bOption,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );
    if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING))
    {
        /* Map Sam error code to MFDF error codes. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    }

    /* Copy the ISO file id's. */
    memcpy(pFidBuffer, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
    pFidBuffer += wRxFrameLen;
    *bNumFIDs = (uint8_t)wRxFrameLen;

    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
    {
        bOption = PH_EXCHANGE_RXCHAINING;

        if (pDataParams->bWrappedMode)
        {
            bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
        }
        /* Initialize the buffer*/
        memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

        bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

        if (pDataParams->bWrappedMode)
        {
            /* Wrap APDU using ISO7816-4 APDU */
            bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
            bOption = PH_EXCHANGE_DEFAULT;
        }

        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            bOption,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRxFrame,
            &wRxFrameLen
            );
        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS ))
        {
            /* Map Sam error code to MFDF error codes. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
        }

        /* Copy the ISO file id's. */
        memcpy(pFidBuffer, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
        (*bNumFIDs) = (uint8_t) ((*bNumFIDs) + wRxFrameLen);

    }/* End of IF status == PH_ERR_SUCCESS_CHAINING. */

    /* Length should be multiple of 2 */
    if (((*bNumFIDs)!= 0) && ((*bNumFIDs) % 2))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    (*bNumFIDs) = (*bNumFIDs) / 2; /* iso file id's are two byte size*/

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetFileSettings(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bFileNo,
    uint8_t * pFSBuffer,
    uint8_t * bBufferLen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for the valid file number */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode get file settings command */
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_GET_FILE_SETTINGS;
    bFrame[bCmdLen++] = bFileNo;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
         bFrame[bCmdLen++] = 0x00 ; /* Add LE byte*/
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
    /* 7 => Data files, 17 => value files, 13 => record files */
    if ((wRxFrameLen != 7) && (wRxFrameLen != 17) && (wRxFrameLen != 13))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    /* Copy the file settings. */
    memcpy(pFSBuffer, pRxFrame, wRxFrameLen); /* PRQA S 3200 */
    *bBufferLen = (uint8_t) wRxFrameLen;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_ChangeFileSettings(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t *pAccessRights
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid file number */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CHANGE_FILE_SETTINGS;
    bFrame[bCmdLen++] = bFileNo;
    if (bOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Encode from which position the encryption should start. */
        bCrypto = PHAL_MFDF_ENC_FLAG |  bCmdLen ;
    }

    bFrame[bCmdLen++] = bCommSett >> 0x04;
    /* Encode Access rights. */
    memcpy(bFrame + bCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 2;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_CreateStdDataFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pISOFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pFileSize
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid file number and option */
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
    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREATE_STD_DATAFILE;
    bFrame[bCmdLen++] = bFileNo;

    if (bOption == 0x01)
    {
        /*Encode ISO file id*/
        memcpy(bFrame + bCmdLen, pISOFileId, 2); /* PRQA S 3200 */
        bCmdLen = bCmdLen + 2;
    }

    /* Encode communication settings. */
    bFrame[bCmdLen++] = bCommSett >> 4;

    /* Encode Access rights. */
    memcpy(bFrame + bCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 2;

    /* Encode file size. */
    memcpy(bFrame + bCmdLen, pFileSize, 3); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 3;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame,bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_CreateBackupDataFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t *pISOFileId,
    uint8_t bCommSett,
    uint8_t *pAccessRights,
    uint8_t * pFileSize
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid file number. */
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

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode create backup data file command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREATE_BKUP_DATAFILE;
    bFrame[bCmdLen++] = bFileNo;

    if (bOption == 0x01)
    {
        /*Encode ISO file id*/
        memcpy(bFrame + bCmdLen, pISOFileId, 2); /* PRQA S 3200 */
        bCmdLen = bCmdLen + 2;
    }

    /* Encode communication settings. */
    bFrame[bCmdLen++] = bCommSett >> 4;

    /* Encode Access rights. */
    memcpy(bFrame + bCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 2;

    /* Encode file size. */
    memcpy(bFrame + bCmdLen, pFileSize, 3); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 3;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_CreateValueFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bFileNo,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pLowerLmit,
    uint8_t * pUpperLmit,
    uint8_t * pValue,
    uint8_t bLimitedCredit
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid file number */
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
    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREATE_VALUE_FILE;
    bFrame[bCmdLen++] = bFileNo;
    bFrame[bCmdLen++] = bCommSett >> 4;
    memcpy(bFrame + bCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    bCmdLen += 2;
    memcpy(bFrame + bCmdLen, pLowerLmit, 4); /* PRQA S 3200 */
    bCmdLen += 4;
    memcpy(bFrame + bCmdLen, pUpperLmit, 4); /* PRQA S 3200 */
    bCmdLen += 4;
    memcpy(bFrame + bCmdLen, pValue, 4); /* PRQA S 3200 */
    bCmdLen += 4;
    bFrame[bCmdLen++] = bLimitedCredit;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_CreateLinearRecordFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for the valid file number. */
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
    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN);  /* PRQA S 3200 */

    /* Encode create linear record file. */
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREATE_LINEAR_RECFILE;
    bFrame[bCmdLen++] = bFileNo;

    if (bOption)
    {
        /* Encode the ISO file id*/
        memcpy(bFrame + bCmdLen, pIsoFileId, 2); /* PRQA S 3200 */
        bCmdLen += 2;
    }

    /* Encode the file communication settings. */
    bFrame[bCmdLen++] = bCommSett >> 4;
    /* Encode the Access rights  */
    memcpy(bFrame + bCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    bCmdLen += 2;
    /* Encode the record size. */
    memcpy(bFrame + bCmdLen, pRecordSize, 3); /* PRQA S 3200 */
    bCmdLen += 3;
    /* Encode the Max number of records. */
    memcpy(bFrame + bCmdLen, pMaxNoOfRec, 3); /* PRQA S 3200 */
    bCmdLen += 3;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_CreateCyclicRecordFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

     /* Check for valid file number. */
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

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the create cyclic record file*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREATE_CYCLIC_RECFILE;
    bFrame[bCmdLen++] = bFileNo;

    if (bOption)
    {
        /* Encode the ISO file id*/
        memcpy(bFrame + bCmdLen, pIsoFileId, 2); /* PRQA S 3200 */
        bCmdLen += 2;
    }

    /* Encode the file communication settings. */
    bFrame[bCmdLen++] = bCommSett >> 4;
    /* Encode the Access rights  */
    memcpy(bFrame + bCmdLen, pAccessRights, 2); /* PRQA S 3200 */
    bCmdLen += 2;
    /* Encode the record size. */
    memcpy(bFrame + bCmdLen, pRecordSize, 3); /* PRQA S 3200 */
    bCmdLen += 3;
    /* Encode the Max number of records. */
    memcpy(bFrame + bCmdLen, pMaxNoOfRec, 3); /* PRQA S 3200 */
    bCmdLen += 3;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_DeleteFile(
                                        phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                        uint8_t bFileNo
                                        )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode delete file command. */
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_DELETE_FILE;
    bFrame[bCmdLen++] = bFileNo;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

/*
* \name Data Manipulation Commands
*/

phStatus_t phalMfdf_SamAV2_X_ReadData(
                                      phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bOption,
                                      uint8_t bFileNo,
                                      uint8_t * pOffset,
                                      uint8_t * pLength,
                                      uint8_t ** ppRxdata,
                                      uint16_t * pRxdataLen
                                      )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRecvBufLen = 0;
    uint16_t    PH_MEMLOC_REM wHalBufSize = 0;
    uint16_t    PH_MEMLOC_REM wOption = PH_EXCHANGE_DEFAULT;
    uint16_t    PH_MEMLOC_REM wNextPos = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecvBuf = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint8_t     PH_MEMLOC_REM bFlag = 1;

     /* Check for valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer. */
    memset(bFrame,0x00,PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    if ((bOption & 0xF0U)== PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication. */
        bCrypto = PHAL_MFDF_MAC_FLAG;
    }
    if ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication. */
        bCrypto = PHAL_MFDF_ENC_FLAG;
        if ((bOption & 0x0FU) != PH_EXCHANGE_RXCHAINING)
        {
            memcpy(bFrame, pLength, 3); /* PRQA S 3200 */
            bCmdLen += 3;
        }
    }

    if ((bOption & 0x0FU) == PH_EXCHANGE_RXCHAINING)
    {
        bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
        wOption = PH_EXCHANGE_RXCHAINING;
    }
    else
    {
        bFrame[bCmdLen++] = PHAL_MFDF_CMD_READ_DATA;
        bFrame[bCmdLen++] = bFileNo;
        memcpy(bFrame + bCmdLen, pOffset, 3); /* PRQA S 3200 */
        bCmdLen += 3;
        memcpy(bFrame + bCmdLen, pLength, 3); /* PRQA S 3200 */
        bCmdLen += 3;
    }

    /* Get the HAL buffer size*/
    PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
        &wHalBufSize));

    do
    {
        if (pDataParams->bWrappedMode)
        {
            /* Wrap APDU using ISO7816-4 APDU */
            if(bCmdLen > 0x0c )
            {
                bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame+3, bCmdLen-3);
                bCmdLen +=3;
            }
            else
            {
                wOption = PH_EXCHANGE_DEFAULT;
                bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
            }
            if( bCmdLen > 0x0a)
            {
                bFrame[bCmdLen++]=0x00; /*Add AF if it is not channing mode*/
            }
            
        }

        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            wOption,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRecvBuf,
            &wRecvBufLen
            );

        status = phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);

        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
        {
            return status;
        }

        *pRxdataLen = *pRxdataLen + wRecvBufLen;
        wNextPos = wNextPos + wRecvBufLen;
        if (bFlag)
        {
            *ppRxdata = pRecvBuf;
            bFlag = 0;
        }

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            wOption = PH_EXCHANGE_RXCHAINING;
            bCmdLen = 0;
            if (pDataParams->bWrappedMode)
            {
                wOption = PH_EXCHANGE_DEFAULT;
                bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
            }
            bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

            PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SetConfig(pDataParams->pHalSamDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wNextPos));

            if ((wNextPos + 60) > wHalBufSize)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SetConfig(pDataParams->pHalSamDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                0x00));
                return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
            }
        }
    } while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING);

    /* Reset the start position */
    return phhalHw_SetConfig(pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        0);
}

phStatus_t phalMfdf_SamAV2_X_WriteData(
                                       phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                       uint8_t bOption,
                                       uint8_t bFileNo,
                                       uint8_t * pOffset,
                                       uint8_t * pTxData,
                                       uint8_t * pTxDataLen
                                       )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint8_t     PH_MEMLOC_REM bMaxApduDataLen = 52;
    uint16_t    PH_MEMLOC_REM wOption = PH_EXCHANGE_BUFFER_FIRST;
    uint32_t    PH_MEMLOC_REM dwTxLen = 0;
    uint8_t     PH_MEMLOC_REM bDLen = 0;
    uint8_t     PH_MEMLOC_REM *pData = NULL; 
    uint8_t     PH_MEMLOC_REM bFrameLen[3]={0x00,0x00,0x00};
    uint32_t    PH_MEMLOC_REM dwOffset = 0;

    /* Check for valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    if ((bOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    pData = pTxData;
    dwTxLen = (uint32_t)( ((uint32_t)pTxDataLen[2]  & 0x0000ff) << 16 ) | \
        ( ((uint32_t)pTxDataLen[1]  & 0x0000ff) << 8 ) | ((uint32_t)pTxDataLen[0]);

    dwOffset = (uint32_t)( ((uint32_t)pOffset[2]  & 0x0000ff) << 16 ) | \
        ( ((uint32_t)pOffset[1]  & 0x0000ff) << 8 ) | ((uint32_t)pOffset[0]);


    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
        bMaxApduDataLen = bMaxApduDataLen - PHAL_MFDF_WRAP_HDR_LEN;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_WRITE_DATA;
    bFrame[bCmdLen++] = bFileNo;
    memcpy(bFrame + bCmdLen, pOffset, 3); /* PRQA S 3200 */
    bCmdLen += 3;

    if (bOption == PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication and 
           Encode from which position the crypto operation should start.*/
        bCrypto = PHAL_MFDF_MAC_FLAG | 0x08 ;
        if(pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE )
        {
            bMaxApduDataLen = 48;
        }
        else
        {
            bMaxApduDataLen = 40;
        }
        if (pDataParams->bWrappedMode)
        {
            bCrypto = PHAL_MFDF_MAC_FLAG | 0x0c ;
            bMaxApduDataLen = bMaxApduDataLen - PHAL_MFDF_WRAP_HDR_LEN;
        }
    }

    if (bOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication and 
           Encode from which position the crypto operation should start. */
        bCrypto = PHAL_MFDF_ENC_FLAG | 0x08 ;
        if(pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES )
        {
            bMaxApduDataLen = 32;
        }
        else
        {
            bMaxApduDataLen = 40;
        }
        if (pDataParams->bWrappedMode)
        {
            bCrypto = PHAL_MFDF_ENC_FLAG | 0x0C ;
            bMaxApduDataLen = bMaxApduDataLen - PHAL_MFDF_WRAP_HDR_LEN;
        }
        
    }
    do
    {
        if (dwTxLen > bMaxApduDataLen)
        {
            bDLen = bMaxApduDataLen;
            dwTxLen -= bMaxApduDataLen;
            wOption = PH_EXCHANGE_BUFFER_FIRST;
            bFrameLen[0] = bDLen;
            memcpy(bFrame + bCmdLen, bFrameLen, 3); /* PRQA S 3200 */
            bCmdLen += 3;
            dwOffset += bDLen;
        }
        else
        {
            bDLen = (uint8_t)dwTxLen;
            dwTxLen = 0;
            bFrameLen[0] = bDLen;
            memcpy(bFrame + bCmdLen, bFrameLen, 3); /* PRQA S 3200 */
            bCmdLen += 3;
            wOption = PH_EXCHANGE_BUFFER_FIRST;
            dwOffset += bDLen;
        }
        if (pDataParams->bWrappedMode)
        {
            /* Wrap APDU using ISO7816-4 APDU */
            bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen + bDLen);
            bCmdLen = (uint8_t)(bCmdLen - bDLen );
        }
        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SamAV2_Cmd_DESFireWriteX(
            pDataParams->pHalSamDataParams,
            wOption,
            bCrypto,
            bFrame,
            bCmdLen
            ));
         status = phhalHw_SamAV2_Cmd_DESFireWriteX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            bCrypto,
            pData,
            bDLen
            );

        pData += bDLen;

        status = phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);

        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            return status;
        }
        if (pDataParams->bWrappedMode)
        {
               bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
        }
        else
        {
            bCmdLen = 0x00;
        }
        bFrame[bCmdLen++] = PHAL_MFDF_CMD_WRITE_DATA;
        bFrame[bCmdLen++] = bFileNo;
        bFrame[bCmdLen++] = (uint8_t) (dwOffset & 0x000000ff );
        bFrame[bCmdLen++] = (uint8_t) ((dwOffset & 0x0000ff00 ) >> 8 );
        bFrame[bCmdLen++] = (uint8_t) ((dwOffset & 0x00ff0000 ) >> 16 );
    } while (dwTxLen>0);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetValue(
                                      phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                      uint8_t bCommOption,
                                      uint8_t bFileNo,
                                      uint8_t * pValue
                                      )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRxFrameLen = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM *pRxFrame = NULL;
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* check for the valid file number */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    if (bCommOption == PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication. */
        bCrypto = PHAL_MFDF_MAC_FLAG;
    }

    if (bCommOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication. */
        bCrypto = PHAL_MFDF_ENC_FLAG;
        bFrame[0x00] = 4; /*Expected bytes to be read*/
        bCmdLen += 3; /* First three bytes denotes expected bytes to be read*/
    }

    bFrame[bCmdLen++]= PHAL_MFDF_CMD_GET_VALUE;
    bFrame[bCmdLen++]= bFileNo;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        if(bCrypto == PHAL_MFDF_ENC_FLAG)
        {
            bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame+3, (uint8_t)bCmdLen-3);
            bCmdLen +=3;
        }
        else
        {
            bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
        }
        bFrame[bCmdLen++]=0x00;/*Add LE byte*/
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        &pRxFrame,
        &wRxFrameLen
        );

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    if (wRxFrameLen != 4)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_AL_MFDF);
    }
    /* Copy the value. */
    memcpy(pValue, pRxFrame, wRxFrameLen); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_Credit(
                                    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                    uint8_t bCommOption,
                                    uint8_t bFileNo,
                                    uint8_t * pValue
                                    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /*Check for the valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the credit command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CREDIT;
    bFrame[bCmdLen++] = bFileNo;

    /* set Maced communication option */
    if (bCommOption == PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication. */
        bCrypto = PHAL_MFDF_MAC_FLAG |  bCmdLen ;
    }

    /* set Encrypted communication option */
    if (bCommOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication. */
        bCrypto = PHAL_MFDF_ENC_FLAG |  bCmdLen ;
    }

    memcpy(bFrame + bCmdLen, pValue, 4); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 4;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_Debit(
                                   phalMfdf_SamAV2_X_DataParams_t * pDataParams,
                                   uint8_t bCommOption,
                                   uint8_t bFileNo,
                                   uint8_t * pValue
                                   )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* check for valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1 ;
    }

    /* Initialize the buffer*/
    memset(bFrame,0x00,PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_DEBIT;
    bFrame[bCmdLen++] = bFileNo;

    /* set Maced communication option */
    if (bCommOption == PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication. */
        bCrypto = PHAL_MFDF_MAC_FLAG | bCmdLen ;
    }

    /* set Encrypted communication option */
    if (bCommOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication. */
        bCrypto = PHAL_MFDF_ENC_FLAG | bCmdLen ;
    }

    memcpy(bFrame + bCmdLen, pValue, 4); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 4;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame,bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_LimitedCredit(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bCommOption,
    uint8_t bFileNo,
    uint8_t * pValue
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for the valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_LIMITED_CREDIT;
    bFrame[bCmdLen++] = bFileNo;

    /* set Maced communication option */
    if (bCommOption == PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication. */
        bCrypto = PHAL_MFDF_MAC_FLAG | bCmdLen ;
    }

    /* set Encrypted communication option */
    if (bCommOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication. */
        bCrypto = PHAL_MFDF_ENC_FLAG | bCmdLen ;
    }

    memcpy(bFrame + bCmdLen, pValue, 4); /* PRQA S 3200 */
    bCmdLen = bCmdLen + 4;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_WriteRecord(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pOffset,
    uint8_t * pTxData,
    uint8_t * pTxDataLen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint8_t     PH_MEMLOC_REM bMaxApduDataLen = 52;
    uint16_t    PH_MEMLOC_REM wOption = PH_EXCHANGE_BUFFER_FIRST;
    uint32_t    PH_MEMLOC_REM dwTxLen = 0;
    uint8_t     PH_MEMLOC_REM bDLen = 0;
    uint8_t     PH_MEMLOC_REM *pData = NULL; 
    uint8_t     PH_MEMLOC_REM bFrameLen[3]={0x00,0x00,0x00};
    uint32_t    PH_MEMLOC_REM dwOffset = 0;

    /* Check for valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    if ((bOption != PHAL_MFDF_COMMUNICATION_PLAIN) &&
        (bOption != PHAL_MFDF_COMMUNICATION_ENC) &&
        (bOption != PHAL_MFDF_COMMUNICATION_MACD))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    pData = pTxData;
    dwTxLen = (uint32_t)( ((uint32_t)pTxDataLen[2]  & 0x0000ff) << 16 ) | \
        ( ((uint32_t)pTxDataLen[1]  & 0x0000ff) << 8 ) | ((uint32_t)pTxDataLen[0]);

    dwOffset = (uint32_t)( ((uint32_t)pOffset[2]  & 0x0000ff) << 16 ) | \
        ( ((uint32_t)pOffset[1]  & 0x0000ff) << 8 ) | ((uint32_t)pOffset[0]);


    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
        bMaxApduDataLen = bMaxApduDataLen - PHAL_MFDF_WRAP_HDR_LEN;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_WRITE_RECORD;
    bFrame[bCmdLen++] = bFileNo;
    memcpy(bFrame + bCmdLen, pOffset, 3); /* PRQA S 3200 */
    bCmdLen += 3;

    if (bOption == PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication and 
           Encode from which position the crypto operation should start.*/
        bCrypto = PHAL_MFDF_MAC_FLAG | 0x08 ;

         if(pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATE )
        {
            bMaxApduDataLen = 48;
        }
        else
        {
            bMaxApduDataLen = 40;
        }
         if (pDataParams->bWrappedMode)
        {
            bCrypto = PHAL_MFDF_MAC_FLAG | 0x0c ;
            bMaxApduDataLen = bMaxApduDataLen - PHAL_MFDF_WRAP_HDR_LEN;
        }
    }

    if (bOption == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication and 
           Encode from which position the crypto operation should start. */
        bCrypto = PHAL_MFDF_ENC_FLAG | 0x08 ;
        if(pDataParams->bAuthMode == PHAL_MFDF_AUTHENTICATEAES )
        {
            bMaxApduDataLen = 32;
        }
        else
        {
            bMaxApduDataLen = 40;
        }
         if (pDataParams->bWrappedMode)
        {
            bCrypto = PHAL_MFDF_ENC_FLAG | 0x0C ;
            bMaxApduDataLen = bMaxApduDataLen - PHAL_MFDF_WRAP_HDR_LEN;
        }
    }

    do
    {
        if (dwTxLen > bMaxApduDataLen)
        {
            bDLen = bMaxApduDataLen;
            dwTxLen -= bMaxApduDataLen;
            wOption = PH_EXCHANGE_BUFFER_FIRST;
            bFrameLen[0] = bDLen;
            memcpy(bFrame + bCmdLen, bFrameLen, 3); /* PRQA S 3200 */
            bCmdLen += 3;
            dwOffset += bDLen;
        }
        else
        {
            bDLen = (uint8_t)dwTxLen;
            dwTxLen = 0;
            bFrameLen[0] = bDLen;
            memcpy(bFrame + bCmdLen, bFrameLen, 3); /* PRQA S 3200 */
            bCmdLen += 3;
            wOption = PH_EXCHANGE_BUFFER_FIRST;
            dwOffset += bDLen;
        }
        if (pDataParams->bWrappedMode)
        {
            /* Wrap APDU using ISO7816-4 APDU */
            bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen + bDLen);
            bCmdLen = (uint8_t)(bCmdLen - bDLen );
        }
        PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SamAV2_Cmd_DESFireWriteX(
            pDataParams->pHalSamDataParams,
            wOption,
            bCrypto,
            bFrame,
            bCmdLen
            ));
         status = phhalHw_SamAV2_Cmd_DESFireWriteX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            bCrypto,
            pData,
            bDLen
            );

        pData += bDLen;

        status = phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);

        if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
        {
            return status;
        }
         if (pDataParams->bWrappedMode)
        {
               bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
        }
        else
        {
            bCmdLen = 0x00;
        }
        bFrame[bCmdLen++] = PHAL_MFDF_CMD_WRITE_RECORD;
        bFrame[bCmdLen++] = bFileNo;
        bFrame[bCmdLen++] = (uint8_t) (dwOffset & 0x000000ff );
        bFrame[bCmdLen++] = (uint8_t) ((dwOffset & 0x0000ff00 ) >> 8 );
        bFrame[bCmdLen++] = (uint8_t) ((dwOffset & 0x00ff0000 ) >> 16 );
    } while (dwTxLen>0);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}


phStatus_t phalMfdf_SamAV2_X_ReadRecords(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t * pOffset,
    uint8_t * pNumRec,
    uint8_t * pRecSize,
    uint8_t ** ppRxdata,
    uint16_t * pRxdataLen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint16_t    PH_MEMLOC_REM wRecvBufLen = 0;
    uint16_t    PH_MEMLOC_REM wHalBufSize = 0;
    uint16_t    PH_MEMLOC_REM wOption = PH_EXCHANGE_DEFAULT;
    uint16_t    PH_MEMLOC_REM wNextPos = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecvBuf = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint8_t     PH_MEMLOC_REM bFlag = 1;

     /* Check for valid file number. */
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

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer. */
    memset(bFrame,0x00,PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    if ((bOption & 0xF0U)== PHAL_MFDF_COMMUNICATION_MACD)
    {
        /* Set bit 4 and 5 to '01' for maced communication. */
        bCrypto = PHAL_MFDF_MAC_FLAG;
    }
    if ((bOption & 0xF0U) == PHAL_MFDF_COMMUNICATION_ENC)
    {
        /* Set bit 4 and 5 to '11' for encrypted communication. */
        bCrypto = PHAL_MFDF_ENC_FLAG;
        if ((bOption & 0x0FU) != PH_EXCHANGE_RXCHAINING)
        {
            memcpy(bFrame, pRecSize, 3); /* PRQA S 3200 */
            bCmdLen += 3;
        }
    }

    if ((bOption & 0x0FU) == PH_EXCHANGE_RXCHAINING)
    {
        bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
        wOption = PH_EXCHANGE_RXCHAINING;
    }
    else
    {
        bFrame[bCmdLen++] = PHAL_MFDF_CMD_READ_RECORDS;
        bFrame[bCmdLen++] = bFileNo;
        memcpy(bFrame + bCmdLen, pOffset, 3); /* PRQA S 3200 */
        bCmdLen += 3;
        memcpy(bFrame + bCmdLen, pNumRec, 3); /* PRQA S 3200 */
        bCmdLen += 3;
    }

    /* Get the HAL buffer size*/
    PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
        &wHalBufSize));

    do
    {
        if (pDataParams->bWrappedMode)
        {
            /* Wrap APDU using ISO7816-4 APDU */
              if(bCmdLen > 0x0c )
            {
                bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame+3, bCmdLen-3);
                bCmdLen +=3;
            }
            else
            {
                wOption = PH_EXCHANGE_DEFAULT;
                bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
            }
            if( bCmdLen > 0x0a)
            {
                bFrame[bCmdLen++]=0x00; /*Add AF if it is not channing mode*/
            }
        }

        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            wOption,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRecvBuf,
            &wRecvBufLen
            );

        status = phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);

        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
        {
            return status;
        }

        *pRxdataLen = *pRxdataLen + wRecvBufLen;
        wNextPos = wNextPos + wRecvBufLen;
        if (bFlag)
        {
            *ppRxdata = pRecvBuf;
            bFlag = 0;
        }

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            wOption = PH_EXCHANGE_RXCHAINING;
            bCmdLen = 0;
            if (pDataParams->bWrappedMode)
            {
                wOption = PH_EXCHANGE_DEFAULT;
                bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
            }
            bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

            PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SetConfig(pDataParams->pHalSamDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wNextPos));

            if ((wNextPos + 60) > wHalBufSize)
            {
                PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SetConfig(pDataParams->pHalSamDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                0x00));
                return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
            }
        }
    } while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING);

    /* Reset the start position */
    return phhalHw_SetConfig(pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
        0);
}


phStatus_t phalMfdf_SamAV2_X_ClearRecordFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bFileNo
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Check for the valid file number. */
    if (bFileNo > 0x1f)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = PHAL_MFDF_CMD_CLEAR_RECORDS_FILE;
    bFrame[bCmdLen++] = bFileNo;
    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_CommitTransaction(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM * pDataOut;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmd = PHAL_MFDF_CMD_COMMIT_TXN;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint8_t     PH_MEMLOC_REM * pCMAC;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];

     /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the format PICC command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_COMMIT_TXN;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

    if ( (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE) &&
           (pDataParams->bAuthMode != PHAL_MFDF_NOT_AUTHENTICATED) )
    {
        /* Tell SAM to generate CMAC on the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            0x00,
            &bCmd,
            0x01,
            &pCMAC,
            &wRxlen
            ));
    }

    /* Do a transperent exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bFrame,
        bCmdLen,
        &pDataOut,
        &wRxlen
        ));

    if (pDataParams->bWrappedMode)
    {
        if ((wRxlen < 2) || (pDataOut[wRxlen -2] != 0x91))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        bStatusByte = pDataOut[wRxlen - 1];
        wRxlen -= 1;
    }
    else
    {
        bStatusByte = pDataOut[0];
    }

    if (bStatusByte != PH_ERR_SUCCESS)
    {
        /* In case of failure in authmodes 0x1A and 0xAA,
        SAM should have already reset its authentication 
        */
        return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
    }

     if ( (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE) &&
           (pDataParams->bAuthMode != PHAL_MFDF_NOT_AUTHENTICATED) )
     {
        if (wRxlen != 0x09)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if (pDataParams->bWrappedMode)
        {
            memcpy(bCMacCard, pDataOut, 8);  /* PRQA S 3200 */
        }        
        else
        {
            memcpy(bCMacCard, &pDataOut[1], 8);  /* PRQA S 3200 */
        }

        /* Verify the CMAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            0x00,
            &bStatusByte,
            0x01
            ));

        /* Lastly send it along with the MAC */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            0x00,
            bCMacCard,
            8
            );
        if (statusTmp != PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFDF);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_AbortTransaction(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM * pDataOut;
    uint16_t    PH_MEMLOC_REM wRxlen;
    uint8_t     PH_MEMLOC_REM bCmd = PHAL_MFDF_CMD_ABORT_TXN;
    uint8_t     PH_MEMLOC_REM bStatusByte;
    uint8_t     PH_MEMLOC_REM * pCMAC;
    uint8_t     PH_MEMLOC_REM bCMacCard[8];

     /* Init wrapped header length for wrapped mode. */
    if (pDataParams->bWrappedMode)
    {
        bCmdLen = PHAL_MFDF_WRAP_HDR_LEN - 1;
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    /* Encode the format PICC command*/
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_ABORT_TXN;

    if (pDataParams->bWrappedMode)
    {
        /* Wrap APDU using ISO7816-4 APDU */
        bCmdLen = phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(bFrame, bCmdLen);
    }

      if ( (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE) &&
           (pDataParams->bAuthMode != PHAL_MFDF_NOT_AUTHENTICATED) )
    {
        /* Tell SAM to generate CMAC on the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_DEFAULT,
            0x00,
            &bCmd,
            0x01,
            &pCMAC,
            &wRxlen
            ));
    }

    /* Do a transperent exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bFrame,
        bCmdLen,
        &pDataOut,
        &wRxlen
        ));

    if (pDataParams->bWrappedMode)
    {
        if ((wRxlen < 2) || (pDataOut[wRxlen -2] != 0x91))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }
        bStatusByte = pDataOut[wRxlen - 1];
        wRxlen -= 1;
    }
    else
    {
        bStatusByte = pDataOut[0];
    }

    if (bStatusByte != PH_ERR_SUCCESS)
    {
        /* In case of failure in authmodes 0x1A and 0xAA,
        SAM should have already reset its authentication 
        */
        return phalMfdf_Int_ComputeErrorResponse(pDataParams, bStatusByte);
    }

    if ( (pDataParams->bAuthMode != PHAL_MFDF_AUTHENTICATE) &&
       (pDataParams->bAuthMode != PHAL_MFDF_NOT_AUTHENTICATED) )
    {
        if (wRxlen != 0x09)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFDF);
        }

        if (pDataParams->bWrappedMode)
        {
            memcpy(bCMacCard, pDataOut, 8);  /* PRQA S 3200 */
        }        
        else
        {
            memcpy(bCMacCard, &pDataOut[1], 8);  /* PRQA S 3200 */
        }

        /* Verify the CMAC */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            0x00,
            &bStatusByte,
            0x01
            ));

        /* Lastly send it along with the MAC */
        statusTmp = phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            0x00,
            bCMacCard,
            8
            );
        if (statusTmp != PH_ERR_SUCCESS)
        {
            return PH_ADD_COMPCODE(PH_ERR_INTEGRITY_ERROR, PH_COMP_AL_MFDF);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_IsoSelectFile(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bSelector,
    uint8_t * pFid,
    uint8_t * pDFname,
    uint8_t bDFnameLen,
    uint8_t ** ppFCI,
    uint16_t * pwFCILen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bLc = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    
    if( (bDFnameLen > 16) || ((bOption!=0x00)&&(bOption!=0x0c)))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* Initialize the buffer*/
    memset(bFrame,0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = 0x00;
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_ISO7816_SELECT_FILE;
    bFrame[bCmdLen++] = bSelector;
    bFrame[bCmdLen++] = bOption;

    switch(bSelector)
    {
    case 0x00:
    case 0x02:
        /* Selection by EF Id*/
        bLc = 2;
        bFrame[bCmdLen++] = bLc;
        /* Reverse before send */
        bFrame[bCmdLen++] = pFid[1];
        bFrame[bCmdLen++] = pFid[0];

        break;

    case 0x04:
        /* Selection by DF Name */
        bFrame[bCmdLen++] = bDFnameLen;
        memcpy(bFrame + bCmdLen, pDFname, bDFnameLen); /* PRQA S 3200 */
        bCmdLen = (uint8_t) bCmdLen + bDFnameLen;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        break;
    }

    status = phhalHw_SamAV2_Cmd_DESFireReadX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_DEFAULT,
        bCrypto,
        bFrame,
        bCmdLen,
        ppFCI,
        pwFCILen
        );

    /* ISO wrapped mode is on */
    pDataParams->bWrappedMode = 1;

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_IsoReadBinary(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bOffset,
    uint8_t bSfid,
    uint8_t bBytesToRead,
    uint8_t ** pRxBuffer,
    uint16_t * pBytesRead
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint16_t    PH_MEMLOC_REM wRecvBufLen = 0;
    uint16_t    PH_MEMLOC_REM wHalBufSize = 0;
    uint16_t    PH_MEMLOC_REM wNextPos = 0;
    uint8_t     PH_MEMLOC_REM *pRecvBuf = 0;
    uint8_t     PH_MEMLOC_REM bFlag = 1;

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */
    
    if (bSfid & 0x80)
    {
        /* Short file id is supplied */
        if ((bSfid & 0x7FU) > 0x1F)
        {
            /* Error condition */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }
    if (wOption == PH_EXCHANGE_DEFAULT)
     {
         bFrame[bCmdLen++] = 0x00;
         bFrame[bCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_BINARY;
         bFrame[bCmdLen++] = bSfid;
         bFrame[bCmdLen++] = bOffset;
         bFrame[bCmdLen++] = bBytesToRead;
    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
        wOption = PH_EXCHANGE_RXCHAINING;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

      /* Get the HAL buffer size*/
    PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_GetConfig(
        pDataParams->pHalSamDataParams,
        PHHAL_HW_CONFIG_RXBUFFER_BUFSIZE,
        &wHalBufSize));

    do
    {
        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            wOption,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRecvBuf,
            &wRecvBufLen
            );
        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
        }
        *pBytesRead = *pBytesRead + wRecvBufLen;
        wNextPos = wNextPos + wRecvBufLen;
        if (bFlag)
        {
            *pRxBuffer = pRecvBuf;
            bFlag = 0;
        }

        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            wOption = PH_EXCHANGE_RXCHAINING;
            bCmdLen = 0;
            bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;

            PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SetConfig(pDataParams->pHalSamDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                wNextPos));

            if ((wNextPos + 60) > wHalBufSize)
            {
                  PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SetConfig(pDataParams->pHalSamDataParams,
                PHHAL_HW_CONFIG_RXBUFFER_STARTPOS,
                0x00));
                return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
            }
        }
    }while ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING);
    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_IsoUpdateBinary(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bOffset,
    uint8_t bSfid,
    uint8_t * pData,
    uint8_t bDataLen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = 0x00;
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_ISO7816_UPDATE_BINARY;
    if (bSfid & 0x80)
    {
        /* Short file id is supplied */
        if ((bSfid & 0x7FU) > 0x1F)
        {
            /* Error condition */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
        }
    }

    bFrame[bCmdLen++] = bSfid;
    bFrame[bCmdLen++] = bOffset;
    bFrame[bCmdLen++] = bDataLen;

    PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SamAV2_Cmd_DESFireWriteX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            bCrypto,
            bFrame,
            bCmdLen
            ));
    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCrypto,
        pData,
        bDataLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_IsoReadRecords(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bRecNo,
    uint8_t bReadAllFromP1,
    uint8_t bSfid,
    uint8_t bBytesToRead,
    uint8_t ** pRxBuffer,
    uint16_t * bBytesRead
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;
    uint16_t    PH_MEMLOC_REM wRecvBufLen = 0;
    uint8_t     PH_MEMLOC_REM *pRecvBuf = 0;

    if (bSfid > 0x1F)
    {
        /* Invalid Short File Id */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    if (wOption == PH_EXCHANGE_DEFAULT)
     {
         bFrame[bCmdLen++] = 0x00;
         bFrame[bCmdLen++] = PHAL_MFDF_CMD_ISO7816_READ_RECORDS;
         bFrame[bCmdLen++] = bRecNo;
         if (bReadAllFromP1)
         {
            bFrame[bCmdLen++] = (uint8_t)((0xf0 & (bSfid << 3) )|(0x0f & 0x05));
         }
         else
         {
            bFrame[bCmdLen++] = (uint8_t)((0xf0 & (bSfid << 3) )|(0x0f & 0x04));
         }
         bFrame[bCmdLen++] = bBytesToRead;
    }
    else if (wOption == PH_EXCHANGE_RXCHAINING)
    {
        wOption = PH_EXCHANGE_RXCHAINING;
         bFrame[bCmdLen++] = PHAL_MFDF_RESP_ADDITIONAL_FRAME;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

        status = phhalHw_SamAV2_Cmd_DESFireReadX(
            pDataParams->pHalSamDataParams,
            wOption,
            bCrypto,
            bFrame,
            bCmdLen,
            &pRecvBuf,
            &wRecvBufLen
            );
        *pRxBuffer = pRecvBuf;
        *bBytesRead = wRecvBufLen;
        if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));
        }
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_IsoAppendRecord(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bSfid,
    uint8_t * pData,
    uint8_t bDataLen
    )
{
    phStatus_t  PH_MEMLOC_REM status = 0;
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    uint8_t     PH_MEMLOC_REM bFrame[PHAL_MFDF_APDU_BUFF_LEN];
    uint8_t     PH_MEMLOC_REM bCmdLen = 0;
    uint8_t     PH_MEMLOC_REM bCrypto = 0;

    if (bSfid > 0x1F)
    {
        /* Invalid Short File Id */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }
    /* Initialize the buffer*/
    memset(bFrame, 0x00, PHAL_MFDF_APDU_BUFF_LEN); /* PRQA S 3200 */

    bFrame[bCmdLen++] = 0x00;
    bFrame[bCmdLen++] = PHAL_MFDF_CMD_ISO7816_APPEND_RECORD;
    bFrame[bCmdLen++] = 0x00;
    bFrame[bCmdLen++] = (uint8_t)((0xf1 & (bSfid << 3) )| 0x00);
    bFrame[bCmdLen++] = bDataLen;

    PH_CHECK_SUCCESS_FCT(statusTmp,phhalHw_SamAV2_Cmd_DESFireWriteX(
            pDataParams->pHalSamDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            bCrypto,
            bFrame,
            bCmdLen
            ));

    status = phhalHw_SamAV2_Cmd_DESFireWriteX(
        pDataParams->pHalSamDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        bCrypto,
        pData,
        bDataLen
        );

    /* Map Sam error code to MFDF error codes. */
    return phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status);
}

phStatus_t phalMfdf_SamAV2_X_IsoGetChallenge(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bLe,
    uint8_t * pRPICC1
    )
{
    pRPICC1 = NULL;
    PHAL_MFDF_UNUSED_VARIABLE(pDataParams);
    PHAL_MFDF_UNUSED_VARIABLE(wKeyNo);
    PHAL_MFDF_UNUSED_VARIABLE(wKeyVer);
    PHAL_MFDF_UNUSED_VARIABLE(bLe);
    PHAL_MFDF_UNUSED_VARIABLE(pRPICC1);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_IsoExternalAuthenticate(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pDataIn,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    PHAL_MFDF_UNUSED_VARIABLE(pDataParams);
    PHAL_MFDF_UNUSED_VARIABLE(pDataIn);
    PHAL_MFDF_UNUSED_VARIABLE(bInputLen);
    PHAL_MFDF_UNUSED_VARIABLE(pDataOut);
    PHAL_MFDF_UNUSED_VARIABLE(pOutLen);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_IsoInternalAuthenticate(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pDataIn,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    PHAL_MFDF_UNUSED_VARIABLE(pDataParams);
    PHAL_MFDF_UNUSED_VARIABLE(pDataIn);
    PHAL_MFDF_UNUSED_VARIABLE(bInputLen);
    PHAL_MFDF_UNUSED_VARIABLE(pDataOut);
    PHAL_MFDF_UNUSED_VARIABLE(pOutLen);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_IsoAuthenticate(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wKeyVer,
    uint8_t bKeyNoCard,
    uint8_t isPICCkey
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp = 0;
    phStatus_t  PH_MEMLOC_REM status = 0;
    uint8_t     PH_MEMLOC_REM bAuthMode = 0;
    uint8_t     PH_MEMLOC_REM bISOMode = 0;

    /* Check for valid card key number. */
    if (bKeyNoCard > 0x0d)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    /* Check for valid SAM keystore number and version. */
    if ((wKeyNo > 0x7f) || (wKeyVer > 0xff))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFDF);
    }

    PHAL_MFDF_UNUSED_VARIABLE(isPICCkey);

    /* Unset the current authentication status */
    pDataParams->bAuthMode = PHAL_MFDF_NOT_AUTHENTICATED;
    pDataParams->bKeyNo = 0xFF;

     /* Key selection always by entry number, set bit one to zero. */
    bAuthMode = (uint8_t)(bAuthMode & 0xfd);

    /* Set ISO Mode */
    bISOMode = 0x80;

    status = phhalHw_SamAV2_Cmd_DESFireAuthenticatePICC(
        pDataParams->pHalSamDataParams,
        bAuthMode,
        bISOMode,
        bKeyNoCard,
        (uint8_t)wKeyNo,
        (uint8_t)wKeyVer,
        NULL,
        0);

    /* Map Sam error code to MFDF error codes. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfdf_SamAV2_X_Int_HalToMfdf(pDataParams, status));

    /* To ensure that getDFNames does not get called in 0x1a or 0xAA modes */
    pDataParams->bAuthMode = PHAL_MFDF_AUTHENTICATEISO;

    pDataParams->bKeyNo = bKeyNoCard;
    pDataParams->bWrappedMode = 1;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFDF);
}

phStatus_t phalMfdf_SamAV2_X_GetConfig(
                                     phalMfdf_SamAV2_X_DataParams_t * pDataParams,
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

phStatus_t phalMfdf_SamAV2_X_SetConfig(
                                       phalMfdf_SamAV2_X_DataParams_t * pDataParams,
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

#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */
