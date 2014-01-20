#include <jni.h>
#include <unistd.h>
#include <string.h>

#include <ph_NxpBuild.h>
#include <ph_Status.h>
#include <phpalI14443p3a.h>
#include <phpalI14443p4.h>
#include <phKeyStore.h>
#include <phalMfc.h>

#include "android/log.h"
extern const char *JNI_TAG_mifare;
extern const char *JNI_CLASS_mifare;
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  JNI_TAG_mifare, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG_mifare, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, JNI_TAG_mifare, fmt, ##args)

#if !defined RC663_RS232 && !defined RC663_I2C
	#error "Must define one of RC663_RS232 and RC663_I2C"
#endif

#if defined RC663_RS232 && defined RC663_I2C
	#error "MACRO RC663_RS232 and RC663_I2C can't defined at same time"
#endif

#define CHIP_KEY_STORE_MAX		85
#define	CHIP_ID_LENGTH_MAX		10
#define MIFARE_C_ID_LENGTH		4
#define MIFARE_C_KEY_LENGTH		6
#define MIFARE_C_KEY_LENGTH_GROUP	12
#define MIFARE_C_BLOCK_MAX		64
#define MIFARE_C_BLOCK_LENGTH		16

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    init_dev
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL init_1dev(JNIEnv *, jobject, jstring);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    release_dev
 * Signature: ()V
 */
