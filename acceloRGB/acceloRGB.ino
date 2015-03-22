#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO

void setup(){
  // join I2C bus (I2Cdev library doesn’t do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(38400);
  // initialize device
  Serial.println("Initializing I2C devices…");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections…");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}


void loop(){
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int x=ax/100;
  int y=ay/100;
  int z=az/100;

  Serial.print(x); 
  Serial.print(",");

 /* Serial.print(y); 
  Serial.print(",");

  Serial.print(z); 
  Serial.print(",");
  */
  Serial.println();
  
  delay(1000);
}

