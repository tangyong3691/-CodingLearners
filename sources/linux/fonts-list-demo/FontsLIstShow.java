/*
    list all fonts in local env.

    Copyright (C) 2015 Tang Yong 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



import java.awt.*;
import java.io.*;
import java.awt.event.*;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.UIManager;
public class FontsLIstShow extends JFrame {
	static final int WIDTH=800;
    static final int HEIGHT=600;
	String[] fontNames;
	int fontindex;
	int fonttot;

  public FontsLIstShow(){
	  super("记事本");
	  
	  java.awt.GraphicsEnvironment eq = java.awt.GraphicsEnvironment.getLocalGraphicsEnvironment();
	  fontNames = eq.getAvailableFontFamilyNames();	  
	  fonttot = fontNames.length;
	  fontindex = 0;
    setSize(WIDTH,HEIGHT);
    setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    
    JMenuBar menubar1=new JMenuBar();//添加菜单条组件
    setJMenuBar(menubar1);//将菜单条添加到顶层容器中
    JMenu menu1=new JMenu("action");  //设置菜单组件    
    menubar1.add(menu1);//将菜单组件添加到菜单条组件中    
    JMenuItem item1=new JMenuItem("next fonts");//创建菜单项组件    
    menu1.add(item1); //将菜单项组件添加到相应的菜单组件中去
   
    
    
    /*addWindowListener(new WindowAdapter(){
      public void windowClosing(WindowEvent e){
      dispose();
      System.exit(0);
      }
    });*/
    final Canvas fontshow =new Canvas(){
        public void paint(Graphics g){
      	  int y = 40;
      	int k =0;
      	if(fontindex >= fonttot){
      		g.drawString("---end---" ,40,y);
      	}
      	for(; k + fontindex < fonttot && k < 10; k++){
      		int ind = k + fontindex;
      		String fontName = fontNames[k + fontindex];
      		System.out.println(ind);
      		  g.setFont(new Font(fontName,Font.PLAIN,30));
      	        g.drawString(ind +  " " + fontName +  " 中文显示" ,40,y);
      	        y += 40;
      	  }
      	 //fontindex += k;
          
          //g.setFont(new Font("AR PL UMing CN",Font.PLAIN,30));
          //g.drawString("中文显示" ,40,80);
        }
      };
      
    item1.addActionListener(
    		new ActionListener()
    	    {
    	    	public void actionPerformed(ActionEvent Event)
    	    	{
    	    		if(fontindex < fonttot){
    	    			fontindex += 10;
    	    		}else{
    	    			fontindex = 0;
    	    		}
    	    		//fontshow.validate();
    	    		fontshow.repaint();
    	    	}
    	    }	
    		);

    JPanel contentPane=new JPanel( );
    //setContentPane(contentPane);
    JButton button1=new JButton("next");
    contentPane.add(button1);

    final JTextArea c = new     JTextArea();
    c.setLineWrap(true);
    c.setAutoscrolls(true);
    c.setPreferredSize(new Dimension (200,200));
    c.setFont(new Font("文泉驿正黑",Font.PLAIN,12));
    c.setText("中文显示\r\nenglish");
    //c.insert("insert", c.getLineCount());
    
    
    contentPane.add(c);
    //add("Center", c);
    //validate();   
    
    add("Center", fontshow);
    setVisible(true);
    button1.addActionListener(new ActionListener()
    {
    	public void actionPerformed(ActionEvent Event)
    	{
    		int k =0;
    		for(; k + fontindex < fonttot && k < 1; k++){
    			String fontName = fontNames[k + fontindex];
    			c.setFont(new Font(fontName,Font.PLAIN,12));
    			c.insert(fontName + "中文\r\n\r\n", 0);
    		}
    		fontindex += k;
    	}
    });
  }

  

public static void main(String[] args){
	/*Font f =  new Font("文泉驿正黑",Font.PLAIN,12);
	
    UIManager.put("Label.font",f);
    UIManager.put("Label.foreground",Color.black);
    UIManager.put("Button.font",f);
    UIManager.put("Menu.font",f);
    UIManager.put("MenuItem.font",f);
    UIManager.put("List.font",f);
    UIManager.put("CheckBox.font",f);
    UIManager.put("RadioButton.font",f);
    UIManager.put("ComboBox.font",f);
    UIManager.put("TextArea.font",f);
    UIManager.put("EditorPane.font",f);
    UIManager.put("ScrollPane.font",f);
    UIManager.put("ToolTip.font",f);
    UIManager.put("TextField.font",f);
    UIManager.put("TableHeader.font",f);
    UIManager.put("Table.font",f);*/
	  
	  //for(int i=0;i<fontNames.length;i++){
	  //System.out.println(fontNames[i]);
	  //} 
    FontsLIstShow NB=new FontsLIstShow();
  }
} 
