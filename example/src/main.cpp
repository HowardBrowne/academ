#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1056, 704, OF_WINDOW);
	ofRunApp(new testApp());
}
