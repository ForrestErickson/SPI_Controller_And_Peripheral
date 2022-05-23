/* SPI_PERIPHERAL
   From: https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
   Modified by Forrest Lee Erickson 20220523
   Change to Controller/Peripheral termonology
   Peripherial Arduino Code:
*/


//SPI PERIPHERAL (ARDUINO UNO)
//SPI COMMUNICATION BETWEEN TWO ARDUINO UNOs
//CIRCUIT DIGEST

//Pramoth.T
#include<SPI.h>
#define LEDpin 7
#define buttonpin 2

volatile boolean received;
volatile byte Peripheralreceived,Peripheralsend;
int buttonvalue;
int x;

void setup()
{
  Serial.begin(115200);
  pinMode(buttonpin,INPUT);               // Setting pin 2 as INPUT
  pinMode(LEDpin,OUTPUT);                 // Setting pin 7 as OUTPUT
  pinMode(MISO,OUTPUT);                   //Sets MISO as OUTPUT (Have to Send data to Controller IN 
  SPCR |= _BV(SPE);                       //Turn on SPI in Peripheral Mode
  received = false;
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
}//end setup()


ISR (SPI_STC_vect)                        //Inerrrput routine function 
{
  Peripheralreceived = SPDR;         // Value received from controller if store in variable Peripheralreceived
  received = true;                        //Sets received as True 
}//end ISR


void loop()
{ if(received)                            //Logic to SET LED ON OR OFF depending upon the value recerived from controller
   {
      if (Peripheralreceived==1) 
      {
        digitalWrite(LEDpin,HIGH);         //Sets pin 7 as HIGH LED ON
        Serial.println("Peripheral LED ON");
      }else
      {
        digitalWrite(LEDpin,LOW);          //Sets pin 7 as LOW LED OFF
        Serial.println("Peripheral LED OFF");
      }
      buttonvalue = digitalRead(buttonpin);  // Reads the status of the pin 2
      if (buttonvalue == HIGH)               //Logic to set the value of x to send to controller
      {
        x=1;
      }else
      {
        x=0;
      }
  Peripheralsend=x;                             
  SPDR = Peripheralsend;                           //Sends the x value to controller via SPDR 

  delay(1000);

}// end received
}//end loop()
