/* SPI_CONTROLLER
   From: https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
   Modified by Forrest Lee Erickson 20220523
   Change to Controller/Peripheral termonology
   Change variable names for start with lowercase. Constants to uper case.
   Controller Arduino Code:
*/

//SPI CONTROLLER (ARDUINO UNO)
//SPI COMMUNICATION BETWEEN TWO ARDUINO UNOs
//CIRCUIT DIGEST

/* Hardware Notes
   SPI Line Pin in Arduino
  MOSI 11 or ICSP-4
  MISO 12 or ICSP-1
  SCK 13 or ICSP-3
  SS 10
*/

#include<SPI.h>                             //Library for SPI 
#define LED_PIN 7                               //Add this LED + Resistor
//#define BUTTON_PIN 2                          //Button to +5V, 10K Resistor to GND
#define BUTTON_PIN 2                          //Button to GND, 10K Resistor to +5V.

int buttonValue;
int x;
byte controllerSend, controlleReceive;


void setup (void)
{
  Serial.begin(115200);                   //Starts Serial Communication at Baud Rate 115200
  delay(500);
  Serial.println("Starting SPI Controller.");
  pinMode(BUTTON_PIN, INPUT);               //Sets pin 2 as input
  pinMode(LED_PIN, OUTPUT);                   //Sets pin 7 as Output

  SPI.begin();                            //Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 8 (16/8=2Mhz)
  pinMode(SS, OUTPUT);                 // Set nCS for output
  pinMode(MISO, INPUT);                  //Sets MISO as INPUT for testing contention ONLY.

  digitalWrite(SS, HIGH);                 // Setting PeripheralSelect as HIGH (So controller doesnt connnect with peripheral)
  delay(100);
}// end setup()


void loop(void)
{

  //  buttonValue = digitalRead(BUTTON_PIN);   //Reads the status of the pin 2
  //  if (buttonValue == HIGH)               //Logic for Setting x value (To be sent to peripheral) depending upon input from pin 2
  //    //  if (buttonValue == LOW)               //Logic for Setting x value (To be sent to peripheral) depending upon input from pin 2
  //  {
  //    x = 1;
  //  }
  //  else
  //  {
  //    x = 0;
  //  }

  //  x = buttonValue;
  controllerSend = digitalRead(BUTTON_PIN);
  Serial.print("SPI Controler: Button Status: ");
  Serial.print(controllerSend);

  digitalWrite(SS, LOW);                  //Starts communication with Peripheral connected to controller
  controlleReceive = SPI.transfer(controllerSend); //Send the controllersend value to peripheral also receives value from peripheral
  digitalWrite(SS, HIGH);                  //End communication with Peripheral connected to controller

  Serial.print(", Peripherial: ");
  Serial.println(controlleReceive);

  //  Serial.print("Peripherial LED: ");
  if (controlleReceive == 1)                  //Logic for setting the LED output depending upon value received from peripheral
  {
    digitalWrite(LED_PIN, HIGH);             //Sets pin 7 HIGH
    //  Serial.println(" ON");
  }
  else
  {
    digitalWrite(LED_PIN, LOW);              //Sets pin 7 LOW
    //Serial.println(" OFF");
  }

  delay(100);
}// end loop()
