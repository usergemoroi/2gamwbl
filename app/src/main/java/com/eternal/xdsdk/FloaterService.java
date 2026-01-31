package com.eternal.xdsdk;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import androidx.annotation.Nullable;

/**
 * Floater Service - Manages the game overlay window
 * This service runs in the foreground and draws the overlay on top of games
 */
public class FloaterService extends Service implements SurfaceHolder.Callback {
    
    private static final String CHANNEL_ID = "GameBlasterChannel";
    private static final int NOTIFICATION_ID = 1;
    
    private WindowManager windowManager;
    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;
    private boolean surfaceReady = false;
    
    // Native methods - implemented in libclient.so
    public native boolean connect(String server);
    public native void disconnect();
    public native void initSurface(Surface surface, int width, int height);
    public native void removeSurface();
    public native void drawOn(byte[] data, int width, int height);
    public native void drawTick();
    public native String findGame(Context context);
    public native void setScreen(int width, int height);
    public native void switchOverlay(boolean enabled);
    
    static {
        System.loadLibrary("client");
    }
    
    @Override
    public void onCreate() {
        super.onCreate();
        
        createNotificationChannel();
        startForeground(NOTIFICATION_ID, createNotification());
        
        windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
        createOverlayWindow();
        
        // Connect to server
        connect("https://gameblaster-server.example.com");
        
        // Find target game
        String game = findGame(getApplicationContext());
        if (game != null && !game.isEmpty()) {
            android.util.Log.i("FloaterService", "Target game found: " + game);
        }
    }
    
    private void createOverlayWindow() {
        surfaceView = new SurfaceView(this);
        
        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.MATCH_PARENT,
            Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ?
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY :
                WindowManager.LayoutParams.TYPE_PHONE,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
            PixelFormat.TRANSLUCENT
        );
        
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
        
        windowManager.addView(surfaceView, params);
    }
    
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        surfaceReady = true;
        Surface surface = holder.getSurface();
        int width = surfaceView.getWidth();
        int height = surfaceView.getHeight();
        
        setScreen(width, height);
        initSurface(surface, width, height);
        
        // Start rendering loop
        startRenderLoop();
    }
    
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        setScreen(width, height);
    }
    
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        surfaceReady = false;
        removeSurface();
    }
    
    private void startRenderLoop() {
        new Thread(() -> {
            while (surfaceReady) {
                drawTick();
                try {
                    Thread.sleep(16); // ~60 FPS
                } catch (InterruptedException e) {
                    break;
                }
            }
        }).start();
    }
    
    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel channel = new NotificationChannel(
                CHANNEL_ID,
                "GameBlaster Overlay",
                NotificationManager.IMPORTANCE_LOW
            );
            channel.setDescription("Game overlay service");
            
            NotificationManager manager = getSystemService(NotificationManager.class);
            manager.createNotificationChannel(channel);
        }
    }
    
    private Notification createNotification() {
        Notification.Builder builder = Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ?
            new Notification.Builder(this, CHANNEL_ID) :
            new Notification.Builder(this);
        
        return builder
            .setContentTitle("GameBlaster Pro")
            .setContentText("Overlay service running")
            .setSmallIcon(android.R.drawable.ic_menu_view)
            .build();
    }
    
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }
    
    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
    
    @Override
    public void onDestroy() {
        super.onDestroy();
        
        surfaceReady = false;
        
        if (surfaceHolder != null) {
            surfaceHolder.removeCallback(this);
        }
        
        if (surfaceView != null && windowManager != null) {
            windowManager.removeView(surfaceView);
        }
        
        disconnect();
    }
}
