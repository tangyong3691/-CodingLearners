package com.ytweb.webviewapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.URLUtil;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.EditText;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    public static final String EXTRA_MESSAGE = "com.ytweb.webviewapp.MESSAGE";
    String[] hisurls = {"Apple","Apple2", "Banana", "Cherry", "Date", "Grape", "http://192.168.1.102:3209", "Kiwi", "Mango", "Pear"};
    //final WeburldbHelper dbhelper;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final WeburldbHelper dbhelper = new WeburldbHelper(this);
        //dbhelper.insert("http://");
        ArrayList<String> lasturls = dbhelper.getAllurls();
        int lastucount = lasturls.size();
        AutoCompleteTextView actv = (AutoCompleteTextView) findViewById(R.id.acTextView);
        if(lastucount == 1) actv.setText(lasturls.get(0));
        else if(lastucount > 1) {
            actv.setText("");
        }
        lasturls.add("http://");
        lasturls.add("https://");
        ArrayAdapter<String> adapter = new ArrayAdapter<String>
                (this, R.layout.custom_dropd_autocom_view,R.id.autocpoptextView, lasturls); //hisurls

        actv.setThreshold(1);//will start working from first character
        actv.setAdapter(adapter);//setting the adapter data into the AutoCompleteTextView
    }

    public void sendMessage(View view) {
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        //EditText editText = (EditText) findViewById(R.id.editTextTextPersonName3);
        AutoCompleteTextView actv = (AutoCompleteTextView) findViewById(R.id.acTextView);

        //String message = editText.getText().toString();
        String message = actv.getText().toString();
        if(URLUtil.isValidUrl(message)) {
            final WeburldbHelper dbhelper = new WeburldbHelper(this);
            dbhelper.insert(message);
            intent.putExtra(EXTRA_MESSAGE, message);
            startActivity(intent);
        }

    }

}