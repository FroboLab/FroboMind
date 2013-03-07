// Copyright (c) 2011 University of Southern Denmark. All rights reserved.
// Use of this source code is governed by the MIT license (see license.txt).
#ifndef CAMERA_JAI_AD_080GE_H_
#define CAMERA_JAI_AD_080GE_H_

#include "JAI/Jai_Types.h"
#include <semaphore.h>
#include <time.h>
//#include "image.h"
//#include "camera.h"

    class JAI_AD080GE  {
     public:
      enum trigger
      {
          internal,
          opt1,
          opt2,
          generator0,
          generator1,
          UserOutput0,
	  UserOutput1
      };
      enum sensor
      {
        RGB,
        NIR
      };
      JAI_AD080GE(char * serial, sensor s, double focalLength);
      ~JAI_AD080GE(void);
      int init(int8_t * serial, sensor s);
      int setTrigger(JAI_AD080GE::trigger inputTrigger);
      int changeTrigger(JAI_AD080GE::trigger inputTrigger);
      int setExposure(float period);
      int setNIRExposure(float period);
      int setGain(float gain);
      int setNIRGain(float gain);
      int setGenerator0FPS(float fps);
      
      int setROI(int x, int y, int width, int height);
      int pollImage(void *image);
      int startAquisition(void);
      int stopAquisition(void);
      void run();
      
     private:
      const static double         pixelSize = 0.00465; //mm
      double focalLength;
      int            openDriver(void);              //open JAI SDK
      int            findCamera(int8_t * serial, sensor s);  //Find specified cameras
      int            openCamera(void);             //Open cameras
      int            setNetworkPacketSize(int size); //packet size
      int            setNetworkPacketDelay(int delay); //Packet delay
      int            setBPP(int bpp);               //Set bit per pixel
      int            setExposure(float period, CAM_HANDLE * sensor);
      int            setGain(float gain, CAM_HANDLE * sensor);
      int            NIRCallback(J_tIMAGE_INFO * pAqImageInfo);
      int            Callback(J_tIMAGE_INFO * pAqImageInfo);
      int            DeviceReset(void);
      char *         serial;
      
      sensor          sensorType;
      static FACTORY_HANDLE  m_hFactory;     // Factory Handle
//      static QMutex FactoryLock; //Mutex for the factory
      int8_t         * m_sCameraId;    // Camera ID's
      int8_t         * m_sCameraIdMono;      // Camera ID's
      CAM_HANDLE       m_hCam;         // Camera Handle
      CAM_HANDLE       m_hCamMono;           // Camera Handle
      static const int BUFFERCOUNT = 3;     // how many buffers
      static const int BUFFERSIZE = 1024*768*2; //How large a buffer
      J_tIMAGE_INFO  * ImageBuffer;       // image buffer
      J_tIMAGE_INFO  * NIRImageBuffer;       // image buffer
      timespec        * TimeBuffer;        //buffer for holding the timestamp
      timespec        * NIRTimeBuffer;        //buffer for holding the timestamp
      int              bitDepth; //bitDepth of images
      int              width;
      int              height;
      int              aqstate;
      int              WritePointer;
      int              ReadPointer;
      int              NIRWritePointer;
      int              NIRReadPointer;
      sem_t            EmptyCount;
      sem_t            FullCount;
      sem_t            Mutex;
      sem_t            NIREmptyCount;
      sem_t            NIRFullCount;
      sem_t            NIRMutex;
      THRD_HANDLE      NIRCallBackThread;
      THRD_HANDLE      CallBackThread;
      bool previewEnabled;
//     signals:
//       void newImage(void * image);
//       void newPreviewImage(void * image);
    public:
    void captureWhenTrigger(JAI_AD080GE::trigger inputTrigger);
    int sendUserOutput0Pulse(void);
    int setUserOutput0(void);
    int resetUserOutput0(void);
    int setUserOutput1(void);
    int resetUserOutput1(void);
    void setOptOut1Output(JAI_AD080GE::trigger inputTrigger);
    void setOptOut2Output(JAI_AD080GE::trigger inputTrigger);
    };

#endif  // CAMERA_JAI_AD_080GE_H_

