package com.yourcompany.fairyland;

import android.util.Log;

public class Console
{
    public void log(String msg)
    {
        msg = msg != null ? msg : "";
        Log.i("Web Console <2.1", msg);
    }
}
