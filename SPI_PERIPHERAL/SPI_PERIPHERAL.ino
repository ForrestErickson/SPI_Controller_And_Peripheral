/* SPI_PERIPHERAL
   From: https://circuitdigest.com/microcontroller-projects/arduino-spi-communication-tutorial
   Modified by Forrest Lee Erickson 20220523
   Change to Controller/Peripheral termonology
   Change variable names for start with lowercase. Constants to upper case.
   Peripherial Arduino Code:
   License: Dedicated to the Public Domain
   Warrenty: This program is designed to kill and render the earth uninhabitable,
   however it is not guaranteed to do so.
   20220524 Get working with the SPI_CONTROLER sketch. Made function updateFromSPI().
   20220925 Changes for GPAD Version 1 PCB.  SS on pin 7 and LED_PIN on D3.
   20220927 Change back for GPAD nCS on Pin 10
*/

//SPI PERIPHERAL (ARDUINO UNO)
//SPI COMMUNICATION BETWEEN TWO ARDUINO UNOs
//CIRCUIT DIGEST

/* Hardware Notes Peripheral
   SPI Line Pin in Arduino, IO setup
  MOSI 11 or ICSP-4  Input
  MISO 12 or ICSP-1 Output
  SCK 13 or ICSP-3  Input
  SS 10 Input
*/

#define GPAD_VERSION1

#ifdef GPAD_VERSION1 //The Version 1 PCB.
//#define SS 7                                // nCS aka /SS Input on GPAD Version 1 PCB.
#define LED_PIN PD3                         // for GPAD LIGHT0
#define BUTTON_PIN PD2                      //GPAD Button to GND,  10K Resistor to +5V.

#else //The proof of concept wiring.
#define LED_PIN 7
#define BUTTON_PIN 2                          //Button to GND, 10K Resistor to +5V.

#endif

#include<SPI.h>

volatile boolean isReceived_SPI;
volatile byte peripheralReceived ;

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting SPI Peripheral.");
  Serial.print("Pin for SS: ");
  Serial.println(SS);

  pinMode(BUTTON_PIN, INPUT);              // Setting pin 2 as INPUT
  pinMode(LED_PIN, OUTPUT);                // Setting pin 7 as OUTPUT

  //  SPI.begin();    // IMPORTANT. Do not set SPI.begin for a peripherial device.
  pinMode(SS, INPUT_PULLUP); //Sets SS as input for peripherial
  pinMode(MOSI, OUTPUT);    //This works for Peripheral
  pinMode(MISO, OUTPUT);    //try this.
  pinMode(SCK, INPUT);                  //Sets clock as input
  SPCR |= _BV(SPE);                       //Turn on SPI in Peripheral Mode
  isReceived_SPI = false;
  SPI.attachInterrupt();                  //Interuupt ON is set for SPI commnucation
}//end setup()

//ISRs
ISR (SPI_STC_vect)                        //Inerrrput routine function
{
  peripheralReceived = SPDR;         // Value received from controller if store in variable peripheralReceived
  isReceived_SPI = true;                        //Sets isReceived_SPI as True
}//end ISR

//Functions
void updateFromSPI() {
  volatile byte peripheralSend;

  if (isReceived_SPI)                           //Logic to SET LED ON OR OFF depending upon the value recerived from controller
  { // Act on the received data.
    if (peripheralReceived == 1)
    {
      digitalWrite(LED_PIN, HIGH);        //Sets pin 7 as HIGH LED ON
      //          Serial.println("Peripheral LED ON");
    } else
    {
      digitalWrite(LED_PIN, LOW);         //Sets pin 7 as LOW LED OFF
      //          Serial.println("Peripheral LED OFF");
    }

    // Send return SPI data. Lets use a button for this example.
    peripheralSend = digitalRead(BUTTON_PIN);
    SPDR = peripheralSend;    //SPDR register of data to be shiffted out.

    Serial.print("SPI_PERIPHERAL, isReceived_SPI: ");
    Serial.println(peripheralReceived);
  }// end received
}//end function updateFromSPI

void loop()
{
  updateFromSPI();

//Check mute button for press.
    if (digitalRead(BUTTON_PIN) != HIGH) {
  //  tone(TONE_PIN, BUZZER_TEST_FREQ);
    Serial.println("Button pressed.");
//    for (int i = 0; i < 5; i++) {
//      digitalWrite(LIGHT[i], HIGH);
//    }
  } else {
//    noTone(TONE_PIN);
//    for (int i = 0; i < 5; i++) {
//      digitalWrite(LIGHT[i], LOW);
//    }
  }
  
}//end loop()
