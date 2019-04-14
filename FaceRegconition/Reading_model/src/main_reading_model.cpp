#include "../header/face_regconition.hpp"
#include "../header/func_face_regcontion.hpp"

int main(int argc, const char *argv[]) {
	int what = userChooseAlg();
	string file_model = "/home/jake/Desktop/Projects/Cpp/FaceRegconition/Training_model/output/";
	//start reading model
	cout << "Reading model..." << endl;
	Ptr<FaceRecognizer> model;
	switch (what)
	{
		case EIGENFACES: {
			file_model += "eigen_face_model.yml";
			cout << "Using default input path: " << file_model << endl;
			model = EigenFaceRecognizer::create();
			break;
		}
		case FISHERFACES: {
			file_model += "fisher_face_model.yml";
			cout << "Using default input path: " << file_model << endl;
			model = FisherFaceRecognizer::create();
			break;
		}
		case LBPHFACES: {
			file_model += "LBPH_face_model.yml";
			cout << "Using default input path: " << file_model << endl;
			model = LBPHFaceRecognizer::create();
			break;
		}
		default: {
			cout << "Khong giai thuat nao duoc chon" << endl;
			system("pause");
			return 1;
		}
	}
	//Reading it
	model->read(file_model);
	if (model->empty()) {
		cout << "Model load khong thanh cong, dung chuong trinh " << endl;
		return 0;
	}
	cout << "Load model successful" << endl;
	//set threshold
	model->setThreshold(1000);

	/* Loading test case */
	vector<Mat> testSample;
	vector<int> testLabel;
	try {
		string fn = "/home/jake/Desktop/Projects/Cpp/FaceRegconition/Data_Preparation/test_set.csv";
		read_csv(fn, testSample, testLabel);
	}
	catch (const cv::Exception& e) {
		cerr << "Error opening file \""  << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		system("pause");
	}

	//CHECK ERROR MATRICES SIZE
	for (int i = 0; i < testSample.size(); i++) {
		if (testSample[i].size().empty()) cout << "Test Errror: " << testLabel[i] << endl;
	}

	/*Loading dataset_id*/
	/*vector<Data> data;
	string path_dataset = "D:/Work/My_Project/Cpp_PetProject/Data_Preparation/Data_Output/dataset_id.csv";
	if (!read_dataset_id(path_dataset, data)) {
		cout << "There are line(s) couldn't be read correctly" << endl;
	}
	for (int i = 0; i < 8; i++) {
		cout << getName(i, data) << endl;
	}*/
	// vecto predict
	vector<int> predicted_label;
	vector<double> predicted_confidence;
	string check = "";


	//Predict output
	for (int i = 0; i < testLabel.size(); i++){
		int label=-1; double confident=0.0;
		model->predict(testSample[i], label, confident);
		predicted_label.push_back(label);
		predicted_confidence.push_back(confident);
		if (predicted_label[i] != testLabel[i]) check = "*";
		else check = "";
		cout << check << "predict: " << predicted_label[i] << " | " << testLabel[i]<<" -- "<< predicted_confidence[i] << endl;
	}
	//Destroy mem
	//data.clear();
	testSample.clear();
	testLabel.clear();
	predicted_label.clear();
	predicted_confidence.clear();
	model->clear();
	return 0;
}