import esphome.codegen as cg
import esphome.components.binary_sensor as binary_sensor
import esphome.config_validation as cv

from . import iBoost

CONF_IBOOST_ID = "iboost_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_IBOOST_ID): cv.use_id(iBoost),
        cv.Optional("water_tank_hot"): binary_sensor.binary_sensor_schema(
            device_class="heat",
            icon="mdi:water-boiler",
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_IBOOST_ID])

    if "water_tank_hot" in config:
        hot_sensor = await binary_sensor.new_binary_sensor(config["water_tank_hot"])
        cg.add(parent.set_water_tank_hot(hot_sensor))
