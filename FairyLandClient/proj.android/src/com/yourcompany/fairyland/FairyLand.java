/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.yourcompany.fairyland;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.telephony.SmsManager;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class FairyLand extends Cocos2dxActivity{
	static FairyLand instance;
	
	PendingIntent sendPI;
	PendingIntent deliverPI;
	final static String SEND_SMS_ACTION = "SEND_SMS";
	final static String DELIVERED_SMS_ACTION = "DELIVERED_SMS";
	
    protected void onCreate(Bundle savedInstanceState){
    	instance = this;
		super.onCreate(savedInstanceState);	

		// create the sentIntent parameter
		Intent sendIntent = new Intent(SEND_SMS_ACTION);
		sendPI = PendingIntent.getBroadcast(this, 0, sendIntent, 0);
		// create the deilverIntent parameter
		Intent deliverIntent = new Intent(DELIVERED_SMS_ACTION);
		deliverPI = PendingIntent.getBroadcast(this, 0, deliverIntent, 0);
		// register the Broadcast Receivers
		registerReceiver(new BroadcastReceiver() {
			@Override
			public void onReceive(Context _context, Intent _intent) {
				if (getResultCode() == Activity.RESULT_OK) {
					Log.d("SMS receiver","sucess");
					nativeSMSSuccessful();
				} else {
					Log.d("SMS receiver","failed");
					nativeSMSFailed();
				}
			}
		}, new IntentFilter(SEND_SMS_ACTION));
		registerReceiver(new BroadcastReceiver() {
			@Override
			public void onReceive(Context _context, Intent _intent) {
				Log.d("SMS receiver","deliveronReceive");
			}
		}, new IntentFilter(DELIVERED_SMS_ACTION));
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// Fairland should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("fairland");
    }
    
    final static String appid = "C001";
    final static String payid = "10000001";
    final static String channelID = "10001";
    
    
    public static void req_zfb(String yuan, String outTrade, String subject, String expend)
    {
    	int fen;
    	fen = Integer.parseInt(yuan) * 100;
    	
    	StringBuffer sb = new StringBuffer();
    	sb.append("ZFBW_");
    	sb.append("&appID=" + appid);
    	sb.append("&payID=" + payid);
    	sb.append("&channelID=" + channelID);
    	sb.append("&payMoney=" + fen);
    	sb.append("&outTrade=" + outTrade);
    	sb.append("&callback=" + "");
    	sb.append("&expend=" + expend);
    	sb.append("&subject=" + subject);
    	Intent intent = new Intent();
    	intent.putExtra("mInfo", sb.toString());
    	intent.setClass(instance, AlipayMain.class);
    	instance.startActivityForResult(intent, 20);
    }
    
    @Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) 
    {
		super.onActivityResult(requestCode, resultCode, data);
//		Logger.i("requestCode = " + requestCode + ", resultCode = " + resultCode);
		if(requestCode == 20)
		{
			if(resultCode == 1)
			{
				// 通知游戏成功
				showDialog("success", "交易成功,请稍后");
			}
			else 
			{
				// 通知游戏交易取消
				showDialog("cancel", "交易取消");
			}
		}
	}
    
    public static void req_szf(String card_n, String card_p, String select_type, String select_money, String mOutTrade, String yoyoYuan, String expend)
    {// 卡号，密码，卡类型，选择金额，订单号，扩展字串
    	Message msg = new Message();
        msg.what = 1103;
        Bundle bundle = new Bundle();
        bundle.putString("card_n", card_n);
        bundle.putString("card_p", card_p);
        bundle.putString("select_type", select_type);
        bundle.putString("select_money", select_money);
        bundle.putString("mOutTrade", mOutTrade);
        bundle.putString("yoyoYuan", yoyoYuan);
        bundle.putString("expend", expend);
        msg.setData(bundle);
        handler.sendMessage(msg);
    }
    
    public void showDialog(String title, String strText) 
    {
        AlertDialog.Builder tDialog = new AlertDialog.Builder(this);
        tDialog.setTitle("提示");
        tDialog.setMessage(strText);
        tDialog.setPositiveButton("确定", null);
        tDialog.show();
    }
    static Handler handler = new Handler(new Handler.Callback() 
    {
		@Override
		public boolean handleMessage(Message msg) 
		{
			if(msg.what == 1103)
            {
                Bundle b = msg.getData();
                try
                {
                    SZFPay szf = new SZFPay(instance);
                    szf.req_szf(b.getString("card_n"), b.getString("card_p"), b.getString("select_type"), b.getString("select_money"), b.getString("mOutTrade"), b.getString("yoyoYuan"), b.getString("expend"));
                }
                catch(Exception e1)
                {
                    e1.printStackTrace();
                }
            }
			return false;
		}
	});
    public native void returnPayInfo(int value);
    public static String getPhoneInfo()
    {
      	Activity mContext = instance;
		TelephonyManager tm = (TelephonyManager) mContext.getSystemService(Context.TELEPHONY_SERVICE);
		String imsi =  tm.getSubscriberId();   //imsi
		String NativePhoneNumber=tm.getLine1Number(); //号码
		String model = android.os.Build.MODEL; //机型
		String ProvidersName = null;           //运营商
		if(imsi != null && !imsi.equals("")){
			if (imsi.startsWith("46000") || imsi.startsWith("46002") || imsi.startsWith("46007") || imsi.startsWith("46020")) {
				ProvidersName = "ChinaMobile";   //中国移动
			} else if (imsi.startsWith("46001") || imsi.startsWith("46006")) {
				ProvidersName = "ChinaUnicom";   //中国联通
			} else if (imsi.startsWith("46003") || imsi.startsWith("46005")) {
				ProvidersName = "ChinaTelecom";   //中国电信
			}
		}
	   if(imsi == null || imsi.equals("")) {
		   imsi = "0";
	   }
	   if(NativePhoneNumber == null || NativePhoneNumber.equals("")) {
		   NativePhoneNumber = "0";
	   }
	   if(model == null || model.equals("")) {
		   model = "0";
	   }
	   if(ProvidersName == null || ProvidersName.equals("")) {
		   ProvidersName = "0";
	   }
	      
	   StringBuffer sb = new StringBuffer();
	   sb.append(model+"_");
	   sb.append(NativePhoneNumber+"_");
	   sb.append(ProvidersName+"_");
	   sb.append(imsi);
	   String strRet = sb.toString();
	   //Log.d("%s",strRet);   
	   return strRet;
    }
    
    // 移动短信支付
	public static void sendMessage(String content, String number) {
		SmsManager sms = SmsManager.getDefault();
		sms.sendTextMessage(number, null, content, instance.sendPI, instance.deliverPI);
	}
	public native void nativeSMSSuccessful();
	public native void nativeSMSFailed();
    
    public static void exitGame()
    {
		android.os.Process.killProcess(android.os.Process.myPid());
		System.exit(0);
    }
}
