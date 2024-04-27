#include "EdsExamples.h"

#include "ofLog.h"

namespace Eds {

	// from EDSDK API sample 6.3.6
	void DownloadImage(EdsDirectoryItemRef directoryItem, ofBuffer& imageBuffer, bool deleteAfterDownload) {
		EdsStreamRef stream = NULL;
		EdsDirectoryItemInfo dirItemInfo;
		Eds::GetDirectoryItemInfo(directoryItem, &dirItemInfo);
		Eds::CreateMemoryStream(0, &stream);
		Eds::Download(directoryItem, dirItemInfo.size, stream);
		Eds::DownloadComplete(directoryItem);
		Eds::CopyStream(stream, imageBuffer);
		if(deleteAfterDownload) {
			Eds::DeleteDirectoryItem(directoryItem);
		}
		Eds::SafeRelease(stream);
	}
	
	// from EDSDK API sample 6.3.10
	void StartLiveview(EdsCameraRef camera) {
		try {
			// Get the output device for the live view image
			EdsUInt32 device;
			Eds::GetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// PC live view starts by setting the PC as the output device for the live view image.
			device |= kEdsEvfOutputDevice_PC;
			Eds::SetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// A property change event notification is issued from the camera if property settings are made successfully.
			// Start downloading of the live view image once the property change notification arrives.
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error starting live view: " << e.what();
		}
	}
	
	// from EDSDK API sample 6.3.10
	bool DownloadEvfData(EdsCameraRef camera, ofBuffer& imageBuffer) {
		EdsStreamRef stream = NULL;
		EdsEvfImageRef evfImage = NULL;
		bool frameNew = false;
		
		try {
			Eds::CreateMemoryStream(0, &stream);
			Eds::CreateEvfImageRef(stream, &evfImage);
			Eds::DownloadEvfImage(camera, evfImage);
			Eds::CopyStream(stream, imageBuffer);
			frameNew = true;
		} catch (Eds::Exception& e) {
			if(e != EDS_ERR_OBJECT_NOTREADY) {
				ofLogError() << "There was an error downloading the live view data:" << e.what();
			}
		}
		
		try {
			Eds::SafeRelease(stream);
			Eds::SafeRelease(evfImage);
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error releasing the live view data: " << e.what();
		}
		
		return frameNew;
	}
	
	// from EDSDK API sample 6.3.10
	void EndLiveview(EdsCameraRef camera) {
		try {		
			// Get the output device for the live view image
			EdsUInt32 device;
			Eds::GetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// PC live view ends if the PC is disconnected from the live view image output device.
			device &= ~kEdsEvfOutputDevice_PC;
			Eds::SetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error closing the live view stream: " << e.what();
		}
	}
	
	
	void CopyStream(EdsStreamRef stream, ofBuffer& buffer) {
		EdsUInt32 length;
		Eds::GetLength(stream, &length);
		char* streamPointer;
		Eds::GetPointer(stream, (EdsVoid**) &streamPointer);
		buffer.set(streamPointer, length);
	}
	
	void SafeRelease(EdsBaseRef& inRef) {
		if(inRef != NULL) {
			Eds::Release(inRef);
			inRef = NULL;
		}
	}
}