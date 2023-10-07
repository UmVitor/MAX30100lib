#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "MAX30100_Interceptor.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"

#define REPORTING_PERIOD_MS 1000

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char *pin = "1234";
float a = 0;
String device_name = "ESP32-BT-Slave";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void TCA9548A(uint8_t bus)
{
    Wire.beginTransmission(0x70); 
    Wire.write(1 << bus);       
    Wire.endTransmission();
}

PulseOximeter pox1;
PulseOximeter pox2;

Interceptor interceptor;

uint32_t tsLastReport1 = 0;
uint32_t tsLastReport2 = 0;
uint16_t ir, red;
uint8_t spo2 = 1;
uint8_t spo2Pox1 = 1;
uint8_t spo2Pox2 = 1;
uint8_t spo2PoxInterceptor = 1;

void onBeatDetectedpox1()
{
    Serial.println("Beat! - POX1");
}

void onBeatDetectedpox2()
{
    Serial.println("Beat! - POX2 ");
}

void setup()
{
    Serial.begin(115200);
    SerialBT.begin(device_name);
    Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

#ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
#endif

    Wire.begin();

    Serial.print("Initializing pulse oximeter..");

    TCA9548A(2);
    if (!pox1.begin())
    {
        Serial.println("FAILED POX1 - MULTI 2");
        for (;;)
            ;
    }
    else
    {
        Serial.println("SUCCESS POX1 - MULTI 2");
    }

    pox1.setIRLedCurrent(MAX30100_LED_CURR_33_8MA);
    pox1.setOnBeatDetectedCallback(onBeatDetectedpox1);

    TCA9548A(3);
    if (!pox2.begin())
    {
        Serial.println("FAILED POX2 - MULTI 3");
        for (;;)
            ;
    }
    else
    {
        Serial.println("SUCCESS POX2 - MULTI 3");
    }

    pox2.setIRLedCurrent(MAX30100_LED_CURR_33_8MA);
    pox2.setOnBeatDetectedCallback(onBeatDetectedpox2);

    TCA9548A(5);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    display.clearDisplay();
    display.setRotation(2);
}

void PrintFloat2Byte(float value)
{
    byte *data = (byte *)&value;
    Serial.write(data, sizeof(value));
}

void loop()
{
    TCA9548A(2);
    // Make sure to call update as fast as possible
    pox1.updateAmp(1, interceptor);
    pox1.update();

    if (millis() - tsLastReport1 > REPORTING_PERIOD_MS)
    {
        spo2Pox1 = pox1.getSpO2();
        Serial.print(" // POX 1 - ");
        Serial.print(spo2Pox1);
        tsLastReport1 = millis();
    }

    TCA9548A(3);
    pox2.updateAmp(2, interceptor);
    pox2.update();

    if (millis() - tsLastReport2 > REPORTING_PERIOD_MS)
    {
        spo2Pox2 = pox2.getSpO2();
        spo2PoxInterceptor = interceptor.GetSpO2();
        Serial.print(" // POX 2 - ");
        Serial.print(spo2Pox2);
        Serial.print(" // INTER - ");
        Serial.print(spo2PoxInterceptor);
        if (spo2PoxInterceptor != 0)
        {
            spo2 = spo2PoxInterceptor;
        }
        else
        {
            if (spo2Pox1 != 0)
                spo2 = spo2Pox1;
            else if (spo2Pox2 != 0)
                spo2 = spo2Pox2;
        }
        Serial.print(" /// FINAL - ");
        Serial.println(spo2);
        tsLastReport2 = millis();
    }

    SerialBT.print(spo2);
    SerialBT.print(" ");
    SerialBT.println(interceptor.GlimpseSignal());

    TCA9548A(5);
    display.clearDisplay();
    display.setTextSize(6);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    display.print(spo2);
    display.println("%");
    display.display();

    delay(10);
}
