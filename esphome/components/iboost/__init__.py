import esphome.codegen as cg

DEPENDENCIES = ["cc1101"]
AUTO_LOAD = ["text_sensor", "binary_sensor"]

iboost_ns = cg.esphome_ns.namespace("iboost")
iBoost = iboost_ns.class_("iBoost", cg.PollingComponent)
