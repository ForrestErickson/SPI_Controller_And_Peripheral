/* SPI_CONTROLLER
   From: https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
   Modified by Forrest Lee Erickson 20220523
   Change to Controller/Peripheral termonology
   Controller Arduino Code:
*/

//SPI CONTROLLER (ARDUINO UNO)
//SPI COMMUNICATION BETWEEN TWO ARDUINO UNOs
//CIRCUIT DIGEST


#include<SPI.h>                             //Library for SPI 
#define LED 7                               //Add this LED + Resistor
#define ipbutton 2                          //Button to +5V, 10K Resistor to GND

int buttonvalue;
int x;

void setup (void)
{
  Serial.begin(115200);                   //Starts Serial Communication at Baud Rate 115200
  pinMode(ipbutton, INPUT);               //Sets pin 2 as input
  pinMode(LED, OUTPUT);                   //Sets pin 7 as Output
  SPI.begin();                            //Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV8);    //Sets clock for SPI communication at 8 (16/8=2Mhz)
  digitalWrite(SS, HIGH);                 // Setting PeripheralSelect as HIGH (So controller doesnt connnect with peripheral)
}// end setup()


void loop(void)
{
  byte Controllersend, Controllereceive;
  buttonvalue = digitalRead(ipbutton);   //Reads the status of the pin 2
  if (buttonvalue == HIGH)               //Logic for Setting x value (To be sent to peripheral) depending upon input from pin 2
  {
    x = 1;
  }else
  {
    x = 0;
  }
  digitalWrite(SS, LOW);                  //Starts communication with Peripheral connected to controller
  Controllersend = x;
  Controllereceive = SPI.transfer(Controllersend); //Send the controllersend value to peripheral also receives value from peripheral
  if (Controllereceive == 1)                  //Logic for setting the LED output depending upon value received from peripheral
  {
    digitalWrite(LED, HIGH);             //Sets pin 7 HIGH
    Serial.println("Controller LED ON");
  }
  else
  {
    digitalWrite(LED, LOW);              //Sets pin 7 LOW
    Serial.println("Controller LED OFF");
  }
  delay(1000);
}// end loop()
