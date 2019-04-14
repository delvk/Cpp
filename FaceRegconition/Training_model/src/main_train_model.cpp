#include "../header/face_regconition.hpp"
#include "../header/func_face_regcontion.hpp"
int main(int argc, const char *argv[])
{
	//initial variables
	string path_training_set = "/home/jake/Desktop/Projects/Cpp/FaceRegconition/Training_model/res/training_set.csv";
	string output_folder = "output";

	//Checking arguments
	cout << "usage: " << argv[0] << endl;
	cout << "<path_training_set> <output_folder>" << endl;
	cout << "\n----------------------------------------------------------\n"
		 << endl;
	if (argc == 1)
	{
		cout << "No other arguments other than default application name, using default value (Y/N)?" << endl;
	}
	else if (argc == 2)
	{
		path_training_set = string(argv[1]);
		cout << "You only input database_path, is this okay (Y/N) ?" << endl;
	}
	else if (argc == 3)
	{
		path_training_set = string(argv[1]);
		output_folder = string(argv[2]);
		cout << "Is this okay (Y/N) ?" << endl;
	}
	else
		exit(1);
	cout << "- path_training_set: " << path_training_set << endl;
	cout << "- output_folder: " << output_folder << endl;
	if (!checkUserEnter())
	{
		cout << "Thoat chuong trinh, bye !" << endl;
		return 0;
	}
	// These vectors hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	// Read in the data. This can fail if no valid
	// input filename is given.
	try
	{
		read_csv(path_training_set, images, labels);
	}
	catch (const cv::Exception &e)
	{
		cerr << "Error opening file \"" << path_training_set << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		system("pause");
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1)
	{
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
	}
	//size 180x200;
	bool ok = true;
	for (int i = 0; i < images.size(); i++)
	{
		if (images[i].size().empty())
		{
			cout << "Test Errror: " << images[i] << endl;
			ok = false;
		}
	}
	if (!ok)
	{
		cout << "Sai biet kich thuoc ma tran trong tap du lieu, xin thu lai voi tap du lieu khac" << endl;
		system("pause");
		return -1;
	}

	/* create output folder */
	const int dir_err = mkdir(output_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err)
	{
		printf("Error creating directory output, maybe already exit!n");
	}

	/*---TRAINING---*/
	int what = -1;
	//cout << "012345678901234567890123456789";
	cout << "____________________________" << endl;
	cout << "Danh sach giai thuat:       |" << endl;
	cout << setw(2) << EIGENFACES << ". " << left << setw(24) << "Eigenfaces"
		 << "|" << endl
		 << right;
	cout << setw(2) << FISHERFACES << ". " << left << setw(24) << "Fisherfaces"
		 << "|" << endl
		 << right;
	cout << setw(2) << LBPHFACES << ". " << left << setw(24) << "LBPHFaces"
		 << "|" << endl
		 << right;
	for (int i = 0; i < 28; i++)
		cout << "_";
	cout << "\nNhap giai thuat ban muon: ";
	cin >> what;
	cout << "\nBan da chon giai thuat so " << what << endl;
	switch (what)
	{
		case EIGENFACES:
		{
			cout << "Training ....." << endl;
			if (Use_EigenFaceRecognizer(images, labels))
				cout << "Success" << endl;
			else
				cout << "Failed" << endl;
			break;
		}
		case FISHERFACES:
		{
			cout << "Training ....." << endl;
			if (Use_FisherFaceRecognizer(images, labels))
				cout << "Success" << endl;
			else
				cout << "Failed" << endl;
			break;
		}
		case LBPHFACES:
		{
			cout << "Training ....." << endl;
			if (Use_LBPHFaceRecognizer(images, labels))
				cout << "Success" << endl;
			else
				cout << "Failed" << endl;
			break;
		}
		default:
		{
			cout << "khong co giai thuat nao duoc chon, ket thuc chuong trinh" << endl;
			break;
		}
	}
	// Check for valid command line arguments, print usage
	// if no arguments were given.
	return 0;
}
