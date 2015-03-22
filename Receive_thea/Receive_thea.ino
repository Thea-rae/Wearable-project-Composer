// Simple serial pass through program
// It initializes the RFM12B radio with optional encryption and passes through any valid messages to the serial port
// felix@lowpowerlab.com

#include <RFM12B.h>

// You will need to initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID           1  //network ID used for this unit
#define NETWORKID       99  //the network ID we are on
#define SERIAL_BAUD 115200

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "ABCDABCDABCDABCD";

// Need an instance of the Radio Module
RFM12B radio;
void setup()
{
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  radio.Encrypt(KEY);      //comment this out to disable encryption
  Serial.begin(SERIAL_BAUD);
  Serial.println("Listening...");
}

typedef struct {  // Message data Structure, this must match Tx
  int A;	  // floating point measurement value
  int B; //NODE ID of sender
  int C,D,E,F,G;
} 
Payload;
Payload sample;  // declare an instance of type Payload named sample


void loop() {
  if (radio.ReceiveComplete()) {
    if (radio.CRCPass()) {
      Serial.print(radio.GetSender());
      Serial.print(": ");
      sample = *(Payload*)rf12_data;  // Access the payload 
      //set payload values to stable variables. Not needed, but makes me feel better 
      int sampleA = sample.A;
      int sampleB = sample.B;
      int sampleC = sample.C;
      int sampleD = sample.D;
      int sampleE = sample.E;
      int sampleF = sample.F;
      int sampleG = sample.G;

      Serial.print(sampleA);
      Serial.print(" ");
      Serial.print(sampleB);
      Serial.print(" ");
      Serial.print(sampleC);
      Serial.print(" ");
      Serial.print(sampleD);
      Serial.print(" ");
      Serial.print(sampleE);
      Serial.print(" ");
      Serial.print(sampleF);
      Serial.print(" ");
      Serial.print(sampleG);
      
      if (radio.ACKRequested())
      {
        radio.SendACK();
      
      }
    }
    else
      Serial.print("BAD-CRC");

    Serial.println();
  }
}






