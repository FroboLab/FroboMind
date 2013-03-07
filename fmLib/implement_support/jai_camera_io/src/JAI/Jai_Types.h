/****************************************************************************
 (c) 2004 by Basler Vision Technologies
 (c) 2005 STEMMER IMAGING

 //  License: This file is published under the license of the EMVA GenICam  Standard Group. 
//  A text file describing the legal terms is included in  your installation as 'GenICam_license.pdf'. 
//  If for some reason you are missing  this file please contact the EMVA or visit the website
//  (http://www.genicam.org) for a full copy.
// 
//  THIS SOFTWARE IS PROVIDED BY THE EMVA GENICAM STANDARD GROUP "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,  
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR  
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE EMVA GENICAM STANDARD  GROUP 
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  SPECIAL, 
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  LIMITED TO, 
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA, OR PROFITS; 
//  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  THEORY OF LIABILITY, 
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) 
//  ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
//  POSSIBILITY OF SUCH DAMAGE.


****************************************************************************/

 /// \file
 /// \brief    Platform independend type definitions
 /// \version  $Revision: 1.12 $
 /// \date     $Date: 2006/07/18 13:31:52 $
 /// \ingroup Base_PublicUtilities


#ifndef GENICAM_TYPES_H
#define GENICAM_TYPES_H

#if ! defined(_MSC_VER)

  #include <stdint.h>

  //#define interface struct */Ruins gphoto2, so not included*/

#else // if defined(_MSC_VER)

  //! \addtogroup Base_PublicUtilities
  //! \{

  //! makes int 8 portable across different platforms
  typedef char int8_t;
  //! makes char 8 portable across different platforms
  typedef char char8_t;
  //! makes unsigned char portable across different platforms
  typedef unsigned char uint8_t;
  //! makes char 8 portable across different platforms
  typedef unsigned char uchar8_t;

  //! makes short portable across different platforms
  typedef short int16_t;
  //! makes unsigned short portable across different platforms
  typedef unsigned short uint16_t;

  //! makes __int32 portable across different platforms
  typedef __int32 int32_t;
  //! makes unsigned int32 portable across different platforms
  typedef unsigned __int32 uint32_t;

  //! makes int64 portable across different platforms
  typedef __int64 int64_t;
  //! makes unsigned int64 portable across different platforms
  typedef unsigned __int64 uint64_t;

  //! use a universal platform dependend int
  typedef __int64 int_t;
  //! use a universal platform dependend unsigned int
  typedef unsigned __int64 uint_t;

  #ifndef _SIZE_T_DEFINED
    #ifdef  _WIN64
      //! use a universal platform dependend unsigned int
      typedef unsigned __int64 size_t;
    #else
      //! use a universal platform dependend unsigned int
      typedef unsigned int size_t;
    #endif
    #define _SIZE_T_DEFINED
  #endif

  #ifndef _INTPTR_T_DEFINED
    #ifdef  _WIN64
      //! use a universal platform dependend int
      typedef __int64 intptr_t;
    #else
      //! use a universal platform dependend int
      typedef int intptr_t;
    #endif
    #define _INTPTR_T_DEFINED
  #endif


  // Utility Types
  //! 8 bit bool
  typedef bool              bool8_t;
  //! universal pointer to 8 bit const char
  typedef const char *      pcchar8_t;
  //! universal pointer to 8 bit char
  typedef char *            pchar8_t;
  //! 32 bit floatingpoint
  typedef float             float32_t;
  //! 64 bit floatingpoint
  typedef double            float64_t;

#endif

#define INT64_MAX     0x7fffffffffffffffLL  /*maximum signed __int64 value */
#define INT64_MIN     0x8000000000000000LL  /*minimum signed __int64 value */
#define UINT64_MAX    0xffffffffffffffffULL  /*maximum unsigned __int64 value */

#define INT32_MAX     0x000000007fffffffLL  /*maximum signed __int32 value */
#define INT32_MIN     0xffffffff80000000LL  /*minimum signed __int32 value */
#define UINT32_MAX    0x00000000ffffffffULL  /*maximum unsigned __int32 value */

#define INT8_MAX     0x000000000000007fLL  /*maximum signed __int8 value */
#define INT8_MIN     0xffffffffffffff80LL  /*minimum signed __int8 value */
#define UINT8_MAX    0x00000000000000ffULL  /*maximum unsigned __int8 value */
  //! \}

// gc base import/export
#ifdef GCBASE_EXPORTS
  #define GCBASE_API __declspec( dllexport )
#else
  #define GCBASE_API __declspec( dllimport )
#endif

//! version
typedef struct
{
    uint16_t Major;        //!> a is incompatible with b if a != b
    uint16_t Minor;        //!> a is incompatible b a > b
    uint16_t SubMinor;    //!> a is aways compatible with b
} Version_t;

/// \brief   Image information structure used in callback and image conversion functions
typedef struct _J_tIMAGE_INFO_TYPE 
{
   uint32_t iPixelType;          ///< Pixel Format Type
   uint32_t iSizeX;              ///< Image width
   uint32_t iSizeY;              ///< Image height
   uint32_t iImageSize;          ///< Number of bytes for image
   uint8_t  *pImageBuffer;       ///< Buffer pointer
   uint64_t iTimeStamp;          ///< Timestamp
   uint32_t iMissingPackets;     ///< Number of missing packets
   uint32_t iAnnouncedBuffers;   ///< Number of announced buffers
   uint32_t iQueuedBuffers;      ///< Number of queued buffers
   uint32_t iOffsetX;            ///< Image offset x
   uint32_t iOffsetY;            ///< Image offset y
   uint32_t iAwaitDelivery;      ///< Number of frames awaiting delivery inside the driver
   uint32_t iBlockId;            ///< GigE Vision Stream Protocol Block-ID
} J_tIMAGE_INFO;

typedef void* FACTORY_HANDLE;       ///< Factory object handle type, obtained by \c J_Factory_Open()
typedef void* CAM_HANDLE;           ///< Camera object handle type, obtained by \c J_Camera_Open() or \c J_Camera_OpenMc()
typedef void* STREAM_HANDLE;        ///< Data Stream object handle type, obtained by \c J_Camera_CreateDataStream()
typedef void* EVT_HANDLE;           ///< Event handle type, obtained by \c J_Event_Register()
typedef void* BUF_HANDLE;           ///< Buffer object handle type, obtained by \c J_DataStream_AnnounceBuffer()
typedef void* NODE_HANDLE;          ///< Node object handle type, obtained by \c J_Camera_GetNode****
typedef void* VIEW_HANDLE;          ///< View object handle type, obtained by \c J_Image_OpenViewWindow() or \c J_Image_OpenViewWindowEx()
typedef void* THRD_HANDLE;          ///< Thread object handle type, obtained by \c J_Image_OpenStream() or \c J_Image_OpenStreamLight()

#endif // GENICAM_TYPES_H
