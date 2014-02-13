/*
*         Copyright(c), NXP Semiconductors Gratkorn / Austria
*
*                    (C)NXP Semiconductors
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
* Wait Routines for SAMAV2 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.12 $
* $Date: Tue May  3 09:21:30 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phbalReg.h>
#include <phhalHw.h>
#include <phKeyStore.h>
#include <phCryptoRng.h>
#include <phCryptoSym.h>
#include <phpalMifare.h>
#include <ph_RefDefs.h>
#include <phTools.h>

#ifdef NXPBUILD__PHHAL_HW_SAMAV2

#include <phhalHw_SamAV2_Cmd.h>
#include "phhalHw_SamAV2.h"
#include "phhalHw_SamAV2_Int.h"
#include "phhalHw_SamAV2_HcUtils.h"
#include "Hc_AV1/phhalHw_SamAV2_Hc_AV1.h"
#include "Hc_AV1/phhalHw_SamAV2_Hc_AV1_Int.h"
#include "Hc_AV2/phhalHw_SamAV2_Hc_AV2.h"
#include "Hc_AV2/phhalHw_SamAV2_Hc_AV2_Int.h"

/*
* Private constants
*/
static const uint8_t PH_MEMLOC_CONST_ROM gaFirstIv[PH_CRYPTOSYM_AES_BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t gaDefaultLe[1] = {PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE};

phStatus_t phhalHw_SamAV2_Cmd_SAM_VerifyMAC(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bNum,
    uint8_t * pTxBuffer,
    uint8_t bTxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* *pRxLength = 0; */

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_VERIFY_MAC_INS;
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bNum;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bTxLength;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        bTxLength,
        NULL,
        NULL));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

        /* Exchange final part with lower layer */
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            NULL,
            0,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
        else
        {
            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }
    }

    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_GenerateMAC(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bNum,
    uint8_t * pTxBuffer,
    uint8_t bTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    *pRxLength = 0;

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_GENERATE_MAC_INS;
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }
        if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bNum;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        }
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bTxLength;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        bTxLength,
        NULL,
        NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Exchange final part with lower layer */
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            /* when an 0xAF is send no Le */
            statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pResponse,
                &wResponseLength);
        }
        else
        {
            statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                gaDefaultLe,
                1,
                &pResponse,
                &wResponseLength);
        }

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
        else
        {
            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        *pRxLength = wResponseLength;
        *ppRxBuffer = pResponse;
    }

    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_EncipherData(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint8_t bTxLength,
    uint8_t   bOffset,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if (pRxLength != NULL)
    {
        *pRxLength = 0;
    }

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_ENCIPHER_DATA_INS;
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bOffset;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bTxLength;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        bTxLength,
        NULL,
        NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Exchange final part with lower layer */
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
        else
        {
            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        /* If we're receiving something, we need valid receive parameters */
        if ((ppRxBuffer == NULL) ||(pRxLength == NULL))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        *pRxLength = wResponseLength;
        *ppRxBuffer = pResponse;
    }

    return statusTmp;
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_DecipherData(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint8_t bTxLength,
    uint8_t * pLenEncInputData,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if (pRxLength != NULL)
    {
        *pRxLength = 0;
    }

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DECIPHER_DATA_INS;

        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

        if (wOption & PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t) bTxLength + 3;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bTxLength;
        }

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        if (wOption & PHHAL_HW_SAMAV2_CMD_DECIPHERDATA_OPTION_WITHLENGTH)
        {
            /* Exchange second part with lower layer */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                pLenEncInputData,
                3,
                NULL,
                NULL));
        }
    }

    /* Exchange third part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        (uint8_t)bTxLength,
        NULL,
        NULL));


    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {

        /* Exchange final part with lower layer */
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
        else
        {
            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        /* If we're receiving something, we need valid receive parameters */
        if ((ppRxBuffer == NULL) ||(pRxLength == NULL))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        *ppRxBuffer = pResponse;
        *pRxLength = wResponseLength;
    }
    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_DecipherOfflineData(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint8_t bTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    *pRxLength = 0;

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DECIPHER_OFFLINE_DATA_INS;
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange third part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        bTxLength,
        NULL,
        NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {

        /* Exchange final part with lower layer */
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
        else
        {
            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        *ppRxBuffer = pResponse;
        *pRxLength = wResponseLength;
    }
    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_EncipherOfflineData(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint8_t bTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    *pRxLength = 0;

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_ENCIPHER_OFFLINE_DATA_INS;
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bTxLength;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        bTxLength,
        NULL,
        NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Exchange final part with lower layer */
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            PH_CHECK_SUCCESS(statusTmp);
        }
        else
        {
            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }

        *pRxLength = wResponseLength;
        *ppRxBuffer = pResponse;
    }

    return statusTmp;
}

/*************************************************************************************************************************/
/**************************************** SAM Key handling Command *******************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/
phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bKeyNo,
    uint8_t bProMas,
    uint8_t * pKeyData,
    uint8_t bKeyDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM pKeyEntryBuffer[PHHAL_HW_SAMAV2_KEYENTRY_SIZE];
    uint8_t     PH_MEMLOC_REM bKeyEntryLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bProMas;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bKeyDataLength;

    /* Buffer the header */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* AV1: Check if we need plain transmission */
    if ((pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE) &&((bOption & PHHAL_HW_SAMAV2_CMD_CHANGE_KEYENTRY_PLAIN) != 0))
    {
        pDataParams->bChangeEntryPlain = PH_ON;
    }
    else
    {
        /* Switch off plain transmission */
        pDataParams->bChangeEntryPlain = PH_OFF;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pKeyData,
        bKeyDataLength,
        &pResponse,
        &wResponseLength));

    /* AV1: Check if MasterKey CMAC mode is enabled */
    if (bKeyNo == 0 &&(pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE))
    {
        if ((pDataParams->bAuthType) & 0x04)
        {
            /* encryption with secret key -> pKeyData is encrypted -> CMAC setting has to be checked by a GetKeyEntry command */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
                pDataParams,
                0x00,
                pKeyEntryBuffer,
                &bKeyEntryLength));

            if (pKeyEntryBuffer[bKeyEntryLength - 1] & 0x01)
            {
                pDataParams->bMasterKeyCmacMode = PH_ON;
            }
            else
            {
                pDataParams->bMasterKeyCmacMode = PH_OFF;
            }
        }
        else
        {
            /* pKeyData is plain */
            if (pKeyData[PHHAL_HW_SAMAV2_KEYENTRY_CONFIG_SET_POS + 1] & 0x01)
            {
                pDataParams->bMasterKeyCmacMode = PH_ON;
            }
            else
            {
                pDataParams->bMasterKeyCmacMode = PH_OFF;
            }
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_GetKeyEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t * pKeyEntry,
    uint8_t * bKeyEntryLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_GET_KEYENTRY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength));

    if (wResponseLength > 13)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
    }

    /* copy the data into the buffer */
    *bKeyEntryLength =(uint8_t)wResponseLength;
    memcpy(pKeyEntry, pResponse, *bKeyEntryLength); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKUCEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bKucNo,
    uint8_t bProMas,
    uint8_t * pKucData,
    uint8_t KucDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bKucNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bProMas;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = KucDataLength;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* AV1: Check if we need plain transmission */
    if ((pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE) &&((bOption & PHHAL_HW_SAMAV2_CMD_CHANGE_KUCENTRY_PLAIN) != 0))
    {
        pDataParams->bChangeEntryPlain = PH_ON;
    }
    else
    {
        /* Switch off plain transmission */
        pDataParams->bChangeEntryPlain = PH_OFF;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pKucData,
        KucDataLength,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_GetKUCEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKucNo,
    uint8_t * pKucEntry
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_GET_KUCENTRY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bKucNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 10)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /**pdwLimit =(uint32_t)((uint32_t)(pResponse[3]) << 24);
    *pdwLimit |=(uint32_t)((uint32_t)(pResponse[2]) << 16);
    *pdwLimit |=(uint32_t)((uint32_t)(pResponse[1]) << 8);
    *pdwLimit |=(uint32_t)((uint32_t)(pResponse[0]) << 0);
    *pKeyCKUC = pResponse[4];
    *pKeyVCKUC = pResponse[5];
    *pdwCurVal =(uint32_t)((uint32_t)(pResponse[9]) << 24);
    *pdwCurVal |=(uint32_t)((uint32_t)(pResponse[8]) << 16);
    *pdwCurVal |=(uint32_t)((uint32_t)(pResponse[7]) << 8);
    *pdwCurVal |=(uint32_t)((uint32_t)(pResponse[6]) << 0);*/

    memcpy(pKucEntry, pResponse, 10); /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_DumpSecretKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCrypto,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * DivInput,
    uint8_t bDivInputLength,
    uint8_t * pSecretKey,
    uint8_t * pSecretKeyLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    *pSecretKeyLength = 0;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DUMP_SECRETKEY_INS;
    if (bDivInputLength!=0)
    {
        bCrypto |= 0x02;
    }
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bCrypto;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(2 + bDivInputLength);
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2] = bKeyV;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2),
        NULL,
        NULL));

    if (bDivInputLength!=0)
    {
        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            DivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    aCmd[0] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aCmd,
        1,
        &pResponse,
        &wResponseLength));

    memcpy(pSecretKey,pResponse,wResponseLength); /* PRQA S 3200 */
    *pSecretKeyLength =(uint8_t)wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_DumpSessionKey(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    phhalHw_SamAV2_DataParams_t * pDataParamsEnc,
    uint8_t bCrypto,
    uint8_t * pSessionKey,
    uint8_t * pSessionKeyLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM bKeyLength;
    uint8_t     PH_MEMLOC_REM bDumpedKeyLength;
    uint8_t     PH_MEMLOC_REM bRndBLength;
    uint8_t     PH_MEMLOC_REM bBlockLength;
    uint8_t     PH_MEMLOC_REM bCrcLength;
    uint8_t     PH_MEMLOC_REM bPaddingLength;
    uint8_t     PH_MEMLOC_REM bLC;
    uint16_t    PH_MEMLOC_REM wCrc16;
    uint16_t    PH_MEMLOC_REM wBlockLength;
    uint16_t    PH_MEMLOC_REM wCounter;
    uint32_t    PH_MEMLOC_REM dwCrc32;

    *pSessionKeyLength = 0;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DUMP_SESSIONKEY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bCrypto;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength));

    if ((bCrypto & 0x01) &&(pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE))
    {
        /* the session key is encrypted with a session key of a second logical channel */
        if (pDataParamsEnc==NULL)
        {
            return PH_ADD_COMPCODE(PH_COMP_HAL, PH_ERR_INVALID_PARAMETER);
        }
        bLC =((uint8_t)(bCrypto >> 1)) & 0x03;


        if (pDataParamsEnc->bLogicalChannel != bLC)
        {
            return PH_ADD_COMPCODE(PH_COMP_HAL, PH_ERR_INVALID_PARAMETER);
        }

        /* Retrieve information about ENC key */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
            pDataParamsEnc->pENCCryptoDataParams,
            PH_CRYPTOSYM_CONFIG_KEY_TYPE,
            &wKeyType));

        /* Get block Length */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_GetConfig(
            pDataParamsEnc->pENCCryptoDataParams,
            PH_CRYPTOSYM_CONFIG_BLOCK_SIZE,
            &wBlockLength));

        /* Load default init vector */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
            pDataParamsEnc->pENCCryptoDataParams,
            gaFirstIv,
            (uint8_t)wBlockLength));

        /* Decrypt session key. */
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
            pDataParamsEnc->pENCCryptoDataParams, 
            (PH_CRYPTOSYM_CIPHER_MODE_CBC),
            pResponse, 
            wResponseLength, 
            pResponse));

        /* Retrieve key length parameters */
        PH_CHECK_SUCCESS_FCT(statusTmp,(phStatus_t) phhalHw_SamAV2_Hc_AV1_Int_GetKeyLengthParams(
            wKeyType,
            pDataParamsEnc->b2K3DESEncOption,
            &bKeyLength,
            &bRndBLength,
            &bBlockLength,
            &bCrcLength));

        /* try to get the real dumped key length */
        bPaddingLength = 0;
        wCounter = wResponseLength - 1;
        while((pResponse[wCounter] == 0x00) &&(bPaddingLength < bBlockLength))
        {
            wCounter --;
            bPaddingLength ++;
        }

        /* Set the bDumpedKeyLength */
        bDumpedKeyLength =(uint8_t)wResponseLength -(bPaddingLength + bCrcLength);

        /* validate CRC */
        if (pDataParamsEnc->bMasterKeyCmacMode)
        {
            memmove(pResponse+bDumpedKeyLength+2, pResponse+bDumpedKeyLength, bCrcLength); /* PRQA S 3200 */
            pResponse[bDumpedKeyLength]=0x90;
            pResponse[bDumpedKeyLength + 1]=0x00;

            /* CRC16 */
            if (bCrcLength == 2)
            {
                /* Crc Calculation */
                PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                    PH_TOOLS_CRC_OPTION_DEFAULT,
                    PH_TOOLS_CRC16_PRESET_ISO14443A,
                    PH_TOOLS_CRC16_POLY_ISO14443,
                    pResponse,
                    bDumpedKeyLength + 2,
                    &wCrc16));

                /* verify crc */
                if (pResponse[bDumpedKeyLength+2]!=(uint8_t)(wCrc16) || pResponse[bDumpedKeyLength+3]!=(uint8_t)(wCrc16 >> 8))
                {
                    return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
                }
            }
            /* CRC32 */
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
                    PH_TOOLS_CRC_OPTION_DEFAULT,
                    PH_TOOLS_CRC32_PRESET_DF8,
                    PH_TOOLS_CRC32_POLY_DF8,
                    pResponse,
                    bDumpedKeyLength + 2,
                    &dwCrc32));

                /* verify crc */
                if (pResponse[bDumpedKeyLength+2]!=(uint8_t)(dwCrc32) || pResponse[bDumpedKeyLength+3]!=(uint8_t)(dwCrc32 >> 8) ||
                    pResponse[bDumpedKeyLength+4]!=(uint8_t)(dwCrc32 >> 16) || pResponse[bDumpedKeyLength+5]!=(uint8_t)(dwCrc32 >> 24))
                {
                    return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
                }                
            }
        }
        else
        {
            /* CRC16 */
            if (bCrcLength == 2)
            {
                /* Crc Calculation */
                PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc16(
                    PH_TOOLS_CRC_OPTION_DEFAULT,
                    PH_TOOLS_CRC16_PRESET_ISO14443A,
                    PH_TOOLS_CRC16_POLY_ISO14443,
                    pResponse,
                    bDumpedKeyLength,
                    &wCrc16));

                /* verify crc */
                if (pResponse[bDumpedKeyLength]!=(uint8_t)(wCrc16) || pResponse[bDumpedKeyLength+1]!=(uint8_t)(wCrc16 >> 8))
                {
                    return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
                }
            }
            /* CRC32 */
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phTools_CalculateCrc32(
                    PH_TOOLS_CRC_OPTION_DEFAULT,
                    PH_TOOLS_CRC32_PRESET_DF8,
                    PH_TOOLS_CRC32_POLY_DF8,
                    pResponse,
                    bDumpedKeyLength,
                    &dwCrc32));

                /* verify crc */
                if (pResponse[bDumpedKeyLength]!=(uint8_t)(dwCrc32) || pResponse[bDumpedKeyLength+1]!=(uint8_t)(dwCrc32 >> 8) ||
                    pResponse[bDumpedKeyLength+2]!=(uint8_t)(dwCrc32 >> 16) || pResponse[bDumpedKeyLength+3]!=(uint8_t)(dwCrc32 >> 24))
                {
                    return PH_ADD_COMPCODE(PHHAL_HW_SAMAV2_ERR_CRYPTO, PH_COMP_HAL);
                }                
            }
        }
        memcpy(pSessionKey,pResponse,bDumpedKeyLength); /* PRQA S 3200 */
        *pSessionKeyLength =(uint8_t)bDumpedKeyLength;

    }
    else
    {
        memcpy(pSessionKey,pResponse,wResponseLength); /* PRQA S 3200 */
        *pSessionKeyLength =(uint8_t)wResponseLength;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_DisableKeyEntry(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint8_t bKeyNo,
    uint8_t * pOfflineCryptogram,
    uint8_t bCryptogramLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t PH_MEMLOC_REM wResponseLength;
    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DISABLE_KEYENTRY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bCryptogramLength;

    if (bCryptogramLength == 0)
    {
        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_DEFAULT,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH,
            &pResponse,
            &wResponseLength));
    }
    else
    {
        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pOfflineCryptogram,
            bCryptogramLength,
            &pResponse,
            &wResponseLength));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}
phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bAuthMode,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pEncRndB,
    uint8_t bEncRndBLength,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t * pEncRndAB,
    uint8_t * pEncRndABLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM pData[PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_MIN_DATA_LENGTH];    
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]  = bAuthMode;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]  = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]  =(uint8_t)(PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_MIN_DATA_LENGTH + bEncRndBLength + bDivInputLength);

    pData[0] = bKeyNo;
    pData[1] = bKeyV;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pData,
        PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_MIN_DATA_LENGTH,
        NULL,
        NULL));

    /* Exchange next part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pEncRndB,
        bEncRndBLength,
        NULL,
        NULL));

    if ((bAuthMode & 0x01) &&(bDivInputLength != 0))
    {
        /* send DivIn if present */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Exchange last part Le byte */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* status check */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        PH_CHECK_SUCCESS(status);
    }

    if ((wResponseLength != 16) &&(wResponseLength != 32))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    memcpy(pEncRndAB, pResponse, wResponseLength); /* PRQA S 3200 */
    *pEncRndABLength =(uint8_t)wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticatePICC_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pEncRndA,
    uint8_t bEncRndALength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]  = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]  = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]  = bEncRndALength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange next part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pEncRndA,
        bEncRndALength,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bAuthMode,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pRndCard,
    uint8_t bRndCardLength,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t * pEncRnd,
    uint8_t * pEncRndLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_MIN_DATA_LENGTH];    
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = bAuthMode;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    =(uint8_t)(PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_MIN_DATA_LENGTH + bRndCardLength + bDivInputLength);

    aData[0] = bKeyNo;
    aData[1] = bKeyV;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_MIN_DATA_LENGTH,
        NULL,
        NULL));

    /* Exchange next part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRndCard,
        bRndCardLength,
        NULL,
        NULL));

    if ((bAuthMode & 0x01) &&(bDivInputLength != 0))
    {
        /* send DivIn if present */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Exchange last part Le byte */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* status check */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        PH_CHECK_SUCCESS(status);
    }

    if ((wResponseLength != 24) &&(wResponseLength != 48))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    memcpy(pEncRnd, pResponse, wResponseLength); /* PRQA S 3200 */
    *pEncRndLength =(uint8_t)wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoAuthenticatePICC_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pRnd,
    uint8_t bRndLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_ISO_AUTHENTICATE_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = bRndLength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange next part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pRnd,
        bRndLength,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyPICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyCompMeth,
    uint8_t bCfg,
    uint8_t bKeyNoCurrent,
    uint8_t bKeyVCurrent,
    uint8_t bKeyNoNew,
    uint8_t bKeyVNew,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t * pEncStream,
    uint8_t * pEncStreamLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MIN_DATA_LENGTH];    
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = bKeyCompMeth;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = bCfg;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MIN_DATA_LENGTH + bDivInputLength;

    aData[0] =(uint8_t)bKeyNoCurrent;
    aData[1] =(uint8_t)bKeyVCurrent;
    aData[2] =(uint8_t)bKeyNoNew;
    aData[3] =(uint8_t)bKeyVNew;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MIN_DATA_LENGTH,
        NULL,
        NULL));

    if (bDivInputLength != 0)
    {
        /* send DivIn if present */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Exchange last part Le byte */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    if (wResponseLength > 32)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    memcpy(pEncStream, pResponse, wResponseLength); /* PRQA S 3200 */
    *pEncStreamLength =(uint8_t)wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

/*************************************************************************************************************************/
/****************************************MIFARE CLASSIC NON X Mode *******************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMifare_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint8_t bOption,
    uint8_t * pUid,
    uint8_t   bKeyNoM,
    uint8_t   bKeyVM,
    uint8_t   bKeyType,
    uint8_t   bBlockNo,
    uint8_t * pRndB,
    uint8_t   bDivBlockNo,
    uint8_t * pTokenAB
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MIFARE_DIVERSIFY)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_DATA_LENGTH_PART1;
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_DATA_LENGTH_PART1 - 1;
    }

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pUid,
        4,
        NULL,
        NULL));

    aCmd[0] = bKeyNoM;
    aCmd[1] = bKeyVM;

    if (bKeyType == PHHAL_HW_MFC_KEYA)
    {
        aCmd[2] = 0x0A;
    }
    else if (bKeyType == PHHAL_HW_MFC_KEYB)
    {
        aCmd[2] = 0x0B;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    aCmd[3] = bBlockNo;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aCmd,
        4,
        NULL,
        NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRndB,
        5,
        NULL,
        NULL));

    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MIFARE_DIVERSIFY)
    {
        aCmd[0] = bDivBlockNo;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            aCmd,
            1,
            NULL,
            NULL));
    }

    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    if (status !=(PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }

    if (wResponseLength != PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_RESPONSE_LENGTH)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Copy output data */
    memcpy(pTokenAB,pResponse,wResponseLength);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMifare_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,                           
    uint8_t * pTokenAB
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_MIFARE_DATA_LENGTH_PART2;

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(pDataParams, PH_EXCHANGE_BUFFER_FIRST, aCmd,5, NULL, NULL));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(pDataParams, PH_EXCHANGE_BUFFER_LAST,pTokenAB,5,&pResponse,&wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyMifare(
    phhalHw_SamAV2_DataParams_t * pDataParams,              
    uint8_t bOption,
    uint8_t bKeyNoM,
    uint8_t bKeyVM_KeyVMA,
    uint8_t bKeyAorB_KeyVMB,
    uint8_t * pAccessConditions,
    uint8_t * pUid,
    uint8_t bDivBlockNo,
    uint8_t * pKeyData,
    uint8_t * pKeyDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]    = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]    = PHHAL_HW_SAMAV2_CMD_SAM_CHANGE_KEY_MIFARE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = 0;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1]= bKeyNoM;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2]= bKeyVM_KeyVMA;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3]= bKeyAorB_KeyVMB;

    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_DIVERSIFY_KEY_A)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x02;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0x05; /* 4 bytes UID + 1 byte div block no */
    }
    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_DIVERSIFY_KEY_B)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x04;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0x05; /* 4 bytes UID + 1 byte div block no */
    }

    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_RETRIEVE_KEY)
    {
        /* key retrival */
        if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_CHANGEKEY_MIFARE_ENCRYPT)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] |= 0x01;
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] += 3;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3,
            NULL,
            NULL));
    }
    else
    {
        /* key change */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] += 7;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3,
            NULL,
            NULL));

        /* access conditions */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pAccessConditions,
            4,
            NULL,
            NULL));

    }

    if (aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS])
    {
        /* diversification needed */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pUid,
            4,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            &bDivBlockNo,
            1,
            NULL,
            NULL));
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    if (wResponseLength > 21)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Copy output data */
    memcpy(pKeyData, pResponse, wResponseLength);  /* PRQA S 3200 */
    *pKeyDataLength =(uint8_t) wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

/*************************************************************************************************************************/
/****************************************MIFARE PLUS NON X Mode **********************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMfp_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pRndB,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t * pRndARndB,
    uint8_t * pRndARndBLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bCardResponseLength = 16;

    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_MF_ENCIPHERED)
    {
        bCardResponseLength = 22;
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_MFP;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 2 /* Key NR, Key V */ + bCardResponseLength + bDivInputLength;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = bKeyV;

    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }  

    if (!(bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_AUTH_FIRST))
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x02;
    }

    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL2_KDF)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x04;
    }

    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_SL3_KDF)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x0C;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2),
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRndB,
        bCardResponseLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bDivInputLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* We expect chaining active error */
    if (status !=(PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }

    /* Check the length */
    if (bOption & PHHAL_HW_SAMAV2_CMD_SAM_AUTHENTICATE_MFP_MF_ENCIPHERED)
    {
        if (wResponseLength == 40)
        {
            memcpy(pRndARndB,pResponse,wResponseLength); /* PRQA S 3200 */
            *pRndARndBLength = (uint8_t) wResponseLength;
        }else
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }else
    {
        if (wResponseLength == 32)
        {
            memcpy(pRndARndB,pResponse,wResponseLength); /* PRQA S 3200 */
            *pRndARndBLength = (uint8_t) wResponseLength;
        }else
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateMfp_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pEncData,
    uint8_t bEncDataLength,
    uint8_t * pPdCap,
    uint8_t * pPcdCap                               
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_MFP;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bEncDataLength;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pEncData,
        bEncDataLength,
        NULL,
        NULL));


    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));


    if ((wResponseLength == 12) &&(bEncDataLength == 32))
    {
        memcpy(pPdCap,pResponse,6); /* PRQA S 3200 */
        memcpy(pPcdCap,&pResponse[6],6); /* PRQA S 3200 */
    }
    else
    {
        if (wResponseLength != 0x00)
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_CombinedReadMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,                               
    uint16_t wOption,
    uint8_t * pData, 
    uint8_t bDataLength,                                
    uint8_t ** ppOutput, 
    uint16_t * pOutputLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    *pOutputLength = 0;

    /* Is this the first chunk of data? */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_READ_MFP;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;

        if (wOption & PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_RESPONSE)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
        }
        else
        {
            if (wOption & PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_READ_MFP_COMMAND_RESPONSE)
            {
                aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x02;
            }
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* In case of manual chaining, we need to update the additional Frame info byte */
    if (wOption & PH_EXCHANGE_TXCHAINING)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateP2(pDataParams, PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME));
    }

    /* Provide data of current exchange */

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pData,
        bDataLength,
        NULL,
        NULL));

    /* Update the LC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc( pDataParams ));

    /* Was this the last frame? */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        ppOutput,
        &wResponseLength);

    if (status !=(PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        status = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    /* Check the length */
    pResponse = *ppOutput;
    *pOutputLength = wResponseLength;
    return status;
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_CombinedWriteMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pData,
    uint8_t bDataLength,
    uint8_t ** ppOutput, 
    uint16_t * pOutputLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    *pOutputLength = 0;

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_WRITE_MFP;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;

        if (wOption & PHHAL_HW_SAMAV2_CMD_SAM_COMBINED_WRITE_MFP_RESPONSE)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pData,
        bDataLength,
        NULL,
        NULL));

    /* Was this the last frame? */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Update the LC */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc( pDataParams ));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        ppOutput,
        &wResponseLength));

    pResponse = *ppOutput;
    /* Check the length */
    *pOutputLength = wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t bKeyV, 
    uint8_t bPpse, 
    uint8_t * pRndRC,
    uint8_t * pDivInput, 
    uint8_t  bDivInputLength,
    uint8_t * pMac
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_VERIFY_PROXIMITY_CHECK;

    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0x01;
    }else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    }
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDivInputLength + 14 /* RNDRC */ + 3 /*standard parameters*/;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = bKeyV;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 3] = bPpse;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3),
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRndRC,
        14,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bDivInputLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);


    if (status !=(PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }

    if (wResponseLength != 8)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }
    memcpy(pMac, pResponse, 8); /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);

}

