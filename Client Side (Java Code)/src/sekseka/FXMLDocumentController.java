/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sekseka;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;
import javax.swing.JOptionPane;

/**
 *
 * @author Omar Shalash
 */
public class FXMLDocumentController implements Initializable {
    private Networking netting;
    private static String direction = "idle";
    @FXML
    private TextField userNameTF;
    @FXML
    private TextField PasswordTF;
    @FXML
    private Label label;
    
    @FXML
    private void handleButtonAction(ActionEvent event) 
    {
        if(((Button)event.getSource()).getText().equals("Login"))
        {
            try
            {
                if(userNameTF.getText().equals("Sekseka") && PasswordTF.getText().equals("1111"))
                    ((Stage)((Button)event.getSource()).getScene().getWindow()).setScene(new Scene(FXMLLoader.load(getClass().getResource("Selection.fxml"))));
                else 
                    JOptionPane.showMessageDialog(null, "Access Denied", "", JOptionPane.ERROR_MESSAGE);//, new ImageIcon(getClass().getResource("wifi.png")));
            }
            catch(Exception ex)
            {
                JOptionPane.showMessageDialog(null, ex.getMessage());
            }
        }
        else if(((Button)event.getSource()).getText().equals("Manual"))
        {            
            try
            {
                ((Stage)((Button)event.getSource()).getScene().getWindow()).setScene(new Scene(FXMLLoader.load(getClass().getResource("ManualControl.fxml"))));                
                //label.setText("connecting...");
                netting = new Networking(label,"Manual");
                netting.setDaemon(true);
                netting.start();
            }
            catch(Exception ex)
            {
                JOptionPane.showMessageDialog(null, ex.getMessage());
            }
        }
        else if(((Button)event.getSource()).getText().equals("Automatic"))
        {
            netting = new Networking(label,"Automatic");
            netting.setDaemon(true);
            netting.start();
        }
    }
    
    @FXML
    private void mousePressed(MouseEvent event)
    {
        //System.out.println(direction);
        ImageView inAction = (ImageView)event.getSource();
        if((event.getEventType()).getName().equals("MOUSE_PRESSED"))
        {
            if(inAction.getId().equals("up"))
            {
                inAction.setImage(new Image("up - Copy.png"));
                direction = "up";
            }
            else if(inAction.getId().equals("down"))
            {
                inAction.setImage(new Image("down - Copy.png"));
                direction = "down";
            }
            else if(inAction.getId().equals("left"))
            {
                inAction.setImage(new Image("left - Copy.png"));
                direction = "left";
            }
            else if(inAction.getId().equals("right"))
            {
                inAction.setImage(new Image("right - Copy.png"));
                direction = "right";
            }
            else if(inAction.getId().equals("in"))
            {
                inAction.setImage(new Image("up - Copy.png"));
                direction = "in";
            }
            else if(inAction.getId().equals("out"))
            {
                inAction.setImage(new Image("down - Copy.png"));
                direction = "out";
            }
        }
        else if((event.getEventType()).getName().equals("MOUSE_RELEASED"))
        {
            if(inAction.getId().equals("up"))
                inAction.setImage(new Image("up.png"));                
            else if(inAction.getId().equals("down"))
                inAction.setImage(new Image("down.png"));
            else if(inAction.getId().equals("left"))
                inAction.setImage(new Image("left.png"));
            else if(inAction.getId().equals("right"))
                inAction.setImage(new Image("right.png"));
            else if(inAction.getId().equals("in"))
                inAction.setImage(new Image("up.png"));                
            else if(inAction.getId().equals("out"))
                inAction.setImage(new Image("down.png"));
            direction = "idle";
        }
        //System.out.println(direction);
    }
    public static String getDirection()
    {
        //System.out.println(direction);
        return direction;
    }
    
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO        
    }    
    
}
