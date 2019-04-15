#include "../header/face_regconition.hpp"
#include <dirent.h>
// Function for Face Detection
void detectAndCrop(Mat &img, CascadeClassifier &cascade, double scale, string &name, int &idx);
void detect_and_crop(const char *image_path, string &output_path, string &name, int &idx);
void dir_inter(string &path, string &output_path, string &name);
int main(int argc, const char **argv)
{
    //parameter
    cout << "usage: " << argv[0] << endl;
    cout << "<label_name>" << endl;
    cout << "\n----------------------------------------------------------\n"
         << endl;
    if (argc == 1)
    {
        cout << "Please declare example label" << endl;
        exit(-1);
    }
    else if (argc == 2)
    {
        cout << "label: " << argv[1] << endl;
    }
    else if (argc > 2)
        cout << "Only take first argument" << endl;
    else
        exit(-1);

    //Initial Variable
    string name = string(argv[1]);                                                           //folder contain raw images
    string folder_path = "res";                                                              //your name
    string output_folder = "output";                                                         //parent output folder
    const int dir_err = mkdir(output_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //create output folder if not exist

    Mat frame, image;

    double scale = 1;
    int idx = 0;

    dir_inter(folder_path, output_folder, name);
    return 0;
}
void detect_and_crop(const char *image_path, string &output_path, string &name, int &idx)
{
    vector<Rect> faces;
    Mat image, image_cropped, gray;
    double scale = 1;
    double fx = 1 / scale;

    image = imread(image_path, IMREAD_COLOR);
    cvtColor(image, gray, COLOR_BGR2GRAY); // Convert to Gray Scale

    // PreDefined trained XML classifiers with facial features
    CascadeClassifier cascade;
    cascade.load("/usr/local/installation/OpenCV/share/opencv4/haarcascades/haarcascade_frontalface_default.xml");

    resize(gray, gray, Size(), fx, fx, INTER_LINEAR);
    cascade.detectMultiScale(gray, faces, 1.2, 5, 0 | CASCADE_SCALE_IMAGE, Size(40, 40));
    for (size_t i = 0; i < faces.size(); i++)
    {
        image_cropped = image(faces[i]);
        resize(image_cropped, image_cropped, Size(180, 200));
        vector<int> compression_params;
        compression_params.push_back(100);
        compression_params.push_back(9);
        try
        {
            string new_name = output_path + "/" + name + "." + to_string(idx++) + ".jpg";
            if (imwrite(new_name, image_cropped, compression_params))
                cout << "created: " << new_name << endl;
        }
        catch (runtime_error &ex)
        {
            fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
        }
    }
}

void dir_inter(string &input_path, string &output_path, string &name)
{
    const char *input_path_c = input_path.c_str();
    DIR *dir = opendir(input_path_c);
    if (dir != NULL)
    {
        struct dirent *ent;
        int idx = 0;
        /* +2: One for directory separator, one for string terminator */
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG || ent->d_type == DT_DIR)
            {
                if (strcmp(ent->d_name, "..") != 0 && strcmp(ent->d_name, ".") != 0)
                {
                    char *newpath = (char *)malloc(strlen(input_path_c) + strlen(ent->d_name) + 2);
                    strcpy(newpath, input_path_c);
                    strcat(newpath, "/");
                    strcat(newpath, ent->d_name);
                    if (ent->d_type == DT_REG)
                    {
                        detect_and_crop(newpath, output_path, name, idx);
                    }
                }
            }
        }
    }
}