phStatus_t phhalHw_SamAV2_Cmd_SAM_ProximityCheckMfp_Part2(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pMac
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t PH_MEMLOC_REM wResponseLength;
    uint8_t * pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_VERIFY_PROXIMITY_CHECK;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 8 /* MAC */;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        5,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pMac,
        8,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_VirtualCardSupportMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t* pData,
    uint8_t bDataLength,
    uint8_t * pPcdCapabilities,
    uint8_t  bPcdCapabilitiesLength,
    uint8_t ** pSupportedCardInfo,
    uint16_t * pSupportedCardInfoLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Is this the first chunk of data? */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_VIRTUAL_CARD_SUPPORT;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pData,
        bDataLength,
        NULL,
        NULL));

    /* Was this the last frame? */
    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Add Length of PCD Caps*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            &bPcdCapabilitiesLength,
            1,
            NULL,
            NULL));

        /* Add PCD Caps*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pPcdCapabilities,
            bPcdCapabilitiesLength,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength));

        *pSupportedCardInfoLength = wResponseLength;
        *pSupportedCardInfo = pResponse;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);


}

phStatus_t phhalHw_SamAV2_Cmd_SAM_SelectVirtualCardMfp(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pRndQ,
    uint8_t * pDivInput,
    uint8_t  bDivInputLength,
    uint8_t * pMac 
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_SELECT_VIRTUAL_CARD;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDivInputLength + 2 /* Key number and version */ + 12 /* RNDQ */;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = bKeyV;

    /* if divinput is available set bit 0 in P1 */
    if (bDivInputLength != 0)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;  
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2),
        NULL,
        NULL));   

    /* Add rndQ */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRndQ,
        12,
        NULL,
        NULL));

    /* Add diff input */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bDivInputLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x08 /* MAC length */)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }
    memcpy(pMac, pResponse, 8);  /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyMfp_Part1(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCmd,
    uint16_t wBlockNo,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t ** ppOutput,
    uint16_t * pOutputLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_CMD_MIN_DATA_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_CMD_MIN_DATA_LENGTH + bDivInputLength);

    if (bDivInputLength != 0)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x02;
    }

    aData[0] = bCmd;
    aData[1] =(uint8_t)(wBlockNo & 0x00FF);
    aData[2] =(uint8_t)(wBlockNo >> 8);
    aData[3] = bKeyNo;
    aData[4] = bKeyV;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_CMD_MIN_DATA_LENGTH,
        NULL,
        NULL));

    if (bDivInputLength != 0)
    {
        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    *ppOutput = pResponse;
    *pOutputLength = wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_ChangeKeyMfp_Part2 (
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pDataIn,
    uint8_t bDataInLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_CHANGE_KEY_MFP_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0x01;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataInLength;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDataIn,
        bDataInLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_GenerateKeyPair(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bPKI_KeyNo,
    uint16_t wPKI_Set,  
    uint8_t bPKI_KeyNoCEK,
    uint8_t bPKI_KeyVCEK,
    uint8_t bPKI_RefNoKUC,
    uint16_t wPKI_NLen,
    uint16_t wPKI_eLen,
    uint8_t * pPKI_e
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM pPayload[PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_LC_MIN];

    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_INS;

    if (bOption & PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_FIXED_E)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0x01;

        /* Max frame size full protection = 240; reseve 10 bytes for PKI header */
        if (wPKI_eLen >= 230)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 240;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(10 + wPKI_eLen);
        }
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 10;
    }

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Build the command payload*/
    pPayload[0] = bPKI_KeyNo;
    pPayload[1] =(uint8_t)((wPKI_Set & 0xFF00) >> 8) ;
    pPayload[2] =(uint8_t)((wPKI_Set & 0x00FF) >> 0) ;
    pPayload[3] = bPKI_KeyNoCEK;
    pPayload[4] = bPKI_KeyVCEK;
    pPayload[5] = bPKI_RefNoKUC;
    pPayload[6] =(uint8_t)((wPKI_NLen & 0xFF00) >> 8) ;
    pPayload[7] =(uint8_t)((wPKI_NLen & 0x00FF) >> 0) ;
    pPayload[8] =(uint8_t)((wPKI_eLen & 0xFF00) >> 8) ;
    pPayload[9] =(uint8_t)((wPKI_eLen & 0x00FF) >> 0) ;

    /* Exchange command payload with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pPayload,
        (uint8_t)PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_LC_MIN,
        NULL,
        NULL));

    if (aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] == PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME)
    {
        /* safety check */
        if (wPKI_eLen < 230)
        {
            return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
        }

        /* chaining needed */

        /* Exchange first frame with lower layer */
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pPKI_e,
            230, /* LE is not present in that command*/
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            return statusTmp;
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(wPKI_eLen - 230);

        /* Exchange second header with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pPKI_e + 230,
            (uint16_t)(wPKI_eLen - 230),
            &pResponse,
            &wResponseLength));
    }
    else
    {
        if (bOption & PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_KEY_PAIR_FIXED_E)
        {
            /* PKI_e has to be send */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                pPKI_e,
                wPKI_eLen,
                &pResponse,
                &wResponseLength));
        }
        else
        {
            /* No PKI_e has to be send -> transmit the frame */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pResponse,
                &wResponseLength));
        }
    }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_PKI_ExportPrivateKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bPKI_KeyNo,
    uint8_t ** ppRxBuffer,
    uint16_t *pRxLength
    )

