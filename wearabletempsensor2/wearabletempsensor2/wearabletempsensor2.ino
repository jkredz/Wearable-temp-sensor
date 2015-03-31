#include <math.h>
#include <avr/pgmspace.h>
#include <SPI.h>  // Serial Peripheral Interface library
#include <SD.h>  // SD Card library

//thermistor Type: Omega 44033
//#Thermistor Stein-Hart Parameters
float A=.001468;
float B=.0002383;
float C=1.007*pow(10,-7);


//* Read analog voltage on pin 0 send to serial port every 3 seconds

 
const int analogInPin0 = A0; // Analog input pin0
const int analogInPin1 = A1; // Analog input pin1
const int analogInPin2 = A2; // Analog input pin2
int sensorValue0 = 0;
int sensorValue1 = 0;
int sensorValue2 = 0;
int Vex=5.0; //Excitation Voltage
float Ro=1000.0; //Known Resistor Ohm
float T;  //Thermistor Air Temp (K)
float lnR;
float Rthm0; //Resistance of Thermistor (ohms)
float Vm0;  //Voltage measured across Known Resistor (Volts)
float Rthm1; //Resistance of Thermistor (ohms)
float Vm1;  //Voltage measured across Known Resistor (Volts)
float Rthm2; //Resistance of Thermistor (ohms)
float Vm2;  //Voltage measured across Known Resistor (Volts)
float Tf; //Thermistor Air Temp (F) 
//MOSI - pin X
//MISO - pin Y
//CLK - pin Z
// CK - pin ZZ

void setup() {
  // initialize serial communications at 9600 bps:  
  Serial.begin(9600);
  Serial.print("Initializing, SD card...");
  pinMode(10, OUTPUT);
 
  if (!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
} 
   
void loop() {
  {
    //string for data to log
    String dataString = "";
    
    
  
  
  // read the analog pin
  sensorValue0 = int(analogRead(analogInPin0));
  sensorValue1 = int(analogRead(analogInPin1));
  sensorValue2 = int(analogRead(analogInPin2));
  delay(4);
 
  // print the results to the serial monitor:
  Serial.print("sensorValue0 (0-1023)= " );
  Serial.println(sensorValue0);
  //Analog Input Reads 0-5 V in integers between 0-1023 (integer intervals .0049V)
  Vm0=sensorValue0*(5.0/1023.0);
  Serial.print("sensorValue0 Voltage reading= " );
  Serial.println(Vm0);
  
  Serial.print("sensorValue1 (0-1023)= " );
  Serial.println(sensorValue1);
  //Analog Input Reads 0-5 V in integers between 0-1023 (integer intervals .0049V)
  Vm1=sensorValue1*(5.0/1023.0);
  Serial.print("sensorValue1 Voltage reading= " );
  Serial.println(Vm1);
  
  Serial.print("sensorValue2 (0-1023)= " );
  Serial.println(sensorValue2);
  //Analog Input Reads 0-5 V in integers between 0-1023 (integer intervals .0049V)
  Vm2=sensorValue2*(5.0/1023.0);
  Serial.print("sensorValue2 Voltage reading= " );
  Serial.println(Vm2);
  
  //Calculate the Resistance of the thermistor using voltage divider calculation
  Rthm0=(Vex*Ro)/Vm0-Ro;
  Serial.print("Thermistor0 Resistance (ohms)= " );
  Serial.println(Rthm0);
  
  Rthm1=(Vex*Ro)/Vm1-Ro;
  Serial.print("Thermistor1 Resistance (ohms)= " );
  Serial.println(Rthm1);
  
  Rthm2=(Vex*Ro)/Vm2-Ro;
  Serial.print("Thermistor2 Resistance (ohms)= " );
  Serial.println(Rthm2); 
  
  //Calculate the Air Temperature using Steinhart-Hart Equation
  // T=1/(A+B*ln(R)+C(ln(R)^3)  (Temp in Kelvins)
  lnR=log(Rthm0);
  T=1/(A+(B*lnR)+C*(lnR*lnR*lnR));
  Serial.print("Thermistor0 Air Temperature(K)= " );
  Serial.println(T);
  Serial.print("Thermistor0 Air Temperature(F)= " );
  Tf=KtoF(T)
  Serial.println(Tf);
  Serial.println();
  dataString += String(Tf) += ",";

  lnR=log(Rthm1);
  T=1/(A+(B*lnR)+C*(lnR*lnR*lnR));
  Serial.print("Thermistor Air1 Temperature(K)= " );
  Serial.println(T);
  Serial.print("Thermistor Air1 Temperature(F)= " );
   Tf=KtoF(T)
  Serial.println(Tf);
  Serial.println();
  dataString += String(Tf) += ",";

  lnR=log(Rthm2);
  T=1/(A+(B*lnR)+C*(lnR*lnR*lnR));
  Serial.print("Thermistor Air2 Temperature(K)= " );
  Serial.println(T);
  Serial.print("Thermistor Air2 Temperature(F)= " );
  Tf=KtoF(T)
  Serial.println(Tf);
  Serial.println();
  dataString += String(Tf);
  
  //open file in SD Card
  File dataFile=SD.open("datalog.txt", FILE_WRITE);
  if (dataFile){
    dataFile.println(dataString);
    dataFile.close();
    //print to the serial port
    Serial.println(dataString);
  }
  else{
    Serial.println("error openning daalog.txt");
  }
   
  
  // wait 3 seconds before the next readings
  delay(6000-4);
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

//Kelvin to Farenheight Calc
float KtoF(float T)){
float result;
result=(T-273.15)*1.8+32;
return result;
}
