////////////////////////////////////////////////////////////////////////////////////////////
/// \file      Jai_Factory.h
/// \brief     JAI SDK
/// \version   Revison: 1.2.5
/// \author    mat, kic (JAI)
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _JAI_FACTORY_H
#define _JAI_FACTORY_H

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XWDFile.h>
#endif

#include "windows.h"
#include "Jai_Types.h"
#include "Jai_Error.h"

#ifndef __linux__
   #ifdef FACTORY_EXPORTS
      #define GCFC_IMPORT_EXPORT __declspec(dllexport)
   #else
      #define GCFC_IMPORT_EXPORT __declspec(dllimport)
   #endif
#else
   #define GCFC_IMPORT_EXPORT
#endif
#ifdef __cplusplus
   #ifndef EXTERN_C
      #define EXTERN_C extern "C"
   #endif
#else
   #define EXTERN_C
#endif

#ifdef __linux__
   #define J_REG_DATABASE  "/usr/lib/Registry.xml"          ///< Default location of the JAI SDK Registry database file
#else
   #define J_REG_DATABASE  "$(JAI_SDK_BIN)\\Registry.xml"   ///< Default location of the JAI SDK Registry database file
#endif

#define J_ROOT_NODE "Root"                                  ///< Name of the topmost node in the GenICam Node tree

#define J_FACTORY_INFO_SIZE (512)   ///< Maximum size needed for factory information strings
#define J_CAMERA_ID_SIZE    (512)   ///< Maximum size needed for camera ID strings
#define J_CAMERA_INFO_SIZE  (512)   ///< Maximum size needed for camera information strings
#define J_STREAM_INFO_SIZE  (512)   ///< Maximum size needed for data stream information strings
#define J_CONFIG_INFO_SIZE  (512)   ///< Maximum size needed for config information strings



//---------------------------------------------------------------------------------------------------------------------------------------------------

#define   J_XGA_WIDTH       (1024)
#define   J_XGA_HEIGHT      (768)
#define   J_SXGA_WIDTH      (1280)
#define   J_SXGA_HEIGHT     (1024)
#define   J_NUM_OF_BUFFER   (10)
#define   J_MAX_BPP         (4)
#define   J_BUFFER_SIZE     (J_XGA_WIDTH * J_XGA_HEIGHT * J_MAX_BPP)

//===================================================
// PIXEL TYPES
//===================================================
/// @}
/// \defgroup WRAP_PIXELFORMATS   GigE Vision Pixel Formats
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The GigE Vision standard defines the pixel formats to be used by the GigE Vision Stream Protocol (GVSP). These values are the
/// values used for the mandatory GenICam feature "PixelFormat" and it indicates how the image data on the GVSP is supposed to be interpreted.
/// The value can be calculated by or'ing the color format, the size and a unique pixel id
// Indicate if pixel is monochrome or RGB
#define J_GVSP_PIX_MONO         0x01000000
#define J_GVSP_PIX_RGB          0x02000000
#define J_GVSP_PIX_COLOR        0x02000000
#define J_GVSP_PIX_CUSTOM       0x80000000
#define J_GVSP_PIX_COLOR_MASK   0xFF000000

// Indicate effective number of bits occupied by the pixel (including padding).
// This can be used to compute amount of memory required to store an image.
#define J_GVSP_PIX_OCCUPY8BIT   0x00080000
#define J_GVSP_PIX_OCCUPY12BIT  0x000C0000
#define J_GVSP_PIX_OCCUPY16BIT  0x00100000
#define J_GVSP_PIX_OCCUPY24BIT  0x00180000
#define J_GVSP_PIX_OCCUPY32BIT  0x00200000
#define J_GVSP_PIX_OCCUPY36BIT  0x00240000
#define J_GVSP_PIX_OCCUPY48BIT  0x00300000
#define J_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_MASK    0x00FF0000
#define J_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_SHIFT   16

// Pixel ID: lower 16-bit of the pixel type
#define J_GVSP_PIX_ID_MASK 0x0000FFFF

// 26.1 Mono buffer format defines
#define J_GVSP_PIX_MONO8         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY8BIT  | 0x0001) ///< 8-bit Monochrome pixel format (Mono8=0x01080001)
#define J_GVSP_PIX_MONO8_SIGNED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY8BIT  | 0x0002) ///< 8-bit Monochrome Signed pixel format (Mono8Signed=0x01080002)
#define J_GVSP_PIX_MONO10        (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0003) ///< 10-bit Monochrome pixel format (Mono10=0x01100003)
#define J_GVSP_PIX_MONO10_PACKED (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT | 0x0004) ///< 10-bit Monochrome Packed pixel format (Mono10Packed=0x010C0004)
#define J_GVSP_PIX_MONO12        (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0005) ///< 12-bit Monochrome pixel format (Mono12=0x01100005)
#define J_GVSP_PIX_MONO12_PACKED (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT | 0x0006) ///< 12-bit Monochrome Packed pixel format (Mono12Packed=0x010C0006)
#define	J_GVSP_PIX_MONO14        (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0025) ///< 14-bit Monochrome pixel format (Mono14=0x01100025)
#define J_GVSP_PIX_MONO16        (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0007) ///< 16-bit Monochrome pixel format (Mono16=0x01100007)

// 26.2 Bayer buffer format defines
#define J_GVSP_PIX_BAYGR8          (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY8BIT  | 0x0008) ///< 8-bit Bayer GR pixel format (BayerGR8=0x01080008)
#define J_GVSP_PIX_BAYRG8          (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY8BIT  | 0x0009) ///< 8-bit Bayer RG pixel format (BayerRG8=0x01080009)
#define J_GVSP_PIX_BAYGB8          (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY8BIT  | 0x000A) ///< 8-bit Bayer GB pixel format (BayerGB8=0x0108000A)
#define J_GVSP_PIX_BAYBG8          (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY8BIT  | 0x000B) ///< 8-bit Bayer BG pixel format (BayerBG8=0x0108000B)
#define J_GVSP_PIX_BAYGR10         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x000C) ///< 10-bit Bayer GR pixel format (BayerGR10=0x0110000C)
#define J_GVSP_PIX_BAYRG10         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x000D) ///< 10-bit Bayer RG pixel format (BayerRG10=0x0110000D)
#define J_GVSP_PIX_BAYGB10         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x000E) ///< 10-bit Bayer GB pixel format (BayerGB10=0x0110000E)
#define J_GVSP_PIX_BAYBG10         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x000F) ///< 10-bit Bayer BG pixel format (BayerBG10=0x0110000F)
#define J_GVSP_PIX_BAYGR12         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0010) ///< 12-bit Bayer GR pixel format (BayerGR12=0x01100010)
#define J_GVSP_PIX_BAYRG12         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0011) ///< 12-bit Bayer RG pixel format (BayerRG12=0x01100011)
#define J_GVSP_PIX_BAYGB12         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0012) ///< 12-bit Bayer GB pixel format (BayerGB12=0x01100012)
#define J_GVSP_PIX_BAYBG12         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0013) ///< 12-bit Bayer BG pixel format (BayerBG12=0x01100013)
#define J_GVSP_PIX_BAYGR16         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x002E) ///< 16-bit Bayer GR pixel format (BayerGR16=0x0110002E)
#define J_GVSP_PIX_BAYRG16         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x002F) ///< 16-bit Bayer RG pixel format (BayerRG16=0x0110002F) 
#define J_GVSP_PIX_BAYGB16         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0030) ///< 16-bit Bayer GB pixel format (BayerGB16=0x01100030) 
#define J_GVSP_PIX_BAYBG16         (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY16BIT | 0x0031) ///< 16-bit Bayer BG pixel format (BayerBG16=0x01100031)

#define J_GVSP_PIX_BAYGR10_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x0026) ///< 10-bit Bayer GR Packed pixel format (BayerGR10Packed=0x010C0026)
#define J_GVSP_PIX_BAYRG10_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x0027) ///< 10-bit Bayer RG Packed pixel format (BayerRG10Packed=0x010C0027)
#define J_GVSP_PIX_BAYGB10_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x0028) ///< 10-bit Bayer GB Packed pixel format (BayerGB10Packed=0x010C0028)
#define J_GVSP_PIX_BAYBG10_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x0029) ///< 10-bit Bayer BG Packed pixel format (BayerBG10Packed=0x010C0029)
#define J_GVSP_PIX_BAYGR12_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x002A) ///< 12-bit Bayer GR Packed pixel format (BayerGR12Packed=0x010C002A) 
#define J_GVSP_PIX_BAYRG12_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x002B) ///< 12-bit Bayer RG Packed pixel format (BayerRG12Packed=0x010C002B) 
#define J_GVSP_PIX_BAYGB12_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x002C) ///< 12-bit Bayer GB Packed pixel format (BayerGB12Packed=0x010C002C) 
#define J_GVSP_PIX_BAYBG12_PACKED  (J_GVSP_PIX_MONO | J_GVSP_PIX_OCCUPY12BIT  | 0x002D) ///< 12-bit Bayer BG Packed pixel format (BayerBG12Packed=0x010C002D) 

// 26.3 RGB Packed buffer format defines
#define J_GVSP_PIX_RGB8_PACKED    (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY24BIT | 0x0014) ///< 8-bit RGB Packed pixel format (RGB8Packed=0x02180014)
#define J_GVSP_PIX_BGR8_PACKED    (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY24BIT | 0x0015) ///< 8-bit BGR Packed pixel format (BGR8Packed=0x02180015)
#define J_GVSP_PIX_RGBA8_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY32BIT | 0x0016) ///< 8-bit RGBA Packed pixel format (RGBA8Packed=0x02200016)
#define J_GVSP_PIX_BGRA8_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY32BIT | 0x0017) ///< 8-bit BGRA Packed pixel format (BGRA8Packed=0x02200017)
#define J_GVSP_PIX_RGB10_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0018) ///< 10-bit RGB Packed pixel format (RGB10Packed=0x02300018)
#define J_GVSP_PIX_BGR10_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0019) ///< 10-bit BGR Packed pixel format (BGR10Packed=0x02300019)
#define J_GVSP_PIX_RGB12_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x001A) ///< 12-bit RGB Packed pixel format (RGB12Packed=0x0230001A)
#define J_GVSP_PIX_BGR12_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x001B) ///< 12-bit BGR Packed pixel format (BGR12Packed=0x0230001B)
#define J_GVSP_PIX_RGB16_PACKED   (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0033) ///< 16-bit RGB Packed pixel format (RGB16Packed=0x02300033)
#define J_GVSP_PIX_RGB10V1_PACKED (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY32BIT | 0x001C) ///< 10-bit RGB V1 Packed pixel format (RGB10V1Packed=0x0220001C)
#define J_GVSP_PIX_RGB10V2_PACKED (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY32BIT | 0x001D) ///< 10-bit RGB V2 Packed pixel format (RGB10V2Packed=0x0220001D)
#define J_GVSP_PIX_RGB12V1_PACKED (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY36BIT | 0x0034) ///< 12-bit RGB V1 Packed pixel format (RGB12V1Packed0x02240034)

// 26.4 YUV Packed buffer format defines
#define J_GVSP_PIX_YUV411_PACKED      (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY12BIT | 0x001E) ///< YUV 4:1:1 Packed pixel format (YUV411Packed=0x020C001E)
#define J_GVSP_PIX_YUV422_PACKED      (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY16BIT | 0x001F) ///< YUV 4:2:2 Packed pixel format (YUV42Packed=0x0210001F)
#define J_GVSP_PIX_YUV422_YUYV_PACKED (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY16BIT | 0x0032) ///< YUV 4:2:2 YUYV Packed pixel format (YUYVPacked=0x02100032)
#define J_GVSP_PIX_YUV444_PACKED      (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY24BIT | 0x0020) ///< YUV 4:4:4 Packed pixel format (YUV444Packed=0x02180020)

// 26.5 RGB Planar buffer format defines
#define J_GVSP_PIX_RGB8_PLANAR  (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY24BIT | 0x0021) ///< 8-bit RGB Planar pixel format (RGB8Planar=0x02180021)
#define J_GVSP_PIX_RGB10_PLANAR (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0022) ///< 10-bit RGB Planar pixel format (RGB10Planar=0x02300022)
#define J_GVSP_PIX_RGB12_PLANAR (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0023) ///< 12-bit RGB Planar pixel format (RGB12Planar=0x02300023)
#define J_GVSP_PIX_RGB16_PLANAR (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0024) ///< 16-bit RGB Planar pixel format (RGB16Planar=0x02300024)

// Internal use only
#define   J_GVSP_PIX_BGR16_PACKED_INTERNAL (J_GVSP_PIX_COLOR | J_GVSP_PIX_OCCUPY48BIT | 0x0000) ///< JAI SDK Internal 16-bit RGB pixel format

/// \brief   This macro calculated number of bytes per pixel. Be aware that this truncates to whole number of bytes so Packed Pixel formats will
/// not be calculated correctly if this macro is used for calculating memory requirements needed for an image!
#define J_BPP(x) (((uint32_t)x & J_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_MASK) >>(uint32_t)J_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_SHIFT)/8

/// \brief   This macro calculated number of bits per pixel. If you multiply this by width and height and divides by 8 then you will get the memory
/// needed to hold the image in memory.
#define J_BitsPerPixel(x) (((uint32_t)x & J_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_MASK) >>(uint32_t)J_GVSP_PIX_EFFECTIVE_PIXEL_SIZE_SHIFT)



// R/G/B structure
typedef struct _J_tDIB24_TYPE 
{
   BYTE   B;
   BYTE   G;
   BYTE   R;
} J_tDIB24;

#define   J_tBGR24   J_tDIB24

// Alpha/R/G/B structure
typedef struct _J_tDIB32_TYPE 
{
   J_tDIB24   BGR;
   BYTE   Alpha;
} J_tDIB32;

// Structure of word three data
typedef struct _J_tBGR48_TYPE 
{
#ifndef __linux__
   uint16_t   B16;
   uint16_t   G16;
   uint16_t   R16;
#else
   uint16_t   B_16;
   uint16_t   G_16;
   uint16_t   R_16;
#endif
} J_tBGR48;

class CJDummyClass
{
};
typedef CJDummyClass * J_IMG_CALLBACK_OBJECT;
typedef void (CJDummyClass::*J_IMG_CALLBACK_FUNCTION)(J_tIMAGE_INFO * pAqImageInfo);
#ifdef __linux__
   typedef void ( *J_STATIC_GVSP_CALLBACK_FUNCTION)(J_tIMAGE_INFO * pAqImageInfo);
   typedef void ( *J_NODE_CALLBACK_FUNCTION)(void * pNode);
#else
   typedef void (__stdcall *J_STATIC_GVSP_CALLBACK_FUNCTION)(J_tIMAGE_INFO * pAqImageInfo);
   typedef void (__stdcall *J_NODE_CALLBACK_FUNCTION)(void * pNode);
#endif


/// @}
/// \defgroup WRAP_FACTORY   Factory specific functions
/// @{
/// \brief The Factory is the entity that contains the whole JAI SDK functionality and in order to access any of the SDK functions the
/// Factory will have to be opened first using the \c J_Factory_Open() function. This will return a handle to the Factory
/// itself. This handle will be used later on in searcing for and accessing the GigE Vision devices. All the Factory specific functions are named 
/// \c J_Factory_...()
/// \code
/// 
/// FACTORY_HANDLE  hFactory;     // Factory Handle
/// CAM_HANDLE      hCamera;      // Camera Handle
/// J_STATUS_TYPE   retval;
/// bool8_t         bHasChanged;
/// uint32_t        nCameras;
/// int8_t          sCameraId[J_CAMERA_ID_SIZE];
/// uint32_t        size;
/// 
/// // Open the Factory
/// retval = J_Factory_Open("" , &hFactory);
/// if (retval == J_ST_SUCCESS)
/// {
///   // Search for cameras on all the networks
///   retval = J_Factory_UpdateCameraList(hFactory, &bHasChanged);
///   if (retval == J_ST_SUCCESS && bHasChanged)
///   {
///     // Get the number of cameras. This number can be larger than the actual camera count
///     // because the cameras can be detected through different driver types!
///     // This mean that we might need to filter the cameras in order to avoid dublicate
///     // references.
///     retval = J_Factory_GetNumOfCameras(hFactory, &nCameras);
///     if (retval == J_ST_SUCCESS && nCameras > 0)
///     {
///       // Run through the list of found cameras
///       for (uint32_t index = 0; index < nCameras; ++index)
///       {
///         size = sizeof(sCameraId);
///         // Get CameraID
///         retval = J_Factory_GetCameraIDByIndex(hFactory, index, sCameraId, &size);
///         if (retval == J_ST_SUCCESS)
///         {
///           printf("Camera %d = %s\n", index, sCameraId);
///         
///           // Open the camera
///           retval = J_Camera_Open(hFactory, sCameraId, &hCamera);
/// 
///           // ... do something
///              
///           // Close the camera again
///           retval = J_Camera_Close(hCamera);    
///         }
///       }
///     }
///   }
/// }
/// // Close the factory
/// retval = J_Factory_Close(hFactory);
/// 
/// \endcode

/////////////////////////////////////////////////////////////////////////////////////
/// \brief Factory information type enumeration.
/// \note This enum is used in the \c J_Factory_GetInfo() function to retrieve detailed information about the \c Jai_Factory.dll
/// \sa \c J_Factory_GetInfo()
/////////////////////////////////////////////////////////////////////////////////////
typedef enum _J_FACTORY_INFO_TYPE
{
   FAC_INFO_VERSION,       ///< \c Jai_Factory.dll Version
   FAC_INFO_BUILDDATE,     ///< \c Jai_Factory.dll Build date
   FAC_INFO_BUILDTIME,     ///< \c Jai_Factory.dll Build time
   FAC_INFO_MANUFACTURER   ///< \c Jai_Factory.dll Manufacturer info
} J_FACTORY_INFO;

/////////////////////////////////////////////////////////////////////////////////////
/// \brief Camera information type enumeration. This enumeration is used in \c J_Factory_GetCameraInfo() to acquire detailed information about each detected camera.
/// \note The camera connection does not need to be established in order to get this information. The information is retrieved during \b Device \b Discovery.
/// \sa \c J_Factory_GetCameraInfo()
/////////////////////////////////////////////////////////////////////////////////////
typedef enum _J_CAMERA_INFO_TYPE
{
   CAM_INFO_MANUFACTURER = 0,    //!< Manufacturer name
   CAM_INFO_MODELNAME,           //!< Model name
   CAM_INFO_IP,                  //!< IP address
   CAM_INFO_MAC,                 //!< MAC address
   CAM_INFO_SERIALNUMBER,        //!< Serial number
   CAM_INFO_USERNAME,            //!< User name
   CAM_INFO_INTERFACE_ID = 100,  //!< Interface name
} J_CAMERA_INFO;

//******************************************************************************************************************
/// \brief                        Open the factory and return a valid handle to be used in all other Factory functions
/// \param [in] psPrivateData     The name of the registry database. Usually \c J_REG_DATABASE should be specified.
/// \param [out] pHandle          Pointer to a variable in which a handle of the factory object is stored.
/// \retval                       Status code defined in the \c J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                         This function creates and opens a new factory object.
///                               \c J_Factory_Close() function must be called when the factory object is no longer needed.
/// \sa \c J_Factory_Close(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_Open(const int8_t* psPrivateData, FACTORY_HANDLE *pHandle);

//******************************************************************************************************************
/// \brief                        Close a previously opened factory handle
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open()
/// \retval                       Status code defined in the \c J_STATUS_CODES. If the function succeeds, returns \c J_ST_SUCCESS.
/// \note
/// When the factory is closed the underlaying transport layer and interfaces will be automatically closed and released as well
/// \sa  \c J_Factory_Open(),  \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_Close(FACTORY_HANDLE hFac);

