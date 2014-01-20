package com.geomobile.rc663;

import com.android.hflibs.Iso15693_native;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
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

public class Scan2 extends ScanActivity implements OnClickListener {
    /** Called when the activity is first created. */
	
	private static final String TAG = "rc663_15693_java";
	private static final String PW_DEV = "/proc/driver/as3992";
	private Iso15693_native dev = new Iso15693_native();
	private Button start_demo;
	private Button get_info;
	private Button submit;
	private TextView main_info;
	private TextView card_info;
	private TextView textView1;
	private EditText block_nr;
	private CheckBox lock_block;
	private CheckBox lock_afi;
	private CheckBox lock_dsfid;
	private DeviceControl power;
	private Spinner spinner2;
	private ListView listView;
	private EditText editText1;
	
	private int block_max = 0;
	private int block_size = 0;
	
	private String imei = "";
	private ArrayAdapter adapter;
	// private String[] myStringArray = {"gen1", "gen2"};
	private List<String> items = new ArrayList<String>();
	private IOCallback fetchController = null, submitController = null;
	private HashMap<String, String> wasteOptionsMap = new HashMap<String, String>();
	private HashMap<String, String> wasteItemTypeMap = new HashMap<String, String>();
	private HashMap<String, String> wasteItemSNMap = new HashMap<String, String>();
	
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.scan2);
        
        TelephonyManager telephonyManager = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);
        this.imei = telephonyManager.getDeviceId();
        
        //start_demo = (Button)findViewById(R.id.button_15693_demo);
        //start_demo.setOnClickListener(this);
        //start_demo.setEnabled(false);
        
        get_info = (Button)findViewById(R.id.button_15693_search);
        get_info.setOnClickListener(this);
        get_info.setEnabled(true);
        
        //submit = (Button)findViewById(R.id.button_15693_upload);
        //submit.setOnClickListener(this);
        //submit.setEnabled(true);
        
        //textView1 = (TextView)findViewById(R.id.textView1);
        //editText1 = (EditText)findViewById(R.id.editText1);
        main_info = (TextView)findViewById(R.id.textView_15693_info);
        main_info.setMovementMethod(ScrollingMovementMethod.getInstance());
        card_info = (TextView)findViewById(R.id.textView_15693_cardinfo);
        
        //block_nr = (EditText)findViewById(R.id.editText_15693_block);
        
        //lock_block = (CheckBox)findViewById(R.id.checkBox_15693_lockblock);
        //lock_afi = (CheckBox)findViewById(R.id.checkBox_15693_lockafi);
        //lock_dsfid = (CheckBox)findViewById(R.id.checkBox_15693_lockdsfid);
        
        //items.add(new String("aloha_1"));
        
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
    
    public void debugMessage(String msg)
    {
    	TextView main_info = (TextView)findViewById(R.id.textView_15693_info);
		main_info.setText(msg);
    }
    
    public class NullCallback implements IOCallback {
    	public void httpRequestDidFinish(int success, String value) {
    		
    	}
    }
    
    
    public class FetchItemCallbackController implements IOCallback {
    	Scan2 activity;
    	String sn;
    	ProgressDialog progDialog;
    	List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
    	public FetchItemCallbackController(Scan2 activity, String sn) {
    		this.sn = sn;
    		this.activity = activity;
    		//NameValuePair postContent = new BasicNameValuePair("sn", sn);
    		//nameValuePairs.add(postContent);
    		new LongRunningGetIO(getString(R.string.url_prefix) + "getRfidWasteName?imei=" + activity.imei + "&rfid=" + sn, nameValuePairs, this).execute();
    		
    		progDialog = ProgressDialog.show(activity, "正在获取信息",
    	            "请稍候...", true);
    	}
    	
    	public void parseJSON(String value) throws JSONException
    	{
    		JSONObject jObject = new JSONObject(value);
    		activity.popupEditText(sn, "原先数值: " + jObject.getString("total"), jObject);
    	}
    	
    	public void httpRequestDidFinish(int success, String value)
    	{
    		Log.d(TAG, value);
    		try {
				parseJSON(value);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				try {
					JSONObject jObject = new JSONObject(value);
					String errmsg = "出现错误\n";
					if(jObject.get("error") instanceof JSONArray) {
						JSONArray jArr = (JSONArray)jObject.get("error");
						for(int i = 0; i < jArr.length(); i++) {
							JSONObject jj = jArr.getJSONObject(i);
							errmsg += jj.getString("rfid") + ": " + jj.getString("des") + "\n";
						}
					} else {
						errmsg += ((JSONObject)(jObject.get("error"))).getString("rfid") + ": " + ((JSONObject)(jObject.get("error"))).getString("des") + "\n";
					}
					activity.alertMessage(errmsg);
				} catch (JSONException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
					if(value.contains("error")) {
						activity.alertMessage("出现错误" + value);
					} else {
						activity.alertMessage("未知错误" + value);
					}
				}
			}
    		progDialog.dismiss();
    		activity.fetchController = null;
    	}
    }
    
    public class SubmitCallbackController implements IOCallback {
    	Scan2 activity;
    	ProgressDialog progDialog;
    	List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
    	public SubmitCallbackController(Scan2 activity, JSONObject postJson) {
    		this.activity = activity;
    		NameValuePair postContent = new BasicNameValuePair("txt_json", postJson.toString());
    		nameValuePairs.add(postContent);
    		new LongRunningGetIO(getString(R.string.url_prefix) + "addWaste", nameValuePairs, this).execute();
    		
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
    		/*AlertDialog.Builder builder = new AlertDialog.Builder(activity);
	        builder.setTitle("NVPUpload")
	        .setMessage(value)
	        .setCancelable(false)
	        .setNegativeButton("确定",new DialogInterface.OnClickListener() {
	            public void onClick(DialogInterface dialog, int id) {
	                dialog.cancel();
	            }
	        });
	        AlertDialog alert = builder.create();
	        alert.show();*/
    		this.parseJSON(value);
	        
	        activity.submitController = null;
    	}
    }
    
    public void popupEditText(final String sn, String value, final JSONObject original)
    {
    	AlertDialog.Builder alert = new AlertDialog.Builder(this);

    	alert.setTitle("新增重量");
    	alert.setMessage("RFID: " + sn + "\n" + value);

    	// Set an EditText view to get user input 
    	final EditText input = new EditText(this);
    	input.setText("");
    	alert.setView(input);
    	
    	final Scan2 myself = this;

    	alert.setPositiveButton("确定", new DialogInterface.OnClickListener() {
    	public void onClick(DialogInterface dialog, int whichButton) {
    	    try {
    	    	Double.parseDouble(input.getText().toString());
    	    } catch(NumberFormatException nfe) {
    	    	myself.alertMessage("请输入数字");
    	    	return;
    	    }
    	    try {
    	    	Integer.parseInt(input.getText().toString());
    	    } catch(NumberFormatException nfe) {
    	    	try {
					if(Integer.parseInt(original.getString("way")) == 1) {
						myself.alertMessage("请输入整数");
						return;
					}
				} catch (NumberFormatException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
    	    }
    	  // Do something with value!
    		JSONObject toUpload = new JSONObject();
    		
    		try {
    			toUpload.put("rfid", sn);
				toUpload.put("wasteid", original.getString("id"));
				toUpload.put("imei", myself.imei);
				toUpload.put("addway", original.getString("way"));
				toUpload.put("addnum", input.getText());
				
				myself.readyToSubmit(toUpload);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    		
    		
    	  }
    	});

    	alert.setNegativeButton("取消", new DialogInterface.OnClickListener() {
    	  public void onClick(DialogInterface dialog, int whichButton) {
    	    // Canceled.
    	  }
    	});

    	alert.show();
    }
    
    public void readyToSubmit(JSONObject content)
    {
    	this.submitController = new SubmitCallbackController(this, content);
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
			
			
			if(fetchController == null)
				fetchController = new FetchItemCallbackController(this, sn);
			
		}
	}
    
}
