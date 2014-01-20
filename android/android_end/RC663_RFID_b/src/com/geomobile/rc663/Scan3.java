package com.geomobile.rc663;

import com.android.hflibs.Iso15693_native;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.*;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.BasicHttpContext;
import org.apache.http.protocol.HttpContext;
import org.json.*;

import android.os.AsyncTask;

public class Scan3 extends ScanActivity implements OnClickListener {
    /** Called when the activity is first created. */
	
	private static final String TAG = "rc663_15693_java";
	private static final String PW_DEV = "/proc/driver/as3992";
	private Iso15693_native dev = new Iso15693_native();
	private Button start_demo;
	private Button get_info;
	private Button submit;
	private TextView main_info;
	private TextView card_info;
	private EditText block_nr;
	private CheckBox lock_block;
	private CheckBox lock_afi;
	private CheckBox lock_dsfid;
	private DeviceControl power;
	private Spinner spinner2;
	private ListView listView;
	public String myTitle = "出库扫描";
	public String myURL =  "";
	
	private int block_max = 0;
	private int block_size = 0;
	
	private String imei = "";
	private ArrayAdapter adapter;
	// private String[] myStringArray = {"gen1", "gen2"};
	private List<String> items = new ArrayList<String>();
	private IOCallback optionFetch, submitController = null;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.myURL = getString(R.string.url_prefix) + "wasteOut";
        setContentView(R.layout.scan34);
        
        TelephonyManager telephonyManager = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);
        this.imei = telephonyManager.getDeviceId();
        ((TextView)findViewById(R.id.textView_addNew)).setText(myTitle);
        
        //start_demo = (Button)findViewById(R.id.button_15693_demo);
        //start_demo.setOnClickListener(this);
        //start_demo.setEnabled(false);
        
        get_info = (Button)findViewById(R.id.button_15693_search);
        get_info.setOnClickListener(this);
        get_info.setEnabled(true);
        
        submit = (Button)findViewById(R.id.button_15693_upload);
        submit.setOnClickListener(this);
        submit.setEnabled(true);
        
        main_info = (TextView)findViewById(R.id.textView_15693_info);
        main_info.setMovementMethod(ScrollingMovementMethod.getInstance());
        card_info = (TextView)findViewById(R.id.textView_15693_cardinfo);
        
        //block_nr = (EditText)findViewById(R.id.editText_15693_block);
        
        //lock_block = (CheckBox)findViewById(R.id.checkBox_15693_lockblock);
        //lock_afi = (CheckBox)findViewById(R.id.checkBox_15693_lockafi);
        //lock_dsfid = (CheckBox)findViewById(R.id.checkBox_15693_lockdsfid);
        
        //items.add(new String("aloha_1"));
        adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, items);
        listView = (ListView) findViewById(R.id.listView1);
        listView.setLongClickable(true);
        listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {

            public boolean onItemLongClick(AdapterView<?> arg0, View arg1,
                    int pos, long id) {
                // TODO Auto-generated method stub

                handleItemModification(listView.getItemAtPosition(pos).toString());

                return true;
            }
        }); 
        listView.setAdapter(adapter);
        
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

    public void handleItemModification(final String itemName)
    {
    	DialogInterface.OnClickListener dialogClickListener = new DialogInterface.OnClickListener() {
    	    @Override
    	    public void onClick(DialogInterface dialog, int which) {
    	        switch (which){
    	        case DialogInterface.BUTTON_POSITIVE:
    	            //Yes button clicked
    	        	debugMessage("YES=" + itemName);
    	        	deleteItemFromListWithName(itemName);
    	            break;

    	        case DialogInterface.BUTTON_NEGATIVE:
    	            //No button clicked
    	        	debugMessage("No");
    	            break;
    	        }
    	    }
    	};

    	AlertDialog.Builder builder = new AlertDialog.Builder(this);
    	builder.setMessage("是否删除「" + itemName + "」").setPositiveButton("是", dialogClickListener)
    	    .setNegativeButton("否", dialogClickListener).show();
    }
    
    public void addNewItemToList(String sn)
    {
    	if(items.contains(sn)) {
    		this.alertMessage("重复条目");
    		return;
    	}
    	String key = sn;
    	items.add(key);
    	Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
    	Ringtone r = RingtoneManager.getRingtone(getApplicationContext(), notification);
    	r.play();
    	
		adapter.notifyDataSetChanged();
    }
    
    public void deleteItemFromListWithName(String itemName)
    {
    	items.remove(itemName);
    	
		adapter.notifyDataSetChanged();
    }
    
    public void debugMessage(String msg)
    {
    	TextView main_info = (TextView)findViewById(R.id.textView_15693_info);
		main_info.setText(msg);
    }
    
    public class NullCallback implements IOCallback {
    	public void httpRequestDidFinish(int success, String value) {
    		
    	}
    }
    
    public class SubmitCallbackController implements IOCallback {
    	Scan3 activity;
    	ProgressDialog progDialog;
    	List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
    	public SubmitCallbackController(Scan3 activity, JSONObject postJson) {
    		this.activity = activity;
    		NameValuePair postContent = new BasicNameValuePair("txt_json", postJson.toString());
    		nameValuePairs.add(postContent);
    		new LongRunningGetIO(activity.myURL, nameValuePairs, this).execute();
    		
    		progDialog = ProgressDialog.show(activity, "正在上传",
    	            "请稍候...", true);
    	}
    	private void parseJSON(String value)
    	{
    		Log.d(TAG, value);
    		ErrorParser.parse(activity, value);
    		
    	}
    	
    	public void httpRequestDidFinish(int success, String value) {
    		progDialog.dismiss();
    		
    		this.parseJSON(value);
	        
	        activity.submitController = null;
    	}
    }
    
    
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		if(arg0 == get_info)
		{
			Log.d(TAG, "get_info clicked");
			
			final ProgressDialog scanningDialog = ProgressDialog.show(this, "正在扫描 RFID 设备",
    	            "请稍候...", true);
			Thread newThread = new Thread() {
				@Override
				public void run() {
					try {
						sleep(1000);
						scanningDialog.dismiss();
					} catch (InterruptedException e) {
						scanningDialog.dismiss();
						e.printStackTrace();
					}
				}
			};
			newThread.start();
			
			String sn = Scanner.scan();
			if(sn == null) {
				this.alertMessage("未找到 RFID 设备");
				return;
			}
			
			this.addNewItemToList(sn);
		

		} else if(arg0 == submit) {
			if(items.size() == 0) {
				this.alertMessage("列表为空");
				return;
			}
			
			JSONArray myjson = new JSONArray();
			Iterator it = items.iterator();
		    while (it.hasNext()) {
		        String thissn = (String)it.next();
		        JSONObject newObj = new JSONObject();
		        try {
					newObj.put("rfid", thissn);
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					Log.d(TAG, "parse error");
					e.printStackTrace();
				}
		        myjson.put(newObj);
		        it.remove(); // avoids a ConcurrentModificationException
		    }
		    JSONObject myupload = new JSONObject();
		    try {
				myupload.put("rfidlist", myjson);
				myupload.put("imei", this.imei);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		    if(submitController == null) submitController = new SubmitCallbackController(this, myupload);
		    adapter.notifyDataSetChanged();
		}
	}
    
}
