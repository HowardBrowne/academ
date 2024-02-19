#include "EdsWrapper.h"

namespace Eds {
	void Retain(EdsBaseRef inRef){EdsError err = EdsRetain(inRef); if(err != EDS_ERR_OK) {throw Exception(err, "Retain");}}
	void Release(EdsBaseRef inRef){EdsError err = EdsRelease(inRef); if(err != EDS_ERR_OK) {throw Exception(err, "Release");}}
	void InitializeSDK(){EdsError err = EdsInitializeSDK(); if(err != EDS_ERR_OK) {throw Exception(err, "InitializeSDK");}}
	void TerminateSDK(){EdsError err = EdsTerminateSDK(); if(err != EDS_ERR_OK) {throw Exception(err, "TerminateSDK");}}
	void GetChildCount(EdsBaseRef inRef, EdsUInt32* outCount) {EdsError err = EdsGetChildCount(inRef, outCount); if(err != EDS_ERR_OK) {throw Exception(err, "GetChildCount");}}
	void GetChildAtIndex(EdsBaseRef inRef, EdsInt32 inIndex, EdsBaseRef* outRef) {EdsError err = EdsGetChildAtIndex(inRef, inIndex, outRef); if(err != EDS_ERR_OK) {throw Exception(err, "GetChildAtIndex");}}
	void GetParent(EdsBaseRef inRef, EdsBaseRef* outParentRef) {EdsError err = EdsGetParent(inRef, outParentRef); if(err != EDS_ERR_OK) {throw Exception(err, "GetParent");}}
	void GetPropertySize(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsInt32 inParam, EdsDataType* outDataType, EdsUInt32* outSize) {EdsError err = EdsGetPropertySize(inRef, inPropertyID, inParam, outDataType, outSize); if(err != EDS_ERR_OK) {throw Exception(err, "GetPropertySize");}}
	void GetPropertyData(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsInt32 inParam, EdsUInt32 inPropertySize, EdsVoid* outPropertyData) {EdsError err = EdsGetPropertyData(inRef, inPropertyID, inParam, inPropertySize, outPropertyData); if(err != EDS_ERR_OK) {throw Exception(err, "GetPropertyData");}}
	void SetPropertyData(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsInt32 inParam, EdsUInt32 inPropertySize, const EdsVoid* inPropertyData) {EdsError err = EdsSetPropertyData(inRef, inPropertyID, inParam, inPropertySize, inPropertyData); if(err != EDS_ERR_OK) {throw Exception(err, "SetPropertyData");}}
	void GetPropertyDesc(EdsBaseRef inRef, EdsPropertyID inPropertyID, EdsPropertyDesc* outPropertyDesc) {EdsError err = EdsGetPropertyDesc(inRef, inPropertyID, outPropertyDesc); if(err != EDS_ERR_OK) {throw Exception(err, "GetPropertyDesc");}}
	void GetCameraList(EdsCameraListRef* outCameraListRef) {EdsError err = EdsGetCameraList(outCameraListRef); if(err != EDS_ERR_OK) {throw Exception(err, "GetCameraList");}}
	void GetDeviceInfo(EdsCameraRef inCameraRef, EdsDeviceInfo* outDeviceInfo) {EdsError err = EdsGetDeviceInfo(inCameraRef, outDeviceInfo); if(err != EDS_ERR_OK) {throw Exception(err, "GetDeviceInfo");}}
	void OpenSession(EdsCameraRef inCameraRef) {EdsError err = EdsOpenSession(inCameraRef); if(err != EDS_ERR_OK) {throw Exception(err, "OpenSession");}}
	void CloseSession(EdsCameraRef inCameraRef) {EdsError err = EdsCloseSession(inCameraRef); if(err != EDS_ERR_OK) {throw Exception(err, "CloseSession");}}
	void SendCommand(EdsCameraRef inCameraRef, EdsCameraCommand inCommand, EdsInt32 inParam) {EdsError err = EdsSendCommand(inCameraRef, inCommand, inParam); if(err != EDS_ERR_OK) {throw Exception(err, "SendCommand");}}
	void SendStatusCommand(EdsCameraRef inCameraRef, EdsCameraStatusCommand inStatusCommand, EdsInt32 inParam) {EdsError err = EdsSendStatusCommand(inCameraRef, inStatusCommand, inParam); if(err != EDS_ERR_OK) {throw Exception(err, "SendStatusCommand");}}
	void SetCapacity(EdsCameraRef inCameraRef, EdsCapacity inCapacity) {EdsError err = EdsSetCapacity(inCameraRef, inCapacity); if(err != EDS_ERR_OK) {throw Exception(err, "SetCapacity");}}
	void GetVolumeInfo(EdsVolumeRef inVolumeRef, EdsVolumeInfo* outVolumeInfo) {EdsError err = EdsGetVolumeInfo(inVolumeRef, outVolumeInfo); if(err != EDS_ERR_OK) {throw Exception(err, "GetVolumeInfo");}}
	void FormatVolume(EdsVolumeRef inVolumeRef) {EdsError err = EdsFormatVolume(inVolumeRef); if(err != EDS_ERR_OK) {throw Exception(err, "FormatVolume");}}
	void GetDirectoryItemInfo(EdsDirectoryItemRef inDirItemRef, EdsDirectoryItemInfo* outDirItemInfo) {EdsError err = EdsGetDirectoryItemInfo(inDirItemRef, outDirItemInfo); if(err != EDS_ERR_OK) {throw Exception(err, "GetDirectoryItemInfo");}}
	void DeleteDirectoryItem(EdsDirectoryItemRef inDirItemRef) {EdsError err = EdsDeleteDirectoryItem(inDirItemRef); if(err != EDS_ERR_OK) {throw Exception(err, "DeleteDirectoryItem");}}
	void Download(EdsDirectoryItemRef inDirItemRef, EdsUInt32 inReadSize, EdsStreamRef outStream) {EdsError err = EdsDownload(inDirItemRef, inReadSize, outStream); if(err != EDS_ERR_OK) {throw Exception(err, "Download");}}
	void DownloadCancel(EdsDirectoryItemRef inDirItemRef) {EdsError err = EdsDownloadCancel(inDirItemRef); if(err != EDS_ERR_OK) {throw Exception(err, "DownloadCancel");}}
	void DownloadComplete(EdsDirectoryItemRef inDirItemRef) {EdsError err = EdsDownloadComplete(inDirItemRef); if(err != EDS_ERR_OK) {throw Exception(err, "DownloadComplete");}}
	void DownloadThumbnail(EdsDirectoryItemRef inDirItemRef, EdsStreamRef outStream) {EdsError err = EdsDownloadThumbnail(inDirItemRef, outStream); if(err != EDS_ERR_OK) {throw Exception(err, "DownloadThumbnail");}}
	void GetAttribute(EdsDirectoryItemRef inDirItemRef, EdsFileAttributes* outFileAttribute) {EdsError err = EdsGetAttribute(inDirItemRef, outFileAttribute); if(err != EDS_ERR_OK) {throw Exception(err, "GetAttribute");}}
	void SetAttribute(EdsDirectoryItemRef inDirItemRef, EdsFileAttributes inFileAttribute) {EdsError err = EdsSetAttribute(inDirItemRef, inFileAttribute); if(err != EDS_ERR_OK) {throw Exception(err, "SetAttribute");}}
	void CreateFileStream(const EdsChar* inFileName, EdsFileCreateDisposition inCreateDisposition, EdsAccess inDesiredAccess, EdsStreamRef* outStream) {EdsError err = EdsCreateFileStream(inFileName, inCreateDisposition, inDesiredAccess, outStream); if(err != EDS_ERR_OK) {throw Exception(err, "CreateFileStream");}}
	void CreateMemoryStream(EdsUInt32 inBufferSize, EdsStreamRef* outStream) {EdsError err = EdsCreateMemoryStream(inBufferSize, outStream); if(err != EDS_ERR_OK) {throw Exception(err, "CreateMemoryStream");}}
	void CreateFileStreamEx(
		#ifdef __MACOS__
			const CFURLRef inURL,
		#else
			const WCHAR* inURL,
		#endif
		EdsFileCreateDisposition inCreateDisposition, EdsAccess inDesiredAccess, EdsStreamRef* outStream) {EdsError err = EdsCreateFileStreamEx(inURL, inCreateDisposition, inDesiredAccess, outStream); if(err != EDS_ERR_OK) {throw Exception(err, "CreateFileStreamEx");}}
	void CreateMemoryStreamFromPointer(EdsVoid* inUserBuffer, EdsUInt32 inBufferSize, EdsStreamRef* outStream) {EdsError err = EdsCreateMemoryStreamFromPointer(inUserBuffer, inBufferSize, outStream); if(err != EDS_ERR_OK) {throw Exception(err, "CreateMemoryStreamFromPointer");}}
	void GetPointer(EdsStreamRef inStream, EdsVoid** outPointer) {EdsError err = EdsGetPointer(inStream, outPointer); if(err != EDS_ERR_OK) {throw Exception(err, "GetPointer");}}
	void Read(EdsStreamRef inStreamRef, EdsUInt32 inReadSize, EdsVoid* outBuffer, EdsUInt32* outReadSize) {EdsError err = EdsRead(inStreamRef, inReadSize, outBuffer, outReadSize); if(err != EDS_ERR_OK) {throw Exception(err, "Read");}}
	void Write(EdsStreamRef inStreamRef, EdsUInt32 inWriteSize, const EdsVoid* inBuffer, EdsUInt32* outWrittenSize) {EdsError err = EdsWrite(inStreamRef, inWriteSize, inBuffer, outWrittenSize); if(err != EDS_ERR_OK) {throw Exception(err, "Write");}}
	void Seek(EdsStreamRef inStreamRef, EdsInt32 inSeekOffset, EdsSeekOrigin inSeekOrigin) {EdsError err = EdsSeek(inStreamRef, inSeekOffset, inSeekOrigin); if(err != EDS_ERR_OK) {throw Exception(err, "Seek");}}
	void GetPosition(EdsStreamRef inStreamRef, EdsUInt32* outPosition) {EdsError err = EdsGetPosition(inStreamRef, outPosition); if(err != EDS_ERR_OK) {throw Exception(err, "GetPosition");}}
	void GetLength(EdsStreamRef inStreamRef, EdsUInt32* outLength) {EdsError err = EdsGetLength(inStreamRef, outLength); if(err != EDS_ERR_OK) {throw Exception(err, "GetLength");}}
	void CopyData(EdsStreamRef inStreamRef, EdsUInt32 inWriteSize, EdsStreamRef outStreamRef) {EdsError err = EdsCopyData(inStreamRef, inWriteSize, outStreamRef); if(err != EDS_ERR_OK) {throw Exception(err, "CopyData");}}
	void SetProgressCallback(EdsBaseRef inRef, EdsProgressCallback inProgressCallback, EdsProgressOption inProgressOption, EdsVoid* inContext) {EdsError err = EdsSetProgressCallback(inRef, inProgressCallback, inProgressOption, inContext); if(err != EDS_ERR_OK) {throw Exception(err, "SetProgressCallback");}}
	void CreateImageRef(EdsStreamRef inStreamRef, EdsImageRef* outImageRef) {EdsError err = EdsCreateImageRef(inStreamRef, outImageRef); if(err != EDS_ERR_OK) {throw Exception(err, "CreateImageRef");}}
	void GetImageInfo(EdsImageRef inImageRef, EdsImageSource inImageSource, EdsImageInfo* outImageInfo) {EdsError err = EdsGetImageInfo(inImageRef, inImageSource, outImageInfo); if(err != EDS_ERR_OK) {throw Exception(err, "GetImageInfo");}}
	void GetImage(EdsImageRef inImageRef, EdsImageSource inImageSource, EdsTargetImageType inImageType, EdsRect inSrcRect, EdsSize inDstSize, EdsStreamRef outStreamRef) {EdsError err = EdsGetImage(inImageRef, inImageSource, inImageType, inSrcRect, inDstSize, outStreamRef); if(err != EDS_ERR_OK) {throw Exception(err, "GetImage");}}
	void SaveImage(EdsImageRef inImageRef, EdsTargetImageType inImageType, EdsSaveImageSetting inSaveSetting, EdsStreamRef outStreamRef) {EdsError err = EdsSaveImage(inImageRef, inImageType, inSaveSetting, outStreamRef); if(err != EDS_ERR_OK) {throw Exception(err, "SaveImage");}}
	void CacheImage(EdsImageRef inImageRef, EdsBool inUseCache) {EdsError err = EdsCacheImage(inImageRef, inUseCache); if(err != EDS_ERR_OK) {throw Exception(err, "CacheImage");}}
	void ReflectImageProperty(EdsImageRef inImageRef) {EdsError err = EdsReflectImageProperty(inImageRef); if(err != EDS_ERR_OK) {throw Exception(err, "ReflectImageProperty");}}
	void CreateEvfImageRef(EdsStreamRef inStreamRef, EdsEvfImageRef *outEvfImageRef){EdsError err = EdsCreateEvfImageRef(inStreamRef, outEvfImageRef); if(err != EDS_ERR_OK) {throw Exception(err, "CreateEvfImageRef");}}
	void DownloadEvfImage(EdsCameraRef inCameraRef, EdsEvfImageRef inEvfImageRef){EdsError err = EdsDownloadEvfImage(inCameraRef, inEvfImageRef); if(err != EDS_ERR_OK) {throw Exception(err, "DownloadEvfImage");}}
	void SetCameraAddedHandler(EdsCameraAddedHandler inCameraAddedHandler, EdsVoid* inContext) {EdsError err = EdsSetCameraAddedHandler(inCameraAddedHandler, inContext); if(err != EDS_ERR_OK) {throw Exception(err, "SetCameraAddedHandler");}}
	void SetPropertyEventHandler(EdsCameraRef inCameraRef, EdsPropertyEvent inEvnet, EdsPropertyEventHandler inPropertyEventHandler, EdsVoid* inContext) {EdsError err = EdsSetPropertyEventHandler(inCameraRef, inEvnet, inPropertyEventHandler, inContext); if(err != EDS_ERR_OK) {throw Exception(err, "SetPropertyEventHandler");}}
	void SetObjectEventHandler(EdsCameraRef inCameraRef, EdsObjectEvent inEvnet, EdsObjectEventHandler inObjectEventHandler, EdsVoid* inContext) {EdsError err = EdsSetObjectEventHandler(inCameraRef, inEvnet, inObjectEventHandler, inContext); if(err != EDS_ERR_OK) {throw Exception(err, "SetObjectEventHandler");}}
	void SetCameraStateEventHandler(EdsCameraRef inCameraRef, EdsStateEvent inEvnet, EdsStateEventHandler inStateEventHandler, EdsVoid* inContext) {EdsError err = EdsSetCameraStateEventHandler(inCameraRef, inEvnet, inStateEventHandler, inContext); if(err != EDS_ERR_OK) {throw Exception(err, "SetCameraStateEventHandler");}}
	void CreateStream(EdsIStream* inStream, EdsStreamRef* outStreamRef) {EdsError err = EdsCreateStream(inStream, outStreamRef); if(err != EDS_ERR_OK) {throw Exception(err, "CreateStream");}}
	void GetEvent(){EdsError err = EdsGetEvent(); if(err != EDS_ERR_OK) {throw Exception(err, "GetEvent");}}	
}