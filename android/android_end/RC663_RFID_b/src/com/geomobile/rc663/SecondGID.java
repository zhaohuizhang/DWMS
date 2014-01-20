package com.geomobile.rc663;

import com.android.hflibs.SecondGID_native;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;


public class SecondGID extends Activity implements OnClickListener {
	
	private static final String PW_DEV = "/proc/driver/as3992";
	private SecondGID_native dev = new SecondGID_native();
	private Button get_info;
	private TextView main_info;
	private DeviceControl power;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.sgid);
        
        get_info = (Button)findViewById(R.id.button_2ndGID_getinfo);
        get_info.setOnClickListener(this);
        get_info.setEnabled(false);
        
        main_info = (TextView)findViewById(R.id.textView_2ndGID_info);
        
        power = new DeviceControl();
        if(power.DeviceOpen(PW_DEV) < 0)
        {
        	main_info.setText(R.string.msg_error_power);
        	return;
        }
        
        if(power.PowerOnDevice() < 0)
        {
        	power.DeviceClose();
        	main_info.setText(R.string.msg_error_power);
        	return;
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
        	main_info.setText("Open device error");
        	return;
        }
        
        get_info.setEnabled(true);
    }
    
    @Override
    public void onDestroy()
    {
    	dev.ReleaseDevice();
    	power.PowerOffDevice();
    	power.DeviceClose();
    	super.onDestroy();
    }

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		
		if(arg0 == get_info)
		{
			byte[] pupi = dev.SearchCard();
			if(pupi == null)
			{
				main_info.setText("No card searched");
				return;
			}
			main_info.setText("PUPI value is : ");
			for(byte x : pupi)
			{
				main_info.append(String.format("0x%02x ", x));
			}
			main_info.append("\n\n");
			
			byte[] uid = dev.GetUID();
			if(uid == null)
			{
				main_info.append("Get UID info failed");
				return;
			}
			main_info.append("UID value is : ");
			for(byte x : uid)
			{
				main_info.append(String.format("0x%02x ", x));
			}
		}
	}
	
}