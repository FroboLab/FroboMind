// Copyright (c) 2011 University of Southern Denmark. All rights reserved.
// Use of this source code is governed by the MIT license (see license.txt).
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <semaphore.h>
//#include "camera.h"

//#include "jai_ad-080ge.moc"

#include "jai_ad-080ge.h"

#include "JAI/Jai_Factory.h"

FACTORY_HANDLE JAI_AD080GE::m_hFactory = NULL;
//QMutex JAI_AD080GE::FactoryLock;

JAI_AD080GE::JAI_AD080GE(char * serial, sensor s, double focalLength){
  this->focalLength = focalLength;
    this->m_sCameraId = new int8_t[J_CAMERA_ID_SIZE];
//    this->m_sCameraIdMono = new int8_t[J_CAMERA_ID_SIZE];
    this->ImageBuffer = new J_tIMAGE_INFO[this->BUFFERCOUNT];
    for(int i = 0; i< this->BUFFERCOUNT; i++)
        this->ImageBuffer[i].pImageBuffer = new uint8_t[this->BUFFERSIZE];
    this->TimeBuffer = new timespec[this->BUFFERCOUNT];
 //   this->NIRImageBuffer = new J_tIMAGE_INFO[this->BUFFERCOUNT];    
 //   for(int i = 0; i< this->BUFFERCOUNT; i++)
 //       this->NIRImageBuffer[i].pImageBuffer = new uint8_t[this->BUFFERSIZE];
//    this->NIRTimeBuffer = new timespec[this->BUFFERCOUNT];
    //this->m_hFactory = NULL;
    this->m_hCam = NULL;
 //   this->m_hCamMono = NULL;
    this->aqstate = 0;
    sem_init(&this->FullCount, 0, this->BUFFERCOUNT);
    sem_init(&this->EmptyCount, 0, 0);
    sem_init(&this->Mutex, 0, 1);
//    sem_init(&this->NIRFullCount, 0, this->BUFFERCOUNT);
//    sem_init(&this->NIREmptyCount, 0, 0);
//    sem_init(&this->NIRMutex, 0, 1);
    this->WritePointer = 0;
    this->ReadPointer = 0;
//    this->NIRWritePointer = 0;
//    this->NIRReadPointer = 0;
    this->serial = new char[20];
    previewEnabled = true;
    while(this->init((int8_t *)serial, s)==EXIT_FAILURE)
      std::cerr << "Failed initializing camera" << std::endl;
}

JAI_AD080GE::~JAI_AD080GE(void){
    this->stopAquisition();
    if (this->m_hCam) {
        // Close camera
//        FactoryLock.lock();
        J_Camera_Close(this->m_hCam);
        this->m_hCam = NULL;
//        FactoryLock.unlock();
        std::cout << "closed camera" << std::endl;
    }
    
 //   if (this->m_hCamMono) {
        // Close camera
//        J_Camera_Close(this->m_hCamMono);
//        this->m_hCamMono = NULL;
 //       std::cout << "closed Grayscale camera" << std::endl;
//    }
//    FactoryLock.lock();
    if (this->m_hFactory != NULL)
    {
        // Close factory
        J_Factory_Close(this->m_hFactory);
        this->m_hFactory = NULL;
        std::cout << "Closed factory" << std::endl;
    }
//    FactoryLock.unlock();
    delete this->m_sCameraId;
//    delete this->m_sCameraIdMono;
    for(int i = 0; i< this->BUFFERCOUNT; i++)
        delete this->ImageBuffer[i].pImageBuffer;
//    for(int i = 0; i< this->BUFFERCOUNT; i++)
//        delete this->NIRImageBuffer[i].pImageBuffer;
    delete this->ImageBuffer;
//    delete this->NIRImageBuffer;
//    delete this->NIRTimeBuffer;
    delete this->TimeBuffer;
    delete this->serial;
}

