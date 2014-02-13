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
* Generic ISO 15693 Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:26:50 2011 $
*
* History:
*  CHu: Generated 10. November 2009
*
*/

#include <phalI15693.h>
#include <phhalHw.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHAL_I15693_SW
#include "Sw/phalI15693_Sw.h"
#endif /* NXPBUILD__PHAL_I15693_SW */

#ifdef NXPBUILD__PHAL_I15693

phStatus_t phalI15693_ReadSingleBlock (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bBlockNo,
                                        uint8_t ** ppRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_ReadSingleBlock");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (ppRxBuffer);
	PH_ASSERT_NULL (pRxLength);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_ReadSingleBlock((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bBlockNo, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    
    return status;
}

phStatus_t phalI15693_ReadMultipleBlocks (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bBlockNo,
                                        uint16_t wNumBlocks,
                                        uint8_t ** ppRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_ReadMultipleBlocks");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wNumBlocks_log, &wNumBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (ppRxBuffer);
	PH_ASSERT_NULL (pRxLength);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_ReadMultipleBlocks ((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bBlockNo, wNumBlocks, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_WriteSingleBlock (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bBlockNo,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_WriteSingleBlock");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pTxBuffer);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_WriteSingleBlock ((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bBlockNo, pTxBuffer, wTxLength);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_WriteMultipleBlocks (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bBlockNo,
                                        uint16_t wNumBlocks,
                                        uint8_t * pTxBuffer,
                                        uint16_t wTxLength
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_WriteMultipleBlocks");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wNumBlocks_log, &wNumBlocks);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pTxBuffer);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_WriteMultipleBlocks ((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bBlockNo, wNumBlocks, pTxBuffer, wTxLength);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_LockBlock (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bBlockNo
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_LockBlock");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_LockBlock ((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bBlockNo);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_WriteAFI (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bAfi
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_WriteAFI");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAfi);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bAfi_log, &bAfi);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_WriteAFI ((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bAfi);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_LockAFI (
                                        void * pDataParams,
                                        uint8_t bOption
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_LockAFI");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_LockAFI ((phalI15693_Sw_DataParams_t *)pDataParams, bOption);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_WriteDSFID (
                                        void * pDataParams,
                                        uint8_t bOption,
                                        uint8_t bDsfid
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_WriteDSFID");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDsfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bDsfid_log, &bDsfid);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_WriteDSFID ((phalI15693_Sw_DataParams_t *)pDataParams, bOption, bDsfid);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_LockDSFID (
                                        void * pDataParams,
                                        uint8_t bOption
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_LockDSFID");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bOption_log, &bOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_LockDSFID ((phalI15693_Sw_DataParams_t *)pDataParams, bOption);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_GetSystemInformation (
                                        void * pDataParams,
                                        uint8_t ** ppRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_GetSystemInformation");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (ppRxBuffer);
	PH_ASSERT_NULL (pRxLength);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_GetSystemInformation ((phalI15693_Sw_DataParams_t *)pDataParams, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if  ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalI15693_GetMultipleBlockSecurityStatus (
                                        void * pDataParams,
                                        uint8_t bBlockNo,
                                        uint16_t wNumBlocks,
                                        uint8_t ** ppRxBuffer,
                                        uint16_t * pRxLength
                                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalI15693_GetMultipleBlockSecurityStatus");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNumBlocks);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, wNumBlocks_log, &wNumBlocks);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (ppRxBuffer);
	PH_ASSERT_NULL (pRxLength);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_I15693)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_I15693_SW
    case PHAL_I15693_SW_ID:
		status = phalI15693_Sw_GetMultipleBlockSecurityStatus ((phalI15693_Sw_DataParams_t *)pDataParams, bBlockNo, wNumBlocks, ppRxBuffer, pRxLength);
        break;
#endif /* NXPBUILD__PHAL_I15693_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_I15693);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if  ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif
