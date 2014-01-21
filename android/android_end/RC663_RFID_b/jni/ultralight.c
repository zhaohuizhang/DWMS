#include <jni.h>
#include <unistd.h>
#include <string.h>

#include <ph_NxpBuild.h>
#include <ph_Status.h>
#include <phalMful.h>
#include <phpalI14443p3a.h>
#include <phhalHw.h>
#include <phKeyStore.h>


#include "android/log.h"
extern const char *JNI_CLASS_Ultralight;
extern const char *JNI_TAG_Ultralight;

#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, JNI_TAG_Ultralight, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG_Ultralight, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, JNI_TAG_Ultralight, fmt, ##args)

#if !defined RC663_RS232 && !defined RC663_I2C
	#error "Must define one of RC663_RS232 and RC663_I2C"
#endif

#if defined RC663_RS232 && defined RC663_I2C
	#error "MACRO RC663_RS232 and RC663_I2C can't defined at same time"
#endif


#define	RC663_DEV_PATH	"/dev/rc663"
#define	CHIP_ID_LENGTH_MAX		10
#define MIFARE_C_BLOCK_MAX		64
#define READ_C_BLOCK_LENGTH		16
#define MIFARE_C_ID_LENGTH		7
#define WRITE_C_BLOCK_LENGTH		4
#define WRITE_COM_C_BLOCK_LENGTH	16

static phStatus_t status;
static phbalReg_Stub_DataParams_t balReader;
static phhalHw_Rc663_DataParams_t halReader;
static uint8_t bHalBufferReader[0x400];
static phalMful_Sw_DataParams_t palMful;
static phpalI14443p3a_Sw_DataParams_t I14443p3a;
static phpalMifare_Sw_DataParams_t palMifare;
static phKeyStore_Rc663_DataParams_t Rc663keyStore;
//static phCryptoSym_Sw_DataParams_t physym;
//static phCryptoRng_Sw_DataParams_t phyrng;

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    init_dev
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_init_1dev
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    release_dev
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_android_ultralight_Mifare_1ultralight_release_1dev
  (JNIEnv *, jobject);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    search_card
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_android_ultralight_Mifare_1ultralight_search_1card
  (JNIEnv *, jobject);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    request
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_android_ultralight_rc663_Mifare_1ultralight_request
  (JNIEnv *, jobject);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    wakeup
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_android_ultralight_Mifare_1ultralight_wakeup
  (JNIEnv *, jobject);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    active_card
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_active_1card
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    halt_card
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_halt_1card
  (JNIEnv *, jobject);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    ulcauthenticate
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_ulcauthenticate
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    read_block
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_android_ultralight_Mifare_1ultralight_read_1block
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    write_block
 * Signature: (I[B)I
 */
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_write_1block
  (JNIEnv *, jobject, jint, jbyteArray);

/*
 * Class:     com_android_ultralight_Mifare_ultralight
 * Method:    compatibility_write
 * Signature: (I[B)[B
 */
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_compatibility_1write
  (JNIEnv *, jobject, jint, jbyteArray);

static JNINativeMethod methods[] = {
	{"init_dev", "(Ljava/lang/String;)I", (void *)Java_com_android_ultralight_Mifare_1ultralight_init_1dev},
	{"release_dev", "()V", (void *)Java_com_android_ultralight_Mifare_1ultralight_release_1dev},
	{"search_card", "()[B", (void *)Java_com_android_ultralight_Mifare_1ultralight_search_1card},
	{"halt_card", "()I", (void *)Java_com_android_ultralight_Mifare_1ultralight_halt_1card},
	{"read_block", "(I)[B", (void *)Java_com_android_ultralight_Mifare_1ultralight_read_1block},
	{"write_block", "(I[B)I", (void *)Java_com_android_ultralight_Mifare_1ultralight_write_1block},
	{"compatibility_write", "(I[B)I", (void *)Java_com_android_ultralight_Mifare_1ultralight_compatibility_1write},
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
	if(!registerNativeMethods(env, JNI_CLASS_Ultralight, methods, sizeof(methods) / sizeof(methods[0])))
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

JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_init_1dev(JNIEnv *env, jobject obj, jstring path)
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

	/* Open device file */
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
	#ifdef RC663_RS232
	halReader.bBalConnectionType = PHHAL_HW_BAL_CONNECTION_RS232;
        #endif
        #ifdef RC663_I2C
	halReader.bBalConnectionType = PHHAL_HW_BAL_CONNECTION_I2C;
        #endif
	LOGD("hal layout init ok");

	/* Initialize the 14443-3A PAL (Protocol Abstraction Layer) component */
	status = phpalI14443p3a_Sw_Init(&I14443p3a, sizeof(phpalI14443p3a_Sw_DataParams_t), &halReader);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("pal 14443-3a layout init ok");
	/* Initialize the ultralight PAL component */
	status = phalMful_Sw_Init(&palMful, sizeof(phalMful_Sw_DataParams_t), &palMifare, &Rc663keyStore, NULL, NULL);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("Mful ultralight layout init ok");
	
	status = phpalMifare_Sw_Init(&palMifare, sizeof(phpalMifare_Sw_DataParams_t), &halReader,  NULL);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("mifare ultralight layout init ok");	
	/* SoftReset the IC.
	 * The SoftReset only resets the RC663 to EEPROM configuration.
	 */
	status = phhalHw_Rc663_Cmd_SoftReset(&halReader);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Soft reset failed, status is 0x%x", status);
		return -1;
	}
	LOGD("Soft reset ok!");

	/* Reset the RF field */
	status = phhalHw_FieldReset(&halReader);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("RF reset failed, status is 0x%x", status);
		return -1;
	}
	LOGD("RF rest ok");
	
	/* Apply the type A protocol settings
		 * and activate the RF field. */
		status = phhalHw_ApplyProtocolSettings(&halReader, PHHAL_HW_CARDTYPE_ISO14443A);
		if(status != PH_ERR_SUCCESS)
		{
			LOGE("Load 14443a protocol failed, status is 0x%x", status);
			return -1;
		}

		LOGD("Inin all ok!");
		return 0;
	}