//******************************************************************************************************************
/// \brief                        Get information about the \c Jai_Factory.dll
/// \param [in] info              The type of information from the factory that is requested
/// \param [out] pBuffer          Pointer to a buffer in which the factory information is stored.
/// \param [in,out] pSize         Specify the size of the buffer. It must be equal or greater then \c J_FACTORY_INFO_SIZE.
///                               The function sets the actual size of data stored into the buffer.
/// \retval                       Status code defined in the \c J_STATUS_CODES. If the function succeeds, returns \c J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample demonstrating how to read the version information from the factory
/// 
/// int8_t sVersion[J_FACTORY_INFO_SIZE];
/// uint32_t size = (uint32_t)sizeof(sVersion);
/// J_Factory_GetInfo(FAC_INFO_VERSION, sVersion, &size);
/// printf("Version of Factory = %s\n", sVersion);
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_GetInfo(J_FACTORY_INFO info, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                        Update a list of all cameras connected to interfaces in the factory object.
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open()
/// \param [out] bHasChanged      A bool that tells if there is found any new cameras
/// \retval                       Status code defined in the \c J_STATUS_CODES. If the function succeeds, returns \c J_ST_SUCCESS.
///
/// \par
/// \code
/// // C Sample that perform Device Discovery and opens all cameras found
/// 
/// J_STATUS_TYPE   retval;
/// bool8_t         bHasChange;
/// uint32_t        nCameras;
/// int8_t          sCameraId[J_CAMERA_ID_SIZE];
/// uint32_t        size;
/// 
/// // Perform Device Discovery
/// retval = J_Factory_UpdateCameraList(hFactory, &bHasChange);
/// if (retval == J_ST_SUCCESS)
/// {
///   // Get the number of cameras found
///   retval = J_Factory_GetNumOfCameras(hFactory, &nCameras);
///   if (retval == J_ST_SUCCESS && nCameras > 0)
///   {
///     // Run through the list of cameras found and get the unique camera ID's
///     for (uint32_t index = 0; index < nCameras; ++index)
///     {
///       size = sizeof(sCameraId);
///       retval = J_Factory_GetCameraIDByIndex(hFactory, index, sCameraId, &size);
///       if (retval == J_ST_SUCCESS)
///       {
///         CAM_HANDLE  hCamera;
///         // And open it
///         J_Camera_Open(hFactory, sCameraId, &hCamera);
///       }
///     }
///   }
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_UpdateCameraList(FACTORY_HANDLE hFac, bool8_t *bHasChanged);

//******************************************************************************************************************
/// \brief                        Get the number of cameras found during \b Device \b Discovery with \c J_Factory_UpdateCameraList().
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open() function.
/// \param [out] pNum             Pointer to a variable in which the number of cameras is stored.
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                         It is important to note that the same physical camera might show up multiple times if more
///                               than one driver is installed and active. Normally both the SocketDriver and the FilterDriver will
///                               be installed and if for instance a single camera is present on the network then the
///                               \c J_Factory_UpdateCameraList() function will find it twice - once via the SocketDriver and
///                               once via the FilterDriver and \c J_Factory_GetNumOfCameras() will return the number 2. It is then
///                               possible for the application to select which driver to use for the connection by opening the camera
///                               via a specific camera ID returned by \c J_Factory_GetCameraIDByIndex() using either index 0 or index 1.
///                               In order to determine the driver type used the application has to parse the camera ID string and 
///                               search for 'INT=>SD' if it is the SocketDriver and 'INT=>FD' if it is the FilterDriver.
///
/// \sa \c J_Factory_UpdateCameraList(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_GetNumOfCameras(FACTORY_HANDLE hFac, uint32_t *pNum);

//******************************************************************************************************************
/// \brief                       Get a string that uniquely identifies the camera, interface and the transport layer. \n
///                              This string will be used to open the camera later on with \c J_Camera_Open() \n
///                              ex: "TL->GevTL::INT->FD::MAC::...::DEV->CV-A10GE".
/// \param [in] hFac             Handle to a valid factory object, obtained by \c J_Factory_Open()
/// \param [in] iNum             Zero based index in the camera list. It must be smaller than the number obtained by \c J_Factory_GetNumOfCameras()
/// \param [out] pBuffer         Pointer to a buffer in which the camera ID is stored. 
/// \param [in,out] pSize        The size of the buffer. It must be equal or larger than J_CAMERA_ID_SIZE.
///                               The function sets the actual size of data stored into the buffer.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                         It is important to note that the same physical camera might show up multiple times if more
///                               than one driver is installed and active. Normally both the SocketDriver and the FilterDriver will
///                               be installed and if for instance a single camera is present on the network then the
///                               \c J_Factory_UpdateCameraList() function will find it twice - once via the SocketDriver and
///                               once via the FilterDriver and \c J_Factory_GetNumOfCameras() will return the number 2. It is then
///                               possible for the application to select which driver to use for the connection by opening the camera
///                               via a specific camera ID returned by \c J_Factory_GetCameraIDByIndex() using either index 0 or index 1.
///                               In order to determine the driver type used the application has to parse the camera ID string and 
///                               search for 'INT=>SD' if it is the SocketDriver and 'INT=>FD' if it is the FilterDriver.
/// \sa \c J_Camera_Open(), \c J_Factory_UpdateCameraList(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_GetCameraIDByIndex(FACTORY_HANDLE hFac, int32_t iNum, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                        Get detailed information about a camera
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open() function.
/// \param [in] pCameraId         Unique Camera ID obtained by \c J_Factory_GetCameraIDByIndex() function. 
/// \param [in] InfoId            The information type that is requested
/// \param [out] pBuffer          Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize         The size of the buffer. It must be equal or larger than J_CAMERA_INFO_SIZE.
///                               The function sets the actual size of data stored into the buffer.
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_CAMERA_INFO, \c J_Factory_GetCameraIDByIndex()
/// \note The camera connection does not need to be established in order to get this information. The information is retrieved during \b Device \b Discovery.
/// \code
/// // C Sample demonstrating how to read the manufacturer information from the camera
/// 
///  int8_t   sManufacturer[J_CAMERA_INFO_SIZE];
///  uint32_t size;
///
///  J_Factory_GetCameraInfo(hFactory, sCameraId, CAM_INFO_MANUFACTURER, sManufacturer, &size);
///
///  printf("Manufacturer = %s\n", sManufacturer);
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_GetCameraInfo(FACTORY_HANDLE hFac, int8_t* pCameraId, J_CAMERA_INFO InfoId, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                        Set Force IP configuration.
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open() function.
/// \param [in] ForceEnabled      =0: Force IP disabled, !=0: Force IP enabled
/// \note                         This function must be called before calling J_Factory_UpdateCameraList();
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_EnableForceIp(FACTORY_HANDLE hFac, uint32_t ForceEnabled);


/// @}
/// \defgroup WRAP_CAM   Camera specific functions
/// @{
/// \brief The Camera specific functions are all used for accessing the GigE Vision devices. In order to access all the
/// camera functions a camera connection will need to be established by "opening" the camera connection using the
/// \c J_Camera_Open() or \c J_Camera_OpenMc() functions.
/// When the camera connection is no longer needed then the \c J_Camera_Close() needs to be called in order to release the
/// resources allocated for the device.
/// All the camera specific functions are named \c J_Camera_...().

/// \brief Device access Flags, used by J_Camera_OpenMc
typedef enum _J_DEVICE_ACCESS_FLAGS_TYPE
{
   DEVICE_ACCESS_NONE = 0,       //!< Access denied
   DEVICE_ACCESS_READONLY = 1,   //!< Host has / can have Read Only access to the device
   DEVICE_ACCESS_CONTROL = 2,    //!< Host has / can have Control Mode access to the device
   DEVICE_ACCESS_EXCLUSIVE = 4,  //!< Host has / can have Exclusive Mode access to the device

   DEVICE_ACCESS_MAX_ID
} J_DEVICE_ACCESS_FLAGS;

/// \brief The J_CONFIG_INFO is used to obtain information about a camera configuration file (XML-file)
typedef enum _J_CONFIG_INFO_TYPE
{
   CONF_INFO_MODEL_NAME = 0,       //!< Model name (CV-A70GE...)
   CONF_INFO_VENDOR_NAME,          //!< Vendor name (JAI)
   CONF_INFO_TOOL_TIP,             //!< Tool tip. Short description about the configuration file
   CONF_INFO_STANDARD_NAME_SPACE,  //!< Standard name space (GigE Vision, IEEE...)
   CONF_INFO_GENAPI_VERSION,       //!< Version of the DLL's GenApi implementation
   CONF_INFO_SCHEMA_VERSION,       //!< GenICam Schema version number
   CONF_INFO_DEVICE_VERSION,       //!< Version of the configuration file
   CONF_INFO_PRODUCT_GUID,         //!< Product GUID. Unique identifier for the camera. This will be unique to the camera type
   CONF_INFO_VERSION_GUID          //!< Version GUID. Unique identifier for the XML-file version. This will help to identify the current XML-file version
} J_CONFIG_INFO;

/// @}
/// \defgroup WRAP_CONNECTION   Camera connection functions
/// \ingroup WRAP_CAM
/// @{
/// \brief The following functions are used for connection to cameras and disconnection from cameras

//******************************************************************************************************************
/// \brief                        Open a camera from a camera ID and return valid camera handle
///                               The ID can be camera ID returned from the \c J_Factory_GetCameraID() or a user defined name
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open() function.
/// \param [in] pCameraID         Camera ID obtained by \c J_Factory_GetCameraIDByIndex() function.
/// \param [out] hCam             Pointer to the variable in which the handle is stored.
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// When a cameras has been opened using \c J_Camera_Open() it needs to be closed using \c J_Camera_Close()
/// \sa \c J_Camera_Close(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_Open(FACTORY_HANDLE hFac, int8_t* pCameraID, CAM_HANDLE* hCam);

//******************************************************************************************************************
/// \brief                        Open a camera with multicasting from a camera ID and return valid camera handle
///                               The camera ID can be returned from the \c J_Factory_GetCameraID() or a user defined name
/// \param [in] hFac              Handle to a valid factory object, obtained by \c J_Factory_Open() function.
/// \param [in] pCameraID         Camera ID obtained by \c J_Factory_GetCameraIDByIndex() function.
/// \param [out] hCam             Pointer to the variable in which the handle is stored.
/// \param [in] iOpenFlags        Device access Flags
/// \param [in] iMcIP             IP address for the multicasting
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// When a cameras has been opened using \c J_Camera_Open() it needs to be closed using \c J_Camera_Close()
/// \sa \c J_Camera_Close(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_OpenMc(FACTORY_HANDLE hFac, int8_t* pCameraID, CAM_HANDLE* hCam, J_DEVICE_ACCESS_FLAGS iOpenFlags, uint32_t iMcIP);

//******************************************************************************************************************
/// \brief                        Close the camera object
/// \param [in] hCam              Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
///
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_Close(CAM_HANDLE hCam);

/// @}
/// \defgroup WRAP_DIRECT_NODE_ACCESS   Direct GenICam node value access functions
/// \ingroup WRAP_CAM
/// @{
/// \brief The following functions are used for directly reading and writing GenICam feature node valued inside cameras without
/// having to explicitly get the node handles. This is the easiest way to get and set the values of the GenICam features.

//******************************************************************************************************************
/// \brief                     Get the value of a GenICam feature node as \c int64_t type.
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \param [in] pValue         Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be read
/// The node type has to be \c IInteger, \c IEnumeration or \c IBoolean
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetValueInt64(CAM_HANDLE hCam, int8_t* sNodeName, int64_t *pValue);

//******************************************************************************************************************
/// \brief                     Get the value of a GenICam feature node as \c double type.
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \param [in] pValue         Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be read
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetValueDouble(CAM_HANDLE hCam, int8_t* sNodeName, double *pValue);

//******************************************************************************************************************
/// \brief                     Get the value of a GenICam feature node as \c int8_t array type.
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \param [in] ValueStr       Pointer to the variable in which the value is stored.
/// \param [in,out] pSize      Specify the size of the value, and then the function sets an actual size of stored value.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be read
/// The node type can be any type. The value will automatically be converted internally in the factory
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetValueString(CAM_HANDLE hCam, int8_t* sNodeName, int8_t* ValueStr, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                     Set the value of a GenICam feature node as \c int64_t type
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \param [in] Value          Value to set
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type has to be \c IInteger, \c IEnumeration or \c IBoolean
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_SetValueInt64(CAM_HANDLE hCam, int8_t* sNodeName, int64_t Value);

//******************************************************************************************************************
/// \brief                     Set the value of a GenICam feature node as \c double type
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \param [in] Value          Value to set
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_SetValueDouble(CAM_HANDLE hCam, int8_t* sNodeName, double Value);

//******************************************************************************************************************
/// \brief                     Set the value of a GenICam feature node as \c string type
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \param [in] ValueStr       Value to set represented as a zero-terminated string
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_SetValueString(CAM_HANDLE hCam, int8_t* sNodeName, int8_t* ValueStr);

//******************************************************************************************************************
/// \brief                     Execute GenICam feature node command
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the GenICam feature node
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type has to be \c ICommand
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_ExecuteCommand(CAM_HANDLE hCam, int8_t* sNodeName);

/// @}
/// \defgroup WRAP_DIRECT_REGISTER_ACCESS   Direct register access functions
/// \ingroup WRAP_CAM
/// @{
/// \brief The following functions are used for reading and writing registers valued inside cameras without
/// using the GenICam feature nodes

//******************************************************************************************************************
/// \brief                       Read data from a register or memory in the camera
/// \param [in] hCam             Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] iAddress         Address to a register or memory in the camera
/// \param [out] pData           Pointer to a buffer in which the read data is stored.
/// \param [in,out] pSize        Specify the size of the data, and then the function sets an actual size of read data.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before read and write operations can be performed
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_ReadData(CAM_HANDLE hCam, int64_t iAddress, void* pData, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                       Write data to a register or memory in the camera
/// \param [in] hCam             Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] iAddress         Address to a register or memory in the camera
/// \param [in] pData            Data to be written
/// \param [in,out] pSize        Specify the size of the data, and then the function sets an actual size of written data.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before read and write operations can be performed
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_WriteData(CAM_HANDLE hCam, int64_t iAddress, const void* pData, uint32_t* pSize);

/// @}
/// \defgroup WRAP_GET_INFO   Configuration information function
/// \ingroup WRAP_CAM
/// @{
/// \brief The following function is used for accessing the XML-file configuration information about a camera.

//******************************************************************************************************************
/// \brief                     Retrieve information about the GenICam XML-file configuration loaded for the device
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] cinfo          The type of information requested
/// \param [out] pBuffer       Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize      Specify the size of the buffer. It must be equal or greater then J_CONFIG_INFO_SIZE.
///                            The function sets the actual size of data stored into the buffer.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the information about the GenICam XML-file can be retrieved
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetConfigInfo(CAM_HANDLE hCam, J_CONFIG_INFO cinfo, int8_t* pBuffer, uint32_t* pSize); 

/// @}
/// \defgroup WRAP_NODE_ACCESS   GenICam Feature Node access functions
/// \ingroup WRAP_CAM
/// @{
/// \brief The following functions are used for accessing the GenICam feature nodes from the XML-file loaded from the camera.

//******************************************************************************************************************
/// \brief                     Get the number of nodes in the entire GenICam node map for the camera
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [out] pNum          Pointer to a variable in which the number of nodes is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the number of nodes can be read
/// \par
/// \code
/// // C Sample enumerates all GenICam nodes in a camera and prints out node names
///
/// uint32_t       nNodes;
/// J_STATUS_TYPE  retval;
/// NODE_HANDLE    hNode;
/// int8_t         sNodeName[256];
/// uint32_t       size;
///  
/// // Get the number of nodes
/// retval = J_Camera_GetNumOfNodes(hCamera, &nNodes);
/// if (retval == J_ST_SUCCESS)
/// {
///   printf("%u nodes were found\n", nNodes);
///   // Run through the list of nodes and print out the names
///   for (uint32_t index = 0; index < nNodes; ++index)
///   {
///     // Get node handle
///     retval = J_Camera_GetNodeByIndex(hCamera, index, &hNode);
///     if (retval == J_ST_SUCCESS)
///     {
///       // Get node name
///       size = sizeof(sNodeName);
///       retval = J_Node_GetName(hNode, sNodeName, &size, 0);
///       if (retval == J_ST_SUCCESS)
///       {
///         // Print out the name
///         printf("%u NodeName = %s\n", index, sNodeName);
///       }
///     }
///   }
/// }
/// \endcode
///  \sa \c J_Camera_Open(), \c J_Node_GetName(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetNumOfNodes(CAM_HANDLE hCam, uint32_t* pNum);

//******************************************************************************************************************
/// \brief                     Get the node by the node map index. Valid indices will be between 0 and the value returned using \c J_Camera_GetNumOfNodes()
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] index          Zero based index of the node in the entire node map.
///                            It must be smaller than the number obtained by \c J_Camera_GetNumOfNodes() function.
/// \param [out] phNode        Pointer to a variable in which the handle of the node object is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the nodes can be opened
/// \par
/// \code
/// // C Sample enumerates all GenICam nodes in a camera and prints out node names
///
/// uint32_t        nNodes;
/// J_STATUS_TYPE   retval;
/// NODE_HANDLE     hNode;
/// int8_t          sNodeName[256];
/// uint32_t        size;
///  
/// // Get the number of nodes
/// retval = J_Camera_GetNumOfNodes(hCamera, &nNodes);
/// if (retval == J_ST_SUCCESS)
/// {
///   printf("%u nodes were found\n", nNodes);
///   // Run through the list of nodes and print out the names
///   for (uint32_t index = 0; index < nNodes; ++index)
///   {
///     // Get node handle
///     retval = J_Camera_GetNodeByIndex(hCamera, index, &hNode);
///     if (retval == J_ST_SUCCESS)
///     {
///       // Get node name
///       size = sizeof(sNodeName);
///       retval = J_Node_GetName(hNode, sNodeName, &size, 0);
///       if (retval == J_ST_SUCCESS)
///       {
///         // Print out the name
///         printf("%u NodeName = %s\n", index, sNodeName);
///       }
///     }
///   }
/// }
/// \endcode
///  \sa \c J_Camera_GetNumOfNodes(), \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetNodeByIndex(CAM_HANDLE hCam, uint32_t index, NODE_HANDLE* phNode);

//******************************************************************************************************************
/// \brief                     Get node by the GenICam node name
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sNodeName      Name of the node requested
/// \param [out] phNode        Pointer to a variable in which the handle of the node object is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the nodes can be opened
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetNodeByName(CAM_HANDLE hCam, int8_t* sNodeName, NODE_HANDLE * phNode);

//******************************************************************************************************************
/// \brief                       Get the number of sub feature nodes in the given node map tree specified by a parent node name
/// \param [in] hCam             Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sParentNodeName  Name of the parent node name in the node map tree.
/// \param [out] pNum            Pointer to a variable in which the number of nodes is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the number of sub feature nodes can be read
/// \par
/// \code
/// // C Sample enumerates all GenICam sub feature nodes in the root tree of a camera and prints out the node names
///
///  J_STATUS_TYPE   retval;
///  uint32_t        nFeatureNodes;
///  NODE_HANDLE     hNode;
///  int8_t          sNodeName[256];
///  uint32_t        size;
/// 
///  retval = J_Camera_GetNumOfSubFeatures(hCamera, J_ROOT_NODE, &nFeatureNodes);
///  if (retval == J_ST_SUCCESS)
///  {
///    printf("%u subfeature nodes were found in the Root node\n", nFeatureNodes);
///    // Run through the list of feature nodes and print out the names
///    for (uint32_t index = 0; index < nFeatureNodes; ++index)
///    {
///      // Get subfeature node handle
///      retval = J_Camera_GetSubFeatureByIndex(hCamera, J_ROOT_NODE, index, &hNode);
///      if (retval == J_ST_SUCCESS)
///      {
///        // Get subfeature node name
///        size = sizeof(sNodeName);
///        retval = J_Node_GetName(hNode, sNodeName, &size, 0);
///        if (retval == J_ST_SUCCESS)
///        {
///          // Print out the name
///          printf("%u Feature Name = %s\n", index, sNodeName);
///        }
///      }
///    }
///  }
/// \endcode
///  \sa \c J_Camera_Open(), \c J_Node_GetName(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetNumOfSubFeatures(CAM_HANDLE hCam, int8_t* sParentNodeName, uint32_t *pNum);

//******************************************************************************************************************
/// \brief                       Get a handle to the feature node object by the index in the given node map
/// \param [in] hCam             Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sParentNodeName  The name of the parent node
/// \param [in] index            Zero based index of the node requested
///                              It must be smaller than the number obtained by \c J_Camera_GetNumOfSubFeatures() function.
/// \param [out] phNode          Pointer to a variable in which the handle of the feature node object is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the sub feature nodes can be opened
/// \par
/// \code
/// // C Sample enumerates all GenICam sub feature nodes in the root tree of a camera and prints out the node names
///
///  J_STATUS_TYPE   retval;
///  uint32_t        nFeatureNodes;
///  NODE_HANDLE     hNode;
///  int8_t          sNodeName[256];
///  uint32_t        size;
/// 
///  retval = J_Camera_GetNumOfSubFeatures(hCamera, J_ROOT_NODE, &nFeatureNodes);
///  if (retval == J_ST_SUCCESS)
///  {
///    printf("%u subfeature nodes were found in the Root node\n", nFeatureNodes);
///    // Run through the list of feature nodes and print out the names
///    for (uint32_t index = 0; index < nFeatureNodes; ++index)
///    {
///      // Get subfeature node handle
///      retval = J_Camera_GetSubFeatureByIndex(hCamera, J_ROOT_NODE, index, &hNode);
///      if (retval == J_ST_SUCCESS)
///      {
///        // Get subfeature node name
///        size = sizeof(sNodeName);
///        retval = J_Node_GetName(hNode, sNodeName, &size, 0);
///        if (retval == J_ST_SUCCESS)
///        {
///          // Print out the name
///          printf("%u Feature Name = %s\n", index, sNodeName);
///        }
///      }
///    }
///  }
/// \endcode
///  \sa \c J_Camera_Open(), \c J_Node_GetName(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetSubFeatureByIndex(CAM_HANDLE hCam, int8_t* sParentNodeName, uint32_t index, NODE_HANDLE * phNode);

//******************************************************************************************************************
/// \brief                       Get feature node by the name
/// \param [in] hCam             Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sFeatureNodeName Name of the feature node.
/// \param [out] phNode          Pointer to a variable in which the handle of the feature node object is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the feature nodes can be opened
/// \sa \c J_Camera_Open(), \c J_Node_GetName(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetFeatureByName(CAM_HANDLE hCam, int8_t* sFeatureNodeName, NODE_HANDLE* phNode);

//******************************************************************************************************************
/// \brief                     Invalidates all GenICam nodes
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the feature nodes can be opened
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_InvalidateNodes(CAM_HANDLE hCam);

/// @}
/// \defgroup WRAP_DATASTREM_ACCESS   Image Data Stream access functions
/// \ingroup WRAP_CAM
/// @{
/// \brief The following functions are used for accesing the Image Data Streams for a camera.

//******************************************************************************************************************
/// \brief                     Get the number of available Data Streams from the Camera
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [out] pNum          Pointer to a variable in which the number of data streams is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the number of data streams can be read. The number of data 
/// streams are used to check the available data streams so the stream channel index used in \c J_Camera_CreateDataStream()
/// is within leagal range.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetNumOfDataStreams(CAM_HANDLE hCam, uint32_t *pNum);

//******************************************************************************************************************
/// \brief                     Create and open data stream channel to the camera
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] iChannel       Data stream channel number. Must be within 0 and the value returned by \c J_Camera_GetNumOfDataStreams()
/// \param [out] pDS           Pointer to a variable in which the data streams handle is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the data stream can be created.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_CreateDataStream(CAM_HANDLE hCam, uint32_t iChannel, STREAM_HANDLE *pDS);

//******************************************************************************************************************
/// \brief                     Create and open data stream channel to the camera
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] iChannel       Data stream channel number. Must be within 0 and the value returned by \c J_Camera_GetNumOfDataStreams()
/// \param [out] pDS           Pointer to a variable in which the data streams handle is stored.
/// \param [in] iMcIP          Multicast IP address. It will be ignored if it is 0
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the data stream can be created.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_CreateDataStreamMc(CAM_HANDLE hCam, uint32_t iChannel, STREAM_HANDLE *pDS, DWORD iMcIP=0);


/// @}
/// \defgroup WRAP_DATASTREAM   Data Stream specific functions
/// @{
/// \brief The Data Stream specific functions are used for controlling the Image Acquisition manually from the user application.
/// \par
/// The JAI SDK has got two major ways of handling the Image Acqusition:\n
/// 1) Automatic Image Acquisition handling via the \c J_Image_OpenStream(), \c J_Image_OpenStreamLight(), \c J_Image_CloseStream() and
/// \c J_Image_GetStreamInfo() functions. The JAI SDK will automatically take care of image buffer allocation as well as creating an Image
/// acquisition thread and all the images will be accessible via callback routines.\n
/// 2) Manual Image Acquisition handling via the \c J_DataStream_...() function described in this section. This requires that the user
/// application take care of all necessary memory allocation as well as creating an image acquisition thread the wait for "new image" events
/// from the underlying drivers. The biggest advantage is that this will give the application full control but this also increases the
/// complexity of the application.
/// \par
/// The basic flow for using the Data Stream specific functions is:\n
/// 1) Allocate memory for image buffers (using standard Malloc or similar)\n
/// 2) Hand over the allocated buffers to the acquisition engine inside the driver using the \c J_DataStream_AnnounceBuffer() \n
/// 3) Make the announced buffers available for image acquisition using the \c J_DataStream_QueueBuffer() \n
/// 4) Create an acquisition thread that will handle all image events from the drivers. The acquisition thread will initially create a condition
/// object using \c J_Event_CreateCondition() and register this new condition by the driver using \c J_DataStream_RegisterEvent(). After that it
/// start the acquisition engine inside the driver and create the stream thread connection to the camera using \c J_DataStream_StartAcquisition().
/// Afterward it will wait for signals on the condition using \c J_Event_WaitForCondition(). For each new event the thread calls
/// \c J_DataStream_GetBufferInfo() and when it is done with the event then it returns the buffer back to the acquisition queue 
/// using \c J_DataStream_QueueBuffer() \n
/// 5) When the image acquisition thread is stopped then the application have to clean up everything. The image acquisition thread inside the
/// driver is stopped using the \c J_DataStream_StopAcquisition().
/// 6) The image buffers are returned from the driver by initially calling \c J_DataStream_FlushQueue() and the calling \c J_DataStream_RevokeBuffer()
/// for each buffer. \n
/// 7) The allocated memory is now ready to be Freed
/// \par 
/// Please refer to the Stream Thread Sample application for more details!


/// \brief The J_ACQ_QUEUE_TYPE is used to select queue type to be flushed using \c J_DataStream_FlushQueue()
typedef enum  _J_ACQ_QUEUE_TYPE_TYPE
{
   ACQ_QUEUE_INPUT_TO_OUTPUT   = 0, ///< Flush queue of frames, waiting for new data through the acquisition engine to the output queue
   ACQ_QUEUE_OUTPUT_DISCARD    = 1  ///< Flush queue of frames, containing new data, waiting to be delivered
   //ACQ_QUEUE_OUTPUT_TO_INPUT   = 2 ///< Flush queue of frames, containing new data, waiting to be delivered
} J_ACQ_QUEUE_TYPE;

/// \brief The J_ACQ_START_FLAGS are used to control the start of image acquisition using \c J_DataStream_StartAcquisition()
typedef enum  _J_ACQ_START_FLAGS_TYPE
{
   ACQ_START_FLAGS_NONE  = 0,    ///< No flags.       
   ACQ_START_NEXT_IMAGE  = 0x1,  ///< Deliver the image acquired after the image delivered last <br>
                                 ///< Missed images only happen if the acquisiton queue is empty and therefore no buffers available
   ACQ_START_LAST_IMAGE  = 0x2,  ///< Deliver the last complete image acquired <br>
                                 ///< This means that between the image deliverd last and the image delivered next can be a number of missed images <br>
                                 ///< Missed images happen if one processing cycle takes to long <br>
                                 ///< The positive effect is that the acquired image is not out of the loop but max one image behind
   ACQ_START_NEW_IMAGE   = 0x3   ///< Deliver the new image acquired while the wait for buffer function is called <br>
                                 ///< The positive effect is that the acquired image is not out of the loop
} J_ACQ_START_FLAGS;