int JAI_AD080GE::init(int8_t * serial, sensor s){
  static bool factoryLoaded = false;
    strncpy(this->serial, (char*)serial, 20);
    
    if(m_hFactory == NULL)
    {
      if(this->openDriver() != EXIT_SUCCESS)
          return EXIT_FAILURE;
    }

    if(this->findCamera(serial, s) != EXIT_SUCCESS)
        return EXIT_FAILURE;

    if(this->openCamera() != EXIT_SUCCESS)
        return EXIT_FAILURE;
//    if(this->DeviceReset() != EXIT_SUCCESS)
//	return EXIT_FAILURE;

    if(this->setNetworkPacketSize(1476) != EXIT_SUCCESS)
      return EXIT_FAILURE;  
    
    if(this->setNetworkPacketDelay(3500) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    
//     if(this->setBPP(12) != EXIT_SUCCESS)
//         return EXIT_FAILURE;
//     
//     if(this->setTrigger(JAI_AD080GE::internal) != EXIT_SUCCESS)
//         return EXIT_FAILURE;
//      
//     if(this->setExposure(3000) != EXIT_SUCCESS)
//         return EXIT_FAILURE;
// 
//  //   if(this->setNIRExposure(3000) != EXIT_SUCCESS)
//  //       return EXIT_FAILURE;
//         
//     if(this->setGain(0.0) != EXIT_SUCCESS)
//         return EXIT_FAILURE;
//         
// //    if(this->setNIRGain(9.0) != EXIT_SUCCESS)
// //        return EXIT_FAILURE;
// 
//     if(this->setROI(0, 0, 1024, 768) != EXIT_SUCCESS)
//         return EXIT_FAILURE;

    this->setOptOut1Output(UserOutput0);
/*
    if(this->setGenerator0FPS(1.0) != EXIT_SUCCESS)
        return EXIT_FAILURE;*/
        
}

int JAI_AD080GE::sendUserOutput0Pulse(void)
{
  setUserOutput0();
//  msleep(5);
  resetUserOutput0();
  return EXIT_SUCCESS;
}

int JAI_AD080GE::resetUserOutput0(void)
{
  J_STATUS_TYPE   retval;
  char nodeName[] = "SoftwareTrigger0";
  if((retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)nodeName, 0))!=J_ST_SUCCESS)
  {
    std::cerr << "Failed setting software0 trigger" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int JAI_AD080GE::resetUserOutput1(void)
{
  J_STATUS_TYPE   retval;
  char nodeName[] = "SoftwareTrigger1";
  if((retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)nodeName, 0))!=J_ST_SUCCESS)
  {
    std::cerr << "Failed setting software1 trigger" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int JAI_AD080GE::setUserOutput0(void)
{
  J_STATUS_TYPE   retval;
  char nodeName[] = "SoftwareTrigger0";
  
  if((retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)nodeName, 1))!=J_ST_SUCCESS)
  {
    std::cerr << "Failed setting software0 trigger" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int JAI_AD080GE::setUserOutput1(void)
{
  J_STATUS_TYPE   retval;
  char nodeName[] = "SoftwareTrigger1";
  
  if((retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)nodeName, 1))!=J_ST_SUCCESS)
  {
    std::cerr << "Failed setting software1 trigger" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int JAI_AD080GE::openDriver(void){
    J_STATUS_TYPE   retval;
//    FactoryLock.lock();
    // Open factory
    retval = J_Factory_Open((int8_t*)"" , &m_hFactory);
//    FactoryLock.unlock();
    if (retval != J_ST_SUCCESS)
    {
        std::cerr << "Could not open factory!" << std::endl;
        return EXIT_FAILURE;
    }
    
	return EXIT_SUCCESS;
}

int JAI_AD080GE::findCamera(int8_t * serial, sensor s){
    J_STATUS_TYPE   retval;
    uint32_t        iSize;
    bool8_t         bHasChange;
    int64_t         int64Val;
    int64_t         iPixelFormat = 0;
    NODE_HANDLE     hNode;
    uint32_t        m_iNumDev; // Number of cameras
//    FactoryLock.lock();
    // Get camera list
    retval = J_Factory_UpdateCameraList(m_hFactory, &bHasChange);
    if (retval != J_ST_SUCCESS)
    {
        std::cerr << "Could not update camera list!" << std::endl;
//        FactoryLock.unlock();
        return EXIT_FAILURE;
    }

    // Get the number of Cameras
    retval = J_Factory_GetNumOfCameras(m_hFactory, &m_iNumDev);
    if (retval != J_ST_SUCCESS)
    {
        std::cerr << "Could not get the number of cameras!" << std::endl;
//        FactoryLock.unlock();
        return EXIT_FAILURE;
    }
    if (m_iNumDev == 0)
    {
        std::cerr << "There is no camera!" << std::endl;
//        FactoryLock.unlock();
        return EXIT_FAILURE;
    }
    //std::cout << m_iNumDev << " gige cameras were found" << std::endl;

    // Get camera ID and discover the camera with our serial number
    {
    int j = 0;
	for(int32_t i =0;i<m_iNumDev;i++)
	{
		iSize = J_CAMERA_ID_SIZE;
		int8_t   sSerial[J_CAMERA_INFO_SIZE];
		int8_t CameraId[J_CAMERA_ID_SIZE];
		retval = J_Factory_GetCameraIDByIndex(this->m_hFactory, i, CameraId, &iSize);
		char * sensorType;
		if (retval != J_ST_SUCCESS)
		{
			std::cerr << "Could not get the camera ID!" << std::endl;
//                        FactoryLock.unlock();
			return EXIT_FAILURE;
		}
	//	std::cout << "Camera " << i << " ID: " << CameraId << std::endl;
		
		J_Factory_GetCameraInfo(this->m_hFactory, CameraId, CAM_INFO_SERIALNUMBER, sSerial, &iSize);
	//	std::cout << "Camera serial:" << sSerial << std::endl;
		
		if(strncmp((char *)serial, (char *)sSerial, sizeof(sSerial))== 0) {
	//	    std::cout << "Found a matching serial number" << std::endl;
		    sensorType=strchr((char *)CameraId,'#') + 1;
		    if(*sensorType == '0') {
                      if(s==RGB)
                      {
	//	        std::cout << "Found Colour sensor" << std::endl;
                        memcpy(this->m_sCameraId, CameraId, sizeof(CameraId));
                      }
		    }    
                    else if(*sensorType == '1') {
                      if(s==NIR)
                      {
                        memcpy(this->m_sCameraId, CameraId, sizeof(CameraId));
          //              std::cout << "Found NIR sensor" << std::endl;
                      }
                    }
		    j++;
                }
                
	    if(j>2)
	    {
			std::cerr << "More than 2 sensors with the serial " << 
			             "found, probably because you are running " <<
			             "in windows? A hint if you want to make this work " <<
			             "Look at selecting between socket and filter driver"
			               << std::endl;
//                                       FactoryLock.unlock();
			return EXIT_FAILURE;    
	    }
	}
	}
//	FactoryLock.unlock();
	return EXIT_SUCCESS;
}

int JAI_AD080GE::openCamera(void){
	J_STATUS_TYPE   retval;

//        FactoryLock.lock();
    // Open the cameras
	retval = J_Camera_Open(this->m_hFactory, 
	                       this->m_sCameraId, &m_hCam);
//        FactoryLock.unlock();
	if (retval != J_ST_SUCCESS)
	{
		std::cerr << "Could not open camera " << "!" << std::endl;
		return EXIT_FAILURE;
	}
//	std::cout << "Opening camera " << " succeeded" << std::endl;
	/*
	retval = J_Camera_Open(this->m_hFactory, 
	                       this->m_sCameraIdMono, &m_hCamMono);
	if (retval != J_ST_SUCCESS)
	{
		std::cerr << "Could not open grayscale camera " << "!" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "Opening Grayscale camera "<< " succeeded" << std::endl;
	*/
	return EXIT_SUCCESS;
}

int JAI_AD080GE::setNetworkPacketSize(int size){
	J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    int64_t         int64Val;
/*
    retval = J_Camera_GetNodeByName(m_hCamMono, (int8_t *)"GevSCPSPacketSize", &hNode);
    if (retval != J_ST_SUCCESS) {
        std::cerr << "Warning: Could not get the 'GevSCPSPacketSize' GeniCam node! The packet size will not be changed" << std::endl << std::flush;
        return EXIT_FAILURE;
    }
    else {
        retval = J_Node_GetMinInt64(hNode, &int64Val);
        if (retval != J_ST_SUCCESS) {
            std::cerr << "Error: Could not get the minimum 'GevSCPSPacketSize' value!" << std::endl << std::flush;
            return EXIT_FAILURE;
        } 
        else {
            if ( int64Val < size) {
                size = int64Val;
                std::cerr << "size specifed was less than minimum supported, setting to minimum=" << size << std::endl;
                return EXIT_FAILURE;
            }
            retval = J_Node_SetValueInt64(hNode, false, int64Val);
            if (retval != J_ST_SUCCESS) {
              std::cerr << "Error: Could not set the 'GevSCPSPacketSize' value!" << std::endl << std::flush;
              return EXIT_FAILURE;
            }
            else
              std::cout << "Changed the packet size into " << int64Val << " bytes" << std::endl << std::flush;                
        }
    }
    */
//    FactoryLock.lock();
    retval = J_Camera_GetNodeByName(m_hCam, (int8_t *)"GevSCPSPacketSize", &hNode);
    if (retval != J_ST_SUCCESS) {
        std::cerr << "Warning: Could not get the 'GevSCPSPacketSize' GeniCam node! The packet size will not be changed" << std::endl << std::flush;
//        FactoryLock.unlock();
        return EXIT_FAILURE;
    }
    else {
        retval = J_Node_GetMinInt64(hNode, &int64Val);
        if (retval != J_ST_SUCCESS) {
            std::cerr << "Error: Could not get the minimum 'GevSCPSPacketSize' value!" << std::endl << std::flush;
//            FactoryLock.unlock();
            return EXIT_FAILURE;
        } 
        else {
            if ( int64Val < size) {
                size = int64Val;
                std::cerr << "size specifed was less than minimum supported, setting to minimum=" << size << std::endl;
//                FactoryLock.unlock();
                return EXIT_FAILURE;
            }
            retval = J_Node_SetValueInt64(hNode, true, int64Val);
            if (retval != J_ST_SUCCESS) {
              std::cerr << "Error: Could not set the 'GevSCPSPacketSize' value!" << std::endl << std::flush;
 //             FactoryLock.unlock();
              return EXIT_FAILURE;
            }
//             std::cout << "Setting package size to " << int64Val;
//             retval = J_Node_GetValueInt64(hNode, true, &int64Val);
//             std::cout << ":" << int64Val << std::endl;
      //      else
      //        std::cout << "Changed the packet size into " << int64Val << " bytes" << std::endl << std::flush;                
        }
    }
//    FactoryLock.unlock();
    return EXIT_SUCCESS;
}

int JAI_AD080GE::setBPP(int bpp){
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    int64_t val;
    int previousAqState =0;

    //Stop aquistition if running
    previousAqState = this->aqstate;
    if(this->stopAquisition() != EXIT_SUCCESS) {
        std::cerr << "Could not stop image Aquisition" << std::endl;
        return EXIT_FAILURE;
    }
//    FactoryLock.lock();            
    if(bpp == 12) { 
       this->bitDepth = 12;
       // Set pixelformat on the camera
       
       retval = J_Camera_GetNodeByName(m_hCam, (int8_t *)"PixelFormat", &hNode);
       if (retval != J_ST_SUCCESS) {
        std::cerr << "Could not find pixelformat on camera " << "!" << std::endl;
   //     FactoryLock.unlock();
        return EXIT_FAILURE;
           }
       
       retval = J_Node_SetValueInt64(hNode, true, J_GVSP_PIX_BAYRG12_PACKED);
       if(retval != J_ST_SUCCESS)
         retval = J_Node_SetValueInt64(hNode, true, J_GVSP_PIX_MONO12_PACKED);
         if(retval != J_ST_SUCCESS)
           std::cerr << "Could not set Bpp" << std::endl;
//         {
//         std::cerr << "Could not set camera bit depth on camera " << "!" << std::endl;
//         FactoryLock.unlock();
//         return EXIT_FAILURE;
// 	   }
         retval = J_Node_GetValueInt64(hNode, true, &val);
        if(!(J_GVSP_PIX_OCCUPY12BIT && val))
        {
          std::cerr << "Could not set Bpp" << std::endl;
 //         FactoryLock.unlock();
          return EXIT_FAILURE;
        }
        
        /*
       retval = J_Camera_GetNodeByName(m_hCamMono, (int8_t *)"PixelFormat", &hNode);
       retval = J_Node_SetValueInt64(hNode, false, J_GVSP_PIX_MONO12_PACKED);
       if (retval != J_ST_SUCCESS) {
        std::cerr << "Could not set camera bit depth on camera " << "!" << std::endl;
        return EXIT_FAILURE;
       }
       */

    } else if(bpp = 8) { 
        this->bitDepth = 8;
       // Set pixelformat on the camera
   //     FactoryLock.lock();
       retval = J_Camera_GetNodeByName(m_hCam, (int8_t *)"PixelFormat", &hNode);
       if (retval != J_ST_SUCCESS) {
        std::cerr << "Could not find pixelformat on camera " << "!" << std::endl;
   //     FactoryLock.unlock();
        return EXIT_FAILURE;
           }
       if(this->sensorType==RGB)
        retval = J_Node_SetValueInt64(hNode, false, J_GVSP_PIX_BAYRG8); //J_GVSP_PIX_BAYRG12_PACKED
       else
        retval = J_Node_SetValueInt64(hNode, false, J_GVSP_PIX_MONO8);
       if (retval != J_ST_SUCCESS) {
        std::cerr << "Could not set camera bit depth on camera " << "!" << std::endl;
  //      FactoryLock.unlock();
        return EXIT_FAILURE;
        }
        /*
       retval = J_Camera_GetNodeByName(m_hCamMono, (int8_t *)"PixelFormat", &hNode);
       retval = J_Node_SetValueInt64(hNode, false, J_GVSP_PIX_MONO8); //J_GVSP_PIX_MONO12_PACKED
       if (retval != J_ST_SUCCESS) {
        std::cerr << "Could not set camera bit depth on camera " << "!" << std::endl;
        return EXIT_FAILURE;
       }
*/
        
    } else {
        std::cerr << "Bitdepth not implemented" << std::endl;
  //      FactoryLock.unlock();
        return EXIT_FAILURE;
    }
  //  FactoryLock.unlock();
    
    if(previousAqState == 1)
        if(this->startAquisition() != EXIT_SUCCESS) {
            std::cerr << "Could not restart image Aquisition" << std::endl;
            return EXIT_FAILURE;
            }
            
 //    std::cout << "BitDepth updated successfully" << std::endl;   
     
    return EXIT_SUCCESS;
}

int JAI_AD080GE::setExposure(float period){
    return this->setExposure(period, &m_hCam);
}

int JAI_AD080GE::setNIRExposure(float period){
    return this->setExposure(period, &m_hCamMono);
}

int JAI_AD080GE::setExposure(float period, CAM_HANDLE * sensor){
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    const float exposureMin = 20;
    const float exposureMax = 20 + 792*42.071;
    int rawPeriod = 0;
    
    if(period < exposureMin) {
        std::cerr << "Exposure time set below minimum" << std::endl;
        return EXIT_FAILURE;
    }
    else if (period > exposureMax) {
        std::cerr << "Exposure time set above Maximum" << std::endl;
        return EXIT_FAILURE;
    }
    
    rawPeriod = floor(((period-20.0)/40.071) + 0.5);
  //  FactoryLock.lock();
    //Set exposure time
    retval = J_Camera_GetNodeByName(*sensor, (int8_t*)"ExposureTimeRaw", &hNode);
    retval = J_Node_SetValueInt64(hNode, 0, rawPeriod);
  //  FactoryLock.unlock();
    if(retval != J_ST_SUCCESS) 
    {
	    std::cerr << "Set exposure time failed" << std::endl;
        return EXIT_FAILURE;
    }
   
   return EXIT_SUCCESS;
}

int JAI_AD080GE::setROI(int x, int y, int width, int height) {
        J_STATUS_TYPE   retval;
        int previousAqState =0;
 
        //Stop aquistition if running
        previousAqState = this->aqstate;
        if(this->stopAquisition() != EXIT_SUCCESS) {
            std::cerr << "Could not stop image Aquisition" << std::endl;
            return EXIT_FAILURE;
        }
        
        //Check input parameters
        if (x > 1016) {
            std::cerr << "x is to large maximum is 1016" << std::endl;
            return EXIT_FAILURE;
        }
        if (x < 0) {
            std::cerr << "y is to small minimum is 0" << std::endl;
            return EXIT_FAILURE;
        }
        if (y > 760) {
            std::cerr << "y is to large maximum is 760" << std::endl;
            return EXIT_FAILURE;
        }
        if (y < 0) {
            std::cerr << "y is to small minimum is 0" << std::endl;
            return EXIT_FAILURE;
        }
        if (width > 1024) {
            std::cerr << "Width is to large maximum is 1024" << std::endl;
            return EXIT_FAILURE;
        }
        if (width < 8) {
            std::cerr << "Width is to small minimum is 8" << std::endl;
            return EXIT_FAILURE;
        }
        if ((x+width) > 1024) {
            std::cerr << "combination of width+offset is to" <<
                         " large maximum is 1024" << std::endl;
            return EXIT_FAILURE;
        }
        if (height > 768) {
            std::cerr << "Height is to large maximum is 768" << std::endl;
            return EXIT_FAILURE;
        }
        if (height < 8) {
            std::cerr << "Height is to small minimum is 8" << std::endl;
            return EXIT_FAILURE;
        }
        if ((height+y) > 768) {
            std::cerr << "combination of height+offset is to" <<
                         " large maximum is 768" << std::endl;
            return EXIT_FAILURE;
        }
        

        /*
        // set ROI
	    retval = J_Camera_SetValueInt64(m_hCamMono, (int8_t*)"OffsetX", x);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set x offset failed" << std::endl;
            return EXIT_FAILURE;
        }
	    retval = J_Camera_SetValueInt64(m_hCamMono, (int8_t*)"OffsetY", y);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set y offset failed" << std::endl;
            return EXIT_FAILURE;
        }
	    retval = J_Camera_SetValueInt64(m_hCamMono, (int8_t*)"Width", width);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set width failed" << std::endl;
            return EXIT_FAILURE;
        }
	    retval = J_Camera_SetValueInt64(m_hCamMono, (int8_t*)"Height", height);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set height failed" << std::endl;
            return EXIT_FAILURE;
        }

	    //Enable fast-readout
	    retval = J_Camera_SetValueInt64(m_hCamMono, (int8_t*)"FastDumpEnable", 1);
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Set fast-readout failed" << std::endl;
            return EXIT_FAILURE;
        }
        */
        // set ROI
  //      FactoryLock.lock();
	    retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)"OffsetX", x);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set x offset failed" << std::endl;
  //              FactoryLock.unlock();
            return EXIT_FAILURE;
        }
	    retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)"OffsetY", y);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set y offset failed" << std::endl;
   //             FactoryLock.unlock();
            return EXIT_FAILURE;
        }
	    retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)"Width", width);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set width failed" << std::endl;
   //             FactoryLock.unlock();
            return EXIT_FAILURE;
        }
	    retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)"Height", height);
        if(retval != J_ST_SUCCESS) 
        {
	        std::cerr << "Set height failed" << std::endl;
   //             FactoryLock.unlock();
            return EXIT_FAILURE;
        }

	    //Enable fast-readout
   	    retval = J_Camera_SetValueInt64(m_hCam, (int8_t*)"FastDumpEnable", 1);
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Set fast-readout failed" << std::endl;
     //               FactoryLock.unlock();
            return EXIT_FAILURE;
        }
        
        this->width = width;
        this->height = height;
     //   FactoryLock.unlock();
        if(previousAqState == 1)
            if(this->startAquisition() != EXIT_SUCCESS)
                return EXIT_FAILURE;
        
        return EXIT_SUCCESS;        
}

