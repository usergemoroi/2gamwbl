package p00093e85d7aface4dfb;

import android.app.Application;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.text.TextUtils;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Objects;

/* loaded from: classes.dex */
public class ProxyApplication extends Application {
    public String a = "";
    public Application b = null;

    public final void a() {
        if (a.b && !TextUtils.isEmpty(this.a)) {
            Application application = (Application) JniBridge.ra(this.a);
            this.b = application;
            application.getClass();
            JniBridge.craa(getApplicationContext(), this.a);
            JniBridge.craoc(this.a);
            a.b = false;
        }
    }

    @Override // android.content.ContextWrapper
    public final void attachBaseContext(Context context) {
        super.attachBaseContext(context);
        Objects.toString(context.getClassLoader());
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        InputStream inputStream = null;
        try {
            try {
                inputStream = getAssets().open("app_name");
                byte[] bArr = new byte[1024];
                while (true) {
                    int read = inputStream.read(bArr);
                    if (read == -1) {
                        break;
                    } else {
                        byteArrayOutputStream.write(bArr, 0, read);
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            this.a = byteArrayOutputStream.toString();
            if (a.a) {
                return;
            }
            ApplicationInfo applicationInfo = context.getApplicationInfo();
            if (applicationInfo != null) {
                a.d(applicationInfo.sourceDir, applicationInfo.dataDir);
                JniBridge.a(applicationInfo.dataDir);
                JniBridge.ia();
                JniBridge.cbde(context.getClassLoader());
                a.a = true;
                return;
            }
            throw new NullPointerException("application info is null");
        } finally {
            a.a(byteArrayOutputStream);
            a.a(inputStream);
        }
    }

    @Override // android.content.ContextWrapper, android.content.Context
    public final Context createPackageContext(String str, int i) {
        if (!TextUtils.isEmpty(this.a)) {
            a();
            return this.b;
        }
        return super.createPackageContext(str, i);
    }

    @Override // android.content.ContextWrapper, android.content.Context
    public final String getPackageName() {
        if (!TextUtils.isEmpty(this.a)) {
            return "";
        }
        return super.getPackageName();
    }

    @Override // android.app.Application
    public final void onCreate() {
        super.onCreate();
        a();
    }
}
