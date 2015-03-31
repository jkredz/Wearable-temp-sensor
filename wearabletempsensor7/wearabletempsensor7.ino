#include <avr/pgmspace.h>
#include <SPI.h>  // Serial Peripheral Interface library
#include <SD.h>  // SD Card library
#include <Adafruit_CC3000.h>
#include <ccspi.h>

#define WLAN_SSID       "HOME-E082"
#define WLAN_PASS       "3C5636914C613A5D"
#define WLAN_SECURITY   WLAN_SEC_WPA2

//#include <CapacitiveSensor.h> //Capacitive Sensor Library

#define ECHO_TO_SERIAL 1//echo data to sethermistor Type: Omega 44033

//#hermistor Stein-Hart Parameters
float A=.001468;
float B=.0002383;
float C=1.007*pow(10,-7);

//Button Setup
int buttonInt = 0;  //aka Digital Pin 2

//* Read analog voltage on pin 0 send to serial port every 3 seconds
byte analogInPin[] = {A0,A1,A2}; // Analog input pin
String thermstate[] = {'cold','cool','warm','hot'};  //Thermal State
const int chipSelect = 10;       // WiFi card ChipSelect
const int SDchipSelect = 8;      // SD Card Chipselect
const int vib = 9;               // Lilypad Vibrator setup
int sensorValue = 0;
int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;

volatile int counter = 0;
volatile int loopstate=1;  // current state of general loop [1 = regular , 0=reset loop]
int Vex=5.0; //Excitation Voltage
float Ro=10000.0; //Known Resistor Ohm
float T;  //Thermistor Air Temp (K)
float lnR;
float Rthm0; //Resistance of Thermistor (ohms)
float Vm;
float Vm0;  //Voltage measured across Known Resistor (Volts)
float Rthm;
float Rthm1; //Resistance of Thermistor (ohms)
float Vm1;  //Voltage measured across Known Resistor (Volts)
float Rthm2; //Resistance of Thermistor (ohms)
float Vm2;  //Voltage measured across Known Resistor (Volts)
float Tf; //Thermistor Air Temp (F) 
String dataString;

//CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);        // 10 megohm resistor between pins 4 & 5, pin 5 is sensor pin, add wire, foil

//MOSI - pin X
//MISO - pin Y
//CLK - pin Z
// CK - pin ZZ

void setup() {
  // initialize serial communications at 9600 bps:  
  Serial.begin(9600);
  
  // Setup for Cap Sensr turn off autocalibrate on channel 1 - just as an example
  //cs_4_5.set_CS_AutocaL_Millis(0xFFFFFFFF);     
  
  //SD Initialization
  Serial.print("Initializing, SD card...");
  pinMode(SDchipSelect, OUTPUT);
 
  if (!SD.begin(SDchipSelect)){
    Serial.println("Card failed, or not present");
    return;
  }
  
  Serial.println("card initialized.");
  //string for data to log
  String dataString = "Time,SkinTemp1,BodySensation,GPSLoc";
  File dataFile=SD.open("datalog2.csv", FILE_WRITE);
  if (dataFile){
    dataFile.println(dataString);
    dataFile.close();
    //print to the serial port
    Serial.println(dataString);
  }
  else{
    Serial.println("error openning datalog.txt");
  }       
  
  //Vibrator Setup
  pinMode(vib, OUTPUT);
  
  //Attaching interupt button, run funtion swap()
  attachInterrupt(buttonInt, interrupt1, Rising);
   
} 

void interrupt2()  {
  counter += 1;  //if button pressed again, add one to counter
  digitalWrite(vib, HIGH);
  digitalWrite(vib, LOW);
  detachInterrupt(buttonInt);  //detatch interrupt to reset
  attachInterrupt(buttonInt, interrupt2, Rising);  //attach the interrupt again to run interrupt2 code to register more inputs to the button
}
  

