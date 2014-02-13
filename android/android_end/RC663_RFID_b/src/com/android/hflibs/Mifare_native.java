package com.android.hflibs;


public class Mifare_native {
	
	public static final int AUTH_TYPEA = 0xA;
	public static final int AUTH_TYPEB = 0xB;
	
	private static final int MAX_KEYSTORE = 80;	//real max value is 84
	
	private static final String DEVPATH = "/dev/rc663";
	
	byte[] current_cid = null;
	
	public int InitDev()
	{
		return init_dev(DEVPATH);
	}
	
	public void ReleaseDev()
	{
		release_dev();
	}
	
	public byte[] SearchCard()
	{
		byte[] tag = search_card();
		if(tag == null)
		{
			return null;
		}
		current_cid = tag;
		return tag;
	}
	
	public int HaltCard()
	{
		current_cid = null;
		return halt_card();
	}
	
	public int ActiveCard(byte[] cid)
	{
		return active_card(cid);
	}
	
	public int StoreKeyToEEPROM(int place, byte[] key)
	{
		return store_key_eeprom(place, key);
	}
	
	public int AuthenticationCardByKey(int type, byte[] cid, int block, byte[] key)
	{
		return auth_card_key(type, cid, block, key);
	}
	
	public int AuthenticationCardByEEPROM(int type, byte[] cid, int block, int place)
	{
		return auth_card_eeprom(type, cid, block, place);
	}
	
	public byte[] ReadBlock(int block)
	{
		return read_block(block);
	}
	
	public int[] ReadBlockValue(int block)
	{
		return read_block_value(block);
	}
	
	public int WriteBlock(int block, byte[] data)
	{
		return write_block(block, data);
	}
	
	public int WriteBlockValue(int block, int value)
	{
		return write_block_value(block, value);
	}
	
	public int IncrementBlockValue(int block, int value)
	{
		return inc_value(block, block, value);
	}
	
	public int DecrementBlockValue(int block, int value)
	{
		return dec_value(block, block, value);
	}
	
	public int IncrementBlockValueToAnotherBlock(int sBlock, int dBlock, int value)
	{
		return inc_value(sBlock, dBlock, value);
	}
	
	public int DecrementBlockValueToAnotherBlock(int sBlock, int dBlock, int value)
	{
		return dec_value(sBlock, dBlock, value);
	}
	
	private native int init_dev(String path);
	private native void release_dev();
	private native byte[] search_card();
	private native int halt_card();
	private native int active_card(byte[] cid);
	private native int store_key_eeprom(int place, byte[] key);
	private native int auth_card_key(int type, byte[] cid, int block, byte[] key);
	private native int auth_card_eeprom(int type, byte[] cid, int block, int place);
	private native byte[] read_block(int block);
	private native int[] read_block_value(int block);
	private native int write_block(int block, byte[] data);
	private native int write_block_value(int block, int value);
	private native int inc_value(int srcBlock, int destBlock, int value);
	private native int dec_value(int srcBlock, int destBlock, int value);
	
	static {
		System.loadLibrary("rc663nxp");
		System.loadLibrary("package");
		System.loadLibrary("rc663mifare");
	}
}