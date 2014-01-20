/*
*         Copyright(c), NXP Semiconductors Gratkorn / Austria
*
*                  (C)NXP Semiconductors
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
* Software ISO15693 application layer of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:49 2011 $
*
* History:
*  CHu: Generated 10. November 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalSli15693.h>
#include <phalI15693.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_I15693_SW

#include "phalI15693_Sw.h"
#include "phalI15693_Sw_Int.h"

phStatus_t phalI15693_Sw_Init(
                              phalI15693_Sw_DataParams_t * pDataParams,
                              uint16_t wSizeOfDataParams,
                              void * pPalSli15693DataParams
                              )
{
    if (sizeof(phalI15693_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }
    PH_ASSERT_NULL(pDataParams);
    PH_ASSERT_NULL(pPalSli15693DataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_I15693 | PHAL_I15693_SW_ID;
    pDataParams->pPalSli15693DataParams = pPalSli15693DataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_I15693);
}

phStatus_t phalI15693_Sw_ReadSingleBlock(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[2];

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_READ_SINGLE_BLOCK;
    bCommand[1] = bBlockNo;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        ppRxBuffer,
        pRxLength));

    /* Length check */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        if (*pRxLength < 2)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);
        }
    }
    else
    {
        if (*pRxLength == 0)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_I15693);
}

phStatus_t phalI15693_Sw_ReadMultipleBlocks(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[3];

    /* Number of wNumBlocks can't be zero */
    if (wNumBlocks == 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* adjust number of blocks. */
    --wNumBlocks;

    /* Check number of blocks doesn't exceed 256 */
    if (((uint16_t)bBlockNo + wNumBlocks) >= 0x100)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_READ_MULTIPLE_BLOCKS;
    bCommand[1] = bBlockNo;
    bCommand[2] =(uint8_t)wNumBlocks;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        3,
        ppRxBuffer,
        pRxLength));

    /* Length check */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        if (*pRxLength <((wNumBlocks + 1) * 2))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);
        }
    }
    else
    {
        if (*pRxLength <(wNumBlocks+1))
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_I15693);
}

phStatus_t phalI15693_Sw_WriteSingleBlock(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint8_t * pTxBuffer,
    uint16_t wTxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_WRITE_SINGLE_BLOCK;
    bCommand[1] = bBlockNo;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCommand,
        2,
        NULL,
        NULL));

    /* Append given data */
    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pTxBuffer,
        wTxLength,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_WriteMultipleBlocks(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t * pTxBuffer,
    uint16_t wTxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[3];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Number of wNumBlocks can't be zero */
    if (wNumBlocks == 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* adjust number of blocks. */
    --wNumBlocks;

    /* Check number of blocks doesn't exceed 256 */
    if (((uint16_t)bBlockNo + wNumBlocks) >= 0x100)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_WRITE_MULTIPLE_BLOCKS;
    bCommand[1] = bBlockNo;
    bCommand[2] =(uint8_t)wNumBlocks;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        bCommand,
        3,
        NULL,
        NULL));

    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pTxBuffer,
        wTxLength,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_LockBlock(
                                    phalI15693_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t bBlockNo
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_LOCK_BLOCK;
    bCommand[1] = bBlockNo;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_WriteAFI(
                                  phalI15693_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption,
                                  uint8_t bAfi
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_WRITE_AFI;
    bCommand[1] = bAfi;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_LockAFI(
                                  phalI15693_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption
                                  )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[1];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_LOCK_AFI;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        1,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_WriteDSFID(
                                    phalI15693_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t bDsfid
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_WRITE_DSFID;
    bCommand[1] = bDsfid;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        2,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_LockDSFID(
                                    phalI15693_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption
                                    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM bCommand[1];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM bRxLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_LOCK_DSFID;

    /* Set or clear the flags option bit indicated by bOption. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, bOption));

    /* Set long timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_LONG_US));

    /* Proceed with the command in lower layers */
    status = phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        1,
        &pRxBuffer,
        &bRxLength);

    /* Write-alike handling */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        return phalI15693_Sw_WriteAlikeHandling(pDataParams, status);
    }

    return status;
}

phStatus_t phalI15693_Sw_GetSystemInformation(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[1];
    uint8_t     PH_MEMLOC_REM bExpLength;

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_GET_SYSTEM_INFO;

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, PHAL_I15693_OPTION_OFF));

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        1,
        ppRxBuffer,
        pRxLength));

    /* Check number of data received */
    if (*pRxLength == 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);
    }

    /* Expected length is 9 by default */
    bExpLength = 9;

    /* Check for DSFID byte */
    if ((*ppRxBuffer[0]) & PHAL_I15693_INFO_FLAGS_DSFID)
    {
        ++bExpLength;
    }

    /* Check for AFI byte */
    if ((*ppRxBuffer[0]) & PHAL_I15693_INFO_FLAGS_AFI)
    {
        ++bExpLength;
    }

    /* Check for MEMSIZE bytes */
    if ((*ppRxBuffer[0]) & PHAL_I15693_INFO_FLAGS_MEMSIZE)
    {
        bExpLength += 2;
    }

    /* Check for ICREF byte */
    if ((*ppRxBuffer[0]) & PHAL_I15693_INFO_FLAGS_ICREF)
    {
        ++bExpLength;
    }

    /* Check response length */
    if (*pRxLength < bExpLength)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_I15693);
}

