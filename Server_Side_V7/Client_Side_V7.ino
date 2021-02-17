#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include <SPI.h>
#include <WiFi.h>

IPAddress ip(111,111,1,111);  //Set your IP
char ssid[] = "XXXXXXXX";     //your network SSID (name)
char pass[] = "XYXYXYXYXYX";  //your network password (use for WPA, or use as key for WEP)


int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);
boolean alreadyConnected = false; // whether or not the client was connected previously

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
const int RESET = 50;
const int JOYSTICK1_VRX = A0;
const int JOYSTICK1_VRY = A1;
const int JOYSTICK1_SW = A2;
const int JOYSTICK2_VRX = A3;
const int JOYSTICK2_VRY = A4;
const int JOYSTICK2_SW = A5;
const int MOTOR_M_DIRECTION = 30;
const int MOTOR_M_PULL = 31;
const int MOTOR_Y1_DIRECTION = 32;
const int MOTOR_Y1_PULL = 33;
const int MOTOR_Y2_DIRECTION = 34;
const int MOTOR_Y2_PULL = 35;
const int MOTOR_X1_DIRECTION = 36;
const int MOTOR_X1_PULL = 37;
const int MOTOR_X2_DIRECTION = 38;
const int MOTOR_X2_PULL = 39;
const int MAX_SPEED = 3000;
const int MAX_ACCELERATION = 2500;
const int COMMON_DELAY = 1;
String directions[6];
AccelStepper stepperM(1, MOTOR_M_PULL, MOTOR_M_DIRECTION);
AccelStepper stepperY1(1, MOTOR_Y1_PULL, MOTOR_Y1_DIRECTION);
AccelStepper stepperY2(1, MOTOR_Y2_PULL, MOTOR_Y2_DIRECTION);
AccelStepper stepperZ1(1, MOTOR_X1_PULL, MOTOR_X1_DIRECTION);
AccelStepper stepperZ2(1, MOTOR_X2_PULL, MOTOR_X2_DIRECTION);
int mode; // 0 = welcome - 1 = Network - 2 = Manual
float x,y,z;
WiFiClient client;
String networkMode;
bool printNetworkStatus;
void setup() 
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Welcome...");
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    lcd.setCursor(0, 1);
    lcd.print("Shield problem!");
    Serial.println("Shield problem!");
    // don't continue:
    while (true);
  }
  WiFi.config(ip);
  stepperM.setMaxSpeed(MAX_SPEED);
  stepperM.setSpeed(MAX_SPEED);
  stepperM.setAcceleration(MAX_ACCELERATION);
  stepperY1.setMaxSpeed(MAX_SPEED);
  stepperY1.setSpeed(MAX_SPEED);
  stepperY1.setAcceleration(MAX_ACCELERATION);
  stepperY2.setMaxSpeed(MAX_SPEED);
  stepperY2.setSpeed(MAX_SPEED);
  stepperY2.setAcceleration(MAX_ACCELERATION);
  stepperZ1.setMaxSpeed(MAX_SPEED);
  stepperZ1.setSpeed(MAX_SPEED);
  stepperZ1.setAcceleration(MAX_ACCELERATION);
  stepperZ2.setMaxSpeed(MAX_SPEED);
  stepperZ2.setSpeed(MAX_SPEED);
  stepperZ2.setAcceleration(MAX_ACCELERATION);
  mode = 0;
  printNetworkStatus = false;
  x = 0;
  y = 0;
  z = 0;
  pinMode(RESET, INPUT_PULLUP);
}
//The Joysticks: VRX (UP = Max, Down = 0) VRY (RIGHT = Max, LEFT = 0) SW (PRESS = 0, ELSE !0)



