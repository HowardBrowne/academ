# ofxEdsdk

ofxEdsdk requires the EDSDK library from Canon.

ofxEdsdk was developed with support from YCAM http://www.ycam.jp/


## Installation/Usage

1. Apply for a developer's license from [Canon's website](http://consumer.usa.canon.com/cusa/support/consumer/eos_slr_camera_systems/eos_digital_slr_cameras/digital_rebel_xt?fileURL=ps_sdk_form&pageKeyCode=downloadLicense&id=0901e02480057a74_1&productOverviewCid=0901e0248003ce28&keycode=Sdk_Lic) 
2. After you have been approved (can take anywhere from minutes to a day or two), follow the download instructions included in your confirmation email from Canon.
3. Download this repo as a zip file, unzip, and place contents in your addons folder.
4. Edit the folder name so that it only says "ofxEdsdk."
5. Open the Canon EDSDK .dmg image and save the files somewhere you can get at them if you need to. 
6. Copy the EDSDK folder into this directory: OF > addons > ofEdsdk > src
7. Open the OF Project Generator application and build a new app, including the ofxEdsdk addon.

Following these steps will include Canon's developer library and this wrapper for all its functions for use in OF. You should be able to run the blank project successfully.

ofxEdsdk is tested with Canon SDK 2.14, on both OS X and Windows 8.1 with openFrameworks 0.9.0

	Update: updated and tested and with Canon SDK 3.4.0 on OS X with openFrameworks 0.9.2