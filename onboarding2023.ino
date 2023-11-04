/* -------------------------------------Altimeter-------------------------------*/   
#include <Wire.h>
#include <Adafruit_BMP280.h>//9600
Adafruit_BMP280 bmp; // I2C  
// Variables/Constants  
double Pressure = 1013.25; //Deafualt 1013.25 changes depending on the day   
/*-------------------------------------SD Card Stuff------------------------------*/ 
#include <SD.h> 
//SD card file to write on 
File myFile;  
char textN[12] = {'t','e','s','t','0','_','_','.','t','x','t','\0'};     
/*-------------------------------------More Varibales and Constants----------------*/  
//Modifiable speed/ time interval data is collected for
double ReadSpeed = 10;  
//Decalration of variable to used to hold time from start
unsigned long tiempo; 
/*---------------------------------------------------------------------------------*/ 
void setup() {
  //Altimeter

  Serial.begin(9600);
  Serial.println(F("BMP280 test"));  
  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    while (1) delay(10);
  }


  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */   

  //SD Card    
  Serial.println("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  } 
  //More SD card 
  int i = 1;  
  while(SD.exists(textN) == true && i != 998) {   
    //precaution below as a number greater than 999 will cause issues
    if (!SD.exists(textN)) { 
      break;  
    }
    if( i < 10){  
      char iChar[2];
      itoa(i, iChar, 10); 
      textN[4] = iChar[0]; 
    } 
    else if (i >= 10 && i < 100) { 
      char iChar[3];
      itoa(i, iChar, 10); 
      textN[4] = iChar[0]; 
      textN[5] = iChar[1];  
    }  
    else if (i <=100) { 
      char iChar[4];
      itoa(i, iChar, 10); 
      textN[4] = iChar[0]; 
      textN[5] = iChar[1]; 
      textN[6] = iChar[2];  
    } 
    i = i + 1;    
  } 
  Serial.println(textN);  
}

void loop() {    
  myFile = SD.open(textN, FILE_WRITE);  
  //sensors_event_t temp; 

  tiempo = millis(); 
  
  Serial.print(F("Temperature = ")); 
  myFile.print(F("Temperature = ")); 
  Serial.print(bmp.readTemperature()); 
  myFile.print(bmp.readTemperature());
  Serial.println(" *C"); 
  myFile.println(" *C"); 
  

  Serial.print(F("Temperature = ")); 
  myFile.print(F("Temperature = ")); 
  Serial.print((bmp.readTemperature()) * (1.8) + (32)); 
  myFile.print(bmp.readTemperature() * (1.8) + (32));
  Serial.println(" *F"); 
  myFile.println(" *F");

  Serial.print(F("Pressure = ")); 
  myFile.print(F("Pressure = "));
  Serial.print(bmp.readPressure()); 
  myFile.print(bmp.readPressure());
  Serial.println(" Pa");
  myFile.println(" Pa");

  Serial.print(F("Approx altitude = ")); 
  myFile.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(Pressure)); /* Adjusted to local forecast!; defualt is 1013.25 */ 
  myFile.print(bmp.readAltitude(Pressure));

  Serial.println(" m"); 
  myFile.println(" m");

  Serial.println(); 
  myFile.println();
  
//delay(ReadSpeed);
//Acelerometer 

  myFile.print(tiempo); 
  myFile.println(" milliseconds");
  
  myFile.close();
  
  delay(ReadSpeed);
}
