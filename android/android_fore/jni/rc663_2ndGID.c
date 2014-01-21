#include <jni.h>
#include <unistd.h>
#include <string.h>

#include <ph_NxpBuild.h>
#include <ph_Status.h>
#include <phhalHw.h>
#include <phbalReg.h>
#include <phpalI14443p3b.h>

#include "android/log.h"
extern const char *JNI_TAG_2nd;
extern const char *JNI_CLASS_2nd;
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  JNI_TAG_2nd, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG_2nd, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, JNI_TAG_2nd, fmt, ##args)

#if !defined RC663_RS232 && !defined RC663_I2C
	#error "Must define one of RC663_RS232 and RC663_I2C"
#endif

#if defined RC663_RS232 && defined RC663_I2C
	#error "MACRO RC663_RS232 and RC663_I2C can't defined at same time"
#endif

JNIEXPORT jint JNICALL init_1dev(JNIEnv *, jobject, jstring);
JNIEXPORT void JNICALL release_1dev(JNIEnv *, jobject);
JNIEXPORT jbyteArray JNICALL search_1card(JNIEnv *, jobject);
JNIEXPORT jbyteArray JNICALL get_1uid(JNIEnv *, jobject);

static phStatus_t status;
static phbalReg_Stub_DataParams_t balReader;
static phhalHw_Rc663_DataParams_t halReader;
static phpalI14443p3b_Sw_DataParams_t  palI14443p3B;
static uint8_t bHalBufferReader[0x400];

static JNINativeMethod methods[] = {
	{"init_dev", "(Ljava/lang/String;)I", (void *)init_1dev},
	{"release_dev", "()V", (void *)release_1dev},
	{"search_card", "()[B", (void *)search_1card},
	{"get_uid", "()[B", (void *)get_1uid},
};

static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *Methods, int numMethods)
{
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if(clazz == NULL)
	{
		LOGE("Can't find jclass: %s", className);
		return JNI_FALSE;
	}
	if((*env)->RegisterNatives(env, clazz, Methods, numMethods) < 0)
	{
		LOGE("Register real action failed %s", className);
		return JNI_FALSE;
	}
	return JNI_TRUE;
}

static int register_func(JNIEnv *env)
{
	if(!registerNativeMethods(env, JNI_CLASS_2nd, methods, sizeof(methods) / sizeof(methods[0])))
	{
		return JNI_FALSE;
	}
	return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	JNIEnv *env = NULL;
	LOGI("JNI_OnLoad");

	if((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGE("ERROR: GetEnv failed");
		return -1;
	}

	if(!register_func(env))
	{
		LOGE("ERROR: registerNatives failed");
		return -1;
	}
	return JNI_VERSION_1_4;
}

JNIEXPORT jint JNICALL init_1dev(JNIEnv *env, jobject obj, jstring path)
{    
	const char *dev = NULL;
	jboolean iscopy;
	/* Initialize the Reader BAL (Bus Abstraction Layer) component */
	status = phbalReg_Stub_Init(&balReader, sizeof(phbalReg_Stub_DataParams_t));
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("bal layout init ok");
	dev = (*env)->GetStringUTFChars(env, path, &iscopy);
	strncpy(balReader.devPath, dev, DEV_PATH_LENGTH);
	LOGD("now open %s\n", dev);
	(*env)->ReleaseStringUTFChars(env, path, dev);
	status = phbalReg_OpenPort(&balReader);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("open device file failed");
		return -1;
	}
	LOGD("open device file ok");

	/* Initialize the Reader HAL (Hardware Abstraction Layer) component */
	status = phhalHw_Rc663_Init(&halReader, sizeof(phhalHw_Rc663_DataParams_t), &balReader, 0, bHalBufferReader, sizeof(bHalBufferReader), bHalBufferReader, sizeof(bHalBufferReader));
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	/* Set the parameter to use the I2C interface */
#ifdef	RC663_RS232
	halReader.bBalConnectionType = PHHAL_HW_BAL_CONNECTION_RS232;
#endif
#ifdef	RC663_I2C
	halReader.bBalConnectionType = PHHAL_HW_BAL_CONNECTION_I2C;
#endif
	LOGD("hal layout init ok");

	status = phpalI14443p3b_Sw_Init(&palI14443p3B, sizeof(palI14443p3B), &halReader);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("pal 14443-3b init ok");

	/* reset the IC  */
	status = phhalHw_Rc663_Cmd_SoftReset(&halReader);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Soft reset failed, status is 0x%x", status);
		return -1;
	}
	LOGD("Soft reset ok!");

	status = phhalHw_ApplyProtocolSettings(&halReader, PHHAL_HW_CARDTYPE_ISO14443B);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Load ISO/IEC 14443-3B protocol failed, status is 0x%x", status);
		return -1;
	}
	LOGD("Init all ok!");

	/* Reset the RF field */
	status = phhalHw_FieldReset(&halReader);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("RF reset failed, status is 0x%x", status);
		return -1;
	}
	LOGD("RF rest ok!");

	return 0;
}

JNIEXPORT void JNICALL release_1dev(JNIEnv *env, jobject obj)
{
	LOGD("Close device file");
	phbalReg_ClosePort(&balReader);
}

