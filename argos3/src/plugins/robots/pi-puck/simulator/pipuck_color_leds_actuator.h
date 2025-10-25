#pragma once

namespace argos {
   class CPiPuckLEDsColorActuator;
   class CDirectionalLEDEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_color_leds_actuator.h>

#include <array>

namespace argos {

   class CPiPuckLEDsColorActuator : public CSimulatedActuator,
                                      public CCI_PiPuckColorLEDsActuator {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckLEDsColorActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckLEDsColorActuator() {};

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void SetRingLEDIndex(UInt32 un_index, CColor c_color);

      virtual void SetRingLEDs(CColor c_color);

      virtual void SetFrontLED(CColor c_color);

      virtual void SetBodyLED(CColor c_color);

   private:

      std::array<std::pair<CDirectionalLEDEntity*, CColor>, 
                 CCI_PiPuckColorLEDsActuator::NUM_LEDS> m_arrLEDs;

   };
}