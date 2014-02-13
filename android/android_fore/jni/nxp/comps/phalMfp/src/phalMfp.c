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
* Generic MIFARE Plus(R) Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:27:02 2011 $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#include <phalMfp.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_MFP_SW
#include "Sw/phalMfp_Sw.h"
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
#include "SamAV2/phalMfp_SamAV2.h"
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
#include "SamAV2_X/phalMfp_SamAV2_X.h"
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFP

phStatus_t phalMfp_WritePerso(
                              void * pDataParams,
                              uint8_t bLayer4Comm,
                              uint16_t wBlockNr,
                              uint8_t * pValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_WritePerso");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLayer4Comm);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLayer4Comm_log, &bLayer4Comm);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 16);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_WritePerso((phalMfp_Sw_DataParams_t *)pDataParams, bLayer4Comm, wBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_WritePerso((phalMfp_SamAV2_DataParams_t *)pDataParams, bLayer4Comm, wBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_WritePerso((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bLayer4Comm, wBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_CommitPerso(
                               void * pDataParams,
                               uint8_t bLayer4Comm
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_CommitPerso");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLayer4Comm);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLayer4Comm_log, &bLayer4Comm);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_CommitPerso((phalMfp_Sw_DataParams_t *)pDataParams,bLayer4Comm);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_CommitPerso((phalMfp_SamAV2_DataParams_t *)pDataParams, bLayer4Comm);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_CommitPerso((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bLayer4Comm);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_AuthenticateSL0(
                                   void * pDataParams,
                                   uint8_t bLayer4Comm,
                                   uint8_t bFirstAuth,
                                   uint16_t wBlockNr,
                                   uint16_t wKeyNumber,
                                   uint16_t wKeyVersion,
                                   uint8_t bLenDivInput,
                                   uint8_t * pDivInput,
                                   uint8_t bLenPcdCap2,
                                   uint8_t * pPcdCap2In,
                                   uint8_t * pPcdCap2Out,
                                   uint8_t * pPdCap2
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_AuthenticateSL0");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLayer4Comm);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFirstAuth);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2In);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2Out);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPdCap2);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLayer4Comm_log, &bLayer4Comm);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFirstAuth_log, &bFirstAuth);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2In_log, pPcdCap2In, bLenPcdCap2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	if (bLenDivInput) PH_ASSERT_NULL (pDivInput);
	if (bLenPcdCap2) PH_ASSERT_NULL (pPcdCap2In);
	PH_ASSERT_NULL (pPcdCap2Out);
	PH_ASSERT_NULL (pPdCap2);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_AuthenticateSL0((phalMfp_Sw_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_AuthenticateSL0((phalMfp_SamAV2_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_AuthenticateSL0((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2Out_log, pPcdCap2Out, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPdCap2_log, pPdCap2, 6);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_AuthenticateSL1(
                                   void * pDataParams,
                                   uint8_t bLayer4Comm,
                                   uint8_t bFirstAuth,
                                   uint16_t wBlockNr,
                                   uint16_t wKeyNumber,
                                   uint16_t wKeyVersion,
                                   uint8_t bLenDivInput,
                                   uint8_t * pDivInput,
                                   uint8_t bLenPcdCap2,
                                   uint8_t * pPcdCap2In,
                                   uint8_t * pPcdCap2Out,
                                   uint8_t * pPdCap2
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_AuthenticateSL1");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLayer4Comm);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFirstAuth);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2In);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2Out);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPdCap2);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLayer4Comm_log, &bLayer4Comm);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFirstAuth_log, &bFirstAuth);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2In_log, pPcdCap2In, bLenPcdCap2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	if (bLenDivInput) PH_ASSERT_NULL (pDivInput);
	if (bLenPcdCap2) PH_ASSERT_NULL (pPcdCap2In);
	PH_ASSERT_NULL (pPcdCap2Out);
	PH_ASSERT_NULL (pPdCap2);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_AuthenticateSL1((phalMfp_Sw_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_AuthenticateSL1((phalMfp_SamAV2_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_AuthenticateSL1((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2Out_log, pPcdCap2Out, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPdCap2_log, pPdCap2, 6);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfp_AuthenticateSL2(
                                   void * pDataParams,
                                   uint8_t bLayer4Comm,
                                   uint8_t bFirstAuth,
                                   uint16_t wBlockNr,
                                   uint16_t wKeyNumber,
                                   uint16_t wKeyVersion,
                                   uint8_t bLenDivInput,
                                   uint8_t * pDivInput,
                                   uint8_t bLenPcdCap2,
                                   uint8_t * pPcdCap2In,
                                   uint8_t * pPcdCap2Out,
                                   uint8_t * pPdCap2,
                                   uint8_t * pKmf
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_AuthenticateSL2");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLayer4Comm);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFirstAuth);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2In);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2Out);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPdCap2);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKmf);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLayer4Comm_log, &bLayer4Comm);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFirstAuth_log, &bFirstAuth);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2In_log, pPcdCap2In, bLenPcdCap2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	if (bLenDivInput) PH_ASSERT_NULL (pDivInput);
	if (bLenPcdCap2) PH_ASSERT_NULL (pPcdCap2In);
	PH_ASSERT_NULL (pPcdCap2Out);
	PH_ASSERT_NULL (pPdCap2);
	PH_ASSERT_NULL (pKmf);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_AuthenticateSL2((phalMfp_Sw_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2, pKmf);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_AuthenticateSL2((phalMfp_SamAV2_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2, pKmf);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_AuthenticateSL2((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bLayer4Comm, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2, pKmf);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2Out_log, pPcdCap2Out, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPdCap2_log, pPdCap2, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKmf_log, pKmf, 6);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_AuthenticateClassicSL2(
    void * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNumber,
    uint16_t wKeyVersion,
    uint8_t * pUid,
    uint8_t bUidLength
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_AuthenticateClassicSL2");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyType_log, &bKeyType);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, bUidLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pUid);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_AuthenticateClassicSL2((phalMfp_Sw_DataParams_t *)pDataParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion, pUid, bUidLength);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_AuthenticateClassicSL2((phalMfp_SamAV2_DataParams_t *)pDataParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion, pUid, bUidLength);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_AuthenticateClassicSL2((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bBlockNo, bKeyType, wKeyNumber, wKeyVersion, pUid, bUidLength);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_MultiBlockRead(
                                  void * pDataParams,
                                  uint8_t bBlockNr,
                                  uint8_t bNumBlocks,
                                  uint8_t * pBlocks
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_MultiBlockRead");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNr_log, &bBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumBlocks_log, &bNumBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pBlocks);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_MultiBlockRead((phalMfp_Sw_DataParams_t *)pDataParams,bBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_MultiBlockRead((phalMfp_SamAV2_DataParams_t *)pDataParams,bBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_MultiBlockRead((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pBlocks_log, pBlocks, 16*bNumBlocks);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_MultiBlockWrite(
                                   void * pDataParams,
                                   uint8_t bBlockNr,
                                   uint8_t bNumBlocks,
                                   uint8_t * pBlocks
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_MultiBlockWrite");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNr_log, &bBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumBlocks_log, &bNumBlocks);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pBlocks_log, pBlocks, 16*bNumBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pBlocks);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_MultiBlockWrite((phalMfp_Sw_DataParams_t *)pDataParams,bBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_MultiBlockWrite((phalMfp_SamAV2_DataParams_t *)pDataParams,bBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_MultiBlockWrite((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_Write(
                         void * pDataParams,
                         uint8_t bEncrypted,
                         uint8_t bWriteMaced,
                         uint16_t wBlockNr,
                         uint8_t bNumBlocks,
                         uint8_t * pBlocks
                         )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_Write");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bEncrypted);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWriteMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bEncrypted_log, &bEncrypted);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWriteMaced_log, &bWriteMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumBlocks_log, &bNumBlocks);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pBlocks_log, pBlocks, 16*bNumBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pBlocks);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_Write((phalMfp_Sw_DataParams_t *)pDataParams, bEncrypted, bWriteMaced, wBlockNr, bNumBlocks, pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_Write((phalMfp_SamAV2_DataParams_t *)pDataParams, bEncrypted, bWriteMaced, wBlockNr, bNumBlocks, pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_Write((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bEncrypted, bWriteMaced, wBlockNr, bNumBlocks, pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_WriteValue(
                         void * pDataParams,
                         uint8_t bEncrypted,
                         uint8_t bWriteMaced,
                         uint16_t wBlockNr,
                         uint8_t * pValue,
                         uint8_t bAddrData
                         )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_WriteValue");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bEncrypted);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bWriteMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddrData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bEncrypted_log, &bEncrypted);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bWriteMaced_log, &bWriteMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddrData_log, &bAddrData);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_WriteValue((phalMfp_Sw_DataParams_t *)pDataParams, bEncrypted, bWriteMaced, wBlockNr, pValue, bAddrData);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_WriteValue((phalMfp_SamAV2_DataParams_t *)pDataParams, bEncrypted, bWriteMaced, wBlockNr, pValue, bAddrData);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_WriteValue((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bEncrypted, bWriteMaced, wBlockNr, pValue, bAddrData);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_ChangeKey(
                             void * pDataParams,  
                             uint8_t bChangeKeyMaced, 
                             uint16_t wBlockNr,   
                             uint16_t wKeyNumber, 
                             uint16_t wKeyVersion,
                             uint8_t bLenDivInput,
                             uint8_t * pDivInput  
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_ChangeKey");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bChangeKeyMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bChangeKeyMaced_log, &bChangeKeyMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
    if (bLenDivInput) PH_ASSERT_NULL (pDivInput);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_ChangeKey((phalMfp_Sw_DataParams_t *)pDataParams, bChangeKeyMaced, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_ChangeKey((phalMfp_SamAV2_DataParams_t *)pDataParams, bChangeKeyMaced, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_ChangeKey((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bChangeKeyMaced, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}
phStatus_t phalMfp_AuthenticateSL3(
                                   void * pDataParams,
                                   uint8_t bFirstAuth,
                                   uint16_t wBlockNr,
                                   uint16_t wKeyNumber,
                                   uint16_t wKeyVersion,
                                   uint8_t bLenDivInput,
                                   uint8_t * pDivInput,
                                   uint8_t bLenPcdCap2,
                                   uint8_t * pPcdCap2In,
                                   uint8_t * pPcdCap2Out,
                                   uint8_t * pPdCap2
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_AuthenticateSL3");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFirstAuth);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNumber);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2In);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPcdCap2Out);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPdCap2);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFirstAuth_log, &bFirstAuth);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNumber_log, &wKeyNumber);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bLenDivInput);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2In_log, pPcdCap2In, bLenPcdCap2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	if (bLenDivInput) PH_ASSERT_NULL (pDivInput);
	if (bLenPcdCap2) PH_ASSERT_NULL (pPcdCap2In);
	PH_ASSERT_NULL (pPcdCap2Out);
	PH_ASSERT_NULL (pPdCap2);


    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_AuthenticateSL3((phalMfp_Sw_DataParams_t *)pDataParams, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2,  pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_AuthenticateSL3((phalMfp_SamAV2_DataParams_t *)pDataParams, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_AuthenticateSL3((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bFirstAuth, wBlockNr, wKeyNumber, wKeyVersion, bLenDivInput, pDivInput, bLenPcdCap2, pPcdCap2In, pPcdCap2Out, pPdCap2);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPcdCap2Out_log, pPcdCap2Out, 6);
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPdCap2_log, pPdCap2, 6);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_Read(
                        void * pDataParams,
                        uint8_t bEncrypted,
                        uint8_t bReadMaced,
                        uint8_t bMacOnCmd,
                        uint16_t wBlockNr,
                        uint8_t bNumBlocks,
                        uint8_t * pBlocks
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_Read");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bEncrypted);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bReadMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMacOnCmd);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bEncrypted_log, &bEncrypted);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bReadMaced_log, &bReadMaced);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMacOnCmd_log, &bMacOnCmd);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNumBlocks_log, &bNumBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pBlocks);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_Read((phalMfp_Sw_DataParams_t *)pDataParams,bEncrypted,bReadMaced,bMacOnCmd,wBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_Read((phalMfp_SamAV2_DataParams_t *)pDataParams,bEncrypted,bReadMaced,bMacOnCmd,wBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_Read((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bEncrypted,bReadMaced,bMacOnCmd,wBlockNr,bNumBlocks,pBlocks);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pBlocks_log, pBlocks, 16 * bNumBlocks);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}



phStatus_t phalMfp_ReadValue(
                        void * pDataParams,
                        uint8_t bEncrypted,
                        uint8_t bReadMaced,
                        uint8_t bMacOnCmd,
                        uint16_t wBlockNr,
                        uint8_t * pValue,
                        uint8_t * pAddrData
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_ReadValue");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bEncrypted);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bReadMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bMacOnCmd);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAddrData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bEncrypted_log, &bEncrypted);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bReadMaced_log, &bReadMaced);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bMacOnCmd_log, &bMacOnCmd);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);
	PH_ASSERT_NULL (pAddrData);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_ReadValue((phalMfp_Sw_DataParams_t *)pDataParams,bEncrypted,bReadMaced,bMacOnCmd,wBlockNr,pValue,pAddrData);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_ReadValue((phalMfp_SamAV2_DataParams_t *)pDataParams,bEncrypted,bReadMaced,bMacOnCmd,wBlockNr,pValue,pAddrData);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_ReadValue((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bEncrypted,bReadMaced,bMacOnCmd,wBlockNr,pValue,pAddrData);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
        PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pAddrData_log, pAddrData);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_ResetAuth(
                             void * pDataParams
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_ResetAuth");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_ResetAuth((phalMfp_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_ResetAuth((phalMfp_SamAV2_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_ResetAuth((phalMfp_SamAV2_X_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_Increment(
                             void * pDataParams,
                             uint8_t bIncrementMaced,
                             uint16_t wBlockNr,
                             uint8_t * pValue
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_Increment");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIncrementMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIncrementMaced_log, &bIncrementMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_Increment((phalMfp_Sw_DataParams_t *)pDataParams, bIncrementMaced,wBlockNr,pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_Increment((phalMfp_SamAV2_DataParams_t *)pDataParams, bIncrementMaced,wBlockNr,pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_Increment((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bIncrementMaced,wBlockNr,pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_Decrement(
                             void * pDataParams,
                             uint8_t bDecrementMaced,
                             uint16_t wBlockNr,
                             uint8_t * pValue
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_Decrement");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDecrementMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDecrementMaced_log, &bDecrementMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_Decrement((phalMfp_Sw_DataParams_t *)pDataParams, bDecrementMaced,wBlockNr,pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_Decrement((phalMfp_SamAV2_DataParams_t *)pDataParams, bDecrementMaced,wBlockNr,pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_Decrement((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bDecrementMaced,wBlockNr,pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_IncrementTransfer(
                                     void * pDataParams,
                                     uint8_t bIncrementTransferMaced,
                                     uint16_t wSourceBlockNr,
                                     uint16_t wDestinationBlockNr,
                                     uint8_t * pValue
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_IncrementTransfer");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bIncrementTransferMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wSourceBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wDestinationBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bIncrementTransferMaced_log, &bIncrementTransferMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wSourceBlockNr_log, &wSourceBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wDestinationBlockNr_log, &wDestinationBlockNr);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_IncrementTransfer((phalMfp_Sw_DataParams_t *)pDataParams,bIncrementTransferMaced,wSourceBlockNr, wDestinationBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_IncrementTransfer((phalMfp_SamAV2_DataParams_t *)pDataParams,bIncrementTransferMaced,wSourceBlockNr, wDestinationBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_IncrementTransfer((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bIncrementTransferMaced,wSourceBlockNr, wDestinationBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_DecrementTransfer(
                                     void * pDataParams,
                                     uint8_t bDecrementTransferMaced,
                                     uint16_t wSourceBlockNr,
                                     uint16_t wDestinationBlockNr,
                                     uint8_t * pValue
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_DecrementTransfer");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDecrementTransferMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wSourceBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wDestinationBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDecrementTransferMaced_log, &bDecrementTransferMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wSourceBlockNr_log, &wSourceBlockNr);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wDestinationBlockNr_log, &wDestinationBlockNr);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_DecrementTransfer((phalMfp_Sw_DataParams_t *)pDataParams,bDecrementTransferMaced,wSourceBlockNr, wDestinationBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_DecrementTransfer((phalMfp_SamAV2_DataParams_t *)pDataParams,bDecrementTransferMaced,wSourceBlockNr, wDestinationBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_DecrementTransfer((phalMfp_SamAV2_X_DataParams_t *)pDataParams,bDecrementTransferMaced,wSourceBlockNr, wDestinationBlockNr, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_Transfer(
                            void * pDataParams,
                            uint8_t bTransferMaced,
                            uint16_t wBlockNr
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_Transfer");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTransferMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTransferMaced_log, &bTransferMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_Transfer((phalMfp_Sw_DataParams_t *)pDataParams, bTransferMaced, wBlockNr);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_Transfer((phalMfp_SamAV2_DataParams_t *)pDataParams, bTransferMaced, wBlockNr);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_Transfer((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bTransferMaced, wBlockNr);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_Restore(
                           void * pDataParams,
                           uint8_t bRestoreMaced,
                           uint16_t wBlockNr
                           )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_Restore");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRestoreMaced);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wBlockNr);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRestoreMaced_log, &bRestoreMaced);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wBlockNr_log, &wBlockNr);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_Restore((phalMfp_Sw_DataParams_t *)pDataParams, bRestoreMaced, wBlockNr);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_Restore((phalMfp_SamAV2_DataParams_t *)pDataParams, bRestoreMaced, wBlockNr);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_Restore((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bRestoreMaced, wBlockNr);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalMfp_ProximityCheck(
                                  void * pDataParams,
                                  uint8_t bGenerateRndC,
                                  uint8_t * pRndC,
                                  uint8_t bPps1,
                                  uint8_t bNumSteps,
                                  uint8_t * pUsedRndC
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_ProximityCheck");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bGenerateRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRndC);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bPps1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumSteps);
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
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	if (!bGenerateRndC) PH_ASSERT_NULL (pRndC);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_ProximityCheck((phalMfp_Sw_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_ProximityCheck((phalMfp_SamAV2_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_ProximityCheck((phalMfp_SamAV2_X_DataParams_t *)pDataParams, bGenerateRndC, pRndC, bPps1, bNumSteps, pUsedRndC);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
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

phStatus_t phalMfp_ResetSecMsgState(
                                    void * pDataParams
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfp_ResetSecMsgState");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFP)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFP_SW
    case PHAL_MFP_SW_ID:
        status = phalMfp_Sw_ResetSecMsgState((phalMfp_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFP_SW */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2
    case PHAL_MFP_SAMAV2_ID:
        status = phalMfp_SamAV2_ResetSecMsgState((phalMfp_SamAV2_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFP_SAMAV2_X
    case PHAL_MFP_SAMAV2_X_ID:
        status = phalMfp_SamAV2_X_ResetSecMsgState((phalMfp_SamAV2_X_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFP_SAMAV2_X */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFP);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

#endif /* NXPBUILD__PHAL_MFP */
