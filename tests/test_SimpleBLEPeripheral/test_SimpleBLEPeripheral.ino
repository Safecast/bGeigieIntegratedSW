/*
 * Copyright (c) 2016 RedBear
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */
 
/******************************************************
 *                      Macros
 ******************************************************/
/*
 * SYSTEM_MODE:
 *     - AUTOMATIC: Automatically try to connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     - SEMI_AUTOMATIC: Manually connect to Wi-Fi and the Particle Cloud, but automatically handle the cloud messages.
 *     - MANUAL: Manually connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     
 * SYSTEM_MODE(AUTOMATIC) does not need to be called, because it is the default state. 
 * However the user can invoke this method to make the mode explicit.
 * Learn more about system modes: https://docs.particle.io/reference/firmware/photon/#system-modes .
 */
#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif

#include <string.h>

/* 
 * BLE peripheral preferred connection parameters:
 *     - Minimum connection interval = MIN_CONN_INTERVAL * 1.25 ms, where MIN_CONN_INTERVAL ranges from 0x0006 to 0x0C80
 *     - Maximum connection interval = MAX_CONN_INTERVAL * 1.25 ms,  where MAX_CONN_INTERVAL ranges from 0x0006 to 0x0C80
 *     - The SLAVE_LATENCY ranges from 0x0000 to 0x03E8
 *     - Connection supervision timeout = CONN_SUPERVISION_TIMEOUT * 10 ms, where CONN_SUPERVISION_TIMEOUT ranges from 0x000A to 0x0C80
 */
#define MIN_CONN_INTERVAL          0x0028 // 50ms.
#define MAX_CONN_INTERVAL          0x0190 // 500ms.
#define SLAVE_LATENCY              0x0000 // No slave latency.
#define CONN_SUPERVISION_TIMEOUT   0x03E8 // 10s.

// Learn about appearance: http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
#define BLE_PERIPHERAL_APPEARANCE  BLE_APPEARANCE_UNKNOWN

#define BLE_DEVICE_NAME            "Safecast"

// Length of characteristic value.
#define CHARACTERISTIC1_MAX_LEN    20

/******************************************************
 *               Variable Definitions
 ******************************************************/
// Primary service 128-bits UUID
//static uint8_t service1_uuid[16] = { 0x71,0x3d,0x00,0x00,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e };
//static uint8_t service1_uuid[16] = { 0x06,0x79,0x78,0xac,0xb5,0x9f,0x4e,0xc9,0x9c,0x09,0x2a,0xb6,0xe5,0xbd,0xad,0x0b };  // same service as BLEBEE 2.0.0
static uint8_t service1_uuid[16] = { 0xef,0x08,0x0d,0x8c,0xc3,0xbe,0x41,0xff,0xbd,0x3f,0x05,0xa5,0xf4,0x79,0x5d,0x7f };  // same service as BLEBEE 1.0.0
// EF080D8C-C3BE-41FF-BD3F-05A5F4795D7F

// Characteristics 128-bits UUID
//static uint8_t char1_uuid[16]    = { 0x71,0x3d,0x00,0x02,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e };
//static uint8_t char2_uuid[16]    = { 0x71,0x3d,0x00,0x03,0x50,0x3e,0x4c,0x75,0xba,0x94,0x31,0x48,0xf1,0x8d,0x94,0x1e };
static uint8_t char1_uuid[16]    = { 0xA1,0xE8,0xF5,0xB1,0x69,0x6B,0x4E,0x4C,0x87,0xC6,0x69,0xDF,0xE0,0xB0,0x09,0x3B }; // RX characteristics

// GAP and GATT characteristics value
static uint8_t  appearance[2] = { 
  LOW_BYTE(BLE_PERIPHERAL_APPEARANCE), 
  HIGH_BYTE(BLE_PERIPHERAL_APPEARANCE) 
};

static uint8_t  change[4] = {
  0x00, 0x00, 0xFF, 0xFF
};

static uint8_t  conn_param[8] = {
  LOW_BYTE(MIN_CONN_INTERVAL), HIGH_BYTE(MIN_CONN_INTERVAL), 
  LOW_BYTE(MAX_CONN_INTERVAL), HIGH_BYTE(MAX_CONN_INTERVAL), 
  LOW_BYTE(SLAVE_LATENCY), HIGH_BYTE(SLAVE_LATENCY), 
  LOW_BYTE(CONN_SUPERVISION_TIMEOUT), HIGH_BYTE(CONN_SUPERVISION_TIMEOUT)
};

