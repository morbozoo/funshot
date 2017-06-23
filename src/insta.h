class insta : public ofThread {

	void insta::threadedFunction() {
		newTime = ofGetTimestampString();
		system("python C:\\Users\\Quasar\\Documents\\of_v0.9.0_vs_release\\apps\\myApps\\funshot\\bin\\data\\insta\\test.py amozurrutiadiego c4n0t4j3");
		for (int i = 1; i < 4; i++) {
			string fileName = "sc";
			fileName += ofToString(i);
			fileName += ".jpg";
			ofFile tempFile(fileName);
			tempFile.copyTo("sent", true, true);
			tempFile.close();

			string newCommand = "rename C:\\Users\\Quasar\\Documents\\of_v0.9.0_vs_release\\apps\\myApps\\funshot\\bin\\data\\sent\\sc" + ofToString(i) + ".jpg " + newTime + "_n" + ofToString(i) + ".jpg";
			cout << "cleaner = " << newTime << endl;
			system(newCommand.c_str());
		}
		string printPath = "rundll32.exe c:\\Windows\\System32\\shimgvw.dll, ImageView_PrintTo \"c:\\Users\\Quasar\\Documents\\of_v0.9.0_vs_release\\apps\\myApps\\funshot\\bin\\data\\finalShot.jpg\" \"HP Color LaserJet Pro MFP M277 PCL 6\"";
		cout << "printPath = " << printPath << endl;
		system(printPath.c_str());
	}
public:
	string newTime;
};