# TelesurgeryAutonomusControl
There are 3 parts that forms the full control of the robotic machine, 
a. The DFlow code, which is connected to the optical navigation system (OptiTrack) and reads all the inputs from the sensors to create the resection shaped then creates the burring path and creates a sequence of coordinates which sent to the machine to perform the resection procedure (burring), sadly DFlow requires license to use.
b. Client side (Java Application) which sends the generated coordinates from a file (Movements.txt in this case) to the machine through wireless network.
c. Server side (Arduino code/Robotic Machine), which receives the coordinates to start resection procedure on the knee.

This code was created to run a 3 DOF CNC robotic machine autonomously and remotely, please refer to my publication "OptiTrack-based Robotic System for Autonomous Unicompartmental Knee Arthroplasty" for more details
--------------------------------------------
For Server Side (Arduino code):
1-Make sure you include AccelStepper Library
2-Set your Server IP Address
3-Set your SSID and password
4-Build and run your application
--------------------------------------------
For Java Code:
1-Make sure you have JDK installed
2-in Networking.java Modify location for
 the file you want to send the coordinates
 to the machine in (in this case check
 Movements.txt file at root)
3-in Networking.java set Server IP (Arduino)


