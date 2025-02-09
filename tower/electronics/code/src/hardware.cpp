#include "Arduino.h" // DEBUG
#include "hardware.h"
#include "encoders.h"
#include "sensors.h"

// ========== Define hardware addresses ==========
#define SEGLEFT_ADDR (0x70)
#define SEGRIGHT_ADDR (0x71)
#define IMU_ADDR (0x6A)
#define GPS_ADDR (0x42)
#define NEOROTARY_ADDR (0x49)
#define CAN_ADDR (0x48)
#define COMPASS_ADDR (0xD)

// ========== Define global flags ==========
bool found7SegLeft  = false;
bool found7SegRight = false;
bool foundIMU       = false;
bool foundGPS       = false;
bool foundQuadEnc   = false;
bool foundCAN       = false;
bool foundCompass   = false;

// ========== Encoder definitions ==========
//static const uint8_t quadEncSwitch[4]  = { 12, 14, 17, 9 };
#define SS_NEO_PIN    18     // I think this is only used for the NeoPixel
#define ENC0_SWITCH   12
#define ENC1_SWITCH   14
#define ENC2_SWITCH   17
#define ENC3_SWITCH   9

//int32_t enc_positions[4] = {0, 0, 0, 0};

// ========== Instantiate hardware objects ==========
// 7-segment displays
Adafruit_7segment segLeft  = Adafruit_7segment();
Adafruit_7segment segRight = Adafruit_7segment();

// IMU
Adafruit_ISM330DHCX imu = Adafruit_ISM330DHCX(); 

// GPS
SFE_UBLOX_GNSS myGNSS;

// NeoRotary
Adafruit_seesaw quadEnc = Adafruit_seesaw(&Wire);

// Compass
//QMC5883L compass;

// -----------------------------------------------------
// initAllHardware()
// -----------------------------------------------------
// Called in setup() to begin I2C, then attempt to init each device.
// -----------------------------------------------------
void initAllHardware()
{
  // If you use the default Wire pins, just call:
  Serial.println("initAll");
  Wire.begin(); 
  Wire.setClock(400000);  // optional: set 400kHz for faster I2C if supported

  // ---------- 7-Segment Left (0x70) ----------
  if (segLeft.begin(SEGLEFT_ADDR))
  {
    found7SegLeft = true;
    Serial.println("Found segLeft!");
    segLeft.clear();
    segLeft.writeDisplay();
  }
  else
  {
    found7SegLeft = false;
    Serial.println("segLeft NOT found!");
  }

  // ---------- 7-Segment Right (0x71) ----------
  if (segRight.begin(SEGRIGHT_ADDR))
  {
    found7SegRight = true;
    Serial.println("Found segRight!");
    segRight.clear();
    segRight.writeDisplay();
  }
  else
  {
    found7SegRight = false;
    Serial.println("segRight NOT found!");
  }

  // ---------- IMU (ISM330DHCX) ----------
  // This calls begin_I2C on the default Wire or you can pass a different TwoWire
  if (imu.begin_I2C()) 
  {
    foundIMU = true;
    Serial.println("Found IMU!");
    // Optional: configure ranges or power modes
    // e.g., imu.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
    //       imu.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS);
  }
  else
  {
    foundIMU = false;
    Serial.println("IMU NOT found!");
  }

  // ---------- IMU (ISM330DHCX) ----------
  // This calls begin_I2C on the default Wire or you can pass a different TwoWire
  /*compass.init();
  if (compass.ready())
  {
    foundCompass = true;
    Serial.println("Found compass!");
  }
  else
  {
    foundCompass = false;
    Serial.println("Compass NOT found!");
  }*/

  // ---------- GPS (SparkFun Ublox) ----------
  // The SparkFun library can auto-detect on the default Wire:
  if (myGNSS.begin(Wire))
  {
    foundGPS = true;
    Serial.println("Found GPS!");
    
    // Enable UBX messages
    myGNSS.setI2COutput(COM_TYPE_UBX);

    // Change update rate
    myGNSS.setNavigationFrequency(5);     // 5 Hz updates, for instance
    
    // Configure Odometer
    myGNSS.enableOdometer();
    uint8_t flags;        // Odometer/Low-speed COG filter flags
    uint8_t odoCfg;       // Odometer filter settings
    uint8_t cogMaxSpeed;  // Speed below which course-over-ground (COG) is computed with the low-speed COG filter : m/s * 0.1
    uint8_t cogMaxPosAcc; // Maximum acceptable position accuracy for computing COG with the low-speed COG filter
    uint8_t velLpGain;    // Velocity low-pass filter level
    uint8_t cogLpGain;    // COG low-pass filter level
    myGNSS.enableDebugging(Serial);
    Serial.println("Get cfg");
    if (myGNSS.getOdometerConfig(&flags, &odoCfg, &cogMaxSpeed, &cogMaxPosAcc, &velLpGain, &cogLpGain))
    {
      flags = UBX_CFG_ODO_USE_ODO; // Enable the odometer
      odoCfg = UBX_CFG_ODO_CAR; // Use the car profile (others are RUN, CYCLE, SWIM, CUSTOM)
      Serial.println("Set cfg");
      myGNSS.setOdometerConfig(flags, odoCfg, cogMaxSpeed, cogMaxPosAcc, velLpGain, cogLpGain); // Set the configuration
      Serial.println("Odometer configuration successful");
    }
    else {
      Serial.println("Error initializing Odometer");
    }
    myGNSS.disableDebugging();
    myGNSS.setAutoNAVODOcallbackPtr(&callbackODO);
    myGNSS.setAutoPVTcallbackPtr(&callbackPVT);
    
    //Serial.println(myGNSS.getHeading());
  }
  else
  {
    foundGPS = false;
    Serial.println("GPS NOT found!");
  }

  // ---------- NeoRotary 4 ----------
  // This is a bit more complicated so it's getting its own file
  if (initEncoders()) {
    Serial.println("Good from within hardware");
    foundQuadEnc = true;
  }
  else
  {
    Serial.println("NeoRotary NOT found!");
  }
}

// -----------------------------------------------------
// checkAllHardware() -- TODO
// -----------------------------------------------------
// Optional function to periodically test if devices are still connected
// and attempt re-init if not found.
// -----------------------------------------------------
void checkAllHardware()
{
  // Example 7-seg check: see if a write succeeds
  if (found7SegLeft)
  {
    Wire.beginTransmission(SEGLEFT_ADDR);
    if (Wire.endTransmission() != 0)
    {
      // Device not responding
      found7SegLeft = false;
      Serial.println("Lost segLeft!");
    }
  }

  // Similarly for segRight, IMU, GPS, etc.
  // ...
}