JNIEXPORT jbyteArray JNICALL search_1card(JNIEnv *env, jobject obj)
{
	uint8_t pPupi[4], pAtqb[13], bAtqbLen, bMbli;
	int iq;
	jbyteArray jba = NULL;

	status = phpalI14443p3b_RequestB(&palI14443p3B, 0, 0, 0, pAtqb, &bAtqbLen);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("RequestB is failed, status is 0x%x", status);
		return NULL;
	}
	LOGD("Type B card detected, ATQB value is ");
	for(iq = 0; iq < bAtqbLen; iq++)
	{
		LOGD("%02X ", pAtqb[iq]);
	}

	status = phpalI14443p3b_GetSerialNo(&palI14443p3B, pPupi);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Get PUPI error, status is 0x%x", status);
		return NULL;
	}
	LOGD("PUPI value is ");
	for(iq = 0; iq < 4; iq++)
	{
		LOGD("%02X ", pPupi[iq]);
	}

	pAtqb[10] |= 1;
	pAtqb[11] |= 1;

	status = phpalI14443p3b_Attrib(&palI14443p3B, pAtqb, bAtqbLen, 8, 8, 0, 0, &bMbli);
	if(PH_ERR_SUCCESS != status)
	{
		LOGE("Attrib card failed, status is 0x%x", status);
		return NULL;
	}
	LOGD("bMbli valud is 0x%x", bMbli);

	jba = (*env)->NewByteArray(env, 4);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in search_card func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, 4, pPupi);

	LOGD("Search and select card ok, get pupi");
	return jba;
}

JNIEXPORT jbyteArray JNICALL get_1uid(JNIEnv *env, jobject obj)
{
	uint8_t *p, CMd[5] = {0, 0x36, 0, 0, 8};
	uint16_t plength;
	int iq;
	jbyteArray jba = NULL;
	
	status = phhalHw_Exchange(&halReader, PH_EXCHANGE_DEFAULT, CMd, 5, &p, &plength);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("GUID exchange failed, status is 0x%x", status);
		return NULL;
	}
	if(plength < 8)
	{
		LOGE("Get UID is not enough, only %d of 8 readed", plength);
		return NULL;
	}
	LOGD("UID is ");
	for(iq = 0; iq < 8; iq++)
	{
		LOGD("%02X ", p[iq]);
	}

	jba = (*env)->NewByteArray(env, 8);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in get_uid func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, 8, p);

	LOGD("Get UID ok");
	return jba;
}

/*	uint8_t pPupi[4], bPupiLength;
	uint8_t bNumSlots;
	uint8_t bAfi, pAtqb[13], bAtqbLen, bExtAtqb;
	uint8_t bCid, bFsdi, bDri, bDsi;
	uint8_t bMbli, bMoreCardsAvaliable;
	uint8_t pppid[4];

	bPupiLength = 0;
	bNumSlots = 0;
	bAfi = 0x00;
	bExtAtqb = 0;
	bFsdi = 8;
	bCid = 0;
	bDri = PHPAL_I14443P3B_DATARATE_106;
	bDsi = PHPAL_I14443P3B_DATARATE_106;

		int iq;
		uint8_t *p, CMd[5] = {0, 0x36, 0, 0, 8};
		uint16_t plength;*/





		/* ------------------------ Type B Section -------------------------- */
/*		PH_CHECK_SUCCESS_FCT(status, phpalI14443p3b_Sw_Init(&palI14443p3B,
				                                   sizeof(palI14443p3B), &halReader));

		PH_CHECK_SUCCESS_FCT(status, phhalHw_ApplyProtocolSettings(&halReader,
				                                  PHHAL_HW_CARDTYPE_ISO14443B));

		PH_CHECK_SUCCESS_FCT(status, phhalHw_FieldReset(&halReader));*/

/*		status = phpalI14443p3b_ActivateCard( &palI14443p3B,
				             pPupi, bPupiLength, bNumSlots, bAfi, bExtAtqb,
				             bFsdi, bCid, bDri, bDsi, pAtqb, &bAtqbLen, &bMbli,
				             &bMoreCardsAvaliable);*/

/*		status = phpalI14443p3b_RequestB(&palI14443p3B, 0, 0, 0, pAtqb, &bAtqbLen);
		if(status != PH_ERR_SUCCESS)
		{
			printf("RequestB is failed, status is 0x%x", status);
			goto OUT;		
		}
		printf("Type B card detected\n");
		printf("ATQB value is ");
		for(iq = 0; iq < bAtqbLen; iq++)
		{
			printf("%02X ", pAtqb[iq]);
		}
		printf("\n");

		status = phpalI14443p3b_GetSerialNo(&palI14443p3B, pppid);
		if(status != PH_ERR_SUCCESS)
		{
			printf("Get PUPI error, status is 0x%x", status);
			goto OUT;
		}
		printf("PUPI value is ");
		for(iq = 0; iq < 4; iq++)
		{
			printf("%02X ", pppid[iq]);
		}
		printf("\n");

		pAtqb[10] |= 1;
		pAtqb[11] |= 1;

		status = phpalI14443p3b_Attrib(&palI14443p3B, pAtqb, bAtqbLen, 8, 8, 0, 0, &bMbli);
		if(PH_ERR_SUCCESS != status)
		{
			printf("No card or tag detected, status is 0x%x\n", status);
			goto OUT;
		}
		printf("bMbli valud is 0x%x\n", bMbli);

		status = phhalHw_Exchange(&halReader, PH_EXCHANGE_DEFAULT, CMd, 5, &p, &plength);
		if(status != PH_ERR_SUCCESS)
		{
			printf("GUID exchange failed, status is 0x%x", status);
			goto OUT;
		}
		printf("UID is ");
		for(iq = 0; iq < (plength >= 8 ? 8 : plength); iq++)
		{
			printf("%02X ", p[iq]);
		}
		printf("\n");
OUT:	phbalReg_ClosePort(&balReader);
	return 0;*/