/// \brief The J_ACQ_STOP_FLAGS are used to control the start of image acquisition using \c J_DataStream_StartAcquisition()
typedef enum  _J_ACQ_STOP_FLAGS_TYPE
{
   /// So Stop Flag set
   ACQ_STOP_FLAGS_NONE = 0,      ///< No stop flags  
   ACQ_STOP_FLAG_KILL = 1        ///< Kill ongoing acquisition instead of waiting for the next image
} J_ACQ_STOP_FLAGS;


/// \brief The J_BUFFER_INFO_CMD is used to get static buffer information \c J_DataStream_GetBufferInfo()
typedef enum _J_BUFFER_INFO_CMD_TYPE
{
   BUFFER_INFO_BASE,               ///< void*    Base address of delivered buffer
   BUFFER_INFO_SIZE,               ///< uint32_t Size in Bytes
   BUFFER_INFO_USER_PTR,           ///< void*    Private Pointer
   BUFFER_INFO_TIMESTAMP,          ///< uint64_t Timestamp
   BUFFER_INFO_NUMBER,             ///< (Not available now) Buffer Number as announced 
   BUFFER_INFO_NEW_DATA,           ///< (Not available now) Flag if Buffer contains new data since it was queued 
   BUFFER_INFO_ISQUEUED,           ///< uint32_t
   BUFFER_INFO_PAYLOADTYPE,        ///< uint32_t
   BUFFER_INFO_PIXELTYPE,          ///< uint32_t
   BUFFER_INFO_WIDTH,              ///< uint32_t
   BUFFER_INFO_HEIGHT,             ///< uint32_t
   BUFFER_INFO_XOFFSET,            ///< uint32_t
   BUFFER_INFO_YOFFSET,            ///< uint32_t
   BUFFER_INFO_XPADDING,           ///< uint32_t
   BUFFER_INFO_YPADDING,           ///< uint32_t
   BUFFER_INFO_NUM_PACKETS_MISSING,///< uint32_t
   BUFFER_INFO_BLOCKID             ///< uint32_t
} J_BUFFER_INFO_CMD;

/// \brief The STREAM_INFO_CMD is used to get acquisition information using \c J_DataStream_GetStreamInfo(), \c J_Image_GetStreamInfo()
typedef enum _J_STREAM_INFO_CMD_TYPE
{
   STREAM_INFO_CMD_NONE                                 = 0,

   STREAM_INFO_CMD_ISGRABBING                           = 1,   ///< This is a bool8_t value. This command inquire if the acquisition engine is running or not. If the value returned is 0 the acquisition engine is currently stopped. 
   STREAM_INFO_CMD_NUMBER_OF_FRAMES_DELIVERED           = 2,   ///< This is a uint64_t value. This command inquire the number of acquired frames since the last acquisition start.  
   STREAM_INFO_CMD_NUMBER_OF_FRAMES_LOST_QUEUE_UNDERRUN = 3,   ///< This is a uint64_t value. This command inquire the number of lost frames due to a queue under-run. If the application is not emptying the acquisition queue as fast as the queue gets filled then the newly acquired frames will be dropped and this value will be increased by 1.
   STREAM_INFO_CMD_NUMBER_OF_FRAMES_ANNOUNCED           = 4,   ///< This is a uint64_t value. This command inquire the number of frame buffers that have been announced to the acquisition engine using \c J_DataStream_AnnounceBuffer(). This will be the maximum number of frame buffers in the system at any point in time.
   STREAM_INFO_CMD_NUMBER_OF_FRAMES_QUEUED              = 5,   ///< This is a uint64_t value. This command inquire the number of frame buffers available and ready for image acquisition. When a new frame is acquired then it will be moved from this queue into the delivery queue and this value will be decreased by 1. The application can return the frame buffer to this queue using \c J_DataStream_QueueBuffer() after handling the new image delivery.
   STREAM_INFO_CMD_NUMBER_OF_FRAMES_AWAIT_DELIVERY      = 6,   ///< This is a uint64_t value. This command inquire the number of frames that have been put into the delivery queue and are ready to be read and processed by the application. When the application gets the image using \c J_Event_GetData() then the frame buffer will automatically be removed from this delivery queue and the application then have to manually call \c J_DataStream_QueueBuffer() to return the buffer to the acquisition queue again.
   STREAM_INFO_CMD_NUMBER_OF_FRAMES_CORRUPT_ON_DELIEVRY = 7,   ///< This is a uint64_t value. This command inquire the number of frames which have been added to the delivery queue despite the fact that they are missing one or more packets. This will only happen if the \c STREAM_PARAM_CMD_PASS_CORRUPT_FRAMES parameter has been set to true using \c J_DataStream_SetStreamParam(). 

   STREAM_INFO_CMD_CUSTOM_INFO                          = 1000 ///< Every command with an id above this is for custom TLC use
} J_STREAM_INFO_CMD;


/// \brief The J_STREAM_PARAM_CMD is used to get/set parameters of streaming using \c J_DataStream_GetStreamParam() and \c J_DataStream_SetStreamParam()
typedef enum _J_STREAM_PARAM_CMD_TYPE
{
   STREAM_PARAM_CMD_CAN_RESEND = 0x1,                    //!< This is a uint32_t flag indicating if the source can handle resends. !=0: Enable resends(default), =0: Disable resends
   STREAM_PARAM_CMD_TOTALNUMBEROFBYTESPERFRAME ,         //!< xx
   STREAM_PARAM_CMD_PASS_CORRUPT_FRAMES,                 //!< This is a uint32_t flag indicating if corrupt frames are passed to the application or not.
   STREAM_PARAM_CMD_MAX_FRAMES_IN_NOT_COMPLETE_LIST,     //!< This is a uint32_t max number of frames in not complete list 
   STREAM_PARAM_CMD_RECIEVE_TIMEOUTS_BEFORE_LISTFLUSH,   //!< This is a uint32_t number of recieve timeouts before incomplete list is flushed / delivered
   STREAM_PARAM_CMD_OOO_PACKETS_BEFORE_RESEND,           //!< This is a uint32_t Out of order Packets before resend is issued
   STREAM_PARAM_CMD_RECIEVE_TIMEOUT,                     //!< This is a uint32_t Timeout in ms for recieve
   STREAM_PARAM_CMD_MAX_ID
}  J_STREAM_PARAM_CMD;

//******************************************************************************************************************
/// \brief                     Announce buffer pointer for the acquisition engine
/// \param [in] hDS            Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] pBuffer        Pointer to a allocated memory buffer to hold image data 
/// \param [in] uiSize         Size of the allocated memory buffer 
/// \param [in] pPrivate       Pointer to private user data to be attached to the image buffer 
/// \param [out] hBuf          Pointer to a variable in which the buffer handle is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The data stream needs to be created using \c J_Camera_CreateDataStream() before buffers can be announced.
/// \code
/// //==============================================================////
/// // Prepare frame buffers for the data stream
/// //==============================================================////
/// uint32_t CStreamThread::PrepareBuffers(int bufferCount, uint32_t bufferSize, void *pPrivateData)
/// {
///   J_STATUS_TYPE   iResult = J_ST_SUCCESS;
///   int             i;
///
///   m_iValidBuffers = 0;
///
///   for(i = 0 ; i < bufferCount ; i++)
///   {
///     // Make the buffer for one frame. 
///     m_pAquBuffer[i] = new uint8_t[bufferSize];
///
///     // Announce the buffer pointer to the Acquisition engine.
///     if(J_ST_SUCCESS!=J_DataStream_AnnounceBuffer(m_hDS,
///                                                  m_pAquBuffer[i],
///                                                  bufferSize,
///                                                  pPrivateData,
///                                                  &(m_pAquBufferID[i])))
///     {
///       delete m_pAquBuffer[i];
///       break;
///     }
///
///     // Queueing it.
///     if(J_ST_SUCCESS != J_DataStream_QueueBuffer(m_hDS, m_pAquBufferID[i]))
///     {
///       delete m_pAquBuffer[i];
///       break;
///     }
///
///     m_iValidBuffers++;
///   }
///
///   return m_iValidBuffers;
/// }
/// \endcode
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_AnnounceBuffer(STREAM_HANDLE hDS, void *pBuffer, uint32_t uiSize, void *pPrivate, BUF_HANDLE *hBuf);


//******************************************************************************************************************
/// \brief                     Flush Queues
/// \param [in] hDS            Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] QueueType      Queue type to Flush 
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The data stream needs to be created using \c J_Camera_CreateDataStream() before queues can be flushed.
/// \code
/// //==============================================================////
/// // Unprepare buffers
/// //==============================================================////
/// BOOL CStreamThread::UnPrepareBuffers(void)
/// {
///   void      *pPrivate;
///   void      *pBuffer;
///   uint32_t   i;
///
///   // Flush Queues
///   J_DataStream_FlushQueue(m_hDS, ACQ_QUEUE_INPUT_TO_OUTPUT);
///   J_DataStream_FlushQueue(m_hDS, ACQ_QUEUE_OUTPUT_DISCARD);
///   
///   for(i = 0 ; i < m_iValidBuffers ; i++)
///   {
///     // Remove the frame buffer from the Acquisition engine.
///     J_DataStream_RevokeBuffer(m_hDS, m_pAquBufferID[i], &pBuffer , &pPrivate);
///
///     delete m_pAquBuffer[i];
///     m_pAquBuffer[i] = NULL;
///     m_pAquBufferID[i] = 0;
///   }
///
///   m_iValidBuffers = 0;
///
///   return TRUE;
/// }
/// \endcode
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_FlushQueue(STREAM_HANDLE hDS, J_ACQ_QUEUE_TYPE QueueType);


//******************************************************************************************************************
/// \brief                 Start the image acquisition on the stream channel
/// \param [in] hDS        Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iFlags     Aquisition start control flag.
/// \param [in] iNumImages Number of images to acquire. The value 0xFFFFFFFFFFFFFFFF indicates unlimited.
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The data stream needs to be created using \c J_Camera_CreateDataStream() and buffers needs to be announced using 
/// \c J_DataStream_AnnounceBuffer() before acquisition can be started.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_StartAcquisition(STREAM_HANDLE hDS, J_ACQ_START_FLAGS iFlags = ACQ_START_FLAGS_NONE, uint64_t iNumImages = 0xFFFFFFFFFFFFFFFFLL);

//******************************************************************************************************************
/// \brief                 Stop the image acquisition on the stream channel
/// \param [in] hDS        Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iFlags     Aquisition stop control flag.
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_StopAcquisition(STREAM_HANDLE hDS, J_ACQ_STOP_FLAGS iFlags = ACQ_STOP_FLAGS_NONE);

//******************************************************************************************************************
/// \brief                      Get detailed information about a data stream
/// \param [in] hDS             Handle to a valid data stream object, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iCmd            The information type that is requested
/// \param [out] pBuffer        Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize       The size of the buffer. It must be equal or larger than J_STREAM_INFO_SIZE.
///                             The function sets the actual size of data stored into the buffer.
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_STREAM_INFO_CMD
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_GetStreamInfo(STREAM_HANDLE hDS, J_STREAM_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize);

//******************************************************************************************************************
/// \brief                 Get the buffer handle by index
/// \param [in] hDS        Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iIndex     Zero based index for the buffer.
/// \param [out] hBuf      Pointer to a variable in which the buffer handle is stored.
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_GetBufferID(STREAM_HANDLE hDS, uint32_t iIndex, BUF_HANDLE *hBuf);

//******************************************************************************************************************
/// \brief                 Close the data stream
/// \param [in] hDS        Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_Close(STREAM_HANDLE hDS);

//******************************************************************************************************************
/// \brief                 Remove the frame buffer from the Acquisition engine
/// \param [in] hDS        Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] hBuf       Handle to the buffer to be revoked 
/// \param [in] pBuffer    Pointer to the buffer to remove from the acquisition engine  
/// \param [in] pPrivate   Pointer to the private data to remove from the acquisition engine 
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// //==============================================================////
/// // Unprepare buffers
/// //==============================================================////
/// BOOL CStreamThread::UnPrepareBuffers(void)
/// {
///   void      *pPrivate;
///   void      *pBuffer;
///   uint32_t  i;
///
///   // Flush Queues
///   J_DataStream_FlushQueue(m_hDS, ACQ_QUEUE_INPUT_TO_OUTPUT);
///   J_DataStream_FlushQueue(m_hDS, ACQ_QUEUE_OUTPUT_DISCARD);
///   
///   for(i = 0 ; i < m_iValidBuffers ; i++)
///   {
///     // Remove the frame buffer from the Acquisition engine.
///     J_DataStream_RevokeBuffer(m_hDS, m_pAquBufferID[i], &pBuffer, &pPrivate);
///
///     delete m_pAquBuffer[i];
///     m_pAquBuffer[i] = NULL;
///     m_pAquBufferID[i] = 0;
///   }
///
///   m_iValidBuffers = 0;
///
///   return TRUE;
/// }
/// \endcode
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_RevokeBuffer(STREAM_HANDLE hDS, BUF_HANDLE hBuf, void **pBuffer, void **pPrivate);

//******************************************************************************************************************
/// \brief                    Queue the buffer in the acquisition engine
/// \param [in] hDS           Handle to a valid data stream, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] hBuf          Handle of the buffer to be queued, obtained by \c J_DataStream_AnnounceBuffer().
/// \retval                   Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The data stream needs to be created using \c J_Camera_CreateDataStream() before buffers can be announced.
/// \code
/// //==============================================================////
/// // Prepare frame buffers for the data stream
/// //==============================================================////
/// uint32_t CStreamThread::PrepareBuffers(int bufferCount, int bufferSize, void *pPrivateData)
/// {
///   J_STATUS_TYPE  iResult = J_ST_SUCCESS;
///   int            i;
///
///   m_iValidBuffers = 0;
///
///   for(i = 0 ; i < bufferCount ; i++)
///   {
///     // Make the buffer for one frame. 
///     m_pAquBuffer[i] = new uint8_t[bufferSize];
///
///     // Announce the buffer pointer to the Acquisition engine.
///     if(J_ST_SUCCESS != J_DataStream_AnnounceBuffer(m_hDS,
///                                                    m_pAquBuffer[i],
///                                                    bufferSize,
///                                                    pPrivateData,
///                                                    &(m_pAquBufferID[i])),
///                                                    0)
///     {
///       delete m_pAquBuffer[i];
///       break;
///     }
///
///     // Queueing it.
///     if(J_ST_SUCCESS != J_DataStream_QueueBuffer(m_hDS, m_pAquBufferID[i]))
///     {
///       delete m_pAquBuffer[i];
///       break;
///     }
///
///     m_iValidBuffers++;
///   }
///
///   return m_iValidBuffers;
/// }
/// \endcode
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_QueueBuffer(STREAM_HANDLE hDS, BUF_HANDLE hBuf);

//******************************************************************************************************************
/// \brief                      Get detailed information about a buffer
/// \param [in] hDS             Handle to a valid data stream object, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] hBuf            Handle of the buffer to be queued, obtained by \c J_DataStream_AnnounceBuffer().
/// \param [in] iCmd            The information type that is requested
/// \param [out] pBuffer        Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize       The size of the buffer. It must be equal or larger than J_STREAM_INFO_SIZE.
///                             The function sets the actual size of data stored into the buffer.
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_STREAM_INFO_CMD
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_GetBufferInfo(STREAM_HANDLE hDS, BUF_HANDLE hBuf, J_BUFFER_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize);

//******************************************************************************************************************
/// \brief                     Get parameters about data stream.
/// \param [in] hDS            Handle to a valid data stream object, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iCmd           The parameter type which is requested
/// \param [out] pBuffer       Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize      The size of the buffer.
///                            The function sets the actual size of data stored into the buffer.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_DataStream_GetParam(STREAM_HANDLE hDS, J_STREAM_PARAM_CMD iCmd, void *pBuffer, uint32_t *pSize);

//******************************************************************************************************************
/// \brief                     Set parameters about data stream.
/// \param [in] hDS            Handle to a valid data stream object, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iCmd           The parameter type which is requested
/// \param [out] pBuffer       Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize      The size of the buffer.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_DataStream_SetParam(STREAM_HANDLE hDS, J_STREAM_PARAM_CMD iCmd, void *pBuffer, uint32_t *pSize);


/// @}
/// \addtogroup WRAP_EventInterface Event Interface functions
/// @{
/// \brief The communication from the Transport Layer and to the user application is normally handled via events. This could for instance be events
/// from the acquisition engine in the driver whenever a new image has been acquired. It could also be camera specific event when for instance the
/// connection state for the camera changes or when the camera sends a GigE Vision Event telegram. \n
/// Events are generally associated with condition objects that will be used for the actual signaling of the event between the Transport Layer
/// and the event handling threads. The conditions are operating system independent objects used for the wait and signal operations needed for
/// the event handling. This will make it possible to keep the same basic event thread implementation both in Windows applications as well in Linux
/// applications. This is a unified way of create wait and signaling code.

/// \brief The J_EVENT_INFO_ID is used for obtaining information about the number of entries in the queue using the \c J_Event_GetInfo() function
typedef enum _J_EVENT_INFO_ID_TYPE
{
   EVENT_INFO_NUM_ENTRYS_IN_QUEUE
} J_EVENT_INFO_ID;

/// \brief The J_EVENT_TYPE is used to specify which type of event to register with the Camera or Transport Layer
typedef enum _J_DEVICE_EVENT_TYPE_TYPE
{
   EVENT_NEW_BUFFER = 0,           ///< New buffer ready from the DataStream. This is only used with \c J_DataStream_RegisterEvent()
   EVENT_GEV_EVENT_CMD = 1,        ///< GigE Vision Event command received. This is only used with \c J_Camera_RegisterEvent()
   EVENT_GEV_EVENTDATA_CMD = 2,    ///< Deprecated! Use Event EVENT_GEV_EVENT_CMD instead.
   EVENT_ERROR = 3,                ///< Error event. This is only used with \c J_Camera_RegisterEvent()
   EVENT_CONNECTION = 4,           ///< Device Connection Status Change. This is only used with \c J_Camera_RegisterEvent()
   EVENT_MAX_ID
} J_EVENT_TYPE;

//******************************************************************************************************************
/// \brief                     Retrieve the event data associated with the event.
/// \param [in] pEventHandle   Internal event handle obtained by the \c J_Camera_RegisterEvent() or \c J_DataStream_RegisterEvent() functions.
/// \param [out] pBuffer       Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize      The size of the buffer.
///                            The function sets the actual size of data stored into the buffer.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The data type returned by this function will depend on the event type. If the event is of type \c EVENT_NEW_BUFFER then this
/// function will return the image buffer handle as described by the \c J_EVENT_DATA_NEW_IMAGE structure.
/// If the event is of type \c EVENT_GEV_EVENT_CMD then this function will return information about the GEV command as 
/// described by the \c J_EVENT_DATA_GEV_EVENT_CMD structure.    
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Event_Register(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_GetData(EVT_HANDLE pEventHandle, void *pBuffer, uint32_t *pSize);

//******************************************************************************************************************
/// \brief                     Retrieve event information.
/// \param [in] pEventHandle   Internal event handle obtained by the \c J_Camera_RegisterEvent() or \c J_DataStream_RegisterEvent() functions.
/// \param [in] iID            Event information type to retrieve
/// \param [out] pBuffer       Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize      The size of the buffer.
///                            The function sets the actual size of data stored into the buffer.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             J_Event_SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Camera_RegisterEvent(), \c J_DataStream_RegisterEvent(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_GetInfo(EVT_HANDLE pEventHandle, J_EVENT_INFO_ID iID, void *pBuffer, uint32_t *pSize);

//******************************************************************************************************************
/// \brief                     Flush all Event currently in the queue
/// \param [in] pEventHandle   Internal event handle obtained by the \c J_Camera_RegisterEvent() or \c J_DataStream_RegisterEvent() functions.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_RegisterEvent(), \c J_DataStream_RegisterEvent(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_Flush(EVT_HANDLE pEventHandle);

//******************************************************************************************************************
/// \brief                     Close Event
/// \param [in] pEventHandle   Internal event handle obtained by the \c J_Camera_RegisterEvent() or \c J_DataStream_RegisterEvent() functions.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_RegisterEvent(), \c J_DataStream_RegisterEvent(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_Close(EVT_HANDLE pEventHandle);

/// @}
/// \defgroup WRAP_EventInterface_camera   Camera event specific functions
/// \ingroup WRAP_EventInterface
/// @{
/// \brief The camera specific events are primarily used for supervising the connection state for the cameras and for handling GigE Vision Event 
/// telegrams sent from the cameras using the GigE Vision Message Channel communication.

/// \brief Camera connection status returned in the \c J_EVENT_DATA_CONNECTION structure.
typedef enum _J_EDC_CONNECTION_TYPE
{
   J_EDC_CONNECTED    = 0,     //!< Camera is connected
   J_EDC_DISCONNECTED = 1,     //!< Camera is disconnected
   J_EDC_LOST_CONTROL = 2,     //!< The camera timed out. We cannot control the camera before re-acquiring control again
   J_EDC_GOT_CONTROL  = 3,     //!< We regained control of the camera
} J_EDC_CONNECTION_TYPE;

/// \brief Struct containing the information for a Connection Event read using \c J_Event_GetData() when the \c J_Camera_RegisterEvent() function
/// is called with the \c EVENT_CONNECTION parameter
typedef struct _J_EVENT_DATA_CONNECTION_TYPE
{
   uint32_t m_iConnectionState;  ///< State of the camera connection
} J_EVENT_DATA_CONNECTION;

/// \brief Struct containing the information provided by a GigE Event. This is the data structure returned by \c J_Event_GetData() when
/// the \c J_Camera_RegisterEvent() function is called with the \c EVENT_GEV_EVENT_CMD parameter
#pragma pack (1)
typedef struct _J_EVENT_DATA_GEV_EVENT_CMD_TYPE
{
   uint16_t m_EventID;                ///< \c EVENT identifying reason.
   uint16_t m_StreamChannelIndex;     ///< Index of stream channel (0xFFFF for no channel).
   uint16_t m_BlockID;                ///< Data block ID (0 for no block).
   uint64_t m_Timestamp;              ///< Event timestamp (0 if not supported).
   uint16_t m_EventDataLength;        ///< Number of bytes data filled into m_EventData
   uint8_t  m_EventData[540];         ///< Data associated with the event. The number of valid data bytes are given by m_EventDataLength member
} J_EVENT_DATA_GEV_EVENT_CMD;
#pragma pack ()

//******************************************************************************************************************
/// \brief                     Register an event with the Transport Layer interface for a specific device
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] iEventType     Event type to register
/// \param [in] hEvent         Handle to the event object to be used for signalling the event. This has to be created using kernel32
///                            function \c CreateEvent()
/// \param [out] pEventHandle  Pointer to a variable in which the internal event handle is stored. This handle is used in
///                            \c J_Event_GetData() and \c J_Event_GetInfo()
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             J_Event_SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Camera_RegisterEvent(CAM_HANDLE hCam, J_EVENT_TYPE iEventType, HANDLE hEvent, EVT_HANDLE *pEventHandle);

//******************************************************************************************************************
/// \brief                     Un-register an event with the Transport Layer interface
/// \param [in] hCam           Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] iEventType     Event type to un-register
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Camera_UnRegisterEvent(CAM_HANDLE hCam, J_EVENT_TYPE iEventType);

