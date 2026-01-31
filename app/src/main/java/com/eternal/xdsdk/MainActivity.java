package com.eternal.xdsdk;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

/**
 * Main Activity - Primary interface for the application
 * Loaded dynamically through the ProxyApplication/ProxyComponentFactory system
 */
public class MainActivity extends Activity {
    
    private boolean serviceRunning = false;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Layout loaded from native resources
        // setContentView(R.layout.activity_main);
        
        initializeUI();
        checkPermissions();
    }
    
    private void initializeUI() {
        // UI initialized from native code
        // Button click listeners, etc.
    }
    
    private void checkPermissions() {
        // Check for required permissions
        // SYSTEM_ALERT_WINDOW, MANAGE_EXTERNAL_STORAGE, etc.
    }
    
    public void startFloaterService(View view) {
        if (!serviceRunning) {
            Intent serviceIntent = new Intent(this, FloaterService.class);
            startService(serviceIntent);
            serviceRunning = true;
        }
    }
    
    public void stopFloaterService(View view) {
        if (serviceRunning) {
            Intent serviceIntent = new Intent(this, FloaterService.class);
            stopService(serviceIntent);
            serviceRunning = false;
        }
    }
    
    @Override
    protected void onResume() {
        super.onResume();
    }
    
    @Override
    protected void onPause() {
        super.onPause();
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (serviceRunning) {
            stopService(new Intent(this, FloaterService.class));
        }
    }
}
