#include <CurieBLE.h>

#include <CurieIMU.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialize IMU
  CurieIMU.begin();
  Serial.println("IMU initialized...");

  // Set Accelerometer range
  CurieIMU.setAccelerometerRange(2);

  // Initialize the CurieBLE
  BLE.begin();

  Serial.println("BLE Accelerometer Central");

  // Scan/look-for a Peripheral
  BLE.scanForUuid("ECE16-37");
}

void loop() {
  // See if we found the Accelerometer Peripheral
  BLEDevice peripheral=BLE.available();

  // If a peripheral has been connected to:
  if(peripheral) {
    // We found the peripheral we were looking for!
    Serial.print("Discovered ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // No need to keep looking for anything else now
    BLE.stopScan();

    

    // Write Accelerometer readings to Peripheral's Accelerometer Service + Characteristics
    write_to_peripheral(peripheral);

    // If we disconnected with Peripheral, try again!
    BLE.scanForUuid("ECE16-37");
  }
}

void write_to_peripheral(BLEDevice peripheral) {
  // Connect to Peripheral
    Serial.println("Connecting...");

    if(peripheral.connect()) {
      Serial.println("Connected!");
    } else {
      Serial.println("Failed to connect!");    
      return; 
    }

    // See if our Peripheral has Characteristics/Attributes
    Serial.println("Discovering Characteristics/Attributes ...");
    if (peripheral.discoverAttributes()) {
      Serial.println("Characteristics/Attributes discovered");
    } else {
      Serial.println("Characteristic/Attribute discovery failed!");
      peripheral.disconnect();      
      return;  
    }
    
  // Retrieve the Accelerometer Characteristic(s) from Peripheral
  BLECharacteristic accelerometer_characteristic_x=peripheral.characteristic("ECE16-37-01");
  BLECharacteristic accelerometer_characteristic_y=peripheral.characteristic("ECE16-37-02");
  BLECharacteristic accelerometer_characteristic_z=peripheral.characteristic("ECE16-37-03");

  // While we are still connected to the Peripheral, update its IMU Characteristics
  while(peripheral.connected()) {
    int x=CurieIMU.readAccelerometer(X_AXIS);
    accelerometer_characteristic_x.writeInt(x);

    int y=CurieIMU.readAccelerometer(Y_AXIS);
    accelerometer_characteristic_y.writeInt(y);

    int z=CurieIMU.readAccelerometer(Z_AXIS);
    accelerometer_characteristic_z.writeInt(z);
  }

  Serial.println("Peripheral disconnected!");
}

