package com.geomobile.rc663;

import com.android.hflibs.Iso15693_native;

import android.app.Activity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;

public class Iso15693 extends Activity implements OnClickListener {
    /** Called when the activity is first created. */
	
	private static final String TAG = "rc663_15693_java";
	private static final String PW_DEV = "/proc/driver/as3992";
	private Iso15693_native dev = new Iso15693_native();
	private Button start_demo;
	private Button get_info;
	private TextView main_info;
	private TextView card_info;
	private EditText block_nr;
	private CheckBox lock_block;
	private CheckBox lock_afi;
	private CheckBox lock_dsfid;
	private DeviceControl power;
	
	private int block_max = 0;
	private int block_size = 0;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.iso15693);
        
        start_demo = (Button)findViewById(R.id.button_15693_demo);
        start_demo.setOnClickListener(this);
        start_demo.setEnabled(false);
        
        get_info = (Button)findViewById(R.id.button_15693_search);
        get_info.setOnClickListener(this);
        get_info.setEnabled(false);
        
        main_info = (TextView)findViewById(R.id.textView_15693_info);
        main_info.setMovementMethod(ScrollingMovementMethod.getInstance());
        card_info = (TextView)findViewById(R.id.textView_15693_cardinfo);
        
        block_nr = (EditText)findViewById(R.id.editText_15693_block);
        
        lock_block = (CheckBox)findViewById(R.id.checkBox_15693_lockblock);
        lock_afi = (CheckBox)findViewById(R.id.checkBox_15693_lockafi);
        lock_dsfid = (CheckBox)findViewById(R.id.checkBox_15693_lockdsfid);
        
        power = new DeviceControl();
        if(power.DeviceOpen(PW_DEV) < 0)
        {
        	main_info.setText(R.string.msg_error_power);
        	return;
        }
        Log.d(TAG, "open file ok");
        
        if(power.PowerOnDevice() < 0)
        {
        	power.DeviceClose();
        	main_info.setText(R.string.msg_error_power);
        	return;
        }
        Log.d(TAG, "open power ok");
        
        try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
		}
        
        Log.d(TAG, "begin to init");
        if(dev.InitDevice() != 0)
        {
        	power.PowerOffDevice();
        	power.DeviceClose();
        	main_info.setText(R.string.msg_error_dev);
        	return;
        }
        Log.d(TAG, "init ok");
        get_info.setEnabled(true);
    }
    
    @Override
    public void onDestroy()
    {
    	Log.d(TAG, "on destory");
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
			byte[] uid = dev.SearchCard(Iso15693_native.ISO15693_ACTIVATE_ADDRESSED, Iso15693_native.ISO15693_FLAG_UPLINK_RATE_HIGH, Iso15693_native.ISO15693_FLAG_NO_USE_AFI, (byte)0, Iso15693_native.ISO15693_FLAG_ONE_SLOTS, null, 0);
			if(uid == null)
			{
				card_info.setText("Error search card");
				main_info.setText("");
				return;
			}
			card_info.setText("SN: 0x");
			for(int i = Iso15693_native.ISO15693_UID_LENGTH - 1; i >= 0; i--)
			{
				card_info.append(String.format("%02X", uid[i]));
			}
		
			byte[] cinfo = dev.ReadCardInfo();
			if(cinfo == null)
			{
				main_info.setText("Error get cardinfo, maybe card don't support");
				return;
			}
			main_info.setText(String.format("AFI:					0x%x\n", cinfo[Iso15693_native.ISO15693_INFO_AT_AFI]));
			main_info.append(String.format("DSFID:				0x%x\n", cinfo[Iso15693_native.ISO15693_INFO_AT_DSFID]));
			main_info.append(String.format("BLOCK NUMBERS:	%d\n", cinfo[Iso15693_native.ISO15693_INFO_AT_BLOCK_NR]));
			main_info.append(String.format("BLOCK SIZE:			%d\n", cinfo[Iso15693_native.ISO15693_INFO_AT_BLOCK_SIZE]));
			main_info.append(String.format("IC :					0x%x\n\n", cinfo[Iso15693_native.ISO15693_INFO_AT_IC]));
			start_demo.setEnabled(true);
			
			block_max = cinfo[Iso15693_native.ISO15693_INFO_AT_BLOCK_NR];
			block_size = cinfo[Iso15693_native.ISO15693_INFO_AT_BLOCK_SIZE];
		}
		else if(arg0 == start_demo)
		{
			int block;
			try
			{
				block = Integer.valueOf(block_nr.getText().toString());
			}
			catch(NumberFormatException p)
			{
				main_info.setText(R.string.msg_error_input);
				return;
			}
			if(block < 0)
			{
				block = 0;
				block_nr.setText(Integer.valueOf(block));
			}
			if(block >= block_max)
			{
				block = block_max - 1;
				block_nr.setText(Integer.valueOf(block));
			}
			
			//write a number to block
			byte[] data = new byte[block_size];
			for(int i = 0; i < block_size; i++)
			{
				data[i] = (byte)(i + 10);
			}
			if(dev.WriteSingleBlock(Iso15693_native.ISO15693_OPTION_OFF, block, data) != 0)
			{
				main_info.append("write block error, maybe block is locked\n\n");
			}
			else
			{
				main_info.append(String.format("write block %d value: ", block));
				for(byte s : data)
				{
					main_info.append(String.format("0x%02X ", s));
				}
				main_info.append(" ok\n\n");
			}
			
			//read a block
			byte[] res = dev.ReadSingleBlock(Iso15693_native.ISO15693_OPTION_OFF, block);
			if(res == null)
			{
				main_info.append("read block error");
				return;
			}
			main_info.append(String.format("read block %d value is ", block));
			for(byte x : res)
			{
				main_info.append(String.format("0x%02X ", x));
			}
			main_info.append("\n\n");
			
			//write multi blocks
			int vblock = block;
			data = new byte[block_size * 3];
			for(int i = 0; i < data.length; i++)
			{
				data[i] = (byte)(i + 10);
			}
			if(dev.WriteMultipleBlocks(Iso15693_native.ISO15693_OPTION_OFF, block, 3, data) != 0)
			{
				main_info.append("write multi blocks failed, maybe card don't support or some blocks is locked\n\n");
			}
			else
			{
				main_info.append("write multi blocks, write value is:");
				for(int i = 0; i < data.length; i++)
				{
					if(i % block_size == 0)
					{
						main_info.append(String.format("\nblock %d:		", vblock++));
					}
					main_info.append(String.format("0x%02X", data[i]));
				}
				main_info.append("\n\n");
			}
			
			//read multi blocks
			res = dev.ReadMultipleBlocks(Iso15693_native.ISO15693_OPTION_OFF, block, 16);
			vblock = block;
			if(res == null)
			{
				main_info.append("read multi block error, maybe card don't suppor\n\n");
			}
			else
			{
				main_info.append("read multi blocks ok, results is:");
				for(int i = 0; i < res.length; i++)
				{
					if(i % block_size == 0)
					{
						main_info.append(String.format("\nblock %d:		", vblock++));
					}
					main_info.append(String.format("0x%02X ", res[i]));
				}
				main_info.append("\n\n");
			}
			
			//wrtie AFI
			if(dev.WriteAFI(Iso15693_native.ISO15693_OPTION_OFF, (byte)0x33) != 0)
			{
				main_info.append("Write AFI failed, maybe card don't support or locked\n\n");
			}
			else
			{
				main_info.append(String.format("Write AFI valud 0x%02x ok\n", 0x33));
			}
			
			//write DSFID
			if(dev.WriteDSFID(Iso15693_native.ISO15693_OPTION_OFF, (byte)0x22) != 0)
			{
				main_info.append("Write DSFID failed, maybe card don't supprot or locked\n\n");
			}
			else
			{
				main_info.append(String.format("Write DSFID value 0x%02x ok\n\n", 0x22));
			}
			
			//read AFI & DSFID again
			byte[] cinfo = dev.ReadCardInfo();
			if(cinfo == null)
			{
				main_info.append("Error read AFI and DSFID info\n\n");
			}
			else
			{
				main_info.append("Read AFI and DSFID again:\n");
				main_info.append(String.format("AFI value: 		0x%x\n", cinfo[Iso15693_native.ISO15693_INFO_AT_AFI]));
				main_info.append(String.format("DSFID value: 	0x%x\n\n", cinfo[Iso15693_native.ISO15693_INFO_AT_DSFID]));
			}
			
			//lock block
			if(lock_block.isChecked())
			{
				if(dev.LockBlock(Iso15693_native.ISO15693_OPTION_OFF, block) != 0)
				{
					main_info.append(String.format("Lock block %d failed, maybe the block is already locked\n\n", block));
				}
				else
				{
					main_info.append(String.format("Lock block %d ok\n\n", block));
				}
			}
			
			//lock afi
			if(lock_afi.isChecked())
			{
				if(dev.LockAFI(Iso15693_native.ISO15693_OPTION_OFF) != 0)
				{
					main_info.append("Lock AFI failed, maybe card don't support or AFI is already locked\n\n");
				}
				else
				{
					main_info.append("Lock AFI ok\n\n");
				}
			}
			
			//lock dsfid
			if(lock_dsfid.isChecked())
			{
				if(dev.LockDSFID(Iso15693_native.ISO15693_OPTION_OFF) != 0)
				{
					main_info.append("Lock DSFID failed, maybe card don't support or DSFID is already locked\n\n");
				}
				else
				{
					main_info.append("Lock DSFID ok\n\n");
				}
			}
			
			//get blocks status
			res = dev.GetMultipleBlockSecurityStatus(block, 3);
			if(res == null)
			{
				main_info.append("Get multi blocks security status failed, maybe card don't support\n");
				return;
			}
			main_info.append("Get multi blocks security status ok, result is:\n");
			vblock = block;
			for(byte s : res)
			{
				main_info.append(String.format("block %d:	", vblock++) + (s == 0 ? "unlocked" : "locked") + "\n");
			}
			main_info.append("\n");
			
			start_demo.setEnabled(false);
		}
	}
    
}
