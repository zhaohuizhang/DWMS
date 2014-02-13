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
* Generic MIFARE Plus(R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:02 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#include <ph_Status.h>
#include <phpalMifare.h>
#include <phalMfp.h>
#include "phalMfp_Int.h"
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFP

phStatus_t phalMfp_Int_ComputeErrorResponse(
    uint16_t wNumBytesReceived,
    uint8_t bStatus,
    uint8_t bLayer4Comm
    )
{
    phStatus_t  PH_MEMLOC_REM status;

    /* Invalid error response */
    if (wNumBytesReceived == 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    /* validate received response */
    if (wNumBytesReceived == 1)
    {
        if (bLayer4Comm)
        {
            switch (bStatus)
            {
            case PHAL_MFP_RESP_ACK_ISO4:
                status = PH_ERR_SUCCESS;
                break;

            case PHAL_MFP_RESP_ERR_AUTH:

                status = PHAL_MFP_ERR_AUTH;
                break;

            case PHAL_MFP_RESP_ERR_CMD_OVERFLOW:

                status = PHAL_MFP_ERR_CMD_OVERFLOW;
                break;

            case PHAL_MFP_RESP_ERR_MAC_PCD:

                status = PHAL_MFP_ERR_MAC_PCD;
                break;

            case PHAL_MFP_RESP_ERR_BNR:

                status = PHAL_MFP_ERR_BNR;
                break;

            case PHAL_MFP_RESP_ERR_CMD_INVALID:

                status = PHAL_MFP_ERR_CMD_INVALID;
                break;

            case PHAL_MFP_RESP_ERR_FORMAT:

                status = PHAL_MFP_ERR_FORMAT;
                break;

            case PHAL_MFP_RESP_ERR_GEN_FAILURE:

                status = PHAL_MFP_ERR_GEN_FAILURE;
                break;
            case PHAL_MFP_RESP_ERR_EXT:
                status = PHAL_MFP_ERR_EXT;
                break;
            default:

                status = PH_ERR_PROTOCOL_ERROR;
                break;
            }

            return PH_ADD_COMPCODE(status, PH_COMP_AL_MFP);
        }else
        {
            switch(bStatus)
            {
            case PHAL_MFP_RESP_ACK_ISO3:
            case PHAL_MFP_RESP_ACK_ISO4:
                status = PH_ERR_SUCCESS;
                break;
                /* Mapping of NAK codes: */
            case PHAL_MFP_RESP_NACK0:
                status =  PHPAL_MIFARE_ERR_NAK0;
                break;
            case PHAL_MFP_RESP_NACK1:
                status =  PHPAL_MIFARE_ERR_NAK1;
                break;
            case PHAL_MFP_RESP_NACK4:
                status =  PHPAL_MIFARE_ERR_NAK4;
                break;
            case PHAL_MFP_RESP_NACK5:
                status =  PHPAL_MIFARE_ERR_NAK5;
                break;
            default:
                status  = PH_ERR_PROTOCOL_ERROR;
                break;
            }

            return PH_ADD_COMPCODE(status, PH_COMP_PAL_MIFARE); /* For compatibility reasons with SW stack we use here PAL MIFARE */
        }

    }

    /* validate received response with wNumBytesReceived > 1 */
    if (bStatus != PHAL_MFP_RESP_ACK_ISO4)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    /* proper error response */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_WritePerso(
                                  void * pPalMifareDataParams,
                                  uint8_t bLayer4Comm,
                                  uint16_t wBlockNr,
                                  uint8_t * pValue
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[3];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* parameter checking */
    if (pValue == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* command frame */
    aCmd[0] = PHAL_MFP_CMD_WRITEPERSO;
    aCmd[1] = (uint8_t)(wBlockNr & 0xFF); /* LSB */
    aCmd[2] = (uint8_t)(wBlockNr >> 8);   /* MSB */

    /* exchange command/response */
    if (bLayer4Comm)
    {
        /* command header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            3,
            &pResponse,
            &wRxLength));

        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pValue,
            PHAL_MFP_MIFARE_BLOCK_SIZE,
            &pResponse,
            &wRxLength));
    }
    else
    {
        /* command header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
            pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            3,
            &pResponse,
            &wRxLength));

        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
            pPalMifareDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pValue,
            PHAL_MFP_MIFARE_BLOCK_SIZE,
            &pResponse,
            &wRxLength));
    }

    /* check response */
    if (wRxLength == 1)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], bLayer4Comm));
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_CommitPerso(
                                   void * pPalMifareDataParams,
                                   uint8_t bLayer4Comm
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    aCmd[0] = PHAL_MFP_CMD_COMMITPERSO;

    /* exchange command/response */
    if (bLayer4Comm)
    {
        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
            pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            aCmd,
            1,
            &pResponse,
            &wRxLength));
    }
    else
    {
        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
            pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            aCmd,
            1,
            &pResponse,
            &wRxLength));
    }

    /* check response */
    if (wRxLength == 1)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], bLayer4Comm));
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_ResetAuth(void * pPalMifareDataParams)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1 /* command code */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    aCmd[0] = PHAL_MFP_CMD_RAUTH;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResponse,
        &wRxLength));

    /* check response */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 1));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_PrepareProximityCheck(
    void * pPalMifareDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* command code */
    aCmd[0] = PHAL_MFP_CMD_PPC;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL4(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        1,
        &pResponse,
        &wRxLength));

    /* check response */
    if (wRxLength == 1)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 1));
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_ProximityCheck(
                                      void * pPalMifareDataParams,
                                      uint8_t bNumSteps,
                                      uint8_t * pRndC,
                                      uint8_t * pRndRC
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1 /* command code */ + 1 /* length */ + 7 /* max RndC length */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;
    uint8_t     PH_MEMLOC_REM bPayloadLen;
    uint8_t     PH_MEMLOC_REM bRndCLen = 0;
    uint8_t     PH_MEMLOC_REM bRndRCLen = 0;

    /* parameter checking */
    if ((bNumSteps == 0) || (bNumSteps > PHAL_MFP_PC_RND_LEN) || (pRndC == NULL) || (pRndRC == NULL))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* command code */
    aCmd[0] = PHAL_MFP_CMD_PC;

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
            bPayloadLen = PHAL_MFP_PC_RND_LEN - bRndCLen;
        }

        /* Length */
        aCmd[1] = bPayloadLen;

        /* RndC */
        memcpy(&aCmd[2], &pRndC[bRndCLen], bPayloadLen);  /* PRQA S 3200 */

        /* command exchange */
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangePc(
            pPalMifareDataParams,
            PH_EXCHANGE_DEFAULT,
            aCmd,
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
            PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 1));
        }
    }

    /* We expect to have exactly 7 bytes RndR + 7 bytes RndC */
    if (bRndRCLen != (PHAL_MFP_PC_RND_LEN * 2))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_MultiBlockRead(
                                      void * pPalMifareDataParams,
                                      uint8_t bBlockNr,
                                      uint8_t bNumBlocks,
                                      uint8_t * pBlocks
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1 /* command code */ + 1 /* wBlockNr */ + 1 /* bNumBlocks */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* parameter checking */
    if (pBlocks == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* command frame */
    aCmd[0] = PHAL_MFP_CMD_MBREAD;
    aCmd[1] = bBlockNr;
    aCmd[2] = bNumBlocks;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        3,
        &pResponse,
        &wRxLength));

    /* check response */
    if ((wRxLength % PHAL_MFP_MIFARE_BLOCK_SIZE == 0) && (wRxLength <= (uint16_t)(3 * PHAL_MFP_MIFARE_BLOCK_SIZE)))
    {
        /* pass back read bytes */
        memcpy(pBlocks, pResponse, wRxLength);  /* PRQA S 3200 */
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phalMfp_Int_ComputeErrorResponse(wRxLength, pResponse[0], 0));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_MultiBlockWrite(
                                       void * pPalMifareDataParams,
                                       uint8_t bBlockNr,
                                       uint8_t bNumBlocks,
                                       uint8_t * pBlocks
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[1 /* command code */ + 1 /* wBlockNr */ + 1 /* bNumBlocks */];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* parameter checking */
    if (pBlocks == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_MFP);
    }

    /* command frame */
    aCmd[0] = PHAL_MFP_CMD_MBWRITE;
    aCmd[1] = bBlockNr;
    aCmd[2] = bNumBlocks;

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        3,
        &pResponse,
        &wRxLength));

    /* command exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        pBlocks,
        (uint16_t)(bNumBlocks * PHAL_MFP_MIFARE_BLOCK_SIZE),
        &pResponse,
        &wRxLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_CreateValueBlock(
                                        uint8_t * pValue,
                                        uint8_t bAddrData,
                                        uint8_t * pBlock
                                        )
{
    pBlock[0] = (uint8_t)(pValue[0]);
    pBlock[1] = (uint8_t)(pValue[1]); 
    pBlock[2] = (uint8_t)(pValue[2]);
    pBlock[3] = (uint8_t)(pValue[3]);
    pBlock[4] = (uint8_t)(~pBlock[0]);
    pBlock[5] = (uint8_t)(~pBlock[1]);
    pBlock[6] = (uint8_t)(~pBlock[2]);
    pBlock[7] = (uint8_t)(~pBlock[3]);
    pBlock[8] = (uint8_t)(pBlock[0]);
    pBlock[9] = (uint8_t)(pBlock[1]);
    pBlock[10] = (uint8_t)(pBlock[2]);
    pBlock[11] = (uint8_t)(pBlock[3]);
    pBlock[12] = (uint8_t)bAddrData;
    pBlock[13] = (uint8_t)~bAddrData;
    pBlock[14] = (uint8_t)bAddrData;
    pBlock[15] = (uint8_t)~bAddrData;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

phStatus_t phalMfp_Int_CheckValueBlockFormat(
    uint8_t * pBlock
    )
{
    /* check format of value block */
    if ((pBlock[0] != pBlock[8]) ||
        (pBlock[1] != pBlock[9]) ||
        (pBlock[2] != pBlock[10]) ||
        (pBlock[3] != pBlock[11]) ||
        (pBlock[4] != (pBlock[0] ^ 0xFF)) ||
        (pBlock[5] != (pBlock[1] ^ 0xFF)) ||
        (pBlock[6] != (pBlock[2] ^ 0xFF)) ||
        (pBlock[7] != (pBlock[3] ^ 0xFF)) ||
        (pBlock[12] != pBlock[14]) ||
        (pBlock[13] != pBlock[15]) ||
        (pBlock[12] != (pBlock[13]^ 0xFF)))
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFP);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFP);
}

#endif /* NXPBUILD__PHAL_MFP */
