/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sekseka;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.scene.control.Label;
import javafx.scene.paint.Color;
import javax.swing.JOptionPane;

/**
 *
 * @author Omar Shalash
 */
public class Networking extends Thread
{
    private String mode;
    private PrintWriter out;
    private BufferedReader in;
    private BufferedReader implant;
    private Label statusLabel;
    private final int COMMON_DELAY = 20;
    private int countLoops = 0;
    Networking(Label statusLabel, String mode) {
        this.statusLabel = statusLabel;
        this.mode = mode;
    }
    @Override
    @SuppressWarnings("SleepWhileInLoop")
    public void run()
    {
        if(mode.equals("Manual"))
        {
            try 
            {
                //JOptionPane.showMessageDialog(null, "...");
                String direction, previous = "notSetYet";
                Socket client = new Socket("192.168.2.100", 80);
                if(client.isConnected())
                {
                    JOptionPane.showMessageDialog(null, "Connected to Server...");
                    statusLabel.setTextFill(Color.GREEN);
                    statusLabel.setText("Connected");
                }  
                else
                    JOptionPane.showMessageDialog(null, "Connected NOT to Server...");
                out = new PrintWriter(client.getOutputStream(), true);
                in = new BufferedReader(new InputStreamReader(client.getInputStream()));
                System.out.println("Sending!!");
                out.println("Manual");
                while(!in.ready())
                    Thread.sleep(1);
                System.out.println("Reading!!");
                while(!in.readLine().equals("Manual"))
                {                    
                    out.println("Manual");
                }
                out.println("OK");
                System.out.println("OK");
                while(true)
                {
                    System.out.println(++countLoops);
                    direction = FXMLDocumentController.getDirection();
                    System.out.println(direction);
                    out.println(direction);  
                    previous = direction;
                    Thread.sleep(COMMON_DELAY);
                }
            }             
            catch (Exception ex) {
                System.out.println(ex.getMessage());
                out.close();
            }
        }
        else if(mode.equals("Automatic"))
        {
            try 
            {
                Socket client = new Socket("192.168.2.100", 80);                
                if(client.isConnected())
                {
                    JOptionPane.showMessageDialog(null, "Connected to Server...");
                }  
                else
                    JOptionPane.showMessageDialog(null, "Connected NOT to Server...");
                out = new PrintWriter(client.getOutputStream(), true);
                in = new BufferedReader(new InputStreamReader(client.getInputStream()));  
                out.println("Automatic");
                while(!in.ready())
                    Thread.sleep(1);
                while(!in.readLine().equals("Automatic"))
                {
                    out.println("Automatic");                    
                }
                out.println("OK");
                implant = new BufferedReader(new FileReader("C:\\Movements.txt"));
                String line;
                while(true)
                {
                    System.out.println(++countLoops);
                    line = implant.readLine();
                    System.out.println(line);
                    out.println(line);
                    while(!in.readLine().equals(line))
                    {
                        out.println(line);
                        Thread.sleep(COMMON_DELAY);
                    }
                    out.println("OK");
                    System.out.println("OKed");
                }
            }             
            catch (Exception ex) 
            {
                System.out.println(ex.getMessage());
                out.close();
            }
        }
    }
}
