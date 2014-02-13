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
* BAL Stub Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:04:54 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/
#if !defined RC663_RS232 && !defined RC663_I2C
	#error "Must define one of RC663_RS232 and RC663_I2C"
#endif

#if defined RC663_RS232 && defined RC663_I2C
	#error "MACRO RC663_RS232 and RC663_I2C can't defind at same time"
#endif

#include <ph_Status.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>
#include <sys/types.h>		//add by jshli
#include <sys/stat.h>		//add by jshli		
#include <fcntl.h>		//add by jshli
#include <unistd.h>		//add by jshli
#include <string.h>		//add by jshli
#ifdef RC663_RS232
#include <sys/select.h>		//add by jshli
#include <sys/time.h>		//add by jshli
#include <termios.h>		//add by jshli
#endif


#ifdef NXPBUILD__PHBAL_REG_STUB

#include "phbalReg_Stub.h"

#include "android/log.h"
static const char *TAG = "CLRC663_nxp";
//#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
//#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
//#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)
#define LOGI(fmt, args...)
#define LOGD(fmt, args...)
#define LOGE(fmt, args...)

#ifdef RC663_RS232
static void setTermios(struct termios * pNewtio, int uBaudRate, int cSize, int cStop)
{
//	bzero(pNewtio, sizeof(struct termios)); /* clear struct for new port settings */
	memset(pNewtio, 0, sizeof(struct termios));
	//8N1
//	pNewtio->c_cflag = uBaudRate | CS8 | CREAD | CLOCAL | CSTOPB;
	pNewtio->c_cflag = uBaudRate | cSize | cStop | CREAD | CLOCAL;
	pNewtio->c_iflag = IGNPAR;
	pNewtio->c_oflag = 0;
	pNewtio->c_lflag = 0; //non ICANON

	pNewtio->c_cc[VINTR] = 0; /* Ctrl-c */
	pNewtio->c_cc[VQUIT] = 0; /* Ctrl-\ */
	pNewtio->c_cc[VERASE] = 0; /* del */
	pNewtio->c_cc[VKILL] = 0; /* @ */
	pNewtio->c_cc[VEOF] = 4; /* Ctrl-d */
	pNewtio->c_cc[VTIME] = 5; /* inter-character timer, timeout VTIME*0.1 */
	pNewtio->c_cc[VMIN] = 0; /* blocking read until VMIN character arrives */
	pNewtio->c_cc[VSWTC] = 0; /* '\0' */
	pNewtio->c_cc[VSTART] = 0; /* Ctrl-q */
	pNewtio->c_cc[VSTOP] = 0; /* Ctrl-s */
	pNewtio->c_cc[VSUSP] = 0; /* Ctrl-z */
	pNewtio->c_cc[VEOL] = 0; /* '\0' */
	pNewtio->c_cc[VREPRINT] = 0; /* Ctrl-r */
	pNewtio->c_cc[VDISCARD] = 0; /* Ctrl-u */
	pNewtio->c_cc[VWERASE] = 0; /* Ctrl-w */
	pNewtio->c_cc[VLNEXT] = 0; /* Ctrl-v */
	pNewtio->c_cc[VEOL2] = 0; /* '\0' */
}
#endif
//#include <RegCtl_I2cHw.h>
//#define DEV_PATH	"/dev/rc663"
//static int dev_fd;

