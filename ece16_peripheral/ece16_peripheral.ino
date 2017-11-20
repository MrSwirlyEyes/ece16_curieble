#include <CurieBLE.h>

// BLE Accelerometer Service
BLEService accelerometer_service("ECE16-37");

// BLE Accelerometer Service Characteristics
BLEIntCharacteristic accelerometer_characteristic_x("ECE16-37-01", BLERead | BLEWrite);
BLEIntCharacteristic accelerometer_characteristic_y("ECE16-37-02", BLERead | BLEWrite);
BLEIntCharacteristic accelerometer_characteristic_z("ECE16-37-03", BLERead | BLEWrite);


void setup() {
  // Start Serial Monitor
  Serial.begin(9600);

  // Initialize the CurieBLE
  BLE.begin();

  // Set a name for the Service we are broadcasting
  BLE.setLocalName("Accelerometer");

  // Advertise this Service
  BLE.setAdvertisedService(accelerometer_service);

  // Add Characteristics to the Service
  accelerometer_service.addCharacteristic(accelerometer_characteristic_x);
  accelerometer_service.addCharacteristic(accelerometer_characteristic_y);
  accelerometer_service.addCharacteristic(accelerometer_characteristic_z);
  
  // Add the Service
  BLE.addService(accelerometer_service);

  // Set initial Value for Characteristics
  accelerometer_characteristic_x.setValue(0);
  accelerometer_characteristic_y.setValue(0);
  accelerometer_characteristic_z.setValue(0);
  

  // Start the broadcast
  BLE.advertise();

  Serial.println("BLE Accelerometer Peripheral");
}

void loop() {
  // Set up a Central to connect to us/Peripheral
  BLEDevice central=BLE.central();

  // If a central has connected to us/Peripheral:
  if(central) {
    Serial.print("A central has connected: ");
    Serial.println(central.address());

    // While Central is still connected to us/Peripheral:
    while(central.connected()) {
  
      // Check if Central has updated/written a new value for Characteristic(s)
      if(accelerometer_characteristic_x.written() ||
          accelerometer_characteristic_y.written() ||
          accelerometer_characteristic_z.written()) {
        Serial.print("(");
        Serial.print(accelerometer_characteristic_x.value()); Serial.print(",");
        Serial.print(accelerometer_characteristic_y.value()); Serial.print(",");
        Serial.print(accelerometer_characteristic_z.value()); Serial.println(")");
      }
    }
  
    // Otherwise, Central has disconnected!
    Serial.print("Central has disconnected: ");
    Serial.println(central.address());
  }
}
