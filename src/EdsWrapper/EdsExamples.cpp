#include "EdsExamples.h"

#include "ofLog.h"

namespace Eds {

	// from EDSDK API sample 6.3.7
	void GetVolume(EdsCameraRef camera, EdsVolumeRef volume) {
		try {
			EdsUInt32 count = 0;
			Eds::GetChildCount(camera, &count);
			Eds::GetChildAtIndex(camera, 0, &volume);
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error getting the file object: " << e.what();
		}
	}
	
	// from EDSDK API sample 6.3.8
	void GetDCIMFolder(EdsVolumeRef volume, EdsDirectoryItemRef directoryItem) {
		try {
			EdsDirectoryItemRef dirItem = NULL;
			EdsDirectoryItemInfo dirItemInfo;
			EdsUInt32 count = 0;
			Eds::GetChildCount(volume, &count);
			for(int i = 0; i < count; i++) {
				Eds::GetChildAtIndex(volume, i, &dirItem);
				Eds::GetDirectoryItemInfo(dirItem, &dirItemInfo);
				if(strcmp(dirItemInfo.szFileName, "DCIM") == 0 && dirItemInfo.isFolder) {
					directoryItem = dirItem;
					break;
				}
				if(dirItem) {
					Eds::Release(dirItem);
				}
			}
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error getting the DCIM folder: " << e.what();
		}
	}
	
	// from EDSDK API sample 6.3.6
	void DownloadImage(EdsDirectoryItemRef directoryItem) {
		try {
			EdsStreamRef stream = NULL;
			EdsDirectoryItemInfo dirItemInfo;
			Eds::GetDirectoryItemInfo(directoryItem, &dirItemInfo);
			Eds::CreateFileStream(dirItemInfo.szFileName, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
			Eds::Download(directoryItem, dirItemInfo.size, stream);
			Eds::DownloadComplete(directoryItem);
			Eds::Release(stream);
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error downloading the image: " << e.what();
		}
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
			//	Create memory stream.
			// This automatically allocates the stream if it's unallocated.
			// If you want to save some time, avoid reallocation by keeping the EdsStreamRef around.
			// Alternatively, you can prepare the memory yourself and use EdsCreateMemoryStreamFromPointer.
			Eds::CreateMemoryStream(0, &stream);
			
			//	Create EvfImageRef.
			Eds::CreateEvfImageRef(stream, &evfImage);
			
			// Download live view image data.
			Eds::DownloadEvfImage(camera, evfImage);
			
			// Get the image data.
			EdsUInt32 length;
			Eds::GetLength(stream, &length);
			
			char* streamPointer;
			Eds::GetPointer(stream, (EdsVoid**) &streamPointer);
			
			imageBuffer.set(streamPointer, length);
			
			frameNew = true;
		} catch (Eds::Exception& e) {
			if(e != EDS_ERR_OBJECT_NOTREADY) {
				ofLogError() << "There was an error downloading the live view data:" << e.what();
			}
		}
		
		try {
			// Release stream
			if(stream != NULL) {
				Eds::Release(stream);
				stream = NULL;
			}
			
			// Release evfImage
			if(evfImage != NULL) {
				Eds::Release(evfImage);
				evfImage = NULL;
			}
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

}