int JAI_AD080GE::startAquisition(void) {
    J_STATUS_TYPE   retval;
    if(this->aqstate == 0) {
      
    sem_init(&this->FullCount, 0, this->BUFFERCOUNT);
    sem_init(&this->EmptyCount, 0, 0);
    sem_init(&this->Mutex, 0, 1);
    this->ReadPointer = 0;
    this->WritePointer = 0;
      /*
        retval = J_Image_OpenStream(this->m_hCamMono, 
                      0, 
                      (J_IMG_CALLBACK_OBJECT)(this), 
                      (J_IMG_CALLBACK_FUNCTION)(&JAI_AD080GE::NIRCallback), 
                      &NIRCallBackThread, 
                      (this->width*this->height*this->bitDepth)/8);
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Setting Callback failed" << std::endl;
            return EXIT_FAILURE;
        }
        */
    //    FactoryLock.lock();
        retval = J_Image_OpenStream(this->m_hCam, 
                      0, 
                      (J_IMG_CALLBACK_OBJECT)(this), 
                      (J_IMG_CALLBACK_FUNCTION)(&JAI_AD080GE::Callback), 
                      &CallBackThread, 
                      (this->width*this->height*this->bitDepth)/8);
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Setting Callback failed" << std::endl;
    //                FactoryLock.unlock();
            return EXIT_FAILURE;
        }
    /*
        retval = J_Camera_ExecuteCommand((void*)m_hCamMono, (int8_t*)"AcquisitionStart");
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Start aquisition failed" << std::endl;
            return EXIT_FAILURE;
        }
        */
        retval = J_Camera_ExecuteCommand((void*)m_hCam, (int8_t*)"AcquisitionStart");
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Start aquisition failed" << std::endl;
    //                FactoryLock.unlock();
                    
            return EXIT_FAILURE;
        }
        this->aqstate = 1;
    //    FactoryLock.unlock();
