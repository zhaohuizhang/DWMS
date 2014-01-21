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
* Internal functions of Software implementation of ICode SLI (R) application layer.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:49 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHALI15693_SW_INT_H
#define PHALI15693_SW_INT_H

#include <ph_Status.h>

#define PHAL_I15693_INFO_FLAGS_DSFID    0x01U   /**< Response flag DSFID. */
#define PHAL_I15693_INFO_FLAGS_AFI      0x02U   /**< Response flag AFI. */
#define PHAL_I15693_INFO_FLAGS_MEMSIZE  0x04U   /**< Response flag MEMSIZE. */
#define PHAL_I15693_INFO_FLAGS_ICREF    0x08U   /**< Response flag ICREF. */

/**
* \name Iso15693 Command Codes
*/
/*@{*/
#define PHAL_I15693_SW_CMD_READ_SINGLE_BLOCK        0x20    /**< ISO15693 read single block command. */
#define PHAL_I15693_SW_CMD_WRITE_SINGLE_BLOCK       0x21    /**< ISO15693 write single block command. */
#define PHAL_I15693_SW_CMD_LOCK_BLOCK               0x22    /**< ISO15693 lock block command. */
#define PHAL_I15693_SW_CMD_READ_MULTIPLE_BLOCKS     0x23    /**< ISO15693 read multiple blocks command. */
#define PHAL_I15693_SW_CMD_WRITE_MULTIPLE_BLOCKS    0x24    /**< ISO15693 write multiple blocks command. */
#define PHAL_I15693_SW_CMD_WRITE_AFI                0x27    /**< ISO15693 write Application Family Identifier command. */
#define PHAL_I15693_SW_CMD_LOCK_AFI                 0x28    /**< ISO15693 lock Application Family Identifier command. */
#define PHAL_I15693_SW_CMD_WRITE_DSFID              0x29    /**< ISO15693 write Data Storage Format IDentifier command. */
#define PHAL_I15693_SW_CMD_LOCK_DSFID               0x2A    /**< ISO15693 lock Data Storage Format IDentifier command. */
#define PHAL_I15693_SW_CMD_GET_SYSTEM_INFO          0x2B    /**< ISO15693 get system information command. */
#define PHAL_I15693_SW_CMD_GET_MULTIPLE_BLOCK_SEC   0x2C    /**< ISO15693 get multiple block security status command. */

/**
* \brief Handle WriteAlike.
* \return Status code
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_Sw_WriteAlikeHandling(
    phalI15693_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
    phStatus_t wExchangeStatus                  /**< [In] Status of preceeding command exchange. */
    );

/**
* \brief Sets or clears the option bit in the \ref phpalSli15693 layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_Sw_Int_SetOptionBit(
    phalI15693_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
    uint8_t bOption                             /**< [In] Option flag; \c #PH_OFF: disable; \c #PH_ON: enable. */
    );

#endif /* PHALI15693_SW_INT_H */
