package com.remote.yingwen.carsochet;

import android.os.Bundle;
import android.os.IBinder;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {
	private static final String ACTION = "com.remote.yingwen.carsochet.action.NEW_FILE";
	private static final String ACTION_FINISH = "com.remote.yingwen.carsochet.action.UPLOAD_FINISH";
	private Button but_center;
	private Button but_left;
	private Button but_right;
	private Button but_up;
	private Button but_below;
	private Button but_r_speenUp;
	private Button but_l_speenUp;
	private Button but_r_slowDown;
	private Button but_l_slowDown;
	private	TextView title;
	private static int js=0;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
 
		initView();

		IntentFilter filter = new IntentFilter(ACTION_FINISH);
		registerReceiver(this.UploadList, filter);
		
		Intent serviceIntent = new Intent();
		serviceIntent.setAction("com.remote.yingwen.carsochet.service");
		bindService(serviceIntent, new ServiceConnection() {
			
			@Override
			public void onServiceDisconnected(ComponentName name) {
				
			}
			
			@Override
			public void onServiceConnected(ComponentName name, IBinder service) {
				
			}
		}, Context.BIND_AUTO_CREATE);
//		startService(serviceIntent);
//		send("WakeUP");
	}
	 public static String repeat(String src, int num) {
	        StringBuffer s = new StringBuffer();
	        for (int i = 0; i < num; i++)
	            s.append(src);
	        return s.toString();
	    }
	 

	private final BroadcastReceiver UploadList = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			    String str=intent.getStringExtra("RESULT");
			    
	            if(str.equals("1")){
	            	title.setText("Connect success");
	          //  	Toast.makeText(MainActivity.this, str, 0).show();
	            }
	            if(str.equals("end")){
	            	str="Connect fail，please exit ！";
	            	Toast.makeText(MainActivity.this, str, 0).show();
	            }
	            
	            if(str.equals("-1")){
	            	title.setText("Connect fail");
	            }
	            if(str.equals("r")){
	            	title.setText("Connect fail, press any key to reconnect");
	            }
	            if(str.equals("-2")){
	            	title.setText("Connect fail");
	            }
	            if(str.equals("3")){
	            	js++;
	            	if(js==10){
	            		js=1;
	            	}
	            	String s=repeat(".",js);
	            	title.setText("Connecting"+s);
	            }
		}
	};

	public void initView() {
		but_center = (Button) findViewById(R.id.but_center);
		but_left = (Button) findViewById(R.id.but_left);
		but_right = (Button) findViewById(R.id.but_right);
		but_up = (Button) findViewById(R.id.but_up);
		but_below = (Button) findViewById(R.id.but_below);
		but_l_slowDown = (Button) findViewById(R.id.but_l_slowDown);
		but_r_slowDown = (Button) findViewById(R.id.but_r_slowDown);
		but_l_speenUp = (Button) findViewById(R.id.but_l_speenUp);
		but_r_speenUp = (Button) findViewById(R.id.but_r_speenUp);
		title= (TextView) findViewById(R.id.title);

		but_center.setOnClickListener(this);
		but_left.setOnClickListener(this);
		but_right.setOnClickListener(this);
		but_up.setOnClickListener(this);
		but_below.setOnClickListener(this);
		but_l_slowDown.setOnClickListener(this);
		but_r_slowDown.setOnClickListener(this);
		but_l_speenUp.setOnClickListener(this);
		but_r_speenUp.setOnClickListener(this);
		
	}


	@Override
	public void onDestroy() {
		super.onDestroy();
		unregisterReceiver(this.UploadList); 
		
		Intent serviceIntent = new Intent();  
        serviceIntent.setAction("com.remote.yingwen.carsochet"); 
        stopService(serviceIntent); 
	}
	
	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.but_center:
			send("0");
			break;
		case R.id.but_left:
			send("3");
			break;
		case R.id.but_right:
			send("4");
			break;
		case R.id.but_up:
			send("1");
			break;
		case R.id.but_below:
			send("2");
			break;
		case R.id.but_l_speenUp:
			send("6");
			break;
		case R.id.but_r_speenUp:
			send("8");
			break;
		case R.id.but_l_slowDown:
			send("7");
			break;
		case R.id.but_r_slowDown:
			send("9");
			break;

		default:
			break;
		}
	}

	private void send(String str) {
		Intent intentAddFile = new Intent(ACTION);
		intentAddFile.putExtra("TYPE", str);
		sendBroadcast(intentAddFile);
	}

	
	 
	  public boolean onKeyDown(int keyCode, KeyEvent event) {
	        if(keyCode==KeyEvent.KEYCODE_BACK ) {
	      		new AlertDialog.Builder(this)
	      	    .setIcon(R.drawable.ic_launcher)
	      	    .setTitle("Exit") 
	      	    .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
	      	        public void onClick(DialogInterface dialog, int which) {
	      	        	 

	      	        }
	      	    })
	      	    .setPositiveButton("OK", new DialogInterface.OnClickListener() {
	      	        public void onClick(DialogInterface dialog, int whichButton) {
	      	    		 
	      	        	finish();
	      	        }
	      	    }).show();
	      		
				return true;
			}  
	 	return super.onKeyDown(keyCode, event); 
	}

	  
	  
}
