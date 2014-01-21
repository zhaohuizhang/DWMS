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
* Rc663 communication functions for SamAV2 in X-Mode configuration.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:09 2011 $
*
* History:
*  CHu: Generated 10. January 2011
*
*/
#include <phhalHw.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_SAMAV2

#include "phhalHw_SamAV2_Int.h"
#include "phhalHw_SamAV2_Rc663_Config.h"
#include <phhalHw_SamAV2_Rc663_Reg.h>

/** Register Settings to return */
static const uint8_t PH_MEMLOC_CONST_ROM gkphhalHw_SamAV2_Rc663_Registers[] =
{
    PHHAL_HW_RC663_REG_DRVMODE,
    PHHAL_HW_RC663_REG_TXAMP,
    PHHAL_HW_RC663_REG_DRVCON,
    PHHAL_HW_RC663_REG_TXI,
    PHHAL_HW_RC663_REG_TXCRCCON,
    PHHAL_HW_RC663_REG_RXCRCCON,
    PHHAL_HW_RC663_REG_TXDATANUM,
    PHHAL_HW_RC663_REG_TXMODWIDTH,
    PHHAL_HW_RC663_REG_TXSYM10BURSTLEN,
    PHHAL_HW_RC663_REG_TXWAITCTRL,
    PHHAL_HW_RC663_REG_FRAMECON,
    PHHAL_HW_RC663_REG_RXCTRL,
    PHHAL_HW_RC663_REG_RXTHRESHOLD,
    PHHAL_HW_RC663_REG_RCV,
    PHHAL_HW_RC663_REG_RXANA
};

/** Register Settings for ISO14443A @ 106kbit/s */
static const uint8_t PH_MEMLOC_CONST_ROM gkphhalHw_SamAV2_Rc663_Reg_106[] =
{
    0x8E,
    0x15,
    0x11,
    0x0A,
    0x18,
    0x18,
    0x0F,
    0x27,
    0x00,
    0xC0,
    0xCF,
    0x04,
    PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_106,
    0x12,
    0x0A
};

/** Register Settings for ISO14443A @ 212kbit/s */
static const uint8_t PH_MEMLOC_CONST_ROM gkphhalHw_SamAV2_Rc663_Reg_212[] =
{
    0x8E,
    0x12,
    0x11,
    0x0A,
    0x18,
    0x18,
    0x0F,
    0x10,
    0x00,
    0xC0,
    0xCF,
    0x05,
    PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_212,
    0x12,
    0x02
};

/** Register Settings for ISO14443A @ 424kbit/s */
static const uint8_t PH_MEMLOC_CONST_ROM gkphhalHw_SamAV2_Rc663_Reg_424[] =
{
    0x8E,
    0x12,
    0x11,
    0x0A,
    0x18,
    0x18,
    0x0F,
    0x08,
    0x00,
    0xC0,
    0xCF,
    0x06,
    PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_424,
    0x12,
    0x02
};

/** Register Settings for ISO14443A @ 848kbit/s */
static const uint8_t PH_MEMLOC_CONST_ROM gkphhalHw_SamAV2_Rc663_Reg_848[] =
{
    0x8F,
    0xDB,
    0x11,
    0x0A,
    0x18,
    0x18,
    0x0F,
    0x02,
    0x00,
    0xC0,
    0xCF,
    0x07,
    PHHAL_HW_SAMAV2_RC663_RXTHRESHOLD_I14443A_848,
    0x12,
    0x02
};

/* Digital delay for all protocols and speeds in microseconds */
static const uint16_t PH_MEMLOC_CONST_ROM gkphhalHw_SamAV2_Rc663_DigiDelays_Us[] = {/* 106KBaud */ 17, /* 212KBaud */ 49, /* 424KBaud */ 45, /* 848KBaud */ 42};

