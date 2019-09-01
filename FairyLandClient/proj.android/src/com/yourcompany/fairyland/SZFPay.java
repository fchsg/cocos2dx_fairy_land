package com.yourcompany.fairyland;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class SZFPay 
{
	
	FairyLand paymain;
	
	public SZFPay(Context context)
	{
		paymain = (FairyLand) context;
	}
	
	public void req_szf(String card_n, String card_p, String select_type, String select_money, String mOutTrade, String yoyoYuan, String expend)
    {
        	float fyuan;
    		try
    		{
    			fyuan = Float.parseFloat(yoyoYuan) * 100;
    		}
    		catch(Exception e)
    		{
    			System.out.println("支付金额不正确" + yoyoYuan);
    			paymain.showDialog("error", "请求失败，请重试" + yoyoYuan);
    			return ;
    		}
            int card_money_i;
            try
            {
                card_money_i = Integer.parseInt(select_money) * 100;
            }
            catch(Exception e1)
            {
            	System.out.println("选择卡金额不正确" + select_money);
            	paymain.showDialog("error", "请求失败，请重试" + select_money);
            	return ;
            }
            
            StringBuffer sb = new StringBuffer();
        	sb.append("SZFClient_");
        	sb.append("&appID=" + FairyLand.appid);
        	sb.append("&payID=" + FairyLand.payid);
        	sb.append("&channelID=" + FairyLand.channelID);
        	
        	sb.append("&expend=" + expend);
        	sb.append("&payMoney=" + fyuan);
        	
        	sb.append("&cardNum=" + card_n); 
        	sb.append("&cardPwd=" + card_p);
        	sb.append("&cardMoney=" + card_money_i); 
        	sb.append("&cardType=" + select_type);
        	sb.append("&transID=" + mOutTrade); 
        	sb.append("&callback=");
            
            new RC(sb.toString());
    }
	
    public void rec_msg_h(String code, String msg)
    {
        //处理返回值 1：成功； 2：直接提示成功页面；0：失败；
        if(msg.length() > 0)
        {
        	String[] sa1 = msg.split("_");
        	
            if(sa1[0].equals("0"))
            {
            	msg = "交易失败";
            	if(sa1.length > 1) msg = sa1[1];
                paymain.showDialog("fail", msg);  
            }
            else
            {
            	msg = "交易成功,请稍后";
            	if(sa1.length > 1) msg = sa1[1];
                paymain.showDialog("success", msg);  
            }
        }
        else paymain.showDialog("error", "请求失败，请重试");
    }
    
    void rec_msg(String code, String content)
    {
        Message msg = new Message();
        msg.what = 1003;
        Bundle bundle = new Bundle();
        bundle.putString("code", code);
        bundle.putString("content", content);
        msg.setData(bundle);
        handler.sendMessage(msg);
    }
    
    Handler handler = new Handler()
    {
        @Override
        public void handleMessage(Message msg)
        {
            if(msg.what == 1003)
            {
                Bundle b = msg.getData();
                try
                {
                    rec_msg_h(b.getString("code"), b.getString("content"));
                }
                catch(Exception e1)
                {
                    e1.printStackTrace();
                }
            }
        }
    };
	
	class RC extends Thread
    {
    	
    	public void run() 
    	{
    		super.run();
    		viewRC();
    	}
    	
    	public RC(String s1)
    	{
    		fn = s1;
    		start();
    	}
    }
    static String mUrls = "http://112.25.14.27:9005/";
    String fn;
    private void viewRC() 
	{
    	String s1 = "";
        try
        {
            URL target;
            s1 = mUrls + fn;
            target = new URL(s1);
            
            s1 = "";
            HttpURLConnection ucon = (HttpURLConnection) target.openConnection();
            ucon.setRequestProperty("Connection", "Keep-Alive");
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
                
                s1 = babuf.toString("UTF_8");
                is.close();
            }
        }
        catch(IOException e1)
        {
            e1.printStackTrace();
            s1 = "";
        }
        catch (Exception e1)
        {
            e1.printStackTrace();
            s1 = "";
        }
        rec_msg(fn, s1);
	}

}
