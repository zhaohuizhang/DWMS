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
* Internal functions of Virtual Card Architecture (R) Application Component.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:24 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <ph_Status.h>
#include <phpalMifare.h>
#include <phalVca.h>
#include "phalVca_Int.h"
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_VCA

phStatus_t phalVca_Int_ComputeErrorResponse(
    uint16_t wNumBytesReceived,
    uint8_t bStatus
    )
{
    phStatus_t PH_MEMLOC_REM status;

    /* validate received response */
    if (wNumBytesReceived > 1)
    {
        if (bStatus != PHAL_VCA_RESP_ACK_ISO4)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
        }

        /* proper error response */
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_VCA);
    }
    else if (wNumBytesReceived == 1)
    {
        switch (bStatus)
        {
        case PHAL_VCA_RESP_ACK_ISO4:
            
            status = PH_ERR_SUCCESS;
            break;

        case PHAL_VCA_RESP_ERR_CMD_INVALID:

            status = PHAL_VCA_ERR_CMD_INVALID;
            break;

        case PHAL_VCA_RESP_ERR_FORMAT:

            status = PHAL_VCA_ERR_FORMAT;
            break;       

        case PHAL_VCA_RESP_ERR_GEN:

            status = PHAL_VCA_ERR_GEN;
            break;      
        default:

            status = PH_ERR_PROTOCOL_ERROR;
            break;
        }

        return PH_ADD_COMPCODE(status, PH_COMP_AL_VCA);
    }
    /* Invalid error response */
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_VCA);
    }
}

phStatus_t phalVca_Int_PrepareProximityCheck(
    void * pPalMifareDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    bCmd[0] = PHAL_VCA_CMD_PPC;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        bCmd,
        1,
        &pResponse,
        &wRxLength));

    /* check response */
    if (wRxLength == 1)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalVca_Int_ProximityCheck(
                                      void * pPalMifareDataParams,  /**< [In] Pointer to a palMifare component context. */
                                      uint8_t bNumSteps,            /**< [In] Number of steps for the PC (1..7). */
                                      uint8_t * pRndC,              /**< [In] RndC[7]. */
                                      uint8_t * pRndRC              /**< [Out] RndRC[14]. */
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmd[1 /* command code */ + 1 /* length */ + 7 /* max RndC length */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM bPayloadLen;
    uint8_t     PH_MEMLOC_REM bRndCLen = 0;
    uint8_t     PH_MEMLOC_REM bRndRCLen = 0;

    /* parameter checking */
    if ((bNumSteps == 0) || (bNumSteps > PHAL_VCA_PC_RND_LEN) || (pRndC == NULL) || (pRndRC == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* command code */
    bCmd[0] = PHAL_VCA_CMD_PC;

    /* Proximity Check loop */
    while (bNumSteps--)
    {
        /* RndC length */
        if (bNumSteps)
        {
            bPayloadLen = 1;
        }
        else
        {
            bPayloadLen = PHAL_VCA_PC_RND_LEN - bRndCLen;
        }

        /* Length */
        bCmd[1] = bPayloadLen;

        /* RndC */
        memcpy(&bCmd[2], &pRndC[bRndCLen], bPayloadLen);  /* PRQA S 3200 */

        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangePc(
            pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            bCmd,
            2 + bPayloadLen,
            &pResponse,
            &wRxLength));

        /* check response */
        if (wRxLength == (uint16_t)bPayloadLen)
        {
            /* copy RndR */
            memcpy(&pRndRC[bRndRCLen], pResponse, wRxLength);         /* PRQA S 3200 */
            bRndRCLen = bRndRCLen + (uint8_t)wRxLength;

            /* copy RndC */
            memcpy(&pRndRC[bRndRCLen], &pRndC[bRndCLen], wRxLength);  /* PRQA S 3200 */
            bRndRCLen = bRndRCLen + (uint8_t)wRxLength;
            bRndCLen = bRndCLen + (uint8_t)wRxLength;
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phalVca_Int_ComputeErrorResponse(wRxLength, pResponse[0]));
        }
    }

    /* We expect to have exactly 7 bytes RndR + 7 bytes RndC */
    if (bRndRCLen != (PHAL_VCA_PC_RND_LEN * 2))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

#endif /* NXPBUILD__PHAL_VCA */