//        if(!this->isRunning())
//          this->start();
    }
    return EXIT_SUCCESS;
}

int JAI_AD080GE::stopAquisition(void) {
    J_STATUS_TYPE   retval;
    if(this->aqstate == 1) {
      /*
        retval = J_Camera_ExecuteCommand((void*)m_hCamMono, (int8_t*)"AcquisitionStop");
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Stop aquisition failed" << std::endl;
            return EXIT_FAILURE;
        }
        */
   //     FactoryLock.lock();
        retval = J_Camera_ExecuteCommand((void*)m_hCam, (int8_t*)"AcquisitionStop");
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Stop aquisition failed" << std::endl;
     //               FactoryLock.unlock();
            return EXIT_FAILURE;
        }
        retval = J_Image_CloseStream(CallBackThread);
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Close image stream failed" << std::endl;
       //             FactoryLock.unlock();
            return EXIT_FAILURE;
        }
        /*
        retval = J_Image_CloseStream(NIRCallBackThread);
        if(retval != J_ST_SUCCESS) 
        {
		    std::cerr << "Close image stream failed" << std::endl;
            return EXIT_FAILURE;
        }
        */
      //  FactoryLock.unlock();
        this->aqstate = 1;
    }
    //Really should terminate through the use of a flag instead...
  //  this->setTerminationEnabled();
  //  this->terminate();
    return EXIT_SUCCESS;
}

int JAI_AD080GE::setNetworkPacketDelay(int delay) {
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    
    int64_t _delay = delay;

    //Setup packet delay
    /*
    retval = J_Camera_GetNodeByName(m_hCamMono, (int8_t *)"GevSCPD", &hNode);
    if (retval != J_ST_SUCCESS) {
       std::cerr << "Warning: Could not get the 'GevSCPSPacketdelay' GeniCam node! The packet size will not be changed" << std::endl << std::flush;
       return EXIT_FAILURE;
    }
    retval = J_Node_SetValueInt64(hNode, false, delay);
    if (retval != J_ST_SUCCESS) {
      std::cerr << "Error: Could not set the 'GevSCPSPacketdelay' value!" << std::endl << std::flush;
      return EXIT_FAILURE;
    }    
    */
  //  FactoryLock.lock();
    retval = J_Camera_GetNodeByName(m_hCam, (int8_t *)"GevSCPD", &hNode);
    if (retval != J_ST_SUCCESS) {
       std::cerr << "Warning: Could not get the 'GevSCPSPacketdelay' GeniCam node! The packet size will not be changed" << std::endl << std::flush;
    ///   FactoryLock.unlock();
       return EXIT_FAILURE;
    }
    retval = J_Node_SetValueInt64(hNode, true, delay);
    if (retval != J_ST_SUCCESS) {
      std::cerr << "Error: Could not set the 'GevSCPSPacketdelay' value!" << std::endl << std::flush;
     // FactoryLock.unlock();
      return EXIT_FAILURE;
    }
    retval = J_Node_GetValueInt64(hNode, true, &_delay);
    if (retval != J_ST_SUCCESS) {
      std::cerr << "Error: Could not get the 'GevSCPSPacketdelay' value!" << std::endl << std::flush;
     // FactoryLock.unlock();
      return EXIT_FAILURE;
    }
  //  FactoryLock.unlock();
    return EXIT_SUCCESS;
}

