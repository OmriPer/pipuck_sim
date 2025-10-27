#include "colored_blob_omnidirectional_camera_directional_sensor.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/positional_indices/positional_index.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_entity.h>
#include <argos3/plugins/simulator/entities/omnidirectional_camera_equipped_entity.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   /****************************************/
   /****************************************/

   class COmnidirectionalCameraLEDCheckOperationDirectional : public CPositionalIndex<CDirectionalLEDEntity>::COperation {

   public:

      COmnidirectionalCameraLEDCheckOperationDirectional(
         CCI_ColoredBlobOmnidirectionalCameraSensor::TBlobList& t_blobs,
         COmnidirectionalCameraEquippedEntity& c_omnicam_entity,
         CEmbodiedEntity& c_embodied_entity,
         CControllableEntity& c_controllable_entity,
         bool b_show_rays,
         Real f_noise_std_dev) :
         m_tBlobs(t_blobs),
         m_cOmnicamEntity(c_omnicam_entity),
         m_cEmbodiedEntity(c_embodied_entity),
         m_cControllableEntity(c_controllable_entity),
         m_bShowRays(b_show_rays),
         m_fDistanceNoiseStdDev(f_noise_std_dev),
         m_pcRNG(nullptr) {
         m_pcRootSensingEntity = &m_cEmbodiedEntity.GetParent();
         if(m_fDistanceNoiseStdDev > 0.0f) {
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      virtual ~COmnidirectionalCameraLEDCheckOperationDirectional() {
         while(! m_tBlobs.empty()) {
            delete m_tBlobs.back();
            m_tBlobs.pop_back();
         }
      }

      virtual bool operator()(CDirectionalLEDEntity& c_led) {
         // debug
         printf("checking LED\n");
         std::cout << "im a LED at " << c_led.GetPosition() << " with color " << c_led.GetColor() << std::endl;

         /* Process this LED only if it's lit */
         if(c_led.GetColor() != CColor::BLACK) {
            if(c_led.HasParent()) {
               /* Filter out the LEDs belonging to the sensing entity by checking if they share the same parent entity */
               m_pcRootOfLEDEntity = &c_led.GetParent();
               while(m_pcRootOfLEDEntity->HasParent()) m_pcRootOfLEDEntity = &m_pcRootOfLEDEntity->GetParent();
               if(m_pcRootSensingEntity == m_pcRootOfLEDEntity) {
                  return true;
               }
            }
            /* If we are here, it's because the LED must be processed */
            m_cOcclusionCheckRay.SetEnd(c_led.GetPosition());
            m_cLEDRelativePos = c_led.GetPosition();
            m_cLEDRelativePos -= m_cCameraPos;
            m_cLEDRelativePosXY.Set(m_cLEDRelativePos.GetX(),
                                    m_cLEDRelativePos.GetY());

            /* Directional LED cone check (camera must be inside LED emission cone)
               Use the directional LED entity's orientation and observable angle.
               CDirectionalLEDEntity exposes GetObservableAngle() and inherits
               CPositionalEntity, so we compute the LED forward direction by
               rotating the Z axis by the LED orientation. If the angle between
               that direction and the vector LED->camera is greater than the
               observable angle, the camera is outside the emission cone. */
            {
               /* vector from LED to camera */
               CVector3 cEntityToCamera(m_cCameraPos - c_led.GetPosition());
               if(cEntityToCamera.SquareLength() == 0.0f) return true;
               /* LED forward direction (local Z rotated by LED orientation) */
               CVector3 cLedDir(CVector3::Z);
               cLedDir.Rotate(c_led.GetOrientation());
               if(cLedDir.SquareLength() == 0.0f) return true;
               /* compute angle between LED direction and LED->camera vector */
               Real fDot = cLedDir.DotProduct(cEntityToCamera);
               CRadians cAngle = ACos(fDot / (cLedDir.Length() * cEntityToCamera.Length()));
               if(cAngle > c_led.GetObservableAngle()) {
                  /* Camera is outside the emission cone */
                  return true;
               }
            }

            if(Abs(m_cLEDRelativePos.GetX()) < m_fGroundHalfRange &&
               Abs(m_cLEDRelativePos.GetY()) < m_fGroundHalfRange &&
               m_cLEDRelativePos.GetZ() < m_cCameraPos.GetZ() &&
               !GetClosestEmbodiedEntityIntersectedByRay(m_sIntersectionItem,
                                                         m_cOcclusionCheckRay,
                                                         m_cEmbodiedEntity)) {
               /* If noise was setup, add it */
               if(m_fDistanceNoiseStdDev > 0.0f) {
                  m_cLEDRelativePosXY += CVector2(
                     m_cLEDRelativePosXY.Length() * m_pcRNG->Gaussian(m_fDistanceNoiseStdDev),
                     m_pcRNG->Uniform(CRadians::UNSIGNED_RANGE));
               }
               m_tBlobs.push_back(new CCI_ColoredBlobOmnidirectionalCameraSensor::SBlob(
                                     c_led.GetColor(),
                                     NormalizedDifference(m_cLEDRelativePosXY.Angle(), m_cCameraOrient),
                                     m_cLEDRelativePosXY.Length() * 100.0f));
               if(m_bShowRays) {
                  m_cControllableEntity.AddCheckedRay(false, CRay3(m_cCameraPos, c_led.GetPosition()));
               }
            }
         }
         return true;
      }

      void Setup(Real f_ground_half_range) {
         while(! m_tBlobs.empty()) {
            delete m_tBlobs.back();
            m_tBlobs.pop_back();
         }
         m_fGroundHalfRange = f_ground_half_range;
         m_cEmbodiedEntity.GetOriginAnchor().Orientation.ToEulerAngles(m_cCameraOrient, m_cTmp1, m_cTmp2);
         m_cCameraPos = m_cEmbodiedEntity.GetOriginAnchor().Position;
         m_cCameraPos += m_cOmnicamEntity.GetOffset();
         m_cOcclusionCheckRay.SetStart(m_cCameraPos);
      }
      
   private:
      
      CCI_ColoredBlobOmnidirectionalCameraSensor::TBlobList& m_tBlobs;
      COmnidirectionalCameraEquippedEntity& m_cOmnicamEntity;
      CEmbodiedEntity& m_cEmbodiedEntity;
      CControllableEntity& m_cControllableEntity;
      Real m_fGroundHalfRange;
      bool m_bShowRays;
      CEntity* m_pcRootSensingEntity;
      CEntity* m_pcRootOfLEDEntity;
      CVector3 m_cCameraPos;
      CRadians m_cCameraOrient;
      CRadians m_cTmp1, m_cTmp2;
      CVector3 m_cLEDRelativePos;
      CVector2 m_cLEDRelativePosXY;
      SEmbodiedEntityIntersectionItem m_sIntersectionItem;
      CRay3 m_cOcclusionCheckRay;
      Real m_fDistanceNoiseStdDev;
      CRandom::CRNG* m_pcRNG;
   };

   /****************************************/
   /****************************************/

   CColoredBlobOmnidirectionalCameraDirectionalSensor::CColoredBlobOmnidirectionalCameraDirectionalSensor() :
      m_pcOmnicamEntity(nullptr),
      m_pcControllableEntity(nullptr),
      m_pcEmbodiedEntity(nullptr),
      m_pcLEDIndex(nullptr),
      m_pcEmbodiedIndex(nullptr),
      m_bShowRays(false) {
   }

   /****************************************/
   /****************************************/

   CColoredBlobOmnidirectionalCameraDirectionalSensor::~CColoredBlobOmnidirectionalCameraDirectionalSensor() {
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::SetRobot(CComposableEntity& c_entity) {
      /* Get omndirectional camera equipped entity */
      m_pcOmnicamEntity = &(c_entity.GetComponent<COmnidirectionalCameraEquippedEntity>("omnidirectional_camera"));
      /* Get controllable entity */
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* Get embodied entity */
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Parent class init */
         CCI_ColoredBlobOmnidirectionalCameraSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise */
         Real fDistanceNoiseStdDev = 0;
         GetNodeAttributeOrDefault(t_tree, "noise_std_dev", fDistanceNoiseStdDev, fDistanceNoiseStdDev);
         /* Get directional LED medium from id specified in the XML */
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcLEDIndex = &(CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>(strMedium).GetIndex());
         /* Create check operation */
         m_pcOperation = new COmnidirectionalCameraLEDCheckOperationDirectional(
            m_sReadings.BlobList,
            *m_pcOmnicamEntity,
            *m_pcEmbodiedEntity,
            *m_pcControllableEntity,
            m_bShowRays,
            fDistanceNoiseStdDev);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the colored blob omnidirectional camera directional sensor", ex);
      }
      /* sensor is disabled by default */
      Disable();
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::Update() {
      /* sensor is disabled--nothing to do */
      if (IsDisabled()) {
        return;
      }

      /* Increase data counter */
      ++m_sReadings.Counter;
      /* Calculate range on the ground */
      CVector3 cCameraPos = m_pcOmnicamEntity->GetOffset();
      cCameraPos += m_pcEmbodiedEntity->GetOriginAnchor().Position;
      Real fGroundHalfRange = cCameraPos.GetZ() * Tan(m_pcOmnicamEntity->GetAperture());
      /* Prepare the operation */
      m_pcOperation->Setup(fGroundHalfRange);
      /* Go through directional LED entities in box range */
      m_pcLEDIndex->ForEntitiesInBoxRange(
         CVector3(cCameraPos.GetX(),
                  cCameraPos.GetY(),
                  cCameraPos.GetZ() * 0.5f),
         CVector3(fGroundHalfRange, fGroundHalfRange, cCameraPos.GetZ() * 0.5f),
         *m_pcOperation);
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::Reset() {
      m_sReadings.Counter = 0;
      m_sReadings.BlobList.clear();
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::Destroy() {
      delete m_pcOperation;
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::Enable() {
      m_pcOmnicamEntity->Enable();
      CCI_Sensor::Enable();
   }

   /****************************************/
   /****************************************/

   void CColoredBlobOmnidirectionalCameraDirectionalSensor::Disable() {
      m_pcOmnicamEntity->Disable();
      CCI_Sensor::Disable();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CColoredBlobOmnidirectionalCameraDirectionalSensor,
                   "colored_blob_omnidirectional_camera", "directional_led",
                   "Adapted",
                   "1.0",

                   "An omnidirectional camera sensor to detect colored blobs emitted by directional LEDs.",
                   "This sensor accesses an omnidirectional camera that detects colored blobs emitted by\n"
                   "directional LEDs. The sensor returns a list of blobs, each defined by a color and a\n"
                   "position with respect to the robot reference point on the ground. In controllers,\n"
                   "include ci_colored_blob_omnidirectional_camera_sensor.h.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <colored_blob_omnidirectional_camera implementation=\"directional_led\"\n"
                   "                                             medium=\"dleds\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "The 'medium' attribute must be set to the id of a <directional_led> medium.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "Use show_rays=\"true\" to draw rays, and noise_std_dev to add distance noise.\n",
                   "Usable"
  );

}
