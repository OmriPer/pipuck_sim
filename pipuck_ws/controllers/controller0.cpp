#include "controller0.hpp"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   /****************************************/
   /****************************************/

   void Controller0::Init(TConfigurationNode& t_tree) {
      m_pcWheels = GetActuator<CCI_PiPuckDifferentialDriveActuator>("pipuck_differential_drive");
      m_pcColoredLEDs = GetActuator<CCI_PiPuckColorLEDsActuator>("pipuck_leds");
   }

   void Controller0::ControlStep() {
         // m_pcWheels->SetLinearVelocity(0.1, 0.1); // 10 centimeter per second forwards
         int step = CSimulator::GetInstance().GetSpace().GetSimulationClock();
         switch (step % 100) {
            case 0 ... 24:
               m_pcColoredLEDs->SetRingLEDs(CColor::RED);
               break;
            case 25 ... 49:
               m_pcColoredLEDs->SetRingLEDs(CColor::GREEN);
               break;
            case 50 ... 74:
               m_pcColoredLEDs->SetRingLEDs(CColor::BLUE);
               break;
            case 75 ... 99:
               m_pcColoredLEDs->SetRingLEDs(CColor::BLACK);
               break;
         }
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(Controller0, "controller0");

}

