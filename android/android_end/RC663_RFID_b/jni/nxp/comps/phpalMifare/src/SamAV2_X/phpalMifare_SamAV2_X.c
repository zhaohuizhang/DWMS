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
* SamAV2_X MIFARE(R) Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:33 2011 $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#include <ph_Status.h>
#include <phhalHw.h>
#include <phpalMifare.h>
#include <phpalI14443p3a.h>
#include <phpalI14443p4.h>
#include <ph_RefDefs.h>
#include <phTools.h>

#ifdef NXPBUILD__PHPAL_MIFARE_SAMAV2_X

#include "phpalMifare_SamAV2_X.h"
#include "phpalMifare_SamAV2_X_Int.h"
#include "../phpalMifare_Int.h"
#include <phhalHw_SamAV2_Cmd.h>

phStatus_t phpalMifare_SamAV2_X_Init(
                                     phpalMifare_SamAV2_X_DataParams_t * pDataParams,
                                     uint16_t wSizeOfDataParams,
                                     phhalHw_SamAV2_DataParams_t * pHalDataParams,
                                     void * pPalI14443p4DataParams
                                     )
{
    if (sizeof(phpalMifare_SamAV2_X_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_PAL_MIFARE);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pHalDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_PAL_MIFARE | PHPAL_MIFARE_SAMAV2_X_ID;
    pDataParams->pHalDataParams         = pHalDataParams;
    pDataParams->pPalI14443p4DataParams = pPalI14443p4DataParams;
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_MIFARE);
}

phStatus_t phpalMifare_SamAV2_X_ExchangeL3(
    phpalMifare_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wValidBits;
    uint16_t    PH_MEMLOC_REM wCrcIn;
    uint16_t    PH_MEMLOC_REM wCrcCalc;
    uint16_t    PH_MEMLOC_REM wMfCryptoOff;
	uint16_t	PH_MEMLOC_REM RxLength;
	uint8_t *	PH_MEMLOC_REM pRxBuffer;

	/* Check if caller has provided valid RxBuffer */
	if (ppRxBuffer == NULL)
	{
		ppRxBuffer = &pRxBuffer;
	}
	if (pRxLength == NULL)
	{
		pRxLength = &RxLength;
	}

    /* Get status of Mifare Crypto */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_DISABLE_MF_CRYPTO1, &wMfCryptoOff));

    /* Switch CRC modes in case of first part of exchange and MIFARE Crypto is off. */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT) && wMfCryptoOff)
    {
        /* Enable TxCrc */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));

        /* Disable RxCrc */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));
    }

    /* Perform Exchange */
    status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength);

    /* Return if no real exchange is done */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        return status;
    }

    /* Convert HAL status codes to palMifare status codes */
    status = phpalMifare_SamAV2_X_ConvertNak(status);

    /* ACK/NAK Handling */
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        /* Check for protocol error */
        if (*pRxLength != 1)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_MIFARE);
        }

        /* Retrieve bitcount */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXLASTBITS, &wValidBits));
        
        /* Check for protocol error */
        if (wValidBits != 4)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_MIFARE);
        }

        /* ACK/NAK Mapping */
        switch ((*ppRxBuffer)[0])
        {
            /* ACK -> everything OK */
        case PHPAL_MIFARE_RESP_ACK:
            status = PH_ERR_SUCCESS;
            break;
            /* Mapping of NAK codes: */
        case PHPAL_MIFARE_RESP_NAK0:
            status = PHPAL_MIFARE_ERR_NAK0;
            break;
        case PHPAL_MIFARE_RESP_NAK1:
            status = PHPAL_MIFARE_ERR_NAK1;
            break;
        case PHPAL_MIFARE_RESP_NAK4:
            status = PHPAL_MIFARE_ERR_NAK4;
            break;
        case PHPAL_MIFARE_RESP_NAK5:
            status = PHPAL_MIFARE_ERR_NAK5;
            break;
        default:
            status = PH_ERR_PROTOCOL_ERROR;
            break;
        }
    }
    /* Normal data stream with CRC */
    else
    {
        /* Check status */
        PH_CHECK_SUCCESS(status);

        /* Check length (min. 1 byte + 2 byte CRC) */
        if (*pRxLength < 3)
        {
            return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_PAL_MIFARE);
        }

        /* Only perform CRC handling, when MIFARE Crypto is off. Otherwise this is done by the SAM */
        if (wMfCryptoOff)
        {
            /* Retrieve CRC */
            wCrcIn  = (uint16_t)(((uint16_t)(*ppRxBuffer)[(*pRxLength) - 1]) << 8);
            wCrcIn |= (uint16_t)((*ppRxBuffer)[(*pRxLength) - 2]);

            /* Remove CRC from input data */
            *pRxLength -= 2;

            /* Calculate CRC */
            PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                PH_TOOLS_CRC_OPTION_DEFAULT,
                PH_TOOLS_CRC16_PRESET_ISO14443A,
                PH_TOOLS_CRC16_POLY_ISO14443,
                *ppRxBuffer, 
                *pRxLength, 
                &wCrcCalc));

            /* CRC Check -> Compare input and calculated crc */
            if (wCrcIn == wCrcCalc)
            {
                status = PH_ERR_SUCCESS;
            }
            else
            {
                status = PH_ERR_INTEGRITY_ERROR;
            }
        }
    }
    
    return PH_ADD_COMPCODE(status, PH_COMP_PAL_MIFARE);
}