void loop() 
{  
  //Serial.println(digitalRead());
  if(mode == 0)
  {
    
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      lcd.print("Manual<.>Network");
      getReadings();
      if(directions[2] == "PRESSED")
      {
          mode = 2;
          lcd.clear();
          lcd.setCursor(0, 0);          
          lcd.print("MANUAL...");
          Serial.println("MANUAL...");
          delay(200);
      }
      else if(directions[5] == "PRESSED")
      {
          mode = 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Network...");  
          Serial.println("Network...");  
      }
      delay(50);
  }
  else if(mode == 1) //Network
  {
    getReadings();
    if(!alreadyConnected)
    {        
        //Serial.println("NOT!!!");
        //Networking Again:
        while ( status != WL_CONNECTED) 
        {
          lcd.setCursor(0, 1);
          lcd.print("Connecting:");
          lcd.print(ssid);
          Serial.print("Connecting: ");
          Serial.println(ssid);
          // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
          status = WiFi.begin(ssid, pass);
          // wait 10 seconds for connection:
          delay(3000);
          
          
          // you're connected now, so print out the status:
          printWifiStatus();
        }
        if(status == WL_CONNECTED && !printNetworkStatus)
         {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Network...");
          lcd.setCursor(0, 1);
          lcd.print("Connected");
          printNetworkStatus = true;
         }
      
        // start the server:
        server.begin();
        client = server.available();        
        //lcd.setCursor(0, 1);
        //lcd.print("Waiting Client..");
        Serial.println("Waiting Client");
        if(client.connected())
        {    
          lcd.setCursor(0, 1);
          lcd.print("Client Connected");  
          //Serial.println("Client Connected");
          alreadyConnected = true;
          networkMode = "starting";
        }
        //Serial.println("waiting");      
    }
    if (client) 
    { 
      if(networkMode == "starting")
      {
        networkMode = getNetworkReading();
        Serial.println(networkMode+"<--OUTSIDE");       
      }
      networkMode.trim();
      //Serial.println(networkMode);
      if(networkMode.equals("Manual"))
      {
        String in = 
        client.readString();
        in.trim();
        Serial.println(in+"<--OUTSIDE");
        // echo the bytes back to the client:
        if(in == "up")
        {
            directions[4] = "RIGHT";
            Serial.println("UP");
        }
        else if(in == "down")
        {
            directions[4] = "LEFT";
            Serial.println("DOWN");
        }
        else if(in == "left")
        {
            directions[1] = "LEFT";
            Serial.println("LEFT");
        }
        else if(in == "right")
        {
            directions[1] = "RIGHT";
            Serial.println("RIGHT");
        }
        else if(in == "in")
        {
            directions[4] = "LEFT";
            Serial.println("IN");
        }
        else if(in == "OUT")
        {
            directions[4] = "RIGHT";
            Serial.println("OUT");
        }
        else if(in == "idle")
        {
            directions[1] = "Nah";
            directions[0] = "Nah";
            directions[4] = "Nah";
            Serial.println("Idle");
        }        
        runIt();
      }
      else if(networkMode == "Automatic")
      {
        char in[60];
        String data = getNetworkReading();
        data.toCharArray(in,60);
        data.trim();
        Serial.println(in);
        if(data == "HALT")
        {
          reset();
          Serial.println("HALTTED!!!");
        }
        else
        {
          /*
          char* tokens = strtok ( in, ";" );
          x = atof(tokens);
          tokens = strtok ( in, ";" );
          y = atof(tokens);
          tokens = strtok ( in, ";" );
          z = atof(tokens);
          Serial.println(x);
          Serial.println(y);
          Serial.println(z);
          */
          int i = 0;
          for(i; i < data.length();i++)
          {            
            if(data.substring(i,i+1) == ";")
            {
              x = data.substring(0,i).toFloat();
              break;
            }
          }
          i +=1;
          int b = i;
          for(i; i < data.length();i++)
          {
            //Serial.println(data.substring(i,i+1)+"<---");
            if(data.substring(i,i+1) == ";")
            {
              y = data.substring(b,i).toFloat();
              break;
            }
          }
          i +=1;
          z = data.substring(i,data.length()).toFloat();
          
          //Serial.println(x);
          //Serial.println(y);
          //Serial.println(z);
          stepperM.moveTo(cmToTurns(x*100));
          stepperZ1.moveTo(cmToTurns(z*100));
          stepperZ2.moveTo(cmToTurns(z*100));
          stepperY1.moveTo(cmToTurns(y*-100));
          stepperY2.moveTo(cmToTurns(y*-100));
          while(stepperM.distanceToGo() != 0 || stepperZ1.distanceToGo() != 0 || stepperY1.distanceToGo() != 0)
          {
            delay(COMMON_DELAY);
            stepperM.run();
            stepperZ1.run();
            stepperZ2.run();
            stepperY1.run();
            stepperY2.run();
          }
          delay(10);
        }
      }
    }
    else if(alreadyConnected)
    {
      lcd.setCursor(0, 1);
      lcd.print("Client disConnected");
      //alreadyConnected = false;
      //reset();
    }
  }
  else //Manual
  {
      getReadings();
      runIt();
      delay(COMMON_DELAY);
  }
}
String getNetworkReading()
{
  int counter = 0;
  char temp[60];
  String reading = "";
  while(!client.available())
  {
    delay(5);
  }
  Serial.println("READ>");
  String networkMsg = client.readStringUntil('\n');
  Serial.println("networkMsg: "+networkMsg);
  //Serial.println(networkMsg+"\n"+reading);
  while(reading != "OK")
  {
    if(reading != "OK" && !reading.equals("\0") && reading != 0)
      networkMsg = reading;
    networkMsg.toCharArray(temp,60);
    Serial.println("networkMsg: "+networkMsg);
    Serial.print("Sending!!");
    Serial.println(temp);
    client.write(temp); 
    Serial.print("Sent!!");
    while(!client.available())
    {
        delay(5);
    }
    Serial.println("READ>");
    reading = client.readStringUntil('\n');
    reading.trim();
    Serial.println("done:  "+reading);
  }
  //Serial.println(networkMsg+"   <--OK");
  return networkMsg;
}
void getReadings()
{
  if(checkReset())
    return;
  //Joystick 1 >>
  if(analogRead(JOYSTICK1_VRX) > 790)
    directions[0] = "DOWN";
  else if(analogRead(JOYSTICK1_VRX) < 720)
    directions[0] = "UP";
  else
    directions[0] = "NAH";
  
  if(analogRead(JOYSTICK1_VRY) > 790)
    directions[1] = "LEFT";
  else if(analogRead(JOYSTICK1_VRY) < 720)
    directions[1] = "RIGHT";
  else
    directions[1] = "NAH";
    
  if(analogRead(JOYSTICK1_SW) == 0)
    directions[2] = "PRESSED";
  else
    directions[2] = "NAH";
  //Joystick 2 >>
  if(analogRead(JOYSTICK2_VRX) > 790)
    directions[3] = "DOWN";
  else if(analogRead(JOYSTICK2_VRX) < 720)
    directions[3] = "UP";
  else
    directions[3] = "NAH";
    
  if(analogRead(JOYSTICK2_VRY) > 790)
    directions[4] = "LEFT";
  else if(analogRead(JOYSTICK2_VRY) < 720)
    directions[4] = "RIGHT";
  else
    directions[4] = "NAH";
    
  if(analogRead(JOYSTICK2_SW) == 0)
    directions[5] = "PRESSED";
  else
    directions[5] = "NAH";
  /*
  //Serial.println("0 "+(String)directions[0]);
  Serial.println("1 "+(String)directions[1]);
  //Serial.println("2 "+(String)directions[2]);
  //Serial.println("3 "+(String)directions[3]);
  //Serial.println("4 "+(String)directions[4]);
  //Serial.println("5 "+(String)directions[5]);
  if(digitalRead(RESET)) 
    Serial.println("RESET: HIGH ");
  else
    Serial.println("RESET: LOW ");
  */
  if(directions[2] == "PRESSED" && directions[5] == "PRESSED")
  {
    mode = 0;
    delay(300);  
  }
}