phStatus_t phalI15693_Sw_GetMultipleBlockSecurityStatus(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint16_t wNumBlocks,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCommand[3];

    /* Number of wNumBlocks can't be zero */
    if (wNumBlocks == 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* adjust number of blocks. */
    --wNumBlocks;

    /* Check number of blocks doesn't exceed 256 */
    if (((uint16_t)bBlockNo + wNumBlocks) >= 0x100)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* Clear Option bit */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalI15693_Sw_Int_SetOptionBit(pDataParams, PHAL_I15693_OPTION_OFF));

    /* Build command */
    bCommand[0] = PHAL_I15693_SW_CMD_GET_MULTIPLE_BLOCK_SEC;
    bCommand[1] = bBlockNo;
    bCommand[2] = (uint8_t)wNumBlocks;

    /* Set short timeout. */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_TIMEOUT_US,
        PHPAL_SLI15693_TIMEOUT_SHORT_US));

    /* Proceed with the command in lower layers */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_Exchange(
        pDataParams->pPalSli15693DataParams,
        PH_EXCHANGE_DEFAULT,
        bCommand,
        3,
        ppRxBuffer,
        pRxLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_I15693);
}

phStatus_t phalI15693_Sw_WriteAlikeHandling(
    phalI15693_Sw_DataParams_t * pDataParams,
    phStatus_t wExchangeStatus
    )
{
    uint8_t     PH_MEMLOC_REM bDsfid;
    uint8_t     PH_MEMLOC_REM bUid[PHPAL_SLI15693_UID_LENGTH];
    uint8_t     PH_MEMLOC_REM bUidLength;
    uint8_t     PH_MEMLOC_REM bData[1];
    uint16_t    PH_MEMLOC_REM wDataLength;

    switch (wExchangeStatus & PH_ERR_MASK)
    {
        /* Check for protocol error  */
    case PH_ERR_SUCCESS:
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_I15693);

        /* Timeout is correct behaviour, send EOF */
    case PH_ERR_IO_TIMEOUT:
        /* card answers after next EOF -> correct status is timeout */
        return phpalSli15693_SendEof(
            pDataParams->pPalSli15693DataParams,
            PHPAL_SLI15693_EOF_WRITE_ALIKE,
            &bDsfid,
            bUid,
            &bUidLength,
            bData,
            &wDataLength);

        /* Framing errors etc. are ignored and the waiting until EOF sending is continued */
    case PH_ERR_INTEGRITY_ERROR:
    case PH_ERR_COLLISION_ERROR:
    case PH_ERR_FRAMING_ERROR:
        return phpalSli15693_SendEof(
            pDataParams->pPalSli15693DataParams,
            PHPAL_SLI15693_EOF_WRITE_ALIKE_WITH_WAIT,
            &bDsfid,
            bUid,
            &bUidLength,
            bData,
            &wDataLength);

        /* Directly return all other errors */
    default:
        return wExchangeStatus;
    }
}

phStatus_t phalI15693_Sw_Int_SetOptionBit(
    phalI15693_Sw_DataParams_t * pDataParams,
    uint8_t bOption
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wFlagsCurrent;
    uint16_t    PH_MEMLOC_REM wFlagsNew;

    /* Next check if the Option Byte has a valid value */
    if ((bOption != PHAL_I15693_OPTION_OFF) &&(bOption != PHAL_I15693_OPTION_ON))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_AL_I15693);
    }

    /* retrieve the flags byte */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_GetConfig(
        pDataParams->pPalSli15693DataParams,
        PHPAL_SLI15693_CONFIG_FLAGS,
        &wFlagsCurrent));

    /* Set Option bit */
    if (bOption != PHAL_I15693_OPTION_OFF)
    {
        wFlagsNew = wFlagsCurrent | PHPAL_SLI15693_FLAG_OPTION;
    }
    /* Clear option bit */
    else
    {
        wFlagsNew = wFlagsCurrent &(uint8_t)~(uint8_t)PHPAL_SLI15693_FLAG_OPTION;
    }

    /* update the flags byte */
    if (wFlagsNew != wFlagsCurrent)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phpalSli15693_SetConfig(
            pDataParams->pPalSli15693DataParams,
            PHPAL_SLI15693_CONFIG_FLAGS,
            wFlagsNew));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_I15693);
}

#endif
