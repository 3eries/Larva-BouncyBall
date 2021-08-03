package com.superbomb.plugins.firebase;

//import android.app.Activity;
//import android.content.Intent;
//import android.os.Bundle;
//import android.text.TextUtils;
//
//import com.google.firebase.analytics.FirebaseAnalytics;
//import com.superbomb.json.SBJSON;
//import com.superbomb.plugins.PluginListener;
//
//import org.cocos2dx.lib.Cocos2dxActivity;
//
//import java.util.Hashtable;
//import java.util.Iterator;
//import java.util.Map;
//
//public class Analytics implements PluginListener {
//    private static Analytics instance = null;
//
//    public static Analytics getInstance() {
//        if( instance == null ) {
//            instance = new Analytics();
//        }
//
//        return instance;
//    }
//
//    private Activity context;
//    private FirebaseAnalytics firebaseAnalytics;
//
//    private Analytics() {
//
//    }
//
//    public static Activity getContext() {
//        return getInstance().context;
//    }
//
//    public static FirebaseAnalytics getFirebaseAnalytics() {
//        return getInstance().firebaseAnalytics;
//    }
//
//    @Override
//    public void init(Cocos2dxActivity context) {
//
//        this.context = context;
//        firebaseAnalytics = FirebaseAnalytics.getInstance(context);
//    }
//
//    @Override
//    public void onStart() {
//    }
//
//    @Override
//    public void onStop() {
//    }
//
//    @Override
//    public void onResume() {
//    }
//
//    @Override
//    public void onPause() {
//    }
//
//    @Override
//    public void onDestroy() {
//    }
//
//    @Override
//    public boolean onBackPressed() {
//        return false;
//    }
//
//    @Override
//    public boolean onActivityResult(int requestCode, int resultCode, Intent data) {
//        return false;
//    }
//
//    public static void setCurrentScreen(final String screen, final String screenClass) {
//
//        getContext().runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                getFirebaseAnalytics().setCurrentScreen(getContext(), screen,
//                        !TextUtils.isEmpty(screenClass) ? screenClass : null);
//            }
//        });
//    }
//
//    public static void logEvent(String event, String jsonParam) {
//
//        Bundle bundle = new Bundle();
//
//        if( !TextUtils.isEmpty(jsonParam) ) {
//            Map<String, Object> params = (Map<String, Object>) SBJSON.parse(jsonParam);
//            Iterator<String> it = params.keySet().iterator();
//
//            while( it.hasNext() ) {
//                String key = it.next();
//                Object value = params.get(key);
//
//                if( value instanceof Integer ) bundle.putInt(key, (Integer) value);
//                else if( value instanceof Long ) bundle.putLong(key, (Long) value);
//                else if( value instanceof Float ) bundle.putFloat(key, (Float) value);
//                else if( value instanceof Double ) bundle.putDouble(key, (Double) value);
//                else if( value instanceof String ) bundle.putString(key, (String) value);
//            }
//        }
//
//        getFirebaseAnalytics().logEvent(event, bundle);
//    }
//}