{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_EXPORT_PRIVATE_KEY_INS;

    if (wOption & PH_EXCHANGE_RXCHAINING)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bPKI_KeyNo;
    }

    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = 0x00;

    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength);

    /* status check */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        status = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    *ppRxBuffer = pResponse;
    *pRxLength = wResponseLength;

    return status;
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_ExportPublicKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bPKI_KeyNo,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_EXPORT_PUBLIC_KEY_INS;

    if (wOption & PH_EXCHANGE_RXCHAINING)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bPKI_KeyNo;
    }

    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = 0x00;

    /* Exchange first part with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength);

    /* status check */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        status = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    *ppRxBuffer = pResponse;
    *pRxLength = wResponseLength;

    return status;
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_SendSignature(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_SEND_SIGNATURE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = PHHAL_HW_SAMAV2_CMD_PKI_SEND_SIGNATURE_LE_VALUE;

    /* Exchange first part with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength);

    /* status check */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        status = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    *ppRxBuffer = pResponse;
    *pRxLength = wResponseLength;

    return status;
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_UpdateKeyEntries(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bNoOfKeyEntries,
    uint8_t bHashingAlg,
    uint8_t bPKI_KeyNo_Enc,
    uint8_t bPKI_KeyNo_Sign,
    uint8_t * pKeyFrame,
    uint16_t wKeyFrameLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint16_t    PH_MEMLOC_REM wBufPos;
    uint8_t     PH_MEMLOC_REM bTxLength;
    uint8_t     PH_MEMLOC_REM bSendHeader;
    uint8_t     PH_MEMLOC_REM aPayload[PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_LC_MIN];

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bHashingAlg |(uint8_t)(bNoOfKeyEntries << 2);
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    aPayload[0] = bPKI_KeyNo_Enc;
    aPayload[1] = bPKI_KeyNo_Sign;

    /* Exchange payload with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aPayload,
        PHHAL_HW_SAMAV2_CMD_PKI_UPDATE_KEY_ENTRIES_LC_MIN,
        NULL,
        NULL));

    bTxLength = 2;
    wBufPos = 0;
    bSendHeader = PH_OFF;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK;

    do {

        if (bSendHeader==PH_ON)
        {
            /* Exchange first part with lower layer */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_FIRST,
                aCmd,
                PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
                NULL,
                NULL));

            bSendHeader = PH_OFF;
        }

        if ((wKeyFrameLength - wBufPos) >(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - bTxLength))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateP2(pDataParams, PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME));

            /* Exchange data */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                pKeyFrame + wBufPos,
                (uint16_t)(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - bTxLength),
                &pResponse,
                &wResponseLength));

            /* last frame -> update Lc */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

            /* Exchange last frame. */
            statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pResponse,
                &wResponseLength);

            /* status check */
            if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
            {
                return statusTmp;
            }
            bSendHeader = PH_ON;
            wBufPos = wBufPos +(uint16_t)(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - bTxLength);
            bTxLength = 0;
        }
        else
        {
            /* Exchange last frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                pKeyFrame + wBufPos,
                (uint16_t)(wKeyFrameLength - wBufPos),
                NULL,
                NULL));

            /* last frame -> update Lc */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

            /* Exchange last frame. */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pResponse,
                &wResponseLength));

            wBufPos = wKeyFrameLength;
        }

    } while(wBufPos < wKeyFrameLength);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_GenerateSignature(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bHashingAlg,
    uint8_t bPKI_KeyNo,
    uint8_t * pHashMsgData,
    uint8_t    bHashMsgLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 1];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_SIGNATURE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bHashingAlg;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(1 + bHashMsgLength);
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1] = bPKI_KeyNo;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 1),
        NULL,
        NULL));

    /* Exchange last part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pHashMsgData,
        bHashMsgLength,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_GenerateHash(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bHashingAlg,
    uint32_t dwMsgLength,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t * pHash,
    uint8_t * pHashLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aPayload[4];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_INS;


        if (wOption & PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_OPTION_FIRST_FRAME)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bHashingAlg;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        }

        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        if (wOption & PHHAL_HW_SAMAV2_CMD_PKI_GENERATE_HASH_OPTION_FIRST_FRAME)
        {
            aPayload[0] =(uint8_t)(dwMsgLength >> 24);
            aPayload[1] =(uint8_t)(dwMsgLength >> 16);
            aPayload[2] =(uint8_t)(dwMsgLength >> 8);
            aPayload[3] =(uint8_t)(dwMsgLength >> 0);

            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_CONT,
                aPayload,
                4,
                NULL,
                NULL));
        }
    }

    /* Exchange data with lower layer */
    if (wTxLength > 0)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pTxBuffer,
            wTxLength,
            NULL,
            NULL));
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pResponse,
                &wResponseLength);

            /* status check */
            if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
            {
                return statusTmp;
            }

            statusTmp = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                gaDefaultLe,
                1,
                &pResponse,
                &wResponseLength));

            if (wResponseLength > 32)
            {
                return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
            }

            memcpy(pHash, pResponse, wResponseLength);  /* PRQA S 3200 */
            *pHashLength =(uint8_t) wResponseLength;
        }
    }

    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_VerifySignature(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bPKI_KeyNo,
    uint8_t bHashingAlg,
    uint8_t * pHashData,
    uint8_t    bHashDataLength,
    uint8_t * pSignature,
    uint16_t wSignatureLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bBufPos;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PKI_VERIFY_SIGNATURE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bHashingAlg;

    if ((1 + wSignatureLength + bHashDataLength) <= PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK)
    {
        /* single frame */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(1 + bHashDataLength + wSignatureLength);
    }
    else
    {
        /* two frames */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK;
    }

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange key no with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        &bPKI_KeyNo,
        1,
        NULL,
        NULL));

    /* Exchange hash with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pHashData,
        bHashDataLength,
        NULL,
        NULL));

    if (aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] == PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE)
    {
        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pSignature,
            wSignatureLength,
            &pResponse,
            &wResponseLength);
    }
    else
    {
        bBufPos =(uint8_t)(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - 1 - bHashDataLength);

        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pSignature,
            bBufPos,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            return statusTmp;
        }

        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(wSignatureLength - bBufPos);

        /* Exchange second part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            pSignature + bBufPos,
            (uint8_t)(wSignatureLength - bBufPos),
            &pResponse,
            &wResponseLength);
    }

    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_PKI_ImportKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bPKI_KeyNo,
    uint16_t wPKI_Set,
    uint8_t bPKI_KeyNoCEK,
    uint8_t bPKI_KeyVCEK,
    uint8_t bPKI_RefNoKUC,
    uint16_t wPKI_NLen,    
    uint16_t wPKI_eLen,    
    uint16_t wPKI_PLen,    
    uint16_t wPKI_QLen,    
    uint8_t * pPKI_N,    
    uint8_t * pPKI_e,    
    uint8_t * pPKI_P,    
    uint8_t * pPKI_Q,    
    uint8_t * pPKI_dP,    
    uint8_t * pPKI_dQ,
    uint8_t * pPKI_iPQ
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aPayloadHeader[PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_LC_MIN];
    uint8_t *   PH_MEMLOC_REM pPayload;
    uint16_t    PH_MEMLOC_REM wPayLoadLen;
    uint16_t    PH_MEMLOC_REM wPayLoadSize;
    uint16_t    PH_MEMLOC_REM wTxLength;
    uint16_t    PH_MEMLOC_REM wRemainingMessageLength;
    uint8_t     PH_MEMLOC_REM bPrivateKeyIncluded;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bState;

    /* Parameter check #1 */
    /* This is to ensure that wRemainingMessageLength variable does not overflow */
    if ((wPKI_NLen > 256) ||(wPKI_eLen > 256))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Parameter check #2 */
    /* This is to ensure that wRemainingMessageLength variable does not overflow */
    if (((wPKI_Set & 0x01) && !(bOption & PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_SETTINGS_ONLY)) &&
        ((wPKI_PLen > 252) ||(wPKI_QLen > 252)))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Build initial frame header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = 0x00;
    wRemainingMessageLength                 = PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH;

    /* Build payload header(settings only). */
    aPayloadHeader[0] = bPKI_KeyNo;
    aPayloadHeader[1] =(uint8_t)((wPKI_Set & 0xFF00) >> 8) ;
    aPayloadHeader[2] =(uint8_t)(wPKI_Set & 0x00FF);
    aPayloadHeader[3] = bPKI_KeyNoCEK;
    aPayloadHeader[4] = bPKI_KeyVCEK;
    aPayloadHeader[5] = bPKI_RefNoKUC;
    wTxLength = 6;

    if (bOption & PHHAL_HW_SAMAV2_CMD_PKI_IMPORT_KEY_SETTINGS_ONLY)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)wTxLength;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            aPayloadHeader,
            wTxLength,
            &pResponse,
            &wResponseLength));

        /* only settings have to be sent -> we are done */
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Check if private key is included */
    if (wPKI_Set & 0x01)
    {
        bPrivateKeyIncluded = 1;
    }
    else
    {
        bPrivateKeyIncluded = 0;
    }

    /* Add additional lengths to payload */
    aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_NLen & 0xFF00) >> 8);
    aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_NLen & 0x00FF) >> 0);
    aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_eLen & 0xFF00) >> 8);
    aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_eLen & 0x00FF) >> 0);
    wRemainingMessageLength = wRemainingMessageLength + wPKI_NLen + wPKI_eLen;
    if (bPrivateKeyIncluded)
    {
        aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_PLen & 0xFF00) >> 8);
        aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_PLen & 0x00FF) >> 0);
        aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_QLen & 0xFF00) >> 8);
        aPayloadHeader[wTxLength++] =(uint8_t)((wPKI_QLen & 0x00FF) >> 0);
        wRemainingMessageLength = wRemainingMessageLength +(wPKI_PLen * 2) +(wPKI_QLen * 3);
    }
    wRemainingMessageLength = wRemainingMessageLength + wTxLength;

    /* Check whether we have to indicate chaining or not */
    if (wRemainingMessageLength >(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - 16))
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
    }

    /* Buffer the command header */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Append payload to message */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aPayloadHeader,
        wTxLength,
        NULL,
        NULL));

    /* wTxLength becomes the overall package length */
    wTxLength = PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + wTxLength;

    /* Chaining Loop */
    bState = 0;
    wPayLoadLen = 0;
    wPayLoadSize = 0;
    do
    {
        /* Distinguish between the different payloads */
        switch (bState)
        {
        case 0:
            pPayload = &pPKI_N[wPayLoadLen];
            wPayLoadSize = wPKI_NLen - wPayLoadLen;
            break;
        case 1:
            pPayload = &pPKI_e[wPayLoadLen];
            wPayLoadSize = wPKI_eLen - wPayLoadLen;
            break;
        case 2:
            pPayload = &pPKI_P[wPayLoadLen];
            wPayLoadSize = wPKI_PLen - wPayLoadLen;
            break;
        case 3:
            pPayload = &pPKI_Q[wPayLoadLen];
            wPayLoadSize = wPKI_QLen - wPayLoadLen;
            break;
        case 4:
            pPayload = &pPKI_dP[wPayLoadLen];
            wPayLoadSize = wPKI_PLen - wPayLoadLen;
            break;
        case 5:
            pPayload = &pPKI_dQ[wPayLoadLen];
            wPayLoadSize = wPKI_QLen - wPayLoadLen;
            break;
        case 6:
            pPayload = &pPKI_iPQ[wPayLoadLen];
            wPayLoadSize = wPKI_QLen - wPayLoadLen;
            break;
        default:
            pPayload = NULL;
            wPayLoadSize = 0;
            break;
        }

        /* Append payload to the buffer */
        if (wPayLoadSize > 0)
        {
            /* Payload fits into current frame without truncation */
            if ((wTxLength + wPayLoadSize) <(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - 16))
            {
                /* Last frame -> send it */
                if ((wRemainingMessageLength -(wTxLength + wPayLoadSize)) == 0)
                {
                    /* Append rest of data */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                        pDataParams,
                        PH_EXCHANGE_BUFFER_CONT,
                        pPayload,
                        wPayLoadSize,
                        NULL,
                        NULL));

                    /* Update LC */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

                    /* perform exchange */
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                        pDataParams,
                        PH_EXCHANGE_BUFFER_LAST,
                        NULL,
                        0,
                        &pResponse,
                        &wResponseLength));
                }
                /* Just buffer the frame */
                else
                {
                    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                        pDataParams,
                        PH_EXCHANGE_BUFFER_CONT,
                        pPayload,
                        wPayLoadSize,
                        NULL,
                        NULL));

                    /* Update current frame length */
                    wTxLength = wTxLength + wPayLoadSize;
                }

                /* Reset payload length */
                wPayLoadLen = 0;

                /* Advance to next payload */
                ++bState;
            }
            /* Else send maximum amount of possible data to SAM */
            else
            {
                wPayLoadSize =(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - 16) - wTxLength;
                wPayLoadLen = wPayLoadLen + wPayLoadSize;

                /* Append rest of data */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                    pDataParams,
                    PH_EXCHANGE_BUFFER_CONT,
                    pPayload,
                    wPayLoadSize,
                    NULL,
                    NULL));

                /* Update LC */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

                /* perform exchange */
                status = phhalHw_SamAV2_Cmd_7816Exchange(
                    pDataParams,
                    PH_EXCHANGE_BUFFER_LAST,
                    NULL,
                    0,
                    &pResponse,
                    &wResponseLength);

                /* status check */
                if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
                {
                    return status;
                }

                /* Update current frame length */
                wTxLength = wTxLength + wPayLoadSize;

                /* Check for internal error */
                if (wRemainingMessageLength < wTxLength)
                {
                    return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
                }

                /* Update remaining message length */
                wRemainingMessageLength = wRemainingMessageLength - wTxLength;

                /* Check whether we have to indicate chaining or not */
                if (wRemainingMessageLength >(PHHAL_HW_SAMAV2_ISO7816_MAX_LC_MULTIPLE_AESBLOCK - 16))
                {
                    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
                }
                else
                {
                    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
                }

                /* Buffer the command header */
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
                    pDataParams,
                    PH_EXCHANGE_BUFFER_FIRST,
                    aCmd,
                    PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
                    NULL,
                    NULL));

                /* Reset current frame length */
                wTxLength = 0;
            }
        }
    }
    while(wPayLoadSize > 0);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_DisableCrypto(
    phhalHw_SamAV2_DataParams_t * pDataParams,    
    uint16_t wProMas
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_MAX_DATA_LENGTH];
    uint16_t    PH_MEMLOC_REM wCmdLength = PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_MIN_DATA_LENGTH;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;


    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_DISABLE_CRYPTO;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = 2;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1]  =(uint8_t) wProMas ;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2]  =(uint8_t)(wProMas >> 8);


    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        wCmdLength,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_LockUnlock(
    phhalHw_SamAV2_DataParams_t * pDataParams,      
    uint8_t bMode,     
    uint16_t wRdKeyNo,
    uint16_t wRdKeyV,
    uint8_t bSamKeyNo,                            
    uint8_t bSamKeyV,                           
    uint8_t bUnlockKeyNo,                      
    uint8_t bUnlockKeyV,                     
    uint32_t bMaxChainBlocks
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[13];
    uint8_t     PH_MEMLOC_REM aRnd1[16];
    uint8_t     PH_MEMLOC_REM aRnd2[16];
    uint8_t     PH_MEMLOC_REM aKey[24];
    uint8_t     PH_MEMLOC_REM aMac[16];
    uint16_t    PH_MEMLOC_REM wKeyType;
    uint8_t     PH_MEMLOC_REM aKxeKey[24];
    uint8_t     PH_MEMLOC_REM aRndAB[32];
    uint8_t     PH_MEMLOC_REM bKeyLength;
    uint8_t     PH_MEMLOC_REM bMacLength = 0;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bLcLength = 2;

    memset(aCmd, 0, sizeof(aCmd));  /* PRQA S 3200 */

    /* Get Key out of the key store object */
    PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
        pDataParams->pKeyStoreDataParams,
        wRdKeyNo,
        wRdKeyV,
        sizeof(aKey),
        aKey,
        &wKeyType));

    if ((wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES128) &&(wKeyType != PH_CRYPTOSYM_KEY_TYPE_AES192))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_INS;    /* INS */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bMode;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1] = bSamKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2] = bSamKeyV;

    switch (bMode)
    {
    case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_UNLOCK:
    case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_LOCK_WO_SPECIFYING_UNLOCK_KEY:
        /* bLc is already ok*/
        break;
    case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_LOCK_SPECIFYING_UNLOCK_KEY:
        bLcLength = bLcLength + 2;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3] = bUnlockKeyNo;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+4] = bUnlockKeyV;
        break;
    case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_P1_AV2_MODE_SWITCH:
        bLcLength = bLcLength + 3;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3] =(uint8_t)(bMaxChainBlocks);
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+4] =(uint8_t)(bMaxChainBlocks >> 8);
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+5] =(uint8_t)(bMaxChainBlocks >> 16);
        break;
    default:
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bLcLength;    

    /* Exchange first part of the command */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        bLcLength + PHHAL_HW_SAMAV2_ISO7816_HEADER_LE_LENGTH,
        &pResponse,
        &wResponseLength);

    /* Reset P1 */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0;

    /* We expect chaining active error */
    if (status !=(PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }
    if (wResponseLength != PHHAL_HW_SAMAV2_HOST_AUTH_RESP1_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }
    memcpy(aRnd2, pResponse, PHHAL_HW_SAMAV2_HOST_AUTH_RESP1_LENGTH);  /* PRQA S 3200 */

    /* now we start with processing of HC Part 2 */

    /* to prepare the 2nd part, we have to MAC: Rnd2 | P1 | other data, use temporarly aRnd2 */
    aRnd2[12] = bMode;
    memset(&aRnd2[13], 0, 3);  /* PRQA S 3200 */
    memcpy(&aRnd2[13], &aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3], bLcLength - 2);  /* PRQA S 3200 */

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pMACCryptoDataParams,
        aKey,
        wKeyType));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pMACCryptoDataParams, 
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aRnd2, 
        16, 
        aMac, 
        &bMacLength));

    /* we have to truncate the MAC*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_TruncateMacBuffer(aMac, &bMacLength));

    /* Initialize the aRnd1 array*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, 12, aRnd1));

    /* prepare the 2nd part of Authenticate*/
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = /* MAC */ bMacLength + /* RND1 */ 12;    /*LC*/

    /* Exchange first part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        5,
        NULL,
        NULL));

    /* Exchange MAC part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aMac,
        bMacLength,
        NULL,
        NULL));

    /* Exchange RND1 part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aRnd1,
        12,
        NULL,
        NULL));

    /* Exchange LE part of the command */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* We expect chaining active error */
    if (status !=(PH_COMP_HAL | PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE))
    {
        return status;
    }

    if (wResponseLength != PHHAL_HW_SAMAV2_HOST_AUTH_RESP2_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* to prepare the 2nd part, we have to MAC: Rnd1 | P1 | other data, use temporarly aRnd2 */
    aRnd1[12] = bMode;
    memset(&aRnd1[13], 0, 3);  /* PRQA S 3200 */
    memcpy(&aRnd1[13], &aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3], bLcLength - 2);  /* PRQA S 3200 */

    /* Key already loaded */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_CalculateMac(
        pDataParams->pMACCryptoDataParams, 
        PH_CRYPTOSYM_MAC_MODE_CMAC,
        aRnd1, 
        16, 
        aMac, 
        &bMacLength));

    /* we have to truncate the MAC*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_HcUtils_TruncateMacBuffer(aMac, &bMacLength));
    /* check they are equal*/
    if (memcmp(aMac, pResponse, bMacLength) ) 
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_HAL);
    }

    /* Derive the Kxe key from kx using Rnd1 and Rnd2 - note: Secret key needs to be loaded into MAC data params*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_GenerateAuthEncKey(
        pDataParams,
        (uint8_t)wKeyType, aRnd1, aRnd2, aKxeKey, &bKeyLength)); 

    /* From now on, we can use aRnd1 and aRnd2 to store RNDA(Rnd1) and RNDB(Rnd2) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadKeyDirect(
        pDataParams->pENCCryptoDataParams,
        aKxeKey,
        wKeyType));

    /* Load default init vector */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        gaFirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams, 
        (PH_CRYPTOSYM_CIPHER_MODE_CBC),
        &pResponse[8], 
        16, 
        aRnd2));

    /* Now we start with part 3 exchange */
    /* Initialize the RndA array*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoRng_Rnd(pDataParams->pCryptoRngDataParams, 16, aRnd1));
    memcpy(aRndAB, aRnd1, 16);  /* PRQA S 3200 */
    /* calculate RndB'*/
    memcpy(&aRndAB[16], &aRnd2[2], 14);  /* PRQA S 3200 */
    aRndAB[30] = aRnd2[0];
    aRndAB[31] = aRnd2[1];

    /* Load default init vector */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        gaFirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* Key is already loaded */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Encrypt(
        pDataParams->pENCCryptoDataParams, 
        PH_CRYPTOSYM_CIPHER_MODE_CBC,  
        aRndAB, 
        32, 
        aRndAB));

    /* prepare the buffer*/
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 32;    /*LC*/

    /* Exchange first part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        5,
        NULL,
        NULL));

    /* Exchange RNDAB part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aRndAB,
        32,
        NULL,
        NULL));

    /* Exchange LE part of the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != PHHAL_HW_SAMAV2_HOST_AUTH_RESP3_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Load default init vector */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_LoadIv(
        pDataParams->pENCCryptoDataParams,
        gaFirstIv,
        PH_CRYPTOSYM_AES_BLOCK_SIZE));

    /* Key is already loaded */
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_Decrypt(
        pDataParams->pENCCryptoDataParams,
        PH_CRYPTOSYM_CIPHER_MODE_CBC,
        pResponse,
        16,
        aRndAB));

    if (memcmp(aRndAB, &aRnd1[2], 14) != 0)
    {
        /* the response for RndA is not equal to sent RndA, Authentication failed PH_ERR_AUTH_ERROR*/
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
    }

    if ((aRnd1[0] != aRndAB[14])||(aRnd1[1] != aRndAB[15]))
    {
        /* the response for RndA is not equal to sent RndA, Authentication failed PH_ERR_AUTH_ERROR*/
        return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_HAL);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey( pDataParams->pENCCryptoDataParams));
    PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey( pDataParams->pMACCryptoDataParams));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_AuthenticateHost(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bAuthType_HostMode,
    uint16_t wRdKeyNo,
    uint16_t wRdKeyV,
    uint8_t bSamKeyNo,
    uint8_t bSamKeyV,
    uint8_t * pDivInput,
    uint8_t bDivInputLength
    )
{
    phStatus_t PH_MEMLOC_REM statusTmp;

    if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_AuthenticateHost(
            pDataParams,
            bAuthType_HostMode,    
            wRdKeyNo,
            wRdKeyV,
            bSamKeyNo,
            bSamKeyV,
            pDivInput,
            bDivInputLength));
    }
    else if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE)
    {
        if (bDivInputLength != 0x00)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_AuthenticateHost(
            pDataParams,
            bAuthType_HostMode,
            wRdKeyNo,
            wRdKeyV,
            bSamKeyNo,
            bSamKeyV));
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_ActivateOfflineKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pDivInput,
    uint8_t bDivInputLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_ACTIVATE_OFFLINE_KEY_MIN_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]    = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]    = PHHAL_HW_SAMAV2_CMD_ACTIVATE_OFFLINE_KEY_INS;

    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = 0x01;    
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    }

    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = bDivInputLength + PHHAL_HW_SAMAV2_CMD_ACTIVATE_OFFLINE_KEY_MIN_DATA_LENGTH;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1]= bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2]= bKeyV;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_ACTIVATE_OFFLINE_KEY_MIN_DATA_LENGTH),
        NULL,
        NULL));

    /* Exchange div data with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pDivInput,
        bDivInputLength, /* LE is not present in that command*/
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_SelectApplication(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pDF_Aid
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_SELECT_APPLICATION_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = PHHAL_HW_SAMAV2_CMD_SELECT_APPLICATION_DATA_LENGTH;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange final part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pDF_Aid,
        3, /* LE is not present in that command*/
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_LoadInitVector(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pInitVector,
    uint8_t bInitVectorLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]    = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]    = PHHAL_HW_SAMAV2_CMD_LOAD_INIT_VECTOR_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    if ((bInitVectorLength == PHHAL_HW_SAMAV2_CMD_LOAD_INIT_VECTOR_DATA_LENGTH_MIN) ||
        (bInitVectorLength == PHHAL_HW_SAMAV2_CMD_LOAD_INIT_VECTOR_DATA_LENGTH_MAX))
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = bInitVectorLength;
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange final part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pInitVector,
        bInitVectorLength, /* LE is not present in that command*/
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_KillAuthentication(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bCmdLength;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM ResponseLength;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_KILL_AUTHENTICATION_INS;    /* INS */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0x00;        /* P1*/
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = 0x00;        /*P2*/
    bCmdLength = PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH;

    if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bOption;
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        bCmdLength,
        &pResponse,
        &ResponseLength));

    /* Reset Crypto in case of success and P1 == 0x00 */
    /* Now, in case of Kill Authenticate, lets reset secure messaging. */
    if ((bOption & PHHAL_HW_SAMAV2_CMD_CHANGE_SAMKILLAUTHENTICATION_PICC_ONLY) == 0)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey(pDataParams->pENCCryptoDataParams));

        if ((pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE) ||(pDataParams->bAuthType & 0x02))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey(pDataParams->pMACCryptoDataParams));
        }

        pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
        pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
        pDataParams->bAuthType = 0x00;
    }
    else
    {
        pDataParams->bMifareCryptoDisabled = PH_ON;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_GetChallenge(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t    bExpL,
    uint8_t  * pRnd
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]       = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]       = PHHAL_HW_SAMAV2_CMD_GET_CHALLENGE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]        = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]        = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS]  = bExpL;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT, 
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != bExpL)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    memcpy(pRnd, pResponse, bExpL);  /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}   

phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoExternalAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pEncRnd,
    uint8_t bEncRndLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_MIN_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = bEncRndLength + PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_MIN_DATA_LENGTH;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1]  = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2]  = bKeyV;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_ISO_EXTERNAL_AUTHENTICATE_MIN_DATA_LENGTH),
        NULL,
        NULL));

    /* Exchange pEncRnd with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pEncRnd,
        bEncRndLength, /* LE is not present in that command*/
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_IsoInternalAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pRPcd2,
    uint8_t bRPcd2Length,
    uint8_t ** pEncResp,
    uint16_t * pEncRespLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Reset response length */
    *pEncRespLength = 0;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_ISO_INTERNAL_AUTHENTICATE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = bRPcd2Length;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange pRPcd2 with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRPcd2,
        bRPcd2Length,   
        NULL,
        NULL));

    /* LE is present in this command*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,   
        pEncResp,
        &wResponseLength));

    /* Set response length */
    *pEncRespLength = wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_GetVersion(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pVersion,
    uint8_t * pVersionLength 
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_GET_VERSION_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    *pVersionLength = 0;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_GET_VERSION_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LE_NO_LC_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_GET_VERSION_DATA_LENGTH),
        &pResponse,
        &wResponseLength));

    /* Finally we can copy the version information */
    memcpy(pVersion,pResponse,wResponseLength);   /* PRQA S 3200 */
    *pVersionLength =(uint8_t)wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_SAM_Sleep(
                                        phhalHw_SamAV2_DataParams_t * pDataParams
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH + PHHAL_HW_SAMAV2_CMD_SLEEP_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_SLEEP_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH + PHHAL_HW_SAMAV2_CMD_SLEEP_DATA_LENGTH),
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_DESFireAuthenticatePICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bAuthMode,
    uint8_t bISOMode,
    uint8_t bDFKeyNo,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pDivInput,
    uint8_t  bDivInputLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_DESFIRE_AUTH_PICC_MIN_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_DESFIRE_AUTH_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = bAuthMode;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = bISOMode;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = (uint8_t)(PHHAL_HW_SAMAV2_CMD_DESFIRE_AUTH_PICC_MIN_DATA_LENGTH + bDivInputLength);

    /* Fill data */
    aData[0] = bDFKeyNo;
    aData[1] = bKeyNo;
    aData[2] = bKeyV;

    /* Exchange first part with lower layer : send Header cmd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange UID with lower layer : send first part of Data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        PHHAL_HW_SAMAV2_CMD_DESFIRE_AUTH_PICC_MIN_DATA_LENGTH,
        NULL,
        NULL));

    if (bDivInputLength != 0)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            &pResponse,
            &wResponseLength));
    }

    /* Send Le */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_DESFireChangeKeyPICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyCompMeth,
    uint8_t bCfg,
    uint8_t bKeyNoCurrent,
    uint8_t bKeyVCurrent,
    uint8_t bKeyNoNew,
    uint8_t bKeyVNew,
    uint8_t * pDivInput,
    uint8_t  bDivInputLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_DESFIRE_CHANGE_KEY_MIN_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_DESFIRE_CHANGE_KEY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = bKeyCompMeth;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = bCfg;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = (uint8_t)(PHHAL_HW_SAMAV2_CMD_DESFIRE_CHANGE_KEY_MIN_DATA_LENGTH + bDivInputLength);

    /* Filling in Data (first part) */
    aData[0] = bKeyNoCurrent;
    aData[1] = bKeyVCurrent;
    aData[2] = bKeyNoNew;
    aData[3] = bKeyVNew;

    /* Exchange first part with lower layer : send Header cmd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange first part with lower layer : send data(first part) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        PHHAL_HW_SAMAV2_CMD_DESFIRE_CHANGE_KEY_MIN_DATA_LENGTH,
        NULL,
        NULL));

    if (bDivInputLength != 0)
    {
        /* Add DivInput if not null */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Send Le byte */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_DESFireWriteX(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bCrypto,
    uint8_t *pData,
    uint8_t bDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Is this the first chunk of data? */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT)) /* == BUFFER_FIRST or DEFAULT*/
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DESFIRE_WRITE_X_INS;

        /* Set the correct value for LFI depending on CHAINING active or not */
        if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_TXCHAINING) 
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;
        else
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_LAST_FRAME;

        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = bCrypto;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            &pResponse,
            &wResponseLength));        
    }

    /* Was this the last frame? */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT) /* == BUFFER_CONT */
    {
        /* Add Data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pData,
            bDataLength,
            &pResponse,
            &wResponseLength));
    }
    else /* == BUFFER_LAST or DEFAULT*/
    {
        /* Add Data*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pData,
            bDataLength,
            &pResponse,
            &wResponseLength));

        /* Update length byte */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

        /* Add LE byte and perform the exchange */
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* status check */
        if ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
        {
            return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
        }
        else
        {
            PH_CHECK_SUCCESS(status);
        }

        /* Check for length error */
        if (wResponseLength != 0)
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_DESFireReadX(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bCrypto,
    uint8_t *pData,
    uint8_t bDataLength,
    uint8_t ** ppRxBuffer,
    uint16_t *pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bNonLastFrame[1];

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_DESFIRE_READ_X_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = bCrypto;

    /* Are we in RX chaining mode? */
    if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING) 
    {
        /* Indicate chaining */
        bNonLastFrame[0] = PHHAL_HW_SAMAV2_ISO7816_CHAINED_FRAME;

        /* Set LC */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 1;

        /* Exchange first part with lower layer : send Header cmd */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        /* Exchange second part with lower layer : send chaining indicator */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            bNonLastFrame,
            1,
            NULL,
            NULL));
    }
    else
    {
        /* Set LC */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

        /* Exchange first part with lower layer : send Header cmd */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        /* Exchange second part with lower layer : send Data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pData,
            bDataLength,
            NULL,
            NULL));
    }

    /* Send Le byte and perform actual exchange */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* Status check */
    if ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        status = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    /* Map receive buffer */
    *ppRxBuffer = pResponse;
    *pRxLength = wResponseLength;    

    return status;
}

phStatus_t phhalHw_SamAV2_Cmd_ULCAuthenticatePICC(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t * pDivInput,
    uint8_t  bDivInputLength 
    )
{

    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_ULC_AUTH_PICC_MIN_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS]   = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS]   = PHHAL_HW_SAMAV2_CMD_ULC_AUTH_PICC_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]    = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]    = PHHAL_HW_SAMAV2_CMD_ULC_AUTH_PICC_MIN_DATA_LENGTH + bDivInputLength;

    if (bDivInputLength != 0)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }

    aData[0] = bKeyNo;
    aData[1] = bKeyV;

    /* Exchange first part with lower layer : send Header cmd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST, 
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange UID with lower layer : send first part of Data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        PHHAL_HW_SAMAV2_CMD_ULC_AUTH_PICC_MIN_DATA_LENGTH,
        NULL,
        NULL));

    if (bDivInputLength != 0)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Exchange frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        NULL,
        0,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

/*************************************************************************************************************************/
/****************************************ISO3A LAYER IMPLEMENTATION X Mode ***********************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/



phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_RequestA_Wakeup(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bReqaWakeupCode,
    uint8_t * pAtqa               
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_CMD_LC;

    /* Filling in ISO7816 Data */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bReqaWakeupCode;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2),
        &pResponse,
        &wResponseLength));

    if (wResponseLength != PHHAL_HW_SAMAV2_CMD_REQUEST_WAKEUP_RESPONSE_LENGTH)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Finally we can copy the ATQA */
    memcpy(pAtqa,pResponse,2);  /* PRQA S 3200 */
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_HaltA(
    phhalHw_SamAV2_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_HALTA_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_AnticollisionSelect(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pSelCodes,
    uint8_t bSelCodesLength,
    uint8_t * pSak,
    uint8_t * pUid,
    uint8_t * pUidLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_ANTICOLLSION_SELECT_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bSelCodesLength;

    /* Checks */
    if ((bSelCodesLength == 0) ||(bSelCodesLength > 3))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    if (pSelCodes == NULL)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Exchange with lower layer : send Header cmd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer : send  SEL sequence*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pSelCodes,
        bSelCodesLength,
        NULL,
        NULL));

    /* Exchange with lower layer : send  LE byte */
    statusTmp = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    if ((statusTmp & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_ISO_UID_INCOMPLETE && 
        (statusTmp & PH_ERR_MASK) != PH_ERR_SUCCESS)
    {
        return statusTmp;
    }

    if (wResponseLength > 11 || wResponseLength < 4)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Copy output data */
    *pUidLength =(uint8_t)( wResponseLength-1 );
    memcpy(pUid, &pResponse[1], *pUidLength);  /* PRQA S 3200 */
    *pSak = pResponse[0];

    return statusTmp;
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_ActivateIdle(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bNumCards,
    uint16_t wTime,
    uint8_t * pAtqaIn,
    uint8_t * pSakIn,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM dataLength;
    uint8_t PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_ACTIVATE_IDLE_CMD_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_ACTIVATE_IDLE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bNumCards;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Check and set the data length and data for the given filters */
    dataLength = 0;

    if (bOption & PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_TIME)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] =(uint8_t)(wTime>>8);
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] =(uint8_t)(wTime);
        dataLength += 2;
    }
    if (bOption & PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_ATQA)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] |= 0x01;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 1] = pAtqaIn[0];
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 2] = pAtqaIn[1];
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 3] = pAtqaIn[2];
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 4] = pAtqaIn[3];
        dataLength += 4;
    }
    if (bOption & PHHAL_HW_SAMAV2_CMD_ISO14443_3_ACTIVATEIDLE_APPLY_SAK)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] |= 0x02;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 1] = pSakIn[0];
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 2] = pSakIn[1];
        dataLength += 2;
    }

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = dataLength;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + dataLength + 1] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    if (dataLength == 0)
    {
        dataLength += PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH;
    }else
    {
        dataLength += PHHAL_HW_SAMAV2_ISO7816_HEADER_LE_LENGTH;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        dataLength,
        &pResponse,
        &wResponseLength));

    *pRxLength = wResponseLength;
    *ppRxBuffer = pResponse;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}



phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_ActivateWakeUp(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pUid,
    uint8_t bUidLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_AKTIVATE_WAKEUP_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bUidLength;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pUid,
        bUidLength,
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_3_TransparentExchange(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pTxBuffer,
    uint8_t bTxLength,
    uint8_t bTxBitLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength,
    uint8_t * pRxBitLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    phStatus_t  PH_MEMLOC_REM status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Reset response length */
    *pRxLength = 0;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bTxBitLength;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bTxLength;

    /* Buffer header */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Append data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pTxBuffer,
        (uint16_t)bTxLength,
        NULL,
        NULL));

    /* Append LE byte and exchange */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* Incomplete byte is not treated as an error */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_INCOMPLETE_BYTE)
    {
        PH_CHECK_SUCCESS(status);
        *pRxBitLength = 0;
    }
    else
    {
        *pRxBitLength =(uint8_t)pDataParams->wAdditionalInfo;
    }

    /* Return received data */
    *pRxLength = wResponseLength;
    *ppRxBuffer = pResponse;

    /* Check length */
    if (wResponseLength > PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_RESPONSE_MAX)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return status;
}

