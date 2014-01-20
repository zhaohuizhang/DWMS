#include <jni.h>
#include <unistd.h>
#include <string.h>

#include <ph_NxpBuild.h>
#include <ph_Status.h>
#include <phhalHw.h>
#include <phbalReg.h>
#include <phpalSli15693.h>
#include <phalI15693.h>

#include "android/log.h"
extern const char *JNI_TAG_15693;
extern const char *JNI_CLASS_15693;
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  JNI_TAG_15693, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG_15693, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, JNI_TAG_15693, fmt, ##args)

#if !defined RC663_RS232 && !defined RC663_I2C
	#error "Must define one of RC663_RS232 and RC663_I2C"
#endif

#if defined RC663_RS232 && defined RC663_I2C
	#error "MACRO RC663_RS232 and RC663_I2C can't defined at same time"
#endif

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    init_dev
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL init_1dev(JNIEnv *, jobject, jstring);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    release_dev
 * Signature: ()V
 */
JNIEXPORT void JNICALL release_1dev(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    search_card
 * Signature: (IIIBI[BI)[B
 */
JNIEXPORT jbyteArray JNICALL search_1card(JNIEnv *, jobject, jint, jint, jint, jbyte, jint, jbyteArray, jint);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    read_card_info
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL read_1card_1info(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    stay_quiet
 * Signature: ()I
 */
JNIEXPORT jint JNICALL stay_1quiet(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    select_card
 * Signature: ()I
 */
JNIEXPORT jint JNICALL select_1card(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    reset_to_ready
 * Signature: ()I
 */
JNIEXPORT jint JNICALL reset_1to_1ready(JNIEnv *, jobject);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    read_block
 * Signature: (II)[B
 */
JNIEXPORT jbyteArray JNICALL read_1block(JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    read_multi_block
 * Signature: (III)[B
 */
JNIEXPORT jbyteArray JNICALL read_1multi_1block(JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    write_block
 * Signature: (II[B)I
 */
JNIEXPORT jint JNICALL write_1block(JNIEnv *, jobject, jint, jint, jbyteArray);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    write_multi_block
 * Signature: (III[B)I
 */
JNIEXPORT jint JNICALL write_1multi_1block(JNIEnv *, jobject, jint, jint, jint, jbyteArray);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    write_AFI
 * Signature: (IB)I
 */
JNIEXPORT jint JNICALL write_1AFI(JNIEnv *, jobject, jint, jbyte);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    write_DSFID
 * Signature: (IB)I
 */
JNIEXPORT jint JNICALL write_1DSFID(JNIEnv *, jobject, jint, jbyte);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    lock_block
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL lock_1block(JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    lock_AFI
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL lock_1AFI(JNIEnv *, jobject, jint);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    lock_DSFID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL lock_1DSFID(JNIEnv *, jobject, jint);

/*
 * Class:     com_geomobile_rc663_Iso15693_native
 * Method:    get_multi_block_security_status
 * Signature: (II)[B
 */
JNIEXPORT jbyteArray JNICALL get_1multi_1block_1security_1status(JNIEnv *, jobject, jint, jint);

/*******************************************************************************
**   Main Function
*******************************************************************************/

static phStatus_t status;
static phbalReg_Stub_DataParams_t balReader;
static phhalHw_Rc663_DataParams_t halReader;
static phpalSli15693_Sw_DataParams_t palSli15693;
static phalI15693_Sw_DataParams_t alI15693;
static uint8_t bHalBufferReader[0x400];

/*static current_card_max_block_nr = 0;
static current_card_block_size	= 0;
static current_card_AFI = 0;
static current_card_DSFID = 0;*/

static JNINativeMethod methods[] = {
	{"init_dev", "(Ljava/lang/String;)I", (void *)init_1dev},
	{"release_dev", "()V", (void *)release_1dev},
	{"search_card", "(IIIBI[BI)[B", (void *)search_1card},
	{"read_card_info", "()[B", (void *)read_1card_1info},
	{"stay_quiet", "()I", (void *)stay_1quiet},
	{"select_card", "()I", (void *)select_1card},
	{"reset_to_ready", "()I", (void *)reset_1to_1ready},
	{"read_block", "(II)[B", (void *)read_1block},
	{"read_multi_block", "(III)[B", (void *)read_1multi_1block},
	{"write_block", "(II[B)I", (void *)write_1block},
	{"write_multi_block", "(III[B)I", (void *)write_1multi_1block},
	{"write_AFI", "(IB)I", (void *)write_1AFI},
	{"write_DSFID", "(IB)I", (void *)write_1DSFID},
	{"lock_block", "(II)I", (void *)lock_1block},
	{"lock_AFI", "(I)I", (void *)lock_1AFI},
	{"lock_DSFID", "(I)I", (void *)lock_1DSFID},
	{"get_multi_block_security_status", "(II)[B", (void *)get_1multi_1block_1security_1status},
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
	if(!registerNativeMethods(env, JNI_CLASS_15693, methods, sizeof(methods) / sizeof(methods[0])))
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
//	strncpy(balReader.devPath, RC663_DEV_PATH, DEV_PATH_LENGTH);
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

	/* init. 15693 pal */
	status = phpalSli15693_Sw_Init(&palSli15693, sizeof(phpalSli15693_Sw_DataParams_t), &halReader);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("pal 15693 layout init ok");

	/* init. 15693 al component */
	status = phalI15693_Sw_Init(&alI15693, sizeof(phalI15693_Sw_DataParams_t), &palSli15693);
	if(status != PH_ERR_SUCCESS)
	{
		return -1;
	}
	LOGD("al 15693 layout init ok");

	/* reset the IC  */
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
	LOGD("RF rest ok!");

	/* Apply the 15693 protocol settings */
	status = phhalHw_ApplyProtocolSettings(&halReader, PHHAL_HW_CARDTYPE_ISO15693);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Load ISO/IEC 15693 protocol failed, status is 0x%x", status);
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

JNIEXPORT jbyteArray JNICALL search_1card(JNIEnv *env, jobject obj, jint type, jint datarate, jint setAFI, jbyte AFI, jint oneslots, jbyteArray mask, jint maskbitlength)
//char *search_card(uint8_t type, int datarate, int setAFI, uint8_t AFI, int oneslots, uint8_t *mask, uint8_t masklength)
{
	uint8_t bUid[PHPAL_SLI15693_UID_LENGTH] = {0};
	uint8_t bMoreCardsAvailable;
	uint8_t bLength;
	uint8_t bMask[PHPAL_SLI15693_UID_LENGTH];
	uint8_t Dsfid;
	uint8_t *pmask, pmaskLength;
	uint8_t flags = 0, iq, res[PHPAL_SLI15693_UID_LENGTH + 2];
	jbyteArray jba = NULL;

	if((type != PHPAL_SLI15693_ACTIVATE_ADDRESSED) && (type != PHPAL_SLI15693_ACTIVATE_SELECTED))
	{
		LOGE("Error active card type");
		return NULL;
	}
	
	if(datarate != 0)
	{
		flags |= PHPAL_SLI15693_FLAG_DATA_RATE;
	}

	if(setAFI != 0)
	{
		flags |= PHPAL_SLI15693_FLAG_AFI;
	}
	else
	{
		AFI = 0;
	}

	if(oneslots != 0)
	{
		flags |= PHPAL_SLI15693_FLAG_NBSLOTS;
	}

	if(mask == NULL)
	{
		memset(bMask, 0, sizeof(bMask));
		pmask = bMask;
		pmaskLength = 0;
		status = phpalSli15693_ActivateCard(&palSli15693, type, flags, AFI, pmask, pmaskLength, &Dsfid, bUid, &bMoreCardsAvailable);
	}
	else
	{
		if(((oneslots != 0) && (maskbitlength > 64)) || ((oneslots == 0) && (maskbitlength > 60)))
		{
			LOGE("Error mask length");
			return NULL;
		}
		pmaskLength = (*env)->GetArrayLength(env, mask);
		if(pmaskLength * 8 < maskbitlength)
		{
			LOGE("mask bit arrary is shorter than maskbitlength");
			return NULL;
		}
		pmask = (*env)->GetByteArrayElements(env, mask, 0);
		pmaskLength = maskbitlength;
		status = phpalSli15693_ActivateCard(&palSli15693, type, flags, AFI, pmask, pmaskLength, &Dsfid, bUid, &bMoreCardsAvailable);
		(*env)->ReleaseByteArrayElements(env, mask, pmask, 0);
	}

	if(status != PH_ERR_SUCCESS)
	{
		LOGE("No card get or select, status is 0x%x", status);
		return NULL;
	}
	LOGD("Card ID: ");
	for(iq = 0; iq < PHPAL_SLI15693_UID_LENGTH; iq++)
	{
		LOGD("%02x ", bUid[iq]);
	}
	LOGD("%d more cards founded", bMoreCardsAvailable);

	jba = (*env)->NewByteArray(env, PHPAL_SLI15693_UID_LENGTH + 2);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in search_card func");
		return NULL;
	}
	memcpy(res, bUid, PHPAL_SLI15693_UID_LENGTH);
	res[PHPAL_SLI15693_UID_LENGTH] = Dsfid;
	res[PHPAL_SLI15693_UID_LENGTH + 1] = bMoreCardsAvailable;

	(*env)->SetByteArrayRegion(env, jba, 0, PHPAL_SLI15693_UID_LENGTH + 2, res);

	LOGD("Search or select card ok");
	return jba;
}

JNIEXPORT jbyteArray JNICALL read_1card_1info(JNIEnv *env, jobject obj)
{
	uint8_t *p = NULL, info[5];
	uint16_t plength;
	jbyteArray jba = NULL;

	status =  phalI15693_GetSystemInformation(&alI15693, &p, &plength);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Get sys info failed, status is 0x%x", status);
		return NULL;
	}
	LOGD("AFI is 0x%02x\n", p[10]);
	LOGD("DSFID is 0x%02x\n", p[9]);
	LOGD("block number is %d\n", p[11] + 1);
	LOGD("block size is %d\n", p[12] + 1);
	LOGD("IC is 0x%02x\n", p[13]);

	jba = (*env)->NewByteArray(env, 5);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in read_card_info");
		return NULL;
	}
	info[0] = p[9];
	info[1] = p[10];
	info[2] = p[11] + 1;
	info[3] = p[12] + 1;
	info[4] = p[13];
	(*env)->SetByteArrayRegion(env, jba, 0, 5, info);

	LOGD("Get card info ok");
	return jba;
/*	current_card_max_block_nr = p[11] + 1;
	current_card_block_size	= p[12] + 1;
	current_card_AFI = p[10];
	current_card_DSFID = p[9];*/
}

JNIEXPORT jint JNICALL stay_1quiet(JNIEnv *env, jobject obj)
{
	status = phpalSli15693_StayQuiet(&palSli15693);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Make current card stay quiet failed, status is 0x%x", status);
		return -1;
	}

	LOGD("Make current card stay quiet ok");
	return 0;
}

JNIEXPORT jint JNICALL select_1card(JNIEnv *env, jobject obj)
{
	status = phpalSli15693_Select(&palSli15693);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Select current card failed, status is 0x%x", status);
		return -1;
	}

	LOGD("Select current card ok");
	return 0;
}

JNIEXPORT jint JNICALL reset_1to_1ready(JNIEnv *env, jobject obj)
{
	status = phpalSli15693_ResetToReady(&palSli15693);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Reset current card to ready failed, status is 0x%x", status);
		return -1;
	}

	LOGD("Reset current to ready ok");
	return 0;
}

JNIEXPORT jbyteArray JNICALL read_1block(JNIEnv *env, jobject obj, jint option, jint block)
//char * read_block(int option, int block)
{
	uint8_t *p = NULL, iq;
	uint16_t plength;
	jbyteArray jba = NULL;

	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return NULL;
	}

/*	if(block >= current_card_max_block_nr)
	{
		LOGE("Block number is out of current card's block range");
		return NULL;
	}
*/
	status = phalI15693_ReadSingleBlock(&alI15693, option, block, &p, &plength);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("read block error: 0x%x\n", status);
		return NULL;
	}

	LOGD("block %d content is: ", block);
	for(iq = 0; iq < plength; iq++)
	{
		LOGD("0x%02x ", p[iq]);
	}
	
	jba = (*env)->NewByteArray(env, plength);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in read_block func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, plength, p);

	return jba;
}

JNIEXPORT jbyteArray JNICALL read_1multi_1block(JNIEnv *env, jobject obj, jint option, jint firstblock, jint nr_block)
//char * read_multi_block(int option, int firstblock, uint16_t nr_block)
{
	uint8_t *p = NULL;
	uint16_t plength;
	uint16_t rd_nr_block;
	int iq;
	jbyteArray jba = NULL;

	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return NULL;
	}

/*	if(firstblock >= current_card_max_block_nr)
	{
		LOGE("Block number is out of current card's block range");
		return NULL;
	}

	if(firstblock + nr_block - 1 >= current_card_max_block_nr)
	{
		rd_nr_block = current_card_max_block_nr - firstblock;
		LOGI("Only %d block will be read", rd_nr_block);
	}
	else
	{
		rd_nr_block = nr_block;
	}	
*/
	rd_nr_block = nr_block;
	if(rd_nr_block == 0)
	{
		LOGE("Error block number");
		return NULL;
	}

	status = phalI15693_ReadMultipleBlocks(&alI15693, option, firstblock, rd_nr_block, &p, &plength);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("read multi block error: 0x%x\n", status);
		return NULL;
	}
	LOGD("block %d to %d content is: ", firstblock, firstblock + rd_nr_block - 1);
	for(iq = 0; iq < plength; iq++)
	{	
		if((iq % 4) == 0)
		{
			LOGD("block %d: ", iq / 4);
		}
		LOGD("0x%02x ", p[iq]);
	}

	jba = (*env)->NewByteArray(env, plength);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in read_multi_block func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, plength, p);

	return jba;
}

JNIEXPORT jint JNICALL write_1block(JNIEnv *env, jobject obj, jint option, jint block, jbyteArray data)
//int write_block(int option, int block, char *p)
{
	uint8_t *dataArray;
	uint16_t dataLength;

	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}
/*
	if(block >= current_card_max_block_nr)
	{
		LOGE("Block number is out of current card's block range");
		return -1;
	}

	if(p.length >= current_card_block_size)
	{
		LOGI("block size is %d, only %d data is write", current_card_block_size, current_card_block_size);
	}
*/
	dataLength = (*env)->GetArrayLength(env, data);
	dataArray = (*env)->GetByteArrayElements(env, data, 0);

	status = phalI15693_WriteSingleBlock(&alI15693, option, block, dataArray, dataLength);						//

	(*env)->ReleaseByteArrayElements(env, data, dataArray, 0);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("write block error: 0x%x\n", status);
		return -1;
	}

	LOGD("write block %d ok", block);
	return 0;
}

