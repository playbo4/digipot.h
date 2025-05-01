import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import output
from esphome.const import CONF_ID
from esphome.core import CORE, HexInt
from esphome.cpp_helpers import setup_component

DEPENDENCIES = ["output"]
AUTO_LOAD = ["number"]

digipot_ns = cg.esphome_ns.namespace("digipot")
DigiPotComponent = digipot_ns.class_("DigiPotComponent", cg.Component)

MULTI_CONF = True

CONF_CS_PIN = "cs_pin"
CONF_UD_PIN = "ud_pin"
CONF_INC_PIN = "inc_pin"

DIGIPOT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DigiPotComponent),
        cv.Required(CONF_CS_PIN): cv.templatable(cv.pin),
        cv.Required(CONF_UD_PIN): cv.templatable(cv.pin),
        cv.Required(CONF_INC_PIN): cv.templatable(cv.pin),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cs_pin = await cg.gpio_pin_expression(config[CONF_CS_PIN])
    cg.add(var.constructor(cs_pin, await cg.gpio_pin_expression(config[CONF_UD_PIN]), await cg.gpio_pin_expression(config[CONF_INC_PIN])))

    # Set up the component
    setup_component(var, config)