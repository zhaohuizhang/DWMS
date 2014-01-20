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
* Generic Felica Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:04 2011 $
*
* History:
*  RSn: Generated 25. January 2010
*
*/

#ifndef PHALFELICA_H
#define PHALFELICA_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PHAL_FELICA_SW

/** \defgroup phalFelica_Sw Component : Software
* @{
*/

#define PHAL_FELICA_SW_ID   0x01    /**< ID for Software alFelica layer */

/**
* \brief Private parameter structure
*/
typedef struct
{
    uint16_t wId;                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pPalFelicaDataParams;    /**< Pointer to palFelica parameter structure. */
    uint16_t wAdditionalInfo;       /**< Holds the last error code sent by the VICC. */
} phalFelica_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phalFelica_Sw_Init(
                              phalFelica_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                              uint16_t wSizeOfDataParams,               /**< [In] Specifies the size of the data parameter structure. */
                              void * pPalFelica_DataParams              /**< [In] Pointer to palFelica parameter structure. */
                              );

/** @} */
#endif /* NXPBUILD__PHAL_FELICA_SW */

#ifdef NXPBUILD__PHAL_FELICA

/** \defgroup phalFelica Felica
* \brief These Components implement the Felica commands.
* @{
*/

/**
* \name Felica constant definitions
*/
/*@{*/
#define PHAL_FELICA_CONFIG_ADD_INFO     0x0000U     /**< Set or get the additional info word. */

/**
* \name FELICA error codes
*/
/*@{*/
#define PHAL_FELICA_ERR_FELICA  (PH_ERR_CUSTOM_BEGIN+0) /**< General Felica error. */
/*@}*/

/**
* \name Misc. Defines
*/
/*@{*/
#define PHAL_FELICA_MAX_SERVICES    121U    /**< Maximum number of services. */
/*@}*/

/**
* \brief When receiving the RequestResponse command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalFelica_RequestResponse(
                                      void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                                      uint8_t * pMode       /**< [Out] Current Card Mode. (0, 1, 2). */
                                      );

/**
* \brief When receiving the RequestService command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalFelica_RequestService(
                                     void * pDataParams,        /**< [In] Pointer to this layers parameter structure. */
                                     uint8_t  bTxNumServices,   /**< [In] Number of services or areas within the command message. */
                                     uint8_t * pTxServiceList,  /**< [In] Service code or area code list within the command message. */
                                     uint8_t * pRxNumServices,  /**< [Out] Number of received services or areas. */
                                     uint8_t * pRxServiceList   /**< [Out] Received Service Key version or area version list, max 64 bytes. */
                                     );

/**
* \brief When receiving the Read command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalFelica_Read(
                           void * pDataParams,          /**< [In] Pointer to this layers parameter structure. */
                           uint8_t  bNumServices,       /**< [In] Number of Services. */
                           uint8_t * pServiceList,      /**< [In] List of Services. */
                           uint8_t  bTxNumBlocks,       /**< [In] Number of Blocks to send. */
                           uint8_t * pBlockList,        /**< [In] List of Blocks to read. */
                           uint8_t  bBlockListLength,   /**< [In] Number of Blocks to read. */
                           uint8_t * pRxNumBlocks,      /**< [Out] Number of received blocks. */
                           uint8_t * pBlockData         /**< [Out] Received Block data. */
                           );

/**
* \brief When receiving the Write command, the VICC shall respond.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalFelica_Write(
                            void * pDataParams,		    /**< [In] Pointer to this layers parameter structure. */
                            uint8_t  bNumServices,      /**< [In] Number of Services. */
                            uint8_t * pServiceList,     /**< [In] List of Services. */
                            uint8_t  bNumBlocks,        /**< [In] Number of Blocks to send. */
                            uint8_t * pBlockList,       /**< [In] List of Blocks to write. */
                            uint8_t  bBlockListLength,  /**< [In] Number of Blocks to write. */
                            uint8_t * pBlockData        /**< [In] Block data to write. */
                            );

/**
* \brief Perform a GetConfig command.
*
* \c wConfig can be one of:\n
* \li #PHAL_FELICA_CONFIG_ADD_INFO
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phalFelica_GetConfig(
                                void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                uint16_t wConfig,   /**< [In] Configuration Identifier. */
                                uint16_t * pValue   /**< [Out] Configuration Value. */
                                );

/** @} */
#endif /* NXPBUILD__PHAL_FELICA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHALFELICA_H */