phStatus_t phhalHw_SamAV2_Rc663_SetCardMode(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wTxDataRate,
    uint16_t wRxDataRate
    )
{
    phStatus_t      PH_MEMLOC_REM statusTmp;
    uint16_t        PH_MEMLOC_COUNT wIndex;
    uint8_t         PH_MEMLOC_REM bTxConfig;
    uint8_t         PH_MEMLOC_REM bRxConfig;
    const uint8_t * PH_MEMLOC_REM pTxRegisterSet;
    const uint8_t * PH_MEMLOC_REM pRxRegisterSet;
    uint8_t         PH_MEMLOC_REM bRegister;

    /* No RegisterSet by default */
    pTxRegisterSet = NULL;

    switch (pDataParams->bCardType)
    {
    case PHHAL_HW_CARDTYPE_ISO14443A:

        switch (wTxDataRate)
        {
        case PHHAL_HW_RF_DATARATE_106:
            bTxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_106;
            pTxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_106;
            break;
        case PHHAL_HW_RF_DATARATE_212:
            bTxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_212;
            pTxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_212;
            break;
        case PHHAL_HW_RF_DATARATE_424:
            bTxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_424;
            pTxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_424;
            break;
        case PHHAL_HW_RF_DATARATE_848:
            bTxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_848;
            pTxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_848;
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        switch (wRxDataRate)
        {
        case PHHAL_HW_RF_DATARATE_106:
            bRxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_106;
            pRxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_106;
            break;
        case PHHAL_HW_RF_DATARATE_212:
            bRxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_212;
            pRxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_212;
            break;
        case PHHAL_HW_RF_DATARATE_424:
            bRxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_424;
            pRxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_424;
            break;
        case PHHAL_HW_RF_DATARATE_848:
            bRxConfig = PHHAL_HW_SAMAV2_RC663_RXTX_I14443A_848;
            pRxRegisterSet = (const uint8_t*)gkphhalHw_SamAV2_Rc663_Reg_848;
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
        break;

    default:

        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    /* Perform LoadProtocol */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_FIFODATA, bRxConfig));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_FIFODATA, bTxConfig));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_COMMAND, PHHAL_HW_SAMAV2_RC663_CMD_LOADPROTOCOL));
    do
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_IRQ0, &bRegister));
    }
    while (!(bRegister & PHHAL_HW_RC663_BIT_IDLEIRQ));

    /* Apply RegisterSet using library register set */
    for (wIndex = 0; wIndex < sizeof(gkphhalHw_SamAV2_Rc663_Registers); ++wIndex)
    {
        /* TX register set is valid */
        if (pTxRegisterSet)
        {
            /* Initially take everything from TX register set */
            bRegister = pTxRegisterSet[wIndex];

            /* RX register set is also valid */
            if (pRxRegisterSet)
            {
                switch (gkphhalHw_SamAV2_Rc663_Registers[wIndex])
                {
                    /* Take the following registers from the RX register set */
                case PHHAL_HW_RC663_REG_RXCRCCON:
                case PHHAL_HW_RC663_REG_RXSOFD:
                case PHHAL_HW_RC663_REG_RXCTRL:
                case PHHAL_HW_RC663_REG_RXWAIT:
                case PHHAL_HW_RC663_REG_RXTHRESHOLD:
                case PHHAL_HW_RC663_REG_RCV:
                case PHHAL_HW_RC663_REG_RXANA:
                    bRegister = pRxRegisterSet[wIndex];
                    break;
                default:
                    break;
                }
            }
        }
        /* ONLY RX register set is valid */
        else if (pRxRegisterSet)
        {
            /* Take everything from RX register set */
            bRegister = pRxRegisterSet[wIndex];
        }
        /* At least one register set is mandatory */
        else
        {
            return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
        }

        /* Preserve TxEn bit in DrvMode register */
        if (gkphhalHw_SamAV2_Rc663_Registers[wIndex] == PHHAL_HW_RC663_REG_DRVMODE)
        {
            /* Read out DrvMode register */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_RC663_REG_DRVMODE, &bTxConfig));

            /* Preserve TxEn bit, take other bits from register set */
            bRegister = (bTxConfig & PHHAL_HW_RC663_BIT_TXEN) | (bRegister & (uint8_t)~(uint8_t)PHHAL_HW_RC663_BIT_TXEN);
        }

        /* Actual write to register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(
            pDataParams,
            gkphhalHw_SamAV2_Rc663_Registers[wIndex],
            bRegister));
    }

    /* Restore ASK 100 Setting */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_ASK100, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_ASK100]));

    /* A change in the bitrate also affects TxWait */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetTxWait(pDataParams, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXWAIT_US]));

    /* Update Datarate in shadow for parity setting */
    pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE] = wTxDataRate;
    pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE] = wRxDataRate;

    /* Apply Parity settings from Shadow */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_PARITY, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_PARITY]));

    /* Apply Tx-CRC settings from Shadow */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXCRC, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXCRC]));

    /* Apply Rx-CRC settings from Shadow */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_RXCRC, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXCRC]));

    /* Apply TxLastBits settings from Shadow */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetConfig(pDataParams, PHHAL_HW_CONFIG_TXLASTBITS, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXLASTBITS]));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc663_SetConfig(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bValue;
    uint8_t     PH_MEMLOC_REM bRegister;

    switch (wConfig)
    {
    case PHHAL_HW_CONFIG_PARITY:

        /* Not Available with enabled MIFARE Crypto */
        if (!(pDataParams->bMifareCryptoDisabled))
        {
            if (wValue == PH_OFF)
            {
                pDataParams->wCfgShadow[wConfig] = PH_OFF;
            }
            else
            {
                pDataParams->wCfgShadow[wConfig] = PH_ON;
            }
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_FRAMECON, &bRegister));

        /* Parity is OFF */
        if (wValue == PH_OFF)
        {
            bValue = bRegister & (uint8_t)~(uint8_t)(PHHAL_HW_SAMAV2_RC663_BIT_TXPARITYEN | PHHAL_HW_SAMAV2_RC663_BIT_RXPARITYEN);
        }
        /* Parity is ON */
        else
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC663_BIT_TXPARITYEN | PHHAL_HW_SAMAV2_RC663_BIT_RXPARITYEN;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_FRAMECON, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXCRC:

        /* Not Available with enabled MIFARE Crypto */
        if (!(pDataParams->bMifareCryptoDisabled))
        {
            if (wValue == PH_OFF)
            {
                pDataParams->wCfgShadow[wConfig] = PH_OFF;
            }
            else
            {
                pDataParams->wCfgShadow[wConfig] = PH_ON;
            }
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXCRCCON, &bRegister));

        if (wValue == PH_OFF)
        {
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_BIT_CRCEN;
        }
        else
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC663_BIT_CRCEN;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXCRCCON, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXCRC:

        /* Not Available with enabled MIFARE Crypto */
        if (!(pDataParams->bMifareCryptoDisabled))
        {
            if (wValue == PH_OFF)
            {
                pDataParams->wCfgShadow[wConfig] = PH_OFF;
            }
            else
            {
                pDataParams->wCfgShadow[wConfig] = PH_ON;
            }
            break;
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXCRCCON, &bRegister));

        if (wValue == PH_OFF)
        {
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_BIT_CRCEN;
        }
        else
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC663_BIT_CRCEN;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXCRCCON, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXLASTBITS:

        /* check parameter */
        if (wValue > PHHAL_HW_SAMAV2_RC663_MASK_TXLASTBITS)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXDATANUM, &bRegister));
        bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_MASK_TXLASTBITS;
        bRegister |= (uint8_t)wValue;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXDATANUM, bRegister));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXALIGN:

        /* check parameter */
        if (wValue > (PHHAL_HW_SAMAV2_RC663_MASK_RXALIGN >> 4))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXBITCTRL, &bRegister));
        bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_MASK_RXALIGN;
        bValue |= (uint8_t)(wValue << 4);

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXBITCTRL, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXDEAFBITS:

        /* Set RxWait */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetRxWait(pDataParams, wValue));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXWAIT_US:

        /* Set TxWait */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetTxWait(pDataParams, wValue));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_CLEARBITSAFTERCOLL:

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXBITCTRL, &bRegister));

        /* Disable */
        if (wValue == PH_OFF)
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC663_BIT_VALUESAFTERCOLL;
        }
        /* Enable */
        else
        {
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_BIT_VALUESAFTERCOLL;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXBITCTRL, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXDATARATE:

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetCardMode(pDataParams, wConfig, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE]));
        break;

    case PHHAL_HW_CONFIG_RXDATARATE:

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetCardMode(pDataParams, pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE], wConfig));
        break;

    case PHHAL_HW_CONFIG_MODINDEX:

        /* Parameter check */
        if (wValue > PHHAL_HW_SAMAV2_RC663_MASK_RESIDUAL_CARRIER)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Read out TxAmp register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXAMP, &bRegister));

        /* Set modulation index value */
        bValue = (bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_MASK_RESIDUAL_CARRIER) | (uint8_t)wValue;

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXAMP, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_ASK100:

        /* Read out DrvMod register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_DRVMODE, &bRegister));

        /* Clear ClkMode bits */
        bValue = bRegister & (uint8_t)~(uint8_t)(PHHAL_HW_SAMAV2_RC663_BIT_RFON | PHHAL_HW_SAMAV2_RC663_BIT_TPUSHON | PHHAL_HW_SAMAV2_RC663_BIT_TPULLON);

        /* Update DrvMod register */
        if (wValue == PH_OFF)
        {
            bValue |= PHHAL_HW_SAMAV2_RC663_BIT_RFON | PHHAL_HW_SAMAV2_RC663_BIT_TPUSHON | PHHAL_HW_SAMAV2_RC663_BIT_TPULLON;
        }
        else
        {
            bValue |= 0x01;  /* 100%ASK: ClkMode is the pause. */
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_DRVMODE, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:
    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:

        /* Calculate values for Microsecond values */
        if (wConfig == PHHAL_HW_CONFIG_TIMEOUT_VALUE_US)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetFdt(pDataParams, PHHAL_HW_TIME_MICROSECONDS, wValue));
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MICROSECONDS;
        }
        /* Calculate values for Millisecond values */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetFdt(pDataParams, PHHAL_HW_TIME_MILLISECONDS, wValue));
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MILLISECONDS;
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    default:
        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_HAL);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc663_Wait(
                                     phhalHw_SamAV2_DataParams_t * pDataParams,
                                     uint8_t bUnit,
                                     uint16_t wTimeout
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bControlReg;
    uint8_t     PH_MEMLOC_REM bIrqRq;

    /* Parameter check */
    if ((bUnit != PHHAL_HW_TIME_MICROSECONDS) && (bUnit != PHHAL_HW_TIME_MILLISECONDS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Set temporary timeout */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetFdt(pDataParams, bUnit, wTimeout));

    /* retrieve content of Timer Control register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TCONTROL, &bControlReg));

    /* clear all irq flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_IRQ1, (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_BIT_SET));

    /* manually start T0 */
    bControlReg |= PHHAL_HW_SAMAV2_RC663_BIT_T0STARTSTOPNOW | PHHAL_HW_SAMAV2_RC663_BIT_T0RUNNING;
    /* manually start T1 */
    bControlReg |= PHHAL_HW_SAMAV2_RC663_BIT_T1STARTSTOPNOW | PHHAL_HW_SAMAV2_RC663_BIT_T1RUNNING;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TCONTROL, bControlReg));

    /* wait for timer interrupt */
    do
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_IRQ1, &bIrqRq));
    }
    while (!(bIrqRq & PHHAL_HW_SAMAV2_RC663_BIT_TIMER1IRQ));

    /* manually stop T0 */
    bControlReg &= (uint8_t)~(uint8_t)(PHHAL_HW_SAMAV2_RC663_BIT_T0RUNNING | PHHAL_HW_SAMAV2_RC663_BIT_T1RUNNING);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TCONTROL, bControlReg));

    /* Restore previous timeout */
    if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetFdt(
            pDataParams,
            PHHAL_HW_TIME_MICROSECONDS,
            pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_US]));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_SetFdt(
            pDataParams,
            PHHAL_HW_TIME_MILLISECONDS,
            pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS]));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}
