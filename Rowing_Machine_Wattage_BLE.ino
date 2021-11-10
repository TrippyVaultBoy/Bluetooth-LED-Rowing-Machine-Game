/*
  PM5 Rowing Machine Wattage

  This example scans for BLE peripherals until a PM5 Rowing Machine is discovered.
  It then connects to it, discovers the attributes of the 0x0030 service,
  subscribes to the Simple Key Characteristic (UUID 0x0036). When the wattage changes 
  a notification is received and the new wattage is
  outputted to the Serial Monitor.

  The circuit:
  - Arduino Nano 33 IoT,
  - PM5 Rowing Machine
*/

#include <ArduinoBLE.h>
#include <FastLED.h>

#define NUM_LEDS  144
#define LED_PIN   2
CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(144);
  
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  Serial.println("BLE Central - PM5 Concept2");
  Serial.println("Make sure to turn on the device.");

  // start scanning for peripheral
  BLE.scan();
}

//-------------------------------------------------------------------

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  int watts = 0;

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // Check if the peripheral is a PM5, the local name will be:
    //GYM PM5 ID 430660404
      
    //MOBILE PM5 430254313
    
    //PM5 430685081
    
    // GAME1 PM5 430974704

    //GAME2 PM5 ID 430395212 Row
    
    if (peripheral.localName() == "PM5 430974707") {
      // stop scanning
      BLE.stopScan();

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      //Update the wattage reading
      monitorWattage(peripheral);

      // peripheral disconnected, start scanning again
      BLE.scan();
    }
  }
}

//-------------------------------------------------------------------

void monitorWattage(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");
  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering service 0x0036 ...");
  if (peripheral.discoverService("CE060030-43E5-11E4-916C-0800200C9A66")) {
    Serial.println("Service discovered");
  } else {
    Serial.println("Attribute discovery failed.");
    peripheral.disconnect();

    while (1);
    return;
  }

  // retrieve the watts characteristic
  BLECharacteristic simpleKeyCharacteristic = peripheral.characteristic("CE060036-43E5-11E4-916C-0800200C9A66");

  // subscribe to the simple key characteristic
  Serial.println("Subscribing to Watts characteristic ...");
  if (!simpleKeyCharacteristic) {
    Serial.println("No Watts characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!simpleKeyCharacteristic.canSubscribe()) {
    Serial.println("Watts characteristic is not subscribable!");
    peripheral.disconnect();
    return; 
  } else if (!simpleKeyCharacteristic.subscribe()) {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed");
    Serial.println("Start Rowing!");
  }

  while (peripheral.connected()) {
    // while the peripheral is connected

    // check if the value of the wattage characteristic has been updated
    if (simpleKeyCharacteristic.valueUpdated()) {
      // yes, get the value, characteristic is 1 byte so use byte value
      
      byte rowData[15];
      
      simpleKeyCharacteristic.readValue(rowData, 15);

//      Serial.print("-{ ");
//      Serial.print(rowData[0]);
//        Serial.print(", ");
//      Serial.print(rowData[1]);
//       Serial.print(", ");
//      Serial.print(rowData[2]);
//       Serial.print(", ");
//      Serial.print(rowData[3]);
//       Serial.print(", ");
//      Serial.print(rowData[4]);
//       Serial.print(", ");
//      Serial.print(rowData[5]);
//       Serial.print(", ");
//      Serial.print(rowData[6]);
//       Serial.print(", ");
//      Serial.print(rowData[7]);
//       Serial.print(", ");
//      Serial.print(rowData[8]);
//       Serial.print(", ");
//      Serial.print(rowData[9]);
//       Serial.print(", ");
//      Serial.print(rowData[10]);
//       Serial.print(", ");
//      Serial.print(rowData[11]);
//       Serial.print(", ");
//      Serial.print(rowData[12]);
//       Serial.print(", ");
//      Serial.print(rowData[13]);
//       Serial.print(", ");
//      Serial.print(rowData[14]);
//       Serial.print(", ");
//      Serial.print(rowData[15]);
//       Serial.println(" }");

       Serial.println(" ");

      int lowBit = rowData[3];
      int hiBit = rowData[4];

      long watts = hiBit * 256 + lowBit;
      
      Serial.print("Watts: ");
      Serial.println(watts);

      //Update LED position / Choose Easy Mode(150w), Normal Mode(250w) 
      //                      Hard Mode(350w), ExtraHard(550), Insane Mode(800w)
      
        modeEasy(watts);

      //modeNormal(watts);
      
      //modeHard(watts);

      //modeExtraHard(watts);

      //modeInsane(watts);

    }
  }

  Serial.println("PM5 disconnected!");
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


void modeEasy(long watts) {

  Serial.print("-[Easy Mode]");

    if((watts > 0) && (watts < 50)) {
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, (watts * 0.96) , CRGB::Green);
      FastLED.show();
      
      Serial.println("Low Wattage");
      
    }

    if((watts >= 50) && (watts < 100)) {

      int wattsRemainder = watts - 50; 
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, ((50 + wattsRemainder) * 0.96) , CRGB::Blue);
      FastLED.show();

      Serial.println("Medium Wattage");
      
    }
    
    if((watts >= 100) && (watts < 1000)) {

      
      int wattsRemainder = watts - 100;
      // fill 10 pixels, starting at leds[5]
      
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();  
      
      fill_solid( leds, ((100 + wattsRemainder) * .96) , CRGB::White);
      FastLED.show();

      Serial.println("High Wattage");
      
    }
    
}   

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


