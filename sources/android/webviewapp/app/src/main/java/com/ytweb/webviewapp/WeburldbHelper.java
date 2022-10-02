package com.ytweb.webviewapp;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.support.annotation.Nullable;

import java.util.ArrayList;

public class WeburldbHelper extends SQLiteOpenHelper {
    public static final String DATABASE_NAME = "WebUrlsDB";
    public static final String WEBURL_TABLE_NAME = "urls";
    private static final int DATABASE_VERSION = 1;
    public WeburldbHelper(@Nullable Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase sqLiteDatabase) {
        sqLiteDatabase.execSQL(
                "create table "+ WEBURL_TABLE_NAME +"(id integer primary key NOT NULL UNIQUE , url text  NOT NULL UNIQUE,date text )"
        );
    }

    @Override
    public void onUpgrade(SQLiteDatabase sqLiteDatabase, int i, int i1) {
        sqLiteDatabase.execSQL("DROP TABLE IF EXISTS "+WEBURL_TABLE_NAME);
        onCreate(sqLiteDatabase);
    }

    public boolean insert(String s) {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put("url", s);
        contentValues.put("date", "empty string");
        try {
            db.insert(WEBURL_TABLE_NAME, null, contentValues);
        } catch (Exception e) {
            // Log.d(TAG, "Error while trying to get posts from database");
        }
        db.close();
        return true;
    }

    public boolean deleterow(String s) {
        SQLiteDatabase db = this.getWritableDatabase();
        try{
            db.delete(WEBURL_TABLE_NAME,  "url=?" , new String[]{s});
        }catch (Exception e) {
        }
        db.close();
        return true;
    }

    public ArrayList<String> getAllurls() {
        ArrayList<String> urllist = new ArrayList<String>();
        String URLS_SELECT_QUERY =
                String.format("SELECT * FROM %s ", WEBURL_TABLE_NAME);
        SQLiteDatabase db = getReadableDatabase();
        Cursor cursor = db.rawQuery(URLS_SELECT_QUERY, null);
        try {
            if (cursor.moveToFirst()) {
                do {
                    urllist.add(cursor.getString(cursor.getColumnIndex("url")));
                } while(cursor.moveToNext());
            }
        } catch (Exception e) {
           // Log.d(TAG, "Error while trying to get posts from database");
        } finally {
            if (cursor != null && !cursor.isClosed()) {
                cursor.close();
            }
        }
        db.close();
        return urllist;
    }

}