/// @}
/// \defgroup WRAP_EventInterface_datastream   Data Stream event specific functions
/// \ingroup WRAP_EventInterface
/// @{
/// \brief These functions are specific to the Data Stream event handling. If the automatic image acquisition functionality of the JAI SDK is
/// used then these functions should not be used! The automatic image acquisition functionality is enabled using the \c J_Image_OpenStream() 
/// or \c J_Image_OpenStreamLight()

/// \brief Struct containing the information for a New Buffer Event. This is the data structure returned by \c J_Event_GetData() when
/// the \c J_DataStream_RegisterEvent() function is called with the \c EVENT_NEW_BUFFER parameter
typedef struct _J_EVENT_DATA_NEW_IMAGE_TYPE
{
   BUF_HANDLE  m_iBufferID;             ///< Buffer handle of the delivered buffer. This handle will be used as a parameter to the \c J_DataStream_GetBufferInfo()
} J_EVENT_DATA_NEW_IMAGE;

//******************************************************************************************************************
/// \brief                     Register an event with the Transport Layer interface
/// \param [in] hDS            Handle to a valid data stream object, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iEventType     Event type to register
/// \param [in] hEvent         Handle to the event object to be used for signalling the event. This has to be created using kernel32
///                            function \c CreateEvent()
/// \param [out] pEventHandle  Pointer to a variable in which the internal event handle is stored. This handle is used in
///                            \c J_Event_GetData() and \c J_Event_GetInfo()
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the basics used in the image aquisition thread for
/// // a camera. It is not intended to be used directly since it has no error handling built-in
/// // but merely to show the API functions needed.
///
/// // Create event object to be used by the acquisition engine
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// // Define internal event handle to be used when the event is fired
/// EVENT_HANDLE   hEvent;
/// 
/// // Register the hEventNewImage event with the acquisition engine to be signaled whenever a 
/// // new image buffer is ready.
/// J_DataStream_RegisterEvent(m_hDS, EVENT_NEW_BUFFER, hCondition, &hEvent);
///
/// // Start the image acquisition
/// iResult = J_DataStream_StartAcquisition(m_hDS, ACQ_START_NEXT_IMAGE, 0 );
///
/// // Main loop for image acquisition
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the buffer associated with this event
///       iSize = (uint32_t)sizeof(void *);
///       J_Event_GetData(hEvent, &hBuffer,  &iSize);
///
///       // Get the pointer to the frame buffer.
///       iSize = (uint32_t)sizeof (void *);
///       iResult = J_DataStream_GetBufferInfo(m_hDS,
///                                            hBuffer,
///                                            BUFFER_INFO_BASE,
///                                            &(tAqImageInfo.pImageBuffer),
///                                            &iSize);
///
///       // Get the effective data size.
///       iSize = (uint32_t)sizeof (uint32_t);
///       iResult = J_DataStream_GetBufferInfo(m_hDS,
///                                            hBuffer,
///                                            BUFFER_INFO_SIZE,
///                                            &(tAqImageInfo.iImageSize),
///                                            &iSize);
///
///       // Get Pixel Format Type.
///       iSize = (uint32_t)sizeof (uint32_t);
///       iResult = J_DataStream_GetBufferInfo(m_hDS,
///                                            hBuffer,
///                                            BUFFER_INFO_PIXELTYPE,
///                                            &(tAqImageInfo.iPixelType),
///                                            &iSize);
///
///       // Get Frame Width.
///       iSize = (uint32_t)sizeof (uint32_t);
///       iResult = J_DataStream_GetBufferInfo(m_hDS,
///                                            hBuffer,
///                                            BUFFER_INFO_WIDTH,
///                                            &(tAqImageInfo.iSizeX),
///                                            &iSize);
///
///       // Get Frame Height.
///       iSize = (uint32_t)sizeof (uint32_t);
///       iResult = J_DataStream_GetBufferInfo(m_hDS,
///                                            hBuffer,
///                                            BUFFER_INFO_HEIGHT,
///                                            &(tAqImageInfo.iSizeY),
///                                            &iSize);
///
///       // Get Timestamp
///       iSize = (uint32_t)sizeof (uint64_t);
///       iResult = J_DataStream_GetBufferInfo(m_hDS,
///                                            hBuffer,
///                                            BUFFER_INFO_TIMESTAMP,
///                                            &(tAqImageInfo.iTimeStamp),
///                                            &iSize);
///
///       if(m_bEnableThread)
///       {
///         // do something with the image data located in tAqImageInfo.pImageBuffer!!!
///         // This could for instance be to display the image.
///         J_Image_ShowImage(reinterpret_cast<VW_HANDLE>(g_hWin),
///                           &tAqImageInfo,
///                           m_iRGain,
///                           m_iGGain,
///                           m_iBGain);
///       }
///
///       // Queue This Buffer Again
///       iResult = J_DataStream_QueueBuffer(m_hDS, iBufferID);
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_RegisterEvent(STREAM_HANDLE hDS, J_EVENT_TYPE iEventType, HANDLE hEvent, EVT_HANDLE *pEventHandle);

//******************************************************************************************************************
/// \brief                     Un-register an event with the Transport Layer interface
/// \param [in] hDS            Handle to a valid data stream object, obtained by \c J_Camera_CreateDataStream() function.
/// \param [in] iEventType     Event type to un-register
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Camera_CreateDataStream(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_DataStream_UnRegisterEvent(STREAM_HANDLE hDS, J_EVENT_TYPE iEventType);

/// @}
/// \defgroup WRAP_EventInterface_condition   Condition specific functions
/// \ingroup WRAP_EventInterface
/// @{
/// \brief The following functions are used for creating and controlling the condition objects. The conditions are operating system independent 
/// objects used for the wait and signal operations needed for the event handling. This will make it possible to keep the same basic event thread 
/// implementation both in Windows applications as well in Linux applications. This is a unified way of create wait and signaling code.

/// \brief Wait result type returned in \c J_Event_WaitForCondition()
typedef enum _J_COND_WAIT_RESULT_TYPE
{
       J_COND_WAIT_TIMEOUT = 0,  //!< \c J_Event_WaitForCondition() timed out
       J_COND_WAIT_SIGNAL=1,     //!< \c J_Event_WaitForCondition() got signaled using \c J_Event_SignalCondition()
       J_COND_WAIT_EXIT=2,       //!< \c J_Event_WaitForCondition() got signaled using \c J_Event_ExitCondition()
       J_COND_WAIT_ERROR=-1,     //!< Error in \c J_Event_WaitForCondition()
} J_COND_WAIT_RESULT;

//******************************************************************************************************************
/// \brief                         Create new Condition object
/// \param [out] pConditionHandle  Pointer to a HANDLE that will be the handle to the Condition that is created.
/// \retval                        Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             J_Event_SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Event_CloseCondition(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_CreateCondition(HANDLE* pConditionHandle);

//******************************************************************************************************************
/// \brief                       Signal Condition object.  \c J_Event_WaitForCondition() will return J_COND_WAIT_SIGNAL
///                              if this is called.
/// \param [in] conditionHandle  Internal Condition handle obtained by the \c J_Event_CreateCondition() function.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             J_Event_SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Event_CreateCondition(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_SignalCondition(HANDLE conditionHandle);

//******************************************************************************************************************
/// \brief                       Send an EXIT signal to the Condition object. This will help to making threads exit
///                              faster. \c J_Event_WaitForCondition() will return J_COND_WAIT_EXIT if this is called.
/// \param [in] conditionHandle  Internal Condition handle obtained by the \c J_Event_CreateCondition() function.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Event_CreateCondition(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_ExitCondition(HANDLE conditionHandle);

//******************************************************************************************************************
/// \brief                       Reset the Condition object state to its initial state.
/// \param [in] conditionHandle  Internal Condition handle obtained by the \c J_Event_CreateCondition() function.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Event_CreateCondition(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_ResetCondition(HANDLE conditionHandle);

//******************************************************************************************************************
/// \brief                       Wait for the Condition object to be signaled.
/// \param [in] conditionHandle  Internal Condition handle obtained by the \c J_Event_CreateCondition() function.
/// \param [in] timeout          Timeout for the Wait operation. If timeout is set to 0 then it will wait forever
/// \param [in] pWaitResult      Result for the Wait operation. This can either be TIMEOUT, SIGNAL, EXIT or ERROR
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = J_Event_CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " J_Event_CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = J_Event_WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             J_Event_SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    J_Event_CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Event_CreateCondition(), \c J_Event_SignalCondition(), \c J_Event_ExitCondition(), \c J_Event_ResetCondition(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_WaitForCondition(HANDLE conditionHandle, DWORD timeout, J_COND_WAIT_RESULT* pWaitResult);

//******************************************************************************************************************
/// \brief                       Close the Condition object.
/// \param [in] conditionHandle  Internal Condition handle obtained by the \c J_Event_CreateCondition() function.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small piece of code demonstrate the use of Camera Events.
/// uint32_t    iSize;
/// J_EVENT_DATA_GEV_EVENT_CMD   GigEEventData;
/// J_STATUS_TYPE error;
///
/// // Mark thread as running
/// m_bThreadRunning = true;
///
/// J_COND_WAIT_RESULT iWaitResult;
/// HANDLE           hCondition;
/// int iRet = CreateCondition(&hCondition);
///
/// if (iRet || (hCondition == NULL))
///    fprintf(stderr, " CreateCondition failed!\n");
///
/// EVT_HANDLE  hEvent; // Connection event handle
///
/// // Register the GigE Vision Event command event with the transport layer
/// J_Camera_RegisterEvent(m_hCamera, EVENT_GEV_EVENT_CMD, hCondition, &hEvent);
///
/// while(m_bEnableThread)
/// {
///    iRet = WaitForCondition(hCondition, 1000, &iWaitResult);
///
///    if(J_COND_WAIT_SIGNAL == iWaitResult)
///    {
///       if(m_bEnableThread == false)
///          break;
///
///       // Get the GigE Vision Event Data from the event
///       iSize = (uint32_t)sizeof(GigEEventData);
///       error = J_Event_GetData(hEvent, &GigEEventData,  &iSize);
///
///       // Here we need to process the EVENT telegram
///       if (error == J_ST_SUCCESS)
///       {
///          // Do something with the event
///          ...
///
///          // Here we need to see if more events has been queued up!
///          uint32_t    NumEventsInQueue = 0;
///          iSize = sizeof(EventInfo);
///
///          error = J_Event_GetInfo(hEvent, (J_EVENT_INFO_ID)EVENT_INFO_NUM_ENTRYS_IN_QUEUE, &NumEventsInQueue, &iSize);
///
///          if ((error == J_ST_SUCCESS) && (NumEventsInQueue > 0))
///          {
///             SignalCondition(m_hEventGEVEvent);
///          }
///       }
///    }
///    else if(iWaitResult == J_COND_WAIT_EXIT)
///    {
///       // Exit the thread.
///       break;
///    }
///    else if(iWaitResult == J_COND_WAIT_TIMEOUT)
///    {
///       // Timeout - normal operation
///    }
///    else if(iWaitResult == J_COND_WAIT_ERROR)
///    {
///       // Error wait result returned from J_Event_WaitForCondition()
///    }
///    else
///    {
///       // Unknown wait result!
///    }
/// }
///
/// // Free the event object
/// if (hEvent != NULL)
/// {
///   J_Event_Close(hEvent);
///   hEvent = NULL;
/// }
///
/// // Free the connection event object
/// if (hCondition != NULL)
///    CloseCondition(hCondition);
/// hCondition = NULL;
///
///  m_bThreadRunning = false;
/// \endcode
/// \sa \c J_Event_CreateCondition(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE  J_Event_CloseCondition(HANDLE conditionHandle);

/// @}
/// \defgroup WRAP_NODE   GenICam node specific functions
/// @{
/// \brief The JAI SDK is able to control camera settings using the GenICam standard. When a camera connection is opened then the camera will 
/// provide the JAI SDK with a XML-file that contains a complete list of all the controllable features inside the camera. Each of these features
/// are accessibles as nodes in a tree-like structure where the top-most level is named "Root". The following code shows how to traverse the
/// node tree with all the controllable features.
/// \code
/// 
/// J_STATUS_TYPE   retval;
/// uint32_t        nFeatureNodes;
/// NODE_HANDLE     hNode;
/// int8_t          sNodeName[256], sSubNodeName[256];
/// uint32_t        size;
/// 
/// retval = J_Camera_GetNumOfSubFeatures(hCamera, J_ROOT_NODE, &nFeatureNodes);
/// if (retval == J_ST_SUCCESS)
/// {
///   printf("%u feature nodes were found in the Root node\n", nFeatureNodes);
///   // Run through the list of feature nodes and print out the names
///   for (uint32_t index = 0; index < nFeatureNodes; ++index)
///   {
///     // Get subfeature node handle
///     retval = J_Camera_GetSubFeatureByIndex(hCamera, J_ROOT_NODE, index, &hNode);
///     if (retval == J_ST_SUCCESS)
///     {
///       // Get subfeature node name
///       size = sizeof(sNodeName);
///       retval = J_Node_GetName(hNode, sNodeName, &size, 0);
///       if (retval == J_ST_SUCCESS)
///       {
///         // Print out the name
///         printf("%u: %s\n", index, sNodeName);
///       }
/// 
///       // Get the node type
///       J_NODE_TYPE NodeType;
///       retval = J_Node_GetType(hNode, &NodeType);
/// 
///       // Is this a category node?
///       if (NodeType == J_ICategory)
///       {
///         // Get number of sub features under the category node
///         uint32_t nSubFeatureNodes;
///         retval = J_Camera_GetNumOfSubFeatures(hCamera, sNodeName, &nSubFeatureNodes);
///         if (nSubFeatureNodes > 0)
///         {
///           printf("\t%u subfeature nodes were found\n", nSubFeatureNodes);
///           for (uint32_t subindex = 0; subindex < nSubFeatureNodes; subindex++)
///           {
///             NODE_HANDLE hSubNode;
///             retval = J_Camera_GetSubFeatureByIndex(hCamera,
///                                                    sNodeName,
///                                                    subindex,
///                                                    &hSubNode);
///             if (retval == J_ST_SUCCESS)
///             { 
///               size = sizeof(sSubNodeName);
///               retval = J_Node_GetName(hSubNode, sSubNodeName, &size, 0);
///          
///               // Print out the name
///               printf("\t%u-%u: %s\n", index, subindex, sSubNodeName);
///             }
///           }
///         }
///       }
///     }
///   }
/// }
/// \endcode
/// \par
/// All the features will have a note type attribute that determines the behaviour and functionality of the nodes. The node type can be read
/// using the \c J_Node_GetType() function.

/// \brief Recommended visibility of an GenICam node
typedef enum _J_NODE_VISIBILITY_TYPE
{
   Beginner = 0,               //!< Always visible
   Expert = 1,                 //!< Visible for experts or Gurus
   Guru = 2,                   //!< Visible for Gurus
   Invisible = 3,              //!< Not Visible
   _UndefinedVisibility  = 99  //!< Object is not yetinitialized
} J_NODE_VISIBILITY;

/// \brief The GenICam interface node type of a specific node
typedef enum _J_NODE_TYPE_TYPE
{
   J_UnknowNodeType = 1, //!< Unknown node type
   J_INode        = 100, //!< INode node
   J_ICategory,          //!< ICategory node
   J_IInteger,           //!< IInteger node
   J_IEnumeration,       //!< IEnumeration node
   J_IEnumEntry,         //!< node IEnumEntry
   J_IMaskedIntReg,      //!< node IMaskedIntReg
   J_IRegister,          //!< IRegister node
   J_IIntReg,            //!< IIntReg node
   J_IFloat,             //!< IFloat node
   J_IFloatReg,          //!< IFloatReg node
   J_ISwissKnife,        //!< ISwissKnife node
   J_IIntSwissKnife,     //!< IIntSwissKnife node
   J_IIntKey,            //!< IIntKey node
   J_ITextDesc,          //!< ITextDesc node
   J_IPort,              //!< IPort node
   J_IConfRom,           //!< IConfRom node
   J_IAdvFeatureLock,    //!< IAdvFeatureLock node
   J_ISmartFeature,      //!< ISmartFeature node
   J_IStringReg,         //!< IStringReg node
   J_IBoolean,           //!< IBoolean node
   J_ICommand,           //!< ICommand node
   J_IConverter,         //!< IConverter node
   J_IIntConverter,      //!< IIntConverter node

   J_IChunkPort,         //!< IChunkPort node
   J_INodeMap,           //!< INodeMap node
   J_INodeMapDyn,        //!< INodeMapDyn node
   J_IDeviceInfo,        //!< IDeviceInfo node
   J_ISelector,          //!< ISelector node
   J_IPortConstruct      //!< IPortConstruct node 
} J_NODE_TYPE;

/// \brief Access mode of a GenICam node
typedef enum _J_NODE_ACCESSMODE_TYPE
{
   NI,                 //!< Not implemented
   NA,                 //!< Not available
   WO,                 //!< Write Only
   RO,                 //!< Read Only
   RW,                 //!< Read and Write
   _UndefinedAccesMode //!< Object is not yetinitialized
} J_NODE_ACCESSMODE; 

/// \brief Defines if a node name is standard or custom namespace
typedef enum _J_NODE_NAMESPACE_TYPE
{
   Custom,             //!< name resides in custom namespace
   Standard,           //!< name resides in one of the standard namespaces
   _UndefinedNameSpace //!< Object is not yetinitialized
} J_NODE_NAMESPACE;

/// \brief Caching mode of a GenICam register node
typedef enum _J_NODE_CACHINGMODE_TYPE
{
   NoCache,              //!< Do not use cache
   WriteThrough,         //!< Write to cache and register
   WriteAround,          //!< Write to register, write to cache on read
   _UndefinedCachingMode //!< Not yet initialized
} J_NODE_CACHINGMODE;

/// \brief GenICam Error Info
typedef struct {
   char sDescription[1024];
   char sNodeName[128];
} tGenICamErrorInfo;

//******************************************************************************************************************
/// \brief                 Get the access mode for a GenICam node
/// \param [in] hNode      Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue    Pointer to the variable in which the data is stored.
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the access mode can be read
/// \sa \c J_Camera_Open(), \c EConfAccessMode, \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetAccessMode(NODE_HANDLE hNode, J_NODE_ACCESSMODE *pValue);

//******************************************************************************************************************
/// \brief                     Get the GenICam name for a node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pBuffer       Pointer to the buffer in which the data is stored.
/// \param [in,out] pSize      Specify the size of the buffer, and then the function sets an actual size of stored data.
/// \param [in] FullQualified
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the GenICam name can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetName(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize, uint32_t FullQualified = 0);

//******************************************************************************************************************
/// \brief                 Get the namespace for a GenICam node
/// \param [in] hNode      Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue    Pointer to the variable in which the data is stored.
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the namespace can be read
/// \sa \c J_Camera_Open(), \c J_NODE_NAMESPACE, \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetNameSpace(NODE_HANDLE hNode, J_NODE_NAMESPACE *pValue);

//******************************************************************************************************************
/// \brief                     Get the recommended visibility for a GenICam node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pVisibility   Pointer to the variable in which the data is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the recommended visibility can be read
/// \sa \c J_Camera_Open(), \c J_NODE_VISIBILITY, \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetVisibility(NODE_HANDLE hNode, J_NODE_VISIBILITY* pVisibility);

//******************************************************************************************************************
/// \brief                      Invalidate the node
/// \param [in] hNode           Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the node can be invalidated
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_Invalidate(NODE_HANDLE hNode);

//******************************************************************************************************************
/// \brief                     Get if the GenICam node is cachable. If the node is not cachable it will need to be read periodically.
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pVal          Pointer to the variable in which the data is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the cachability can be read
/// \sa \c J_Camera_Open(), \c J_Node_GetPollingTime(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetIsCachable(NODE_HANDLE hNode, uint32_t* pVal);

//******************************************************************************************************************
/// \brief                     Get the caching mode for a GenICam node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the data is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the caching mode can be read
/// \sa \c J_Camera_Open(), \c J_NODE_CACHINGMODE, \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetCachingMode(NODE_HANDLE hNode, J_NODE_CACHINGMODE *pValue);

//******************************************************************************************************************
/// \brief                     Get the recommended polling time for the GenICam node. This is only necessary if the node is not cachable.
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the data is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the cachability can be read
/// \sa \c J_Camera_Open(), \c J_Node_GetIsCachable(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetPollingTime(NODE_HANDLE hNode, int64_t *pValue);

//******************************************************************************************************************
/// \brief                     Get the tooltip for a node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pBuffer       Pointer to the buffer in which the data is stored.
/// \param [in,out] pSize      Specify the size of the buffer, and then the function sets an actual size of stored data.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the tooltip can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetToolTip(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                     Get the description for a node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pBuffer       Pointer to the buffer in which the data is stored.
/// \param [in,out] pSize      Specify the size of the buffer, and then the function sets an actual size of stored data.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the description can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetDescription(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                     Get the display name for a node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pBuffer       Pointer to the buffer in which the data is stored.
/// \param [in,out] pSize      Specify the size of the buffer, and then the function sets an actual size of stored data.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the display name can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetDisplayName(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                     Get the Event ID string for a node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pBuffer       Pointer to the buffer in which the data is stored.
/// \param [in,out] pSize      Specify the size of the buffer, and then the function sets an actual size of stored data.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the Event ID can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetEventID(NODE_HANDLE hNode, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                     Get if the GenICam node is streamable
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pVal          Pointer to the variable in which the data is stored. Non zero if the node is streamable.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the streamability can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetIsStreamable(NODE_HANDLE hNode, uint32_t* pVal);

//******************************************************************************************************************
/// \brief                     Get the number of properties on the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pNum          Pointer to the variable in which the number of properties is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the number of properties can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetNumOfProperties(NODE_HANDLE hNode, uint32_t* pNum);

//******************************************************************************************************************
/// \brief                     Get the name of the property by index
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] index          Zero based index of the property. It must be smaller than the number obtained by \c J_Node_GetNumOfProperties()
/// \param [out] pBuffer       Pointer to a buffer in which the data is stored.
/// \param [in,out] pSize      Specify the size of the buffer, and then the function sets an actual size of stored data.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the property names can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetPropertyNameByIndex(NODE_HANDLE hNode, uint32_t index, int8_t* pBuffer, uint32_t* pSize);

//******************************************************************************************************************
/// \brief                             Get the property value and attribute string by property name
/// \param [in] hNode                  Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] PropertyName           Name of the property retrieved using \c J_Node_GetPropertyNameByIndex()
/// \param [out] ValueStr              Pointer to a buffer in which value is stored.
/// \param [in,out] pSizeValueStr      Specify the size of the buffer for the value, and then the function sets an actual size of stored data.
/// \param [out] AttributeStr          Pointer to a buffer in which attribute is stored.
/// \param [in,out] pSizeAttributeStr  Specify the size of the buffer for the attribute, and then the function sets an actual size of stored data.
/// \retval                            Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                              If a property has multiple values/attribute they come with Tabs as delimiters
/// The camera needs to be opened using \c J_Camera_Open() before the property values and attributes can be read
/// \sa \c J_Camera_Open(), \c J_Node_GetPropertyNameByIndex(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetPropertyByName(NODE_HANDLE hNode, const int8_t* PropertyName, 
                                                                   int8_t* ValueStr, uint32_t* pSizeValueStr,
                                                                   int8_t* AttributeStr, uint32_t* pSizeAttributeStr);

//******************************************************************************************************************
/// \brief                         Change the access mode for a node
/// \param [in] hNode              Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] AccessMode         The imposed access mode
/// \retval                        Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the access mode can be changed
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_ImposeAccessMode(NODE_HANDLE hNode, J_NODE_ACCESSMODE AccessMode);

//******************************************************************************************************************
/// \brief                         Change the recommended visibility for a node
/// \param [in] hNode              Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Visibility         The recommended visibility mode 
/// \retval                        Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the recommended visibility can be changed
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_ImposeVisibility(NODE_HANDLE hNode, J_NODE_VISIBILITY Visibility);

