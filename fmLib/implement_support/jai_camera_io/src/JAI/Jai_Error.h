////////////////////////////////////////////////////////////////////////////////////////////
/// \file		Jai_Error.h
/// \brief		JAI SDK
/// \version	Revison: 1.2.5
/// \author		mat, kic (JAI)
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _FACTORY_ERROR_H
#define _FACTORY_ERROR_H

  /// Factroy status type
  typedef int J_STATUS_TYPE;
  
  /// \brief Error return code enumeration. This is returned by all \c Jai_Factory.dll functions
  typedef enum  
  {
    J_ST_SUCCESS             = 0,   ///< OK      
    J_ST_INVALID_BUFFER_SIZE = -1,  ///< Invalid buffer size
    J_ST_INVALID_HANDLE      = -2,  ///< Invalid handle
    J_ST_INVALID_ID          = -3,  ///< Invalid ID
    J_ST_ACCESS_DENIED       = -4,  ///< Access denied
    J_ST_NO_DATA             = -5,  ///< No data
    J_ST_ERROR               = -6,  ///< Generic errorcode
    J_ST_INVALID_PARAMETER   = -7,  ///< Invalid parameter
    J_ST_TIMEOUT             = -8,  ///< Timeout
    J_ST_INVALID_FILENAME    = -9,  ///< Invalid file name
    J_ST_INVALID_ADDRESS     = -10, ///< Invalid address
    J_ST_FILE_IO             = -11, ///< File IO error
    J_ST_GC_ERROR            = -12, ///< GenICam error. Use \c J_Factory_GetGenICamErrorInfo() to get detailed information
    J_ST_VALIDATION_ERROR    = -13, ///< Settings File Validation Error. Use \c J_Camera_GetValidationErrorInfo() to get detailed information
    J_ST_VALIDATION_WARNING  = -14  ///< Settings File Validation Warning. Use \c J_Camera_GetValidationErrorInfo() to get detailed information
  } J_STATUS_CODES;


#endif //_FACTORY_ERROR_H
