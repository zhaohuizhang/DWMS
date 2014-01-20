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
* Software ISO14443-3A Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.7 $
* $Date: Fri Apr 15 09:26:40 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalI14443p3a.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHPAL_I14443P3A_SAMAV2_X

#include "phpalI14443p3a_SamAV2_X.h"
#include <phhalHw_SamAV2_Cmd.h>

/** ReqA Command code */
#define I14443P3A_SAMAV2_REQUEST_CMD       0x26

/** WupA Command code */
#define I14443P3A_SAMAV2_WAKEUP_CMD        0x52

phStatus_t phpalI14443p3a_SamAV2_X_Init(
                                        phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
                                        uint16_t wSizeOfDataParams,
                                        phhalHw_SamAV2_DataParams_t * pHalDataParams
                                        )
{
    if (sizeof(phpalI14443p3a_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_ISO14443P3A);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId            = PH_COMP_PAL_ISO14443P3A | PHPAL_I14443P3A_SAMAV2_X_ID;
    pDataParams->pHalDataParams = pHalDataParams;
    pDataParams->bUidLength     = 0;
    pDataParams->bUidComplete   = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_SamAV2_X_RequestA(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pAtqa
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* Set RxDeafBits */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXDEAFBITS,
        8));

    /* Perform RequestA_Wakeup command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_3_RequestA_Wakeup(
        pDataParams->pHalDataParams,
        I14443P3A_SAMAV2_REQUEST_CMD,
        pAtqa));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_SamAV2_X_WakeUpA(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pAtqa
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* Set RxDeafBits */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
        pDataParams->pHalDataParams,
        PHHAL_HW_CONFIG_RXDEAFBITS,
        9));

    /* Perform RequestA_Wakeup command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_3_RequestA_Wakeup(
        pDataParams->pHalDataParams,
        I14443P3A_SAMAV2_WAKEUP_CMD,
        pAtqa));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_SamAV2_X_HaltA(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    /* Preform HaltA command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_3_HaltA(pDataParams->pHalDataParams));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P3A);

}

phStatus_t phpalI14443p3a_SamAV2_X_Anticollision(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bCascadeLevel,
    uint8_t * pUidIn,
    uint8_t bNvbUidIn,
    uint8_t * pUidOut,
    uint8_t * pNvbUidOut
    )
{
    /* satisfy compiler */
    if (pDataParams || bCascadeLevel || pUidIn || bNvbUidIn || pUidOut || pNvbUidOut);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_SamAV2_X_Select(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bCascadeLevel,
    uint8_t * pUidIn,
    uint8_t * pSak
    )
{
    /* satisfy compiler */
    if (pDataParams || bCascadeLevel || pUidIn || pSak);

    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_SamAV2_X_ActivateCard(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pUidIn,
    uint8_t bLenUidIn,
    uint8_t * pUidOut,
    uint8_t * pLenUidOut,
    uint8_t * pSak,
    uint8_t * pMoreCardsAvailable
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* Parameter check */
    if ((bLenUidIn != 0) &&
        (bLenUidIn != 4) &&
        (bLenUidIn != 7) &&
        (bLenUidIn != 10))
    {
        /* Given UID length is invalid, return error */
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_ISO14443P3A);
    }

    /* Reset UID complete flag */
    pDataParams->bUidComplete = 0;

    /* No possibility to check that */
    *pMoreCardsAvailable = 0;

    if (bLenUidIn != 0)
    {
        /* Call activate Wakeup command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_3_ActivateWakeUp(
            pDataParams->pHalDataParams,
            pUidIn,
            bLenUidIn));

        /* Store UID */
        pDataParams->bUidLength = bLenUidIn;
        memcpy(pDataParams->abUid, pUidIn, pDataParams->bUidLength);  /* PRQA S 3200 */

        /* Return UID */
        *pLenUidOut = pDataParams->bUidLength;
        memcpy(pUidOut, pDataParams->abUid, pDataParams->bUidLength);  /* PRQA S 3200 */
    }
    else
    {
        /* Preform Activate Card command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_X_ISO14443_3_ActivateIdle(
            pDataParams->pHalDataParams,
            PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_DEFAULT,
            1,
            0,
            NULL,
            NULL,
            &pRxBuffer,
            &wRxLength));

        /* Retrieve SAK byte */
        *pSak = pRxBuffer[2];

        /* Retrieve UID */
        pDataParams->bUidLength = pRxBuffer[3];
        memcpy(pDataParams->abUid, &pRxBuffer[4], pDataParams->bUidLength);  /* PRQA S 3200 */

        /* Return UID */
        *pLenUidOut = pDataParams->bUidLength;
        memcpy(pUidOut, pDataParams->abUid, pDataParams->bUidLength);  /* PRQA S 3200 */

        /* set default card timeout */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS,
            PHPAL_I14443P3A_TIMEOUT_DEFAULT_MS));

        /* enable TxCrc */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_TXCRC,
            PH_ON));

        /* enable RxCrc */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(
            pDataParams->pHalDataParams,
            PHHAL_HW_CONFIG_RXCRC,
            PH_ON));
    }

    /* Set UID complete flag */
    pDataParams->bUidComplete = 1;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P3A);
}

phStatus_t phpalI14443p3a_SamAV2_X_Exchange(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    /* Preform Exchange command */
    return phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength);
}

phStatus_t phpalI14443p3a_SamAV2_X_GetSerialNo(
    phpalI14443p3a_SamAV2_X_DataParams_t * pDataParams,
    uint8_t * pRxBuffer,
    uint8_t * pRxLength
    )
{
    /* Check if UID is valid */
    if (!pDataParams->bUidComplete)
    {
        return PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_PAL_ISO14443P3A);
    }

    memcpy(pRxBuffer,pDataParams->abUid, pDataParams->bUidLength);  /* PRQA S 3200 */
    *pRxLength = pDataParams->bUidLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_ISO14443P3A);
}

#endif /* NXPBUILD__PHPAL_I14443P3A_SAMAV2_X */
