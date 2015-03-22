// Simple RFM12B sender program, with ACK and optional encryption
// It initializes the RFM12B radio with optional encryption and passes through any valid messages to the serial port
// felix@lowpowerlab.com
#include "I2Cdev.h"


#include <RFM12B.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif


// You will need to initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     50  // # of ms to wait for an ack
#define SERIAL_BAUD  115200

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "ABCDABCDABCDABCD";

int interPacketDelay = 250; //wait this many ms between sending packets

// Need an instance of the Radio Module
RFM12B radio;
bool requestACK=false;

int sensorPin1 = A0;
int sensorPin = A1;
int flexValue = 0;
int forceValue = 0;

void setup(){
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif


  Serial.begin(SERIAL_BAUD);
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  radio.Encrypt(KEY);
  radio.Sleep(); //sleep right away to save power
  Serial.println("Transmitting...\n\n");
}

//this is our data packet. We're sending the nodeID and the analogRead Value.
typedef struct {  // Message data Structure, this must match Tx
  int A;  
  int B; 
  int C,D,E,F,G;
} 
Payload;
Payload sample;  // declare an instance of type Payload named sample (this is referenced as sample from now on)

void loop(){

  flexValue = analogRead(sensorPin);
  forceValue = analogRead(sensorPin1);
  sample.A = flexValue;
  sample.B = forceValue;
  sample.C = 0;
  sample.D = 0;
  sample.E = 0;
  sample.F = 0;
  sample.G = 0;

  radio.Send(GATEWAYID, &sample, sizeof sample);

  Serial.println(sample.A);
  Serial.println(sample.B);
  Serial.println(sample.C);
  Serial.println(sample.D);
  Serial.println(sample.E);
  Serial.println(sample.F);
  Serial.println(sample.G);
  Serial.println();
  delay(interPacketDelay);
}

// wait a few milliseconds for proper ACK, return true if received
static bool waitForAck() {
  long now = millis();
  while (millis() - now <= ACK_TIME)
    if (radio.ACKReceived(GATEWAYID))
      return true;
  return false;
}





