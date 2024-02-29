#pragma once
/*
 EdsdkWrapper is a collection of functions that add Exceptions to the standard Edsdk collection.
 This is the only file that should reference Edsdk.
 */

#define __MACOS__
#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"

#include "EdsStrings.h"
#include "EdsException.h"
#include "EdsExamples.h"

#include "ofxEdsdk.h"

namespace Eds {
	void Retain(EdsBaseRef inRef);
	void Release(EdsBaseRef inRef);
	void InitializeSDK();
	void TerminateSDK();
	void GetChildCount(EdsBaseRef inRef, EdsUInt32* outCount);
	void GetChildAtIndex(EdsBaseRef inRef, EdsInt32 inIndex, EdsBaseRef* outRef);
	void GetParent(EdsBaseRef inRef, EdsBaseRef* outParentRef);
	void GetPropertySize(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsInt32 inParam, EdsDataType* outDataType, EdsUInt32* outSize);
	void GetPropertyData(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsInt32 inParam, EdsUInt32 inPropertySize, EdsVoid* outPropertyData);
	void SetPropertyData(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsInt32 inParam, EdsUInt32 inPropertySize, const EdsVoid* inPropertyData);
	void GetPropertyDesc(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsPropertyDesc* outPropertyDesc);
	void GetCameraList(EdsCameraListRef* outCameraListRef);
	void GetDeviceInfo(EdsCameraRef inCameraRef, EdsDeviceInfo* outDeviceInfo);
	void OpenSession(EdsCameraRef inCameraRef);
	void CloseSession(EdsCameraRef inCameraRef);
	void SendCommand(EdsCameraRef inCameraRef, EdsCameraCommand inCommand, EdsInt32 inParam);
	void SendStatusCommand(EdsCameraRef inCameraRef, EdsCameraStatusCommand inStatusCommand, EdsInt32 inParam);
	void SetCapacity(EdsCameraRef inCameraRef, EdsCapacity inCapacity);
	void GetVolumeInfo(EdsVolumeRef inVolumeRef, EdsVolumeInfo* outVolumeInfo);
	void FormatVolume(EdsVolumeRef inVolumeRef);
	void GetDirectoryItemInfo(EdsDirectoryItemRef inDirItemRef, EdsDirectoryItemInfo* outDirItemInfo);
	void DeleteDirectoryItem(EdsDirectoryItemRef inDirItemRef);
	void Download(EdsDirectoryItemRef inDirItemRef, EdsUInt32 inReadSize, EdsStreamRef outStream);
	void DownloadCancel(EdsDirectoryItemRef inDirItemRef);
	void DownloadComplete(EdsDirectoryItemRef inDirItemRef);
	void DownloadThumbnail(EdsDirectoryItemRef inDirItemRef, EdsStreamRef outStream);
	void GetAttribute(EdsDirectoryItemRef inDirItemRef, EdsFileAttributes* outFileAttribute);
	void SetAttribute(EdsDirectoryItemRef inDirItemRef, EdsFileAttributes inFileAttribute);
	void CreateFileStream(const EdsChar* inFileName, EdsFileCreateDisposition inCreateDisposition, EdsAccess inDesiredAccess, EdsStreamRef* outStream);
	void CreateMemoryStream(EdsUInt32 inBufferSize, EdsStreamRef* outStream);
	void CreateFileStreamEx(
		#ifdef __MACOS__
			const CFURLRef inURL,
		#else
			const WCHAR* inURL,
		#endif
		EdsFileCreateDisposition inCreateDisposition, EdsAccess inDesiredAccess, EdsStreamRef* outStream);
	void CreateMemoryStreamFromPointer(EdsVoid* inUserBuffer, EdsUInt32 inBufferSize, EdsStreamRef* outStream);
	void GetPointer(EdsStreamRef inStream, EdsVoid** outPointer);
	void Read(EdsStreamRef inStreamRef, EdsUInt32 inReadSize, EdsVoid* outBuffer, EdsUInt32* outReadSize);
	void Write(EdsStreamRef inStreamRef, EdsUInt32 inWriteSize, const EdsVoid* inBuffer, EdsUInt32* outWrittenSize);
	void Seek(EdsStreamRef inStreamRef, EdsInt32 inSeekOffset, EdsSeekOrigin inSeekOrigin);
	void GetPosition(EdsStreamRef inStreamRef, EdsUInt32* outPosition);
	void GetLength(EdsStreamRef inStreamRef, EdsUInt32* outLength);
	void CopyData(EdsStreamRef inStreamRef, EdsUInt32 inWriteSize, EdsStreamRef outStreamRef);
	void SetProgressCallback(EdsBaseRef inRef, EdsProgressCallback inProgressCallback, EdsProgressOption inProgressOption, EdsVoid* inContext);
	void CreateImageRef(EdsStreamRef inStreamRef, EdsImageRef* outImageRef);
	void GetImageInfo(EdsImageRef inImageRef, EdsImageSource inImageSource, EdsImageInfo* outImageInfo);
	void GetImage(EdsImageRef inImageRef, EdsImageSource inImageSource, EdsTargetImageType inImageType, EdsRect inSrcRect, EdsSize inDstSize, EdsStreamRef outStreamRef);
	void SaveImage(EdsImageRef inImageRef, EdsTargetImageType inImageType, EdsSaveImageSetting inSaveSetting, EdsStreamRef outStreamRef);
	void CacheImage(EdsImageRef inImageRef, EdsBool inUseCache);
	void ReflectImageProperty(EdsImageRef inImageRef);
	void CreateEvfImageRef(EdsStreamRef inStreamRef, EdsEvfImageRef *outEvfImageRef);
	void DownloadEvfImage(EdsCameraRef inCameraRef, EdsEvfImageRef inEvfImageRef);
	void SetCameraAddedHandler(EdsCameraAddedHandler inCameraAddedHandler, EdsVoid* inContext);
	void SetPropertyEventHandler(EdsCameraRef inCameraRef, EdsPropertyEvent inEvnet, EdsPropertyEventHandler inPropertyEventHandler, EdsVoid* inContext);
	void SetObjectEventHandler(EdsCameraRef inCameraRef, EdsObjectEvent inEvnet, EdsObjectEventHandler inObjectEventHandler, EdsVoid* inContext);
	void SetCameraStateEventHandler(EdsCameraRef inCameraRef, EdsStateEvent inEvnet, EdsStateEventHandler inStateEventHandler, EdsVoid* inContext);
	void CreateStream(EdsIStream* inStream, EdsStreamRef* outStreamRef);
	void GetEvent();	
}