int JAI_AD080GE::setTrigger(JAI_AD080GE::trigger inputTrigger) {
    J_STATUS_TYPE   retval;

    //Set capture trigger
    /*
    retval = J_Camera_SetValueString(m_hCamMono, (int8_t*)"TriggerSelector", (int8_t*)"FrameStart");
    if (retval != J_ST_SUCCESS)
    {
	    std::cerr << "SetValueString Trigger Selector failed" << std::endl;
        return EXIT_FAILURE;
    }
    retval = J_Camera_SetValueString(m_hCamMono, (int8_t*)"TriggerMode", (int8_t*)"On");
    if (retval != J_ST_SUCCESS)
    {
	    std::cerr << "SetValueString Trigger Mode failed" << std::endl;
        return EXIT_FAILURE;
    }
    if(inputTrigger == opt1) {
        retval = J_Camera_SetValueString(m_hCamMono, (int8_t*)"TriggerSource", (int8_t*)"Line5");//Line5 = optical in 1
        if (retval != J_ST_SUCCESS)
        {
	        std::cerr << "SetValueString Trigger Source failed" << std::endl;
            return EXIT_FAILURE;
        }
    } else
    {
        std::cerr << "Unknown trigger input" << std::endl;
        return EXIT_FAILURE;
    }
    */
  //    FactoryLock.lock();
      retval = J_Camera_SetValueString(m_hCam, (int8_t*)"TriggerSelector", (int8_t*)"FrameStart");
      if (retval != J_ST_SUCCESS)
      {
              std::cerr << "SetValueString Trigger Selector failed" << std::endl;
 //             FactoryLock.unlock();
          return EXIT_FAILURE;
      }
      retval = J_Camera_SetValueString(m_hCam, (int8_t*)"TriggerMode", (int8_t*)"On");
      if (retval != J_ST_SUCCESS)
      {
              std::cerr << "SetValueString Trigger Mode failed" << std::endl;
   //           FactoryLock.unlock();
          return EXIT_FAILURE;
      }
  //    FactoryLock.unlock();
      changeTrigger(inputTrigger);
    return EXIT_SUCCESS;
}

int JAI_AD080GE::changeTrigger(JAI_AD080GE::trigger inputTrigger)
{
  J_STATUS_TYPE   retval;
  
  if(inputTrigger==JAI_AD080GE::internal)
  {
          std::cout << "Changing trigger to internal" << std::endl;
    //      FactoryLock.lock();
          retval = J_Camera_SetValueString(m_hCam, (int8_t*)"TriggerSource", (int8_t*)"PulseGenerator0");//Line5 = optical in 1
      //    FactoryLock.unlock();
          if (retval != J_ST_SUCCESS)
          {
              std::cerr << "SetValueString Trigger Source failed" << std::endl;
              return EXIT_FAILURE;
          }
  } else if(inputTrigger==JAI_AD080GE::opt1)
  {
          std::cout << "Changing trigger to external" << std::endl;
     //     FactoryLock.lock();
          retval = J_Camera_SetValueString(m_hCam, (int8_t*)"TriggerSource", (int8_t*)"Line5");//Line5 = optical in 1
    //      FactoryLock.unlock();
          if (retval != J_ST_SUCCESS)
          {
                  std::cerr << "SetValueString Trigger Source failed" << std::endl;
              return EXIT_FAILURE;
          }
  }
}


