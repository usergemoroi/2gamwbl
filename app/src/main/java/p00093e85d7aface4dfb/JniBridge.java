package p00093e85d7aface4dfb;

import android.content.Context;
import java.io.File;

/* loaded from: classes.dex */
public class JniBridge {
    public static void a(String str) {
        String[] strArr = {"lib2f8c0b3257fcc345.so"};
        try {
            String b = a.b();
            StringBuilder sb = new StringBuilder();
            sb.append(str);
            String str2 = File.separator;
            sb.append(str2);
            sb.append("hurri-libs");
            sb.append(str2);
            sb.append(b);
            File[] listFiles = new File(sb.toString()).listFiles();
            if (listFiles != null) {
                for (File file : listFiles) {
                    String absolutePath = file.getAbsolutePath();
                    if (absolutePath.endsWith(File.separator + strArr[0])) {
                        System.load(absolutePath);
                    }
                }
            }
        } catch (Throwable unused) {
        }
    }

    public static native void cbde(ClassLoader classLoader);

    public static native void craa(Context context, String str);

    public static native void craoc(String str);

    public static native String gap();

    public static native String gdp();

    public static native void ia();

    public static native Object ra(String str);

    public static native String rapn();

    public static native String rcf();

    public static native void rde(ClassLoader classLoader, String str);
}