JNIEXPORT jint JNICALL write_1multi_1block(JNIEnv *env, jobject obj, jint option, jint firstblock, jint nr_block, jbyteArray data)
//int write_multi_block(int option, int firstblock, int nr_block, char *p)
{
	uint16_t rd_nr_block;
	uint8_t *dataArray;
	uint16_t dataLength;

	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}

/*	if(firstblock >= current_card_max_block_nr)
	{
		LOGE("Block number is out of current card's block range");
		return -1;
	}

	if(firstblock + nr_block - 1 >= current_card_max_block_nr)
	{
		rd_nr_block = current_card_max_block_nr - firstblock;
		LOGI("Only %d block will be write", rd_nr_block);
	}
	else
	{
		rd_nr_block = nr_block;
	}	
*/
	rd_nr_block = nr_block;
	if(rd_nr_block == 0)
	{
		LOGE("Error block number");
		return -1;
	}

	dataLength = (*env)->GetArrayLength(env, data);
	dataArray = (*env)->GetByteArrayElements(env, data, 0);

	status = phalI15693_WriteMultipleBlocks(&alI15693, option, firstblock, rd_nr_block, dataArray, dataLength);			//

	(*env)->ReleaseByteArrayElements(env, data, dataArray, 0);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("wrtie multi block error: 0x%x\n", status);
		return -1;
	}

	LOGD("write blocks ok");
	return 0;
}

