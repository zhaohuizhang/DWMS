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
* Rc523 communication functions for SamAV2 in X-Mode configuration.
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
#include <phhalHw_SamAV2_Rc523_Reg.h>

phStatus_t phhalHw_SamAV2_Rc523_SetConfig(
    phhalHw_SamAV2_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bValue;
    uint8_t     PH_MEMLOC_REM bModWidthReg;
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

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_MFRX, &bRegister));

        if (wValue == PH_OFF)
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC523_BIT_PARITYDISABLE;
        }
        else
        {
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_BIT_PARITYDISABLE;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_MFRX, bValue));
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

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TXMODE, &bRegister));

        if (wValue == PH_OFF)
        {
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_BIT_CRCEN;
        }
        else
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC523_BIT_CRCEN;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TXMODE, bValue));
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

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_RXMODE, &bRegister));

        if (wValue == PH_OFF)
        {
            bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_BIT_CRCEN;
        }
        else
        {
            bValue = bRegister | PHHAL_HW_SAMAV2_RC523_BIT_CRCEN;
        }

        /* Only perform the operation, if the new value is different */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_RXMODE, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXLASTBITS:

        /* check parameter */
        if (wValue > PHHAL_HW_SAMAV2_RC523_MASK_TXBITS)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_BITFRAMING, &bRegister));
        bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_MASK_TXBITS;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_BITFRAMING, bRegister | (uint8_t)wValue));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXALIGN:

        /* check parameter */
        if (wValue > 7)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* adjust parameter */
        wValue = (uint16_t)(wValue << 4);

        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_BITFRAMING, &bRegister));
        bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_MASK_RXALIGN;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_BITFRAMING, bRegister |  (uint8_t)wValue));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXDEAFBITS:

        /* check parameter */
        if (wValue > PHHAL_HW_SAMAV2_RC523_MASK_RXWAIT)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* configure the Rx Deaf Time in bits */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_RXSEL, &bRegister));
        bRegister &= (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_MASK_RXWAIT;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_RXSEL, bRegister |  (uint8_t)wValue));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_CLEARBITSAFTERCOLL:

        /* Modify bit */
        if (wValue == PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COLL, PHHAL_HW_SAMAV2_RC523_BIT_VALUESAFTERCOLL));
        }
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COLL, 0x00));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TXDATARATE:

        /* Retrieve TxMode register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TXMODE, &bRegister));
        bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_MASK_SPEED;

        /* update current data rate */
        switch (wValue)
        {
        case PHHAL_HW_RF_DATARATE_106:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_106KBPS;
            bModWidthReg = 0x26;
            break;
        case PHHAL_HW_RF_DATARATE_212:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_212KBPS;
            bModWidthReg = 0x15;
            break;
        case PHHAL_HW_RF_DATARATE_424:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_424KBPS;
            bModWidthReg = 0x0A;
            break;
        case PHHAL_HW_RF_DATARATE_848:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_848KBPS;
            bModWidthReg = 0x05;
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Only apply the settings if the speed has changed */
        if (bValue != bRegister)
        {
            /* Apply data rate */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TXMODE, bValue));

            /* Configure the modulation width for ISO 14443-3A */
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_MODWIDTH, bModWidthReg));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_RXDATARATE:

        /* Retrieve RxMode register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_RXMODE, &bRegister));
        bValue = bRegister & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_MASK_SPEED;

        /* update current data rate */
        switch (wValue)
        {
        case PHHAL_HW_RF_DATARATE_106:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_106KBPS;
            break;
        case PHHAL_HW_RF_DATARATE_212:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_212KBPS;
            break;
        case PHHAL_HW_RF_DATARATE_424:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_424KBPS;
            break;
        case PHHAL_HW_RF_DATARATE_848:
            bValue |= PHHAL_HW_SAMAV2_RC523_BIT_848KBPS;
            break;
        default:
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Apply data rate */
        if (bValue != bRegister)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_RXMODE, bValue));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_MODINDEX:

        if (wValue & (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_MASK_MODGSP)
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Set ModGsP register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_MODGSP, (uint8_t)wValue));

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_ASK100:

        /* switch off 100% ASK */
        if (wValue == PH_OFF)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TXASK, 0x00));
        }
        /* switch on 100% ASK */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TXASK, PHHAL_HW_SAMAV2_RC523_BIT_FORCE100ASK));
        }

        /* Write config data into shadow */
        pDataParams->wCfgShadow[wConfig] = wValue;
        break;

    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_US:
    case PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS:

        /* Calculate values for Microsecond values */
        if (wConfig == PHHAL_HW_CONFIG_TIMEOUT_VALUE_US)
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(pDataParams, PHHAL_HW_TIME_MICROSECONDS, wValue));
            pDataParams->bTimeoutUnit = PHHAL_HW_TIME_MICROSECONDS;
        }
        /* Calculate values for Millisecond values */
        else
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(pDataParams, PHHAL_HW_TIME_MILLISECONDS, wValue));
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

