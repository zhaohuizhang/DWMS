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

public class ScanAndUpload extends ScanActivity implements OnClickListener {
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
	
	private int block_max = 0;
	private int block_size = 0;
	
	private String imei = "";
	private ArrayAdapter adapter;
	// private String[] myStringArray = {"gen1", "gen2"};
	private List<String> items = new ArrayList<String>();
	private IOCallback optionFetch, submitController = null;
	private HashMap<String, String> wasteOptionsMap = new HashMap<String, String>();
	private HashMap<String, String> wasteItemTypeMap = new HashMap<String, String>();
	private HashMap<String, String> wasteItemSNMap = new HashMap<String, String>();
	private HashMap<String, String> wasteItemWayMap = new HashMap<String, String>();
	
	
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.scanandupload);
        
        TelephonyManager telephonyManager = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);
        this.imei = telephonyManager.getDeviceId();
        
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
        
        spinner2 = (Spinner) findViewById(R.id.spinner2);
        //this.addItemsOnSpinner2();
        optionFetch = new OptionsCallbackController(this);
        
    }
    
    public void addItemsOnSpinner2(List<String> list) {
    	ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(this,
    		android.R.layout.simple_spinner_item, list);
    	dataAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
    	spinner2.setAdapter(dataAdapter);
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
    
    public void addNewItemToList(String sn, Integer way)
    {
    	String selectItem = (String)spinner2.getSelectedItem();
    	String selectId = wasteOptionsMap.get(selectItem);
    	String key = new String(/*(wasteItemTypeMap.size() + 1) + ". " + */sn);
    	items.add(key);
    	wasteItemTypeMap.put(key, selectId);
    	wasteItemSNMap.put(key, sn);
    	wasteItemWayMap.put(key, way.toString());
    	Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
    	Ringtone r = RingtoneManager.getRingtone(getApplicationContext(), notification);
    	r.play();
    	
		adapter.notifyDataSetChanged();
    }
    
    public void deleteItemFromListWithName(String itemName)
    {
    	items.remove(itemName);
    	wasteItemTypeMap.remove(itemName);
    	wasteItemSNMap.remove(itemName);
    	
		adapter.notifyDataSetChanged();
    }
    
    
    public void popupSelect(final String sn)
    {
    	AlertDialog levelDialog;
    	final ScanAndUpload myself = this;

    	// Strings to Show In Dialog with Radio Buttons
    	final CharSequence[] items = {"桶装","袋装"};
    	            
    	                // Creating and Building the Dialog 
    	                AlertDialog.Builder builder = new AlertDialog.Builder(this);
    	                builder.setTitle("请选择装包方式");
    	                builder.setSingleChoiceItems(items, -1, new DialogInterface.OnClickListener() {
    	                public void onClick(DialogInterface dialog, int item) {
    	                   
    	                	dialog.dismiss();
    	                    if(myself.wasteItemSNMap.containsValue(sn)) {
    	                    	myself.alertMessage("已经添加过该废弃物");
    	                    } else {
    	                    	myself.addNewItemToList(sn, item);
    	                    }
    	                        
    	                    }
    	                });
    	                levelDialog = builder.create();
    	                levelDialog.show();
    }
    
    public void debugMessage(String msg)
    {
    	TextView main_info = (TextView)findViewById(R.id.textView_15693_info);
		main_info.setText(msg);
		
		/*
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Debug msg")
        .setMessage(msg)
        .setCancelable(false)
        .setNegativeButton("确定",new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.cancel();
            }
        });
        AlertDialog alert = builder.create();
        alert.show();
        */
    }
    
    public class NullCallback implements IOCallback {
    	public void httpRequestDidFinish(int success, String value) {
    		
    	}
    }
    
    public class OptionsCallbackController implements IOCallback {
    	ScanAndUpload activity;
    	List<String> list = new ArrayList<String>();
    	public OptionsCallbackController(ScanAndUpload activity) {
    		this.activity = activity;
    		new LongRunningGetIO(getString(R.string.url_prefix) + "getWasteName?imei=" + activity.imei, null, this).execute();
    	}
    	
    	private void parseJSON(String value) throws JSONException
    	{
    		JSONObject jObject = new JSONObject(value);
    		JSONArray jArray = jObject.getJSONArray("wasteOptions");
    		for (int i=0; i < jArray.length(); i++)
    		{
    		    try {
    		        JSONObject oneObject = jArray.getJSONObject(i);
    		        // Pulling items from the array
    		        String optionName = oneObject.getString("name");
    		        String optionId = oneObject.getString("id");
    		        activity.wasteOptionsMap.put(optionName, optionId);
    		        list.add(optionName);
    		    } catch (JSONException e) {
    		        // Oops
    		    	activity.alertMessage("返回列表解析错误");
    		    }
    		}
    	}
    	
    	public void httpRequestDidFinish(int success, String value) {
    		if(success == 0) {
    			AlertDialog.Builder builder = new AlertDialog.Builder(activity);
		        builder.setTitle("不能连接到服务器")
		        .setMessage("获取废弃物列表错误")
		        .setCancelable(false)
		        .setNegativeButton("确定",new DialogInterface.OnClickListener() {
		            public void onClick(DialogInterface dialog, int id) {
		                dialog.cancel();
		            }
		        });
		        AlertDialog alert = builder.create();
		        alert.show();
		        return;
    		}
    		activity.debugMessage(value);
    		try {
				this.parseJSON(value);
				activity.addItemsOnSpinner2(list);
				
			} catch (JSONException e) {
				ErrorParser.parse(activity, value);
				e.printStackTrace();
			}
    		
    	}
    }
    
    public class SubmitCallbackController implements IOCallback {
    	ScanAndUpload activity;
    	ProgressDialog progDialog;
    	List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
    	public SubmitCallbackController(ScanAndUpload activity, JSONArray postJson) {
    		this.activity = activity;
    		JSONObject newPostJson = new JSONObject();
    		try {
				newPostJson.put("imei", activity.imei);
				newPostJson.put("wasteBindList", postJson);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    		NameValuePair postContent = new BasicNameValuePair("txt_json", newPostJson.toString());
    		Log.d(TAG, newPostJson.toString());
    		nameValuePairs.add(postContent);
    		new LongRunningGetIO(getString(R.string.url_prefix) + "bindRfid", nameValuePairs, this).execute();
    		
    		progDialog = ProgressDialog.show(activity, "正在上传",
    	            "请稍候...", true);
    	}
    	private void parseJSON(String value)
    	{
    		ErrorParser.parse(activity, value);
    	}
    	
    	public void httpRequestDidFinish(int success, String value) {
    		progDialog.dismiss();
    		
    		// nameValuePairs.add(new BasicNameValuePair("id", "12345"));
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
			
			
			this.popupSelect(sn);
		
		} else if(arg0 == submit) {
			if(wasteItemSNMap.size() == 0) {
				this.alertMessage("列表为空");
				return;
			}
			
			JSONArray myjson = new JSONArray();
			Iterator it = wasteItemTypeMap.entrySet().iterator();
		    while (it.hasNext()) {
		        Map.Entry pairs = (Map.Entry)it.next();
		        //System.out.println(pairs.getKey() + " = " + pairs.getValue());
		        try {
					// myjson.put(wasteItemSNMap.get(pairs.getKey()), pairs.getValue());
		        	JSONObject myobj = new JSONObject();
		        	myobj.put("rfid", wasteItemSNMap.get(pairs.getKey()));
		        	myobj.put("wasteid", pairs.getValue());
		        	myobj.put("addway", wasteItemWayMap.get(pairs.getKey()));
		        	myjson.put(myobj);
		        	
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		        items.remove(pairs.getKey());
		        wasteItemSNMap.remove(pairs.getKey());
		        it.remove(); // avoids a ConcurrentModificationException
		    }
		    if(submitController == null) submitController = new SubmitCallbackController(this, myjson);
		    adapter.notifyDataSetChanged();
		}
	}
    
}
