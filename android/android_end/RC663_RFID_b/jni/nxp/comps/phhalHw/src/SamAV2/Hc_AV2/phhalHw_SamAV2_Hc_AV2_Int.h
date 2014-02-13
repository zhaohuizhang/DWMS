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
* $Revision: 1.3 $
* $Date: Fri Apr 15 09:26:42 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_SAMAV2_HC_AV2_INT_H
#define PHHALHW_SAMAV2_HC_AV2_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

#define PHHAL_HW_SAMAV2_HOST_AUTH_RESP1_LENGTH 0x0C
#define PHHAL_HW_SAMAV2_HOST_AUTH_RESP2_LENGTH 0x18
#define PHHAL_HW_SAMAV2_HOST_AUTH_RESP3_LENGTH 0x10

/* AV2 defines for key generation */
#define PHHAL_HW_SAMAV2_SECMSG_HC_AUTH_ENC_SV1A_AES128 0x91;
#define PHHAL_HW_SAMAV2_SECMSG_HC_AUTH_ENC_SV1A_AES192 0x93;
#define PHHAL_HW_SAMAV2_SECMSG_HC_AUTH_ENC_SV1B_AES192 0x94;

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_InitAndLoadIV(phhalHw_SamAV2_DataParams_t * pDataParams, 
												   uint8_t* pIV,
												   uint8_t encryptionIV);




phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GenerateAuthEncKey(phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */                                                
                                                        uint8_t keyType,                                /**< [In] the key type for the SamAV1 (DES, 3K3DES, AES128 or AES192)*/
                                                        uint8_t * pRnd1Data,                            /**< [In] pointer to RndA data*/                              
                                                        uint8_t * pRnd2Data,                            /**< [In] pointer to RndB data*/                              
                                                        uint8_t * pSessionKey,                          /**< [Out] pointer to the Session Host key */                              
                                                        uint8_t * pKeyLength                            /**< [Out] length of the Session Host key */                              
                                                        );

phStatus_t phhalHw_SamAV2_Hc_AV2_Int_GenerateSessionKey(phhalHw_SamAV2_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */                                                
                                                        uint8_t keyType,                                /**< [In] the key type for the SamAV1 (DES, 3K3DES, AES128 or AES192)*/
                                                        uint8_t * pRndAData,                            /**< [In] pointer to RndA data*/                              
                                                        uint8_t * pRndBData,                            /**< [In] pointer to RndB data*/                              
                                                        uint8_t * pEncSessionKey,                       /**< [Out] pointer to the ENC Session Host key */                              
                                                        uint8_t * pMacSessionKey,                       /**< [Out] pointer to the MAC Session Host key */                              
                                                        uint8_t * pKeyLength                            /**< [Out] length of the Session Host key */                              
                                                        );

/** @}
* end of ph_Private group
*/

#endif /* PHHALHW_SAMAV2_HC_AV2_INT_H */
