#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;
#ifdef _WIN32
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif
//initial argument
void findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr)
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
int size_calc(const char *path, int ratio)
{
	//@ratio on scale to 10, for example ratio = 8 mean 80% training, 20% testing
	if (1 < ratio || ratio > 9)
	{
		cout << "Wrong size for ratio" << endl;
		return -1;
	}
	int file_count = 0;
	DIR *dir;
	struct dirent *entry;
	dir = opendir("path"); /* There should be error handling after this */
	if (dir != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_REG)
			{ /* If the entry is a regular file */
				file_count++;
			}
		}
		closedir(dir);
		return EXIT_SUCCESS;
	}
	else
	{
		perror("couln't open directory");
		return EXIT_FAILURE;
	}
}
void my_readdir(const char *path, FILE *fp_train, FILE *fp_test, int &id)
{
	//initial variable
	int sample_size = 20,
		size_train = (int)sample_size * 0.8,
		size_test = (int)sample_size * 0.2,
		size_vali = 0,
		count = 0;

	DIR *dir = opendir(path);
	if (dir != NULL)
	{
		struct dirent *ent;
		/* +2: One for directory separator, one for string terminator */
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type == DT_REG || ent->d_type == DT_DIR)
			{
				if (strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0)
				{
					char *newpath = (char *)malloc(strlen(path) + strlen(ent->d_name) + 2);
					strcpy(newpath, path);
					strcat(newpath, DIR_SEPARATOR);
					strcat(newpath, ent->d_name);
					if (ent->d_type == DT_DIR)
					{
						/* +2: One for directory separator, one for string terminator */
						/* Call myself recusively */
						my_readdir(newpath, fp_train, fp_test, id);
						free(newpath);

						// Tang id moi lan qua mot thu muc
						id++;
					}
					if (ent->d_type == DT_REG)
					{
						if (count < size_train)
						{
							fprintf(fp_train, "%s;%d\n", newpath, id);
							count++;
						}
						else
						{
							fprintf(fp_test, "%s;%d\n", newpath, id);
						}
					}
				}
			}
		}
	}
}
int main(int argc, const char *argv[])
{
	//Initial Variable for argument
	string input_database_path;
	string output_folder_path = "output";
	// Check for valid command line arguments, print usage
	// if no arguments were given.
	//message
	cout << "usage: " << argv[0] << endl;
	cout << "<input_database_path> <output_folder_path>" << endl;
	cout << "\n----------------------------------------------------------\n"
		 << endl;
	if (argc == 1)
	{
		cout << "No other arguments other than default application name, please input at least dataset path" << endl;
		exit(-1);
	}
	else if (argc == 2)
	{
		input_database_path = string(argv[1]);
		cout << "You only input database_path, is this okay (Y/N) ?" << endl;
	}
	else if (argc == 3)
	{
		input_database_path = string(argv[1]);
		output_folder_path = string(argv[2]);
		cout << "Is this okay (Y/N) ?" << endl;
	}
	else
		exit(1);
	cout << "- Database_folder_path: " << input_database_path << endl;
	cout << "- Output_folder_path: " << output_folder_path << endl;
	// Exist if user want
	char c[30];
	int attemp = 5;
	while (attemp > 0)
	{
		cin.clear();
		cin.getline(c, 30);
		if (strcmp("y", c) == 0 || strcmp("Y", c) == 0 || strcmp("yes", c) == 0 || strcmp("YES", c) == 0)
			break;
		else if (strcmp("n", c) == 0 || strcmp("N", c) == 0 || strcmp("NO", c) == 0 || strcmp("no", c) == 0 || strcmp("exit", c) == 0 || strcmp("EXIT", c) == 0)
		{
			cout << "Thoat chuong trinh" << endl;
			cin.clear();
			exit(-1);
		}
		else
		{
			cout << c << endl;
			cout << "Vui long nhap lai" << endl;
			attemp--;
		}
	}
	//Exit after 5 attempt
	if (attemp == 0)
		exit(0);

	/* create output folder */
	const int dir_err = mkdir(output_folder_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err)
	{
		printf("Error creating directory output, maybe already exit! \n");
	}
	char train_file[30],
		test_file[30];
	strcpy(train_file, "train.txt");
	strcpy(test_file, "test.txt");
	FILE *fp_train = fopen(train_file, "w");
	FILE *fp_test = fopen(test_file, "w");
	int idx = 0;

	if (input_database_path.back() == '/')
		input_database_path = input_database_path.substr(0, input_database_path.length() - 1);
	my_readdir(input_database_path.c_str(), fp_train, fp_test, idx);
	if (fp_train != NULL)
		fclose(fp_train);
	if (fp_test != NULL)
		fclose(fp_test);
	cout << "Success, make sure to check 3 files output" << endl;
	return 0;
}