phStatus_t phhalHw_SamAV2_Rc523_Wait(
                                     phhalHw_SamAV2_DataParams_t * pDataParams,
                                     uint8_t bUnit,
                                     uint16_t wTimeout
                                     )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint8_t		PH_MEMLOC_REM bIrqRq;
    uint16_t    PH_MEMLOC_REM wTimerShift;
    uint16_t    PH_MEMLOC_REM wTimeoutNew;

    /* Parameter check */
    if ((bUnit != PHHAL_HW_TIME_MICROSECONDS) && (bUnit != PHHAL_HW_TIME_MILLISECONDS))
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
    }

    /* Terminate a probably running command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COMMAND, PHHAL_HW_SAMAV2_RC523_CMD_IDLE));

    /* Retrieve RxDataRate */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams, PHHAL_HW_CONFIG_RXDATARATE, &wTimerShift));
    ++wTimerShift;

    /* Calculate timershift */
    wTimerShift = (uint16_t)(PHHAL_HW_SAMAV2_RC523_TIMER_SHIFT * (PHHAL_HW_SAMAV2_ETU_106 / (float32_t)wTimerShift));

    /* do as long as we have an overflow in the IC timer */
    do
    {
        /* Set temporary timeout */
        if (bUnit == PHHAL_HW_TIME_MICROSECONDS)
        {
            wTimeoutNew = (wTimeout > wTimerShift) ? (wTimeout - wTimerShift) : 0;
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(pDataParams, PHHAL_HW_TIME_MICROSECONDS, wTimeoutNew));
            wTimeout = 0;
        }
        else
        {
            /* Timer would overflow -> use maximum value and decrement overall value for next iteration */
            if (wTimeout > ((PHHAL_HW_SAMAV2_RC523_TIMER_MAX_VALUE_US / 1000) - 1))
            {
                wTimeoutNew = (PHHAL_HW_SAMAV2_RC523_TIMER_MAX_VALUE_US / 1000) - 1;
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(pDataParams, PHHAL_HW_TIME_MILLISECONDS, wTimeoutNew));
                wTimeout = wTimeout - wTimeoutNew;
            }
            /* No overflow -> set the complete time */
            else
            {
                PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(pDataParams, PHHAL_HW_TIME_MILLISECONDS, wTimeout));
                wTimeout = 0;
            }
        }

        /* retrieve content of Control register */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_CONTROL, &bRegister));

        /* enable timer interrupt */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COMMIEN, PHHAL_HW_SAMAV2_RC523_BIT_TIMERI));

        /* clear all irq flags */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COMMIRQ, (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_BIT_SET));

        /* start timer */
        bRegister |= PHHAL_HW_SAMAV2_RC523_BIT_TSTARTNOW;
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_CONTROL, bRegister));

        /* wait for timer interrupt */
        do
        {
            PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COMMIRQ, &bIrqRq));
        }
        while (!(bIrqRq & (PHHAL_HW_SAMAV2_RC523_BIT_ERRI | PHHAL_HW_SAMAV2_RC523_BIT_TIMERI)));

        /* clear all irq flags */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_COMMIRQ, (uint8_t)~(uint8_t)PHHAL_HW_SAMAV2_RC523_BIT_SET));
    }
    while (wTimeout > 0);

    /* Restore previous timeout */
    if (pDataParams->bTimeoutUnit == PHHAL_HW_TIME_MICROSECONDS)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(
            pDataParams,
            PHHAL_HW_TIME_MICROSECONDS,
            pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_US]));
    }
    else
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_SamAV2_Rc523_SetFdt(
            pDataParams,
            PHHAL_HW_TIME_MILLISECONDS,
            pDataParams->wCfgShadow[PHHAL_HW_CONFIG_TIMEOUT_VALUE_MS]));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}