JNIEXPORT void JNICALL Java_com_android_ultralight_Mifare_1ultralight_release_1dev(JNIEnv *env, jobject obj)
{
	LOGD("Close device file");
	phbalReg_ClosePort(&balReader);
}
JNIEXPORT jbyteArray JNICALL Java_com_android_ultralight_Mifare_1ultralight_search_1card(JNIEnv *env, jobject obj)
{
	uint8_t bSak[1];
	uint8_t bUid[CHIP_ID_LENGTH_MAX];
	uint8_t bMoreCardsAvailable;
	uint8_t bLength;
	uint8_t iq;
	jbyteArray jba = NULL;

	/* Activate the communication layer part 3
	 * of the ISO 14443A standard. */
	status = phpalI14443p3a_ActivateCard(&I14443p3a, NULL, 0, bUid, &bLength, bSak, &bMoreCardsAvailable);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("No card searched or error, status is 0x%x", status);
		return NULL;
	}

	/* Check if we have a card in the RF field.
	 * If so, check what card it is. */
	LOGD("Card ID: ");
	for(iq = 0; iq < bLength; iq++)
	{
		LOGD("%2x ", bUid[iq]);
	}
	LOGD("%d more cards founded", bMoreCardsAvailable);

	jba = (*env)->NewByteArray(env, bLength);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in search_card func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, bLength, bUid);

	LOGD("Search card ok");
	return jba;
}
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_halt_1card(JNIEnv *env, jobject obj)
{
	status = phpalI14443p3a_HaltA(&I14443p3a);
		if(status != PH_ERR_SUCCESS)
		{
			LOGE("Halt card error, status is 0x%x", status);
			return -1;
		}

		LOGD("Halt card ok");
		return 0;
}
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_active_1card(JNIEnv *env, jobject obj, jbyteArray cid)
{
	uint8_t bSak[1];
		uint8_t *Uid, UidLength;
		uint8_t bUid[CHIP_ID_LENGTH_MAX];
		uint8_t bMoreCardsAvailable;
		uint8_t bLength;

		UidLength = (*env)->GetArrayLength(env, cid);
		if(UidLength != MIFARE_C_ID_LENGTH)
		{
			LOGE("Error length of Card ID");
			return -1;
		}
		Uid = (*env)->GetByteArrayElements(env, cid, 0);

		status = phpalI14443p3a_ActivateCard(&I14443p3a, Uid, UidLength, bUid, &bLength, bSak, &bMoreCardsAvailable);

		(*env)->ReleaseByteArrayElements(env, cid, Uid, 0);
		if(status != PH_ERR_SUCCESS)
		{
			LOGE("Active card failed, status is 0x%x", status);
			return -1;
		}

		if (0x08 != (*bSak & 0x08))
		{
			LOGE("Not a Mifare classic card, status is 0x%x", status);
			return -1;
		}

		LOGD("Active card ok, %d more cards founded", bMoreCardsAvailable);
		return 0;
}