/*************************************************************************************************************************/
/****************************************ISO4A LAYER IMPLEMENTATION X Mode ***********************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_Init(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCid,
    uint8_t bDri,
    uint8_t bDsi,
    uint8_t bFwi,
    uint8_t bFsci
    )
{

    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_INIT_DATA_LENGTH_MAX];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_INIT_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_INIT_DATA_LENGTH_MAX;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+1] = bCid;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+2] = bDri;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+3] = bDsi;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+4] = bFwi;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS+5] = bFsci;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_INIT_DATA_LENGTH_MAX),
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_Deselect(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bFreeCid
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_DESELECT_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bFreeCid;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_FreeCid(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pCid,
    uint8_t bCidLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_FREE_CID_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bCidLength;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        &pResponse,
        &wResponseLength));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pCid,
        bCidLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_RATS_PPS(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCidIn,
    uint8_t bDsiIn,
    uint8_t bDriIn,
    uint8_t * pCidOut,
    uint8_t * pDsiOut,
    uint8_t * pDriOut,
    uint8_t * pAts
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LE_LENGTH + PHHAL_HW_SAMAV2_RATS_PPS_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_RATS_PPS_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_RATS_PPS_DATA_LENGTH;

    /* Filling in ISO7816 Data */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bCidIn;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = bDriIn;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 3] = bDsiIn;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 4] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_LE_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LE_LENGTH + PHHAL_HW_SAMAV2_RATS_PPS_DATA_LENGTH),
        &pResponse,
        &wResponseLength));

    if (wResponseLength < PHHAL_HW_SAMAV2_RATS_PPS_RESPONSE_DATA)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    *pCidOut = pResponse[0];
    *pDriOut = pResponse[1];
    *pDsiOut = pResponse[2];

    memcpy(pAts, &pResponse[3], wResponseLength-3);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_PresenceCheck(
    phhalHw_SamAV2_DataParams_t * pDataParams
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_PRESENCE_CHECK_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_X_ISO14443_4_Exchange(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pAppDataIn,
    uint8_t bLenAppData,
    uint8_t ** ppAppDataOut,
    uint16_t * pAppDataOutLength
    )

{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Prepare ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_EXCHANGE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]  = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Data has to be send to the card */
    if (bLenAppData > 0)
    {
        /* Prepare ISO7816 header */
        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]  = 0xAF;
        }
        else
        {
            aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS]  = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        }
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]  = bLenAppData;

        /* Buffer the header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        /* Exchange with lower layer including given data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pAppDataIn,
            bLenAppData,
            NULL,
            NULL));

        if (wOption & PH_EXCHANGE_TXCHAINING)
        {
            status = phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                NULL,
                0,
                &pResponse,
                &wResponseLength);
        }
        else
        {
            status = phhalHw_SamAV2_Cmd_7816Exchange(
                pDataParams,
                PH_EXCHANGE_BUFFER_LAST,
                gaDefaultLe,
                1,
                &pResponse,
                &wResponseLength);
        }
    }
    /* chaining from card */
    else
    {
        /* Prepare ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0x00;

        /* Exchange with lower layer */
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_DEFAULT,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            &pResponse,
            &wResponseLength);
    }

    /* Return received data */
    *pAppDataOutLength = wResponseLength;
    *ppAppDataOut = pResponse;

    /* Check status */
    if ((status & PH_ERR_MASK) != PH_ERR_SUCCESS_CHAINING)
    {
        PH_CHECK_SUCCESS(status);
    }
    else
    {
        status = PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    /* Check length */
    if (wResponseLength >(PHHAL_HW_SAMAV2_ISO7816_EXCHANGE_RESPONSE_MAX))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

/*************************************************************************************************************************/
/****************************************MIFARE CLASSIC X Mode ***********************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

phStatus_t phhalHw_SamAV2_Cmd_MfRead(
                                     phhalHw_SamAV2_DataParams_t * pDataParams,    
                                     uint8_t bOption,
                                     uint8_t * pBlocks,
                                     uint8_t bBlocksLength,
                                     uint8_t ** ppData,
                                     uint16_t * pDataLength
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bDataLength;
    uint8_t     PH_MEMLOC_COUNT bIndex;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_READ_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS]  = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    if (bOption & PHHAL_HW_SAMAV2_CMD_CHANGE_MFREAD_MULTIBLOCK)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = 0x01;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bBlocksLength;

        /* Calculate expected response length */
        bDataLength = 0;
        for (bIndex = 1; bIndex < bBlocksLength; bIndex = bIndex + 2)
        {
            bDataLength = bDataLength +(pBlocks[bIndex] << 4);
        }
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bBlocksLength;
        bDataLength = bBlocksLength << 4;
    }

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pBlocks,
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS],
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    /* Check response length */
    if (wResponseLength != bDataLength)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Return response */
    *ppData = pResponse;
    *pDataLength = wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfWrite(
                                      phhalHw_SamAV2_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pData,                            
                                      uint8_t bDataLength
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bType = 0;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if (wOption & PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MFULTRALIGTH)
    {
        if (wOption & PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MULTIBLOCK)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        bType |= 0x01;
    }

    if (wOption & PHHAL_HW_SAMAV2_CMD_CHANGE_MFWRITE_TYPE_MULTIBLOCK)
    {
        bType |= 0x02;
    }

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_WRITE_INS;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bType;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)bDataLength;

        /* Exchange first part with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Set the data field */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_CONT, 
        pData,
        bDataLength,
        NULL,
        NULL));

    /* not the last block -> return */
    if ((wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    /* Actual exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_LAST, 
        NULL,
        0,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfValueWrite(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBlocks,
    uint8_t   bBlocksLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if ((bBlocksLength % 9) != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_WRITE_VALUE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bBlocksLength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pBlocks,
        bBlocksLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfIncrement(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBlocks,
    uint8_t   bBlocksLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if ((bBlocksLength % 6) != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_INCREMENT_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bBlocksLength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pBlocks,
        bBlocksLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfDecrement(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pBlocks,
    uint8_t   bBlocksLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if ((bBlocksLength % 6) != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_DECREMENT_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bBlocksLength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pBlocks,
        bBlocksLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfRestore(
                                        phhalHw_SamAV2_DataParams_t * pDataParams,
                                        uint8_t * pBlocks,
                                        uint8_t   bBlocksLength
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if ((bBlocksLength % 2) != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_RESTORE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bBlocksLength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pBlocks,
        bBlocksLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t * pUid,
    uint8_t bKeyNoM,
    uint8_t bKeyVM,
    uint8_t bKeyType,
    uint8_t bAuthBlockNo,
    uint8_t bDivBlockNo
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aInfo[PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_INFO_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t     PH_MEMLOC_REM bInfoLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_EXCHANGE_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    aInfo[0] = bKeyNoM;
    aInfo[1] = bKeyVM;
    if (bKeyType == PHHAL_HW_MFC_KEYA)
    {
        aInfo[2] = 0x0A;
    }else if (bKeyType == PHHAL_HW_MFC_KEYB)
    {
        aInfo[2] = 0x0B;
    }else
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    aInfo[3] = bAuthBlockNo;
    aInfo[4] = bDivBlockNo;

    if ((bOption & PHHAL_HW_SAMAV2_CMD_CHANGE_MFAUTHENTICATE_DIVERSIFICATION) != 0)
    {
        /* UID ||INFO || DIV INFO */
        bInfoLength = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_INFO_LENGTH;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 4+bInfoLength;
    }else
    {
        /* UID ||INFO */
        bInfoLength = PHHAL_HW_SAMAV2_CMD_MF_AUTHENTICATE_INFO_LENGTH - 1;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 4+bInfoLength;
    }

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange UID with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pUid,
        4,
        NULL,
        NULL));

    /* Exchange UID with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        aInfo,
        bInfoLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfAuthenticateRead(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pUid,
    uint8_t bCmdSettings,
    uint8_t bKeyNoM,
    uint8_t bKeyVM,
    uint8_t bKeyType,
    uint8_t bAuthBlockNo,
    uint8_t bDivBlockNo,
    uint8_t * pBlocks,
    uint8_t bBlocksLength,
    uint8_t ** ppData,
    uint16_t * pDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM pAuthData[7];
    uint8_t     PH_MEMLOC_REM bAuthDataLength=0;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        if (bCmdSettings & 0x01)
        {
            /* the first frame has to contain authentication data */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTH_READ_INS;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

        /* Length = uid_length + auth_length + num_blocks + blocks. */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 4 + 5 + 1 + bBlocksLength;

        if (bCmdSettings & 0x02)
        {
            /* diversify the key -> authentication block number needed. */
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]++;
        }

        /* Exchange first part with lower layer : send Header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST, 
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH, 
            NULL, 
            NULL));

        /* Exchange first part with lower layer : send Header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT, 
            pUid,
            4, 
            NULL, 
            NULL));
    }

    pAuthData[bAuthDataLength++] = bCmdSettings;
    if ((bCmdSettings & 0x01) == 0)
    {
        pAuthData[bAuthDataLength++] = bKeyNoM;
        pAuthData[bAuthDataLength++] = bKeyVM;
        pAuthData[bAuthDataLength++] = bKeyType;
    }
    pAuthData[bAuthDataLength++] = bAuthBlockNo;

    if (bCmdSettings & 0x02)
    {
        pAuthData[bAuthDataLength++] = bDivBlockNo;
    }
    pAuthData[bAuthDataLength++] = bBlocksLength;

    /* Exchange auth data with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_CONT, 
        pAuthData,
        bAuthDataLength, 
        NULL, 
        NULL));

    /* Exchange blocks with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_CONT, 
        pBlocks,
        bBlocksLength, 
        NULL, 
        NULL));

    /* not the last block -> return */
    if ((wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    /* Actual exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_LAST, 
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    *ppData = pResponse;
    *pDataLength = wResponseLength;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}


phStatus_t phhalHw_SamAV2_Cmd_MfAuthenticateWrite(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pUid,
    uint8_t bCmdSettings,
    uint8_t bKeyNoM,
    uint8_t bKeyVM,
    uint8_t bKeyType,
    uint8_t bAuthBlockNo,
    uint8_t bDivBlockNo,
    uint8_t * pBlocks,
    uint8_t bBlocksLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM pAuthData[7];
    uint8_t     PH_MEMLOC_REM bAuthDataLength=0;
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if (bBlocksLength % 17 != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        if (bCmdSettings & 0x01)
        {
            /* the first frame has to contain authentication data */
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_AUTH_WRITE_INS;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

        /* length = uid_length + auth_length + num_blocks + blocks. */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 4 + 5 + 1 + bBlocksLength;

        if (bCmdSettings & 0x02)
        {
            /* diversify the key -> authentication block number needed. */
            aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS]++;
        }


        /* Exchange first part with lower layer : send Header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST, 
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH, 
            NULL, 
            NULL));

        /* Exchange first part with lower layer : send Header */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT, 
            pUid,
            4, 
            NULL, 
            NULL));
    }

    pAuthData[bAuthDataLength++] = bCmdSettings;
    if ((bCmdSettings & 0x01) == 0)
    {
        pAuthData[bAuthDataLength++] = bKeyNoM;
        pAuthData[bAuthDataLength++] = bKeyVM;
        pAuthData[bAuthDataLength++] = bKeyType;
    }
    pAuthData[bAuthDataLength++] = bAuthBlockNo;

    if (bCmdSettings & 0x02)
    {
        pAuthData[bAuthDataLength++] = bDivBlockNo;
    }
    pAuthData[bAuthDataLength++] =(uint8_t)(bBlocksLength / 17);

    /* Exchange auth data with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_CONT, 
        pAuthData,
        bAuthDataLength, 
        NULL, 
        NULL));

    /* Exchange blocks with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_CONT, 
        pBlocks,
        bBlocksLength, 
        NULL,
        NULL));

    /* not the last block -> return */
    if ((wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams, 
        PH_EXCHANGE_BUFFER_LAST, 
        NULL,
        0, 
        &pResponse,
        &wResponseLength));


    if (wResponseLength != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfChangeKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyCompMeth,
    uint8_t bKeyNoM,
    uint8_t bKeyVMA,
    uint8_t bKeyVMB,
    uint8_t bBlockNo,
    uint8_t *pAccessConditions,
    uint8_t *pUid,
    uint8_t bDivBlockNo
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_NO_DIV_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bKeyCompMeth;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    if ((bKeyCompMeth & 0x06) != 0)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_LENGTH;
    }
    else
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_NO_DIV_LENGTH;
    }

    aData[0] = bKeyNoM;
    aData[1] = bKeyVMA;
    aData[2] = bKeyVMB;
    aData[3] = bBlockNo;
    memcpy(&aData[4], pAccessConditions, 4);  /* PRQA S 3200 */

    /* Exchange first part with lower layer : send Header cmd */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange Data with lower layer */
    if ((bKeyCompMeth & 0x06) == 0)
    {
        /* Send data without div info*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            aData,
            PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_NO_DIV_LENGTH,
            &pResponse,
            &wResponseLength));
    }
    else
    {
        /* Send data with div info*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            aData,
            PHHAL_HW_SAMAV2_CMD_MF_CHANGE_KEY_DATA_NO_DIV_LENGTH,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pUid,
            4,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            &bDivBlockNo,
            1,
            &pResponse,
            &wResponseLength));
    }

    if (wResponseLength != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

/*************************************************************************************************************************/
/****************************************MIFARE PLUS X Mode **************************************************************/
/*************************************************************************************************************************/
/*************************************************************************************************************************/

phStatus_t phhalHw_SamAV2_Cmd_MfpWritePerso(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint8_t * pValues,
    uint8_t   bValuesLength,
    uint8_t * pPiccReturnCode
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    if ((bValuesLength % 18) != 0)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_WRITE_PERSO_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bValuesLength;

    /* Exchange first part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange second part with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pValues,
        bValuesLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* Assign status code upfront */
    if (wResponseLength)
    {
        *pPiccReturnCode = pResponse[0];
    }
    else
    {
        *pPiccReturnCode = 0x90;
    }

    /* Check the return code */
    PH_CHECK_SUCCESS(status);

    if (wResponseLength != 1)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfpCombinedWrite(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pData,
    uint8_t bDataLength,
    uint8_t * pPiccReturnCode 
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Is this the first chunk of data? */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_WRITE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));        
    }

    /* Was this the last frame? */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        /* Add Data */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pData,
            bDataLength,
            NULL,
            NULL));        

        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }
    else
    {
        /* Add Data*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pData,
            bDataLength,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

        /* Perform Exchange - No data Provided */
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* Check for MFP Error */
        if (((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_CRYPTO) &&
            ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN) &&
            ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
        {
            /* Check the return code */
            PH_CHECK_SUCCESS(status);
        }

        /* Assign status code upfront */
        if (wResponseLength)
        {
            *pPiccReturnCode = pResponse[0];
        }
        else
        {
            *pPiccReturnCode = 0x90;
        }

        if (wResponseLength != 1)
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }

        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }
}