void runIt()
{
  if(directions[0] == "UP")
  {
      y-=1;
      stepperY1.setSpeed(-MAX_SPEED);
      stepperY2.setSpeed(-MAX_SPEED);
      stepperY1.runSpeed();
      stepperY2.runSpeed();
  }
  else if(directions[0] == "DOWN")
  {    
      y+=1;
      stepperY1.setSpeed(MAX_SPEED);
      stepperY2.setSpeed(MAX_SPEED);
      stepperY1.runSpeed();
      stepperY2.runSpeed();       
  }  
  if(directions[1] == "LEFT")
  {
      z-=1;
      stepperZ1.setSpeed(-MAX_SPEED);
      stepperZ2.setSpeed(-MAX_SPEED);
      stepperZ1.runSpeed();
      stepperZ2.runSpeed();
  }
  else if(directions[1] == "RIGHT")
  {    
      z+=1;
      stepperZ1.setSpeed(MAX_SPEED);
      stepperZ2.setSpeed(MAX_SPEED);
      stepperZ1.runSpeed();
      stepperZ2.runSpeed();       
  }  
  if(directions[4] == "LEFT")
  {
      x+=1;
      stepperM.setSpeed(MAX_SPEED);
      stepperM.runSpeed();
  }
  else if(directions[4] == "RIGHT")
  {    
      x-=1;
      stepperM.setSpeed(-MAX_SPEED);
      stepperM.runSpeed(); 
  } 
}

int cmToTurns(float x)
{
  return x*2000;
}

void printWifiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
bool checkReset()
{
  if(digitalRead(RESET)==HIGH)
  {
    //reset();
    return true;
  }
  return false;
}

void reset()
{
  lcd.clear();
  if(client.connected())
  {
    WiFi.disconnect();
    alreadyConnected = false;
    lcd.setCursor(0, 1);
    lcd.print("Net. disConnected");
    printNetworkStatus = false;
  }
  mode = 0;
  
  lcd.setCursor(0, 0);          
  lcd.print("RESETTING...");
  Serial.println("Resetting...");
  //Serial.print(stepperY1.distanceToGo());
  
  stepperM.stop();
  stepperZ1.stop();
  stepperZ2.stop();
  stepperY1.stop();
  stepperY2.stop(); 
  stepperM.moveTo(0);
  stepperZ1.moveTo(0);
  stepperZ2.moveTo(0);
  stepperY1.moveTo(0);
  stepperY2.moveTo(0);
  delay(300);
  while(stepperM.distanceToGo() != 0 || stepperZ1.distanceToGo() != 0 || stepperY1.distanceToGo() != 0)
  {
    delay(COMMON_DELAY);
    stepperM.run();
    stepperZ1.run();
    stepperZ2.run();
    stepperY1.run();
    stepperY2.run();
  }
  x = 0;
  y = 0;
  z = 0;
  lcd.clear();
  lcd.setCursor(0, 0);          
  lcd.print("Welcome...");
}

