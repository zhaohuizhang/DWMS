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
* Generic Virtual Card Architecture (R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:24 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <ph_Status.h>
#include <phalVca.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_VCA_SW
#include "Sw/phalVca_Sw.h"
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
#include "SamAV2/phalVca_SamAV2.h"
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
#include "SamAV2_X/phalVca_SamAV2_X.h"
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

#ifdef NXPBUILD__PHAL_VCA

phStatus_t phalVca_VcSupport(
                             void * pDataParams,
                             uint8_t * pIid,
                             uint16_t wKeyEncNumber,
                             uint16_t wKeyEncVersion,
                             uint16_t wKeyMacNumber,
                             uint16_t wKeyMacVersion
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_VcSupport");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIid_log, pIid, 16);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncNumber_log, &wKeyEncNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncVersion_log, &wKeyEncVersion);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacNumber_log, &wKeyMacNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacVersion_log, &wKeyMacVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pIid);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_VcSupport((phalVca_Sw_DataParams_t *)pDataParams,pIid ,wKeyEncNumber, wKeyEncVersion, wKeyMacNumber, wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_VcSupport((phalVca_SamAV2_DataParams_t *)pDataParams,pIid ,wKeyEncNumber, wKeyEncVersion, wKeyMacNumber, wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_VcSupport((phalVca_SamAV2_X_DataParams_t *)pDataParams,pIid ,wKeyEncNumber, wKeyEncVersion, wKeyMacNumber, wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */
    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_VcSupportLast(
                                 void * pDataParams,
                                 uint8_t * pIid,
                                 uint8_t bLenCap,
                                 uint8_t * pPcdCapabilities,
                                 uint16_t wKeyEncNumber,
                                 uint16_t wKeyEncVersion,
                                 uint16_t wKeyMacNumber,
                                 uint16_t wKeyMacVersion
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_VcSupportLast");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLenCap);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCapabilities);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyEncVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyMacVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIid_log, pIid, 16);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLenCap_log, &bLenCap);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCapabilities_log, pPcdCapabilities, bLenCap);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncNumber_log, &wKeyEncNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyEncVersion_log, &wKeyEncVersion);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacNumber_log, &wKeyMacNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyMacVersion_log, &wKeyMacVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pIid);
	if (bLenCap) PH_ASSERT_NULL (pPcdCapabilities);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_VcSupportLast(
            (phalVca_Sw_DataParams_t *)pDataParams, 
            pIid,
            bLenCap,
            pPcdCapabilities,
            wKeyEncNumber,                       
            wKeyEncVersion,
            wKeyMacNumber,                     
            wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_VcSupportLast(
            (phalVca_SamAV2_DataParams_t *)pDataParams, 
            pIid,
            bLenCap,
            pPcdCapabilities,
            wKeyEncNumber,                       
            wKeyEncVersion,
            wKeyMacNumber,                     
            wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_VcSupportLast(
            (phalVca_SamAV2_X_DataParams_t *)pDataParams, 
            pIid,
            bLenCap,
            pPcdCapabilities,
            wKeyEncNumber,                       
            wKeyEncVersion,
            wKeyMacNumber,                     
            wKeyMacVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_StartCardSelection (
                                       void * pDataParams
                                       )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_StartCardSelection");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_StartCardSelection(
            (phalVca_Sw_DataParams_t *)pDataParams
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_StartCardSelection(
            (phalVca_SamAV2_DataParams_t *)pDataParams
            );
        break; 
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_StartCardSelection(
            (phalVca_SamAV2_X_DataParams_t *)pDataParams
            );
        break; 
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_FinalizeCardSelection (
    void * pDataParams,
    uint16_t * pNumValidIids
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_FinalizeCardSelection");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumValidIids);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pNumValidIids);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_FinalizeCardSelection(
            (phalVca_Sw_DataParams_t *)pDataParams, 
            pNumValidIids
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_FinalizeCardSelection(
            (phalVca_SamAV2_DataParams_t *)pDataParams, 
            pNumValidIids
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_FinalizeCardSelection(
            (phalVca_SamAV2_X_DataParams_t *)pDataParams, 
            pNumValidIids
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pNumValidIids_log, pNumValidIids);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_GetIidInfo(
                              void * pDataParams,
                              uint16_t wValidIidIndex,
                              uint16_t * pIidIndex,
                              uint8_t * pVcUidSize,
                              uint8_t * pVcUid,
                              uint8_t * pInfo,
                              uint8_t * pPdCapabilities
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_GetIidInfo");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValidIidIndex);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIidIndex);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVcUidSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVcUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPdCapabilities);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValidIidIndex_log, &wValidIidIndex);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pIidIndex);
	PH_ASSERT_NULL (pVcUidSize);
	PH_ASSERT_NULL (pVcUid);
	PH_ASSERT_NULL (pInfo);
	PH_ASSERT_NULL (pPdCapabilities);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_GetIidInfo(
            (phalVca_Sw_DataParams_t *)pDataParams, 
            wValidIidIndex,
            pIidIndex,
            pVcUidSize,
            pVcUid, 
            pInfo,
            pPdCapabilities 
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_GetIidInfo(
            (phalVca_SamAV2_DataParams_t *)pDataParams, 
            wValidIidIndex,
            pIidIndex,
            pVcUidSize,
            pVcUid, 
            pInfo,
            pPdCapabilities 
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_GetIidInfo(
            (phalVca_SamAV2_X_DataParams_t *)pDataParams, 
            wValidIidIndex,
            pIidIndex,
            pVcUidSize,
            pVcUid, 
            pInfo,
            pPdCapabilities 
            );
        break;    
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ( (status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pIidIndex_log, pIidIndex);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pVcUidSize_log, pVcUidSize);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVcUid_log, pVcUid, *pVcUidSize);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pInfo_log, pInfo);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPdCapabilities_log, pPdCapabilities, 2);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_SelectVc(
                            void * pDataParams,
                            uint16_t wValidIidIndex,
                            uint16_t wKeyNumber,
                            uint16_t wKeyVersion
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_SelectVc");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValidIidIndex);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValidIidIndex_log, &wValidIidIndex);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_SelectVc((phalVca_Sw_DataParams_t *)pDataParams, wValidIidIndex, wKeyNumber,wKeyVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_SelectVc((phalVca_SamAV2_DataParams_t *)pDataParams, wValidIidIndex, wKeyNumber,wKeyVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_SelectVc((phalVca_SamAV2_X_DataParams_t *)pDataParams, wValidIidIndex, wKeyNumber,wKeyVersion);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_DeselectVc(
                              void * pDataParams
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_DeselectVc");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_DeselectVc((phalVca_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_DeselectVc((phalVca_SamAV2_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_DeselectVc((phalVca_SamAV2_X_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalVca_ProximityCheck(
                                  void * pDataParams,
                                  uint8_t bGenerateRndC,
                                  uint8_t * pRndC,
                                  uint8_t bPps1,
                                  uint8_t bNumSteps,
                                  uint16_t wKeyNumber,
                                  uint16_t wKeyVersion,
                                  uint8_t * pUsedRndC
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalVca_ProximityCheck");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bGenerateRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPps1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumSteps);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUsedRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bGenerateRndC_log, &bGenerateRndC);
#ifdef NXPBUILD__PH_LOG
    if (!bGenerateRndC)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRndC_log, pRndC, 7);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bPps1_log, &bPps1);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumSteps_log, &bNumSteps);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	if (!bGenerateRndC) PH_ASSERT_NULL (pRndC);
	PH_ASSERT_NULL (pUsedRndC);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_VCA)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_VCA_SW
    case PHAL_VCA_SW_ID:
        status = phalVca_Sw_ProximityCheck((phalVca_Sw_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, wKeyNumber, wKeyVersion, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_VCA_SW */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2
    case PHAL_VCA_SAMAV2_ID:
        status = phalVca_SamAV2_ProximityCheck((phalVca_SamAV2_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, wKeyNumber, wKeyVersion, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2 */

#ifdef NXPBUILD__PHAL_VCA_SAMAV2_X
    case PHAL_VCA_SAMAV2_X_ID:
        status = phalVca_SamAV2_X_ProximityCheck((phalVca_SamAV2_X_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, wKeyNumber, wKeyVersion, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_VCA_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_VCA);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUsedRndC_log, pUsedRndC, 7);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPBUILD__PHAL_VCA */
