package com.android.hflibs;


public class Iso15693_native {
	
	public static final int ISO15693_OPTION_OFF = 0;
	public static final int ISO15693_OPTION_ON = 1;
	
	public static final int ISO15693_ACTIVATE_ADDRESSED = 0;
	public static final int ISO15693_ACTIVATE_SELECTED = 1;
	
	public static final int ISO15693_INFO_AT_DSFID = 0;
	public static final int ISO15693_INFO_AT_AFI = 1;
	public static final int ISO15693_INFO_AT_BLOCK_NR = 2;
	public static final int ISO15693_INFO_AT_BLOCK_SIZE = 3;
	public static final int ISO15693_INFO_AT_IC = 4;
	
	public static final int ISO15693_UID_LENGTH = 8;
	public static final int ISO15693_UID_AT_DSFID = ISO15693_UID_LENGTH;
	public static final int ISO15693_UID_AT_MORECARD = ISO15693_UID_LENGTH + 1;
	
	public static final int ISO15693_FLAG_UPLINK_RATE_LOW = 0;
	public static final int ISO15693_FLAG_UPLINK_RATE_HIGH = 1;
	
	public static final int ISO15693_FLAG_NO_USE_AFI = 0;
	public static final int ISO15693_FLAG_USE_AFI = 1;
	
	public static final int ISO15693_FLAG_SIXTEEN_SLOTS = 0;
	public static final int ISO15693_FLAG_ONE_SLOTS = 1;
	
	private static final String DEVPATH = "/dev/rc663";
	
	public int InitDevice()
	{
		return init_dev(DEVPATH);
	}
	
	public void ReleaseDevice()
	{
		release_dev();
	}
	
	public byte[] SearchCard(int type, int datarate, int setAFI, byte AFI, int oneslots, byte[] mask, int masklength)
	{
		return search_card(type, datarate, setAFI, AFI, oneslots, mask, masklength);
	}
	
	public byte[] ReadCardInfo()
	{
		return read_card_info();
	}
	
	public int StayQuiet()
	{
		return stay_quiet();
	}
	
	public int Select()
	{
		return select_card();
	}
	
	public int ResetToReady()
	{
		return reset_to_ready();
	}
	
	public byte[] ReadSingleBlock(int option, int block)
	{
		return read_block(option, block);
	}
	
	public byte[] ReadMultipleBlocks(int option, int firstblock, int block_nr)
	{
		return read_multi_block(option, firstblock, block_nr);
	}
	
	public int WriteSingleBlock(int option, int block, byte[] data)
	{
		return write_block(option, block, data);
	}
	
	public int WriteMultipleBlocks(int option, int firstblock, int block_nr, byte[] data)
	{
		return write_multi_block(option, firstblock, block_nr, data);
	}
	
	public int WriteAFI(int option, byte AFI)
	{
		return write_AFI(option, AFI);
	}
	
	public int WriteDSFID(int option, byte DSFID)
	{
		return write_DSFID(option, DSFID);
	}
	
	public int LockBlock(int option, int block)
	{
		return lock_block(option, block);
	}
	
	public int LockAFI(int option)
	{
		return lock_AFI(option);
	}
	
	public int LockDSFID(int option)
	{
		return lock_DSFID(option);
	}
	
	public byte[] GetMultipleBlockSecurityStatus(int firstblock, int nr_block)
	{
		return get_multi_block_security_status(firstblock, nr_block);
	}
	
	private native int init_dev(String path);
	private native void release_dev();
	private native byte[] search_card(int type, int datarate, int setAFI, byte AFI, int oneslots, byte[] mask, int masklength);
	private native byte[] read_card_info();
	private native int stay_quiet();
	private native int select_card();
	private native int reset_to_ready();
	private native byte[] read_block(int option, int block);
	private native byte[] read_multi_block(int option, int firstblock, int block_nr);
	private native int write_block(int option, int block, byte[] data);
	private native int write_multi_block(int option, int firstblock, int block_nr, byte[] data);
	private native int write_AFI(int option, byte AFI);
	private native int write_DSFID(int option, byte DSFID);
	private native int lock_block(int option, int block);
	private native int lock_AFI(int option);
	private native int lock_DSFID(int option);
	private native byte[] get_multi_block_security_status(int firstblock, int nr_block);
	
	static {
		System.loadLibrary("rc663nxp");
		System.loadLibrary("package");
		System.loadLibrary("rc663_15693");
	}
}