phStatus_t phbalReg_Stub_Init(
                              phbalReg_Stub_DataParams_t * pDataParams,
                              uint16_t wSizeOfDataParams
                              )
{
    if (sizeof(phbalReg_Stub_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    }
	PH_ASSERT_NULL (pDataParams);

    pDataParams->wId = PH_COMP_BAL | PHBAL_REG_STUB_ID;
    memset(pDataParams->devPath, 0, DEV_PATH_LENGTH);	//add by jshli
    pDataParams->fd = 0;				//add by jshli

    /* TODO: Replace and insert initialisation code here */
//    pDataParams = NULL;

//    RegCtl_I2cHwInit();
//	dev_fd = open(DEV_PATH, O_RDWR);
//	if(dev_fd < 0)
//	{
//		return PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
//	}//add by jshli

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetPortList(
    phbalReg_Stub_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    )
{
    /* TODO: Replace and insert code for getting the port list here */
    pDataParams = NULL;
    wPortBufSize = 0;
    pPortNames = NULL;
    *pNumOfPorts = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_SetPort(
                            phbalReg_Stub_DataParams_t * pDataParams,
                            uint8_t * pPortName
                            )
{
    /* TODO: Replace and insert code for setting a port name here */
    pDataParams = NULL;
    pPortName = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_OpenPort(
                                  phbalReg_Stub_DataParams_t * pDataParams
                                  )
{
    /* TODO: Replace and insert code for opening a bus connection here */
//    pDataParams = NULL;
#ifdef RC663_RS232
	struct termios newtio;
#endif
	pDataParams->fd = open(pDataParams->devPath, O_RDWR);
	if(pDataParams->fd < 0)
	{
		LOGE("open port failed");
		return PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
	}//add by jshli
#ifdef RC663_RS232
	setTermios(&newtio, B115200, CS8, 0);	// 115200bps 8n1
	tcflush(pDataParams->fd, TCIFLUSH);
	if(tcsetattr(pDataParams->fd, TCSANOW, &newtio) < 0)
	{
		LOGE("change serial port param failed");
		return PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
	}
	LOGD("set serialport to 115200 8n1, fd is %d", pDataParams->fd);
#endif

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_ClosePort(
                                   phbalReg_Stub_DataParams_t * pDataParams
                                   )
{
    /* TODO: Replace and insert code for closing a bus connection here */
//    pDataParams = NULL;
	close(pDataParams->fd);
	LOGD("close serialport");

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_Exchange(
                                  phbalReg_Stub_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength,
                                  uint16_t wRxBufSize,
                                  uint8_t * pRxBuffer,
                                  uint16_t * pRxLength
                                  )
{
    /* TODO: Replace and insert code for exchanging data on the bus here */
	phStatus_t status = PH_ERR_SUCCESS;
	int reval;

#ifdef RC663_RS232
	struct timeval tv;
	fd_set rfds;

	if(wTxLength == 2)
	{
		LOGD("perform a write cmd");
		LOGD("want to write reg is %d", *pTxBuffer);
		LOGD("want to write value is %d", *(pTxBuffer + 1));

	}
	else
	{
		LOGD("perform a read cmd");
		LOGD("want to read reg is %d", (*pTxBuffer) & 0x7f);
	}
	reval = write(pDataParams->fd, pTxBuffer, wTxLength);
	if(reval != wTxLength)
	{
		LOGE("write reg address failed, return value is %d", reval);
		status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
		*pRxLength = 0;
		return PH_ADD_COMPCODE(status, PH_COMP_BAL);
	}
	FD_ZERO(&rfds);
	FD_SET(pDataParams->fd, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	if((reval = select(1 + pDataParams->fd, &rfds, NULL, NULL, &tv)) > 0)
	{
		if(FD_ISSET(pDataParams->fd, &rfds))
		{
			reval = read(pDataParams->fd, pRxBuffer, wRxBufSize);
			if(reval != wRxBufSize)
			{
				LOGE("read port failed");
				status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
				*pRxLength = 0;
				return PH_ADD_COMPCODE(status, PH_COMP_BAL);
			}
			LOGD("ok to read return value is %d", *pRxBuffer);
			LOGD("%d us is used", 100000 - tv.tv_usec);

			*pRxLength = wRxBufSize;
			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
		else
		{
			LOGE("not the watch fd is ready to read");
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
			*pRxLength = 0;
			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
	}
	else
	{
		if(reval == 0)
		{
			LOGE("time out while waiting return value");
		}
		else
		{
			LOGE("other errro");
		}
		status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
		*pRxLength = 0;
		return PH_ADD_COMPCODE(status, PH_COMP_BAL);
	}
#endif

#ifdef RC663_I2C
	/* write operation ? */
	if (0 == wRxBufSize)
	{
		/* Yes -> perform write operation */
//		status = RegCtl_I2cHwSetReg(pTxBuffer[0], pTxBuffer[1]);
		reval = pwrite(pDataParams->fd, &pTxBuffer[1], 1, pTxBuffer[0]);
		if(reval < 0)
		{
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
		}
		/* no bytes read */
		*pRxLength = 0;
	}
	else
	{
		// perform read operation
//		status = RegCtl_I2cHwGetReg(pTxBuffer[0], pRxBuffer);
		reval = pread(pDataParams->fd, pRxBuffer, 1, pTxBuffer[0]);
		if(reval < 0)
		{
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
			*pRxLength = 0;
    			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
		// 1 byte read returned
		*pRxLength = 1;
	}
//
	return PH_ADD_COMPCODE(status, PH_COMP_BAL);

#endif	

#if 0
//    pDataParams = NULL;
//    wOption = PH_EXCHANGE_DEFAULT;
//    pTxBuffer = NULL;
//    wTxLength = 0;
//    wRxBufSize = 0;
//    pRxBuffer = NULL;
//    pRxLength = NULL;

	/* write operation ? */
//	if (0 == wRxBufSize)
//	{
		/* Yes -> perform write operation */
//		status = RegCtl_I2cHwSetReg(pTxBuffer[0], pTxBuffer[1]);
/*		reval = pwrite(pDataParams->fd, &pTxBuffer[1], 1, pTxBuffer[0]);
		if(reval < 0)
		{
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
		}*/
		if(wTxLength == 2)
		{
			LOGD("perform a write cmd");
			LOGD("want to write reg is %d", *pTxBuffer);
			LOGD("want to write value is %d", *(pTxBuffer + 1));

		}
		else
		{
			LOGD("perform a read cmd");
			LOGD("want to read reg is %d", (*pTxBuffer) & 0x7f);
		}
		reval = write(pDataParams->fd, pTxBuffer, wTxLength);
		if(reval != wTxLength)
		{
			LOGE("write reg address failed, return value is %d", reval);
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
			*pRxLength = 0;
			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
/*		if(wTxLength == 2)
		{
			*pRxBuffer = *pTxBuffer;
			*pRxLength = wRxBufSize;
    			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}*/
		FD_ZERO(&rfds);
		FD_SET(pDataParams->fd, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		if((reval = select(1 + pDataParams->fd, &rfds, NULL, NULL, &tv)) > 0)
		{
			if(FD_ISSET(pDataParams->fd, &rfds))
			{
				reval = read(pDataParams->fd, pRxBuffer, wRxBufSize);
				if(reval != wRxBufSize)
				{
					LOGE("read port failed");
					status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
					*pRxLength = 0;
					return PH_ADD_COMPCODE(status, PH_COMP_BAL);
				}
				LOGD("ok to read return value is %d", *pRxBuffer);
				LOGD("%d us is used", 100000 - tv.tv_usec);
			}
			else
			{
				LOGE("not the watch fd is ready to read");
				status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
				*pRxLength = 0;
				return PH_ADD_COMPCODE(status, PH_COMP_BAL);
			}
		}
		else
		{
			if(reval == 0)
			{
				LOGE("time out while waiting return value");
			}
			else
			{
				LOGE("other errro");
			}
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
			*pRxLength = 0;
    			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
/*		if(wTxLength == 2)
		{
			reval = write(pDataParams->fd, pTxBuffer + 1, 1);
			if(reval != 1)
			{
				LOGE("write reg value failed, return value is %d", reval);
				status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
				*pRxLength = 0;
				return PH_ADD_COMPCODE(status, PH_COMP_BAL);
			}
		}*/
		/* no bytes read */
		*pRxLength = wRxBufSize;
/*	}
	else
	{
		// perform read operation
//		status = RegCtl_I2cHwGetReg(pTxBuffer[0], pRxBuffer);
//		reval = pread(pDataParams->fd, pRxBuffer, 1, pTxBuffer[0]);
//		if(reval < 0)
//		{
//			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
//		}
		reval = write(pDataParams->fd, pTxBuffer, wTxLength);
		if(reval != 1)
		{
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
			*pRxLength = 0;
    			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
		FD_ZERO(&rfds);
		FD_SET(pDataParams->fd, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		if((reval = select(1 + pDataParams->fd, &rfds, NULL, NULL, &tv)) > 0)
		{
			if(FD_ISSET(pDataParams->fd, &rfds))
			{
				reval = read(pDataParams->fd, pRxBuffer, 1);
				if(reval != 1)
				{
					status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
					*pRxLength = 0;
					return PH_ADD_COMPCODE(status, PH_COMP_BAL);
				}
			}
			else
			{
				status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
				*pRxLength = 0;
				return PH_ADD_COMPCODE(status, PH_COMP_BAL);
			}
		}
		else
		{
			status = PH_ADD_COMPCODE(PH_ERR_IO_TIMEOUT, PH_COMP_BAL);
			*pRxLength = 0;
    			return PH_ADD_COMPCODE(status, PH_COMP_BAL);
		}
		// 1 byte read returned
		*pRxLength = 1;
	}*/

    return PH_ADD_COMPCODE(status, PH_COMP_BAL);
#endif
}

phStatus_t phbalReg_Stub_SetConfig(
                                   phbalReg_Stub_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t wValue
                                   )
{
    /* TODO: Replace and insert code to configure the connection here */
    pDataParams = NULL;
    wConfig = 0;
    wValue = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetConfig(
                                   phbalReg_Stub_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   )
{
    /* TODO: Replace and insert code to read connection configuration here */
    pDataParams = NULL;
    wConfig = 0;
    pValue = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_STUB */
