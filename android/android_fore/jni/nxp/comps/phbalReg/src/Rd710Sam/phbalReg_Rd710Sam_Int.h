/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Internal definitions for RD710Sam BAL Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:52 2011 $
*
* History:
*  CHu: Generated 05. July 2010
*
*/

#ifndef PHBALREG_RD710SAM_INT_H
#define PHBALREG_RD710SAM_INT_H

#include <ph_Status.h>

#define PHBAL_RD710SAM_CMD_ACTIVATE_CONTACT_CARD    0x8001U
#define PHBAL_RD710SAM_CMD_DEACTIVATE_CONTACT_CARD  0x8006U
#define PHBAL_RD710SAM_CMD_CC_TRANSMIT_DATA         0x8008U
#define PHBAL_RD710SAM_CMD_CC_SEND_PPS              0x8009U
#define PHBAL_RD710SAM_CMD_RD_OPS_SET_PCSC_MODE		0xA00DU	/**< PCSC mode: Standard (0x00) or Direct (0x01). */

#define PHBAL_RD710SAM_FRAME_HEADER_LEN     6U  /**< Length of a command header. */
#define PHBAL_RD710SAM_FRAME_CMD_POS        0U  /**< Position of the command code (ushort). */
#define PHBAL_RD710SAM_FRAME_STATUS_POS     2U  /**< Position of the status (ushort). */
#define PHBAL_RD710SAM_FRAME_LEN_POS        4U  /**< Position of the length (ushort). */
#define PHBAL_RD710SAM_FRAME_PAYLOAD_POS    6U  /**< Position of the payload. */

phStatus_t phbalReg_Rd710Sam_CheckResponse(
    uint16_t wCmd,
    uint8_t * pRxBuffer,
    uint16_t wRxLength,
    uint8_t ** ppData,
    uint16_t * pDataLength
    );

phStatus_t phbalReg_Rd710Sam_ParseAtr(
    uint8_t * pAtr,
    uint16_t wAtrLenght,
    uint8_t * pTa1,
    uint8_t * pSpecificMode
    );

#endif /* PHBALREG_RD710SAM_INT_H */
