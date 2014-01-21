package com.android.hflibs;

public class ultralight_native {
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
	public byte[] ReadBlock(int block)
	{
		return read_block(block);
	}
	public int WriteBlock(int block, byte[] data)
	{
		return write_block(block, data);
	}
	public int compatibility_Write_Block(int block, byte[] data)
	{
		return compatibility_write(block, data);
	}
	private native int init_dev(String path);
	private native void release_dev();
	private native byte[] search_card();
	private native byte[] request();
	private native byte[] wakeup();
	private native int active_card(byte[] cid);
	private native int halt_card();
	private native int ulcauthenticate(int keynumber,int keyversion);
	private native byte[] read_block(int block);
	private native int write_block(int block, byte[] data);
	private native int compatibility_write(int block, byte[] data);
	
	static {
		System.loadLibrary("rc663nxp");
		System.loadLibrary("package");
		System.loadLibrary("rc663ultraght");
	}

}