phStatus_t phhalHw_SamAV2_Rc523_SetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,
                                       uint8_t bUnit,
                                       uint16_t wTimeout
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    float32_t   PH_MEMLOC_REM fTimeoutUs;
    float32_t   PH_MEMLOC_REM fTemp;
    uint8_t     PH_MEMLOC_REM bTPrescaler;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint16_t    PH_MEMLOC_REM wDataRate;
    uint16_t    PH_MEMLOC_REM wPrescaler;
    uint16_t    PH_MEMLOC_REM wReload;

    /* Retrieve RxDataRate */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams, PHHAL_HW_CONFIG_RXDATARATE, &wDataRate));
    wDataRate++;

    /* Handle timeout in microseconds */
    if (bUnit == PHHAL_HW_TIME_MICROSECONDS)
    {
        fTimeoutUs = (float32_t)wTimeout;
    }
    else
    {
        fTimeoutUs = (float32_t)wTimeout * 1000.0f;
    }

    /* Add timershift to timeout value */
    fTimeoutUs = fTimeoutUs + (PHHAL_HW_SAMAV2_RC523_TIMER_SHIFT * (PHHAL_HW_SAMAV2_ETU_106 / (float32_t)wDataRate));

    /* Max. timeout check */
    if (fTimeoutUs > PHHAL_HW_SAMAV2_RC523_TIMER_MAX_VALUE_US)
    {
        return PH_ADD_COMPCODE(PH_ERR_PARAMETER_OVERFLOW, PH_COMP_HAL);
    }

    /* Read Prescaler value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TPRESCALER, &bTPrescaler));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TMODE, &bRegister));

    /* Get prescaler value */
    wPrescaler = ((uint16_t)(bRegister & PHHAL_HW_SAMAV2_RC523_MASK_TPRESCALER_HI) << 8) | bTPrescaler;

    /* Calculate the reload value */
    fTemp = (fTimeoutUs * PHHAL_HW_SAMAV2_TIMER_FREQ) / (1.0f + (2.0f * (float32_t)wPrescaler));

    /* Except for a special case, the float32_t value will have some commas
    and therefore needs to be "ceiled" */
    if (fTemp > (float32_t)(uint16_t)fTemp)
    {
        wReload = (uint16_t)(fTemp + 1);
    }
    else
    {
        wReload = (uint16_t)fTemp;
    }

    /* write reload value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TRELOADLO, (uint8_t)(wReload & 0xFF)));
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TRELOADHI, (uint8_t)((wReload >> 8) & 0xFF)));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_SamAV2_Rc523_GetFdt(
                                       phhalHw_SamAV2_DataParams_t * pDataParams,
                                       uint32_t * pTime
                                       )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegister;
    uint16_t    PH_MEMLOC_REM wDataRate;
    uint16_t    PH_MEMLOC_REM wPsReload;
    uint16_t    PH_MEMLOC_REM wTReload;
    uint16_t    PH_MEMLOC_REM wTValue;
    float32_t   PH_MEMLOC_REM fTimeout;

    /* Retrieve RxDataRate */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_GetConfig(pDataParams, PHHAL_HW_CONFIG_RXDATARATE, &wDataRate));
    wDataRate++;

    /* Get prescaler value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TMODE, &bRegister));
    wPsReload = (uint16_t)(bRegister & PHHAL_HW_SAMAV2_RC523_MASK_TPRESCALER_HI) << 8;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TPRESCALER, &bRegister));
    wPsReload |= (uint16_t)bRegister;

    /* Get reload value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TRELOADHI, &bRegister));
    wTReload = (uint16_t)bRegister << 8;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TRELOADLO, &bRegister));
    wTReload |= (uint16_t)bRegister;

    /* Get counter value */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TCOUNTERVALHI, &bRegister));
    wTValue = (uint16_t)bRegister << 8;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_SAMAV2_RC523_REG_TCOUNTERVALLO, &bRegister));
    wTValue |= (uint16_t)bRegister;

    /* Subtract reload- and counter values */
    wTValue = wTReload - wTValue;

    /* Calculate the timeout value */
    fTimeout = ((float32_t)wTValue * (1.0f + (2.0f * (float32_t)wPsReload))) / PHHAL_HW_SAMAV2_TIMER_FREQ;

    /* Subtract timershift from timeout value */
    fTimeout = fTimeout - (PHHAL_HW_SAMAV2_RC523_TIMER_SHIFT * (PHHAL_HW_SAMAV2_ETU_106 / (float32_t)wDataRate));

    /* Return the value */
    *pTime = (uint32_t)fTimeout;

    /* Round the value if neccessary */
    if ((fTimeout - (float32_t)*pTime) >= 0.5)
    {
        ++(*pTime);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_SAMAV2 */
