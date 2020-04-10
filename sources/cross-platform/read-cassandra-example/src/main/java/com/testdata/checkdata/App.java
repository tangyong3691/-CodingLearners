package com.testdata.checkdata;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

//import org.json.JSONObject;

import com.datastax.driver.core.*;
//import java.sql.*;

//import com.datastax.driver.core.Cluster;
//import com.datastax.driver.core.ResultSet;
//import com.datastax.driver.core.Session;


/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {

        System.out.println( "Hello World!" );

        String query = "SELECT * FROM test1 limit 10";
        Cluster cluster = Cluster.builder().addContactPoint("192.168.1.33").build();  /*database ip */

        Metadata metadata = cluster.getMetadata();

        System.out.printf("Connected to cluster: %s\n", metadata.getClusterName());
        for (Host host: metadata.getAllHosts()) {
            System.out.printf("Datacenter: %s; Host: %s; Rack: %s\n", host.getDatacenter(), host.getAddress(), host.getRack());
        }


        Session session = cluster.connect("testdatabase"); //database
        ResultSet rs = session.execute(query);
        //System.out.println(result.all());

        List<Object> list = new ArrayList<Object>();
        ColumnDefinitions rscd = rs.getColumnDefinitions();
        int count = rscd.size();
        Map<String, String> reflect = new HashMap<String, String>();
        for (int i = 0; i < count; i++) {
            String column = rscd.getName(i);
            System.out.printf("comlumn %d: %s\n", i, column);
            String tcolumn = column.replaceAll("_", "");

            reflect.put(column, column);

        }
        for (Row row : rs.all()) {
            JSONObject obj = new JSONObject();
            for (String column : reflect.keySet()) {
                String key = reflect.get(column);
                Object value = row.getObject(column);
                System.out.println(value);
                obj.put(key, value);
            }
            Object object = obj;

            list.add(object);
        }





        /*while(result.next()) {
            System.out.printf("val: %s", result.getString(1));
        }*/

        //ResultSetMetaData rsmd = result.getMetaData();
        //for (int i = 1; i <= rsmd.getColumnCount(); i++) { String name = rsmd.getColumnName(i);  ystem.out.printf("column %d: %s", i, name); }
        session.close();
        cluster.close();
    }
}
