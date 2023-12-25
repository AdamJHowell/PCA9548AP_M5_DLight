/**
 * PCA9548AP_PortScanner.ino - I2C bus scanner for Arduino
 * This should work for the TCA series as well.
 *
 * Based on: https://learn.adafruit.com/adafruit-pca9548-8-channel-stemma-qt-qwiic-i2c-multiplexer/arduino
 * Based on: https://playground.arduino.cc/Main/I2cScanner/
 */
#include <M5_DLight.h>
#include "Wire.h"

#define PCA_ADDRESS 0x70

M5_DLight sensor0;
M5_DLight sensor1;
M5_DLight sensor2;
M5_DLight sensor3;
unsigned long loopDelay = 5000;					// The maximum value of 4,294,967,295 allows for a delay of about 49.7 days.
unsigned long lastLoop = 0;						// Holds the time when the most recent loop completed.
unsigned long loopCount = 0;
const char * sketchName = "PCA9548AP_PortScanner";	// The name of this sketch.
int startPort = 0x00;
int endPort = 0x77;
uint16_t lux;


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
  pcaSelect( 0 );
  sensor0.begin();
  // CONTINUOUSLY_H_RESOLUTION_MODE
  // CONTINUOUSLY_H_RESOLUTION_MODE2
  // CONTINUOUSLY_L_RESOLUTION_MODE
  // ONE_TIME_H_RESOLUTION_MODE
  // ONE_TIME_H_RESOLUTION_MODE2
  // ONE_TIME_L_RESOLUTION_MODE
  sensor0.setMode( CONTINUOUSLY_H_RESOLUTION_MODE );
  pcaSelect( 1 );
  sensor1.begin();
  sensor1.setMode( CONTINUOUSLY_H_RESOLUTION_MODE );
  pcaSelect( 2 );
  sensor2.begin();
  sensor2.setMode( CONTINUOUSLY_H_RESOLUTION_MODE );
  pcaSelect( 3 );
  sensor3.begin();
  sensor3.setMode( CONTINUOUSLY_H_RESOLUTION_MODE );
  Serial.println( "\nI2C scanner and lux sensor are ready!" );
}


void loop() 
{
	unsigned long time = millis();
	if( ( lastLoop == 0 ) || ( time - lastLoop ) > loopDelay )
	{
		Serial.printf( "\n%s will scan the I2C bus for device addresses in the range of 0x%X to 0x%X.\n", sketchName, startPort, endPort );
		loopCount++;
    // Iterate through every port on the PCA9548AP.
    for( uint8_t port = 0; port < 8; port++ ) 
    {
      pcaSelect( port );
      Serial.printf( "PCA Port #%d\n", port );

      lux = sensor0.getLUX();
      Serial.printf( "Port %d lux: %d\n", port, lux );

      // Iterate through every potentially valid I2C address.
      for( uint8_t addr = 0; addr <= 127; addr++ ) 
      {
        if( addr == PCA_ADDRESS )
          continue;
        Wire.beginTransmission( addr );
        if( !Wire.endTransmission() )
          Serial.printf( "  Port %d has an I2C device at address 0x%X\n", port, addr );
      }
    }
    Serial.printf( "Completed %lu scans.  Next scan in %lu seconds.\n\n", loopCount, loopDelay / 1000 );
		lastLoop = millis();
  }
}