phStatus_t phhalHw_SamAV2_Cmd_MfpCombinedRead(
    phhalHw_SamAV2_DataParams_t * pDataParams, 
    uint16_t wOption,
    uint8_t *pPlainCommand,
    uint8_t bPlainCommandLength,
    uint8_t ** pData,
    uint16_t * pDataLength,
    uint8_t * pPiccReturnCode
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_READ;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bPlainCommandLength;

    /* Are we in RX chaining mode? */
    if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING) 
    {
        /* Exchange with lower layer */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 0;
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_DEFAULT,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            &pResponse,
            &wResponseLength); 
    }
    else
    {
        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pPlainCommand,
            bPlainCommandLength,
            NULL,
            NULL));

        /* Exchange with lower layer */
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);
    }

    /* Assign status code upfront */
    if (wResponseLength)
    {
        *pPiccReturnCode = pResponse[0];
    }
    else
    {
        *pPiccReturnCode = 0x90;
    }

    /* Length check */
    if ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_MIFARE_GEN)
    {
        if (!wResponseLength)
        {
            /* Remapping of return values */
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }

    if (((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE) &&(status != PH_ERR_SUCCESS))
    {            
        return status;
    }

    if ((wOption & PH_EXCHANGE_MODE_MASK) == PH_EXCHANGE_RXCHAINING) 
    {
        /* We only receive data */
        *pPiccReturnCode = 0x90;
    }
    else
    {           
        /* Increment response length as we do not want to return return code */
        pResponse++;
        wResponseLength--;
    }

    *pData = pResponse;
    *pDataLength =(wResponseLength); 

    if ((status & PH_ERR_MASK) == PHHAL_HW_SAMAV2_ERR_OK_CHAINING_ACTIVE)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS_CHAINING, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(status, PH_COMP_HAL);   
}


phStatus_t phhalHw_SamAV2_Cmd_MfpAuthenticate(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bOption,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint16_t wBlockNo,
    uint8_t * pPcdCapsIn,
    uint8_t bPcdCapsInLength,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t * pPcdCapsOut,
    uint8_t * pPdCaps,
    uint8_t * pPiccReturnCode
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[5];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    aData[0] = bKeyNo;
    aData[1] = bKeyV;
    aData[2] =(uint8_t) wBlockNo; 
    aData[3] =(uint8_t)(wBlockNo >> 8);
    aData[4] = bPcdCapsInLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 1 /* Key Nr */ + 1 /* Key Ver */ + 2 /* BNR */ + 1 /* Len PCDCaps */ + bPcdCapsInLength + bDivInputLength;

    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }  

    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }  

    if (!(bOption & PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST))
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x02;
    }

    if (bOption & PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL2_KDF)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x04;
    }

    if (bOption & PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_SL3_KDF)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x0C;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        5,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pPcdCapsIn,
        bPcdCapsInLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bDivInputLength,
        NULL,
        NULL));


    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* Assign status code upfront */
    if (wResponseLength)
    {
        *pPiccReturnCode = pResponse[0];
    }

    /* Check the return code */
    PH_CHECK_SUCCESS(status);

    /* Assign success in case of status is ok */
    *pPiccReturnCode = 0x90;

    if (bOption & PHHAL_HW_SAMAV2_CMD_MFP_AUTHENTICATE_AUTH_FIRST)
    {
        if (wResponseLength !=(6 /* PdCaps */ + 6 /* PcdCaps */))
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }

        memcpy(pPdCaps, pResponse, 6);          /* PRQA S 3200 */
        memcpy(pPcdCapsOut, &pResponse[6], 6);  /* PRQA S 3200 */

    }
    else
    {
        if (wResponseLength != 0)
        {
            return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
        }
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL); 
}

