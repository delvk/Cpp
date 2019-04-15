#include "../header/face_regconition.hpp"
#include "../header/func_face_regcontion.hpp"
#include <limits.h>
int main(int argc, const char *argv[])
{

	double threshold = DBL_MAX;
	//Get Parameters
	cout << "usage: " << argv[0] << endl;
	cout << "<pre-trained_model> <testing_set> [threshold]" << endl;
	cout << "\n----------------------------------------------------------\n\n";
	if (argc < 3)
	{
		cout << "Please point to pre-trained model facefont and testing set" << endl;
		exit(-1);
	}
	else
	{
		if (argc > 3)
			try
			{
				threshold = atof(argv[3]);
			}
			catch (exception e)
			{
				cout << "Error in convert your threshold to double" << endl;
				exit(-1);
			}
		cout << "- " << argv[1] << endl;
		cout << "- " << argv[2] << endl;
		cout << "- " << threshold << endl;
	}

	string file_model = string(argv[1]);
	string fn = string(argv[2]);

	//GUI
	int what = userChooseAlg();

	//start reading model
	cout << "Reading model..." << endl;
	Ptr<FaceRecognizer> model;
	switch (what)
	{
	case EIGENFACES:
	{
		cout << "Using default input path: " << file_model << endl;
		model = EigenFaceRecognizer::create();
		break;
	}
	case FISHERFACES:
	{
		cout << "Using default input path: " << file_model << endl;
		model = FisherFaceRecognizer::create();
		break;
	}
	case LBPHFACES:
	{
		cout << "Using default input path: " << file_model << endl;
		model = LBPHFaceRecognizer::create();
		break;
	}
	default:
	{
		cout << "Khong giai thuat nao duoc chon" << endl;
		system("pause");
		return 1;
	}
	}
	//Reading it
	model->read(file_model);
	if (model->empty())
	{
		cout << "Model load khong thanh cong, dung chuong trinh " << endl;
		return 0;
	}
	cout << "Load model successful" << endl;
	
	//set threshold
	model->setThreshold(threshold);

	/* Loading test case */
	vector<Mat> testSample;
	vector<int> testLabel;
	try
	{

		read_csv(fn, testSample, testLabel);
	}
	catch (const cv::Exception &e)
	{
		cerr << "Error opening file \""
			 << "\". Reason: " << e.msg << endl;
		exit(-1);
	}

	//CHECK ERROR MATRICES SIZE
	for (int i = 0; i < testSample.size(); i++)
	{
		if (testSample[i].size().empty())
			cout << "Test Errror: " << testLabel[i] << endl;
	}

	// vecto predict
	vector<int> predicted_label;
	vector<double> predicted_confidence;
	string check = "";

	//Predict output
	for (int i = 0; i < testLabel.size(); i++)
	{
		int label = -1;
		double confident = 0.0;
		model->predict(testSample[i], label, confident);
		predicted_label.push_back(label);
		predicted_confidence.push_back(confident);
		if (predicted_label[i] != testLabel[i])
			check = "*";
		else
			check = "";
		cout << check << "predict: " << predicted_label[i] << " | " << testLabel[i] << " -- " << predicted_confidence[i] << endl;
	}
	cout << "Threshold: " << model->getThreshold() << endl;
	//Destroy mem
	//data.clear();
	testSample.clear();
	testLabel.clear();
	predicted_label.clear();
	predicted_confidence.clear();
	model->clear();
	return 0;
}