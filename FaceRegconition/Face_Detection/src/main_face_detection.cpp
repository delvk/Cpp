// CPP program to detects face in a video

// Include required header files from OpenCV directory
#include "../header/face_regconition.hpp"
#include "../header/func_face_regcontion.hpp"
#include <opencv2/core/types_c.h>
// Function for Face Detection
void detectAndDraw(Mat &img, CascadeClassifier &cascade, double scale, int id);
string cascadeName;

int main(int argc, const char **argv)
{

	//Get Parameters
	cout << "usage: " << argv[0] << endl;
	cout << "<pre-trained_model> <test_file_1> <test_file_2> ...." << endl;
	cout << "\n----------------------------------------------------------\n\n";
	if (argc == 1)
	{
		cout << "Please point to pre-trained model facefont" << endl;
		exit(-1);
	}
	else
	{
		cout << argv[1] << endl;
	}

	// VideoCapture class for playing video for which faces to be detected
	VideoCapture capture;
	Mat frame, image;

	// PreDefined trained XML classifiers with facial features
	CascadeClassifier cascade;
	double scale = 1;

	// Change path before execution
	cascade.load(string(argv[1]));
	for (int i = 2; i < argc; i++)
	{
		string test_path = string(argv[i]);
		image = imread(test_path, IMREAD_COLOR);
		int id = i - 2;
		detectAndDraw(image, cascade, scale, id);
	}
	waitKey(0);
	return 0;
}

void detectAndDraw(Mat &img, CascadeClassifier &cascade, double scale, int id)
{
	vector<Rect> faces;
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY); // Convert to Gray Scale

	double fx = 1 / scale;

	// Resize the Grayscale Image
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
	//equalizeHist(smallImg, smallImg);

	cascade.detectMultiScale(smallImg, faces, 1.33, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// This code help you decide image scale, your welcome :))

	//double var = 1.1;
	//double best_suit = 0;
	//int temp = 0;

	//for (int i = 0; i < 1000; i++) {
	//	var += 0.01;
	//	cascade.detectMultiScale(smallImg, faces, var, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	//	if (faces.size() == 17) {
	//		temp = faces.size();
	//		break;
	//	}
	//	else {
	//		if (abs(18 - faces.size()) < abs(18 - temp)) {
	//			best_suit = var;
	//			temp = faces.size();
	//		}
	//	}
	//}
	//cout << "Var: " << var << endl;
	//cout << "Count: " << temp<< endl;

	for (size_t i = 0; i < faces.size(); i++)
	{
		Mat smallImgROI;
		smallImgROI = smallImg(faces[i]);
		resize(smallImgROI, smallImgROI, Size(180, 200));
		//hello=model->predict(smallImgROI);
		Rect r = faces[i];
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = Scalar(255, 255, 0); // Color for Drawing tool
		double aspect_ratio = (double)r.width / r.height;
		rectangle(img, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
				  cvPoint(cvRound((r.x + r.width - 1) * scale),
						  cvRound((r.y + r.height - 1) * scale)),
				  color, 3, 8, 0);
		//imshow("result", smallImgROI);
	}
	imshow(to_string(id), img);
	cout << "ID: " << id << " - "
		 << "Count: " << faces.size() << endl;
}