JNIEXPORT void JNICALL release_1dev(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    search_card
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL search_1card(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    halt_card
 * Signature: ()I
 */
JNIEXPORT jint JNICALL halt_1card(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    active_card
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL active_1card(JNIEnv *, jobject, jbyteArray);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    store_key_eeprom
 * Signature: (I[B)I
 */
JNIEXPORT jint JNICALL store_1key_1eeprom(JNIEnv *, jobject, jint, jbyteArray);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    auth_card_key
 * Signature: (I[BI[B)I
 */
JNIEXPORT jint JNICALL auth_1card_1key(JNIEnv *, jobject, jint, jbyteArray, jint, jbyteArray);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    auth_card_eeprom
 * Signature: (I[BII)I
 */
JNIEXPORT jint JNICALL auth_1card_1eeprom(JNIEnv *, jobject, jint, jbyteArray, jint, jint);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    read_block
 * Signature: (I)[B
 */
JNIEXPORT jbyteArray JNICALL read_1block(JNIEnv *, jobject, jint);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    read_block_value
 * Signature: (I)[I
 */
JNIEXPORT jintArray JNICALL read_1block_1value(JNIEnv *, jobject, jint);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    write_block
 * Signature: (I[B)I
 */
JNIEXPORT jint JNICALL write_1block(JNIEnv *, jobject, jint, jbyteArray);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    write_block_value
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL write_1block_1value(JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    inc_value
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL inc_1value(JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     com_geomobile_rc663_Mifare_native
 * Method:    dec_value
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL dec_1value(JNIEnv *, jobject, jint, jint, jint);

/*******************************************************************************
**   Main Function
*******************************************************************************/

static phStatus_t status;
static phbalReg_Stub_DataParams_t balReader;
static phhalHw_Rc663_DataParams_t halReader;
static phpalI14443p3a_Sw_DataParams_t I14443p3a;
static phpalI14443p4_Sw_DataParams_t I14443p4;
static phpalMifare_Sw_DataParams_t palMifare;
static phKeyStore_Rc663_DataParams_t Rc663keyStore;
static phalMfc_Sw_DataParams_t alReader;
static uint8_t bHalBufferReader[0x400];

static JNINativeMethod methods[] = {
	{"init_dev", "(Ljava/lang/String;)I", (void *)init_1dev},
	{"release_dev", "()V", (void *)release_1dev},
	{"search_card", "()[B", (void *)search_1card},
	{"halt_card", "()I", (void *)halt_1card},
	{"active_card", "([B)I", (void *)active_1card},
	{"store_key_eeprom", "(I[B)I", (void *)store_1key_1eeprom},
	{"auth_card_key", "(I[BI[B)I", (void *)auth_1card_1key},
	{"auth_card_eeprom", "(I[BII)I", (void *)auth_1card_1eeprom},
	{"read_block", "(I)[B", (void *)read_1block},
	{"read_block_value", "(I)[I", (void *)read_1block_1value},
	{"write_block", "(I[B)I", (void *)write_1block},
	{"write_block_value", "(II)I", (void *)write_1block_1value},
	{"inc_value", "(III)I", (void *)inc_1value},
	{"dec_value", "(III)I", (void *)dec_1value},
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
	if(!registerNativeMethods(env, JNI_CLASS_mifare, methods, sizeof(methods) / sizeof(methods[0])))
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

	/* Initialize the 14443-4 PAL component */
	status = phpalI14443p4_Sw_Init(&I14443p4, sizeof(phpalI14443p4_Sw_DataParams_t), &halReader);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("pal 14443-4 layout init ok");

	/* Initialize the Mifare PAL component */
	status = phpalMifare_Sw_Init(&palMifare, sizeof(phpalMifare_Sw_DataParams_t), &halReader, &I14443p4);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("pal Mifare layout init ok");

	/* Initialize the keystore component */
	status = phKeyStore_Rc663_Init(&Rc663keyStore, sizeof(phKeyStore_Rc663_DataParams_t), &halReader);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("keystore layout init ok");

	status = phalMfc_Sw_Init(&alReader, sizeof(phalMfc_Sw_DataParams_t), &palMifare, NULL/*&Rc663keyStore*/);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("al Mifare classic layout init ok");

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

JNIEXPORT void JNICALL release_1dev(JNIEnv *env, jobject obj)
{
	LOGD("Close device file");
	phbalReg_ClosePort(&balReader);
}

JNIEXPORT jbyteArray JNICALL search_1card(JNIEnv *env, jobject obj)
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

	// Check if there is a Mifare Classic card in the RF field
	if (0x08 != (*bSak & 0x08))
	{
		LOGE("Not a Mifare classic card, status is 0x%x", status);
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

/*	// Check if there is an ISO-4 compliant card in the RF field
	if (0x20 == (*bSak & 0x20))
	{
		return NULL;
	}
	// The last possibility is a Mifare UltraLight card
	else
	{
		return NULL;//bUid;
	}*/
}

JNIEXPORT jint JNICALL halt_1card(JNIEnv *env, jobject obj)
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

JNIEXPORT jint JNICALL active_1card(JNIEnv *env, jobject obj, jbyteArray cid)
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

JNIEXPORT jint JNICALL store_1key_1eeprom(JNIEnv *env, jobject obj, jint place, jbyteArray keypair)
{
	uint8_t *keyArray, keyLength;

	if((place >= CHIP_KEY_STORE_MAX) || (place < 0))
	{
		LOGE("Error place to store key");
		return -1;
	}
	
	keyLength = (*env)->GetArrayLength(env, keypair);
	if(keyLength != MIFARE_C_KEY_LENGTH_GROUP)
	{
		LOGE("Error key pair length");
		return -1;
	}
	keyArray = (*env)->GetByteArrayElements(env, keypair, 0);

	status = phKeyStore_SetKey(&Rc663keyStore, place, 0, PH_KEYSTORE_KEY_TYPE_MIFARE, keyArray, 0);

	(*env)->ReleaseByteArrayElements(env, keypair, keyArray, 0);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Error store key to eeprom, status is 0x%x", status);
		return -1;
	}

	LOGD("store key to eeprom ok");
	return 0;
}

JNIEXPORT jint JNICALL auth_1card_1key(JNIEnv *env, jobject obj, jint type, jbyteArray cid, jint block, jbyteArray key)
{
	uint8_t *Uid, UidLength;
	uint8_t *keyArray, keyLength;

	if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
	{
		LOGE("Error block number");
		return -1;
	}

	if((type != PHPAL_MIFARE_KEYA) && (type != PHPAL_MIFARE_KEYB))
	{
		LOGE("Error key type");
		return -1;
	}

	UidLength = (*env)->GetArrayLength(env, cid);
	if(UidLength != MIFARE_C_ID_LENGTH)
	{
		LOGE("Error length of Card ID");
		return -1;
	}

	keyLength = (*env)->GetArrayLength(env, key);
	if(keyLength != MIFARE_C_KEY_LENGTH)
	{
		LOGE("Error key length");
		return -1;
	}

	Uid = (*env)->GetByteArrayElements(env, cid, 0);
	keyArray = (*env)->GetByteArrayElements(env, key, 0);

	status = phpalMifare_MfcAuthenticate(&palMifare, block, type, keyArray, Uid);

	(*env)->ReleaseByteArrayElements(env, key, keyArray, 0);
	(*env)->ReleaseByteArrayElements(env, cid, Uid, 0);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("auth block with key failed, status is 0x%x", status);
		return -1;
	}

	LOGD("auth block with key ok");
	return 0;
}

JNIEXPORT jint JNICALL auth_1card_1eeprom(JNIEnv *env, jobject obj, jint type, jbyteArray cid, jint block, jint place)
{
	uint8_t *Uid, UidLength;

	if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
	{
		LOGE("Error block number");
		return -1;
	}

	if((type != PHPAL_MIFARE_KEYA) && (type != PHPAL_MIFARE_KEYB))
	{
		LOGE("Error key type");
		return -1;
	}

	UidLength = (*env)->GetArrayLength(env, cid);
	if(UidLength != MIFARE_C_ID_LENGTH)
	{
		LOGE("Error length of Card ID");
		return -1;
	}
	Uid = (*env)->GetByteArrayElements(env, cid, 0);

	status = phpalMifare_MfcAuthenticateKeyNo(&palMifare, block, type, place, 0, Uid);

	(*env)->ReleaseByteArrayElements(env, cid, Uid, 0);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("auth block with eeprom failed, status is 0x%x", status);
		return -1;
	}

	LOGD("auth block with eeprom ok");
	return 0;
}

JNIEXPORT jbyteArray JNICALL read_1block(JNIEnv *env, jobject obj, jint block)
{
	uint8_t data[MIFARE_C_BLOCK_LENGTH] = {0};
	jbyteArray jba = NULL;

	if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
	{
		LOGE("Error block number");
		return NULL;
	}

	status = phalMfc_Read(&alReader, block, data);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Read block %d failed, status is 0x%x", block, status);
		return NULL;
	}
	jba = (*env)->NewByteArray(env, MIFARE_C_BLOCK_LENGTH);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed read block func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, MIFARE_C_BLOCK_LENGTH, data);

	LOGD("Read block %d ok", block);
	return jba;
}

JNIEXPORT jintArray JNICALL read_1block_1value(JNIEnv *env, jobject obj, jint block)
{
	uint8_t nouse, data[4] = {0};
	int value = 0;
	jintArray jia = NULL;

	if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
	{
		LOGE("Error block number");
		return NULL;
	}

	status = phalMfc_ReadValue(&alReader, block, data, &nouse);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Read block %d value failed, status is 0x%x", block, status);
		return NULL;
	}
	value = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
	jia = (*env)->NewIntArray(env, 1);
	if(jia == NULL)
	{
		LOGE("Memory alloc failed read block value func");
		return NULL;
	}
	(*env)->SetIntArrayRegion(env, jia, 0, 1, &value);

	LOGD("Read block %d value is %d", block, value);
	return jia;
}

JNIEXPORT jint JNICALL write_1block(JNIEnv *env, jobject obj, jint block, jbyteArray data)
{
	uint8_t *dataArray, dataLength;

	if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
	{
		LOGE("Error block number");
		return -1;
	}

	dataLength = (*env)->GetArrayLength(env, data);
	if(dataLength != MIFARE_C_BLOCK_LENGTH)
	{
		LOGE("Error length of block");
		return -1;
	}
	dataArray = (*env)->GetByteArrayElements(env, data, 0);

	status = phalMfc_Write(&alReader, block, dataArray);

	(*env)->ReleaseByteArrayElements(env, data, dataArray, 0);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Write block %d failed, status is 0x%x", block, status);
		return -1;
	}

	LOGD("Write block %d ok", block);
	return 0;
}

JNIEXPORT jint JNICALL write_1block_1value(JNIEnv *env, jobject obj, jint block, jint value)
{
	uint8_t dataArray[4] = {0};

	if((block >= MIFARE_C_BLOCK_MAX) || (block < 0))
	{
		LOGE("Error block number");
		return -1;
	}

	dataArray[0] = value;
	dataArray[1] = value >> 8;
	dataArray[2] = value >> 16;
	dataArray[3] = value >> 24;

	status = phalMfc_WriteValue(&alReader, block, dataArray, block);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Write value %d to block %d failed, status is 0x%x", value, block, status);
		return -1;
	}

	LOGD("Write value %d to block %d ok", value, block);
	return 0;
}

JNIEXPORT jint JNICALL inc_1value(JNIEnv *env, jobject obj, jint sblock, jint dblock, jint value)
{
	uint8_t dataArray[4] = {0};

	if((sblock >= MIFARE_C_BLOCK_MAX) || (sblock < 0))
	{
		LOGE("Error sblock number");
		return -1;
	}

	if((dblock >= MIFARE_C_BLOCK_MAX) || (dblock < 0))
	{
		LOGE("Error dblock number");
		return -1;
	}

	dataArray[0] = value;
	dataArray[1] = value >> 8;
	dataArray[2] = value >> 16;
	dataArray[3] = value >> 24;

	status = phalMfc_IncrementTransfer(&alReader, sblock, dblock, dataArray);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Inc value %d from block %d to block %d failed, status is 0x%x", value, sblock, dblock, status);
		return -1;
	}

	LOGD("Inc value %d frome block %d to block %d ok", value, sblock, dblock);
	return 0;
}

JNIEXPORT jint JNICALL dec_1value(JNIEnv *env, jobject obj, jint sblock, jint dblock, jint value)
{
	uint8_t dataArray[4] = {0};

	if((sblock >= MIFARE_C_BLOCK_MAX) || (sblock < 0))
	{
		LOGE("Error sblock number");
		return -1;
	}

	if((dblock >= MIFARE_C_BLOCK_MAX) || (dblock < 0))
	{
		LOGE("Error dblock number");
		return -1;
	}

	dataArray[0] = value;
	dataArray[1] = value >> 8;
	dataArray[2] = value >> 16;
	dataArray[3] = value >> 24;

	status = phalMfc_DecrementTransfer(&alReader, sblock, dblock, dataArray);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Dec value %d from block %d to block %d failed, status is 0x%x", value, sblock, dblock, status);
		return -1;
	}

	LOGD("Dec value %d from block %d to block %d ok", value, sblock, dblock);
	return 0;
}
/******************************************************************************
 **                            End Of File
 ******************************************************************************/
