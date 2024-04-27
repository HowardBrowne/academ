#include "EdsStrings.h"

namespace Eds {
	
	std::string getErrorString(EdsError error) {
		switch(error) {
				/*-----------------------------------------------------------------------
				 ED-SDK Error Code Masks
				 ------------------------------------------------------------------------*/
			case EDS_ISSPECIFIC_MASK: return "EDS_ISSPECIFIC_MASK";
			case EDS_COMPONENTID_MASK: return "EDS_COMPONENTID_MASK";
			case EDS_RESERVED_MASK: return "EDS_RESERVED_MASK";
			case EDS_ERRORID_MASK: return "EDS_ERRORID_MASK";
				
				/*-----------------------------------------------------------------------
				 ED-SDK Base Component IDs
				 ------------------------------------------------------------------------*/
			case EDS_CMP_ID_CLIENT_COMPONENTID: return "EDS_CMP_ID_CLIENT_COMPONENTID";
			case EDS_CMP_ID_LLSDK_COMPONENTID: return "EDS_CMP_ID_LLSDK_COMPONENTID";
			case EDS_CMP_ID_HLSDK_COMPONENTID: return "EDS_CMP_ID_HLSDK_COMPONENTID";
				
				/*-----------------------------------------------------------------------
				 ED-SDK Functin Success Code
				 ------------------------------------------------------------------------*/
			case EDS_ERR_OK: return "EDS_ERR_OK";
				
				/*-----------------------------------------------------------------------
				 ED-SDK Generic Error IDs
				 ------------------------------------------------------------------------*/
				/* Miscellaneous errors */
			case EDS_ERR_UNIMPLEMENTED: return "EDS_ERR_UNIMPLEMENTED";
			case EDS_ERR_INTERNAL_ERROR: return "EDS_ERR_INTERNAL_ERROR";
			case EDS_ERR_MEM_ALLOC_FAILED: return "EDS_ERR_MEM_ALLOC_FAILED";
			case EDS_ERR_MEM_FREE_FAILED: return "EDS_ERR_MEM_FREE_FAILED";
			case EDS_ERR_OPERATION_CANCELLED: return "EDS_ERR_OPERATION_CANCELLED";
			case EDS_ERR_INCOMPATIBLE_VERSION: return "EDS_ERR_INCOMPATIBLE_VERSION";
			case EDS_ERR_NOT_SUPPORTED: return "EDS_ERR_NOT_SUPPORTED";
			case EDS_ERR_UNEXPECTED_EXCEPTION: return "EDS_ERR_UNEXPECTED_EXCEPTION";
			case EDS_ERR_PROTECTION_VIOLATION: return "EDS_ERR_PROTECTION_VIOLATION";
			case EDS_ERR_MISSING_SUBCOMPONENT: return "EDS_ERR_MISSING_SUBCOMPONENT";
			case EDS_ERR_SELECTION_UNAVAILABLE: return "EDS_ERR_SELECTION_UNAVAILABLE";
				
				/* File errors */
			case EDS_ERR_FILE_IO_ERROR: return "EDS_ERR_FILE_IO_ERROR";
			case EDS_ERR_FILE_TOO_MANY_OPEN: return "EDS_ERR_FILE_TOO_MANY_OPEN";
			case EDS_ERR_FILE_NOT_FOUND: return "EDS_ERR_FILE_NOT_FOUND";
			case EDS_ERR_FILE_OPEN_ERROR: return "EDS_ERR_FILE_OPEN_ERROR";
			case EDS_ERR_FILE_CLOSE_ERROR: return "EDS_ERR_FILE_CLOSE_ERROR";
			case EDS_ERR_FILE_SEEK_ERROR: return "EDS_ERR_FILE_SEEK_ERROR";
			case EDS_ERR_FILE_TELL_ERROR: return "EDS_ERR_FILE_TELL_ERROR";
			case EDS_ERR_FILE_READ_ERROR: return "EDS_ERR_FILE_READ_ERROR";
			case EDS_ERR_FILE_WRITE_ERROR: return "EDS_ERR_FILE_WRITE_ERROR";
			case EDS_ERR_FILE_PERMISSION_ERROR: return "EDS_ERR_FILE_PERMISSION_ERROR";
			case EDS_ERR_FILE_DISK_FULL_ERROR: return "EDS_ERR_FILE_DISK_FULL_ERROR";
			case EDS_ERR_FILE_ALREADY_EXISTS: return "EDS_ERR_FILE_ALREADY_EXISTS";
			case EDS_ERR_FILE_FORMAT_UNRECOGNIZED: return "EDS_ERR_FILE_FORMAT_UNRECOGNIZED";
			case EDS_ERR_FILE_DATA_CORRUPT: return "EDS_ERR_FILE_DATA_CORRUPT";
			case EDS_ERR_FILE_NAMING_NA: return "EDS_ERR_FILE_NAMING_NA";
				
				/* Directory errors */          
			case EDS_ERR_DIR_NOT_FOUND: return "EDS_ERR_DIR_NOT_FOUND";
			case EDS_ERR_DIR_IO_ERROR: return "EDS_ERR_DIR_IO_ERROR";
			case EDS_ERR_DIR_ENTRY_NOT_FOUND: return "EDS_ERR_DIR_ENTRY_NOT_FOUND";
			case EDS_ERR_DIR_ENTRY_EXISTS: return "EDS_ERR_DIR_ENTRY_EXISTS";
			case EDS_ERR_DIR_NOT_EMPTY: return "EDS_ERR_DIR_NOT_EMPTY";
				
				/* Property errors */
			case EDS_ERR_PROPERTIES_UNAVAILABLE: return "EDS_ERR_PROPERTIES_UNAVAILABLE";
			case EDS_ERR_PROPERTIES_MISMATCH: return "EDS_ERR_PROPERTIES_MISMATCH";
			case EDS_ERR_PROPERTIES_NOT_LOADED: return "EDS_ERR_PROPERTIES_NOT_LOADED";
				
				/* Function Parameter errors */     
			case EDS_ERR_INVALID_PARAMETER: return "EDS_ERR_INVALID_PARAMETER";
			case EDS_ERR_INVALID_HANDLE: return "EDS_ERR_INVALID_HANDLE";
			case EDS_ERR_INVALID_POINTER: return "EDS_ERR_INVALID_POINTER";
			case EDS_ERR_INVALID_INDEX: return "EDS_ERR_INVALID_INDEX";
			case EDS_ERR_INVALID_LENGTH: return "EDS_ERR_INVALID_LENGTH";
			case EDS_ERR_INVALID_FN_POINTER: return "EDS_ERR_INVALID_FN_POINTER";
			case EDS_ERR_INVALID_SORT_FN: return "EDS_ERR_INVALID_SORT_FN";
				
				/* Device errors */
			case EDS_ERR_DEVICE_NOT_FOUND: return "EDS_ERR_DEVICE_NOT_FOUND";
			case EDS_ERR_DEVICE_BUSY: return "EDS_ERR_DEVICE_BUSY";
			case EDS_ERR_DEVICE_INVALID: return "EDS_ERR_DEVICE_INVALID";
			case EDS_ERR_DEVICE_EMERGENCY: return "EDS_ERR_DEVICE_EMERGENCY";
			case EDS_ERR_DEVICE_MEMORY_FULL: return "EDS_ERR_DEVICE_MEMORY_FULL";
			case EDS_ERR_DEVICE_INTERNAL_ERROR: return "EDS_ERR_DEVICE_INTERNAL_ERROR";
			case EDS_ERR_DEVICE_INVALID_PARAMETER: return "EDS_ERR_DEVICE_INVALID_PARAMETER";
			case EDS_ERR_DEVICE_NO_DISK: return "EDS_ERR_DEVICE_NO_DISK";
			case EDS_ERR_DEVICE_DISK_ERROR: return "EDS_ERR_DEVICE_DISK_ERROR";
			case EDS_ERR_DEVICE_CF_GATE_CHANGED: return "EDS_ERR_DEVICE_CF_GATE_CHANGED";
			case EDS_ERR_DEVICE_DIAL_CHANGED: return "EDS_ERR_DEVICE_DIAL_CHANGED";
			case EDS_ERR_DEVICE_NOT_INSTALLED: return "EDS_ERR_DEVICE_NOT_INSTALLED";
			case EDS_ERR_DEVICE_STAY_AWAKE: return "EDS_ERR_DEVICE_STAY_AWAKE";
			case EDS_ERR_DEVICE_NOT_RELEASED: return "EDS_ERR_DEVICE_NOT_RELEASED";
				
				
				/* Stream errors */
			case EDS_ERR_STREAM_IO_ERROR: return "EDS_ERR_STREAM_IO_ERROR";
			case EDS_ERR_STREAM_NOT_OPEN: return "EDS_ERR_STREAM_NOT_OPEN";
			case EDS_ERR_STREAM_ALREADY_OPEN: return "EDS_ERR_STREAM_ALREADY_OPEN";
			case EDS_ERR_STREAM_OPEN_ERROR: return "EDS_ERR_STREAM_OPEN_ERROR";
			case EDS_ERR_STREAM_CLOSE_ERROR: return "EDS_ERR_STREAM_CLOSE_ERROR";
			case EDS_ERR_STREAM_SEEK_ERROR: return "EDS_ERR_STREAM_SEEK_ERROR";
			case EDS_ERR_STREAM_TELL_ERROR: return "EDS_ERR_STREAM_TELL_ERROR";
			case EDS_ERR_STREAM_READ_ERROR: return "EDS_ERR_STREAM_READ_ERROR";
			case EDS_ERR_STREAM_WRITE_ERROR: return "EDS_ERR_STREAM_WRITE_ERROR";
			case EDS_ERR_STREAM_PERMISSION_ERROR: return "EDS_ERR_STREAM_PERMISSION_ERROR";
			case EDS_ERR_STREAM_COULDNT_BEGIN_THREAD: return "EDS_ERR_STREAM_COULDNT_BEGIN_THREAD";
			case EDS_ERR_STREAM_BAD_OPTIONS: return "EDS_ERR_STREAM_BAD_OPTIONS";
			case EDS_ERR_STREAM_END_OF_STREAM: return "EDS_ERR_STREAM_END_OF_STREAM";
				
				/* Communications errors */
			case EDS_ERR_COMM_PORT_IS_IN_USE: return "EDS_ERR_COMM_PORT_IS_IN_USE";
			case EDS_ERR_COMM_DISCONNECTED: return "EDS_ERR_COMM_DISCONNECTED";
			case EDS_ERR_COMM_DEVICE_INCOMPATIBLE: return "EDS_ERR_COMM_DEVICE_INCOMPATIBLE";
			case EDS_ERR_COMM_BUFFER_FULL: return "EDS_ERR_COMM_BUFFER_FULL";
			case EDS_ERR_COMM_USB_BUS_ERR: return "EDS_ERR_COMM_USB_BUS_ERR";
				
				/* Lock/Unlock */
			case EDS_ERR_USB_DEVICE_LOCK_ERROR: return "EDS_ERR_USB_DEVICE_LOCK_ERROR";
			case EDS_ERR_USB_DEVICE_UNLOCK_ERROR: return "EDS_ERR_USB_DEVICE_UNLOCK_ERROR";
				
				/* STI/WIA */
			case EDS_ERR_STI_UNKNOWN_ERROR: return "EDS_ERR_STI_UNKNOWN_ERROR";
			case EDS_ERR_STI_INTERNAL_ERROR: return "EDS_ERR_STI_INTERNAL_ERROR";
			case EDS_ERR_STI_DEVICE_CREATE_ERROR: return "EDS_ERR_STI_DEVICE_CREATE_ERROR";
			case EDS_ERR_STI_DEVICE_RELEASE_ERROR: return "EDS_ERR_STI_DEVICE_RELEASE_ERROR";
			case EDS_ERR_DEVICE_NOT_LAUNCHED: return "EDS_ERR_DEVICE_NOT_LAUNCHED";
				
			case EDS_ERR_ENUM_NA: return "EDS_ERR_ENUM_NA";
			case EDS_ERR_INVALID_FN_CALL: return "EDS_ERR_INVALID_FN_CALL";
			case EDS_ERR_HANDLE_NOT_FOUND: return "EDS_ERR_HANDLE_NOT_FOUND";
			case EDS_ERR_INVALID_ID: return "EDS_ERR_INVALID_ID";
			case EDS_ERR_WAIT_TIMEOUT_ERROR: return "EDS_ERR_WAIT_TIMEOUT_ERROR";
				
				/* PTP */
			case EDS_ERR_SESSION_NOT_OPEN: return "EDS_ERR_SESSION_NOT_OPEN";
			case EDS_ERR_INVALID_TRANSACTIONID: return "EDS_ERR_INVALID_TRANSACTIONID";
			case EDS_ERR_INCOMPLETE_TRANSFER: return "EDS_ERR_INCOMPLETE_TRANSFER";
			case EDS_ERR_INVALID_STRAGEID: return "EDS_ERR_INVALID_STRAGEID";
			case EDS_ERR_DEVICEPROP_NOT_SUPPORTED: return "EDS_ERR_DEVICEPROP_NOT_SUPPORTED";
			case EDS_ERR_INVALID_OBJECTFORMATCODE: return "EDS_ERR_INVALID_OBJECTFORMATCODE";
			case EDS_ERR_SELF_TEST_FAILED: return "EDS_ERR_SELF_TEST_FAILED";
			case EDS_ERR_PARTIAL_DELETION: return "EDS_ERR_PARTIAL_DELETION";
			case EDS_ERR_SPECIFICATION_BY_FORMAT_UNSUPPORTED: return "EDS_ERR_SPECIFICATION_BY_FORMAT_UNSUPPORTED";
			case EDS_ERR_NO_VALID_OBJECTINFO: return "EDS_ERR_NO_VALID_OBJECTINFO";
			case EDS_ERR_INVALID_CODE_FORMAT: return "EDS_ERR_INVALID_CODE_FORMAT";
			case EDS_ERR_UNKNOWN_VENDOR_CODE: return "EDS_ERR_UNKNOWN_VENDOR_CODE";
			case EDS_ERR_CAPTURE_ALREADY_TERMINATED: return "EDS_ERR_CAPTURE_ALREADY_TERMINATED";
			case EDS_ERR_INVALID_PARENTOBJECT: return "EDS_ERR_INVALID_PARENTOBJECT";
			case EDS_ERR_INVALID_DEVICEPROP_FORMAT: return "EDS_ERR_INVALID_DEVICEPROP_FORMAT";
			case EDS_ERR_INVALID_DEVICEPROP_VALUE: return "EDS_ERR_INVALID_DEVICEPROP_VALUE";
			case EDS_ERR_SESSION_ALREADY_OPEN: return "EDS_ERR_SESSION_ALREADY_OPEN";
			case EDS_ERR_TRANSACTION_CANCELLED: return "EDS_ERR_TRANSACTION_CANCELLED";
			case EDS_ERR_SPECIFICATION_OF_DESTINATION_UNSUPPORTED: return "EDS_ERR_SPECIFICATION_OF_DESTINATION_UNSUPPORTED";
				
				/* PTP Vendor */
			case EDS_ERR_UNKNOWN_COMMAND: return "EDS_ERR_UNKNOWN_COMMAND";
			case EDS_ERR_OPERATION_REFUSED: return "EDS_ERR_OPERATION_REFUSED";
			case EDS_ERR_LENS_COVER_CLOSE: return "EDS_ERR_LENS_COVER_CLOSE";
			case EDS_ERR_LOW_BATTERY: return "EDS_ERR_LOW_BATTERY";
			case EDS_ERR_OBJECT_NOTREADY: return "EDS_ERR_OBJECT_NOTREADY";
				
			case EDS_ERR_TAKE_PICTURE_AF_NG: return "EDS_ERR_TAKE_PICTURE_AF_NG";
			case EDS_ERR_TAKE_PICTURE_RESERVED: return "EDS_ERR_TAKE_PICTURE_RESERVED";
			case EDS_ERR_TAKE_PICTURE_MIRROR_UP_NG: return "EDS_ERR_TAKE_PICTURE_MIRROR_UP_NG";
			case EDS_ERR_TAKE_PICTURE_SENSOR_CLEANING_NG: return "EDS_ERR_TAKE_PICTURE_SENSOR_CLEANING_NG";
			case EDS_ERR_TAKE_PICTURE_SILENCE_NG: return "EDS_ERR_TAKE_PICTURE_SILENCE_NG";
			case EDS_ERR_TAKE_PICTURE_NO_CARD_NG: return "EDS_ERR_TAKE_PICTURE_NO_CARD_NG";
			case EDS_ERR_TAKE_PICTURE_CARD_NG: return "EDS_ERR_TAKE_PICTURE_CARD_NG";
			case EDS_ERR_TAKE_PICTURE_CARD_PROTECT_NG: return "EDS_ERR_TAKE_PICTURE_CARD_PROTECT_NG";
			case EDS_ERR_TAKE_PICTURE_MOVIE_CROP_NG: return "EDS_ERR_TAKE_PICTURE_MOVIE_CROP_NG";
			case EDS_ERR_TAKE_PICTURE_STROBO_CHARGE_NG: return "EDS_ERR_TAKE_PICTURE_STROBO_CHARGE_NG";
				
			case EDS_ERR_LAST_GENERIC_ERROR_PLUS_ONE: return "EDS_ERR_LAST_GENERIC_ERROR_PLUS_ONE";
			default: return "[unrecognized EdsError]";
		}
	}