//******************************************************************************************************************
/// \brief                     Get a handle to a node object which describes the same feature in a different way
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] phAliasNode   Pointer to the variable in which the handle to alias node object is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the alias node can be read
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetAlias(NODE_HANDLE hNode, NODE_HANDLE* phAliasNode);

//******************************************************************************************************************
/// \brief                        Register a callback function to a specific node. The callback function will be called when the node changes state or value
/// \param [in] hNode             Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] CallBackFunction  Pointer to user call back function.
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the callback fucntion can be registered
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_RegisterCallback(NODE_HANDLE hNode, J_NODE_CALLBACK_FUNCTION CallBackFunction);

//******************************************************************************************************************
/// \brief                     Get the GenICam node type of the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pNodeType     Pointer to the variable in which the data is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the node type can be read
/// \sa \c J_Camera_Open(), \c J_NODE_TYPE, \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetType(NODE_HANDLE hNode, J_NODE_TYPE* pNodeType);

//******************************************************************************************************************
/// \brief                     Get if the GenICam node is a Selector
/// \param [in] hNode          A valid handle to a GenICam node
/// \param [out] pValue        Pointer to the variable in which the value is stored. Non zero if the node is a Selector
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetIsSelector(NODE_HANDLE hNode, uint32_t* pValue);

//******************************************************************************************************************
/// \brief                     Get the number of Selected Features on the Selector node
/// \param [in] hNode          A valid handle to a GenICam Selector node
/// \param [out] pNum          The number of features that are selected by this Selector
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                      Use <c>J_NodeGetIsSelector()</c> to determine if the node is a Selector node or not.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetNumOfSelectedFeatures(NODE_HANDLE hNode, uint32_t *pNum);

//******************************************************************************************************************
/// \brief                     Get the selected feature node by index
/// \param [in] hNode          A valid handle to a GenICam Selector node
/// \param [in] index          Index of the Selected feature node entry
/// \param [out] hSelectedNode A valid handle to a GenICam Selector node
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                      Use <c>J_NodeGetIsSelector()</c> to determine if the node is a Selector node or not.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetSelectedFeatureByIndex(NODE_HANDLE hNode, uint32_t index, NODE_HANDLE * hSelectedNode);

//******************************************************************************************************************
/// \brief                     Get the number of Selecting Features on the Selector node
/// \param [in] hNode          A valid handle to a GenICam Selector node
/// \param [out] pNum          The number of features that are selecting this Selector
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                      Use <c>J_NodeGetIsSelector()</c> to determine if the node is a Selector node or not.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetNumOfSelectingFeatures(NODE_HANDLE hNode, uint32_t *pNum);

//******************************************************************************************************************
/// \brief                      Get the selecting feature node by index
/// \param [in] hNode           A valid handle to a GenICam Selector node
/// \param [in] index           Index of the Selecting feature node entry
/// \param [out] hSelectingNode A valid handle to a GenICam selecting node
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                       Use <c>J_NodeGetIsSelector()</c> to determine if the node is a Selector node or not.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetSelectingFeatureByIndex(NODE_HANDLE hNode, uint32_t index, NODE_HANDLE * hSelectingNode);

//******************************************************************************************************************
/// \brief                     Get GenICam error info
/// \param [in] gc             Handle to a valid command node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Factory_GetGenICamErrorInfo(tGenICamErrorInfo* gc);

/// @}
/// \defgroup WRAP_NODE_INT   IInteger GenICam node specific functions
/// \ingroup WRAP_NODE
/// @{
/// \brief The Integer nodes have got the following special 64-bit integer attributes: Minimum, Maximum and an Increment. To read and write the 
/// node values you need to use the \c J_Node_GetValueInt64() and \c J_Node_SetValueInt64() functions.

//******************************************************************************************************************
/// \brief                     Get the minimum value as int64_t type of the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the minimum value can be read
/// The node type has to be \c IInteger
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetMinInt64(NODE_HANDLE hNode, int64_t* pValue);

//******************************************************************************************************************
/// \brief                     Get the maximum value as int64_t type of the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the maximum value can be read
/// The node type has to be \c IInteger
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetMaxInt64(NODE_HANDLE hNode, int64_t* pValue);

//******************************************************************************************************************
/// \brief                     Get the increment value of the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the increment value can be read
/// The node type has to be \c IInteger
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetInc(NODE_HANDLE hNode,int64_t *pValue);

//******************************************************************************************************************
/// \brief                     Set the value as int64_t type to the node object
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Verify         Verify the value set
/// \param [in] Value          Value to set
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type has to be \c IInteger, \c IEnumeration or \c IBoolean
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_SetValueInt64(NODE_HANDLE hNode, bool Verify, int64_t Value);

//******************************************************************************************************************
/// \brief                     Get the value as int64_t type from the node object
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Verify         Verify the value set.
/// \param [in] pValue         Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be read
/// The node type has to be \c IInteger, \c IEnumeration or \c IBoolean
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetValueInt64(NODE_HANDLE hNode, bool Verify, int64_t* pValue);

/// @}
/// \defgroup WRAP_NODE_FLOAT   IFloat GenICam node specific functions
/// \ingroup WRAP_NODE
/// @{
/// \brief The Floating Point nodes have got the following special double precision attributes: Minimum, Maximum and an Increment.
/// To read and write the node values you need to use the \c J_Node_GetValueDouble() and \c J_Node_SetValueDouble() functions.

//******************************************************************************************************************
/// \brief                     Get the minimum value as double type of the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the minimum value can be read
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetMinDouble(NODE_HANDLE hNode, double* pValue);


//******************************************************************************************************************
/// \brief                     Get the maximum value as double type of the node
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the maximum value can be read
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetMaxDouble(NODE_HANDLE hNode, double* pValue);

//******************************************************************************************************************
/// \brief                     Set the value as double type to the node object
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Verify         Verify the value set
/// \param [in] Value          Value to set
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_SetValueDouble(NODE_HANDLE hNode, bool Verify, double Value);

//******************************************************************************************************************
/// \brief                     Get the value as double type from the node object
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Verify         Verify the value set.
/// \param [in] pValue         Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be read
/// The node type has to be \c IFloat
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetValueDouble(NODE_HANDLE hNode, bool Verify, double* pValue);

/// @}
/// \defgroup WRAP_NODE_ENUM   IEnumeration GenICam node specific functions
/// \ingroup WRAP_NODE
/// @{
/// \brief Some of the camera features are represented as a list of possible values that can be selected via a name from a list. These features are
/// normally refered to as enumerations. If the node type retyrned by \c J_Node_GetType() is \c J_IEnumeration then the following functions are used
/// for getting the list of enumeration entries that can be used for setting the feature and possible values returned from the feature. \n
/// To read and write the value of an enumeration you will need to use one of these funtions: \c J_Node_GetValueInt64(), \c J_Node_GetValueString(),
/// \c J_Node_SetValueInt64() or \c J_Node_SetValueString()

//******************************************************************************************************************
/// \brief                     Get the number of enumeration entries on a J_NODE_TYPE.IEnumeration node
/// \param [in] hEnumNode      Handle to a valid enumeration node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pNum          Pointer to the variable in which the value is stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the number of enumeration entries can be read.
/// The node type has to be \c J_NODE_TYPE.IEnumeration
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetNumOfEnumEntries(NODE_HANDLE hEnumNode, uint32_t *pNum);

//******************************************************************************************************************
/// \brief                       Get the enumeration entry on a IEnumeration node. The node type returned will be \c J_NODE_TYPE.IEnumEntry
/// \param [in] hEnumNode        Handle to a valid enumeration node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] index            Zero based index of the entry nodes. It must be smaller than the number obtained by \c J_Node_GetNumOfEnumEntries().
/// \param [out] hEnumEntryNode  Pointer to the variable in which the value is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the enumeration entry handle can be read.
/// The node type has to be \c J_NODE_TYPE.IEnumeration
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetEnumEntryByIndex(NODE_HANDLE hEnumNode, uint32_t index, NODE_HANDLE* hEnumEntryNode);

//******************************************************************************************************************
/// \brief                       Get the integer value of a J_NODE_TYPE.IEnumEntry node
/// \param [in] hEnumNode        Handle to a valid enumeration entry node object, obtained by \c J_Node_GetEnumEntryByIndex()
/// \param [out] pValue          Pointer to the variable in which the value is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the enumeration entry value can be read.
/// The node type has to be \c J_NODE_TYPE.IEnumeration
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetEnumEntryValue(NODE_HANDLE hEnumNode, int64_t* pValue);

/// @}
/// \defgroup WRAP_NODE_CMD   ICommand GenICam node specific functions
/// \ingroup WRAP_NODE
/// @{
/// \brief Some features are defined as commands. These command features cannot be read or written to but instead the command can be executed using
/// the \c J_Node_ExecuteCommand() function.

//******************************************************************************************************************
/// \brief                     Execute the GenICam command
/// \param [in] hNode          Handle to a valid command node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the command can be executed.
/// The node type has to be \c J_NODE_TYPE.ICommand
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_ExecuteCommand(NODE_HANDLE hNode);

//******************************************************************************************************************
/// \brief                     Read if the GenICam command has been executed
/// \param [in] hNode          Handle to a valid command node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue        Pointer to the variable in which the value is stored. Non zero if the command execution is done.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the commandstatus can be read.
/// The node type has to be \c J_NODE_TYPE.ICommand
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetCommandIsDone(NODE_HANDLE hNode, uint32_t* pValue);

/// @}
/// \defgroup WRAP_NODE_STR   IString GenICam node specific functions
/// \ingroup WRAP_NODE
/// @{
/// \brief Some registers inside the cameras are defined as either string registers or string features. These are only accessible using the
/// \c J_Node_GetString() and \c J_Node_SetString() functions. It is also possible to use these two function to read and write the values of 
/// integer, floating point, boolean and enumeration features!

//******************************************************************************************************************
/// \brief                     Set the value as string type to the node object
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Verify         Verify the value set
/// \param [in] ValueStr       Value to set
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be set
/// The node type can be any type. The value will automatically be converted internally in the factory
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_SetValueString(NODE_HANDLE hNode, bool Verify, int8_t* ValueStr);


//******************************************************************************************************************
/// \brief                     Get the value as string type from the node object
/// \param [in] hNode          Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] Verify         Verify the value set.
/// \param [in] ValueStr       Pointer to the variable in which the value is stored.
/// \param [in,out] pSize      Specify the size of the value, and then the function sets an actual size of stored value.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the value can be read
/// The node type can be any type. The value will automatically be converted internally in the factory
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetValueString(NODE_HANDLE hNode, bool Verify, int8_t* ValueStr, uint32_t* pSize);

/// @}
/// \defgroup WRAP_NODE_REG   IRegister GenICam node specific functions
/// \ingroup WRAP_NODE
/// @{
/// \brief Some of the camera features might be specified as register types. The functions below makes it possible to read and write these registers
/// directly as binary data without interpreting this data as for instance integers and floating point values. This is typically used for accessing
/// LUT data as a binary chunk of data instead of direct access using LUT index selectors that would be much slower than the direct register access.

//******************************************************************************************************************
/// \brief                       Writes register values to nodes of type J_NODE_TYPE.IRegister, IIntReg, IStringReg or IFloatReg
/// \param [in] hNode            Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [in] pBuffer          Pointer to a buffer with the register values to be written to the camera.
/// \param [in] Length           Specify the length of the buffer (in bytes). The register length can be obtained using \c J_Node_GetRegisterLength()
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the register values can be written.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_SetValueRegister(NODE_HANDLE hNode, const uint8_t *pBuffer, int64_t Length);

//******************************************************************************************************************
/// \brief                       Read register values of nodes of type J_NODE_TYPE.IRegister, IIntReg, IStringReg or IFloatReg
/// \param [in] hNode            Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pBuffer         Pointer to a buffer in which the value are to be stored.
/// \param [in] Length           Specify the length of the buffer (in bytes). The register length can be obtained using \c J_Node_GetRegisterLength()
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the register values can be read.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetValueRegister(NODE_HANDLE hNode, uint8_t *pBuffer, int64_t Length);

//******************************************************************************************************************
/// \brief                       Get the length of a J_NODE_TYPE.IRegister, IIntReg, IStringReg and IFloatReg node
/// \param [in] hNode            Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue          Pointer to the variable in which the value is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the register length value can be read.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetRegisterLength(NODE_HANDLE hNode, int64_t *pValue);

//******************************************************************************************************************
/// \brief                       Get the address of a J_NODE_TYPE.IRegister, IIntReg, IStringReg and IFloatReg node
/// \param [in] hNode            Handle to a valid node object, obtained by \c J_Camera_GetNode****, \c J_Camera_GetFeature**** or so on.
/// \param [out] pValue          Pointer to the variable in which the value is stored.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// The camera needs to be opened using \c J_Camera_Open() before the register address can be read.
/// \sa \c J_Camera_Open(), \c J_STATUS_CODES, \c J_NODE_TYPE
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Node_GetRegisterAddress(NODE_HANDLE hNode, int64_t *pValue);

/// @}

/// @}
/// \defgroup WRAP_IMAGE   Image acquisition and manipulation functions
/// @{
/// \brief The JAI SDK contains a number of functions to handle automatic image acquisition via callback functions, 
/// image manipulation and image display.

/// \brief Bayer interpolation algorithm options used in \c J_Image_ShowImageEx(), \c J_Image_FromRawToDIBEx() and \c J_Image_FromRawToImageEx()
typedef enum _J_BAYER_ALGORITHM_TYPE 
{
   BAYER_STANDARD = 0,           //!< Standard Algorithm
   BAYER_EXTEND = 1,             //!< Extented Algorithm
   BAYER_STANDARD_MULTI = 2,     //!< Standard Algorithm with multi core processing
   BAYER_EXTEND_MULTI = 3        //!< Extented Algorithm with multi core processing
} J_BAYER_ALGORITHM;

/// @}
/// \defgroup WRAP_IMAGE_VIEW   View window specific functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief In order to diplay the acquired images the View Window specific functions need to be used for creating a display window. Many View Windows
/// can be created for a single application and these View Windows can either be created as new windows or it can reuse child windows already
/// created by the application.

/// \brief Window type used in \c J_Image_OpenViewWindowEx()
typedef enum _J_IVW_WINDOW_TYPE_TYPE
{
   J_IVW_OVERLAPPED = 0,         //!< Overlapped window
   J_IVW_CHILD = 1,              //!< Child window
   J_IVW_CHILD_STRETCH = 2,      //!< Stretched child window
   J_IVW_OVERLAPPED_STRETCH = 3, //!< Overlapped and stretched window
} J_IVW_WINDOW_TYPE;

//******************************************************************************************************************
/// \brief   Open View Window (Create)
///
/// A window of size MaxSize is created for the client area. When the images are displayed using \c J_ImageShowImage() the view window will automatically be resized. 
///
/// \param [in] pWindowName   Name to be displayed in the view window caption
/// \param [in] *pPoint       Point of the upper left corner of the view window (absolute coordinate).
/// \param [in] *pMaxSize     Maximum size of the view window
/// \param [out] *pWin        The handle of the created window (window object) is returned.
/// \retval                   Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that opens a live view window and starts acquisition
///
/// VIEW_HANDLE     hView;
/// THRD_HANDLE     hThread;
/// J_STATUS_TYPE   retval;
/// NODE_HANDLE     hNode;
/// int64_t         int64Val;
/// SIZE            ViewSize = {100, 100};
/// POINT           TopLeft = {0, 0};
///
/// // Get Width from the camera
/// if (J_Camera_GetValueInt64(hCamera, "Width", &int64Val) == J_ST_SUCCESS)
/// {
///   ViewSize.cx = (LONG)int64Val;     // Set window size cx
/// }
/// // Get Height from the camera
/// if (J_Camera_GetValueInt64(hCamera, "Height", &hNode) == J_ST_SUCCESS)
/// {
///   ViewSize.cy = (LONG)int64Val;     // Set window size cy
/// }
///
/// // Open view window
/// retval = J_Image_OpenViewWindow((LPCTSTR)"Live view!", &TopLeft, &ViewSize, &hView);
/// if (retval == J_ST_SUCCESS)
/// {
///   // Open stream
///   retval = J_Image_OpenStreamLight(hCamera, 0, &hThread); 
///   if (retval == J_ST_SUCCESS)
///   {
///     // Acquisition Start
///     J_Camera_ExecuteCommand(hCamera, "AcquisitionStart");
///   }
/// }
///
/// // Message loop
/// printf("Any key to exit!");
/// while (!_kbhit())
/// {
///   MSG msg;
///   if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
///       ::TranslateMessage(&msg);
///       ::DispatchMessage(&msg);
///   }
/// }
///
/// // Acquisition Stop
/// J_Camera_ExecuteCommand(hCamera, "AcquisitionStop");
///
/// // Close stream
/// retval = J_Image_CloseStream(hThread);
///
/// // Close view window
/// retval = J_Image_CloseViewWindow(hView);
/// \endcode
/// \sa \c J_Image_OpenViewWindow(), \c J_Camera_GetNodeByName(), \c J_Node_ExecuteCommand(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_OpenViewWindow(LPCTSTR pWindowName, POINT *pPoint, SIZE *pMaxSize, VIEW_HANDLE *pWin);

//******************************************************************************************************************
/// \brief   Open View Window Ex (Create)
/// 
/// A window of size MaxSize is created for the client area. When the images are displayed using \c J_ImageShowImage() the view window will automatically be resized.
/// The window of size pFrameRect with window name pWindowName is created, and a view window of size pMaxSize is made for the client area.
/// At this time, the size of the window displayed in the first stage is specified for pFrameRect, and the assumed maximum image size is specified for pViewRect. 
/// Therefore, it usually becomes
/// pFrameRect<=*pMaxSize. 
///
/// \param  [in] iWindowType   Window type
/// \param  [in] pWindowName   Window name
/// \param  [in] *pFrameRect   Externals size of frame window(absolute coordinate).
/// \param  [in] *pMaxSize     Maximum size of image data.
/// \param  [in] hParent       Handle to parent window.
/// \param  [out] *pWin        The handle of the created window (window object) is returned.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_OpenViewWindowEx(J_IVW_WINDOW_TYPE iWindowType, LPCTSTR pWindowName, RECT *pFrameRect, SIZE *pMaxSize, HWND hParent, VIEW_HANDLE *pWin);

//******************************************************************************************************************
/// \brief   Open View Window Ex for Linux (Create)
///
/// A window of size MaxSize is created for the client area. When the images are displayed using \c J_ImageShowImage() the view window will automatically be resized.
/// The window of size pFrameRect with window name pWindowName is created, and a view window of size pMaxSize is made for the client area.
/// At this time, the size of the window displayed in the first stage is specified for pFrameRect, and the assumed maximum image size is specified for pViewRect.
/// Therefore, it usually becomes
/// pFrameRect<=*pMaxSize.
///
/// \param  [in] iWindowType   Window type
/// \param  [in] pWindowName   Window name
/// \param  [in] *pFrameRect   Externals size of frame window(absolute coordinate).
/// \param  [in] *pMaxSize     Maximum size of image data.
/// \param  [in] hParent       Handle to parent window.
/// \param  [out] *pWin        The handle of the created window (window object) is returned.
/// \param  [in] *pDisplay     Pointer to parent window's Display.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_OpenViewWindowLinux(J_IVW_WINDOW_TYPE iWindowType, LPCTSTR pWindowName, RECT *pFrameRect, SIZE *pMaxSize, HWND hParent, VIEW_HANDLE *pWin, Display * pDisplay);

//******************************************************************************************************************
/// \brief   Close view window previously opened with \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx() (Destroy it)
/// 
/// The window of handle hWin that \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx() returned is destroyed.
///
/// \param [in] hWin   Handle that \c J_Image_OpenViewWindow() or c J_Image_OpenViewWindowEx() returned.
/// \retval            Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_CloseViewWindow(VIEW_HANDLE hWin);

//******************************************************************************************************************
/// \brief   Display an image in a view window. 
/// 
/// This function automatically converts the RAW image specified with pAqImageInfo into DIB, and display it in the view window in the window of handle
/// hWin made with \c J_Image_OpenViewWindow() or c J_Image_OpenViewWindowEx() . 
///
/// \param [in]  hWin          Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx().
/// \param [out] pAqImageInfo  ImageInfo structure with information on RAW image.
/// \param [in]  iRGain        Gain for the red color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in]  iGGain        Gain for the green color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in]  iBGain        Gain for the blue color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ShowImage(VIEW_HANDLE hWin, J_tIMAGE_INFO * pAqImageInfo, uint32_t iRGain=0x01000, uint32_t iGGain=0x01000, uint32_t iBGain=0x01000);

//******************************************************************************************************************
/// \brief   Display an image in a view window with extra bayer conversion option. 
/// 
/// This function automatically converts the RAW image specified with pAqImageInfo into DIB, and display it in the view window in the window of handle
/// hWin made with \c J_Image_OpenViewWindow() or c J_Image_OpenViewWindowEx() . 
///
/// \param [in] hWin             Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx().
/// \param [out] pAqImageInfo    ImageInfo structure with information on RAW image.
/// \param [in] iBayerAlgorithm  Extra bayer conversion option.
/// \param [in] iRGain           Gain for the red color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iGGain           Gain for the green color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iBGain           Gain for the blue color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ShowImageEx(VIEW_HANDLE hWin, J_tIMAGE_INFO * pAqImageInfo, J_BAYER_ALGORITHM iBayerAlgorithm = BAYER_STANDARD, uint32_t iRGain=0x01000, uint32_t iGGain=0x01000, uint32_t iBGain=0x01000);

//******************************************************************************************************************
/// \brief   Set image offset and zoom ratio.
///
/// Sets the offset and the zoom ratio of the view window image. 
///
/// \param [in] hWin       Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
/// \param [in] *pOffset   POINT structure pointer with the horizontal, vertical offset (number of pixels).
/// \param [in] ZoomRatio  Zoom ratio.
///                        100:The original image is reduced to the window size.
///                        1  :1/100 of the original images is expanded to the window size. 
/// \retval                Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SetImageOffset(VIEW_HANDLE hWin, POINT *pOffset, int32_t ZoomRatio);

//******************************************************************************************************************
/// \brief   Move view window.
/// 
/// Moved the view window to the absolute position specified by \c Point
///
/// \param [in] hWin      Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
/// \param [in] *pPoint   Upper left position of view window.
/// \retval               Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_MoveViewWindow(VIEW_HANDLE hWin, POINT *pPoint);

//******************************************************************************************************************
/// \brief   Resize child(view) window.
/// 
/// Resize the child view window to new rectangle specified by \c Rect.
///
/// \param [in] hWin      Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
/// \param [in] *pRect    Relative rectangular coordinates from parent window.
/// \retval               Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ResizeChildWindow(VIEW_HANDLE hWin, RECT *pRect);

//******************************************************************************************************************
/// \brief   Get rectangular coordinates of the client area of View Window. 
/// 
/// The current position and size of the client area of the view window is read into a \c RECT structure.
///
/// \param [in] hWin      Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
/// \param [out] *pRect   Present window coordinates.
/// \retval               Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetViewWindowRect(VIEW_HANDLE hWin, RECT *pRect);

//******************************************************************************************************************
/// \brief   Get position and size of the view window including the windows frame
/// 
/// The current position and size of the frame of the view window is read into a \c RECT structure.
///
/// \param [in] hWin      Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
/// \param [out] *pRect   Present window coordinates.
/// \retval               Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetFrameWindowRect(VIEW_HANDLE hWin, RECT *pRect);

//******************************************************************************************************************
/// \brief   Set the title of view window
/// 
/// Modify the caption text of the view window 
///
/// \param [in] hWin           Handle to a view window previously opened using \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
/// \param [in] *pWindowTitle  Character string that wants to be displayed in title bar.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SetViewWindowTitle(VIEW_HANDLE hWin, LPCTSTR pWindowTitle);

