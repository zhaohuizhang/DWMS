/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

#ifndef PHALMFDF_SAMAV2_X_INT_H
#define PHALMFDF_SAMAV2_X_INT_H

#include <phalMfdf.h>

#define PHAL_MFDF_APDU_BUFF_LEN     0x28U   /* Buffer size used for framing APDU. */
#define PHAL_MFDF_ENC_FLAG          0x30U   /* Encrypted communication flag for ReadX and WriteX function */
#define PHAL_MFDF_MAC_FLAG          0x10U   /* Macced communication flag for ReadX and WriteX function */


uint8_t phalMfdf_SamAV2_X_Int_WrapInIso7816apdu(
    uint8_t * pApdu,
    uint8_t bCmdIndex
    );

phStatus_t phalMfdf_SamAV2_X_Int_HalToMfdf(
    phalMfdf_SamAV2_X_DataParams_t * pDataParams,
    uint16_t wStatus
    );
#endif /* PHALMFDF_SAMAV2_X_INT_H */