	std::string getPropertyIDString(EdsPropertyID property) {
		switch(property) {
				/*----------------------------------
				 Camera Setting Properties
				 ----------------------------------*/
			case kEdsPropID_Unknown: return "kEdsPropID_Unknown";
				
			case kEdsPropID_ProductName: return "kEdsPropID_ProductName";
			case kEdsPropID_OwnerName: return "kEdsPropID_OwnerName";
			case kEdsPropID_MakerName: return "kEdsPropID_MakerName";
			case kEdsPropID_DateTime: return "kEdsPropID_DateTime";
			case kEdsPropID_FirmwareVersion: return "kEdsPropID_FirmwareVersion";
			case kEdsPropID_BatteryLevel: return "kEdsPropID_BatteryLevel";
			case kEdsPropID_CFn: return "kEdsPropID_CFn";
			case kEdsPropID_SaveTo: return "kEdsPropID_SaveTo";
			case kEdsPropID_CurrentStorage: return "kEdsPropID_CurrentStorage";
			case kEdsPropID_CurrentFolder: return "kEdsPropID_CurrentFolder";
			case kEdsPropID_MyMenu: return "kEdsPropID_MyMenu";
				
			case kEdsPropID_BatteryQuality: return "kEdsPropID_BatteryQuality";
			case kEdsPropID_BodyIDEx: return "kEdsPropID_BodyIDEx";				
			case kEdsPropID_HDDirectoryStructure: return "kEdsPropID_HDDirectoryStructure";
				
				
				/*----------------------------------
				 Image Properties
				 ----------------------------------*/
			case kEdsPropID_ImageQuality: return "kEdsPropID_ImageQuality";
			case kEdsPropID_JpegQuality: return "kEdsPropID_JpegQuality";
			case kEdsPropID_Orientation: return "kEdsPropID_Orientation";
			case kEdsPropID_ICCProfile: return "kEdsPropID_ICCProfile";
			case kEdsPropID_FocusInfo: return "kEdsPropID_FocusInfo";
			case kEdsPropID_DigitalExposure: return "kEdsPropID_DigitalExposure";
			case kEdsPropID_WhiteBalance: return "kEdsPropID_WhiteBalance";
			case kEdsPropID_ColorTemperature: return "kEdsPropID_ColorTemperature";
			case kEdsPropID_WhiteBalanceShift: return "kEdsPropID_WhiteBalanceShift";
			case kEdsPropID_Contrast: return "kEdsPropID_Contrast";
			case kEdsPropID_ColorSaturation: return "kEdsPropID_ColorSaturation";
			case kEdsPropID_ColorTone: return "kEdsPropID_ColorTone";
			case kEdsPropID_Sharpness: return "kEdsPropID_Sharpness";
			case kEdsPropID_ColorSpace: return "kEdsPropID_ColorSpace";
			case kEdsPropID_ToneCurve: return "kEdsPropID_ToneCurve";
			case kEdsPropID_PhotoEffect: return "kEdsPropID_PhotoEffect";
			case kEdsPropID_FilterEffect: return "kEdsPropID_FilterEffect";
			case kEdsPropID_ToningEffect: return "kEdsPropID_ToningEffect";
			case kEdsPropID_ParameterSet: return "kEdsPropID_ParameterSet";
			case kEdsPropID_ColorMatrix: return "kEdsPropID_ColorMatrix";
			case kEdsPropID_PictureStyle: return "kEdsPropID_PictureStyle";
			case kEdsPropID_PictureStyleDesc: return "kEdsPropID_PictureStyleDesc";
			case kEdsPropID_PictureStyleCaption: return "kEdsPropID_PictureStyleCaption";
				
				/*----------------------------------
				 Image Processing Properties
				 ----------------------------------*/
			case kEdsPropID_Linear: return "kEdsPropID_Linear";
			case kEdsPropID_ClickWBPoint: return "kEdsPropID_ClickWBPoint";
			case kEdsPropID_WBCoeffs: return "kEdsPropID_WBCoeffs";
				
				
				/*----------------------------------
				 Image GPS Properties
				 ----------------------------------*/
			case kEdsPropID_GPSVersionID: return "kEdsPropID_GPSVersionID";
			case kEdsPropID_GPSLatitudeRef: return "kEdsPropID_GPSLatitudeRef";
			case kEdsPropID_GPSLatitude: return "kEdsPropID_GPSLatitude";
			case kEdsPropID_GPSLongitudeRef: return "kEdsPropID_GPSLongitudeRef";
			case kEdsPropID_GPSLongitude: return "kEdsPropID_GPSLongitude";
			case kEdsPropID_GPSAltitudeRef: return "kEdsPropID_GPSAltitudeRef";
			case kEdsPropID_GPSAltitude: return "kEdsPropID_GPSAltitude";
			case kEdsPropID_GPSTimeStamp: return "kEdsPropID_GPSTimeStamp";
			case kEdsPropID_GPSSatellites: return "kEdsPropID_GPSSatellites";
			case kEdsPropID_GPSStatus: return "kEdsPropID_GPSStatus";
			case kEdsPropID_GPSMapDatum: return "kEdsPropID_GPSMapDatum";
			case kEdsPropID_GPSDateStamp: return "kEdsPropID_GPSDateStamp";
				
				
				/*----------------------------------
				 Property Mask
				 ----------------------------------*/
			case kEdsPropID_AtCapture_Flag: return "kEdsPropID_AtCapture_Flag";
				
				
				/*----------------------------------
				 Capture Properties
				 ----------------------------------*/
			case kEdsPropID_AEMode: return "kEdsPropID_AEMode";
			case kEdsPropID_DriveMode: return "kEdsPropID_DriveMode";
			case kEdsPropID_ISOSpeed: return "kEdsPropID_ISOSpeed";
			case kEdsPropID_MeteringMode: return "kEdsPropID_MeteringMode";
			case kEdsPropID_AFMode: return "kEdsPropID_AFMode";
			case kEdsPropID_Av: return "kEdsPropID_Av";
			case kEdsPropID_Tv: return "kEdsPropID_Tv";
			case kEdsPropID_ExposureCompensation: return "kEdsPropID_ExposureCompensation";
			case kEdsPropID_FlashCompensation: return "kEdsPropID_FlashCompensation";
			case kEdsPropID_FocalLength: return "kEdsPropID_FocalLength";
			case kEdsPropID_AvailableShots: return "kEdsPropID_AvailableShots";
			case kEdsPropID_Bracket: return "kEdsPropID_Bracket";
			case kEdsPropID_WhiteBalanceBracket: return "kEdsPropID_WhiteBalanceBracket";
			case kEdsPropID_LensName: return "kEdsPropID_LensName";
			case kEdsPropID_AEBracket: return "kEdsPropID_AEBracket";
			case kEdsPropID_FEBracket: return "kEdsPropID_FEBracket";
			case kEdsPropID_ISOBracket: return "kEdsPropID_ISOBracket";
			case kEdsPropID_NoiseReduction: return "kEdsPropID_NoiseReduction";
			case kEdsPropID_FlashOn: return "kEdsPropID_FlashOn";
			case kEdsPropID_RedEye: return "kEdsPropID_RedEye";
			case kEdsPropID_FlashMode: return "kEdsPropID_FlashMode";
			case kEdsPropID_LensStatus: return "kEdsPropID_LensStatus";
			case kEdsPropID_Artist: return "kEdsPropID_Artist";
			case kEdsPropID_Copyright: return "kEdsPropID_Copyright";
			case kEdsPropID_DepthOfField: return "kEdsPropID_DepthOfField";
			case kEdsPropID_EFCompensation: return "kEdsPropID_EFCompensation";
				
				/*----------------------------------
				 EVF Properties
				 ----------------------------------*/
			case kEdsPropID_Evf_OutputDevice: return "kEdsPropID_Evf_OutputDevice";
			case kEdsPropID_Evf_Mode: return "kEdsPropID_Evf_Mode";
			case kEdsPropID_Evf_WhiteBalance: return "kEdsPropID_Evf_WhiteBalance";
			case kEdsPropID_Evf_ColorTemperature: return "kEdsPropID_Evf_ColorTemperature";
			case kEdsPropID_Evf_DepthOfFieldPreview: return "kEdsPropID_Evf_DepthOfFieldPreview";
				
				// EVF IMAGE DATA Properties
			case kEdsPropID_Evf_Zoom: return "kEdsPropID_Evf_Zoom";
			case kEdsPropID_Evf_ZoomPosition: return "kEdsPropID_Evf_ZoomPosition";
			case kEdsPropID_Evf_FocusAid: return "kEdsPropID_Evf_FocusAid";
			case kEdsPropID_Evf_Histogram: return "kEdsPropID_Evf_Histogram";
			case kEdsPropID_Evf_ImagePosition: return "kEdsPropID_Evf_ImagePosition";
			case kEdsPropID_Evf_HistogramStatus: return "kEdsPropID_Evf_HistogramStatus";
			case kEdsPropID_Evf_AFMode: return "kEdsPropID_Evf_AFMode";
				
			case kEdsPropID_Evf_CoordinateSystem: return "kEdsPropID_Evf_CoordinateSystem";
			case kEdsPropID_Evf_ZoomRect: return "kEdsPropID_Evf_ZoomRect";
				
			default: return "[unrecognized EdsPropertyID]";
		}
	}
	