/* 
 * BLE peripheral advertising parameters:
 *     - advertising_interval_min: [0x0020, 0x4000], default: 0x0800, unit: 0.625 msec
 *     - advertising_interval_max: [0x0020, 0x4000], default: 0x0800, unit: 0.625 msec
 *     - advertising_type: 
 *           BLE_GAP_ADV_TYPE_ADV_IND 
 *           BLE_GAP_ADV_TYPE_ADV_DIRECT_IND 
 *           BLE_GAP_ADV_TYPE_ADV_SCAN_IND 
 *           BLE_GAP_ADV_TYPE_ADV_NONCONN_IND
 *     - own_address_type: 
 *           BLE_GAP_ADDR_TYPE_PUBLIC 
 *           BLE_GAP_ADDR_TYPE_RANDOM
 *     - advertising_channel_map: 
 *           BLE_GAP_ADV_CHANNEL_MAP_37 
 *           BLE_GAP_ADV_CHANNEL_MAP_38 
 *           BLE_GAP_ADV_CHANNEL_MAP_39 
 *           BLE_GAP_ADV_CHANNEL_MAP_ALL
 *     - filter policies: 
 *           BLE_GAP_ADV_FP_ANY 
 *           BLE_GAP_ADV_FP_FILTER_SCANREQ 
 *           BLE_GAP_ADV_FP_FILTER_CONNREQ 
 *           BLE_GAP_ADV_FP_FILTER_BOTH
 *     
 * Note:  If the advertising_type is set to BLE_GAP_ADV_TYPE_ADV_SCAN_IND or BLE_GAP_ADV_TYPE_ADV_NONCONN_IND, 
 *        the advertising_interval_min and advertising_interval_max should not be set to less than 0x00A0.
 */
static advParams_t adv_params = {
  .adv_int_min   = 0x0030,
  .adv_int_max   = 0x0030,
  .adv_type      = BLE_GAP_ADV_TYPE_ADV_IND,
  .dir_addr_type = BLE_GAP_ADDR_TYPE_PUBLIC,
  .dir_addr      = {0,0,0,0,0,0},
  .channel_map   = BLE_GAP_ADV_CHANNEL_MAP_ALL,
  .filter_policy = BLE_GAP_ADV_FP_ANY
};

// BLE peripheral advertising data
static uint8_t adv_data[] = {
  0x02,
  BLE_GAP_AD_TYPE_FLAGS,
  BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE,   
  
  0x11,
  BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE,
  0x7f, 0x5d, 0x79, 0xf4, 0xa5, 0x05, 0x3f, 0xbd, 0xff, 0x41, 0xbe, 0xc3, 0x8c, 0x0d, 0x08, 0xef
};

// BLE peripheral scan respond data
static uint8_t scan_response[] = {
  0x07,
  BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
  'B', 'L',  'E', 'B', 'e', 'e'
};

// Characteristic value handle
static uint16_t character1_handle = 0x0000;
// Buffer of characterisitc value.
static uint8_t characteristic1_data[CHARACTERISTIC1_MAX_LEN] = { 0 };

/******************************************************
 *               Function Definitions
 ******************************************************/
/**
 * @brief Connect handle.
 *
 * @param[in]  status   BLE_STATUS_CONNECTION_ERROR or BLE_STATUS_OK.
 * @param[in]  handle   Connect handle.
 *
 * @retval None
 */
void deviceConnectedCallback(BLEStatus_t status, uint16_t handle) {
  switch (status) {
    case BLE_STATUS_OK:
      Serial.println("Device connected!");
      break;
    default: break;
  }
}

/**
 * @brief Disconnect handle.
 *
 * @param[in]  handle   Connect handle.
 *
 * @retval None
 */
void deviceDisconnectedCallback(uint16_t handle) {
  Serial.println("Disconnected.");
}

/**
 * @brief Callback for reading event.
 *
 * @note  If characteristic contains client characteristic configuration,then client characteristic configration handle is value_handle+1.
 *        Now can't add user_descriptor.
 *
 * @param[in]  value_handle    
 * @param[in]  buffer 
 * @param[in]  buffer_size    Ignore it.
 *
 * @retval  Length of current attribute value.
 */
uint16_t gattReadCallback(uint16_t value_handle, uint8_t * buffer, uint16_t buffer_size) {   
  uint8_t characteristic_len = 0;

  Serial.print("Read value handler: ");
  Serial.println(value_handle, HEX);

  if (character1_handle == value_handle) {   // Characteristic value handle.
    Serial.println("Character1 read:");
    memcpy(buffer, characteristic1_data, CHARACTERISTIC1_MAX_LEN);
    characteristic_len = CHARACTERISTIC1_MAX_LEN;
  }
  else if (character1_handle+1 == value_handle) {   // Client Characteristic Configuration Descriptor Handle.
    Serial.println("Character1 cccd read:");
    uint8_t buf[2] = { 0x01,0x00 };
    memcpy(buffer, buf, 2);
    characteristic_len = 2;
  }
  return characteristic_len;
}

