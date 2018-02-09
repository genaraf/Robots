package com.remote.yingwen.carsochet;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

@SuppressLint("HandlerLeak")
public class UploadFileService extends Service {
	private static final String ACTION = "com.remote.yingwen.carsochet.action.NEW_FILE";
	private static final String ACTION_FINISH = "com.remote.yingwen.carsochet.action.UPLOAD_FINISH";
	private HandleThread thread;
	static public List<Map<String, Object>> data = new ArrayList<Map<String, Object>>();
	private String type = null;
	static Socket client;
	 public static final String IP = "192.168.1.1";
//	public static final String IP = "iot.cduino.com";
	public static final int PORT = 9003;

	public IBinder onBind(Intent intent) {
		return null;
	}

	public void onCreate() {
		super.onCreate();
		IntentFilter filter = new IntentFilter(ACTION);
		registerReceiver(this.UploadReceiver, filter);

		thread = new HandleThread();
		thread.start();
	}

	public boolean SocketClient(String site, int port) {
		boolean res;
		try {
			client = new Socket(site, port);
			res = true;

		} catch (IOException e) {

			res = false;

		}

		return res;
	}

	public String sendMsg(String msg) {
		try {
			BufferedReader in = new BufferedReader(new InputStreamReader(
					client.getInputStream()));
			OutputStream out = client.getOutputStream();
			out.write((msg + "\r\n").getBytes());
			out.flush();
			return in.readLine();

		} catch (IOException e) {
			e.printStackTrace();
		}
		return "";
	}

	public void closeSocket() {
		try {
			if (client != null)
				client.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		unregisterReceiver(this.UploadReceiver);
		thread.requestExit();
		try {
			if (client != null)
				client.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private final BroadcastReceiver UploadReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String path = intent.getStringExtra("TYPE");

			Map<String, Object> item = new HashMap<String, Object>();
			item.put("TYPE", path);

			synchronized (data) {
				data.add(item);
				data.notify();
			}
		}
	};

	private void initFileInfo(Map<String, Object> cache) {

		type = (String) cache.get("TYPE");
	}

	private void noticeUploadList(String str) {

		Intent intent1 = new Intent(ACTION_FINISH);
		intent1.putExtra("RESULT", str);
		sendBroadcast(intent1);

	}

	private class HandleThread extends Thread {

		private Map<String, Object> cache = null;
		private boolean bRun = true;

		public void requestExit() {
			bRun = false;
			synchronized (data) {
				data.notify();
			}
		}

		public void run() {
			int i = 0, s_flag = 0;
			while (i++ < 200) {
				if (SocketClient(IP, PORT)) {
					noticeUploadList("1");
					data.clear();
					s_flag = 1;
					break;
				}
				noticeUploadList("3");
				try {
					sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			if(i>=200){
				
				noticeUploadList("end");
				
				return;
			}
			while (bRun && s_flag == 1) {

				synchronized (data) {
					if (data.size() > 0) {
						cache = data.get(0);
					} else {
						try {
							data.wait();
						} catch (InterruptedException e) {
							Log.d("data", "data.wait");
						}
						continue;
					}
				}
				if (cache != null) {
					initFileInfo(cache);
					cache = null;
					String res = sendMsg(type);
					// 注意看这里返回时“ok”还是“ok\r\n"，可以单步调试看看先
					data.remove(0);
					if (res==null ||res.length() <= 0) {
						closeSocket();
					
						if (SocketClient(IP, PORT)) {
							noticeUploadList("1");
							data.clear();
						}else{
							
							noticeUploadList("r");
						}

					}
				}
			}

		}
	}
}


