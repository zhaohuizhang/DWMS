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

uint8_t phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(
    uint8_t * pApdu,
    uint8_t bCmdIndex
    )
{
    pApdu[0] = PHAL_MFDF_WRAPPEDAPDU_CLA;/* Encode wrapped APDU class*/
    pApdu[1] = pApdu[4];  /* Desfire command code. */
    pApdu[2] = PHAL_MFDF_WRAPPEDAPDU_P1; /* P1 set to zero. */
    pApdu[3] = PHAL_MFDF_WRAPPEDAPDU_P2; /* P2 set to zero. */
    /* Encode APDU Length*/
    pApdu[4]= bCmdIndex-PHAL_MFDF_WRAP_HDR_LEN; /* Set APDU Length. */
    return bCmdIndex;
}


phStatus_t phalMfdf_SamAV2_X_Int_HalToMfdf(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wStatus
    )
{
    uint16_t PH_MEMLOC_REM statusTmp = 0;

    if ((wStatus & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_DESFIRE_GEN)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(
            pDataParams->pHalSamDataParams,
            PHHAL_HW_CONFIG_ADDITIONAL_INFO,
            &wStatus));
        if( wStatus & 0xff00 )
        {
            if( (wStatus & 0xff00)== 0x9100 )
            {  
                /* For wrapped mode. */
                statusTmp = phalMfdf_Int_ComputeErrorResponse(pDataParams, (uint8_t)wStatus);
            }
            else
            {
                statusTmp = phalMfdf_Int_ComputeErrorResponse(pDataParams, wStatus);
            }
        }
        else
        {
            statusTmp = phalMfdf_Int_ComputeErrorResponse(pDataParams, (uint8_t)wStatus);
        }
    }
    else
    {
        if((wStatus & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_AL_MFDF);
        }
        else
        {
            /* Should be a SAM HAL error */
            return wStatus;
        }
    }

    return statusTmp;
}

#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */
