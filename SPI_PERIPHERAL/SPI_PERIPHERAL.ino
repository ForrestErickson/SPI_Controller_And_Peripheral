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

/* Hardware Notes
   SPI Line Pin in Arduino
  MOSI 11 or ICSP-4
  MISO 12 or ICSP-1
  SCK 13 or ICSP-3
  SS 10
*/

//Pramoth.T
#include<SPI.h>
#define LED_PIN 7
//#define BUTTON_PIN 2                          //Button to +5V, 10K Resistor to GND
#define BUTTON_PIN 2                          //Button to GND, 10K Resistor to +5V.

volatile boolean isReceived_SPI;
volatile byte peripheralReceived, peripheralSend;
int buttonValue;
int x;

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting SPI Peripheral.");

  pinMode(BUTTON_PIN, INPUT);              // Setting pin 2 as INPUT
  pinMode(LED_PIN, OUTPUT);                // Setting pin 7 as OUTPUT

  //  SPI.begin();    // IMPORTANT. Do not set SPI.begin for a peripherial device.
  pinMode(SS, INPUT_PULLUP);                    //Sets SS as input for peripherial
  pinMode(MOSI, OUTPUT);    //ToDo, this seams backwards.
//  pinMode(MISO, INPUT);    //ToDo, this seams backwards.
  pinMode(MISO, OUTPUT);    //try this.
  pinMode(SCK, INPUT);                  //Sets clock as input
  SPCR |= _BV(SPE);                       //Turn on SPI in Peripheral Mode
  isReceived_SPI = false;
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
}//end setup()


ISR (SPI_STC_vect)                        //Inerrrput routine function
{
  peripheralReceived = SPDR;         // Value received from controller if store in variable peripheralReceived
  isReceived_SPI = true;                        //Sets isReceived_SPI as True
//  SPDR = peripheralSend;  
}//end ISR

void updateFromSPI() {
  if (isReceived_SPI)                           //Logic to SET LED ON OR OFF depending upon the value recerived from controller
  {
        if (peripheralReceived == 1)
        {
          digitalWrite(LED_PIN, HIGH);        //Sets pin 7 as HIGH LED ON
//          Serial.println("Peripheral LED ON");
        } else
        {
          digitalWrite(LED_PIN, LOW);         //Sets pin 7 as LOW LED OFF
//          Serial.println("Peripheral LED OFF");
        }
    buttonValue = digitalRead(BUTTON_PIN);  // Reads the status of the pin 2
    if (buttonValue == HIGH)               //Logic to set the value of x to send to controller
    {
      x = 1;
    }
    else
    {
      x = 0;
    }
    peripheralSend = x;
    //    Serial.print("Peripherial Switch ");
    //    Serial.println(x);
    SPDR = peripheralSend;    //SPDR register of data to be shiffted out.

    //    delay(1000);
    Serial.println("SPI_PERIPHERAL, isReceived_SPI True.");
  }// end received

}//end updateFromSPI


void loop()
{

  updateFromSPI();
}//end loop()