phStatus_t phhalHw_SamAV2_Cmd_MfpChangeKey(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bCmd,
    uint16_t wBlockNo,
    uint8_t bKeyNo,
    uint8_t bKeyV, 
    uint8_t * pDivInput,
    uint8_t bDivInputLength, 
    uint8_t * pPiccReturnCode
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t     PH_MEMLOC_REM aData[5];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_CHANGE_KEY_INS;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] =(uint8_t)(5 + bDivInputLength);

    aData[0] = bCmd;
    aData[1] =(uint8_t)(wBlockNo & 0xff); /* LSB */
    aData[2] =(uint8_t)(wBlockNo >> 8);   /* MSB */

    aData[3] = bKeyNo;
    aData[4] = bKeyV;

    *pPiccReturnCode = 0;

    if (bDivInputLength != 0)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        aData,
        5,
        NULL,
        NULL));

    if (bDivInputLength != 0)
    {
        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pDivInput,
            bDivInputLength,
            NULL,
            NULL));
    }

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* Check for MFP Error */
    if (((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_CRYPTO) &&
        ((status & PH_ERR_MASK) != PHHAL_HW_SAMAV2_ERR_MIFARE_PLUS_GEN) &&
        ((status & PH_ERR_MASK) != PH_ERR_SUCCESS))
    {
        /* Check the return code */
        PH_CHECK_SUCCESS(status);
    }

    /* Assign status code upfront */
    if (wResponseLength)
    {
        *pPiccReturnCode = pResponse[0];
    }
    else
    {
        *pPiccReturnCode = 0x90;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfpProximityCheck(
    phhalHw_SamAV2_DataParams_t * pDataParams,  
    uint8_t bOption,
    uint8_t bKeyNo,
    uint8_t bKeyV,
    uint8_t bBlCnt,
    uint8_t * pDivInput,
    uint8_t bDivInputLength,
    uint8_t * pPiccReturnCode 
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = 3 + bDivInputLength;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = bKeyV;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 3] = bBlCnt; 

    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }

    if (bOption & PHHAL_HW_SAMAV2_CMD_MFP_PROXIMITY_CHECK_RAND_PROCESSING)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x02;
        /* Override key number in case of random processing */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = 0xFF;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = 0xFF;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 3),
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bDivInputLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);


    /* Assign status code upfront */
    if (wResponseLength)
    {
        *pPiccReturnCode = pResponse[0];
    }
    /* Check the return code */
    PH_CHECK_SUCCESS(status);

    /* Assign 0x90 in case of success */
    *pPiccReturnCode = 0x90;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfpVirtualCardSupport(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t* pData,
    uint8_t bDataLength,
    uint8_t * pPcdCapabilities,
    uint8_t  bPcdCapabilitiesLength,
    uint8_t ** pSupportedCardInfo,
    uint16_t * pSupportedCardInfoLength,
    uint8_t * pPiccReturnCode
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Is this the first chunk of data? */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Filling in ISO7816 header */
        aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_VIRTUAL_CARD_SUPPORT;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
        aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

        /* Exchange with lower layer */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_FIRST,
            aCmd,
            PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
            NULL,
            NULL));
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pData,
        bDataLength,
        NULL,
        NULL));


    /* Was this the last frame? */
    if (!(wOption & PH_EXCHANGE_BUFFERED_BIT))
    {
        /* Add Length of PCD Caps*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            &bPcdCapabilitiesLength,
            1,
            NULL,
            NULL));

        /* Add PCD Caps*/
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_CONT,
            pPcdCapabilities,
            bPcdCapabilitiesLength,
            NULL,
            NULL));

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_UpdateLc(pDataParams));

        /* Exchange with lower layer */
        status = phhalHw_SamAV2_Cmd_7816Exchange(
            pDataParams,
            PH_EXCHANGE_BUFFER_LAST,
            gaDefaultLe,
            1,
            &pResponse,
            &wResponseLength);

        /* Assign status code upfront */
        if (wResponseLength)
        {
            *pPiccReturnCode = pResponse[0];
        }

        /* Check the return code */
        PH_CHECK_SUCCESS(status);

        *pPiccReturnCode = 0x90;
        *pSupportedCardInfoLength =(uint8_t)wResponseLength;
        *pSupportedCardInfo = pResponse;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_MfpSelectVirtualCard(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bKeyNo,   
    uint8_t bKeyV, 
    uint8_t * pIid, 
    uint8_t * pDivInput, 
    uint8_t  bDivInputLength,
    uint8_t * pPiccReturnCode
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp, status;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2];
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint8_t *   PH_MEMLOC_REM pResponse;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_MFP_SELECT_VIRTUAL_CARD;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDivInputLength + 2 /* Key number and version */ + 16 /* IID */;

    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] = bKeyNo;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] = bKeyV;

    /* check for diversification */
    if (bDivInputLength)
    {
        aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] |= 0x01;
    }

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + 2),
        NULL,
        NULL));

    /* Add Iid */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pIid,
        16,
        NULL,
        NULL));

    /* Add diff input */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pDivInput,
        bDivInputLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    status = phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength);

    /* Assign status code upfront */
    if (wResponseLength)
    {
        *pPiccReturnCode = pResponse[0];
    }

    /* Check the return code */
    PH_CHECK_SUCCESS(status);

    /* Assign 0x90 in case of success */
    *pPiccReturnCode = 0x90;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_RC_RFControl(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wTime
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL_DATA_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */                   
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL_DATA_LENGTH;

    /* Setting time - Note: Time==0 -> Field OFF */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 1] =(uint8_t)(wTime);
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS + 2] =(uint8_t)(wTime >> 8);

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        (PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH + PHHAL_HW_SAMAV2_CMD_RC_RFCONTROL_DATA_LENGTH),
        &pResponse,
        &wResponseLength));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_RC_ReadRegister(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t *pRegAddress,
    uint8_t bRegAddressLength,
    uint8_t *pValue
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;
    uint16_t    PH_MEMLOC_REM wExpectedResponseLength;

    /* Calculate expected response lengths */
    wExpectedResponseLength = bRegAddressLength;
    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_RC_READREGISTER;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bRegAddressLength;

    /* Send first part of command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Send registers part of command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_CONT,
        pRegAddress,
        bRegAddressLength,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        gaDefaultLe,
        1,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != wExpectedResponseLength)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* copy data to the output buffer */
    memcpy(pValue,pResponse,wResponseLength);   /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_RC_WriteRegister(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t * pData,
    uint8_t bDataLength
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Check if the length is a multiple of two */
    if (((bDataLength % 2) != 0)||((bDataLength<<1) > PHHAL_HW_SAMAV2_CMD_RC_WRITEREGISTER_MAX_DATA_LENGTH))
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_RC_WRITEREGISTER;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P1_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

    /* Send first part of command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pData,
        bDataLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_RC_Init(
                                      phhalHw_SamAV2_DataParams_t * pDataParams,
                                      uint8_t bLoadReg    
                                      )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_RC_INIT;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bLoadReg;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;

    /* Exchange with lower layer */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_DEFAULT,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_RC_LoadRegisterValueSet(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bStoreReg,
    uint8_t *pData,
    uint8_t bDataLength
    )  
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCmd[PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH];
    uint8_t *   PH_MEMLOC_REM pResponse;
    uint16_t    PH_MEMLOC_REM wResponseLength;

    /* Check if StoreReg number is correct(< 7)*/
    if (bStoreReg > 0x07)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Check if the Number of adress is different from 0 and <= 31*/
    if ((bDataLength == 0) ||(((uint8_t)(bDataLength/2)) > PHHAL_HW_SAMAV2_CMD_RC_LOAD_REG_VALUE_SET_MAX_DATA_LENGTH))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Filling in ISO7816 header */
    aCmd[PHHAL_HW_SAMAV2_ISO7816_CLA_POS] = PHHAL_HW_SAMAV2_ISO7816_CLA_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_INS_POS] = PHHAL_HW_SAMAV2_CMD_RC_LOAD_REG_VALUE_SET;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P1_POS] = bStoreReg;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_P2_POS] = PHHAL_HW_SAMAV2_ISO7816_DEFAULT_P2_BYTE;
    aCmd[PHHAL_HW_SAMAV2_ISO7816_LC_POS] = bDataLength;

    /* Send first part of command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_FIRST,
        aCmd,
        PHHAL_HW_SAMAV2_ISO7816_HEADER_LENGTH,
        NULL,
        NULL));

    /* Exchange address and data */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Cmd_7816Exchange(
        pDataParams,
        PH_EXCHANGE_BUFFER_LAST,
        pData,
        bDataLength,
        &pResponse,
        &wResponseLength));

    if (wResponseLength != 0x00)
    {
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Cmd_7816Exchange(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wOption, 
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxLength
    )
{
    phStatus_t  PH_MEMLOC_REM status;
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bUseEncryption;
    uint8_t     PH_MEMLOC_REM bUseMac;
    uint8_t     PH_MEMLOC_REM bSamCommand;
    uint8_t     PH_MEMLOC_REM bP1;
    uint8_t     PH_MEMLOC_REM bP2;
    uint8_t     PH_MEMLOC_REM bFirstResp = PH_OFF;
    uint8_t     PH_MEMLOC_REM bFirstCmd = PH_OFF;
    uint8_t     PH_MEMLOC_REM bLast = PH_OFF;
    uint8_t *   PH_MEMLOC_REM pTmpBuffer;
    uint16_t    PH_MEMLOC_REM wTmpBufferSize;
    uint8_t *   PH_MEMLOC_REM pRxBufferTmp;
    uint16_t    PH_MEMLOC_REM wRxLengthTmp;
    uint16_t    PH_MEMLOC_REM wTxStartPosTmp;
    uint16_t    PH_MEMLOC_REM wTxLengthTmp;

    /* reset received length */
    if (pRxLength != NULL)
    {
        *pRxLength = 0;
    }

    /* Check if caller has provided valid RxBuffer */
    if (ppRxBuffer == NULL)
    {
        ppRxBuffer = &pRxBufferTmp;
    }
    if (pRxLength == NULL)
    {
        pRxLength = &wRxLengthTmp;
    }

    /* Get / Check command */
    if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
    {
        /* Check minimum length for first call */
        if (wTxLength < PHHAL_HW_SAMAV2_ISO7816_HEADER_NO_LC_LENGTH)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        bSamCommand = pTxBuffer[PHHAL_HW_SAMAV2_ISO7816_INS_POS];
    }
    else
    {
        /* Preserved commands(all except exchange) */
        if (pDataParams->wTxBufLen_Cmd != 0)
        {
            /* Ensure SAM command is unequal exchange */
            bSamCommand = PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS ^ 0x01;
        }
        else
        {
            /* Exchange command */
            if (pDataParams->wTxBufLen != 0)
            {
                bSamCommand = PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS;
            }
            /* Everything else is definitely an internal error */
            /*(minimum length for first call is > 0) */
            else
            {
                return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
            }
        }
    }

    /* Exchange command can use default buffers */
    if (bSamCommand == PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS)
    {
        /* Reset TxLength */
        if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
        {
            pDataParams->wTxBufLen = 0;
            pDataParams->wTxBufLen_Cmd = 0;
        }

        wTxStartPosTmp = pDataParams->wTxBufStartPos;
        pTmpBuffer = &pDataParams->pTxBuffer[wTxStartPosTmp + pDataParams->wTxBufLen];
        wTmpBufferSize = pDataParams->wTxBufSize -(wTxStartPosTmp + pDataParams->wTxBufLen);
    }
    /* Other commands -> Preserve all buffer data */
    else
    {
        /* Reset TxLength */
        if (!(wOption & PH_EXCHANGE_LEAVE_BUFFER_BIT))
        {
            pDataParams->wTxBufLen_Cmd = 0;
        }

        /* TxBuffer equals RxBuffer */
        if (pDataParams->pTxBuffer == pDataParams->pRxBuffer)
        {
            /* Start at TxLength if neccessary */
            if ((pDataParams->wTxBufStartPos + pDataParams->wTxBufLen) >=
                (pDataParams->wRxBufLen))
            {
                wTxStartPosTmp = pDataParams->wTxBufStartPos + pDataParams->wTxBufLen;
                pTmpBuffer = &pDataParams->pTxBuffer[wTxStartPosTmp + pDataParams->wTxBufLen_Cmd];
                wTmpBufferSize = pDataParams->wTxBufSize -(wTxStartPosTmp + pDataParams->wTxBufLen_Cmd);
            }
            /* Start at RxLength if neccessary */
            else
            {
                wTxStartPosTmp = pDataParams->wRxBufLen;
                pTmpBuffer = &pDataParams->pTxBuffer[wTxStartPosTmp + pDataParams->wTxBufLen_Cmd];
                wTmpBufferSize = pDataParams->wTxBufSize -(wTxStartPosTmp + pDataParams->wTxBufLen_Cmd);
            }
        }
        /* Buffers are different */
        else
        {
            wTxStartPosTmp = pDataParams->wTxBufLen;
            pTmpBuffer = &pDataParams->pTxBuffer[wTxStartPosTmp + pDataParams->wTxBufLen_Cmd];
            wTmpBufferSize = pDataParams->wTxBufSize -(wTxStartPosTmp + pDataParams->wTxBufLen_Cmd);
        }
    }

    /* Check for buffer overflow */
    if (wTxLength > wTmpBufferSize)
    {
        return PH_ADD_COMPCODE(PH_ERR_BUFFER_OVERFLOW, PH_COMP_HAL);
    }

    /* Copy TxBuffer contents */
    memcpy(pTmpBuffer, pTxBuffer, wTxLength);  /* PRQA S 3200 */

    /* Exchange uses wTxBufLen */
    if (bSamCommand == PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS)
    {
        pDataParams->wTxBufLen = pDataParams->wTxBufLen + wTxLength;
        wTxLengthTmp = pDataParams->wTxBufLen;
    }
    /* Other commands use wTxBufLen_Cmd */
    else
    {
        pDataParams->wTxBufLen_Cmd = pDataParams->wTxBufLen_Cmd + wTxLength;
        wTxLengthTmp = pDataParams->wTxBufLen_Cmd;
    }

    /* Shall we already perform the Exchange? */
    if (wOption & PH_EXCHANGE_BUFFERED_BIT)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
    }

    /* Set the corresponding logical channel in the CLA Byte */
    pDataParams->pTxBuffer[wTxStartPosTmp + PHHAL_HW_SAMAV2_ISO7816_CLA_POS] |= pDataParams->bLogicalChannel;

    /* Retrieve some command information */
    bSamCommand = pDataParams->pTxBuffer[wTxStartPosTmp + PHHAL_HW_SAMAV2_ISO7816_INS_POS];
    bP1 = pDataParams->pTxBuffer[wTxStartPosTmp + PHHAL_HW_SAMAV2_ISO7816_P1_POS];
    bP2 = pDataParams->pTxBuffer[wTxStartPosTmp + PHHAL_HW_SAMAV2_ISO7816_P2_POS];

    /* AV1 Mode */
    if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        /* Retrieve ENC / MAC status */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_GetCmdProtection(
            pDataParams,
            bSamCommand,
            bP1,
            bP2,
            PH_OFF,
            &bUseEncryption,
            &bUseMac));

        /* Encrypt the data stream if neccessary */
        if (bUseEncryption)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_Encrypt(
                pDataParams,
                &pDataParams->pTxBuffer[wTxStartPosTmp],
                wTxLengthTmp,
                wTmpBufferSize + wTxLengthTmp,
                &wTxLengthTmp));
        }

        /* MAC the data stream if neccessary */
        if (bUseMac)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_CalculateMac(
                pDataParams, 
                &pDataParams->pTxBuffer[wTxStartPosTmp],
                wTxLengthTmp));
        }
    }
    /* AV2 Mode */
    else if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_GetFirstLastCommand(
            pDataParams, 
            bSamCommand, 
            bP1,
            bP2,
            &bFirstCmd,
            &bLast));

        if (pDataParams->bCmdSM & PHHAL_HW_SAMAV2_HC_AV2_ENC)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_Encrypt(
                pDataParams, 
                &pDataParams->pTxBuffer[wTxStartPosTmp],
                wTxLengthTmp,
                wTmpBufferSize + wTxLengthTmp,
                &wTxLengthTmp,
                bFirstCmd,
                bLast));
        }

        if (pDataParams->bCmdSM & PHHAL_HW_SAMAV2_HC_AV2_MAC)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_AppendMac(
                pDataParams, 
                &pDataParams->pTxBuffer[wTxStartPosTmp],
                wTxLengthTmp,
                wTmpBufferSize + wTxLengthTmp,
                &wTxLengthTmp,
                bFirstCmd,
                bLast));
        }

        if (bFirstCmd)
        {
            /* Increment CmdCtr */
            switch (bSamCommand)
            {
            case PHHAL_HW_SAMAV2_CMD_AUTHENTICATE_HOST_INS:
            case PHHAL_HW_SAMAV2_CMD_LOCK_UNLOCK_INS:
                break;
            default:
                /* Increment the command counter in case we have a first response */
                ++(pDataParams->Cmd_Ctr);
                break;
            }
        }
    }
    /* Undefined mode */
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    /* Exchange and Decipher command can use default buffers */
    if ((bSamCommand == PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS) ||
        (bSamCommand == PHHAL_HW_SAMAV2_CMD_DECIPHER_DATA_INS))
    {
        *ppRxBuffer = &pDataParams->pRxBuffer[pDataParams->wRxBufStartPos];
        wTmpBufferSize = pDataParams->wRxBufSize - pDataParams->wRxBufStartPos;
    }
    /* Other commands -> Preserve all buffer data */
    else
    {
        /* TxBuffer equals RxBuffer */
        if (pDataParams->pTxBuffer == pDataParams->pRxBuffer)
        {
            /* Start after TxBuffer contents */
            if (pDataParams->wTxBufLen > pDataParams->wRxBufLen)
            {
                *ppRxBuffer = &pDataParams->pTxBuffer[pDataParams->wTxBufStartPos + pDataParams->wTxBufLen];
                wTmpBufferSize = pDataParams->wTxBufSize -(pDataParams->wTxBufStartPos + pDataParams->wTxBufLen);
            }
            /* Start after RxBuffer contents */
            else
            {
                *ppRxBuffer = &pDataParams->pRxBuffer[pDataParams->wRxBufLen];
                wTmpBufferSize = pDataParams->wRxBufSize - pDataParams->wRxBufLen;
            }
        }
        /* Buffers are different */
        else
        {
            /* Use TxBuffer if it has more space */
            if ((pDataParams->wTxBufSize - pDataParams->wTxBufLen) >(pDataParams->wRxBufSize - pDataParams->wRxBufLen))
            {
                *ppRxBuffer = &pDataParams->pTxBuffer[pDataParams->wTxBufStartPos + pDataParams->wTxBufLen];
                wTmpBufferSize = pDataParams->wTxBufSize -(pDataParams->wTxBufStartPos + pDataParams->wTxBufLen);
            }
            /* Else use RxBuffer */
            else
            {
                *ppRxBuffer = &pDataParams->pRxBuffer[pDataParams->wRxBufLen];
                wTmpBufferSize = pDataParams->wRxBufSize - pDataParams->wRxBufLen;
            }
        }
    }

    /* Perform command exchange */
    status = phbalReg_Exchange(
        pDataParams->pBalDataParams,
        PH_EXCHANGE_DEFAULT,
        &pDataParams->pTxBuffer[wTxStartPosTmp],
        wTxLengthTmp,
        wTmpBufferSize,
        *ppRxBuffer,
        pRxLength);

    /* Reset TxBufferLength */
    if (bSamCommand == PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS)
    {
        pDataParams->wTxBufLen = 0;
    }
    else
    {
        pDataParams->wTxBufLen_Cmd = 0;
    }

    /* Success check */
    PH_CHECK_SUCCESS(status);

    /* We need at least 2 bytes in the answer */
    if (*pRxLength < PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH)
    {
        pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
        pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
        pDataParams->bCommandChaining = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;
        pDataParams->bResponseChaining = PHHAL_HW_SAMAV2_HC_AV2_NO_CHAINING;

        /* Remapping of return values */
        return PH_ADD_COMPCODE(PH_ERR_LENGTH_ERROR, PH_COMP_HAL);
    }

    /* AV1 Mode */
    if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV1_MODE)
    {
        /* Retrieve ENC / MAC status */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_GetCmdProtection(
            pDataParams,
            bSamCommand,
            bP1,
            bP2,
            PH_ON,
            &bUseEncryption,
            &bUseMac));

        /* Verify MAC if neccessary */
        if (bUseMac)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_VerifyRemoveMac(
                pDataParams,
                *ppRxBuffer,
                *pRxLength,
                pRxLength));
        }

        /* Decrypt the response if neccessary */
        if (bUseEncryption)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV1_Int_Decrypt(
                pDataParams,
                *ppRxBuffer,
                *pRxLength,
                pRxLength));
        }
    }
    /* AV2 Mode */
    else if (pDataParams->bHostMode == PHHAL_HW_SAMAV2_HC_AV2_MODE)
    {
        /* catch host protocol error */
        if ((*pRxLength == 2) &&((*ppRxBuffer)[0] == 0x6A) &&((*ppRxBuffer)[1] == 0x84))
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey(pDataParams->pENCCryptoDataParams));
            PH_CHECK_SUCCESS_FCT(statusTmp, phCryptoSym_InvalidateKey(pDataParams->pMACCryptoDataParams));
            pDataParams->bCmdSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
            pDataParams->bRespSM = PHHAL_HW_SAMAV2_HC_AV2_NO_SM;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_GetFirstLastResponse(
            pDataParams,
            (*ppRxBuffer)[*pRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH],
            (*ppRxBuffer)[*pRxLength -(PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH-1)],
            &bFirstResp,
            &bLast));

        if (pDataParams->bRespSM & PHHAL_HW_SAMAV2_HC_AV2_MAC)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_VerifyRemoveMac(
                pDataParams, 
                *ppRxBuffer,                     
                *pRxLength,                    
                pRxLength,
                bFirstResp,
                bLast));
        }

        if (pDataParams->bRespSM & PHHAL_HW_SAMAV2_HC_AV2_ENC)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Hc_AV2_Int_Decrypt(
                pDataParams, 
                *ppRxBuffer,
                *pRxLength, 
                pRxLength,
                bFirstResp,
                bLast));
        }
    }
    /* Undefined mode */
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    /* Check the return code */
    *pRxLength = *pRxLength - PHHAL_HW_SAMAV2_ISO7816_SW1SW2_LENGTH;
    status = phhalHw_SamAV2_HcUtils_Cmd_ResolveErrorCode(&((*ppRxBuffer)[*pRxLength]), PH_COMP_HAL);

    /* Always return complete buffer on exchange */
    if (bSamCommand == PHHAL_HW_SAMAV2_CMD_TRANSPARENT_EXCHANGE_INS)
    {
        *ppRxBuffer = pDataParams->pRxBuffer;
        *pRxLength = *pRxLength + pDataParams->wRxBufStartPos;
        pDataParams->wRxBufLen = *pRxLength;
    }

    /* Special handling for certain status codes */
    switch ((status & PH_ERR_MASK))
    {
    case PH_ERR_SUCCESS_INCOMPLETE_BYTE:
        /* Retrieve number of bits from second byte of status code */
        pDataParams->wAdditionalInfo =((*ppRxBuffer)[*pRxLength + 1]);
        break;
    case PHHAL_HW_SAMAV2_ERR_DESFIRE_GEN:
        /* Retrieve return code from card from first data byte */
        if (*pRxLength == 1)
        {
            pDataParams->wAdditionalInfo = (*ppRxBuffer)[0];
        }
        else
        {
            if (*pRxLength == 2)
            {
                pDataParams->wAdditionalInfo = ((uint16_t)((*ppRxBuffer)[0]) << 8 ) | (uint16_t)(*ppRxBuffer)[1];
            }
        }
        break;
    default:
        break;
    }

    return status;
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