/**
 * @brief Callback for writing event.
 *
 * @param[in]  value_handle  
 * @param[in]  *buffer       The buffer pointer of writting data.
 * @param[in]  size          The length of writting data.   
 *
 * @retval 
 */
int gattWriteCallback(uint16_t value_handle, uint8_t *buffer, uint16_t size) {
  Serial.print("Write value handler: ");
  Serial.println(value_handle, HEX);

  if (character1_handle == value_handle) {
    memcpy(characteristic1_data, buffer, size);
    Serial.print("Characteristic1 write value: ");
    for (uint8_t index = 0; index < size; index++) {
      Serial.print(characteristic1_data[index], HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  else if (character1_handle+1 == value_handle) {
    Serial.print("Characteristic1 cccd write value: ");
    for (uint8_t index = 0; index < size; index++) {
      Serial.print(buffer[index], HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
  return 0;
}

char dummy_log[] = "$BNRDD,300,2012-12-16T17:58:31Z,30,1,116,A,4618.9612,N,00658.4831,E,443.7,A,5,1.28*6D\r\n";

/**
 * @brief Setup.
 */
void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("BLE peripheral demo.");
    
  // Open debugger, must befor init().
  //ble.debugLogger(true);
  //ble.debugError(true);
  //ble.enablePacketLogger();
    
  // Initialize ble_stack.
  ble.init();

  // Register BLE callback functions.
  ble.onConnectedCallback(deviceConnectedCallback);
  ble.onDisconnectedCallback(deviceDisconnectedCallback);
  //ble.onDataReadCallback(gattReadCallback);
  //ble.onDataWriteCallback(gattWriteCallback);

  // Add GAP service and characteristics
  ble.addService(BLE_UUID_GAP);
  ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_DEVICE_NAME, ATT_PROPERTY_READ|ATT_PROPERTY_WRITE, (uint8_t*)BLE_DEVICE_NAME, sizeof(BLE_DEVICE_NAME));
  ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_APPEARANCE, ATT_PROPERTY_READ, appearance, sizeof(appearance));
  ble.addCharacteristic(BLE_UUID_GAP_CHARACTERISTIC_PPCP, ATT_PROPERTY_READ, conn_param, sizeof(conn_param));

  // Add GATT service and characteristics
  ble.addService(BLE_UUID_GATT);
  ble.addCharacteristic(BLE_UUID_GATT_CHARACTERISTIC_SERVICE_CHANGED, ATT_PROPERTY_INDICATE, change, sizeof(change));

  // Add primary service1.
  ble.addService(service1_uuid);
  // Add characteristic to service1, return value handle of characteristic.
  //character1_handle = ble.addCharacteristicDynamic(char1_uuid, ATT_PROPERTY_NOTIFY|ATT_PROPERTY_WRITE_WITHOUT_RESPONSE, characteristic1_data, CHARACTERISTIC1_MAX_LEN);
  character1_handle = ble.addCharacteristicDynamic(char1_uuid, ATT_PROPERTY_NOTIFY, characteristic1_data, CHARACTERISTIC1_MAX_LEN);
  
  // Set BLE advertising parameters
  ble.setAdvertisementParams(&adv_params);

  // Set BLE advertising and scan respond data
  ble.setAdvertisementData(sizeof(adv_data), adv_data);
  ble.setScanResponseData(sizeof(scan_response), scan_response);
  
  // Start advertising.
  ble.startAdvertising();
  Serial.println("BLE start advertising.");
    
}

/**
 * @brief Loop.
 */
void loop() {

  int len = strlen(dummy_log);
  int buff_count = 0;
  int str_count = 0;
  while (str_count < len)
  {
    char c = dummy_log[str_count];

    characteristic1_data[buff_count] = c;

    str_count++;
    buff_count++;

    if (c == '\n' || buff_count == CHARACTERISTIC1_MAX_LEN)
    {
      ble.sendNotify(character1_handle, characteristic1_data, CHARACTERISTIC1_MAX_LEN);
      for (int i = 0 ; i < CHARACTERISTIC1_MAX_LEN ; i++)
        characteristic1_data[i] = 0x00;
      buff_count = 0;
    }

    if (c == '\n')
      break;
  }
  Serial.println(dummy_log);
  delay(5000);

}

