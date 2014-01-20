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

/** \file
* Generic MIFARE DESFire EV1 Application Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.1 $
* $Date: Fri Apr 15 09:29:18 2011 $
*
* History:
*  SA: Generated 02. Feb 2010
*
*/

#include <phalMfdf.h>
#include <ph_Status.h>
#include <ph_TypeDefs.h>

#ifdef NXPBUILD__PHAL_MFDF_SW
#include "Sw/phalMfdf_Sw.h"
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
#include "SamAV2/phalMfdf_SamAV2.h"
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
#include "SamAV2_X/phalMfdf_SamAV2_X.h"
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF

phStatus_t phalMfdf_Authenticate(
                                 void * pDataParams,
                                 uint16_t wOption,
                                 uint16_t wKeyNo,
                                 uint16_t wKeyVer,
                                 uint8_t bKeyNoCard,
                                 uint8_t * pDivInput,
                                 uint8_t bDivLen
                                 )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_Authenticate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (bDivLen) PH_ASSERT_NULL(pDivInput);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_Authenticate(
            (phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_Authenticate(
            (phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_Authenticate(
            (phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_AuthenticateISO(
                                    void * pDataParams,
                                    uint16_t wOption,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVer,
                                    uint8_t bKeyNoCard,
                                    uint8_t * pDivInput,
                                    uint8_t bDivLen
                                    )
{
    /**
    * The key type can be DES, 3DES, 3K3DES.
    * Random numbers can be 8 or 16 bytes long
    * Init vector can be 8 or 16 bytes long
    * Session key max size is 24 bytes if 3k3DES keys  are used.
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_AuthenticateISO");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (bDivLen) PH_ASSERT_NULL(pDivInput);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_AuthenticateISO(
            (phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_AuthenticateISO(
            (phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_AuthenticateISO(
            (phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_AuthenticateAES(
                                    void * pDataParams,
                                    uint16_t wOption,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVer,
                                    uint8_t bKeyNoCard,
                                    uint8_t * pDivInput,
                                    uint8_t bDivLen
                                    )
{
    /**
    * The key type can be AES only.
    * Random numbers are 16 bytes long
    * Init vector is 16 bytes long
    * Session key size is 16 bytes.
    *
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_AuthenticateAES");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (bDivLen) PH_ASSERT_NULL(pDivInput);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_AuthenticateAES(
            (phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_AuthenticateAES(
            (phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_AuthenticateAES(
            (phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wOption,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_ChangeKeySettings(
                                      void * pDataParams,
                                      uint8_t bKeySettings
                                      )
{
    /**
    * This  function will handle all the three authentication modes: 0x0A, 1A and AA.
    * and all crypto modes i.e., DES, 3DES, 3K3DES, AES
    * The previous authentication status including key number and session key is
    * present in the params  structure.
    * Successful auth. with PICC master key is required if AID = 0x00 else
    * an auth. with the application master key is required.
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_ChangeKeySettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeySettings);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeySettings_log, &bKeySettings);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_ChangeKeySettings((phalMfdf_Sw_DataParams_t *)pDataParams, bKeySettings);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_ChangeKeySettings((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bKeySettings);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_ChangeKeySettings((phalMfdf_SamAV2_DataParams_t *)pDataParams, bKeySettings);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetKeySettings(
                                   void * pDataParams,
                                   uint8_t * pResponse
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetKeySettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pResponse);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pResponse);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetKeySettings((phalMfdf_Sw_DataParams_t *)pDataParams, pResponse);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetKeySettings((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pResponse);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetKeySettings((phalMfdf_SamAV2_DataParams_t *)pDataParams, pResponse);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pResponse_log, pResponse, 2);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_ChangeKey(
                              void * pDataParams,
                              uint16_t wOption,
                              uint16_t wOldKeyNo,
                              uint16_t wOldKeyVer,
                              uint16_t wNewKeyNo,
                              uint16_t wNewKeyVer,
                              uint8_t bKeyNoCard,
                              uint8_t * pDivInput,
                              uint8_t bDivLen
                              )
{
    /**
    * This  function will handle all the three authentication modes: 0x0A, 1A and AA.
    * and all crypto modes i.e., DES, 3DES, 3K3DES, AES
    * The previous authentication status including key number and session key is
    * present in the params  structure.
    * Successful auth. with PICC master key is required if AID = 0x00 else
    * an auth. with the application master key is required.
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_ChangeKey");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOldKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNewKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOldKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wNewKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDivInput);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDivLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOldKeyVer_log, &wOldKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNewKeyVer_log, &wNewKeyVer);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDivInput_log, pDivInput, bDivLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOldKeyNo_log, &wOldKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wNewKeyNo_log, &wNewKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDivLen_log, &bDivLen);

    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    if (bDivLen > 0)PH_ASSERT_NULL (pDivInput);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_ChangeKey((phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            wOldKeyNo,
            wOldKeyVer,
            wNewKeyNo,
            wNewKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_ChangeKey((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wOption,
            wOldKeyNo,
            wOldKeyVer,
            wNewKeyNo,
            wNewKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_ChangeKey((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wOption,
            wOldKeyNo,
            wOldKeyVer,
            wNewKeyNo,
            wNewKeyVer,
            bKeyNoCard,
            pDivInput,
            bDivLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phalMfdf_GetKeyVersion(
                                  void * pDataParams,
                                  uint8_t bKeyNo,
                                  uint8_t * pResponse
                                  )
{
    /**
    * This command can be issued without valid authentication
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetKeyVersion");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pResponse);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNo_log, &bKeyNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pResponse);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetKeyVersion((phalMfdf_Sw_DataParams_t *)pDataParams, bKeyNo, pResponse);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetKeyVersion((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bKeyNo, pResponse);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetKeyVersion((phalMfdf_SamAV2_DataParams_t *)pDataParams, bKeyNo, pResponse);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pResponse_log, pResponse,1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/*
* PICC level commands
*/

phStatus_t phalMfdf_CreateApplication(
                                      void * pDataParams,
                                      uint8_t bOption,
                                      uint8_t * pAid,
                                      uint8_t bKeySettings1,
                                      uint8_t bKeySettings2,
                                      uint8_t * pISOFileId,
                                      uint8_t * pISODFName,
                                      uint8_t bISODFNameLen
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateApplication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeySettings1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeySettings2);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISOFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISODFName);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bISODFNameLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeySettings1_log, &bKeySettings1);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeySettings2_log, &bKeySettings2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bISODFNameLen_log, &bISODFNameLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAid_log, pAid, 3);
    if (bOption & 0x01)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISOFileId_log, pISOFileId, 2);
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISODFName_log, pISODFName, bISODFNameLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    if (bOption & 0x01) PH_ASSERT_NULL (pISOFileId);
    if (bISODFNameLen > 0) PH_ASSERT_NULL (pISODFName);
    PH_ASSERT_NULL (pAid);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateApplication((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            pAid,
            bKeySettings1,
            bKeySettings2,
            pISOFileId,
            pISODFName,
            bISODFNameLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CreateApplication((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            pAid,
            bKeySettings1,
            bKeySettings2,
            pISOFileId,
            pISODFName,
            bISODFNameLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CreateApplication((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            pAid,
            bKeySettings1,
            bKeySettings2,
            pISOFileId,
            pISODFName,
            bISODFNameLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_DeleteApplication(
                                      void * pDataParams,
                                      uint8_t * pAid
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_DeleteApplication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAid_log, pAid, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAid);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {

#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_DeleteApplication((phalMfdf_Sw_DataParams_t *)pDataParams, pAid);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_DeleteApplication((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pAid);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_DeleteApplication((phalMfdf_SamAV2_DataParams_t *)pDataParams, pAid);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetApplicationIDs(
                                      void * pDataParams,
                                      uint8_t * pAidBuff,
                                      uint8_t * pNumAIDs
                                      )
{
    /**
    A PICC can store upto 28 applications. PICC will return all
    AIDs (3 byte/aid) in single response if the number of applications <= 20
    else, it will send the  remaining AIDs in the second transmission. The
    first response sent by PICC will have 0xAF in the status byte.
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetApplicationIDs");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAidBuff);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumAIDs);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAidBuff);
    PH_ASSERT_NULL (pNumAIDs);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetApplicationIDs((phalMfdf_Sw_DataParams_t *)pDataParams, pAidBuff, pNumAIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetApplicationIDs((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pAidBuff, pNumAIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetApplicationIDs((phalMfdf_SamAV2_DataParams_t *)pDataParams, pAidBuff, pNumAIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAidBuff_log, pAidBuff, (*pNumAIDs) * 3);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pNumAIDs_log, pNumAIDs);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetDFNames(
                               void * pDataParams,
                               uint8_t bOption,
                               uint8_t * pDFBuffer,
                               uint8_t * pDFInfoLen
                               )
{
    /*
    Returns AID(3B), FID(2B), DF-Name(1..16B) in one frame.
    */
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetDFNames");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDFBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDFInfoLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pDFBuffer);
    PH_ASSERT_NULL (pDFInfoLen);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetDFNames((phalMfdf_Sw_DataParams_t *)pDataParams, bOption, pDFBuffer, pDFInfoLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetDFNames((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bOption, pDFBuffer, pDFInfoLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetDFNames((phalMfdf_SamAV2_DataParams_t *)pDataParams, bOption, pDFBuffer, pDFInfoLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFBuffer_log, pDFBuffer, (*pDFInfoLen));
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFInfoLen_log, pDFInfoLen, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_SelectApplication(
                                      void * pDataParams,
                                      uint8_t * pAppId
                                      )
{
    phStatus_t  PH_MEMLOC_REM status = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_SelectApplication");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAppId);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAppId_log, pAppId, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAppId);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }
    switch (PH_GET_COMPID(pDataParams))
    {

#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_SelectApplication((phalMfdf_Sw_DataParams_t *)pDataParams, pAppId);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_SelectApplication((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pAppId);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_SelectApplication((phalMfdf_SamAV2_DataParams_t *)pDataParams, pAppId);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_FormatPICC(
                               void * pDataParams
                               )
{
    phStatus_t  PH_MEMLOC_REM status = 0;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_FormatPICC");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_FormatPICC((phalMfdf_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_FormatPICC((phalMfdf_SamAV2_X_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_FormatPICC((phalMfdf_SamAV2_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetVersion(
                               void * pDataParams,
                               uint8_t * pVerInfo
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetVersion");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pVerInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pVerInfo);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetVersion((phalMfdf_Sw_DataParams_t *)pDataParams, pVerInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetVersion((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pVerInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetVersion((phalMfdf_SamAV2_DataParams_t *)pDataParams, pVerInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pVerInfo_log, pVerInfo, 28);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_FreeMem(
                            void * pDataParams,
                            uint8_t * pMemInfo
                            )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_FreeMem");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMemInfo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pMemInfo);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_FreeMem((phalMfdf_Sw_DataParams_t *)pDataParams, pMemInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_FreeMem((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pMemInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_FreeMem((phalMfdf_SamAV2_DataParams_t *)pDataParams, pMemInfo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMemInfo_log, pMemInfo, 3);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_SetConfiguration(
                                     void * pDataParams,
                                     uint8_t bOption,
                                     uint8_t * pData,
                                     uint8_t bDataSize
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_SetConfiguration");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDataSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDataSize_log, &bDataSize);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, bDataSize);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pData);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {

#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_SetConfiguration((phalMfdf_Sw_DataParams_t *)pDataParams, bOption, pData, bDataSize);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_SetConfiguration((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bOption, pData, bDataSize);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_SetConfiguration((phalMfdf_SamAV2_DataParams_t *)pDataParams, bOption, pData, bDataSize);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetCardUID(
                               void * pDataParams,
                               uint8_t * pUid
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetCardUID");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pUid);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetCardUID((phalMfdf_Sw_DataParams_t *)pDataParams, pUid);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetCardUID((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pUid);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetCardUID((phalMfdf_SamAV2_DataParams_t *)pDataParams, pUid);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetFileIDs(
                               void * pDataParams,
                               uint8_t * pResponse,
                               uint8_t * bNumFIDs
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetFileIDs");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pResponse);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNumFIDs);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pResponse);
    PH_ASSERT_NULL (bNumFIDs);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetFileIDs((phalMfdf_Sw_DataParams_t *)pDataParams, pResponse, bNumFIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetFileIDs((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pResponse, bNumFIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetFileIDs((phalMfdf_SamAV2_DataParams_t *)pDataParams, pResponse, bNumFIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pResponse_log, pResponse, (*bNumFIDs));
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bNumFIDs_log, bNumFIDs, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetISOFileIDs(
                                  void * pDataParams,
                                  uint8_t * pFidBuffer,
                                  uint8_t * pNumFIDs
                                  )
{

    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetISOFileIDs");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFidBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumFIDs);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pFidBuffer);
    PH_ASSERT_NULL (pNumFIDs);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetISOFileIDs((phalMfdf_Sw_DataParams_t *)pDataParams, pFidBuffer, pNumFIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetISOFileIDs((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, pFidBuffer, pNumFIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetISOFileIDs((phalMfdf_SamAV2_DataParams_t *)pDataParams, pFidBuffer, pNumFIDs);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFidBuffer_log, pFidBuffer, (*pNumFIDs) * 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pNumFIDs_log, pNumFIDs, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetFileSettings(
                                    void * pDataParams,
                                    uint8_t bFileNo,
                                    uint8_t * pFSBuffer,
                                    uint8_t * bBufferLen
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetFileSettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFSBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBufferLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pFSBuffer);
    PH_ASSERT_NULL (bBufferLen);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetFileSettings((phalMfdf_Sw_DataParams_t *)pDataParams, bFileNo, pFSBuffer, bBufferLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetFileSettings((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bFileNo, pFSBuffer, bBufferLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetFileSettings((phalMfdf_SamAV2_DataParams_t *)pDataParams, bFileNo, pFSBuffer, bBufferLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG,pFSBuffer_log, pFSBuffer, (*bBufferLen));
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, bBufferLen_log, bBufferLen, 1);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_ChangeFileSettings(
                                       void * pDataParams,
                                       uint8_t bOption,
                                       uint8_t bFileNo,
                                       uint8_t bCommSett,
                                       uint8_t *pAccessRights
                                       )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_ChangeFileSettings");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAccessRights);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_ChangeFileSettings((phalMfdf_Sw_DataParams_t *)pDataParams, bOption, bFileNo, bCommSett, pAccessRights);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_ChangeFileSettings((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bOption, bFileNo, bCommSett, pAccessRights);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_ChangeFileSettings((phalMfdf_SamAV2_DataParams_t *)pDataParams, bOption, bFileNo, bCommSett, pAccessRights);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_CreateStdDataFile(
                                      void * pDataParams,
                                      uint8_t bOption,
                                      uint8_t bFileNo,
                                      uint8_t *pISOFileId,
                                      uint8_t bCommSett,
                                      uint8_t *pAccessRights,
                                      uint8_t * pFileSize
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateStdDataFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISOFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFileSize);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFileSize_log, pFileSize, 3);
    if (bOption == 0x01)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISOFileId_log, pISOFileId, 2);
    }
    
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (bOption == 0x01) PH_ASSERT_NULL (pISOFileId);
    PH_ASSERT_NULL (pAccessRights);
    PH_ASSERT_NULL (pFileSize);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateStdDataFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CreateStdDataFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CreateStdDataFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_CreateBackupDataFile(
    void * pDataParams,
    uint8_t bOption,
    uint8_t bFileNo,
    uint8_t *pISOFileId,
    uint8_t bCommSett,
    uint8_t *pAccessRights,
    uint8_t * pFileSize
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateBackupDataFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pISOFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFileSize);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    if (bOption == 0x01)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pISOFileId_log, pISOFileId, 2);
    }
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFileSize_log, pFileSize, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (bOption == 1)PH_ASSERT_NULL (pISOFileId);
    PH_ASSERT_NULL (pAccessRights);
    PH_ASSERT_NULL (pFileSize);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateBackupDataFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CreateBackupDataFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CreateBackupDataFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pISOFileId,
            bCommSett,
            pAccessRights,
            pFileSize
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_CreateValueFile(
                                    void * pDataParams,
                                    uint8_t bFileNo,
                                    uint8_t bCommSett,
                                    uint8_t * pAccessRights,
                                    uint8_t * pLowerLmit,
                                    uint8_t * pUpperLmit,
                                    uint8_t * pValue,
                                    uint8_t bLimitedCredit
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateValueFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pLowerLmit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUpperLmit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLimitedCredit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pLowerLmit_log, pLowerLmit, 4);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUpperLmit_log, pUpperLmit, 4);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue, 4);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLimitedCredit_log, &bLimitedCredit);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAccessRights);
    PH_ASSERT_NULL (pLowerLmit);
    PH_ASSERT_NULL (pUpperLmit);
    PH_ASSERT_NULL (pValue);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateValueFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bFileNo,
            bCommSett,
            pAccessRights,
            pLowerLmit,
            pUpperLmit,
            pValue,
            bLimitedCredit
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CreateValueFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bFileNo,
            bCommSett,
            pAccessRights,
            pLowerLmit,
            pUpperLmit,
            pValue,
            bLimitedCredit
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CreateValueFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bFileNo,
            bCommSett,
            pAccessRights,
            pLowerLmit,
            pUpperLmit,
            pValue,
            bLimitedCredit
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_CreateLinearRecordFile(
    void * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )

{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateLinearRecordFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIsoFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRecordSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMaxNoOfRec);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);    
    if (bOption == 0x01)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIsoFileId_log, pIsoFileId, 2);
    }
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRecordSize_log, pRecordSize, 3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMaxNoOfRec_log, pMaxNoOfRec, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    if (bOption==1) PH_ASSERT_NULL (pIsoFileId);
    PH_ASSERT_NULL (pAccessRights);
    PH_ASSERT_NULL (pRecordSize);
    PH_ASSERT_NULL (pMaxNoOfRec);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateLinearRecordFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pIsoFileId,
            bCommSett,
            pAccessRights,
            pRecordSize,
            pMaxNoOfRec
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CreateLinearRecordFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pIsoFileId,
            bCommSett,
            pAccessRights,
            pRecordSize,
            pMaxNoOfRec
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CreateLinearRecordFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pIsoFileId,
            bCommSett,
            pAccessRights,
            pRecordSize,
            pMaxNoOfRec
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2*/

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_CreateCyclicRecordFile(
    void * pDataParams,
    uint8_t bOption,
    uint8_t  bFileNo,
    uint8_t  *pIsoFileId,
    uint8_t bCommSett,
    uint8_t * pAccessRights,
    uint8_t * pRecordSize,
    uint8_t * pMaxNoOfRec
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CreateCyclicRecordFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pIsoFileId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommSett);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pAccessRights);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRecordSize);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pMaxNoOfRec);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    if (bOption == 0x01)
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pIsoFileId_log, pIsoFileId, 2);
    }    
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommSett_log, &bCommSett);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pAccessRights_log, pAccessRights, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pRecordSize_log, pRecordSize, 3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pMaxNoOfRec_log, pMaxNoOfRec, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pAccessRights);
    if (bOption == 0x01) PH_ASSERT_NULL (pIsoFileId);
    PH_ASSERT_NULL (pRecordSize);
    PH_ASSERT_NULL (pMaxNoOfRec);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CreateCyclicRecordFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pIsoFileId,
            bCommSett,
            pAccessRights,
            pRecordSize,
            pMaxNoOfRec
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CreateCyclicRecordFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pIsoFileId,
            bCommSett,
            pAccessRights,
            pRecordSize,
            pMaxNoOfRec
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CreateCyclicRecordFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pIsoFileId,
            bCommSett,
            pAccessRights,
            pRecordSize,
            pMaxNoOfRec
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_DeleteFile(
                               void * pDataParams,
                               uint8_t bFileNo
                               )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_DeleteFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_DeleteFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bFileNo
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_DeleteFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bFileNo
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_DeleteFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bFileNo
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_ReadData(
                             void * pDataParams,
                             uint8_t bOption,
                             uint8_t bFileNo,
                             uint8_t * pOffset,
                             uint8_t * pLength,
                             uint8_t ** ppRxdata,
                             uint16_t * pRxdataLen
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_ReadData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pLength);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxdata);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxdataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset, 3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pLength_log, pLength, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pOffset);
    PH_ASSERT_NULL (pLength);
    PH_ASSERT_NULL (ppRxdata);
    PH_ASSERT_NULL (pRxdataLen);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_ReadData((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pOffset,
            pLength,
            ppRxdata,
            pRxdataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_ReadData((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pOffset,
            pLength,
            ppRxdata,
            pRxdataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_ReadData((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            bFileNo,
            pOffset,
            pLength,
            ppRxdata,
            pRxdataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if (*pRxdataLen != 0)
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxdata_log, *ppRxdata, (*pRxdataLen));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pRxdataLen_log, pRxdataLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_WriteData(
                              void * pDataParams,
                              uint8_t bCommOption,
                              uint8_t bFileNo,
                              uint8_t * pOffset,
                              uint8_t * pTxData,
                              uint8_t * pTxDataLen
                              )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_WriteData");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset, 3);

    wDataLen = (uint16_t)pTxDataLen[1];
    wDataLen = wDataLen << 8;
    wDataLen |= pTxDataLen[0];

    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxData_log, pTxData, wDataLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxDataLen_log, pTxDataLen, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pOffset);
    PH_ASSERT_NULL (pTxData);
    PH_ASSERT_NULL (pTxDataLen);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_WriteData((phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pTxData,
            pTxDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_WriteData((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pTxData,
            pTxDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_WriteData((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pTxData,
            pTxDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_GetValue(
                             void * pDataParams,
                             uint8_t bCommOption,
                             uint8_t bFileNo,
                             uint8_t * pValue
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetValue");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetValue((phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetValue((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetValue((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue,4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_Credit(
                           void * pDataParams,
                           uint8_t bCommOption,
                           uint8_t bFileNo,
                           uint8_t * pValue
                           )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_Credit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_Credit(
            (phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_Credit(
            (phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_Credit((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue,4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;

}

phStatus_t phalMfdf_Debit(
                          void * pDataParams,
                          uint8_t bCommOption,
                          uint8_t bFileNo,
                          uint8_t * pValue
                          )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_Debit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_Debit(
            (phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_Debit(
            (phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_Debit(
            (phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG,pValue_log, pValue,4);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_LimitedCredit(
                                  void * pDataParams,
                                  uint8_t bCommOption,
                                  uint8_t bFileNo,
                                  uint8_t * pValue
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_LimitedCredit");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue,4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_LimitedCredit(
            (phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_LimitedCredit(
            (phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_LimitedCredit(
            (phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pValue
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_WriteRecord(
                                void * pDataParams,
                                uint8_t bCommOption,
                                uint8_t bFileNo,
                                uint8_t * pOffset,
                                uint8_t * pData,
                                uint8_t * pDataLen
                                )
{
    phStatus_t  PH_MEMLOC_REM status;
    uint16_t    PH_MEMLOC_REM wDataLen;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_WriteRecord");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataLen);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset,3);

    wDataLen = (uint16_t)pDataLen[1];
    wDataLen = wDataLen << 8;
    wDataLen |= pDataLen[0];

    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, wDataLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDataLen_log, pDataLen, 3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pOffset);
    PH_ASSERT_NULL (pData);
    PH_ASSERT_NULL (pDataLen);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_WriteRecord((phalMfdf_Sw_DataParams_t *)pDataParams, bCommOption, bFileNo, pOffset, pData, pDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_WriteRecord((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bCommOption, bFileNo, pOffset, pData, pDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_WriteRecord((phalMfdf_SamAV2_DataParams_t *)pDataParams, bCommOption, bFileNo, pOffset, pData, pDataLen);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_ReadRecords(
                                void * pDataParams,
                                uint8_t bCommOption,
                                uint8_t bFileNo,
                                uint8_t * pOffset,
                                uint8_t * pNumRec,
                                uint8_t * pRecSize,
                                uint8_t ** ppRxdata,
                                uint16_t * pRxdataLen
                                )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_ReadRecords");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCommOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNumRec);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxdata);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRxdataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCommOption_log, &bCommOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pOffset_log, pOffset,3);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pNumRec_log, pNumRec,3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pOffset);
    PH_ASSERT_NULL (pNumRec);
    PH_ASSERT_NULL (ppRxdata);
    PH_ASSERT_NULL (pRxdataLen);

    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_ReadRecords((phalMfdf_Sw_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pNumRec,
            pRecSize,
            ppRxdata,
            pRxdataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_ReadRecords((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pNumRec,
            pRecSize,
            ppRxdata,
            pRxdataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_ReadRecords((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bCommOption,
            bFileNo,
            pOffset,
            pNumRec,
            pRecSize,
            ppRxdata,
            pRxdataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    if (*pRxdataLen != 0)
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG,ppRxdata_log, *ppRxdata, (*pRxdataLen));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pRxdataLen_log, pRxdataLen);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_ClearRecordFile(
                                    void * pDataParams,
                                    uint8_t bFileNo
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_ClearRecordFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bFileNo);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bFileNo_log, &bFileNo);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_ClearRecordFile((phalMfdf_Sw_DataParams_t *)pDataParams, bFileNo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_ClearRecordFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, bFileNo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_ClearRecordFile((phalMfdf_SamAV2_DataParams_t *)pDataParams, bFileNo);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_CommitTransaction(
                                      void * pDataParams
                                      )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_CommitTransaction");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_CommitTransaction((phalMfdf_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_CommitTransaction((phalMfdf_SamAV2_X_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_CommitTransaction((phalMfdf_SamAV2_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_AbortTransaction(
                                     void * pDataParams
                                     )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_AbortTransaction");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_AbortTransaction((phalMfdf_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_AbortTransaction((phalMfdf_SamAV2_X_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_AbortTransaction((phalMfdf_SamAV2_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoSelectFile(
                                  void * pDataParams,
                                  uint8_t bOption,
                                  uint8_t bSelector,
                                  uint8_t * pFid,
                                  uint8_t * pDFname,
                                  uint8_t bDFnameLen,
                                  uint8_t **ppFCI,
                                  uint16_t *pwFCILen
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoSelectFile");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSelector);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pFid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDFname);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDFnameLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOption_log, &bOption);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSelector_log, &bSelector);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pFid_log, pFid,2);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDFnameLen_log, &bDFnameLen);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pDFname_log, pDFname,bDFnameLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoSelectFile((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOption,
            bSelector,
            pFid,
            pDFname,
            bDFnameLen,
            ppFCI,
            pwFCILen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoSelectFile((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOption,
            bSelector,
            pFid,
            pDFname,
            bDFnameLen,
            ppFCI,
            pwFCILen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoSelectFile((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOption,
            bSelector,
            pFid,
            pDFname,
            bDFnameLen,
            ppFCI,
            pwFCILen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoReadBinary(
                                  void * pDataParams,
                                  uint16_t wOption,
                                  uint8_t bOffset,
                                  uint8_t bSfid,
                                  uint8_t bBytesToRead,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pBytesRead
                                  )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoReadBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBytesToRead);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBytesRead);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBytesToRead_log, &bBytesToRead);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pBytesRead);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoReadBinary((phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            bOffset,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoReadBinary((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wOption,
            bOffset,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoReadBinary((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wOption,
            bOffset,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, (*pBytesRead));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, pBytesRead_log, pBytesRead);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoUpdateBinary(
                                    void * pDataParams,
                                    uint8_t bOffset,
                                    uint8_t bSfid,
                                    uint8_t * pData,
                                    uint8_t bDataLen
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoUpdateBinary");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bOffset);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bOffset_log, &bOffset);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData,bDataLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_INFO, bDataLen_log, &bDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pData);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoUpdateBinary((phalMfdf_Sw_DataParams_t *)pDataParams,
            bOffset,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoUpdateBinary((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bOffset,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoUpdateBinary((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bOffset,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoReadRecords(
                                   void * pDataParams,
                                   uint16_t wOption,
                                   uint8_t bRecNo,
                                   uint8_t bReadAllFromP1,
                                   uint8_t bSfid,
                                   uint8_t bBytesToRead,
                                   uint8_t ** ppRxBuffer,
                                   uint16_t * pBytesRead
                                   )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoReadRecords");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bRecNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bReadAllFromP1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBytesToRead);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pBytesRead);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bRecNo_log, &bRecNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bReadAllFromP1_log, &bReadAllFromP1);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBytesToRead_log, &bBytesToRead);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (ppRxBuffer);
    PH_ASSERT_NULL (pBytesRead);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoReadRecords((phalMfdf_Sw_DataParams_t *)pDataParams,
            wOption,
            bRecNo,
            bReadAllFromP1,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoReadRecords((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wOption,
            bRecNo,
            bReadAllFromP1,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoReadRecords((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wOption,
            bRecNo,
            bReadAllFromP1,
            bSfid,
            bBytesToRead,
            ppRxBuffer,
            pBytesRead
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, (*pBytesRead));
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pBytesRead_log, pBytesRead);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoAppendRecord(
                                    void * pDataParams,
                                    uint8_t bSfid,
                                    uint8_t * pData,
                                    uint8_t bDataLen
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoAppendRecord");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSfid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pData);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bDataLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSfid_log, &bSfid);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pData_log, pData, bDataLen);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bDataLen_log, &bDataLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pData);
    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoAppendRecord((phalMfdf_Sw_DataParams_t *)pDataParams,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoAppendRecord((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoAppendRecord((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            bSfid,
            pData,
            bDataLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoGetChallenge(
                                    void * pDataParams,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVer,
                                    uint8_t bLe,
                                    uint8_t * pRPICC1
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoGetChallenge");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bLe);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pRPICC1);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVer_log, &wKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bLe_log, &bLe);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pRPICC1);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoGetChallenge((phalMfdf_Sw_DataParams_t *)pDataParams, wKeyNo, wKeyVer, bLe, pRPICC1);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoGetChallenge((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, wKeyNo, wKeyVer, bLe, pRPICC1);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoGetChallenge((phalMfdf_SamAV2_DataParams_t *)pDataParams, wKeyNo, wKeyVer, bLe, pRPICC1);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_INFO, pRPICC1_log, pRPICC1, 8);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoExternalAuthenticate(
    void * pDataParams,
    uint8_t * pDataIn,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoExternalAuthenticate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bInputLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bInputLen_log, &bInputLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pDataIn);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoExternalAuthenticate((phalMfdf_Sw_DataParams_t *)pDataParams,
            pDataIn,
            bInputLen,
            pDataOut,
            pOutLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoExternalAuthenticate((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            pDataIn,
            bInputLen,
            pDataOut,
            pOutLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoExternalAuthenticate((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            pDataIn,
            bInputLen,
            pDataOut,
            pOutLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoInternalAuthenticate(
    void * pDataParams,
    uint8_t * pDataIn,
    uint8_t bInputLen,
    uint8_t * pDataOut,
    uint8_t * pOutLen
    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoInternalAuthenticate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bInputLen);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bInputLen_log, &bInputLen);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);

    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pDataIn);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoInternalAuthenticate((phalMfdf_Sw_DataParams_t *)pDataParams,
            pDataIn,
            bInputLen,
            pDataOut,
            pOutLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoInternalAuthenticate((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            pDataIn,
            bInputLen,
            pDataOut,
            pOutLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoInternalAuthenticate((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            pDataIn,
            bInputLen,
            pDataOut,
            pOutLen
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

phStatus_t phalMfdf_IsoAuthenticate(
                                    void * pDataParams,
                                    uint16_t wKeyNo,
                                    uint16_t wKeyVer,
                                    uint8_t bKeyNoCard,
                                    uint8_t isPICCkey
                                    )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_IsoAuthenticate");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyNoCard);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(isPICCkey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVer_log, &wKeyVer);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyNoCard_log, &bKeyNoCard);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, isPICCkey_log, &isPICCkey);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* parameter validation */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_IsoAuthenticate((phalMfdf_Sw_DataParams_t *)pDataParams,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            isPICCkey
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_IsoAuthenticate((phalMfdf_SamAV2_X_DataParams_t *)pDataParams,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            isPICCkey
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */

#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_IsoAuthenticate((phalMfdf_SamAV2_DataParams_t *)pDataParams,
            wKeyNo,
            wKeyVer,
            bKeyNoCard,
            isPICCkey
            );
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

	return status;
}

phStatus_t phalMfdf_GetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t * pValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_GetConfig((phalMfdf_Sw_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */
#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_GetConfig((phalMfdf_SamAV2_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */
#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_GetConfig((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, wConfig, pValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */
    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
    {
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}


phStatus_t phalMfdf_SetConfig(
                              void * pDataParams,
                              uint16_t wConfig,
                              uint16_t wValue
                              )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phalMfdf_GetConfig");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_AL_MFDF)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHAL_MFDF_SW
    case PHAL_MFDF_SW_ID:
        status = phalMfdf_Sw_SetConfig((phalMfdf_Sw_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SW */
#ifdef NXPBUILD__PHAL_MFDF_SAMAV2
    case PHAL_MFDF_SAMAV2_ID:
        status = phalMfdf_SamAV2_SetConfig((phalMfdf_SamAV2_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2 */
#ifdef NXPBUILD__PHAL_MFDF_SAMAV2_X
    case PHAL_MFDF_SAMAV2_X_ID:
        status = phalMfdf_SamAV2_X_SetConfig((phalMfdf_SamAV2_X_DataParams_t *)pDataParams, wConfig, wValue);
        break;
#endif /* NXPBUILD__PHAL_MFDF_SAMAV2_X */
    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFDF);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPBUILD__PHAL_MFDF */
