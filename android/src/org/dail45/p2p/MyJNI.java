package org.dail45.p2p;

import android.app.Activity;
import android.database.Cursor;
import android.net.Uri;
import android.content.ContentResolver;
import android.os.ParcelFileDescriptor;

import java.io.FileNotFoundException;

public class MyJNI
{
    private final Activity m_MainActivity;
    public MyJNI(final Activity MainActivity) {
        m_MainActivity = MainActivity;
    }
    public int getFdFromString(String suri) {
        ContentResolver cr = m_MainActivity.getContentResolver();
        Uri uri = Uri.parse(suri);
        if (uri.toString().length() > 0) {
            try {
                ParcelFileDescriptor fd = cr.openFileDescriptor(uri, "r");
                int res = fd.getFd();
                return res;
            } catch (java.io.IOException e){
                return -1;
            }
        }
        return -1;
    }
    public String getFileNameFromString(String suri) {
        ContentResolver cr = m_MainActivity.getContentResolver();
        Uri uri = Uri.parse(suri);
        Cursor cur = cr.query(uri, null, null, null, null);
        cur.moveToFirst();
        String name = cur.getString(cur.getColumnIndex("_display_name"));
        return name;
    }
}
