#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//initial argument
string train_folder = "Data_Training";
string vali_folder = "Data_Validation";
string test_folder = "Data_Testing";
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}
int count_size(const filesystem::path path) {
	int temp = 0;
	for (const auto & entry : filesystem::directory_iterator(path)) {
		temp++;
	}
	return temp;
}

void write_csv(const string &input, const string &output) {
	int id = -1;
	int size_each_example = 0;
	int size_train=0,
		size_test=0,
		size_vali=0;
	int train_idx = 0;
	string s_path = "";

	ofstream file_train, file_test, file_dataset;

	string path1 = string(output) + "/training_set.csv";
	file_train.open(path1);

	string path2 = string(output) + "/testing_set.csv";
	file_test.open(path2);

	string path3 = string(output) + "/dataset_id.csv";
	file_dataset.open(path3);

	for (const auto & entry : filesystem::recursive_directory_iterator(input)) {
		if (filesystem::is_directory(entry.path())) {
			if (id < 0)
			{
				//mac dinh tinh size tu thu muc dau tien no tim dc
				size_each_example = count_size(entry.path());
				//chia so training va test
				size_train = (int)size_each_example * 0.8;
				size_test = (int) size_each_example * 0.2;
				cout << "size train: " << size_train << endl;
				cout << "size test: " << size_test << endl;
			}
			id++;
			train_idx = 0; //reset bien train_idx moi khi qua mot thu muc moi
			string name = entry.path().filename().string();
			cout << "new: " << id << "; "<< name << endl;
			file_dataset <<id<<";"<< name<< endl;
		}
		else 
		{ 
			s_path = filesystem::absolute(entry).string();
			findAndReplaceAll(s_path, "\\", "/"); //chuan hoa path
			if (train_idx < size_train) { //nho hon so phai training thi ghi vao file training
				cout << "training: " << s_path << ";" << id << endl;
				file_train << s_path << ";" << id << endl;

			}
			else { //nguoc lai thi ghi testing
				cout << "testing: " << s_path << ";" << id << endl;
				file_test << s_path << ";" << id << endl;
			}
			train_idx++;
		}
	}
	file_train.close();
	file_test.close();
}

int main(int argc, const char *argv[]) {
	//Initial Variable for argument
	string input_database_path = "database/faces94/female";
	string output_folder_path = "Data_Output";
	// Check for valid command line arguments, print usage
	// if no arguments were given.
	//message
	cout << "usage: " << argv[0] << endl;
	cout << "<input_database_path> <output_folder_path>" << endl;
	cout << "\n----------------------------------------------------------\n" << endl;
	if (argc == 1) {
		cout << "No other arguments other than default application name, using default value (Y/N)?" << endl;
	}
	else if (argc == 2) {
		input_database_path = string(argv[1]);
		cout << "You only input database_path, is this okay (Y/N) ?" << endl;
	}
	else if (argc == 3) {
		input_database_path = string(argv[1]);
		output_folder_path = string(argv[2]);
		cout << "Is this okay (Y/N) ?" << endl;
	}
	else exit(1);
	cout << "- Database_folder_path: " << input_database_path << endl;
	cout << "- Output_folder_path: " << output_folder_path << endl;
	//Exist if user want
	char c[30];
	int attemp = 5;
	while (attemp>0) {
		cin.clear();
		cin.getline(c, 30);
		if (strcmp("y",c) == 0 || strcmp("Y", c) == 0 || strcmp("yes", c) == 0 || strcmp("YES", c) == 0) break;
		else if(strcmp("n", c) == 0 || strcmp("N", c) == 0 || strcmp("NO", c) == 0 || strcmp("no", c) == 0 || strcmp("exit", c) == 0 || strcmp("EXIT", c) == 0){
			cout << "Thoat chuong trinh" << endl;
			cin.get();
			exit(-1);
		}
		else {
			cout << c << endl;
			cout << "Vui long nhap lai" << endl;
			attemp--;
		}
	}
	//Exit after 5 attempt
	if (attemp == 0) exit(0);

	//Check folder exist 
	if (CreateDirectory(output_folder_path.c_str(), NULL))
	{
		cout << "Created folders Output " << output_folder_path << " !!!" << endl;
	}
	else
	{
		// Failed to create directory.
		cout << "Can't create folder Output, maybe it already exist" << endl;
	}
	//start writing - Prepare data
	write_csv(input_database_path, output_folder_path);
	cout << "Done, make sure to check 3 files exist in " << output_folder_path << endl;
	system("pause");
	return 0;
}
