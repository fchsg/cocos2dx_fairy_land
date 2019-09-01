package com.yourcompany.fairyland;

import java.lang.reflect.Method;

public class BridgeReflect
{
    public static void invokeMethod(Class<?> cls, Object obj, String methodName, Class<?>[] paramTypeCls, Object[] paramObj)
    {
        try
        {
        Method method = cls.getDeclaredMethod(methodName, paramTypeCls);
        method.setAccessible(true);
        method.invoke(obj, paramObj);
        }
        catch (Exception e)
        {
    	    e.printStackTrace();
        }
    }
}