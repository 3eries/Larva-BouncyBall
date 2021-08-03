package com.superbomb.plugins;

import android.app.Activity;
import android.content.Intent;

import org.cocos2dx.lib.Cocos2dxActivity;

public interface PluginListener {

    public void init(Cocos2dxActivity context);
    public void onStart();
    public void onStop();
    public void onResume();
    public void onPause();
    public void onDestroy();
    public boolean onBackPressed();
    public boolean onActivityResult(int requestCode, int resultCode, Intent data);
}