/// @}
/// \defgroup WRAP_IMAGE_STREAM   Stream channel specific functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The Stream Channel specific functions are used for utilizing the automatic Image Acquisition via callback functions.
/// \par
/// The JAI SDK has got two major ways of handling the Image Acqusition:\n
/// 1) Automatic Image Acquisition handling via the \c J_Image_OpenStream(), \c J_Image_OpenStreamLight(), \c J_Image_CloseStream() and
/// \c J_Image_GetStreamInfo() functions. The JAI SDK will automatically take care of image buffer allocation as well as creating an Image
/// acquisition thread and all the images will be accessible via callback routines.\n
/// 2) Manual Image Acquisition handling via the \c J_DataStream_...() function described in this section. This requires that the user
/// application take care of all necessary memory allocation as well as creating an image acquisition thread the wait for "new image" events
/// from the underlying drivers. The biggest advantage is that this will give the application full control but this also increases the
/// complexity of the application.

//******************************************************************************************************************
/// \brief   Open image stream channel and start an internal image acquisition thread.
///
/// This function will start an internal stream processing thread, which will automatically call a CallBack function 
/// whenever it receives a new image. The callback can either be a static (global) function or it can be a local 
/// function defined inside a C++ class. The local callback functions should always be used in applications with 
/// multiple cameras connected.
///
/// \param [in] hCam          Camera Handle
/// \param [in] iChannel      Stream Channel
/// \param [in] CBObject      Class object that belongs to local CallBack function. If global static callback is used then set this parameter to NULL.
/// \param [in] CBFunction    CallBack function.
/// \param [out] phThread     Pointer of HANDLE that receives the new thread handle.
/// \param [in] iBufferSize   Buffersize to be allocated per image
/// \param [in] iMcIP         Multicast IP address
/// \retval                   Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that opens a live view window and starts acquisition
/// 
/// VIEW_HANDLE     g_hView;    // Global view handle
/// 
/// // Call back sample function
/// static void __stdcall StreamCBFunc(J_tIMAGE_INFO * pAqImageInfo)
/// {
///   // Shows image
///   J_Image_ShowImage(g_hView, pAqImageInfo);
/// }
/// 
/// // Live View sample function
/// BOOL LiveView(CAM_HANDLE hCamera)
/// {
///   THRD_HANDLE     hThread;
///   NODE_HANDLE     hNode;
///   int64_t         int64Val;
///   SIZE            ViewSize = {100, 100};
///   POINT           TopLeft = {0, 0};
/// 
///   // Get Width from the camera
///   if (J_Camera_GetValueInt64(hCamera, "Width", &int64Val) != J_ST_SUCCESS)
///     return FALSE;
///   ViewSize.cx = (LONG)int64Val;     // Set window size cx
/// 
///   // Get Height from the camera
///   if (J_Camera_GetValueInt64(hCamera, "Height", &int64Val) != J_ST_SUCCESS)
///     return FALSE;
///   ViewSize.cy = (LONG)int64Val;     // Set window size cy
/// 
///   // Open view window
///   if (J_Image_OpenViewWindow((LPCTSTR)"Live view!",
///                              &TopLeft,
///                              &ViewSize,
///                              &g_hView) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   // Open stream
///   void *vfptr = reinterpret_cast<void*>(StreamCBFunc);
///   J_IMG_CALLBACK_FUNCTION *cbfptr =  reinterpret_cast<J_IMG_CALLBACK_FUNCTION*>(&vfptr);
///
///   if (J_Image_OpenStream(hCamera,
///                          0,
///                          NULL,
///                          *cbfptr,
///                          &hThread,
///                          ViewSize.cx*ViewSize.cy*6) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   // Acquisition Start
///   if (J_Camera_ExecuteCommand(hCamera, "AcquisitionStart") != J_ST_SUCCESS)
///     return FALSE;
/// 
///   // Message loop
///   printf("Any key to exit!");
///   while (!_kbhit())
///   {
///     MSG msg;
///     if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
///       ::TranslateMessage(&msg);
///       ::DispatchMessage(&msg);
///     }
///   }
/// 
///   // Acquisition Stop
///   if (J_Camera_ExecuteCommand(hCamera, "AcquisitionStop") != J_ST_SUCCESS)
///     return FALSE;
/// 
///   // Close stream
///   J_Image_CloseStream(hThread);
/// 
///   // Close view window
///   J_Image_CloseViewWindow(g_hView);
///   
///   return TRUE;
/// }
/// \endcode
/// \sa \c J_Image_OpenViewWindow(), \c J_Camera_GetNodeByName(), \c J_Node_ExecuteCommand(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_OpenStream(CAM_HANDLE hCam, uint32_t iChannel, J_IMG_CALLBACK_OBJECT CBObject, J_IMG_CALLBACK_FUNCTION CBFunction, THRD_HANDLE * phThread, uint32_t iBufferSize=(J_XGA_WIDTH * J_XGA_HEIGHT * J_MAX_BPP), DWORD iMcIP=0);

//******************************************************************************************************************
/// \brief   Open image stream channel and let the factory automatically create all callback delegates and automatically display live images
///
/// \param [in] hCam         Handle to the camera
/// \param [in] iChannel     Stream channel index. This index is zero-based
/// \param [out] phThread    The handle of the created stream channel is returned.
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// This is the easiest way to create a view window that will display live video from the cameras. After opening the stream channel the acquisition has to be started
/// using the standard GenICam command \c AcquisitionStart.
/// 
/// \par
/// \code
/// // C Sample that opens a live view window and starts acquisition
///
/// VIEW_HANDLE     hView;
/// THRD_HANDLE     hThread;
/// J_STATUS_TYPE   retval;
/// NODE_HANDLE     hNode;
/// int64_t         int64Val;
/// SIZE            ViewSize = {100, 100};
/// POINT           TopLeft = {0, 0};
///
/// // Get Width from the camera
/// if (J_Camera_GetValueInt64(hCamera, "Width", &int64Val) == J_ST_SUCCESS)
/// {
///   ViewSize.cx = (LONG)int64Val;     // Set window size cx
/// }
/// // Get Height from the camera
/// if (J_Camera_GetValueInt64(hCamera, "Height", &int64Val) == J_ST_SUCCESS)
/// {
///   ViewSize.cy = (LONG)int64Val;     // Set window size cy
/// }
///
/// // Open view window
/// retval = J_Image_OpenViewWindow((LPCTSTR)"Live view!", &TopLeft, &ViewSize, &hView);
/// if (retval == J_ST_SUCCESS)
/// {
///   // Open stream
///   retval = J_Image_OpenStreamLight(hCamera, 0, &hThread); 
///   if (retval == J_ST_SUCCESS)
///   {
///     // Acquisition Start
///     J_Camera_ExecuteCommand(hCamera, "AcquisitionStart");
///   }
/// }
///
/// // Message loop
/// printf("Any key to exit!");
/// while (!_kbhit())
/// {
///   MSG msg;
///   if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
///       ::TranslateMessage(&msg);
///       ::DispatchMessage(&msg);
///   }
/// }
///
/// // Acquisition Stop
/// J_Camera_ExecuteCommand(hCamera, "AcquisitionStop");
///
/// // Close stream
/// retval = J_Image_CloseStream(hThread);
///
/// // Close view window
/// retval = J_Image_CloseViewWindow(hView);
/// \endcode
/// \sa \c J_Image_OpenViewWindow(), \c J_Camera_GetNodeByName(), \c J_Node_ExecuteCommand(), \c J_STATUS_CODES
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_OpenStreamLight(CAM_HANDLE hCam, uint32_t iChannel,  THRD_HANDLE* phThread);

//******************************************************************************************************************
/// \brief   Close a stream channel that has previously been opened using \c J_Image_OpenStream() or \c J_Image_OpenStreamLight().
/// 
/// Terminate the stream processing thread
///
/// \param [in] hThread   Handle of the thread.
/// \retval               Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_CloseStream(THRD_HANDLE hThread);

//******************************************************************************************************************
/// \brief                      Get detailed information about a data stream
/// \param [in] hThread         Handle to a valid thread object, obtained by \c J_Image_OpenStream() or \c J_Image_OpenStreamLight() function.
/// \param [in] iCmd            The information type that is requested
/// \param [out] pBuffer        Pointer to a buffer in which the information is stored.
/// \param [in,out] pSize       The size of the buffer. It must be equal or larger than J_STREAM_INFO_SIZE.
///                             The function sets the actual size of data stored into the buffer.
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_Image_OpenStream(), \c J_Image_OpenStreamLight(), \c J_STREAM_INFO_CMD
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetStreamInfo(THRD_HANDLE hThread, J_STREAM_INFO_CMD iCmd, void *pBuffer, uint32_t *pSize);

/// @}
/// \defgroup WRAP_IMAGE_MANI   Image manipulation functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The JAI SDK contains a number of functions that are used for manipulating the raw images acquired from the cameras. 

// Output Format of ConvertImage()

/// \brief Image conversion options used in \c J_Image_ConvertImage()
typedef enum _J_PIXEL_FORMAT_TYPE
{
   PF_24BIT_SWAP_RB = 1,     //!< Swaps the data of R and B. (24bit)
   PF_8BIT_Y,                //!< Converts from RGB(24/48bit) to Y(8bit).
   PF_24BIT_YUV,             //!< Not supported yet.
   PF_24BIT_YCBCR,           //!< Not supported yet.
   PF_48BIT_SWAP_RB,         //!< Swaps the data of R and B. (48bit)
   PF_16BIT_Y,               //!< Converts from RGB(24/48bit) to Y(16bit).
   PF_SWAP_RB,               //!< Selects the bit-length automatically. (24bit->24bit, 48bit->48bit)
   PF_Y,                     //!< Selects the bit-length automatically. (24bit->8bit, 48bit->16bit)
} J_PIXEL_FORMAT;

#pragma pack(1)
/// \brief Structure used for accessing Mono8 pixel values
typedef struct _t_Mono8
{
   uint8_t Value;
} MONO8, *PMONO8;

/// \brief Structure used for accessing Mono16 pixel values
typedef struct _t_Mono16
{
   uint16_t Value;
} MONO16, *PMONO16;

/// \brief Structure used for accessing BGR24 pixel values
typedef struct _t_BGR24
{
   uint8_t BValue;
   uint8_t GValue;
   uint8_t RValue;
} BGR24, *PBGR24;

/// \brief Structure used for accessing BGR24 pixel values
typedef struct _t_BGR48
{
   uint16_t BValue;
   uint16_t GValue;
   uint16_t RValue;
} BGR48, *PBGR48;

/// \brief Structure used for accessing different pixel values when using \c J_Image_GetPixel()
typedef struct _t_PixelValue
{
   union
   {
      MONO8 Mono8Type;    ///< Mono8 pixel format type
      MONO16 Mono16Type;  ///< Mono16 pixel format type
      BGR24 BGR24Type;    ///< BGR24 pixel format type
      BGR48 BGR48Type;    ///< BGR48 pixel format type
   } PixelValueUnion;
} PIXELVALUE, *PPIXELVALUE;
#pragma pack()

//******************************************************************************************************************
/// \brief   Convert image from RAW to DIB
/// 
/// Convert the RAW image specified with \c pAqImageInfo into 32 bit aRGB, and store it in the area specified with pBufferInfo.
/// The Alpha-channel will be set to 255 for all pixels.
///
/// \param [in] *pAqImageInfo   Pointer of J_tIMAGE_INFO structure with information on RAW data.
/// \param [in] *pBufferInfo    Pointer of J_tIMAGE_INFO structure in which information on converted image is written.
/// \param [in] iRGain          R Gain for Bayer (0x01000(4096) = 1.00)
/// \param [in] iGGain          G Gain for Bayer (0x01000(4096) = 1.00)
/// \param [in] iBGain          B Gain for Bayer (0x01000(4096) = 1.00)
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FromRawToDIB(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, uint32_t iRGain=0x01000, uint32_t iGGain=0x01000, uint32_t iBGain=0x01000);

//******************************************************************************************************************
/// \brief   Convert image from RAW to DIB with extra bayer conversion option.  
/// 
/// Convert the RAW image specified with \c pAqImageInfo into 32 bit aRGB, and store it in the area specified with pBufferInfo.
/// The Alpha-channel will be set to 255 for all pixels.
///
/// \param [in] *pAqImageInfo   Pointer of J_tIMAGE_INFO structure with information on RAW data.
/// \param [in] *pBufferInfo    Pointer of J_tIMAGE_INFO structure in which information on converted image is written.
/// \param [in] iBayerAlgorithm Extra bayer conversion option.
/// \param [in] iRGain          R Gain for Bayer (0x01000(4096) = 1.00)
/// \param [in] iGGain          G Gain for Bayer (0x01000(4096) = 1.00)
/// \param [in] iBGain          B Gain for Bayer (0x01000(4096) = 1.00)
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FromRawToDIBEx(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, J_BAYER_ALGORITHM iBayerAlgorithm = BAYER_STANDARD, uint32_t iRGain=0x01000, uint32_t iGGain=0x01000, uint32_t iBGain=0x01000);

//******************************************************************************************************************
/// \brief   Convert from RAW to an image that can be saved or modified
/// 
/// Convert the RAW image specified with AqImageInfo by the following rule, and store it in the area specified with BufferInfo.\n\n
/// Information on the converted image is returned to BufferInfo:\n
///  8bpp Grayscale ->  8bpp Grayscale\n
/// 10bpp Grayscale -> 16bpp Grayscale (shifted to MSB)\n
/// 24bpp RGB       -> 24bpp BGR\n
/// 30bpp RGB       -> 48bpp BGR (shifted to MSB)\n
///  8bpp Bayer     -> 24bpp BGR\n
/// 10bpp Bayer     -> 48bpp BGR (shifted to MSB)\n
///
/// \param [in] pAqImageInfo   Pointer of ImageInfo structure with information on RAW data.
/// \param [out] pBufferInfo   Pointer of ImageInfo structure in which the converted image is written.
/// \param [in] iRGain          Gain for the red color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iGGain          Gain for the green color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iBGain          Gain for the blue color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that converts a raw image and saved the result to a file
/// BOOL SaveImageToFile(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   J_tIMAGE_INFO tCnvImageInfo;    // Image info structure
/// 
///   // Allocate the buffer to hold converted the image
///   if (J_Image_Malloc(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Convert the raw image to image format
///   if (J_Image_FromRawToImage(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Save the image to disk in TIFF format
///   if (J_Image_SaveFile(&tCnvImageInfo, sFileName) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Free up the image buffer
///   if (J_Image_Free(&tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FromRawToImage(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, uint32_t iRGain=0x01000, uint32_t iGGain=0x01000, uint32_t iBGain=0x01000);

//******************************************************************************************************************
/// \brief   Convert from RAW to an image that can be saved or modified with extra bayer conversion option. 
/// 
/// Convert the RAW image specified with AqImageInfo by the following rule, and store it in the area specified with BufferInfo.\n\n
/// Information on the converted image is returned to BufferInfo:\n
///  8bpp Grayscale ->  8bpp Grayscale\n
/// 10bpp Grayscale -> 16bpp Grayscale (shifted to MSB)\n
/// 24bpp RGB       -> 24bpp BGR\n
/// 30bpp RGB       -> 48bpp BGR (shifted to MSB)\n
///  8bpp Bayer     -> 24bpp BGR\n
/// 10bpp Bayer     -> 48bpp BGR (shifted to MSB)\n
///
/// \param [in] pAqImageInfo   Pointer of ImageInfo structure with information on RAW data.
/// \param [out] pBufferInfo   Pointer of ImageInfo structure in which the converted image is written.
/// \param [in] iBayerAlgorithm Extra bayer conversion option.
/// \param [in] iRGain          Gain for the red color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iGGain          Gain for the green color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iBGain          Gain for the blue color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample converts raw Bayer image using extended color interpolation and converts
/// // it to Y (monochrome) and saves the result to a file
///
/// J_tIMAGE_INFO   BufferInfo;
/// J_tIMAGE_INFO   YBufferInfo;
/// 
/// // Allocates buffer memory for RGB image.
/// if (J_ST_SUCCESS == J_Image_Malloc(pAqImageInfo, &BufferInfo))
/// {
///   // Converts from RAW to full bit image.
///   if(J_ST_SUCCESS == J_Image_FromRawToImageEx(pAqImageInfo,
///                                               &BufferInfo,
///                                               BAYER_EXTEND,
///                                               iBayerGainR,
///                                               iBayerGainG,
///                                               iBayerGainB))
///   {
///     J_Image_SaveFile(&BufferInfo, sFileName); // Stores RGB image to a TIFF file.
///
///     // Allocates buffer memory for Y image.
///     if(J_ST_SUCCESS == J_Image_MallocEx(&BufferInfo,
///                                         &YBufferInfo,
///                                         PF_Y/*PF_16BIT_Y*//*PF_8BIT_Y*/))
///     {
///       // Converts from RGB to Y.
///       if(J_ST_SUCCESS == J_Image_ConvertImage(&BufferInfo,
///                                               &YBufferInfo,
///                                               PF_Y/*PF_16BIT_Y*//*PF_8BIT_Y*/))
///       {
///         J_Image_SaveFile(&YBufferInfo, sFileNameY); // Stores Y image to a TIFF file.
///       }
///
///       // Frees buffer memory for Y image.
///       J_Image_Free(&YBufferInfo);
///     }
///
///     // Frees buffer memory for RGB image.
///     J_Image_Free(&BufferInfo);
///   }
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FromRawToImageEx(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo, J_BAYER_ALGORITHM iBayerAlgorithm = BAYER_STANDARD, uint32_t iRGain=0x01000, uint32_t iGGain=0x01000, uint32_t iBGain=0x01000);

//******************************************************************************************************************
/// \brief   Convert from an image to an image with some other format such as Color to Monochrome conversion. 
/// 
/// Convert an image, which has been previously converted with J_Image_FromRawToImage() or J_Image_FromRawToImageEx(),
/// to an image with a format specified with iOutputFormat.
///
/// \param [in] pInputBufferInfo   Pointer of ImageInfo structure which had been previously converted with J_Image_FromRawToImage() or J_Image_FromRawToImageEx().
/// \param [out] pOutputBufferInfo   Pointer of ImageInfo structure in which the converted image will be stored.
/// \param [in] iOutputFormat Pixel format to which the image will be converted
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample converts raw Bayer image using extended color interpolation and converts
/// // it to Y (monochrome) and saves the result to a file
///
/// J_tIMAGE_INFO   BufferInfo;
/// J_tIMAGE_INFO   YBufferInfo;
/// 
/// // Allocates buffer memory for RGB image.
/// if (J_ST_SUCCESS == J_Image_Malloc(pAqImageInfo, &BufferInfo))
/// {
///   // Converts from RAW to full bit image.
///   if(J_ST_SUCCESS == J_Image_FromRawToImageEx(pAqImageInfo,
///                                               &BufferInfo, 
///                                               BAYER_EXTEND,
///                                               iBayerGainR,
///                                               iBayerGainG,
///                                               iBayerGainB))
///     J_Image_SaveFile(&BufferInfo, sFileName); // Stores RGB image to a TIFF file.
///
///   // Allocates buffer memory for Y image.
///   if(J_ST_SUCCESS == J_Image_MallocEx(&BufferInfo,
///                                       &YBufferInfo,
///                                       PF_Y/*PF_16BIT_Y*//*PF_8BIT_Y*/))
///   {
///     // Converts from RGB to Y.
///     if(J_ST_SUCCESS == J_Image_ConvertImage(&BufferInfo,
///                                             &YBufferInfo,
///                                             PF_Y/*PF_16BIT_Y*//*PF_8BIT_Y*/))
///       J_Image_SaveFile(&YBufferInfo, sFileNameY); // Stores Y image to a TIFF file.
///
///     // Frees buffer memory for Y image.
///     J_Image_Free(&YBufferInfo);
///   }
///
///   // Frees buffer memory for RGB image.
///   J_Image_Free(&BufferInfo);
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ConvertImage(J_tIMAGE_INFO * pInputBufferInfo, J_tIMAGE_INFO * pOutputBufferInfo, J_PIXEL_FORMAT iOutputFormat);

//******************************************************************************************************************
/// \brief Allocate the buffer memory for the image
///
/// Allocate the buffer to store the converted image.
/// Information on the allocated buffer is returned in J_tIMAGE_INFO structure.
///
/// \param [in] pAqImageInfo   Pointer of J_tIMAGE_INFO structure with information on RAW data.
/// \param [out] pBufferInfo   Pointer of J_tIMAGE_INFO structure in which the information about the allocated buffer will be stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that converts a raw image and saved the result to a file
/// BOOL SaveImageToFile(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   J_tIMAGE_INFO tCnvImageInfo;    // Image info structure
/// 
///   // Allocate the buffer to hold converted the image
///   if (J_Image_Malloc(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Convert the raw image to image format
///   if (J_Image_FromRawToImage(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Save the image to disk in TIFF format
///   if (J_Image_SaveFile(&tCnvImageInfo, sFileName) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   // Free up the image buffer
///   if (J_Image_Free(&tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_Malloc(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo);

//******************************************************************************************************************
/// \brief   Allocate the buffer memory for the DIB (32 bit ARGB)
///
/// Allocate the buffer to store the converted image from J_Image_FromRawToDIB.
/// Information on the allocated buffer is returned in J_tIMAGE_INFO structure.
///
/// \param [in] pAqImageInfo   Pointer of J_tIMAGE_INFO structure with information on RAW data.
/// \param [out] pBufferInfo   Pointer of J_tIMAGE_INFO structure in which the information about the allocated buffer will be stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that converts a raw image and saved the result to a file
/// BOOL ConvertImageToDIB(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   J_tIMAGE_INFO tCnvImageInfo;    // Image info structure
/// 
///   // Allocate the buffer to hold converted the image
///   if (J_Image_MallocDIB(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Convert the raw image to image format
///   if (J_Image_FromRawToDIB(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Process the image
///   ... do something with the ARGB buffer
///   // Free up the image buffer
///   if (J_Image_Free(&tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_MallocDIB(J_tIMAGE_INFO * pAqImageInfo, J_tIMAGE_INFO * pBufferInfo);

//******************************************************************************************************************
/// \brief   Allocate the buffer memory for the image to use J_Image_ConvertImage()
///
/// Allocate the buffer to store the converted image to use J_Image_ConvertImage().
/// Information on the allocated buffer is returned in pOutputBufferInfo.
///
/// \param [in] pInputBufferInfo     Pointer of J_tIMAGE_INFO structure with information on image converted with J_Image_FromRawToImage() or J_Image_FromRawToImageEx().
/// \param [out] pOutputBufferInfo   Pointer of J_tIMAGE_INFO structure in which the information about the allocated buffer will be stored.
/// \param [in] iOutputFormat        Pixel format with which the image will be allocated
/// \retval                          Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample converts raw Bayer image using extended color interpolation and converts
/// // it to Y (monochrome) and saves the result to a file
///
/// J_tIMAGE_INFO   BufferInfo;
/// J_tIMAGE_INFO   YBufferInfo;
/// 
/// // Allocates buffer memory for RGB image.
/// if (J_ST_SUCCESS == J_Image_Malloc(pAqImageInfo, &BufferInfo))
/// {
///   // Converts from RAW to full bit image.
///   if(J_ST_SUCCESS == J_Image_FromRawToImageEx(pAqImageInfo,
///                                               &BufferInfo,
///                                               BAYER_EXTEND,
///                                               iBayerGainR,
///                                               iBayerGainG,
///                                               iBayerGainB))
///     J_Image_SaveFile(&BufferInfo, sFileName); // Stores RGB image to a TIFF file.
///
///   // Allocates buffer memory for Y image.
///   if(J_ST_SUCCESS == J_Image_MallocEx(&BufferInfo,
///                                       &YBufferInfo,
///                                       PF_Y/*PF_16BIT_Y*//*PF_8BIT_Y*/))
///   {
///     // Converts from RGB to Y.
///     if(J_ST_SUCCESS == J_Image_ConvertImage(&BufferInfo,
///                                             &YBufferInfo,
///                                             PF_Y/*PF_16BIT_Y*//*PF_8BIT_Y*/))
///       J_Image_SaveFile(&YBufferInfo, sFileNameY); // Stores Y image to a TIFF file.
///
///     // Frees buffer memory for Y image.
///     J_Image_Free(&YBufferInfo);
///   }
///
///   // Frees buffer memory for RGB image.
///   J_Image_Free(&BufferInfo);
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_MallocEx(J_tIMAGE_INFO * pInputBufferInfo, J_tIMAGE_INFO * pOutputBufferInfo, J_PIXEL_FORMAT iOutputFormat);

