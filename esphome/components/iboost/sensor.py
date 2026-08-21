import esphome.codegen as cg
import esphome.components.sensor as sensor
import esphome.components.text_sensor as text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.const import (
    UNIT_WATT, UNIT_WATT_HOURS, UNIT_MINUTE,
    DEVICE_CLASS_POWER, DEVICE_CLASS_ENERGY, DEVICE_CLASS_DURATION,
    STATE_CLASS_TOTAL_INCREASING
)
from . import iBoost

CONF_TX_POWER_PA_ENTRY = "tx_power_pa_entry"


def validate_tx_power_pa_entry(value):
    if isinstance(value, str):
        parsed = value.strip()
        try:
            value = int(parsed, 16 if parsed.lower().startswith("0x") else 10)
        except ValueError as err:
            raise cv.Invalid("tx_power_pa_entry must be an integer from 0 to 255") from err
    return cv.int_range(min=0, max=255)(value)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(iBoost),
        cv.Optional(CONF_TX_POWER_PA_ENTRY, default=0xC5): validate_tx_power_pa_entry,

        # Numeric Sensors
        cv.Optional("heating_import"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT, accuracy_decimals=0, device_class=DEVICE_CLASS_POWER
        ),
        cv.Optional("heating_power"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT, accuracy_decimals=0, device_class=DEVICE_CLASS_POWER
        ),
        cv.Optional("heating_today"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS, accuracy_decimals=0, device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING
        ),
        cv.Optional("heating_yesterday"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS, accuracy_decimals=0, device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING
        ),
        cv.Optional("heating_last_7"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS, accuracy_decimals=0, device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING
        ),
        cv.Optional("heating_last_28"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS, accuracy_decimals=0, device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING
        ),
        cv.Optional("heating_last_gt"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS, accuracy_decimals=0, device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING
        ),
        cv.Optional("heating_boost_time"): sensor.sensor_schema(
            unit_of_measurement=UNIT_MINUTE, accuracy_decimals=0, device_class=DEVICE_CLASS_DURATION
        ),

        # Text Sensors
        cv.Optional("heating_mode"): text_sensor.text_sensor_schema(),
        cv.Optional("heating_warn"): text_sensor.text_sensor_schema(),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])  # Ensure correct ID usage
    await cg.register_component(var, config)
    cg.add(var.set_tx_power_pa_entry(config[CONF_TX_POWER_PA_ENTRY]))

    # Register numeric sensors
    for key in ["heating_import", "heating_power", "heating_today", "heating_yesterday", "heating_last_7", "heating_last_28", "heating_last_gt", "heating_boost_time"]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}")(sens))

    # Register text sensors
    if "heating_mode" in config:
        mode_sensor = await text_sensor.new_text_sensor(config["heating_mode"])
        cg.add(var.set_heating_mode(mode_sensor))  

    if "heating_warn" in config:
        warn_sensor = await text_sensor.new_text_sensor(config["heating_warn"])
        cg.add(var.set_heating_warn(warn_sensor))  