	std::string getPropertyEventString(EdsPropertyEvent event) {
		switch(event) {
			case kEdsPropertyEvent_All: return "kEdsPropertyEvent_All"; break;
			case kEdsPropertyEvent_PropertyChanged: return "kEdsPropertyEvent_PropertyChanged"; break;
			case kEdsPropertyEvent_PropertyDescChanged: return "kEdsPropertyEvent_PropertyDescChanged"; break;
			default: return "[unrecognized EdsPropertyEvent]";
		}
	}
	
	std::string getObjectEventString(EdsObjectEvent event) {
		switch(event) {
			case kEdsObjectEvent_All: return "kEdsObjectEvent_All"; break;
			case kEdsObjectEvent_VolumeInfoChanged: return "kEdsObjectEvent_VolumeInfoChanged"; break;
			case kEdsObjectEvent_VolumeUpdateItems: return "kEdsObjectEvent_VolumeUpdateItems"; break;
			case kEdsObjectEvent_FolderUpdateItems: return "kEdsObjectEvent_FolderUpdateItems"; break;
			case kEdsObjectEvent_DirItemCreated: return "kEdsObjectEvent_DirItemCreated"; break;
			case kEdsObjectEvent_DirItemRemoved: return "kEdsObjectEvent_DirItemRemoved"; break;
			case kEdsObjectEvent_DirItemInfoChanged: return "kEdsObjectEvent_DirItemInfoChanged"; break;
			case kEdsObjectEvent_DirItemContentChanged: return "kEdsObjectEvent_DirItemContentChanged"; break;
			case kEdsObjectEvent_DirItemRequestTransfer: return "kEdsObjectEvent_DirItemRequestTransfer"; break;
			case kEdsObjectEvent_DirItemRequestTransferDT: return "kEdsObjectEvent_DirItemRequestTransferDT"; break;
			case kEdsObjectEvent_DirItemCancelTransferDT: return "kEdsObjectEvent_DirItemCancelTransferDT"; break;
			case kEdsObjectEvent_VolumeAdded: return "kEdsObjectEvent_VolumeAdded"; break;
			case kEdsObjectEvent_VolumeRemoved: return "kEdsObjectEvent_VolumeRemoved"; break;
			default: return "[unrecognized EdsObjectEvent]";
		}
	}
	
	
	std::string getStateEventString(EdsStateEvent event) {
		switch(event) {
			case kEdsStateEvent_All: return "kEdsStateEvent_All"; break;
			case kEdsStateEvent_Shutdown: return "kEdsStateEvent_Shutdown"; break;
			case kEdsStateEvent_JobStatusChanged: return "kEdsStateEvent_JobStatusChanged"; break;
			case kEdsStateEvent_WillSoonShutDown: return "kEdsStateEvent_WillSoonShutDown"; break;
			case kEdsStateEvent_ShutDownTimerUpdate: return "kEdsStateEvent_ShutDownTimerUpdate"; break;
			case kEdsStateEvent_CaptureError: return "kEdsStateEvent_CaptureError"; break;
			case kEdsStateEvent_InternalError: return "kEdsStateEvent_InternalError"; break;
			case kEdsStateEvent_AfResult: return "kEdsStateEvent_AfResult"; break;
			case kEdsStateEvent_BulbExposureTime: return "kEdsStateEvent_BulbExposureTime"; break;
			default: return "[unrecognized EdsStateEvent]";
		}
	}
}