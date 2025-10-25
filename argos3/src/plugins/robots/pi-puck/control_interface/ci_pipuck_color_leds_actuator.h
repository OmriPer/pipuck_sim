/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_color_leds_actuator.h>
 */

#pragma once

namespace argos {
   class CCI_PiPuckColorLEDsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   class CCI_PiPuckColorLEDsActuator : public CCI_Actuator {

   public:

      virtual ~CCI_PiPuckColorLEDsActuator() {}

      virtual void SetRingLEDIndex(UInt32 un_index, CColor c_color) = 0;

      virtual void SetRingLEDs(CColor c_color) = 0;

      virtual void SetFrontLED(CColor c_color) = 0;

      virtual void SetBodyLED(CColor c_color) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      const static UInt32 NUM_LEDS = 10;
      const static UInt32 NUM_RING_LEDS = 8;
      const static UInt32 INDEX_FRONT_LED = 8;
      const static UInt32 INDEX_BODY_LED = 9;

   };

}