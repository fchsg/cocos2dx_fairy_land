package com.yourcompany.fairyland;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Paint;
import android.net.http.SslError;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.webkit.SslErrorHandler;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class AlipayMain extends Activity {
	
	WebView webview;
	
	RelativeLayout fl;
	
	TextView tv;
	TextView btn;
	TextView tvInfo;
	TextView tvback;
	
	Timer timer;
	String reqInfo, getInfo;
	int result;
	
	final static String mUrls = "http://112.25.14.27:9005/";
	
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		reqInfo = intent.getStringExtra("mInfo");
		
		index = 0;
		loading = true;
		isSend = 0;
		result = 0;
		initView();
		startTimer();
		
		IntentFilter filter = new IntentFilter();
		filter.addAction("com.yoyo.pay.alipay.loading");
		registerReceiver(receiver, filter);
	}
	
	TimerReceiver receiver = new TimerReceiver();
	
	int index;
	String[] point = new String[]{" .", " ..", " ..."};
	String tipInfo = "请稍后";
	boolean loading;
	int isSend;
	long ticks;
	
	class TimerReceiver extends BroadcastReceiver
	{

		@Override
		public void onReceive(Context context, Intent intent) {
			if(intent.getAction().equals("com.yoyo.pay.alipay.loading"))
			{
				if(loading)
				{
					index ++;
					if(index > 2) index = 0;
					if(tv != null)
					{
						tv.setText(tipInfo + point[index]);
						tv.invalidate();
					}
					if(tvback != null)
					{
						tvback.setText(tipInfo + point[index]);
						tvback.invalidate();
					}
				}
			}
		}
		
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		
		unregisterReceiver(receiver);
		cancelTimer();
	}
	
	boolean isKeyDown = false;
	
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    { 
//        Logger.i("alipay on key down");
        isKeyDown = true;
        return true;
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
//        Logger.i("alipay on key up");
        if(isKeyDown)
        {
        	isKeyDown = false;
        	if(keyCode == KeyEvent.KEYCODE_BACK)
        	{
	            setResult(result);
	            finish();
        	}
        }
        return true;
    }

	
	public void cancelTimer()
	{
		if(timer != null)
		{
			timer.cancel();
			timer = null;
		}
	}
	
	void stopLoading(String info)
	{
		if(tv != null)
		{
			tv.setText(info);
		}
		if(tvback != null)
		{
			tvback.setText(info);
		}
		loading = false;
	}

	void startTimer()
	{
		TimerTask tt = new TimerTask() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				
				ticks++;
				if(loading && ticks % 5 == 0)
				{
					Intent intent = new Intent();
					intent.setAction("com.yoyo.pay.alipay.loading");
					sendBroadcast(intent);
				}
				
				if(isSend == 0 || (isSend > 0 && isSend < 3))
				{
					String url;
					url = mUrls + reqInfo;
//					url = "http://119.161.223.42:9005/";
//					url = "http://112.25.14.27:9005/" + reqInfo;
//					url = "http://192.168.0.197:9005/" + reqInfo;

					String str = send(url); 

					if(str.startsWith("err-"))
					{
						isSend++;
					}
					else if(str.length() > 0)
					{
						Map<String, String> map = parseRC(str);
						if(map != null && map.containsKey("alipay_trade"))
						{
							getInfo = map.get("alipay_trade");
							Message msg = new Message();
							msg.what = 20001;
							mh.sendMessage(msg);
							isSend = -1;
						}
						else isSend = -2;
					}
					
					if(isSend >= 3) isSend = -2;
					if(isSend == -2)
					{
						Message msg = new Message();
						msg.what = 20002;
						mh.sendMessage(msg);
					}
				}
				
			}
		};
		
		timer = new Timer();
		
		timer.schedule(tt, 50, 50);
	}
	
	Handler mh = new Handler()
	{
		@Override
		public void handleMessage(Message msg) 
		{
			if(msg.what == 20001)
			{
				sendRedirect(getInfo);
			}
			else if(msg.what == 20002)
			{
				stopLoading("请求失败，请返回重试");
			}
		}
		
	};
	
	private String send(String mUrl) 
	{
		String response = "";
        try
        {
            URL target;
            target = new URL(mUrl);

            HttpURLConnection ucon = (HttpURLConnection) target.openConnection();
            ucon.setRequestProperty("Connection", "Keep-Alive;charset=utf-8");
            ucon.setConnectTimeout(30 * 1000);
            ucon.setReadTimeout(30 * 1000);

            int rc = ucon.getResponseCode();
            
            if (rc == 200)
            {
                InputStream is = ucon.getInputStream();
                ByteArrayOutputStream babuf = new ByteArrayOutputStream();
                byte[] buf = new byte[5120];
                int reading;
                while ((reading = is.read(buf)) != -1)
                {
                    babuf.write(buf, 0, reading);
                }
                response = babuf.toString("UTF_8");
                is.close();
            }
        }
        catch(IOException e1)
        {
            e1.printStackTrace();
            response = "err-" + e1.toString();
        }
        catch (Exception e1)
        {
            e1.printStackTrace();
            response = "err-" + e1.toString();
        }
        return response;
	}
	
	public static Map<String, String> parseRC(String s1)
    {
        if(s1 == null) return null;
        
        Map<String, String> mNote = new HashMap<String, String>();
        String s2;
        int i1, i2, i3, i4;
        
        i1 = s1.indexOf("<?xml");
        i2 = s1.indexOf("?>");
        if(i1 != -1 && i2 != -1 && i1 < i2)
        {
        	s1 = s1.substring(i2 + 2);
        }
        
        i1 = s1.indexOf("<");
        i2 = s1.indexOf(">");
        while(s1.length() > 0 && i1 != -1 && i2 != -1 && i1 < i2)
        {
            String tt, tt1, info;
            tt = s1.substring(i1 + 1, i2).trim();
            if(tt.indexOf(" ") != -1)
            {
            	tt = tt.substring(0, tt.indexOf(" "));
            }
            s1 = s1.substring(i2 + 1);
            i3 = s1.indexOf("</" + tt);
            if(i3 == -1) break;
            s2 = s1.substring(i3);
            i4 = s2.indexOf(">") + i3;
            if(i3 != -1 && i3 < i4)
            {
                tt1 = s1.substring(i3 + 2, i4).trim();
                if(tt1.equals(tt))
                {
                    info = s1.substring(0, i3).trim();
                    mNote.put(tt, info);
                }
                else break;
            }else break;
            
            s1 = s1.substring(i4 + 1);
            i1 = s1.indexOf("<");
            i2 = s1.indexOf(">");
        }
        return mNote;
    }

	public  void sendRedirect(String url)
	{
		webview = new WebView(this);
		
		webview.setWebChromeClient(new WebChromeClient()
		{
			public void onProgressChanged(WebView view, int progress) 
			{
				String info = " 页面加载中..." + progress + "%";
//				Logger.e("WebChromeClient. === onProgressChanged " + "页面加载中..." + progress + "%"); 
		        if (progress == 100) 
		        {  
		        	info = "";
		        }
		        tvInfo.setText(info);
			}  
		});
		
		webview.setWebViewClient(new WebViewClient()
		{
			@Override
			public void onPageFinished(WebView view, String url) 
			{
//				Logger.e("view. === onPageFinished##" + url);
				stopLoading("");
				view.loadUrl("javascript:window.local_obj.showSource('<head>'+" +
		                "document.getElementsByTagName('html')[0].innerHTML+'</head>');");
				super.onPageFinished(view, url);
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) 
			{
				loading = true;
//				Logger.e("view. === onPageStarted");
				super.onPageStarted(view, url, favicon);
			}

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) 
			{
//				Logger.e("view. === shouldOverrideUrlLoading##" + url);
				if(backTyep(url) == 0) view.loadUrl(url);
				return true;
			}

			@Override
			public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
				// TODO Auto-generated method stub
				handler.proceed();
			}
			
		});
		
      webview.setOnTouchListener(new OnTouchListener(){
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				if(event.getAction()==MotionEvent.ACTION_UP){
					upMouseY=(int) event.getY();
					moveY=downMouseY-upMouseY;
					myhandler.sendEmptyMessageDelayed(0, 200);
				}else if(event.getAction()==MotionEvent.ACTION_DOWN){
					preScrollY=webview.getScrollY();
					startScrollY=preScrollY;
					downMouseY=(int) event.getY();
					
				}
				return false;
			}
      });
		
		WebView.enablePlatformNotifications();
		
		configWebview(webview);
		
		webview.addJavascriptInterface(new InJavaScriptLocalObj(), "local_obj");
		
		webview.loadUrl(url);
		
		RelativeLayout.LayoutParams lp_view, lp_txt;
		
		lp_view = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.FILL_PARENT, RelativeLayout.LayoutParams.FILL_PARENT);
		lp_view.addRule(RelativeLayout.CENTER_IN_PARENT);
