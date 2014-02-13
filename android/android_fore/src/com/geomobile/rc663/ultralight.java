package com.geomobile.rc663;


import com.android.hflibs.ultralight_native;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class ultralight extends Activity implements OnClickListener{

	private static final String PW_DEV = "/proc/driver/as3992";
	private ultralight_native dev = new ultralight_native();
	private Button start_demo;
	private TextView main_info;
	private EditText block_nr;
	private DeviceControl power;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ultralight);
        Log.d("mifare", "in oncreate");
       
        start_demo = (Button)findViewById(R.id.button_mifare_set);
        start_demo.setOnClickListener(this);
        start_demo.setEnabled(false);
        
        main_info = (TextView)findViewById(R.id.textView_mifare_info);
        block_nr = (EditText)findViewById(R.id.editText_mifare_block);
        
        power = new DeviceControl();
        if(power.DeviceOpen(PW_DEV) < 0)
        {
        	main_info.setText("msg_error_open");
        	return;
        }
        
        if(power.PowerOnDevice() < 0)
        {
        	power.DeviceClose();
        	main_info.setText("msg_error_power");
        	return;
        }
        
        try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
		}
        
        Log.d("mifare", "begin to init");
        if(dev.InitDev() != 0)
        {
        	power.PowerOffDevice();
        	power.DeviceClose();
        	main_info.setText("msg_error_dev");
        	return;
        }
        Log.d("mifare", "init ok");
        start_demo.setEnabled(true);
    }

     @Override
       public void onDestroy()
    {
    	dev.ReleaseDev();
    	power.PowerOffDevice();
    	power.DeviceClose();
    	super.onDestroy();
    }
		public void onClick(View arg0)		 
		{
			// TODO Auto-generated method stub
			if(arg0 == start_demo)
			{
				int block;
				main_info.setText("msg_start");
				try
				{
					block = Integer.valueOf(block_nr.getText().toString());
				}
				catch(NumberFormatException p)
				{
					main_info.setText("msg_error_input");
					return;
				}
				if(block < 0)
				{
					block = 0;
					block_nr.setText(Integer.valueOf(block));
				}
				if(block >= 16)
				{
					ultralight.this.finish();
				}
				//search a valid card
				byte[] ID = dev.SearchCard();
				if(ID == null)
				{
					main_info.setText("msg_mifare_error_nocard");
					return;
				}
				String IDString = new String(" 0x");
				for(byte a : ID)
				{
					IDString += String.format("%02X", a);
				}
				main_info.setText("msg_mifare_ok_findcard");
				main_info.append(IDString);
				main_info.append("\n\n");
				
				//read data from the same block
				byte[] getdata = dev.ReadBlock(block);
				if(getdata == null)
				{
					main_info.append(getString(R.string.msg_mifare_error_readblock));
					return;
				}
				String getdataString = new String();
				for(byte i : getdata)
				{
					getdataString += String.format(" 0x%02x", i);
				}
				main_info.append(getString(R.string.msg_mifare_ok_readblock));
				main_info.append(getdataString);
				main_info.append("\n\n"); 
				
				//write data to block directly
				byte[] data = new byte[4];
				String dataString = new String();
				for(int i = 0; i < 4; i++)
				{
					data[i] = (byte)(i + 10);
					dataString += String.format(" 0x%02x", data[i]);
				}
				if(dev.WriteBlock(block, data) != 0)
				{
					main_info.append(getString(R.string.msg_mifare_error_writeblock));
					return;
				}
				main_info.append(getString(R.string.msg_mifare_ok_writeblock));
				main_info.append(dataString);
				main_info.append("\n\n");
				
				//write compatibility data to block directly
				byte[] data1 = new byte[16];
				String dataString1 = new String();
				for(int i = 0; i < 16; i++)
				{
					data1[i] = (byte)(i + 10);
					dataString1 += String.format(" 0x%02x", data1[i]);
				}
				if(dev.compatibility_Write_Block(block, data1) != 0)
				{
					main_info.append(getString(R.string.msg_mifare_error_compatibility_read));
					return;
				}
				main_info.append(getString(R.string.msg_mifare_ok_compatibility_read));
				main_info.append(dataString);
				main_info.append("\n\n");
				
				//halt current card
				if(dev.HaltCard() != 0)
				{
					main_info.append(getString(R.string.msg_mifare_error_haltcard));
					return;
				}
				main_info.append(getString(R.string.msg_mifare_ok_haltcard));
				main_info.append("\n\n");
				
				main_info.append(getString(R.string.msg_mifare_ok_allok));
				
			}	
		}

}
