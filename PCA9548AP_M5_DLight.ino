/**
 * PCA9548AP_PortScanner.ino - I2C bus scanner for Arduino
 * This should work for the TCA series as well.
 *
 * Based on: https://learn.adafruit.com/adafruit-pca9548-8-channel-stemma-qt-qwiic-i2c-multiplexer/arduino
 * Based on: https://playground.arduino.cc/Main/I2cScanner/
 */
#include <M5_DLight.h>
#include "Wire.h"


unsigned long lastLoop = 0;						// Holds the time when the most recent loop completed.
unsigned long loopCount = 0;
const unsigned long loopDelay = 5000;					// The maximum value of 4,294,967,295 allows for a delay of about 49.7 days.
const char * sketchName = "PCA9548AP_PortScanner";	// The name of this sketch.
const int PCA_ADDRESS = 0x70;
const int startPort = 0x00;
const int endPort = 0x77;
const unsigned int numSensors = 4;
uint16_t luxArray[4];
M5_DLight sensorArray[4];
unsigned int sensorAddresses[4];// = {0, 1, 4, 5};


void pcaSelect( uint8_t i )
{
  if( i > 7 )
    return;
  Wire.beginTransmission( PCA_ADDRESS );
  Wire.write( 1 << i );
  Wire.endTransmission();
}


void setup()
{
  Serial.begin( 115200 );
  if( !Serial );
    delay( 1000 );

  Wire.begin();

  sensorAddresses[0] = 0;
  sensorAddresses[1] = 1;
  sensorAddresses[2] = 4;
  sensorAddresses[3] = 5;
  /*
   * Sensor modes:
   * CONTINUOUSLY_H_RESOLUTION_MODE
   * CONTINUOUSLY_H_RESOLUTION_MODE2
   * CONTINUOUSLY_L_RESOLUTION_MODE
   * ONE_TIME_H_RESOLUTION_MODE
   * ONE_TIME_H_RESOLUTION_MODE2
   * ONE_TIME_L_RESOLUTION_MODE
   */
  for( uint8_t i = 0; i < numSensors; i++ )
  {
    pcaSelect( sensorAddresses[i] );
    sensorArray[i].begin();
    sensorArray[i].setMode( CONTINUOUSLY_H_RESOLUTION_MODE );
  }
  Serial.println( "\nI2C scanner and lux sensor are ready!" );
}


void loop() 
{
	unsigned long time = millis();
	if( ( lastLoop == 0 ) || ( time - lastLoop ) > loopDelay )
	{
		Serial.printf( "\n%s will scan the DLight sensors.\n", sketchName );
		loopCount++;

    for( uint8_t i = 0; i < numSensors; i++ )
    {
      pcaSelect( sensorAddresses[i] );
      luxArray[i] = sensorArray[i].getLUX();
    }
    for( uint8_t i = 0; i < numSensors; i++ )
    {
      Serial.printf( "PCA Port # %d, lux: %d\n", sensorAddresses[i], luxArray[i] );
    }
    Serial.printf( "Completed %lu scans.  Next scan in %lu seconds.\n\n", loopCount, loopDelay / 1000 );
		lastLoop = millis();
  }
}