JNIEXPORT jint JNICALL lock_1block(JNIEnv *env, jobject obj, jint option, jint block)
//int lock_block(int option, int block)
{
	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}

/*	if(block >= current_card_max_block_nr)
	{
		LOGE("Block number is out of current card's block range");
		return -1;
	}
*/
	status = phalI15693_LockBlock(&alI15693, option, block);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("lock block %d error: 0x%x", block, status);
		return -1;
	}

	LOGD("block %d is locked");
	return 0;
}

JNIEXPORT jint JNICALL lock_1AFI(JNIEnv *env, jobject obj, jint option)
//int lock_AFI(int option)
{
	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}

	status = phalI15693_LockAFI(&alI15693, option);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Lock AFI error");
		return -1;
	}

	LOGD("Lock AFI ok");
	return 0;
}

JNIEXPORT jint JNICALL write_1AFI(JNIEnv *env, jobject obj, jint option, jbyte AFI)
//int write_AFI(int option, uint8_t AFI)
{
	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}

	status = phalI15693_WriteAFI(&alI15693, option, AFI);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("write AFI error: 0x%x\n", status);
		return -1;
	}

	LOGD("write AFI ok");
	return 0;
}

JNIEXPORT jint JNICALL lock_1DSFID(JNIEnv *env, jobject obj, jint option)
//int lock_DSFID(int option)
{
	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}

	status = phalI15693_LockDSFID(&alI15693, option);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Lock DSFID error, status is 0x%x", status);
		return -1;
	}

	LOGD("Lock DSFID ok");
	return 0;
}