//******************************************************************************************************************
/// \brief   Free a previously allocated image buffer
/// 
/// Frees the memory allocated to the image buffer 
///
/// \param [in] pBufferInfo  Pointer of J_tIMAGE_INFO structure with a buffer previously allocated with \c J_Image_Malloc()
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that converts a raw image and saved the result to a file
/// BOOL SaveImageToFile(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   J_tIMAGE_INFO tCnvImageInfo;    // Image info structure
/// 
///   // Allocate the buffer to hold converted the image
///   if (J_Image_Malloc(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Convert the raw image to image format
///   if (J_Image_FromRawToImage(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Save the image to disk in TIFF format
///   if (J_Image_SaveFile(&tCnvImageInfo, sFileName) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Free up the image buffer
///   if (J_Image_Free(&tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_Free(J_tIMAGE_INFO * pBufferInfo);

/// @}
/// \defgroup WRAP_IMAGE_save   Image save functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief These functions are used for saving the acquired images to disk. 
/// \par
/// The following file formats are currently supported: \n
/// 1) Tiff: This format is an uncompressed format and it can be used for saving images in up to 16-bit pixel depth (48-bit color). \n
/// 2) Jpeg: This format compressed format where the compression ration can be specified as a parameter to the \c J_Image_SaveFileEx(). This format
/// only support 8-bit pixel depth (24-bit color) \n
/// 3) BMP: This is an uncompressed Windows Bitmap format and it only supports 8-bit pixel depth (24-bit color) \n
/// \par
/// The \c J_Image_SaveFileRaw() function will save the contents of the acquired image without any image file header information - just the RAW data
/// received from the camera.

/// \brief File type used in \c J_Image_SaveFileEx()
typedef enum
{
   J_FF_TIFF = 1,    //!< TIFF file format
   J_FF_JPEG,        //!< JPEG file format
   J_FF_BMP,         //!< BMP file format
} J_SIF_FILE_FORMAT;

//******************************************************************************************************************
/// \brief   Save an image to disk as TIFF file
/// 
/// Saves an image to disk as a TIFF file. A raw image needs to be converted using \c J_Image_FromRawToImage() before it can be saved
///
/// \param [in] pBufferInfo  Pointer of J_tIMAGE_INFO structure in which the information about the allocated buffer will be stored.
/// \param [in] pPath        Full file name and path of the tiff file to be stored to disk.
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that converts a raw image and saved the result to a file
/// BOOL SaveImageToFile(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   J_tIMAGE_INFO tCnvImageInfo;    // Image info structure
/// 
///   // Allocate the buffer to hold converted the image
///   if (J_Image_Malloc(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Convert the raw image to image format
///   if (J_Image_FromRawToImage(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Save the image to disk in TIFF format
///   if (J_Image_SaveFile(&tCnvImageInfo, sFileName) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Free up the image buffer
///   if (J_Image_Free(&tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SaveFile(J_tIMAGE_INFO * pBufferInfo, LPCTSTR pPath);


//******************************************************************************************************************
/// \brief   Save an image to disk as TIFF, JPEG or BMP file
/// 
/// Saves an image to disk as a TIFF, JPEG or BMP file. A raw image needs to be converted using \c J_Image_FromRawToImage() before it can be saved
///
/// \param [in] pBufferInfo       Pointer of J_tIMAGE_INFO structure in which the information about the allocated buffer will be stored.
/// \param [in] pPath             Full file name and path of the tiff file to be stored to disk.
/// \param [in] iFileFormat       Type of the file to be saved.
/// \param [in] iEncoderParameter Encoder parameter. The meaning of the value depends on which File format that is selected. If Jpeg file format is selected this parameter specifies the Quality level for the jpeg file. Acceptable value range is 1 to 100, default is 75, 5-95 is useful range. Larger number goes toward higher quality and larger file size. 
/// \retval                       Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that converts a raw image and saved the result to a file
/// BOOL SaveImageToFile(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   J_tIMAGE_INFO tCnvImageInfo;    // Image info structure
/// 
///   // Allocate the buffer to hold converted the image
///   if (J_Image_Malloc(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Convert the raw image to image format
///   if (J_Image_FromRawToImage(ptRawImageInfo, &tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Save the image to disk in TIFF format
///   if (J_Image_SaveFileEx(&tCnvImageInfo, sFileName, J_FF_TIFF) != J_ST_SUCCESS)
///     return FALSE;
///
///   // Free up the image buffer
///   if (J_Image_Free(&tCnvImageInfo) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SaveFileEx(J_tIMAGE_INFO * pBufferInfo, LPCTSTR pPath, J_SIF_FILE_FORMAT iFileFormat=J_FF_TIFF,  uint8_t iEncoderParameter=75);

//******************************************************************************************************************
/// \brief   Save an image to disk as a raw binary file
/// 
/// Saves an image to disk as a raw binary file. The image is saved without any header information so it is not possible to display the image directly.
///
/// \param [in] pBufferInfo  Pointer of J_tIMAGE_INFO structure in which the raw image data is read.
/// \param [in] pPath        Full file name and path of the raw binary file to be stored to disk.
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \par
/// \code
/// // C Sample that saves a raw image to a file
/// BOOL SaveImageToRawBinaryFile(J_tIMAGE_INFO* ptRawImageInfo, LPCTSTR sFileName)
/// {
///   // Save the image to disk in raw binary format
///   if (J_Image_SaveFileRaw(&tCnvImageInfo, sFileName) != J_ST_SUCCESS)
///     return FALSE;
/// 
///   return TRUE;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SaveFileRaw(J_tIMAGE_INFO * pBufferInfo, LPCTSTR pPath);

/// @}
/// \defgroup WRAP_IMAGE_pixel   Pixel access functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief These functions are used for accessing pixels directly inside an image. A raw image needs to be converted using 
/// \c J_Image_FromRawToImage() before pixels can be read or written.

//******************************************************************************************************************
/// \brief   Get the pixel at a specified position
/// 
/// Read the current pixel value at the position \c pPoint. A raw image needs to be converted using \c J_Image_FromRawToImage() before pixels can be read.
///
/// \param [in] pBufferInfo  J_tIMAGE_INFO structure in which the pixel value will be read.
/// \param [in] pPoint       Absolute position in which pixel value is read.
/// \param [out] pPixel      Pixel value data read from the image.
///                          Where to get the actual values from the \c Pixel Value structure depends on the \c PixelFormat that can be read from \c pBufferInfo.
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_tIMAGE_INFO
/// \par
/// \code
/// // Here we want to read a certain pixel value from the image:
/// // In order to do so we need to convert the image from Raw to Image (to interpolate
/// // Bayer if needed and unpack the Packed pixel formats as well)
///
/// // Allocate the buffer to hold converted the image. (We only want to do this once for
/// // performance reasons)
/// if (m_CnvImageInfo.pImageBuffer == NULL)
/// {
///   if (J_Image_Malloc(pAqImageInfo, &m_CnvImageInfo) != J_ST_SUCCESS)
///     return;
/// }
///
/// if (m_CnvImageInfo.pImageBuffer != NULL)
/// {
///   // Convert the raw image to "real image" format
///   if (J_Image_FromRawToImage(pAqImageInfo, &m_CnvImageInfo) == J_ST_SUCCESS)
///   {
///     // Now we can read the pixel value
///     POINT pt;
///     pt.x = m_XPos;
///     pt.y = m_YPos;
///
///     PIXELVALUE pixelValue;
///
///     if (J_Image_GetPixel(&m_CnvImageInfo, &pt, &pixelValue) == J_ST_SUCCESS)
///     {
///       // Use the PixelType to select how to interpret the pixelValue structure info
///       switch (m_CnvImageInfo.iPixelType)
///       {
///         case J_GVSP_PIX_MONO8:
///           valueString.Format(_T("Mono8 PixelValue at (%d,%d): %d"),
///                              m_XPos,
///                              m_YPos,
///                              pixelValue.PixelValueUnion.Mono8Type.Value);
///           break;
/// 
///         case J_GVSP_PIX_MONO16:
///           // Be aware that the Mono 16 values are "normalized" into 16-bit values by
///           // shifting the 10- and 12-bit values from the cameras up to MSB!
///           // If you need to get 10-bit or 12-bit values instead of the normalized
///           // values you will ned to do like this:
///           // value = pixelValue.PixelValueUnion.Mono16Type.Value & OxFFC0) >> 6 // 10bit mono
///           // ... or
///           // value = pixelValue.PixelValueUnion.Mono16Type.Value & OxFFF0) >> 4 // 12bit mono
///           valueString.Format(_T("Mono16 Pixel Value at (%d,%d): %d"),
///                              m_XPos,
///                              m_YPos,
///                              pixelValue.PixelValueUnion.Mono16Type.Value);
///           break;
///
///         case J_GVSP_PIX_BGR8_PACKED:
///           valueString.Format(_T("BGR24 Pixel Value at (%d,%d): (R,G,B)=(%d,%d,%d)"),
///                              m_XPos,
///                              m_YPos,
///                              pixelValue.PixelValueUnion.BGR24Type.RValue,
///                              pixelValue.PixelValueUnion.BGR24Type.GValue,
///                              pixelValue.PixelValueUnion.BGR24Type.BValue);
///           break;
///
///         case J_GVSP_PIX_BGR16_PACKED_INTERNAL:
///           // Be aware that the BGR 16 values are "normalized" into 16-bit values by
///           // shifting the 10- and 12-bit values from the cameras up to MSB!
///           // If you need to get 10-bit or 12-bit values instead of the normalized
///           // values you will ned to do like this:
///           // RValue = pixelValue.PixelValueUnion.BGR48Type.RValue & OxFFC0) >> 6 // 10bit Color
///           // GValue = pixelValue.PixelValueUnion.BGR48Type.GValue & OxFFC0) >> 6 // 10bit Color
///           // BValue = pixelValue.PixelValueUnion.BGR48Type.BValue & OxFFC0) >> 6 // 10bit Color
///           // ... or
///           // RValue = pixelValue.PixelValueUnion.BGR48Type.RValue & OxFFF0) >> 4 // 12bit Color
///           // GValue = pixelValue.PixelValueUnion.BGR48Type.GValue & OxFFF0) >> 4 // 12bit Color
///           // BValue = pixelValue.PixelValueUnion.BGR48Type.BValue & OxFFF0) >> 4 // 12bit Color
///           valueString.Format(_T("BGR48 Pixel Value at (%d,%d): (R,G,B)=(%d,%d,%d)"),
///                              m_XPos,
///                              m_YPos,
///                              pixelValue.PixelValueUnion.BGR48Type.RValue,
///                              pixelValue.PixelValueUnion.BGR48Type.GValue,
///                              pixelValue.PixelValueUnion.BGR48Type.BValue);
///           break;
///       }
///     }
///   }
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetPixel(J_tIMAGE_INFO * pBufferInfo, POINT *pPoint, void * pPixel);

//******************************************************************************************************************
/// \brief   Set the pixel at a specified position
/// 
/// Writes the current pixel value at the position \c pPoint. A raw image needs to be converted using \c J_Image_FromRawToImage() before pixels can be written.
///
/// \param [in] pBufferInfo    J_tIMAGE_INFO structure in which the pixel value will be written.
/// \param [in] pPoint         Absolute position in which pixel value is written.
/// \param [in] pPixel         Pixel value data to write.
///                            Where to set the actual values inside the \c PixelValue structure depends on the \c PixelFormat that can be read from \c BufferInfo.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \sa \c J_tIMAGE_INFO
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SetPixel(J_tIMAGE_INFO * pBufferInfo, POINT *pPoint, void * pPixel);

//******************************************************************************************************************
/// \brief   Measures pixel mean value.
///
/// \param [in] pBufferInfo    J_tIMAGE_INFO structure which contains a converted image.
/// \param [in] pMeasureRect   Measurement rectangular coordinates.
/// \param [out] pRGBAverage   Measured average pixel value for all the pixels inside the measurement rectangle. 
///                            The pixel format for the pRGBAverage structure will always be in BGR48 format (16 bit per color)
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetAverage(J_tIMAGE_INFO * pBufferInfo, RECT *pMeasureRect, J_tBGR48 *pRGBAverage);

/// @}
/// \defgroup WRAP_IMAGE_WB   Image white balance control functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The JAI SDK has the possibility to estimate the gain values needed in order to get a correct White-balance for color
/// cameras with a Bayer Color Filter Array. The White-balancing is later done on the host PC side during the color-interpolation

//******************************************************************************************************************
/// \brief   Set RGB software gain.
/// 
/// The gain is only used inside the factory when the \c J_Image_OpenStreamLight() has been used to display live video. 
/// \param [in] hThread     Handle to the stream channel opened using \c J_Image_OpenStream() or \c J_Image_OpenStreamLight().
/// \param [in] iRGain      Gain for the red color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iGGain      Gain for the green color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [in] iBGain      Gain for the blue color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \note                 
/// This function doesn't return the white balance gain from inside the camera. It is only used inside the factory when 
/// the \c J_Image_OpenStreamLight() has been used to display live video
/// \retval                 Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                   This function doesn't affect the gain in the camera.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE    J_Image_SetGain(HANDLE hThread, uint32_t iRGain, uint32_t iGGain, uint32_t iBGain);

//******************************************************************************************************************
/// \brief   Get RGB software gain.
/// 
/// The gain is only used inside the factory when the \c J_Image_OpenStreamLight() has been used to display live video. \n
/// If \c J_Image_ExecuteWhiteBalance() has been called, the resulting gain values can be read with this function after the next 
/// image has been received on the stream channel.
/// \param [in] hThread     Handle to the stream channel opened using \c J_Image_OpenStream() or \c J_Image_OpenStreamLight().
/// \param [out] piRGain    Gain for the red color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [out] piGGain    Gain for the green color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \param [out] piBGain    Gain for the blue color channel for Bayer conversion (0x01000(4096) = 1.00)
/// \note                 
/// This function doesn't affect the white balance settings inside the camera.
/// \retval                 Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE    J_Image_GetGain(HANDLE hThread, uint32_t * piRGain, uint32_t * piGGain, uint32_t * piBGain);

//******************************************************************************************************************
/// \brief   Calculate software auto white balance
/// 
/// When this function has been called, the resulting gain values can be read with \c J_Image_GetGain() function after the next 
/// image has been received on the stream channel.
/// The resulting white balance is only used inside the factory when the \c J_Image_OpenStreamLight() has been used to display live video. 
/// 
/// \param [in] hThread     Handle to the stream channel opened using \c J_Image_OpenStream() or \c J_Image_OpenStreamLight().
/// \retval                 Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note
/// After executing this function, you can use the \c J_Image_GetGain() function to get the resulting gain values.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE    J_Image_ExecuteWhiteBalance(THRD_HANDLE hThread);

/// @}
/// \defgroup WRAP_SETTING   Save or Load Setting functions
/// \ingroup WRAP_CAM
/// @{
/// \brief It is possible to save camera settings to disk and later on load them into the camera. It is possible to control
/// the way the load and save functions works

/////////////////////////////////////////////////////////////////////////////////////
/// \brief Save Camera Settings functionality enumeration
/// \note This enum is used in the \c J_Camera_SaveSettings() function to select the way the settings are persisted
/// \sa \c J_Camera_SaveSettings()
/////////////////////////////////////////////////////////////////////////////////////
typedef enum _J_SAVE_SETTINGS_FLAG
{
   SAVE_AUTO,               ///< Save all camera settings to disk. If the usage of the GenICam Streamable flags is detected then it will be used. Otherwise all features that are both Readable and Writable will be saved
   SAVE_STREAMABLE_ONLY,    ///< Force the persistence algorithm to use the GenICam Streamable flag. All features that have the Streamable flag set and are both Readable and Writable will be saved
   SAVE_FORCE_ALL,          ///< Force the persistence algorithm to ignore the GenICam Streamable flag and save all features that are both Readable and Writable
} J_SAVE_SETTINGS_FLAG;

/////////////////////////////////////////////////////////////////////////////////////
/// \brief Load Camera Settings functionality enumeration
/// \note This enum is used in the \c J_Camera_LoadSettings() function to select the way the settings are loaded
/// \sa \c J_Camera_LoadSettings()
/////////////////////////////////////////////////////////////////////////////////////
typedef enum _J_LOAD_SETTINGS_FLAG
{
   LOAD_AUTO,               ///< Load all camera settings from disk and validate the settings before writing the values to the camera. If the validation fails then a detailed validation error information will be available using the \c J_Camera_GetSettingsValidationErrorInfo() function. If a validation error is detected then the writing to the camera will be aborted.
   LOAD_VALIDATE_ONLY,      ///< Load all camera settings from disk and validate the settings. The values will not be written to the camera. If the validation fails then a detailed validation error information will be available using the \c J_Camera_GetSettingsValidationErrorInfo() function.
   LOAD_FORCE_WRITE,        ///< Force all the settings to be written to the camera without validating the feature names or the values.
} J_LOAD_SETTINGS_FLAG;

//******************************************************************************************************************
/// \brief   Save camera settings to disk. The format of the settings file is XML so it is possible to edit it using
///          a normal text editor.
/// \param [in] hCam              Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sSettingsFileName Zero terminated string containing the file name for the settings file.
/// \param [in] saveFlag          Enumeration value that selects the behaviour of the Save function. See \c J_SAVE_SETTINGS_FLAG for more information.
/// \retval                 Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // These small samples illustrates how to save setting from a connected 
/// // camera into a XML-file:
///
/// J_STATUS_TYPE   retval;
/// // Save settings using default SAVE_AUTO flag. The algorithm will 
/// // automatically detect if the camera utilizes the GenICam Streamable flag.
/// // If no Streamable flags are found then it will save all settings that are
/// // both Readable and Writable.
/// retval = J_Camera_SaveSettings(m_hCam, "CameraSettings.script");
///
/// // Save settings using SAVE_STREAMABLE_ONLY flag.
/// // This will enforce the usage of the Streamable flag
/// retval = J_Camera_SaveSettings(m_hCam, 
///                                "CameraSettings.script", 
///                                SAVE_STREAMABLE_ONLY);
///
/// // Save settings using SAVE_FORCE_ALL flag. 
/// // This will save all features that are both Readable and Writable
/// retval = J_Camera_SaveSettings(m_hCam, 
///                                "CameraSettings.script", 
///                                SAVE_FORCE_ALL);
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_SaveSettings(CAM_HANDLE hCam, int8_t* sSettingsFileName, J_SAVE_SETTINGS_FLAG saveFlag = SAVE_AUTO);

//******************************************************************************************************************
/// \brief   Load camera settings from disk and either validate or send the the settings to the connected camera.
/// \param [in] hCam              Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] sSettingsFileName Zero terminated string containing the file name for the settings file.
/// \param [in] loadFlag          Enumeration value the selects the behaviour of the Load function. See \c J_LOAD_SETTINGS_FLAG for more information
/// \retval                 Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This small samples illustrates how to validate the contents of a settings file:
///
/// J_STATUS_TYPE   retval;
/// // We want to validate a settings file and print out any warnings or errors
/// // to the Debug Output window
/// retval = J_Camera_LoadSettings(m_hCam, 
///                                "CameraSettings.script", 
///                                LOAD_VALIDATE_ONLY);
///
/// if ((retval==J_ST_VALIDATION_ERROR)||(retval==J_ST_VALIDATION_WARNING))
/// {
///     uint32_t BufferSize = 0;
///     // First we need to see how big an error string buffer we need so
///     // we set the BufferSize to the value 0. BufferSize will then be 
///     // updated with the actual size required.
///     retval = J_Factory_GetSettingsValidationErrorInfo(NULL, &BufferSize);
///
///     if (retval == J_ST_SUCCESS)
///     {
///         // Allocate enough room for the info string!
///         int8_t *buffer = (int8_t *)malloc(BufferSize);
///
///         // And now we get the actual erro information
///         retval = J_Camera_GetSettingsValidationErrorInfo(m_hCam, buffer, &BufferSize);
///         OutputDebugStringA(buffer);
///
///         // Remember to free the buffer again
///         free(buffer);
///     }
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_LoadSettings(CAM_HANDLE hCam, int8_t* sSettingsFileName, J_LOAD_SETTINGS_FLAG loadFlag = LOAD_AUTO);

//******************************************************************************************************************
/// \brief                      Get Camera Settings Validation error information
/// \param [in] hCam            Handle to a valid camera object, obtained by \c J_Camera_Open() function.
/// \param [in] errorInfoBuffer Pointer to a buffer to retrieve detailed validation information in.
/// \param [in] pBufferSize     Pointer to a variable that contains the size of the user allocated buffer. If the size
///                             is set to 0 by the user application then this function will return the size required to
///                             hold the complete error information string. If the size is set to non-zero then it will 
///                             be updated with the actual size of the string returned in errorInfoBuffer
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \note                       The Validation error information string has seperated each validation error or warning by
///                             a carriage-return character.
/// \code
/// // This small samples illustrates how to validate the contents of a settings file:
///
/// J_STATUS_TYPE   retval;
/// // We want to validate a settings file and print out any warnings or errors
/// // to the Debug Output window
/// retval = J_Camera_LoadSettings(m_hCam, 
///                                "CameraSettings.script", 
///                                LOAD_VALIDATE_ONLY);
///
/// if ((retval==J_ST_VALIDATION_ERROR)||(retval==J_ST_VALIDATION_WARNING))
/// {
///     uint32_t BufferSize = 0;
///     // First we need to see how big an error string buffer we need so
///     // we set the BufferSize to the value 0. BufferSize will then be 
///     // updated with the actual size required.
///     retval = J_Factory_GetSettingsValidationErrorInfo(NULL, &BufferSize);
///
///     if (retval == J_ST_SUCCESS)
///     {
///         // Allocate enough room for the info string!
///         int8_t *buffer = (int8_t *)malloc(BufferSize);
///
///         // And now we get the actual error information
///         retval = J_Camera_GetSettingsValidationErrorInfo(m_hCam, buffer, &BufferSize);
///         OutputDebugStringA(buffer);
///
///         // Remember to free the buffer again
///         free(buffer);
///     }
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Camera_GetSettingsValidationErrorInfo(CAM_HANDLE hCam, int8_t* errorInfoBuffer, uint32_t *pBufferSize);

/// @}
/// \defgroup WRAP_IMAGE_HDR   High Dynamic Range functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The JAI SDK support creating High Dynamic Range images from multiple exposures of the same scene where the
/// exposure time and/or gain is different during the two exposures. This is very usefull for the 2-CCD monochrome 
/// cameras as well as the JAI cameras that has got built-in support for multiple sequences. But basically these functions 
/// will work with any camera as well where the exposure is controlled by an application and where High Dynamic
/// Range output is required.

//******************************************************************************************************************
/// \brief   Allocate the buffer memory for the HDR output image.
/// 
/// \par
/// 16-bit per pixel will be allocated and they will be 16-byte alligned in order to speed up the HDR processing.
/// The allocated HDR image will need to be freed up using \c J_Image_Free()
/// Information on the allocated buffer is returned in the J_tIMAGE_INFO structure.
/// \note The input images needs to be Mono8, Mono10 or Mono12 pixel format for monochrome images or Bayer8, Bayer10
/// or Bayer12 for color images. It is also required that the two images has got identical pixel format
///
/// \param [in] pImageInfo1    Pointer to J_tIMAGE_INFO structure with information on RAW data for first image.
/// \param [in] pImageInfo2    Pointer to J_tIMAGE_INFO structure with information on RAW data for second image.
/// \param [out] pBufferInfo   Pointer to J_tIMAGE_INFO structure in which the information about the allocated buffer will be stored.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_MallocHDR(J_tIMAGE_INFO * pImageInfo1, J_tIMAGE_INFO * pImageInfo2, J_tIMAGE_INFO * pBufferInfo);
 
