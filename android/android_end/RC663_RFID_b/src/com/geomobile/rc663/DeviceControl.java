package com.geomobile.rc663;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import android.util.Log;

public class DeviceControl
{
	private BufferedWriter CtrlFile = null;
	
	public int DeviceOpen(String path)// throws IOException	
	{
		File DeviceName = new File(path);
		try {
			CtrlFile = new BufferedWriter(new FileWriter(DeviceName, false));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			return -1;
		}	//open file
		return 0;
	}
	
	public int PowerOnDevice()// throws IOException		//poweron barcode device
	{
		try {
			CtrlFile.write("on");
			CtrlFile.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			return -1;
		}
		return 0;
	}
	
	public int PowerOffDevice()// throws IOException	//poweroff barcode device
	{
  		try {
			CtrlFile.write("off");
			CtrlFile.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			return -1;
		}
  		return 0;
	}
	
	public int DeviceClose()// throws IOException		//close file
	{
		try {
			CtrlFile.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			return -1;
		}
		return 0;
	}
}