int JAI_AD080GE::setGenerator0FPS(float fps) {
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    //We should add valid range checks here

    //Input to the prescaler is fixed at 33.75MHz
  //  FactoryLock.lock();
    retval = J_Camera_GetNodeByName(m_hCam, (int8_t*)"ClockPreScaler", &hNode);
    retval = J_Node_SetValueInt64(hNode, false, 128); //A division of 128 equals 33750000÷128 = 263671,875Hz
    if (retval != J_ST_SUCCESS) {
      std::cerr << "Error: Could not set the 'Clock Pre Scaler' value!" << std::endl << std::flush;
  //    FactoryLock.unlock();
      return EXIT_FAILURE;
    }

    retval = J_Camera_SetValueString(m_hCam, (int8_t*)"PulseGeneratorSelector", (int8_t*)"PulseGenerator0");
    if (retval != J_ST_SUCCESS)
    {
        std::cerr << "Set PulseGeneratorSelector failed" << std::endl;
  //      FactoryLock.unlock();
        return EXIT_FAILURE;
    }
    // We have a input clock of 263671,875Hz, so 10000 equals 26,3671875Hz
    retval = J_Camera_GetNodeByName(m_hCam, (int8_t*)"PulseGeneratorLength", &hNode);
    //retval = J_Node_SetValueInt64(hNode, false, 20000);
    retval = J_Node_SetValueInt64(hNode, false, ceil(263671.875/fps));
    if (retval != J_ST_SUCCESS) {
      std::cerr << "Error: Could not set the 'pulse generator length' value!" << std::endl << std::flush;
  //    FactoryLock.unlock();
      return EXIT_FAILURE;
    }
    
    retval = J_Camera_GetNodeByName(m_hCam, (int8_t*)"PulseGeneratorEndPoint", &hNode);
    retval = J_Node_SetValueInt64(hNode, false, 1024);
    if (retval != J_ST_SUCCESS) {
      std::cerr << "Error: Could not set the 'pulse generator end point' value!" << std::endl << std::flush;
  //    FactoryLock.unlock();
      return EXIT_FAILURE;
    }
  //  FactoryLock.unlock();
    return EXIT_SUCCESS;
}