//******************************************************************************************************************
/// \brief   Analyzes two images and try to determine the exposure difference (called Dark Gain) later to be used 
///          as a parameter to the HDR fusing function \c J_Image_FuseHDR()
/// \par
/// This function runs through the image data from the two input images and try to determine the exposure difference
/// between the two images by looking at the pixels with the highest values that are not saturated.
/// It will break out the analyzis when the gain start to become "unlinear" (when for instance the darkest image 
/// values gets close to the black level).
/// It will return J_ST_ERROR if it didn't find any pixels where there is an "overlap" so it couldn't calculate the gain.
/// The function also updates a flag that indicates wheter Image 1 is brighter than Image 2. 
/// 
/// \par
/// The minimun values inside Image 1 and Image 2 are returned as recommended Black Level values.
/// If none of the pixel values in any of the input images are as low as the black level then the estimeted 
/// black level will be wrong. This means that the black level values used in \c J_Image_FuseHDR() need to be set
/// to a more correct value. A way to identify that this is the case is to compare the black level returned for the 
/// two input images. If the black level value for the brightest image is more than twice the black levels of the dark
/// image then the recommended black level for the bright image might very well be wrong.
///
/// \note The input images needs to be Mono8, Mono10 or Mono12 pixel format for monochrome images or Bayer8, Bayer10
/// or Bayer12 for color images. It is also required that the two images has got identical pixel format
///
///
/// \param [in] pImageInfo1         Pointer to J_tIMAGE_INFO structure with information on RAW data for first image.
/// \param [in] pImageInfo2         Pointer to J_tIMAGE_INFO structure with information on RAW data for second image.
/// \param [out] pImage1IsBrighter  Pointer to flag that will be set to 1 if Image 1 is brighter than Image 2.
/// \param [out] pBlackLevelImage1  Pointer to an int value that will be updated with the recommended Black Level for Image 1 to be subtracted from the image data during HDR merging.
/// \param [out] pBlackLevelImage2  Pointer to an int value that will be updated with the recommended Black Level for Image 2 to be subtracted from the image data during HDR merging.
/// \param [out] pDarkGain          Pointer to a float value that will contain the gain between the two images.
/// \retval                         Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_AnalyzeHDR(J_tIMAGE_INFO * pImageInfo1, J_tIMAGE_INFO * pImageInfo2, uint8_t* pImage1IsBrighter, int32_t *pBlackLevelImage1, int32_t *pBlackLevelImage2, float* pDarkGain);
 
//******************************************************************************************************************
/// \brief   Fuses two input images into a "High Dynamic Range" output image.
/// \par
/// This function will combine two input images into a HDR image by picking the unsaturated pixels from the 
/// brightest image and wherever the brightest image is saturated it will instead use the pixel values from the 
/// darkest of the two input images to create a replacement value for the saturated value. 
///
/// \note The input images needs to be Mono8, Mono10 or Mono12 pixel format for monochrome images or Bayer8, Bayer10
/// or Bayer12 for color images. It is also required that the two images has got identical pixel format
///
/// \param [in] pBrighterImageInfo    Pointer to J_tIMAGE_INFO structure with information on the brightest image.
/// \param [in] pDarkerImageInfo      Pointer to J_tIMAGE_INFO structure with information on the darkest image.
/// \param [in,out] pOutputBufferInfo Pointer to J_tIMAGE_INFO structure that will receive the 16-bit HDR output image.
/// \param [in] brighterBlackLevel    Black Level to be subtracted from the Brighter Image during calculation.
/// \param [in] darkerBlackLevel      Black Level to be subtracted from the Darker Image during calculation.
/// \param [in] fDarkGain             Gain value to be multiplied with the Darker Image pixel values when the Brighter Image pixels are saturated.
/// \param [in] fDualSlopeGain        Value to be multiplied with fDarkGain to produce "Dual Slope" effect.
/// \param [in] logOutput             Flag that will select if the HDR Output Image will be tranformed using log2() before it is converted into 16-bit unsigned integer values.
/// \retval                           Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FuseHDR(J_tIMAGE_INFO* pBrighterImageInfo, J_tIMAGE_INFO* pDarkerImageInfo, J_tIMAGE_INFO* pOutputBufferInfo, int32_t brighterBlackLevel, int32_t darkerBlackLevel, float fDarkGain, float fDualSlopeGain, uint8_t logOutput);

/// @}
/// \defgroup WRAP_IMAGE_LUT   Lookup-Table functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The Lookup-Table functions makes it possible for the user to process the camera images through user-configurable
/// lookup-tables.

/// \brief Structure containing LUT data.
typedef struct _J_tLUT_TYPE
{
   uint32_t iLUTEntries;        ///< Number of LUT entries allocated. This will depend on the pixel depth!
   uint32_t iPixelDepth;        ///< Pixel depth! This can be 8-bit, 10-bit, 12-bit, 14-bit or 16-bit.
   uint32_t iColors;            ///< Number of colors. This is either 1 (monochrome) or 3 for Bayer and RGB
   void  *pLutR;                ///< Pointer to the actual LUT data for color 1 (Monochrome or Red channel)
   void  *pLutG;                ///< Pointer to the actual LUT data for color 2 (Green channel). This will be NULL for Monochrome LUT
   void  *pLutB;                ///< Pointer to the actual LUT data for color 3 (Blue channel). This will be NULL for Monochrome LUT
} J_tLUT;

//******************************************************************************************************************
/// \brief   Allocate the buffer memory for the LUT based on the pixel format read from the image buffer.
///
/// \par
/// The LUT will be initialized and the LUT memory will be allocated based on the pixel format read from the image.
/// The allocated LUT will need to be freed up using \c J_Image_FreeLUT()
///
/// \param [in] pImageInfo       Pointer to J_tIMAGE_INFO structure with information on the image data.
/// \param [in] pLutInfo         Pointer to J_tLUT structure with information on LUT.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_MallocLUT(J_tIMAGE_INFO* pImageInfo, J_tLUT* pLutInfo);

//******************************************************************************************************************
/// \brief   Free the LUT memory previously allocated using \c J_Image_MallocLUT().
///
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on LUT.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FreeLUT(J_tLUT* pLutInfo);

//******************************************************************************************************************
/// \brief   Convert an existing LUT into a different size and copy the data.
///
/// \param [in] pSourceLutInfo      Pointer to Source tLUT structure with information on LUT.
/// \param [in] pDestinationLutInfo Pointer to Source tLUT structure with information on LUT.
/// \retval                         Status code defined in the VL_ERROR. If the function succeeds, returns VL_ERR_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ConvertLUT(J_tLUT* pSourceLutInfo, J_tLUT* pDestinationLutInfo);

//******************************************************************************************************************
/// \brief   Process the image by sending all pixels through the LUT.
///
/// \par
/// All the pixels values in the image will be replaced with the corresponding value from the LUT.
/// \note The LUT has to be created using \c J_Image_MallocLUT() from an image with the identical pixel format so the
/// LUT has the correct dimension.
///
/// \param [in] pImageInfo     Pointer to J_tIMAGE_INFO structure with the image data.
/// \param [in] pLutInfo       Pointer to J_tLUT structure with the LUT.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_LUT(J_tIMAGE_INFO* pImageInfo, J_tLUT* pLutInfo);

//******************************************************************************************************************
/// \brief   This function creates a user-defined Gamma LUT and fills in the values to a LUT that has previously been
/// allocated using \c J_Image_MallocLUT().
/// \par
/// \image html gamma_correction.png
/// \image latex gamma_correction.eps "Gamma Correction" width=15cm
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() before this function can be called.
///
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on LUT.
/// \param [in] ColorIndex     Select the LUT color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] gamma          Gamma value to be used for the creation of the LUT entries.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_CreateGammaLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, double gamma);

//******************************************************************************************************************
/// \brief   This function applies a user-defined Gamma LUT to a LUT that has already been initialized with values.
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() before this function can be called.
///
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] ColorIndex     Select the LUT color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] gamma          Gamma value to be used for the creation of the LUT entries.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ApplyGammaToLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, double gamma);

//******************************************************************************************************************
/// \brief   This function initializes a LUT with linear values between an user defined input and output range. Any
/// existing LUT values will be overwritten.
/// \par
/// The output values for input values between X1 and X2 will be linear between Y1 and Y2.
/// \par
/// The output values for input values between 0 and iMix will be min(Y1,Y2) and the output values for inputs between
/// X2 and the maximum input value will be max(Y1,Y2).
/// \par
/// \image html LinearLUT.png
/// \image latex LinearLUT.eps "Linear LUT examples" width=15cm
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() before this function can be called.
///
/// \param [in] pLutInfo     Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] ColorIndex   Select the LUT color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] X1           Minimum input value to be used for the creation of the LUT entries.
/// \param [in] X2           Maximum input value to be used for the creation of the LUT entries.
/// \param [in] Y1           Minimum output value to be used for the creation of the LUT entries.
/// \param [in] Y2           Maximum output value to be used for the creation of the LUT entries.
/// \retval                  Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This pseudo code illustrates how the LUT values are created:
///
/// slope = (Y2 - Y1)/(X2 - X1);
/// offset = Y1 - (slope * X1);
///
/// for (val=0; val<numLUTEntries; val++)
/// {
///     if ((val>=0) && (val<X1))
///     {
///        LUT[val] = min(Y1,Y2);
///     }
///     else if ((val>=X1) && (val<X2))
///     {
///        LUT[val] = offset + slope*val;
///     }
///     else if (val>=X2))
///     {
///        LUT[val] = max(Y1,Y2);
///     }
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_CreateLinearLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t X1, uint32_t X2, uint32_t Y1, uint32_t Y2);

//******************************************************************************************************************
/// \brief   This function initializes a LUT with linear values between two user defined knee-points. The first linear
/// segment will be between (0,0) and Knee1. Second linear segment will bee between Knee1 and Knee2 and the last linear
/// segment will be between Knee2 and (Input Max,Output Max). Any existing LUT values will be overwritten.
/// \par
/// \image html KneeLUT.png
/// \image latex KneeLUT.eps "Linear LUT from 2 knee-points" width=8cm
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() before this function can be called.
///
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] ColorIndex     Select the LUT color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] knee1          First Knee-point to be used for the creation of the LUT entries.
/// \param [in] knee2          Second Knee-point to be used for the creation of the LUT entries.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
/// \code
/// // This pseudo code illustrates how the LUT values are created:
///
/// slope1 = Y2/X2;
///
/// for (val=0; val<Knee1.X; val++)
/// {
///    LUT[val] = offset1 + slope1*val;
/// }
///
/// slope2 = (Knee2.Y - Knee1.Y)/(Knee2.X - Knee1.X);
/// offset2 = Knee1.Y - (slope2 * Knee1.X);
///
/// for (val=Knee1.X; val<Knee2.X; val++)
/// {
///    LUT[val] = offset2 + slope2*val;
/// }
///
/// slope3 = (Output Max - Knee2.Y)/(Input Max - Knee2.X);
/// offset3 = Knee2.Y - (slope3 * Knee2.X);
///
/// for (val=Knee2.X; val<Input Max; val++)
/// {
///    LUT[val] = offset3 + slope3*val;
/// }
/// \endcode
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_CreateKneeLUT(J_tLUT* pLutInfo, uint32_t ColorIndex, POINT knee1, POINT knee2);

//******************************************************************************************************************
/// \brief   This function directly reads a value from the LUT.
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() and initialized before this function can be called.
///
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] ColorIndex     Select the LUT color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] LUTIndex       Zero-based index into the LUT.
/// \param [in] pLUTValue      Pointer to uint32_t variable where the LUT value is to be returned.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetLUTValue(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t LUTIndex, uint32_t* pLUTValue);

//******************************************************************************************************************
/// \brief   This function directly writes a value to the LUT.
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() and initialized before this function can be called.
///
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] ColorIndex     Select the LUT color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] LUTIndex       Zero-based index into the LUT.
/// \param [in] LUTValue       New uint32_t value to be entered into the LUT.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SetLUTValue(J_tLUT* pLutInfo, uint32_t ColorIndex, uint32_t LUTIndex, uint32_t LUTValue);

#ifdef UNICODE
#define J_Image_SaveLUT  J_Image_SaveLUTW
#else
#define J_Image_SaveLUT  J_Image_SaveLUTA
#endif // !UNICODE

//******************************************************************************************************************
/// \brief   This function saves a LUT to disk as a binary file
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() and initialized before this function can be called.
///
/// \note
/// In order to use the same code in both 8-bit (ANSI) string applications and Unicode string applications please use the
/// \c J_Image_SaveLUT definition. This will automatically select the appropriate version of the function.
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] pPath          Filename and path to where the LUT will be written. The file Name is a 8-bit (ANSI) character string.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SaveLUTA(J_tLUT* pLutInfo, LPCSTR pPath);

//******************************************************************************************************************
/// \brief   This function saves a LUT to disk as a binary file
///
/// \note The LUT has to be created using \c J_Image_MallocLUT() and initialized before this function can be called.
///
/// \note
/// In order to use the same code in both 8-bit (ANSI) string applications and Unicode string applications please use the
/// \c J_Image_SaveLUT definition. This will automatically select the appropriate version of the function.
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on existing LUT.
/// \param [in] pPath          Filename and path to where the LUT will be written. The file Name is an Unicode character string.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_SaveLUTW(J_tLUT* pLutInfo, LPCWSTR pPath);

#ifdef UNICODE
#define J_Image_LoadLUT  J_Image_LoadLUTW
#else
#define J_Image_LoadLUT  J_Image_LoadLUTA
#endif // !UNICODE

//******************************************************************************************************************
/// \brief   This function loads a LUT from disk. If the LUT already contains data then it will be overwritten.
///
/// \note This function will automatically free any memory allocated for the existing LUT and allocate new memory
/// that will be large enough to hold the LUT that is read from disk.
///
/// \note
/// In order to use the same code in both 8-bit (ANSI) string applications and Unicode string applications please use the
/// \c J_Image_LoadLUT definition. This will automatically select the appropriate version of the function.
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on LUT.
/// \param [in] pPath          Filename and path to the LUT file to be read. The filename is a 8-bit (ANSI) character
///                            string.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_LoadLUTA(J_tLUT* pLutInfo, LPCSTR pPath);

//******************************************************************************************************************
/// \brief   This function loads a LUT from disk. If the LUT already contains data then it will be overwritten.
///
/// \note This function will automatically free any memory allocated for the existing LUT and allocate new memory
/// that will be large enough to hold the LUT that is read from disk.
///
/// \note
/// In order to use the same code in both 8-bit (ANSI) string applications and Unicode string applications please use the
/// \c J_Image_LoadLUT definition. This will automatically select the appropriate version of the function.
/// \param [in] pLutInfo       Pointer to J_tLUT structure with information on LUT.
/// \param [in] pPath          Filename and path to the LUT file to be read. The filename is an Unicode character string.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_LoadLUTW(J_tLUT* pLutInfo, LPCWSTR pPath);

/// @}
/// \defgroup WRAP_IMAGE_HIST   Histogram functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The Histogram functions makes it possible to create a Histogram from an image.

/// \brief Structure containing Histogram data.
typedef struct _J_tHIST_TYPE
{
   uint32_t iHistEntries;       ///< Number of Histogram entries allocated. This will depend on the pixel depth!
   uint32_t iPixelDepth;        ///< Pixel depth! This can be 8-bit, 10-bit, 12-bit, 14-bit or 16-bit.
   uint32_t iColors;            ///< Number of colors. This is either 1 (monochrome) or 3 for Bayer and RGB
   void  *pHistR;               ///< Pointer to the actual Histogram data for color 1 (Monochrome or Red channel)
   void  *pHistG;               ///< Pointer to the actual Histogram data for color 2 (Green channel). This will be NULL for Monochrome LUT
   void  *pHistB;               ///< Pointer to the actual Histogram data for color 3 (Blue channel). This will be NULL for Monochrome LUT
} J_tHIST;

//******************************************************************************************************************
/// \brief   Allocate the buffer memory for the Histogram based on the pixel format read from the image buffer.
///
/// \par
/// The Histogram will be initialized and the memory will be allocated based on the pixel format read from the image.
/// The allocated Histogram will need to be freed up using \c J_Image_FreeHistogram()
///
/// \param [in] pImageInfo       Pointer to J_tIMAGE_INFO structure with information on the image data.
/// \param [in] pHistogramInfo   Pointer to J_tHIST structure with information on Histogram.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_MallocHistogram(J_tIMAGE_INFO* pImageInfo, J_tHIST* pHistogramInfo);

//******************************************************************************************************************
/// \brief   Free the Histogram memory previously allocated using \c J_Image_MallocHistogram().
///
/// \param [in] pHistogramInfo Pointer to J_tHIST structure with information on Histogram.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_FreeHistogram(J_tHIST* pHistogramInfo);

//******************************************************************************************************************
/// \brief   Clear the contents of the Histogram memory previously allocated using \c J_Image_MallocHistogram().
///
/// \param [in] pHistogramInfo Pointer to J_tHIST structure with information on Histogram.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_ClearHistogram(J_tHIST* pHistogramInfo);

//******************************************************************************************************************
/// \brief   Creates the Histogram from the image contents.
///
/// \par
/// The Histogram will be has to be allocated in advance using \c J_Image_MallocHistogram()
///
/// \param [in] pImageInfo       Pointer to J_tIMAGE_INFO structure with information on the image data.
/// \param [in] pMeasureRect     Measurement rectangular coordinates.
/// \param [in] pHistogramInfo   Pointer to J_tHIST structure with information on Histogram.
/// \retval                      Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_CreateHistogram(J_tIMAGE_INFO* pImageInfo, RECT *pMeasureRect, J_tHIST* pHistogramInfo);

//******************************************************************************************************************
/// \brief   This function directly reads a value from the Histogram.
///
/// \note The Histogram has to be created using \c J_Image_MallocHistogram() and initialized using \c J_Image_CreateHistogram()
/// before this function can be called.
///
/// \param [in] pHistogramInfo  Pointer to J_tHIST structure with information on existing Histogram.
/// \param [in] ColorIndex      Select the Histogram color index to use. 0=Monochrome or Red, 1=Green, 2=Blue
/// \param [in] HistogramIndex  Zero-based index into the Histogram.
/// \param [in] pHistogramValue Pointer to uint32_t variable where the Histogram value is to be returned.
/// \retval                     Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_GetHistogramValue(J_tHIST* pHistogramInfo, uint32_t ColorIndex, uint32_t HistogramIndex, uint32_t* pHistogramValue);


/// @}
/// \defgroup WRAP_IMAGE_TRANSFORM   Transformation functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The Transformation functions makes it possible for the user to flip and rotate images.

/// \brief Flip type used in \c J_Image_Flip()
typedef enum _J_IMAGE_FLIP_TYPE
{
   J_FLIP_HORIZONTAL = 0,         //!< Flip Horizontally (Mirror)
   J_FLIP_VERTICAL = 1,           //!< Flip Vertically
} J_IMAGE_FLIP_TYPE;

/// \brief Rotate type used in \c J_Image_Rotate()
typedef enum _J_IMAGE_ROTATE_TYPE
{
   J_ROTATE_90_DEG_CW = 0,        //!< Rotate 90 degree clockwise
   J_ROTATE_90_DEG_CCW = 1,       //!< Rotate 90 degree counter-clockwise
   J_ROTATE_180_DEG = 2,          //!< Rotate 180 degree (upside-down)
} J_IMAGE_ROTATE_TYPE;

//******************************************************************************************************************
/// \brief   Flip the image contents in the image buffer.
///
/// \note The input images needs to be 8, 10, 12, 14 or 16-bit Monochrome or Bayer-Color pixel format.
/// \param [in] pImageInfo     Pointer to J_tIMAGE_INFO structure with information on the image data.
/// \param [in] flipType       Type of flip operation. \c J_FLIP_HORIZONTAL will Mirror the image and \c J_FLIP_VERTICAL will
///                            flip the image upside-down.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_Flip(J_tIMAGE_INFO* pImageInfo, J_IMAGE_FLIP_TYPE flipType);

//******************************************************************************************************************
/// \brief   Rotate the image contents in the image buffer.
///
/// \note The input images needs to be 8, 10, 12, 14 or 16-bit Monochrome or Bayer-Color unpacked pixel format.
///
/// \param [in] pImageInfo     Pointer to J_tIMAGE_INFO structure with information on the image data.
/// \param [in] rotateType     Type of rotate operation. \c J_ROTATE_90_DEG_CW will rotate the image 90 degrees clockwise,
///                            \c J_ROTATE_90_DEG_CCW will rotate the image 90 degrees counter-clockwise and
///                            \c J_ROTATE_180_DEG will rotate the image 180 degrees similar to if the camera was mounted
///                            upside-down.
/// \retval                    Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_Rotate(J_tIMAGE_INFO* pImageInfo, J_IMAGE_ROTATE_TYPE rotateType);

/// @}
/// \defgroup WRAP_IMAGE_PROC   Image Processing functions
/// \ingroup WRAP_IMAGE
/// @{
/// \brief The Image Processing functions makes it possible for the user to perform various tasks on images.

/// \brief Image processing function types used in \c J_Image_Processing()
typedef enum _J_IMAGE_PROCESS_TYPE
{
	RED_COMPENSATION = 0,			//!< Red compensation
	RED_COMPENSATION_MULTI,			//!< Red compensation multi-processor
	GREEN_COMPENSATION,			//!< Green compensation
	GREEN_COMPENSATION_MULTI,		//!< Green compensation multi-processor
	COLOR_NOISE_REDUCTION,			//!< Color noise reduction
	COLOR_NOISE_REDUCTION_MULTI,            //!< Color noise reduction multi-processor
	CONVOLUTION,				//!< Convolution
	CONVOLUTION_MULTI,			//!< Convolution multi-processor
	PSEUDO_COLOR,				//!< Pseudo color
	PSEUDO_COLOR_MULTI,			//!< Pseudo color multi-processor
	LENS_DISTORTION,			//!< Lens distortion
	LENS_DISTORTION_MULTI,			//!< Lens distortion multi-processor
} J_IMAGE_PROCESSING;

/// \brief Structure containing the color compensation parameters for each ROI. This is referenced by \c J_tCOLOR_COMPEN
typedef struct _J_tCOLOR_COMPEN_PARAMS
{
	RECT		RectOfROI;		///< Rectangle of a region of interest.
	uint32_t	BThreshold;		///< Threshold of blue channel.
	uint32_t	GThreshold;		///< Threshold of green channel.
	uint32_t	RThreshold;		///< Threshold of red channel.
	uint32_t	BGain;			///< Blue gain for compensation. 4096:1.00
	uint32_t	GGain;			///< Green gain for compensation. 4096:1.00
	uint32_t	RGain;			///< Red gain for compensation. 4096:1.00
} J_tCOLOR_COMPEN_PARAMS;

/// \brief Struct containing all the parameters for the color compensations.
typedef struct _J_tCOLOR_COMPEN
{
	uint32_t	NumOfROI;		///< Number of ROI for color compensation [1..10].
	J_tCOLOR_COMPEN_PARAMS CompenParams[10];	///< Pointer to structure of parameters for each ROI.
} J_tCOLOR_COMPEN;

//******************************************************************************************************************
/// \brief  This function processes the image by the function specified by the \c iProcessFunction parameter.
///
/// \param [in] pImageInfo            Pointer to J_tIMAGE_INFO structure with information on the image data.
/// \param [in] iProcessFunction      Type of image processing.
/// \param [in] pParameters           Pointer to parameters.
/// \retval                           Status code defined in the J_STATUS_CODES. If the function succeeds, returns J_ST_SUCCESS.
///
EXTERN_C GCFC_IMPORT_EXPORT J_STATUS_TYPE J_Image_Processing(J_tIMAGE_INFO* pImageInfo, J_IMAGE_PROCESSING iProcessFunction, void * pParameters);

/// @}
#endif
