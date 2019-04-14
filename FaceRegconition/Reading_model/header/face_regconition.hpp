#ifndef _OPENCV_HEADER_
#define _OPENCV_HEADER_
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace cv::face;
using namespace std;
enum Algorithm {

	EIGENFACES = 1,

	FISHERFACES = 2,

	LBPHFACES = 3
};
class Data
{
public:
	Data();
	Data(int &, string &);
	Data(const int, const string);
	Data(int, const char *);
	~Data();
	int id;
	string name;
private:

};

Data::Data()
{
	id = -1;
	name = "";
}
Data::Data(int &i_id, string &i_name)
{
	id = i_id;
	name = i_name;
}
Data::Data(const int i_id, const string i_name) {
	id = i_id;
	name = i_name;
}
Data::Data(int i_id, const char *i_name) {
	id = i_id;
	name = string(i_name);
}

Data::~Data()
{

}
static bool Use_EigenFaceRecognizer(vector<Mat> &images, vector<int> &labels);
static bool Use_FisherFaceRecognizer(vector<Mat> &images, vector<int> &labels);
static bool Use_LBPHFaceRecognizer(vector<Mat> &images, vector<int> &labels);
static Mat norm_0_255(InputArray _src);
static void read_csv(string& filename, vector<Mat>& images, vector<string>& names, vector<int>& labels, char separator);
static bool checkUserEnter();
static bool checkUserEnter(int &attemp_number);
static int userChooseAlg();
string getName(int id, vector<Data> &data);
bool read_dataset_id(const string &path, vector<Data> &data);
#endif
