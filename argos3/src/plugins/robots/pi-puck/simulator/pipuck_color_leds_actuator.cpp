/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_leds_color_actuator.cpp>
 */

#include "pipuck_color_leds_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>

namespace argos {

    CPiPuckLEDsColorActuator::CPiPuckLEDsColorActuator() {
        // Initialize all LEDs to off (black)
        for(UInt32 un_index = 0; un_index < CCI_PiPuckColorLEDsActuator::NUM_LEDS; un_index++) {
            m_arrLEDs[un_index] = std::make_pair(nullptr, CColor::BLACK);
        }
    }

   /****************************************/
   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::SetRobot(CComposableEntity& c_entity) {
      CDirectionalLEDEquippedEntity* pcLEDEquippedEntity = 
         &(c_entity.GetComponent<CDirectionalLEDEquippedEntity>("directional_leds"));
      pcLEDEquippedEntity->Enable();
      CDirectionalLEDEquippedEntity::SInstance::TVector& sInstances =
         pcLEDEquippedEntity->GetInstances();
      if(sInstances.size() != CCI_PiPuckColorLEDsActuator::NUM_LEDS) {
         THROW_ARGOSEXCEPTION("Simulator reports an incorrect number of LEDs for the Pi-Puck");
      }
      // map the LEDs to the internal color array
      for(UInt32 un_index = 0; un_index < CCI_PiPuckColorLEDsActuator::NUM_LEDS; un_index++) {
         m_arrLEDs[un_index].first = &sInstances[un_index].LED;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckColorLEDsActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the Pi-Puck color LEDs actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::Update() {
      for(std::pair<CDirectionalLEDEntity*, CColor>& c_led : m_arrLEDs) {
         c_led.first->SetColor(c_led.second);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckLEDsColorActuator::Reset() {
      for(std::pair<CDirectionalLEDEntity*, CColor>& c_led : m_arrLEDs) {
         c_led.second = CColor::BLACK;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::SetRingLEDIndex(UInt32 un_index, CColor c_color) {
      if(un_index < NUM_RING_LEDS) {
         m_arrLEDs[un_index].second = c_color;
      }
      else {
         LOGERR << "[ERROR] Pi-Puck ring LED index out of range" << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::SetRingLEDs(CColor c_color) {
      for(UInt32 un_index = 0; un_index < NUM_RING_LEDS; un_index++) {
         m_arrLEDs[un_index].second = c_color;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::SetFrontLED(CColor c_color) {
      m_arrLEDs[INDEX_FRONT_LED].second = c_color;
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsColorActuator::SetBodyLED(CColor c_color) {
      m_arrLEDs[INDEX_BODY_LED].second = c_color;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CPiPuckLEDsColorActuator,
                     "pipuck_leds", "color",
                     "Author Name [email@example.com]",
                     "0.1",
                     "The Pi-Puck color LEDs actuator.",
                     "[docs url]",
                     "Beta"
   );

   /****************************************/
   /****************************************/
   
}
