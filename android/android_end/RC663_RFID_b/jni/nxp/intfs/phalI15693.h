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
* Generic ISO 15693 Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:19 2011 $
*
* History:
*  CHu: Generated 10. November 2009
*
*/

#ifndef PHALI15693_H
#define PHALI15693_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PHAL_I15693_SW

/** \defgroup phalI15693_Sw Component : Software
* @{
*/

#define PHAL_I15693_SW_ID       0x01    /**< ID for Software I15693 layer */

/**
* \brief Private parameter structure
*/
typedef struct
{
    uint16_t wId;                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalSli15693DataParams;  /**< Pointer to palSli15693 parameter structure. */
} phalI15693_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalI15693_Sw_Init(
                              phalI15693_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wSizeOfDataParams,               /**< [In] Specifies the size of the data parameter structure. */
                              void * pPalSli15693DataParams             /**< [In] Pointer to palSli15693 parameter structure. */
                              );

/** @} */
#endif /* NXPBUILD__PHAL_I15693_SW */

#ifdef NXPBUILD__PHAL_I15693

/** \defgroup phalI15693 ISO 15693
* \brief These Components implement the ISO 15693 commands.
* @{
*/

/**
* \name Options
*/
/*@{*/
#define PHAL_I15693_OPTION_OFF      PH_OFF                  /**< Option Bit is cleared. */
#define PHAL_I15693_OPTION_ON       PH_ON                   /**< Option Bit is set. */
#define PHAL_I15693_OPTION_DEFAULT  PHAL_I15693_OPTION_OFF  /**< Default Option. */
/*@}*/

/**
* \brief When receiving the Read single block command, the VICC shall read 
* the requested block and send back its value in pRxBuffer.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_ReadSingleBlock(
                                      void * pDataParams,       /**< [In] Pointer to this layers parameter structure. */
                                      uint8_t bOption,          /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                                      uint8_t bBlockNo,         /**< [In] Block number. */
                                      uint8_t ** ppRxBuffer,    /**< [Out] Pointer to received data. */
                                      uint16_t * pRxLength      /**< [Out] number of received data bytes. */
                                      );

/**
* \brief When receiving the Read multiple blocks command, the VICC shall read 
* the requested block(s) and send back their value in pRxBuffer.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_ReadMultipleBlocks(
    void * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
    uint8_t bOption,        /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
    uint8_t bBlockNo,       /**< [In] First Block number. */
    uint16_t wNumBlocks,    /**< [In] Number of blocks. */
    uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data. */
    uint16_t * pRxLength    /**< [Out] number of received data bytes. */
    );

/**
* \brief When receiving the Write single block command, the VICC shall 
* write the requested block with the data contained in the pTxBuffer
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_WriteSingleBlock(
                                       void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                       uint8_t bOption,     /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                                       uint8_t bBlockNo,    /**< [In] Block number. */
                                       uint8_t * pTxBuffer, /**< [In] Data to transmit. */
                                       uint16_t wTxLength   /**< [In] number of bytes to send */
                                       );

/**
* \brief When receiving the Write multiple blocks command, the VICC shall 
* write the requested block(s) with the data contained in the pTxBuffer
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_WriteMultipleBlocks(
    void * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
    uint8_t bOption,        /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
    uint8_t bBlockNo,       /**< [In] First Block number. */
    uint16_t wNumBlocks,    /**< [In] Number of blocks (real number, is automatically decremented by one). */
    uint8_t * pTxBuffer,    /**< [In] Data to transmit. */
    uint16_t wTxLength      /**< [In] number of bytes to send */
    );

/**
* \brief When receiving the Lock block command, the VICC shall lock 
* permanently the requested block.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_LockBlock(
                                void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                uint8_t bOption,    /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                                uint8_t bBlockNo    /**< [In] Block number. */
                                );

/**
* \brief When receiving the Write AFI request, the VICC shall write the 
* AFI value into its memory.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_WriteAFI(
                               void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                               uint8_t bOption,     /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                               uint8_t bAfi         /**< [In] Value of Application Family Identifier. */
                               );

/**
* \brief When receiving the Lock AFI request, the VICC shall lock the 
* AFI value permanently into its memory.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_LockAFI(
                              void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                              uint8_t bOption       /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                              );

/**
* \brief When receiving the Write DSFID request, the VICC shall write 
* the DSFID value into its memory.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_WriteDSFID(
                                 void * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                                 uint8_t bOption,       /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                                 uint8_t bDsfid         /**< [In] Value of DSFID(data storage format identifier). */
                                 );

/**
* \brief When receiving the Lock DSFID request, the VICC shall lock the 
* DSFID value permanently into its memory.
*
* Option_flag can be set by using \ref phpalSli15693_SetConfig command
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_LockDSFID(
                                void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                uint8_t bOption     /**< [In] Option flag; \c #PHAL_I15693_OPTION_OFF: disable; \c #PHAL_I15693_OPTION_ON: enable. */
                                );

/**
* \brief This command allows for retrieving the system information value 
* from the VICC.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_GetSystemInformation(
    void * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
    uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data. */
    uint16_t * pRxLength    /**< [Out] number of received data bytes. */
    );

/**
* \brief When receiving the Get multiple block security status command, 
* the VICC shall send back the block security status.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalI15693_GetMultipleBlockSecurityStatus(
    void * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
    uint8_t bBlockNo,       /**< [In] Block number. */
    uint16_t wNumBlocks,    /**< [In] Number of blocks. */
    uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data. */
    uint16_t * pRxLength    /**< [Out] number of received data bytes. */
    );

/** @} */
#endif /* NXPBUILD__PHAL_I15693 */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALI15693_H */
