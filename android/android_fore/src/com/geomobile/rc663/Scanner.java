package com.geomobile.rc663;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.util.Log;

import com.android.hflibs.Iso15693_native;
import com.android.hflibs.Mifare_native;

public class Scanner {
	
	private static final String PW_DEV = "/proc/driver/as3992";
	
	public static String try1443a()
	{
		Mifare_native dev1443a = new Mifare_native();
		DeviceControl power = new DeviceControl();
        if(power.DeviceOpen(PW_DEV) < 0)
        {
        	return null;
        }
        
        if(power.PowerOnDevice() < 0)
        {
        	power.DeviceClose();
        	return null;
        }
        
        try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
		}
        
        if(dev1443a.InitDev() != 0)
        {
        	power.PowerOffDevice();
        	power.DeviceClose();
        	return null;
        }
		byte[] ID = dev1443a.SearchCard();
		dev1443a.ReleaseDev();
    	power.PowerOffDevice();
    	power.DeviceClose();
		Log.d("ALERT", "wtf");
		if(ID == null) return null;
		String IDString = "";
		for(byte a : ID)
		{
			IDString += String.format("%02X", a);
		}
		return IDString;
	}
	
	public static String scan() {
		Iso15693_native dev = new Iso15693_native();
		DeviceControl power = new DeviceControl();
        if(power.DeviceOpen(PW_DEV) < 0)
        {
        	return null;
        }
        
        if(power.PowerOnDevice() < 0)
        {
        	power.DeviceClose();
        	return null;
        }
        
        try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
		}
        
        if(dev.InitDevice() != 0)
        {
        	power.PowerOffDevice();
        	power.DeviceClose();
        	return null;
        }
		byte[] uid = dev.SearchCard(Iso15693_native.ISO15693_ACTIVATE_ADDRESSED, Iso15693_native.ISO15693_FLAG_UPLINK_RATE_HIGH, Iso15693_native.ISO15693_FLAG_NO_USE_AFI, (byte)0, Iso15693_native.ISO15693_FLAG_ONE_SLOTS, null, 0);
		dev.ReleaseDevice();
    	power.PowerOffDevice();
    	power.DeviceClose();
		
		if(uid == null)
		{ 
			return Scanner.try1443a();
		}
		
		String sn = "";
		for(int i = Iso15693_native.ISO15693_UID_LENGTH - 1; i >= 0; i--)
		{
			
			sn = sn + String.format("%02X", uid[i]);
		}
		return sn;
		
	}
}
