#ifndef COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_DIRECTIONAL_SENSOR_H
#define COLORED_BLOB_OMNIDIRECTIONAL_CAMERA_DIRECTIONAL_SENSOR_H

namespace argos {
   class CColoredBlobOmnidirectionalCameraDirectionalSensor;
   class COmnidirectionalCameraEquippedEntity;
   class CDirectionalLEDEntity;
   class CControllableEntity;
   class COmnidirectionalCameraLEDCheckOperationDirectional;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_colored_blob_omnidirectional_camera_sensor.h>

namespace argos {

  class CColoredBlobOmnidirectionalCameraDirectionalSensor : public CCI_ColoredBlobOmnidirectionalCameraSensor,
                                                          public CSimulatedSensor {

   public:

      CColoredBlobOmnidirectionalCameraDirectionalSensor();

      virtual ~CColoredBlobOmnidirectionalCameraDirectionalSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Destroy();

      virtual void Enable();
      
      virtual void Disable();
		
      inline bool IsShowRays() {
         return m_bShowRays;
      }

      inline void SetShowRays(bool b_show_rays) {
         m_bShowRays = b_show_rays;
      }

   protected:
      COmnidirectionalCameraEquippedEntity*    m_pcOmnicamEntity;
      CControllableEntity*                     m_pcControllableEntity;
      CEmbodiedEntity*                         m_pcEmbodiedEntity;
      CPositionalIndex<CDirectionalLEDEntity>* m_pcLEDIndex;
      CPositionalIndex<CEmbodiedEntity>*       m_pcEmbodiedIndex;
      COmnidirectionalCameraLEDCheckOperationDirectional* m_pcOperation;
      bool                                     m_bShowRays;

   };
}

#endif