phStatus_t phhalHw_SamAV2_Rc663_SetRxWait(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wTimeEtu
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;

    /* Since we use DBFREQ*2, we need to adjust the value for ETU granularity */

    /* DBFREQ*2 = 53KHz*2 = 106KHz = 1/2 ETU */
    /* parameter check */
    if (wTimeEtu > (PHHAL_HW_SAMAV2_RC663_MASK_RXWAIT >> 1))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }
    wTimeEtu <<= 1;

    /* set the RxWait register  */
    bRegister = PHHAL_HW_SAMAV2_RC663_BIT_RXWAITDBFREQ | (uint8_t)wTimeEtu;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_RXWAIT, bRegister));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc663_SetTxWait(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wTimeUs
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;
    float32_t   PH_MEMLOC_REM fTime;
    uint16_t    PH_MEMLOC_REM wDataRate;

    /* Retrieve Tx-Datarate */
    wDataRate = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TXDATARATE];

    /* Since we use DBFREQ*2, we need to adjust the value for ETU granularity */
    fTime = ((float32_t)wDataRate + 1.0f) * (PHHAL_HW_SAMAV2_ETU_106 / 2.0f);

    /* perform calculation [value] / [timer granularity] */
    fTime = (float32_t)wTimeUs / fTime;
    wTimeUs = (uint16_t)fTime;
    if ((float32_t)wTimeUs < fTime)
    {
        ++wTimeUs;
    }

    /* remove digital circutry delay */
    if (wTimeUs > PHHAL_HW_SAMAV2_RC663_DIGI_DELAY_TXWAIT_DBFREQ)
    {
        wTimeUs = wTimeUs - PHHAL_HW_SAMAV2_RC663_DIGI_DELAY_TXWAIT_DBFREQ;
    }
    else
    {
        wTimeUs = 0;
    }

    /* check parameter */
    if (wTimeUs > 0x7FF)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Read TxWaitControl register*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXWAITCTRL, &bRegister));

    /* Set TxWait configuration */
    bRegister |= PHHAL_HW_SAMAV2_RC663_BIT_TXWAIT_START_RX | PHHAL_HW_SAMAV2_RC663_BIT_TXWAIT_DBFREQ;

    /* Set the 3 MSBits */
    bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC663_MASK_TXWAITHI;
    bRegister |= (uint8_t)((wTimeUs >> 5) & PHHAL_HW_SAMAV2_RC663_MASK_TXWAITHI);

    /* Write changed register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXWAITCTRL, bRegister));

    /* Set the 8 LSBs */
    bRegister = (uint8_t)wTimeUs;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_TXWAITLO, bRegister));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc663_SetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bUnit,
                                       uint16_t wTimeout
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wT0Reload;
    uint16_t    PH_MEMLOC_REM wT1Reload;
    float32_t   PH_MEMLOC_REM fTime;
    uint16_t    PH_MEMLOC_REM wDelay;

    /* Calculate values for Microsecond values */
    if (bUnit == PHHAL_HW_TIME_MICROSECONDS)
    {
        /* Retrieve and add digital delay */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_GetDigiDelay(pDataParams, PH_ON, &wDelay));
        fTime = (float32_t)wTimeout + (float32_t)wDelay;

        /* Timeout can be realized with just one timer */
        if (!((fTime * PHHAL_HW_SAMAV2_TIMER_FREQ) > (float32_t)PHHAL_HW_SAMAV2_RC663_TIMER_RELOAD_MAX))
        {
            fTime = fTime * PHHAL_HW_SAMAV2_TIMER_FREQ;
            wT0Reload = (uint16_t)fTime;

            /* Since timer IRQ is raised at underflow, wReload has to decremented by one.
            However, if the resulting reload value has commas, it must be 'ceiled' */
            if (!(fTime > (float32_t)wT0Reload))
            {
                --wT0Reload;
            }

            /* No timer 1 */
            wT1Reload = 0x0000;
        }
        /* If not, set prescaler to 1 ETU */
        else
        {
            /* T0 Overflow occurs at exactly 1 ETU (from ISO14443 protocol @ 106) */
            wT0Reload = (uint16_t)(PHHAL_HW_SAMAV2_TIMER_FREQ * PHHAL_HW_SAMAV2_ETU_106);

            /* T1 reload is the given value divided by the prescaler T0 frequency */
            fTime = fTime / PHHAL_HW_SAMAV2_ETU_106;
            wT1Reload = (uint16_t)fTime;

            /* Since timer IRQ is raised at underflow, wReload has to decremented by one.
            However, if the resulting reload value has commas, it must be 'ceiled' */
            if (!(fTime > (float32_t)wT1Reload))
            {
                --wT1Reload;
            }
        }
    }
    /* Calculate values for Millisecond values */
    else
    {
        /* T0 overflow occurs every millisecond */
        wT0Reload = (uint16_t)(1000.0f * PHHAL_HW_SAMAV2_TIMER_FREQ);

        /* T1 reload matches exactly the given value */
        wT1Reload = ((wTimeout > 0) ? --wTimeout : wTimeout);
    }

    /* Write T0 reload value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T0RELOADHI, (uint8_t)(wT0Reload >> 8)));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T0RELOADLO, (uint8_t)(wT0Reload)));

    /* Write T1 reload value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T1RELOADHI, (uint8_t)(wT1Reload >> 8)));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T1RELOADLO, (uint8_t)(wT1Reload)));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc663_GetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,
                                       phStatus_t wExchangeStatus,
                                       uint32_t * pTime
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint16_t    PH_MEMLOC_REM wT0Reload;
    uint16_t    PH_MEMLOC_REM wT1Reload;
    uint16_t    PH_MEMLOC_REM wT0Value;
    uint16_t    PH_MEMLOC_REM wT1Value;
    float32_t   PH_MEMLOC_REM fTime;
    uint16_t    PH_MEMLOC_REM wDelay;

    /* Get T0 reload value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T0RELOADHI, &bRegister));
    wT0Reload = (uint16_t)bRegister << 8;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T0RELOADLO, &bRegister));
    wT0Reload |= (uint16_t)bRegister;

    /* Get T1 reload value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T1RELOADHI, &bRegister));
    wT1Reload = (uint16_t)bRegister << 8;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T1RELOADLO, &bRegister));
    wT1Reload |= (uint16_t)bRegister;

    /* Get T0 counter value */
    if ((wExchangeStatus & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T0COUNTERVALHI, &bRegister));
        wT0Value = (uint16_t)bRegister << 8;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T0COUNTERVALLO, &bRegister));
        wT0Value |= (uint16_t)bRegister;
    }
    else
    {
        wT0Value = 0;
    }

    /* Get T1 counter value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T1COUNTERVALHI, &bRegister));
    wT1Value = (uint16_t)bRegister << 8;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC663_REG_T1COUNTERVALLO, &bRegister));
    wT1Value |= (uint16_t)bRegister;

    /* Subtract reload- and counter values */
    wT0Value = wT0Reload - wT0Value;
    wT1Value = wT1Reload - wT1Value;

    /*
    Now we can calculate the time in microseconds according to the following formular:
    {
    time[us] = (wT1Value * (1 / (PHHAL_HW_SAMAV2_TIMER_FREQ / wT0Reload))) + (wT0Value * (1 / PHHAL_HW_SAMAV2_TIMER_FREQ))
    }
    */
    fTime = (float32_t)wT1Value / (PHHAL_HW_SAMAV2_TIMER_FREQ / (float32_t)wT0Reload);
    fTime = fTime + ((float32_t)wT0Value / PHHAL_HW_SAMAV2_TIMER_FREQ);

    /* Return the value */
    *pTime = (uint32_t)fTime;
    if ((float32_t)*pTime < fTime)
    {
        ++(*pTime);
    }

    /* Retrieve and subtract digital delay */
    if ((wExchangeStatus & PH_ERR_MASK) != PH_ERR_IO_TIMEOUT)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc663_GetDigiDelay(pDataParams, PH_OFF, &wDelay));
        *pTime = *pTime - wDelay;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc663_GetDigiDelay(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint8_t bIsTimeout,
    uint16_t * pDelayUs
    )
{
    uint16_t PH_MEMLOC_REM wDataRate;

    wDataRate = pDataParams->wCfgShadow[PHHAL_HW_CONFIG_RXDATARATE];

    *pDelayUs = gkphhalHw_SamAV2_Rc663_DigiDelays_Us[wDataRate];
    /* In a timeout case we have to add an additional delay */
    if (bIsTimeout != PH_OFF)
    {
        *pDelayUs = *pDelayUs + PHHAL_HW_SAMAV2_RC663_I14443_ADD_DELAY_US;
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