//		lp_view.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
//		lp_view.addRule(RelativeLayout.BELOW, 1);
		
		fl.addView(webview, lp_view);
		
		tv = new TextView(this);
		tv.setGravity(Gravity.CENTER);
		tv.setTextColor(0xff4f638d);
		tv.setTextSize(30);
		tv.setId(2);
		tv.setText(tipInfo + point[2]);
		
		lp_txt = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		lp_txt.addRule(RelativeLayout.CENTER_IN_PARENT);
		
		fl.addView(tv, lp_txt);
		
		fl.removeView(btn);
		fl.removeView(tvInfo);
		fl.removeView(tvback);
	}
	
	/**
	 * 保存上次检测时ScrollY位置
	 */
	public int preScrollY=0;
	/**
	 * 开始拖动时的ScrollY
	 */
	public int startScrollY=0;

	public int downMouseY,upMouseY;
	/**
	 * 拖拽的距离
	 */
	public int moveY;
	
	Handler myhandler=new Handler(){
		@Override
		public void handleMessage(Message msg) {
			int y=webview.getScrollY();
			if(y==preScrollY){
				//触摸并拖动20像素以上
				if(moveY>20){
					//说明没有拖动
					if(preScrollY==startScrollY){
						//这就到了最后了。。。可以这里面调用网页的JS方法，实现数据翻页或者动态加载数据效果
//						Log.i("tyh", "bottom:y="+y+",Y="+webview.getScrollY());
					}
				}
			}else{
				//说明还在滚动中，200毫秒后再来判断
				myhandler.sendEmptyMessageDelayed(0, 200);
//				Log.i("tyh", "scorll:y="+y+",Y="+webview.getScrollY());
			}
			preScrollY=y;
		}
	};
	
	final class InJavaScriptLocalObj 
	{
	    public void showSource(String html) 
	    {
	    	if((html.indexOf("付款成功") != -1) || html.indexOf("支付成功") != -1)
	    	{
	    		result = 1;
	    	}
	    	else result = 0;
	    	
	    	if((html.indexOf("交易失败") != -1) || html.indexOf("付款失败") != -1)
	    	{
	    		result = 0;
	    	}
	    }
	}
	
	public int backTyep(String html)
	{
	    int i1;
	    i1 = 0;
	    String s1;
	    System.out.println("html#" + html);
	    s1 = "http://119.161.223.42:10004/wapPayDemoRSA/servlet/success.jsp";
	    if((html.startsWith(s1)) || html.startsWith("http://yoyo-fairland2/success.jsp"))
        {
            result = 1;
            i1 = 1;
            
            setResult(result);
            finish();
        }
        else result = 0;
        
	    s1 = "http://119.161.223.42:10004/wapPayDemoRSA/servlet/fail.jsp";
        if((html.startsWith(s1)) || html.startsWith("http://yoyo-fairland2/fail.jsp"))
        {
            result = 0;
            i1 = 1;
            
            setResult(result);
            finish();
        }
        
        return i1;
        
	}
	
	
	private void initView()
	{
		btn = new TextView(this);
		btn.setText("返回");
		btn.setTextSize(btn.getTextSize() + 4);
		btn.setId(1);
		btn.setTextColor(0xfff1f8f7);
		btn.getPaint().setAntiAlias(true);
		btn.getPaint().setFlags(Paint.UNDERLINE_TEXT_FLAG);
		btn.setOnClickListener(new OnClickListener() {
			
			public void onClick(View arg0) {
				setResult(result);
				finish();
			}
		});
		
		btn.setOnTouchListener(new OnTouchListener() 
		{
			public boolean onTouch(View v, MotionEvent event) 
			{
				if(event.getAction() == MotionEvent.ACTION_DOWN)
				{
					btn.setTextColor(0xffdfe146);
				}
				else if(event.getAction() == MotionEvent.ACTION_UP)
				{
					btn.setTextColor(0xff0f7fcc);
				}
				return false;
			}
		});
		
		tvInfo = new TextView(this);
		tvInfo.setText("");
		tvInfo.setId(3);
		tvInfo.setTextColor(0xfff1f8f7);
		
		tvback = new TextView(this);
		tvback.setGravity(Gravity.CENTER);
		tvback.setTextColor(0xff4f638d);
		tvback.setTextSize(30);
		tvback.setId(2);
		tvback.setText(tipInfo + point[2]);
		tvback.setBackgroundColor(Color.WHITE);
		
		fl = new RelativeLayout(this);
		
		fl.setBackgroundColor(0xff77aadf); //C2D7E5 // 5C94CB
		
		RelativeLayout.LayoutParams lp_btn, lp_txt, lp_info;
		
		lp_btn = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		lp_btn.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		lp_btn.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
		lp_btn.rightMargin = 20;
		lp_btn.topMargin = 5;
		
		fl.addView(btn, lp_btn);
		
		lp_info = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		lp_info.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		lp_info.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
		lp_info.topMargin = 10;
		fl.addView(tvInfo, lp_info);
		
		lp_txt = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.FILL_PARENT, RelativeLayout.LayoutParams.FILL_PARENT);
		lp_txt.addRule(RelativeLayout.CENTER_IN_PARENT);
		lp_txt.addRule(RelativeLayout.BELOW, 1);
		
		
		fl.addView(tvback, lp_txt);
		
		setContentView(fl);
	}
	
	private void configWebview(WebView webview) 
	{
		getWindow().getDecorView().setDrawingCacheEnabled(true);
		webview.setInitialScale(0);
		webview.setVerticalScrollBarEnabled(false);
		webview.setHorizontalScrollBarEnabled(false);
		webview.requestFocusFromTouch();
		webview.requestFocus();
		webview.setFocusable(true);
		webview.setOnTouchListener(new View.OnTouchListener()
	    {
			public boolean onTouch(View arg0, MotionEvent arg1) {
				
				System.out.println("webView on touch");
				switch (arg1.getAction()) 
				{
			        case 0:
			        case 1:
			          if (arg0.hasFocus()) break;
			          arg0.requestFocus();
		        }
				return false;
			}
	    });
	    if (Integer.valueOf(Build.VERSION.SDK).intValue() <= 7) {
	    	webview.addJavascriptInterface(new Console(), "console");
	    }

	    WebSettings webSettings = webview.getSettings();
	    webSettings.setDefaultTextEncodingName("utf-8");
	    webSettings.setJavaScriptEnabled(true);
	    webSettings.setJavaScriptCanOpenWindowsAutomatically(true);
	    webSettings.setNeedInitialFocus(true);
	    webSettings.setSaveFormData(false);
	    webSettings.setSavePassword(false);
	    webSettings.setPluginsEnabled(true);
	    webSettings.setBuiltInZoomControls(false);
	    webSettings.setSupportZoom(true);
	    webSettings.setCacheMode(2);
	    webSettings.setLayoutAlgorithm(WebSettings.LayoutAlgorithm.NORMAL);

	    if (Build.VERSION.SDK_INT < 11) {
	        webSettings.setNavDump(true);
	    }
	    Class<?> webSettingCls = webSettings.getClass();

	    BridgeReflect.invokeMethod(webSettingCls, webSettings, "setPageCacheCapacity", new Class[] { Integer.TYPE }, new Object[] { Integer.valueOf(20) });

	    BridgeReflect.invokeMethod(webSettingCls, webSettings, "setDomStorageEnabled", new Class[] { Boolean.TYPE }, new Object[] { Boolean.valueOf(true) });

	    BridgeReflect.invokeMethod(webSettingCls, webSettings, "setDatabaseEnabled", new Class[] { Boolean.TYPE }, new Object[] { Boolean.valueOf(true) });

	    String dbPath = getApplicationContext().getDir("database", 0).getPath();
	    BridgeReflect.invokeMethod(webSettingCls, webSettings, "setDatabasePath", new Class[] { String.class }, new Object[] { dbPath });
	  }
}