void interrupt1()  {
  counter += 1;
  digitalWrite(vib, HIGH);  //vibrate to indicate input
  digitalWrite(vib, LOW);   //go back to low to shut off vibration
  detachInterrupt(buttonInt);  //detach iterrupt to reset the button
  attachInterrupt(buttonInt, interrupt2, Rising);  

// if button pressed, start the counter and milli counter, and turn on iterrupt flag loop
//go back to regular loop and enter separate loop to test the milli counter 
//
  
  //delay for 10 secs? to determine 
  
  
  //.For 10 secs register, up strokes or downstrokes,
  //for each up stroke or down stroke buzz
  //count # of strokes to determine comfort
  // for hi low gesture, turn on wifi and buzz for a long time.
  
  State1
  //determine how hot/how cold and responsd with buzzer
  //label the body sensation
  //record location
  //loop through temp for 10 mins, and respond with long buzz for input

  // if button is pressed
  //if (button pressed){  
    
    //trigger-on=1;
    //loop through recording unless user does not input sensation
    //while(trigger-on==1){
      //Determine what is the sensation based on # of swipes
      //indicate= # times pressed + 1 for initial;
      //sensation='';
      //switch(indicate){
      //case '1':
        //sensation='warm';
        //break;
      //case '2':
        //sensation='hot';
        //break;
      //case '-1':
        //sensation='cool';
        //break;
      //case '-2':
        //sensation='cold';
        //break;
      //}
    
    //continously record and ask for sensation after 5mins
     // time = milli();
     // record-duration= (5 *60 *1000); //5mins*60sec/min*1000milli/s
       // while(time < record-duration){
        
          //save time stamp
          //ctime=xyzddskjfdkj
          //dataString += string(ctime)

          //int k=3; //#of sensors
          //for (int i=0; i < k-1; i++){
            //sensorValue = int(analogRead(analogInPin[i]));
            //Vm=sensorValue*(5.0/1023.0);
            //To be replaced with above Voltage_Measured(int x, int y)
            //Rthm=Therm_Resist(Ro,Vm,Vex);
            //lnR=log(Rthm);
            //T=S_H_EQ(A,B,C,lnR); //temp in kelvin
            //Tf=KtoF(T); //temp in F
            //dataString += String(Tf) + ","
            //Serial.println("Sensor #" + String(i) + "'s temperature reading is" + String(Tf) + "F")     
          //}
          
          //dataString += ",";
        
          //record GPS location
        //  dataString += GPS location;
        
          //record Time, Temp Skin, Sensation, and GPS into SD.
          //dataFile.println(dataString);
          //dataFile.close();

          //Delay to record temp every X seconds  
          //delay(6000-4);
          //}
        
        //buzz user for 5 secs digitalwrite(buzzer 3)
    //if button not triggered for 10 secs{
      //trigger-on=0;
    //else{
      //trigger-on=1;
      //}
    //}  
    //}
   //}

//save time stamp
//ctime=xyzddskjfdkj
//dataString += string(ctime)

//trigger a new state to reset general loop  state = regular =0





  State 2
  
   
  State 3
  
  

}  
   
   
   
void loop() {
  
//Code for button pressed situation

   

//set state= regular=1
  loopstate=1;
  while (loopstate)  {

//regular loop to run while loop (state= regular=1)     
  //Clear string
  dataString.remove(0);
  
  //Save Time
  int time = 0;
  Serial.println(time);
  dataString += String(time) + ",";
      
  // read the analog pin  
  
  int k=3; //#of sensors
    for (int i=0; i < k-1; i++){
      sensorValue = int(analogRead(analogInPin[i]));  
      dataString += String(sensorValue) + ",";
      Serial.println("Sensor #" + String(i) + "'s analog reading is " + String(sensorValue));     
    }
  
  dataString += "Netural,-";  // when button is not pressed, body is considered netural condition and no GPS recorded
  


  //open file in SD Card
 File dataFile=SD.open("datalog3.csv", FILE_WRITE);
 if (dataFile){
    dataFile.println(dataString);
    dataFile.close();
    //print to the serial port
    Serial.println(dataString);
  }
  else{
    Serial.println("error openning datalog.txt");
  }
   
  
  // wait 10 seconds before the next reading
  dataString.remove(0);
  delay(10000);
}
}  
// Functions

//Voltage Measured across known Resistor
//from analog read function x= the analog read value & y=excitation voltage
int Voltage_Measured(int x, int y){
int result;

result=x*(y/1023.0);
return result;  
}

//Thermistor Resistance Function (voltage divider calculation)
float Therm_Resist(float Ro, int Vm, int Vex){
float result;

result=(Vex*Ro)/Vm-Ro;
return result;
}

//SteinHart Equation Funtion
float S_H_Eq(float A, float B, float C, float lnR){
float result;
result=1/(A+(B*lnR)+C*(lnR*lnR*lnR));
return result;
}


//Kelvin to Farenheight Calc
float KtoF(float T){
float result;
result=(T-273.15)*1.8+32;
return result;
}