void modeNormal(long watts) {

  Serial.print("-[Normal Mode]"); 

    if((watts > 0) && (watts < 83)) {
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, watts * 0.57, CRGB::Green);
      FastLED.show();
      
      Serial.println("Low Wattage");
      
    }

    if((watts >= 116) && (watts < 166)) {

      int wattsRemainder = watts - 166; 
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, (166 + wattsRemainder) * 0.57, CRGB::Blue);
      FastLED.show();

      Serial.println("Medium Wattage");
      
    }
    
    if((watts >= 249) && (watts < 1000)) {

      
      int wattsRemainder = watts - 249;
      // fill 10 pixels, starting at leds[5]
      
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();  
      
      fill_solid( leds, (249 + wattsRemainder) * 0.57, CRGB::White);
      FastLED.show();

      Serial.println("High Wattage");
      
    }
    
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

void modeHard(long watts) {

  Serial.print("-[Hard Mode]"); 

    if((watts > 0) && (watts < 116)) {
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, watts * 0.41, CRGB::Green);
      FastLED.show();
      
      Serial.println("Low Wattage");
      
    }

    if((watts >= 116) && (watts < 232)) {

      int wattsRemainder = watts - 532; 
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, (532 + wattsRemainder) * 0.41 , CRGB::Blue);
      FastLED.show();

      Serial.println("Medium Wattage");
      
    }
    
    if((watts >= 232) && (watts < 1000)) {

      
      int wattsRemainder = watts - 532;
      // fill 10 pixels, starting at leds[5]
      
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();  
      
      fill_solid( leds, (532 + wattsRemainder) * 0.41, CRGB::White);
      FastLED.show();

      Serial.println("High Wattage");
      
    }
    
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

void modeExtraHard(long watts) {

  Serial.print("-[Hard Mode]"); 

    if((watts > 0) && (watts < 183)) {
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, watts * 0.26, CRGB::Green);
      FastLED.show();
      
      Serial.println("Low Wattage");
      
    }

    if((watts >= 183) && (watts < 366)) {

      int wattsRemainder = watts - 183; 
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, (183 + wattsRemainder) * 0.26, CRGB::Blue);
      FastLED.show();

      Serial.println("Medium Wattage");
      
    }
    
    if((watts >= 366) && (watts < 1000)) {

      
      int wattsRemainder = watts - 366;
      // fill 10 pixels, starting at leds[5]
      
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();  
      
      fill_solid( leds, (366 + wattsRemainder) * 0.26, CRGB::White);
      FastLED.show();

      Serial.println("High Wattage");
      
    }
    
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

void modeInsane(long watts) {

  Serial.print("-[Hard Mode]"); 

    if((watts > 0) && (watts < 266)) {
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, watts * 0.18, CRGB::Green);
      FastLED.show();
      
      Serial.println("Low Wattage");
      
    }

    if((watts >= 266) && (watts < 532)) {

      int wattsRemainder = watts - 532; 
      // fill 10 pixels, starting at leds[5]

      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      
      fill_solid( leds, (532 + wattsRemainder) * 0.18 , CRGB::Blue);
      FastLED.show();

      Serial.println("Medium Wattage");
      
    }
    
    if((watts >= 532) && (watts < 1000)) {

      
      int wattsRemainder = watts - 532;
      // fill 10 pixels, starting at leds[5]
      
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();  
      
      fill_solid( leds, (532 + wattsRemainder) * 0.18, CRGB::White);
      FastLED.show();

      Serial.println("High Wattage");
      
    }
    
}
