class insta : public ofThread {

	void insta::threadedFunction() {
		system("python C:\\Users\\Quasar\\Documents\\of_v0.9.0_vs_release\\apps\\myApps\\funshot\\bin\\data\\insta\\test.py amozurrutiadiego c4n0t4j3");
		for (int i = 1; i < 4; i++) {
			string fileName = "sc";
			fileName += ofToString(i);
			fileName += ".jpg";
			cout << "filename: " << fileName << endl;
			ofFile tempFile(fileName);
			tempFile.copyTo("sent", true, true);
			
		}
	}
public:

};