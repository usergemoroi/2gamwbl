package p00093e85d7aface4dfb;

import android.app.Activity;
import android.app.AppComponentFactory;
import android.app.Application;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ContentProvider;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.text.TextUtils;
import java.lang.reflect.Method;
import java.util.Objects;

/* loaded from: classes.dex */
public class ProxyComponentFactory extends AppComponentFactory {
    public static AppComponentFactory a;

    public static AppComponentFactory a(ClassLoader classLoader) {
        if (a == null) {
            String rcf = JniBridge.rcf();
            if (!TextUtils.isEmpty(rcf)) {
                try {
                    AppComponentFactory a2 = c.a(Class.forName(rcf, true, classLoader).newInstance());
                    a = a2;
                    return a2;
                } catch (Exception unused) {
                }
            }
        }
        return a;
    }

    @Override // android.app.AppComponentFactory
    public final Activity instantiateActivity(ClassLoader classLoader, String str, Intent intent) {
        Objects.toString(classLoader);
        Objects.toString(intent);
        AppComponentFactory a2 = a(classLoader);
        if (a2 != null) {
            try {
                return (Activity) AppComponentFactory.class.getDeclaredMethod("instantiateActivity", ClassLoader.class, String.class, Intent.class).invoke(a2, classLoader, str, intent);
            } catch (Exception unused) {
            }
        }
        return super.instantiateActivity(classLoader, str, intent);
    }

    @Override // android.app.AppComponentFactory
    public final Application instantiateApplication(ClassLoader classLoader, String str) {
        AppComponentFactory a2;
        Objects.toString(classLoader);
        if (!a.a) {
            if (a.c() != null) {
                String str2 = a.c().dataDir;
                a.d(a.c().sourceDir, str2);
                JniBridge.a(str2);
            } else {
                throw new NullPointerException("application info is null");
            }
        }
        JniBridge.ia();
        String rapn = JniBridge.rapn();
        if (!a.a) {
            JniBridge.cbde(classLoader);
            a.a = true;
            a2 = a(classLoader);
        } else {
            a2 = a(classLoader);
        }
        a.b = false;
        if (a2 != null) {
            try {
                Method declaredMethod = a2.getClass().getDeclaredMethod("instantiateApplication", ClassLoader.class, String.class);
                return !TextUtils.isEmpty(rapn) ? (Application) declaredMethod.invoke(a2, classLoader, rapn) : (Application) declaredMethod.invoke(a2, classLoader, str);
            } catch (Exception unused) {
            }
        }
        if (!TextUtils.isEmpty(rapn)) {
            try {
                Class.forName(rapn, false, classLoader);
            } catch (ClassNotFoundException unused2) {
                if (a.c() != null) {
                    rapn = a.c().packageName + "." + rapn;
                }
            }
            return super.instantiateApplication(classLoader, rapn);
        }
        return super.instantiateApplication(classLoader, str);
    }

    @Override // android.app.AppComponentFactory
    public final ClassLoader instantiateClassLoader(ClassLoader classLoader, ApplicationInfo applicationInfo) {
        Objects.toString(classLoader);
        Objects.toString(applicationInfo);
        a.d(applicationInfo.sourceDir, applicationInfo.dataDir);
        JniBridge.a(applicationInfo.dataDir);
        JniBridge.ia();
        AppComponentFactory a2 = a(classLoader);
        JniBridge.cbde(classLoader);
        a.a = true;
        if (a2 != null) {
            try {
                return (ClassLoader) AppComponentFactory.class.getDeclaredMethod("instantiateClassLoader", ClassLoader.class, ApplicationInfo.class).invoke(a2, classLoader, applicationInfo);
            } catch (Exception unused) {
            }
        }
        return super.instantiateClassLoader(classLoader, applicationInfo);
    }

    @Override // android.app.AppComponentFactory
    public final ContentProvider instantiateProvider(ClassLoader classLoader, String str) {
        Objects.toString(classLoader);
        AppComponentFactory a2 = a(classLoader);
        if (a2 != null) {
            try {
                return (ContentProvider) AppComponentFactory.class.getDeclaredMethod("instantiateProvider", ClassLoader.class, String.class).invoke(a2, classLoader, str);
            } catch (Exception unused) {
            }
        }
        return super.instantiateProvider(classLoader, str);
    }

    @Override // android.app.AppComponentFactory
    public final BroadcastReceiver instantiateReceiver(ClassLoader classLoader, String str, Intent intent) {
        Objects.toString(classLoader);
        Objects.toString(intent);
        AppComponentFactory a2 = a(classLoader);
        if (a2 != null) {
            try {
                return (BroadcastReceiver) AppComponentFactory.class.getDeclaredMethod("instantiateReceiver", ClassLoader.class, String.class, Intent.class).invoke(a2, classLoader, str, intent);
            } catch (Exception unused) {
            }
        }
        return super.instantiateReceiver(classLoader, str, intent);
    }

    @Override // android.app.AppComponentFactory
    public final Service instantiateService(ClassLoader classLoader, String str, Intent intent) {
        Objects.toString(classLoader);
        Objects.toString(intent);
        AppComponentFactory a2 = a(classLoader);
        if (a2 != null) {
            try {
                return (Service) AppComponentFactory.class.getDeclaredMethod("instantiateService", ClassLoader.class, String.class, Intent.class).invoke(a2, classLoader, str, intent);
            } catch (Exception unused) {
            }
        }
        return super.instantiateService(classLoader, str, intent);
    }
}
