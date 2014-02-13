package com.android.hflibs;


public class SecondGID_native
{
	private static final String DEVPATH = "/dev/rc663";
	
	public int InitDevice()
	{
		return init_dev(DEVPATH);
	}
	
	public void ReleaseDevice()
	{
		release_dev();
	}
	
	public byte[] SearchCard()
	{
		return search_card();
	}
	
	public byte[] GetUID()
	{
		return get_uid();
	}
	
	private native int init_dev(String path);
	private native void release_dev();
	private native byte[] search_card();
	private native byte[] get_uid();
	
	static {
		System.loadLibrary("rc663nxp");
		System.loadLibrary("package");
		System.loadLibrary("rc663_2ndGID");
	}
}