JNIEXPORT jbyteArray JNICALL Java_com_android_ultralight_Mifare_1ultralight_read_1block(JNIEnv *env, jobject obj, jint block)
{
	uint8_t data[READ_C_BLOCK_LENGTH] = {0};
		jbyteArray jba = NULL;

		if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
		{
			LOGE("Error block number");
			return NULL;
		}

		status = phalMful_Read(&palMful, block, data);
		if(status != PH_ERR_SUCCESS)
		{
			LOGE("Read block %d failed, status is 0x%x", block, status);
			return NULL;
		}
		jba = (*env)->NewByteArray(env, READ_C_BLOCK_LENGTH);
		if(jba == NULL)
		{
			LOGE("Memory alloc failed read block func");
			return NULL;
		}
		(*env)->SetByteArrayRegion(env, jba, 0, READ_C_BLOCK_LENGTH, data);

		LOGD("Read block %d ok", block);
		return jba;
}
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_write_1block(JNIEnv *env, jobject obj, jint block, jbyteArray data)
{
	uint8_t *dataArray, dataLength;

		if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
		{
			LOGE("Error block number");
			return -1;
		}

		dataLength = (*env)->GetArrayLength(env, data);
		if(dataLength != WRITE_C_BLOCK_LENGTH)
		{
			LOGE("Error length of block");
			return -1;
		}
		dataArray = (*env)->GetByteArrayElements(env, data, 0);
		LOGD("%d",dataLength);
		status = phalMful_Write(&palMful, block, dataArray);

		(*env)->ReleaseByteArrayElements(env, data, dataArray, 0);
		if(status != PH_ERR_SUCCESS)
		{

			LOGE("Write block %d failed, status is 0x%x", block, status);
			return -1;
		}

		LOGD("Write block %d ok", block);
		return 0;
}
JNIEXPORT jint JNICALL Java_com_android_ultralight_Mifare_1ultralight_compatibility_1write(JNIEnv *env, jobject obj, jint cblock, jbyteArray data)
{
	        uint8_t *dataArray, dataLength;

		if((cblock >= MIFARE_C_BLOCK_MAX) || (cblock < 0))
		{
			LOGE("Error cblock number");
			return -1;
		}

		dataLength = (*env)->GetArrayLength(env, data);
		if(dataLength != WRITE_COM_C_BLOCK_LENGTH)
		{
			LOGE("Error length of cblock");
			return -1;
		}
		dataArray = (*env)->GetByteArrayElements(env, data, 0);

		status = phalMful_Sw_CompatibilityWrite(&palMful, cblock, dataArray);

		(*env)->ReleaseByteArrayElements(env, data, dataArray, 0);
		if(status != PH_ERR_SUCCESS)
		{
			LOGE("Write cblock %d failed, status is 0x%x", cblock, status);
			return -1;
		}

		LOGD("Write block %d ok", cblock);
		return 0;
}
