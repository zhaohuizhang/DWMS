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
* Internal definitions for ISO14443-4 Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.5 $
* $Date: Fri Apr 15 09:26:33 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_SAMAV2_HC_AV1_INT_H
#define PHHALHW_SAMAV2_HC_AV1_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

#define PH_SECMSG_HC_SAMAV1_INT_NO_AUTH                0x00
#define PH_SECMSG_HC_SAMAV1_INT_AUTH_PART1          0x01
#define PH_SECMSG_HC_SAMAV1_INT_AUTH_PART2          0x02
#define PH_SECMSG_HC_SAMAV1_INT_AUTH_PART3          0x03

#define PH_SECMSG_HC_SAMAV1_INT_CHANGEKEYENTRY_UNMACED_RESP_SIZE          0x02
#define PH_SECMSG_HC_SAMAV1_INT_CHANGEKEYENTRY_EXTSET_BYTE_OFFSET         0x41

#define PH_SECMSG_HC_SAMAV1_INT_KEYTYPE_MASK            0x38U
#define PH_SECMSG_HC_SAMAV1_INT_3DESDF4_MASK            0x00U
#define PH_SECMSG_HC_SAMAV1_INT_2K3DES_MASK                0x01U
#define PH_SECMSG_HC_SAMAV1_INT_2K3DESDF8_MASK            0x06U

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_GenerateSessionKey(
    uint8_t * pKey,         /**< [In]the DES key */
    uint16_t keyType,       /**< [In] the key type for the SamAV1 (DES, 3K3DES, AES128 or AES192)*/
    uint8_t * pRndAData,    /**< [In] pointer to RndA data*/                              
    uint8_t * pRndBData,    /**< [In] pointer to RndB data*/                              
    uint8_t * pSessionKey   /**< [Out] pointer to the Session Host key */                              
    );

/**
* \brief Encode the KeyVersion in a DES or 3KDES.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Operation aborted.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phhalHw_SamAV2_Hc_AV1_Int_EncodeVersionInDESKey(
    uint8_t * pKey,         /**< [In] pointer to the key to encode */
    uint8_t KeyVersion,     /**< [In] KeyVersion used for encoding */
    uint8_t round,          /**< [In] number of block(8bytes) to encode */
    uint8_t * pEncodedKey   /**< [Out] pointer to the encoded key */
    );

phStatus_t phhalHw_SamAV2_Hc_AV1_Int_GetKeyLengthParams(
    uint16_t wKeyType,
    uint8_t b2K3DESOption,
    uint8_t * pKeyLength,
    uint8_t * pRndBLength,
    uint8_t * pBlockLength,
    uint8_t * pCrcLength
    );

/** @}
* end of ph_Private group
*/

#endif /* PHHALHW_SAMAV2_HC_AV1_INT_H */
