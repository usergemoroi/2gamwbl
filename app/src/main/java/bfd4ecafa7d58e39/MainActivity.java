package bfd4ecafa7d58e39;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.widget.TextView;
import com.eternal.xdsdk.R;
import java.util.Arrays;
import java.util.Base64;

/* loaded from: classes.dex */
public class MainActivity extends Activity {
    @Override // android.app.Activity
    public final void onCreate(Bundle bundle) {
        Base64.Decoder decoder;
        byte[] decode;
        super.onCreate(bundle);
        setContentView(R.bool.BhaiGaandMarao);
        TextView textView = (TextView) findViewById(R.attr.BhaiGaandMarao);
        StringBuilder sb = new StringBuilder();
        if (Build.VERSION.SDK_INT >= 26) {
            decoder = Base64.getDecoder();
            decode = decoder.decode("QnVycmkgQnVycmkgRW5jcnlwdGlvbiBFcnJvcg==");
            sb.append(Arrays.toString(decode));
        } else {
            sb.append(Arrays.toString(android.util.Base64.decode("QnVycmkgQnVycmkgRW5jcnlwdGlvbiBFcnJvcg==", 0)));
        }
        sb.append("\nApplication: ");
        sb.append(getApplication().getClass().getName());
        textView.setText(sb);
    }
}