phStatus_t phpalMifare_SamAV2_X_ExchangeL4(
    phpalMifare_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
#ifdef NXPBUILD__PHPAL_I14443P4
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wValue;
    
    /* Not available if no Layer4 has been set */
    if (pDataParams->pPalI14443p4DataParams == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_MIFARE);
    }

    /* Retrieve TxCrc-setting */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, &wValue));

    /* Enable TxCrc */
    if (wValue == PH_OFF)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON));
    }

    /* Retrieve RxCrc-setting */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, &wValue));

    /* Enable RxCrc */
    if (wValue == PH_OFF)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_ON));
    }

    /* Perform Exchange */
    PH_CHECK_SUCCESS_FCT(statusTmp,  phpalI14443p4_Exchange(
        pDataParams->pPalI14443p4DataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_MIFARE);
#else
    /* satisfy compiler */
    if (pDataParams || wOption || pTxBuffer || wTxLength || ppRxBuffer || pRxLength);
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_MIFARE);
#endif
}

phStatus_t phpalMifare_SamAV2_X_ExchangePc(
    phpalMifare_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    /* satisfy compiler */
    if (pDataParams || wOption || pTxBuffer || wTxLength || ppRxBuffer || pRxLength);
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_PAL_MIFARE);
}

phStatus_t phpalMifare_SamAV2_X_ExchangeRaw(
    phpalMifare_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t bTxLastBits,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength,
    uint8_t * pRxLastBits
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wValue = 0;
	uint16_t	PH_MEMLOC_REM RxLength;
	uint8_t *	PH_MEMLOC_REM pRxBuffer;

	/* Check if caller has provided valid RxBuffer */
	if (ppRxBuffer == NULL)
	{
		ppRxBuffer = &pRxBuffer;
	}
	if (pRxLength == NULL)
	{
		pRxLength = &RxLength;
	}

    /* Do not switch Parity / CRC modes if no real exchange is done */
    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* TxLastBits > 7 is invalid  */
        if (bTxLastBits > 7)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_MIFARE);
        }

        /* Retrieve TxCrc-setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, &wValue));

        /* Disable TxCrc */
        if (wValue == PH_ON)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF));
        }

        /* Retrieve RxCrc-setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, &wValue));

        /* Disable RxCrc */
        if (wValue == PH_ON)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXCRC, PH_OFF));
        }

        /* Retrieve Parity-setting */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_PARITY, &wValue));

        /* Disable Parity */
        if (wValue == PH_ON)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_PARITY, PH_OFF));
        }

        /* Set TxLastBits */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, bTxLastBits));
    }
    else
    {
        /* TxLastBits != 0 is invalid for buffered operation */
        if (bTxLastBits != 0)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_PAL_MIFARE);
        }
    }

    /* Perform Exchange */
    status = phhalHw_Exchange(
        pDataParams->pHalDataParams,
        wOption,
        pTxBuffer,
        wTxLength,
        ppRxBuffer,
        pRxLength);

    /* Return if no real exchange is done */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        return status;
    }

    /* Restore Parity-setting again since many PAL layers expect it */
    if (wValue == PH_ON)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_PARITY, wValue));
    }

    /* Retrieve RxLastBits */
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_RXLASTBITS, &wValue));
        *pRxLastBits = (uint8_t)wValue;
    }
    else
    {
        PH_CHECK_SUCCESS(status);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_PAL_MIFARE);
}

phStatus_t phpalMifare_SamAV2_X_MfcAuthenticateKeyNo(
    phpalMifare_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Execute HAL authenticate function */
    status = phhalHw_MfcAuthenticateKeyNo(
        pDataParams->pHalDataParams,
        bBlockNo,
        bKeyType,
        wKeyNo,
        wKeyVersion,
        pUid);

    /* Change component code for AUTH error and invalid paramter */
    if (((status & PH_ERR_MASK) == PH_ERR_AUTH_ERROR) ||
        ((status & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER))
    {
        status = PH_ADD_COMPCODE((status & PH_ERR_MASK), PH_COMP_PAL_MIFARE);
    }

    return status;
}

phStatus_t phpalMifare_SamAV2_X_MfcAuthenticate(
    phpalMifare_SamAV2_X_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
    )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Execute HAL authenticate function */
    status = phhalHw_MfcAuthenticate(
        pDataParams->pHalDataParams,
        bBlockNo,
        bKeyType,
        pKey,
        pUid);

    /* Change component code for AUTH error and invalid paramter */
    if (((status & PH_ERR_MASK) == PH_ERR_AUTH_ERROR) ||
        ((status & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER))
    {
        status = PH_ADD_COMPCODE((status & PH_ERR_MASK), PH_COMP_PAL_MIFARE);
    }

    return status;
}

phStatus_t phpalMifare_SamAV2_X_ConvertNak(
    phStatus_t status
    )
{
    switch (status & PH_ERR_MASK)
    {
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK0:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK0, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK1:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK1, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK4:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK4, PH_COMP_PAL_MIFARE);
    case PHHAL_HW_SAMAV2_ERR_MIFARE_NAK5:
        return PH_ADD_COMPCODE(PHPAL_MIFARE_ERR_NAK5, PH_COMP_PAL_MIFARE);
    default:
        return status;
    }
}

#endif /* NXPBUILD__PHPAL_MIFARE_SAMAV2_X */
