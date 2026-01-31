package p00093e85d7aface4dfb;

import android.content.pm.ApplicationInfo;
import java.io.Closeable;
import java.io.IOException;
import java.lang.reflect.Field;

/* loaded from: classes.dex */
public abstract class a {
    public static volatile boolean a = false;
    public static volatile boolean b = true;

    public static void a(Closeable closeable) {
        if (closeable != null) {
            try {
                closeable.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static String b() {
        try {
            Class<?> cls = Class.forName("dalvik.system.VMRuntime");
            return (String) cls.getDeclaredMethod("vmInstructionSet", null).invoke(cls.getDeclaredMethod("getRuntime", null).invoke(null, null), null);
        } catch (Exception e) {
            e.printStackTrace();
            return "arm64";
        }
    }

    public static ApplicationInfo c() {
        try {
            Class<?> cls = Class.forName("android.app.ActivityThread");
            Field declaredField = cls.getDeclaredField("sCurrentActivityThread");
            declaredField.setAccessible(true);
            Object obj = declaredField.get(null);
            Field declaredField2 = cls.getDeclaredField("mBoundApplication");
            declaredField2.setAccessible(true);
            Object obj2 = declaredField2.get(obj);
            Field declaredField3 = obj2.getClass().getDeclaredField("appInfo");
            declaredField3.setAccessible(true);
            return (ApplicationInfo) declaredField3.get(obj2);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /* JADX WARN: Code restructure failed: missing block: B:32:0x00c1, code lost:
    
        r10 = new byte[4096];
        r2 = new java.io.BufferedOutputStream(new java.io.FileOutputStream(r2));
        r0 = new java.io.BufferedInputStream(r11.getInputStream(r6));
     */
    /* JADX WARN: Code restructure failed: missing block: B:33:0x00d6, code lost:
    
        r11 = r0.read(r10);
     */
    /* JADX WARN: Code restructure failed: missing block: B:34:0x00da, code lost:
    
        if (r11 == (-1)) goto L48;
     */
    /* JADX WARN: Code restructure failed: missing block: B:35:0x00dc, code lost:
    
        r2.write(r10, 0, r11);
     */
    /* JADX WARN: Code restructure failed: missing block: B:37:0x00e1, code lost:
    
        r6.getName();
        r6.getCrc();
        a(r2);
     */
    /* JADX WARN: Removed duplicated region for block: B:26:0x00a9 A[Catch: Exception -> 0x00eb, TryCatch #0 {Exception -> 0x00eb, blocks: (B:23:0x009a, B:24:0x00a3, B:26:0x00a9, B:29:0x00b9, B:32:0x00c1, B:33:0x00d6, B:35:0x00dc, B:37:0x00e1), top: B:22:0x009a }] */
    /* JADX WARN: Removed duplicated region for block: B:45:0x00ef A[EDGE_INSN: B:45:0x00ef->B:38:0x00ef BREAK  A[LOOP:1: B:24:0x00a3->B:43:0x00a3], SYNTHETIC] */
    /*
        Code decompiled incorrectly, please refer to instructions dump.
        To view partially-correct add '--show-bad-code' argument
    */
    public static void d(java.lang.String r10, java.lang.String r11) {
        /*
            java.lang.String r0 = b()
            java.io.File r1 = new java.io.File
            java.lang.StringBuilder r2 = new java.lang.StringBuilder
            r2.<init>()
            r2.append(r11)
            java.lang.String r11 = java.io.File.separator
            r2.append(r11)
            java.lang.String r3 = "hurri-libs"
            r2.append(r3)
            r2.append(r11)
            r2.append(r0)
            java.lang.String r2 = r2.toString()
            r1.<init>(r2)
            java.lang.StringBuilder r2 = new java.lang.StringBuilder
            java.lang.String r3 = "assets/burriiiii/"
            r2.<init>(r3)
            r2.append(r0)
            java.lang.String r0 = "/lib2f8c0b3257fcc345.so"
            r2.append(r0)
            java.lang.String r0 = r2.toString()
            java.lang.String r1 = r1.getAbsolutePath()
            java.lang.System.currentTimeMillis()
            java.io.File r2 = new java.io.File
            r2.<init>(r1)
            boolean r3 = r2.exists()
            if (r3 != 0) goto L4d
            r2.mkdirs()
        L4d:
            java.io.File r2 = new java.io.File
            java.lang.StringBuilder r3 = new java.lang.StringBuilder
            r3.<init>()
            r3.append(r1)
            r3.append(r11)
            java.lang.String r11 = "lib2f8c0b3257fcc345.so"
            r3.append(r11)
            java.lang.String r11 = r3.toString()
            r2.<init>(r11)
            boolean r11 = r2.exists()
            r1 = -1
            r3 = 4096(0x1000, float:5.74E-42)
            r4 = 0
            if (r11 == 0) goto L9a
            r11 = 0
            java.io.FileInputStream r6 = new java.io.FileInputStream     // Catch: java.lang.Throwable -> L97
            r6.<init>(r2)     // Catch: java.lang.Throwable -> L97
            java.util.zip.CheckedInputStream r7 = new java.util.zip.CheckedInputStream     // Catch: java.lang.Throwable -> L97
            java.util.zip.CRC32 r8 = new java.util.zip.CRC32     // Catch: java.lang.Throwable -> L97
            r8.<init>()     // Catch: java.lang.Throwable -> L97
            r7.<init>(r6, r8)     // Catch: java.lang.Throwable -> L97
            byte[] r11 = new byte[r3]     // Catch: java.lang.Throwable -> L96
        L83:
            int r6 = r7.read(r11)     // Catch: java.lang.Throwable -> L96
            if (r6 == r1) goto L8a
            goto L83
        L8a:
            java.util.zip.Checksum r11 = r7.getChecksum()     // Catch: java.lang.Throwable -> L96
            long r4 = r11.getValue()     // Catch: java.lang.Throwable -> L96
            a(r7)
            goto L9a
        L96:
            r11 = r7
        L97:
            a(r11)
        L9a:
            java.util.zip.ZipFile r11 = new java.util.zip.ZipFile     // Catch: java.lang.Exception -> Leb
            r11.<init>(r10)     // Catch: java.lang.Exception -> Leb
            java.util.Enumeration r10 = r11.entries()     // Catch: java.lang.Exception -> Leb
        La3:
            boolean r6 = r10.hasMoreElements()     // Catch: java.lang.Exception -> Leb
            if (r6 == 0) goto Lef
            java.lang.Object r6 = r10.nextElement()     // Catch: java.lang.Exception -> Leb
            java.util.zip.ZipEntry r6 = (java.util.zip.ZipEntry) r6     // Catch: java.lang.Exception -> Leb
            java.lang.String r7 = r6.getName()     // Catch: java.lang.Exception -> Leb
            boolean r7 = r7.equals(r0)     // Catch: java.lang.Exception -> Leb
            if (r7 == 0) goto La3
            long r7 = r6.getCrc()     // Catch: java.lang.Exception -> Leb
            int r9 = (r4 > r7 ? 1 : (r4 == r7 ? 0 : -1))
            if (r9 == 0) goto La3
            byte[] r10 = new byte[r3]     // Catch: java.lang.Exception -> Leb
            java.io.FileOutputStream r0 = new java.io.FileOutputStream     // Catch: java.lang.Exception -> Leb
            r0.<init>(r2)     // Catch: java.lang.Exception -> Leb
            java.io.BufferedOutputStream r2 = new java.io.BufferedOutputStream     // Catch: java.lang.Exception -> Leb
            r2.<init>(r0)     // Catch: java.lang.Exception -> Leb
            java.io.BufferedInputStream r0 = new java.io.BufferedInputStream     // Catch: java.lang.Exception -> Leb
            java.io.InputStream r11 = r11.getInputStream(r6)     // Catch: java.lang.Exception -> Leb
            r0.<init>(r11)     // Catch: java.lang.Exception -> Leb
        Ld6:
            int r11 = r0.read(r10)     // Catch: java.lang.Exception -> Leb
            if (r11 == r1) goto Le1
            r3 = 0
            r2.write(r10, r3, r11)     // Catch: java.lang.Exception -> Leb
            goto Ld6
        Le1:
            r6.getName()     // Catch: java.lang.Exception -> Leb
            r6.getCrc()     // Catch: java.lang.Exception -> Leb
            a(r2)     // Catch: java.lang.Exception -> Leb
            goto Lef
        Leb:
            r10 = move-exception
            r10.printStackTrace()
        Lef:
            java.lang.System.currentTimeMillis()
            return
        */
        throw new UnsupportedOperationException("Method not decompiled: p00093e85d7aface4dfb.a.d(java.lang.String, java.lang.String):void");
    }
}
