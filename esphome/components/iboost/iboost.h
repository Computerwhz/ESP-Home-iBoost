/*************************************************************************************************

 ________    ______    _______    ____  ____                                 
|_   __  | .' ____ \  |_   __ \  |_   ||   _|                                
  | |_ \_| | (___ \_|   | |__) |   | |__| |     .--.    _ .--..--.    .---.  
  |  _| _   _.____`.    |  ___/    |  __  |   / .'`\ \ [ `.-. .-. |  / /__\\ 
 _| |__/ | | \____) |  _| |_      _| |  | |_  | \__. |  | | | | | |  | \__., 
|________|  \______.' |_____|    |____||____|  '.__.'  [___||__||__]  '.__.' 
                                                                             
              _    ______                                _                   
             (_)  |_   _ \                              / |_                 
             __     | |_) |    .--.     .--.    .--.   `| |-'                
            [  |    |  __'.  / .'`\ \ / .'`\ \ ( (`\]   | |                  
             | |   _| |__) | | \__. | | \__. |  `'.'.   | |,                 
            [___] |_______/   '.__.'   '.__.'  [\__) )  \__/                 
                                                                             
                                                                             

MIT License

Copyright (c) 2023 JNSwanson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


*************************************************************************************************
    Default ESP32 wiring used by this component:

      CC1101        ESP32

      CSN           GPIO5
      CSK(CLK)      GPIO18
      MISO          GPIO19
      MOSI          GPIO23
      GND           GND
      VCC           3.3V

    GDO0 / GDO2 are not used by this firmware.

	
******************************************************************************************************/

#ifndef ESPHOME_COMPONENTS_IBOOST_IBOOST_H
#define ESPHOME_COMPONENTS_IBOOST_IBOOST_H

#pragma once

#include "esphome.h"
#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/cc1101/cc1101.h"

namespace esphome {
namespace iboost {
	
#define PRINTLN(x, ...) Serial.println(x, ##__VA_ARGS__)
#define PRINT(x, ...) Serial.print(x, ##__VA_ARGS__)

#if defined(USE_ESP32) || defined(ARDUINO_ARCH_ESP32) || defined(ESP32)
static const uint8_t CC1101_CSN_PIN = 5;
static const uint8_t CC1101_SCK_PIN = 18;
static const uint8_t CC1101_MISO_PIN = 19;
static const uint8_t CC1101_MOSI_PIN = 23;
static const int IBOOST_STATUS_RGB_PIN = 16;
static const int IBOOST_POWER_LED_PIN = 2;
static const bool IBOOST_POWER_LED_ACTIVE_HIGH = true;
#elif defined(USE_ESP8266) || defined(ARDUINO_ARCH_ESP8266) || defined(ESP8266)
static const uint8_t CC1101_CSN_PIN = D8;
static const uint8_t CC1101_SCK_PIN = D5;
static const uint8_t CC1101_MISO_PIN = D2;
static const uint8_t CC1101_MOSI_PIN = D7;
static const int IBOOST_STATUS_RGB_PIN = -1;
static const bool IBOOST_POWER_LED_ACTIVE_HIGH = true;
#ifdef LED_BUILTIN
static const int IBOOST_POWER_LED_PIN = LED_BUILTIN;
#else
static const int IBOOST_POWER_LED_PIN = -1;
#endif
#else
static const uint8_t CC1101_CSN_PIN = SS;
static const uint8_t CC1101_SCK_PIN = SCK;
static const uint8_t CC1101_MISO_PIN = MISO;
static const uint8_t CC1101_MOSI_PIN = MOSI;
static const int IBOOST_STATUS_RGB_PIN = -1;
static const bool IBOOST_POWER_LED_ACTIVE_HIGH = true;
#ifdef LED_BUILTIN
static const int IBOOST_POWER_LED_PIN = LED_BUILTIN;
#else
static const int IBOOST_POWER_LED_PIN = -1;
#endif
#endif


static const char *const TAG = "iboost";

extern sensor::Sensor *heating_import;
extern sensor::Sensor *heating_power;
extern sensor::Sensor *heating_today;
extern sensor::Sensor *heating_yesterday;
extern sensor::Sensor *heating_last_7;
extern sensor::Sensor *heating_last_28;
extern sensor::Sensor *heating_last_gt;
extern sensor::Sensor *heating_boost_time;
extern text_sensor::TextSensor *heating_mode;
extern text_sensor::TextSensor *heating_warn;
extern binary_sensor::BinarySensor *water_tank_hot;

extern long today, yesterday, last7, last28, total;

class iBoost : public PollingComponent {
 public:
  static const uint8_t DEFAULT_TX_POWER_PA_ENTRY = 0xC5;
 
  // Constructor
  //iBoost();
  iBoost() : PollingComponent(15000), radio(CC1101_CSN_PIN, CC1101_MISO_PIN, CC1101_SCK_PIN, CC1101_MOSI_PIN) {}  // Poll every 15 seconds


  // Override setup() from PollingComponent
  void setup() override;
  void loop() override;
  void update() override;
  void boost(uint8_t boost_time);
  void set_inhibit(bool v);



    void set_heating_import(sensor::Sensor *sensor) { heating_import = sensor; }
    void set_heating_power(sensor::Sensor *sensor) { heating_power = sensor; }
    void set_heating_today(sensor::Sensor *sensor) { heating_today = sensor; }
    void set_heating_yesterday(sensor::Sensor *sensor) { heating_yesterday = sensor; }
    void set_heating_last_7(sensor::Sensor *sensor) { heating_last_7 = sensor; }
    void set_heating_last_28(sensor::Sensor *sensor) { heating_last_28 = sensor; }
    void set_heating_last_gt(sensor::Sensor *sensor) { heating_last_gt = sensor; }
    void set_heating_boost_time(sensor::Sensor *sensor) { heating_boost_time = sensor; }
    void set_heating_mode(text_sensor::TextSensor *sensor) { heating_mode = sensor; }
    void set_heating_warn(text_sensor::TextSensor *sensor) { heating_warn = sensor; }
    void set_water_tank_hot(binary_sensor::BinarySensor *sensor) { water_tank_hot = sensor; }
    void set_tx_power_pa_entry(uint8_t value) { tx_power_pa_entry_ = value; }

private:
    sensor::Sensor *heating_import{nullptr};
    sensor::Sensor *heating_power{nullptr};
    sensor::Sensor *heating_today{nullptr};
    sensor::Sensor *heating_yesterday{nullptr};
    sensor::Sensor *heating_last_7{nullptr};
    sensor::Sensor *heating_last_28{nullptr};
    sensor::Sensor *heating_last_gt{nullptr};
    sensor::Sensor *heating_boost_time{nullptr};
    text_sensor::TextSensor *heating_mode{nullptr};
    text_sensor::TextSensor *heating_warn{nullptr};
    binary_sensor::BinarySensor *water_tank_hot{nullptr};
    uint8_t tx_power_pa_entry_{DEFAULT_TX_POWER_PA_ENTRY};

  
  esphome::cc1101::CC1101 radio;
  
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }



};
}  // namespace iboost
}  // namespace esphome
#endif
