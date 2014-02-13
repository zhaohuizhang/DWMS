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
* Internal functions for RD710Sam BAL Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:52 2011 $
*
* History:
*  CHu: Generated 05. July 2010
*
*/

#include <phbalReg.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHBAL_REG_RD710SAM

#include "phbalReg_Rd710Sam_Int.h"

phStatus_t phbalReg_Rd710Sam_CheckResponse(
    uint16_t wCmd,
    uint8_t * pRxBuffer,
    uint16_t wRxLength,
    uint8_t ** ppData,
    uint16_t * pDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLengthTmp;

    /* Reset data length */
    if (ppData != NULL)
    {
        *ppData = NULL;
    }
    if (pDataLength != NULL)
    {
        *pDataLength = 0;
    }

    /* Frame has to be at least 6 bytes */
    if (wRxLength < PHBAL_RD710SAM_FRAME_HEADER_LEN)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_BAL);
    }
    else
    {
        /* Retrieve length */
        wDataLengthTmp = pRxBuffer[PHBAL_RD710SAM_FRAME_LEN_POS];
        wDataLengthTmp |= ((uint16_t)pRxBuffer[PHBAL_RD710SAM_FRAME_LEN_POS+1] << 8);

        /* Length should match */
        if ((wRxLength - PHBAL_RD710SAM_FRAME_HEADER_LEN) != wDataLengthTmp)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_BAL);
        }
        else
        {
            /* Command Code should match */
            if ((pRxBuffer[PHBAL_RD710SAM_FRAME_CMD_POS] != (uint8_t)((uint16_t)wCmd >> 8)) ||
                (pRxBuffer[PHBAL_RD710SAM_FRAME_CMD_POS+1] != ((uint8_t)wCmd | 0x80)))
            {
                return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_BAL);
            }
        }
    }

    /* Retrieve status */
    status = PH_ADD_COMPCODE(
        (uint16_t)pRxBuffer[PHBAL_RD710SAM_FRAME_STATUS_POS],
        ((uint16_t)pRxBuffer[PHBAL_RD710SAM_FRAME_STATUS_POS+1] << 8));

    /* Return data pointer */
    if (ppData != NULL)
    {
        *ppData = &pRxBuffer[PHBAL_RD710SAM_FRAME_PAYLOAD_POS];
    }
    /* Return data length  */
    if (pDataLength != NULL)
    {
        *pDataLength = wDataLengthTmp;
    }

    return status;
}

phStatus_t phbalReg_Rd710Sam_ParseAtr(
                                      uint8_t * pAtr,
                                      uint16_t wAtrLenght,
                                      uint8_t * pTa1,
                                      uint8_t * pSpecificMode
                                      )
{
    uint16_t PH_MEMLOC_REM wY;
    uint16_t PH_MEMLOC_REM wIndex = 0;

    /* Length Check */
    if (wAtrLenght < 2)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_BAL);
    }
    
    /* Skip TS */
    ++wIndex;

    /* Retrieve Y1 */
    wY = pAtr[wIndex++];

    /* Parse TA1 (Fi and Di) */
    if (wY & 0x10)
    {
        *pTa1 = pAtr[wIndex++];
    }
    /* Default Di and Di */
    else
    {
        *pTa1 = 0x11;
    }

    /* Ignore TB1 */
    if (wY & 0x20)
    {
        ++wIndex;
    }

    /* Ignore TC1 */
    if (wY & 0x40)
    {
        ++wIndex;
    }

    /* Parse TD1 */
    if (wY & 0x80)
    {
        /* Retrieve Y2 */
        wY = pAtr[wIndex++];

        /* Parse TA2 */
        if (wY & 0x10)
        {
            *pSpecificMode = (uint8_t)((pAtr[wIndex] & 0x80) ? 1 : 0);
        }
        else
        {
            *pSpecificMode = 0;
        }
    }
    else
    {
        *pSpecificMode = 0;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_RD710SAM */
