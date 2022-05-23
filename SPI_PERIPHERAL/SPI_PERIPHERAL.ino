/* SPI_PERIPHERAL
   From: https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
   Modified by Forrest Lee Erickson 20220523
   Change to Controller/Peripheral termonology
   Change variable names for start with lowercase. Constants to uper case.
   Peripherial Arduino Code:
*/


//SPI PERIPHERAL (ARDUINO UNO)
//SPI COMMUNICATION BETWEEN TWO ARDUINO UNOs
//CIRCUIT DIGEST

//Pramoth.T
#include<SPI.h>
#define LED_PIN 7
#define BUTTON_PIN 2

volatile boolean isReceived_SPI;
volatile byte peripheralReceived,peripheralSend;
int buttonValue;
int x;

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN,INPUT);               // Setting pin 2 as INPUT
  pinMode(LED_PIN,OUTPUT);                 // Setting pin 7 as OUTPUT
  pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Controller IN 
  SPCR |= _BV(SPE);                       //Turn on SPI in Peripheral Mode
  isReceived_SPI = false;
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
}//end setup()


ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
  peripheralReceived = SPDR;         // Value received from controller if store in variable peripheralReceived
  isReceived_SPI = true;                        //Sets isReceived_SPI as True 
}//end ISR


void loop()
{ if(isReceived_SPI)                            //Logic to SET LED ON OR OFF depending upon the value recerived from controller
   {
      if (peripheralReceived==1) 
      {
        digitalWrite(LED_PIN,HIGH);         //Sets pin 7 as HIGH LED ON
        Serial.println("Peripheral LED ON");
      }else
      {
        digitalWrite(LED_PIN,LOW);          //Sets pin 7 as LOW LED OFF
        Serial.println("Peripheral LED OFF");
      }
      buttonValue = digitalRead(BUTTON_PIN);  // Reads the status of the pin 2
      if (buttonValue == HIGH)               //Logic to set the value of x to send to controller
      {
        x=1;
      }else
      {
        x=0;
      }
  peripheralSend=x;                             
  SPDR = peripheralSend;                           //Sends the x value to controller via SPDR 

  delay(1000);

}// end received
}//end loop()
