package com.geomobile.rc663;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class RC663_RFIDActivity extends Activity implements OnClickListener {
    /** Called when the activity is first created. */
	
	private Button start_mifare;
	private Button start_15693;	private Button start_2ndGID;
	private Button start_ultralight;
	private Button start_scan;
	private Button start_scan2;
	private Button start_scan3;
	private Button start_scan4;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        //start_mifare = (Button)findViewById(R.id.button_mifare);
        //start_15693 = (Button)findViewById(R.id.button_15693);
        //start_2ndGID = (Button)findViewById(R.id.button_2ndGID);
        //start_ultralight=(Button)findViewById(R.id.button_ultralight);
        start_scan = (Button)findViewById(R.id.button_ScanAndUpload);
        start_scan2 = (Button)findViewById(R.id.button_scan2);
        start_scan3 = (Button)findViewById(R.id.button_scan3);
        start_scan4 = (Button)findViewById(R.id.button_scan4);
        
        //start_mifare.setOnClickListener(this);
        //start_15693.setOnClickListener(this);
        //start_2ndGID.setOnClickListener(this);
        //start_ultralight.setOnClickListener(this);
        start_scan.setOnClickListener(this);
        start_scan2.setOnClickListener(this);
        start_scan3.setOnClickListener(this);
        start_scan4.setOnClickListener(this);
    }

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		if(arg0 == start_mifare)
		{
			Intent i = new Intent();
			i.setClass(this, Mifare.class);
			startActivity(i);
			finish();
		}
		else if(arg0 == start_15693)
		{
			Intent i = new Intent();
			i.setClass(this, Iso15693.class);
       		startActivity(i);
       		finish();
		}
		else if(arg0 == start_2ndGID)		{
			Intent i = new Intent();
			i.setClass(this, SecondGID.class);
			startActivity(i);
			finish();
		}
		else if(arg0 == start_ultralight)
		{
			Intent i = new Intent();
			i.setClass(this, ultralight.class);
			startActivity(i);
			finish();
		}
		else if(arg0 == start_scan) {
			Intent i = new Intent();
			i.setClass(this, ScanAndUpload.class);
			startActivity(i);
			//finish();
		} else if (arg0 == start_scan2) {
			Intent i = new Intent();
			i.setClass(this, Scan2.class);
			startActivity(i);
			//finish();
		} else if (arg0 == start_scan3) {
			Intent i = new Intent();
			i.setClass(this, Scan3.class);
			startActivity(i);
			//finish();
		} else if (arg0 == start_scan4) {
			Intent i = new Intent();
			i.setClass(this, Scan4.class);
			startActivity(i);
			//finish();
		}
	}
}