JNIEXPORT jint JNICALL write_1DSFID(JNIEnv *env, jobject obj, jint option, jbyte DSFID)
//int write_DSFID(int option, uint8_t DSFID)
{
	if((option != PHAL_I15693_OPTION_OFF) && (option != PHAL_I15693_OPTION_ON))
	{
		LOGE("Error option bit");
		return -1;
	}

	status = phalI15693_WriteDSFID(&alI15693, option, DSFID);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("write DSFID error: 0x%x\n", status);
		return -1;
	}

	LOGD("write DSFID ok");
	return 0;
}

JNIEXPORT jbyteArray JNICALL get_1multi_1block_1security_1status(JNIEnv *env, jobject obj, jint firstblock, jint nr_block)
//char * get_multi_block_security_status(int firstblock, int nr_block)
{
	uint8_t *p = NULL;
	uint16_t plength, rd_nr_block;
	int iq;
	jbyteArray jba = NULL;

/*	if(firstblock >= current_card_max_block_nr)
	{
		LOGE("Block number is out of current card's block range");
		return NULL;
	}

	if(firstblock + nr_block - 1 >= current_card_max_block_nr)
	{
		rd_nr_block = current_card_max_block_nr - firstblock;
		LOGI("Only %d block will be check", rd_nr_block);
	}
	else
	{
		rd_nr_block = nr_block;
	}	
*/
	rd_nr_block = nr_block;
	if(rd_nr_block == 0)
	{
		LOGE("Error block numbers");
		return NULL;
	}

	status = phalI15693_GetMultipleBlockSecurityStatus(&alI15693, firstblock, rd_nr_block, &p, &plength);
	if(status != PH_ERR_SUCCESS)
	{
		LOGE("Get Multiple block security status error");
		return NULL;
	}

	LOGD("block %d to %d security status is: ", firstblock, firstblock + rd_nr_block - 1);
	for(iq = 0; iq < plength; iq++)
	{
		LOGD("0x%02x ", p[iq]);
	}

	jba = (*env)->NewByteArray(env, plength);
	if(jba == NULL)
	{
		LOGE("Memory alloc failed in get_multi_block_security_status func");
		return NULL;
	}
	(*env)->SetByteArrayRegion(env, jba, 0, plength, p);
	return jba;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