void JAI_AD080GE::setOptOut1Output(JAI_AD080GE::trigger inputTrigger) {
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    if(inputTrigger == generator0)
    {
   //     FactoryLock.lock();
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSelector", (int8_t*)"Line3");//Line3 = optical Out 1
        if (retval != J_ST_SUCCESS)
        {
	        std::cerr << "SetValueString Line Selector failed" << std::endl;
   //             FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
        
        
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSource", (int8_t*)"PulseGenerator0");//PulseGenerator0
        if (retval != J_ST_SUCCESS)
        {
	        std::cerr << "SetValueString Line Source failed" << std::endl;
     //           FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
     //   FactoryLock.unlock();
    }
    else if(inputTrigger == UserOutput0)
    {
     //   FactoryLock.lock();
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSelector", (int8_t*)"Line3");//Line3 = optical Out 1
        if (retval != J_ST_SUCCESS)
        {
                std::cerr << "SetValueString Line Selector failed" << std::endl;
     //           FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
        
        
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSource", (int8_t*)"UserOutput0");//PulseGenerator0
        if (retval != J_ST_SUCCESS)
        {
                std::cerr << "SetValueString Line Source failed" << std::endl;
     //           FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
     ///   FactoryLock.unlock();      
    }
    else {
        std::cerr << "trigger for output not yet implemented" << std::endl;
        return;// EXIT_FAILURE;
    }
    
    return;// EXIT_SUCCESS;
}

void JAI_AD080GE::setOptOut2Output(JAI_AD080GE::trigger inputTrigger) {
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    if(inputTrigger == generator0)
    {
     //   FactoryLock.lock();
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSelector", (int8_t*)"Line3");//Line3 = optical Out 1
        if (retval != J_ST_SUCCESS)
        {
	        std::cerr << "SetValueString Line Selector failed" << std::endl;
     //           FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
        
        
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSource", (int8_t*)"PulseGenerator0");//PulseGenerator0
        if (retval != J_ST_SUCCESS)
        {
	        std::cerr << "SetValueString Line Source failed" << std::endl;
       //         FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
      //  FactoryLock.unlock();
    }
    else if(inputTrigger == UserOutput1)
    {
      //  FactoryLock.lock();
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSelector", (int8_t*)"Line4");//Line4 = optical Out 2
        if (retval != J_ST_SUCCESS)
        {
                std::cerr << "SetValueString Line Selector failed" << std::endl;
      //          FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
        
        
        retval = J_Camera_SetValueString(m_hCam, (int8_t*)"LineSource", (int8_t*)"UserOutput1");//PulseGenerator0
        if (retval != J_ST_SUCCESS)
        {
                std::cerr << "SetValueString Line Source failed" << std::endl;
     //           FactoryLock.unlock();
            return;// EXIT_FAILURE;
        }
     //   FactoryLock.unlock();      
    }
    else {
        std::cerr << "trigger for output not yet implemented" << std::endl;
        return;// EXIT_FAILURE;
    }
    
    return;// EXIT_SUCCESS;
}

int JAI_AD080GE::setGain(float gain, CAM_HANDLE * sensor) {
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    const float gainMin = -3;
    const float gainMax = 21;
    int rawGain = 0;
    
    if(gain < gainMin) {
        std::cerr << "gain set below minimum" << std::endl;
        return EXIT_FAILURE;
    }
    else if (gain > gainMax) {
        std::cerr << "gain set above Maximum" << std::endl;
        return EXIT_FAILURE;
    }
    
    rawGain = floor((gain/0.0358)+0.5);

    //Set Gain
//    FactoryLock.lock();
    retval = J_Camera_GetNodeByName(*sensor, (int8_t*)"GainRaw", &hNode);
    retval = J_Node_SetValueInt64(hNode, 0, rawGain);
//    FactoryLock.unlock();
    if(retval != J_ST_SUCCESS) 
    {
	    std::cerr << "Set gain failed" << std::endl;
        return EXIT_FAILURE;
    }
   
   return EXIT_SUCCESS;
}

int JAI_AD080GE::setGain(float gain)
{
    return this->setGain(gain, &m_hCam);
}

int JAI_AD080GE::setNIRGain(float gain)
{
    return this->setGain(gain, &m_hCamMono);
}

/*
int JAI_AD080GE::NIRCallback(J_tIMAGE_INFO * pAqImageInfo) {
    void * img;
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    sem_wait(&this->NIRFullCount);
    sem_wait(&this->NIRMutex);
    int i = this->NIRWritePointer;
    if(this->NIRWritePointer < (this->BUFFERCOUNT-1)) {
        this->NIRWritePointer++;
    } else {
        this->NIRWritePointer = 0;
    }
    sem_post(&this->NIRMutex);
    this->NIRTimeBuffer[i] = t;
    img = this->NIRImageBuffer[i].pImageBuffer;
    memcpy(&this->NIRImageBuffer[i], pAqImageInfo, sizeof(*pAqImageInfo));
    this->NIRImageBuffer[i].pImageBuffer = (uint8_t *)img;
    memcpy(this->NIRImageBuffer[i].pImageBuffer, pAqImageInfo->pImageBuffer, 
            pAqImageInfo->iImageSize);
    sem_post(&this->NIREmptyCount);
}
*/

int JAI_AD080GE::Callback(J_tIMAGE_INFO * pAqImageInfo) {
    void * img;
    timespec t;
    if(this->previewEnabled==false)
      std::cout << "Received callback" << std::endl;
    clock_gettime(CLOCK_REALTIME, &t);
    sem_wait(&this->FullCount);
    sem_wait(&this->Mutex);
    int i = this->WritePointer;
    if(this->WritePointer < (this->BUFFERCOUNT-1)) {
        this->WritePointer++;
    } else {
        this->WritePointer = 0;
    }
    sem_post(&this->Mutex);
    this->TimeBuffer[i] = t;
    img = this->ImageBuffer[i].pImageBuffer;
    memcpy(&(this->ImageBuffer[i]), pAqImageInfo, sizeof(*pAqImageInfo));
    this->ImageBuffer[i].pImageBuffer = (uint8_t *)img;
    memcpy((this->ImageBuffer[i].pImageBuffer), pAqImageInfo->pImageBuffer, 
            pAqImageInfo->iImageSize);
    sem_post(&this->EmptyCount);
}

int JAI_AD080GE::pollImage(void* image) {
    sem_wait(&this->EmptyCount);
 //   sem_wait(&this->NIREmptyCount);
//     image->setImage(&this->ImageBuffer[ReadPointer],
//                     &this->TimeBuffer[ReadPointer], 
//                     this->serial);
    memcpy(image, (void*)(this->ImageBuffer[ReadPointer].pImageBuffer), (1024*768*3)/2);
    if(this->ReadPointer < (this->BUFFERCOUNT-1)) {
        this->ReadPointer++;
    } else {
        this->ReadPointer = 0;
    }
    /*
    if(this->NIRReadPointer < (this->BUFFERCOUNT-1)) {
        this->NIRReadPointer++;
    } else {
        this->NIRReadPointer = 0;
    }
    */
    sem_post(&this->FullCount);
  //  sem_post(&this->NIRFullCount);
    
    return EXIT_SUCCESS;
}

void JAI_AD080GE::run()
{
  void * img;
  img = malloc((1024*768*3)/2);
  while(true)
  {
    pollImage(img);
   // std::cout << "Received image" << std::endl;
    //emit newImage(img);
 //   img->setFocalLength(this->focalLength);
 //   img->setPixelSize(this->pixelSize);
//     if(previewEnabled)
//     {
//       emit newPreviewImage(img);
//     }
//     else
//     {
//       //Enable internal trigger
//  //     changeTrigger(Camera::internal);
//       this->previewEnabled = true;
//       std::cout << "Sending new image" << std::endl;
//       emit newImage(img);
//     }
  }
}

int JAI_AD080GE::DeviceReset(void) {
    J_STATUS_TYPE   retval;
    NODE_HANDLE     hNode;
    /*
    retval = J_Node_SetValueInt64(m_hCamMono, (int8_t*)"DeviceReset", 1);
    if(retval != J_ST_SUCCESS) 
    {
            std::cerr << "Device reset failed" << std::endl;
        return EXIT_FAILURE;
    }
    */
//    FactoryLock.lock();
    retval = J_Node_SetValueInt64(m_hCam, (int8_t*)"DeviceReset", 1);
//    FactoryLock.unlock();
    if(retval != J_ST_SUCCESS) 
    {
            std::cerr << "Device reset failed" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void JAI_AD080GE::captureWhenTrigger(JAI_AD080GE::trigger inputTrigger)
{
//  changeTrigger(inputTrigger);
//  this->yieldCurrentThread();
//  msleep(300);
  this->previewEnabled = false;
}

