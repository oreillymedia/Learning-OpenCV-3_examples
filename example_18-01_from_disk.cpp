// Example 18-1. Reading a chessboard’s width and height, reading them from disk and calibrating 
// the requested number of views, and calibrating the camera 

// You need these includes for the function
// #include <windows.h>  // for windows systems
#include <dirent.h>     // for linux systems
#include <sys/stat.h>   // for linux systems
#include <iostream>     // cout
#include <algorithm>    // std::sort
#include <opencv2/opencv.hpp>

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

// Returns a list of files in a directory (except the ones that begin with a dot)
int readFilenames(vector<string>& filenames, const string& directory) {
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return;  // no files found
    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        filenames.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const string file_name = ent->d_name;
        const string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

        // filenames.push_back(full_file_name);  // returns full path
        filenames.push_back(file_name);  // returns just filename
    }
    closedir(dir);
#endif
    std::sort(filenames.begin(), filenames.end());  // optional, sort the filenames
    return(filenames.size());  // return how many we found
}  // GetFilesInDirectory

void help(const char **argv) {
    cout << "\n\n"
         << "Example 18-1 (from disk):\Enter a chessboard’s width and height,\n"
         << "              reading in a directory of chessboard images,\n"
         << "              and calibrating the camera\n\n"
         << "Call:\n" << argv[0] << " <1board_width> <2board_height> <3number_of_boards>"
         << " <4ms_delay_framee_capture> <5image_scaling_factor> <6path_to_calibration_images>\n\n"
         << "\nExample:\n"
         << "./example_18-01_from_disk 9 6 14 100 1.0 ../stereoData/\nor:\n"
         << "./example_18-01_from_disk 12 12 28 100 0.5 ../calibration/\n\n"
         << " * First it reads in checker boards and calibrates itself\n"
         << " * Then it saves and reloads the calibration matricies\n"
         << " * Then it creates an undistortion map and finaly\n"
         << " * It displays an undistorted image\n"
         << endl;
}


int main(int argc, const char *argv[]) {
    float image_sf = 0.5f;    // image scaling factor
    int delay = 250;          // miliseconds
    int board_w = 0;
    int board_h = 0;

    if (argc != 7) {
        cout << "\nERROR: Wrong number (" << argc - 1
             << ") of arguments, should be (6) input parameters\n";
        help(argv);
        return -1;
    }

    board_w = atoi(argv[1]);
    board_h = atoi(argv[2]);
    int n_boards = atoi(argv[3]);  // how many boards max to read
    delay = atof(argv[4]);         // milisecond delay
    image_sf = atof(argv[5]);
    int board_n = board_w * board_h;  // number of corners
    cv::Size board_sz = cv::Size(board_w, board_h);  // width and height of the board

    string folder = argv[6];
    cout << "Reading in directory " << folder << endl;
    vector<string> filenames;
    int num_files = readFilenames(filenames, folder);
    cout << "   ... Done. Number of files = " << num_files << "\n" << endl;

    // PROVIDE PPOINT STORAGE
    //
    vector<vector<cv::Point2f> > image_points;
    vector<vector<cv::Point3f> > object_points;

    /////////// COLLECT //////////////////////////////////////////////
    // Capture corner views: loop through all calibration images
    // collecting all corners on the boards that are found
    //
    cv::Size image_size;
    int board_count = 0;
    for (size_t i = 0; (i < filenames.size()) && (board_count < n_boards); ++i) {
        cv::Mat image, image0 = cv::imread(folder + filenames[i]);
        board_count += 1;
        if (!image0.data) {  // protect against no file
            cerr << folder + filenames[i] << ", file #" << i << ", is not an image" << endl;
            continue;
        }
        image_size = image0.size();
        cv::resize(image0, image, cv::Size(), image_sf, image_sf, cv::INTER_LINEAR);

        // Find the board
        //
        vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(image, board_sz, corners);

        // Draw it
        //
        drawChessboardCorners(image, board_sz, corners, found);  // will draw only if found

        // If we got a good board, add it to our data
        //
        if (found) {
            image ^= cv::Scalar::all(255);
            cv::Mat mcorners(corners);

            // do not copy the data
            mcorners *= (1.0 / image_sf);

            // scale the corner coordinates
            image_points.push_back(corners);
            object_points.push_back(vector<cv::Point3f>());
            vector<cv::Point3f> &opts = object_points.back();

            opts.resize(board_n);
            for (int j = 0; j < board_n; j++) {
                opts[j] = cv::Point3f(static_cast<float>(j / board_w),
                                      static_cast<float>(j % board_w), 0.0f);
            }
            cout << "Collected " << static_cast<int>(image_points.size())
                 << "total boards. This one from chessboard image #"
                 << i << ", " << folder + filenames[i] << endl;
        }
        cv::imshow("Calibration", image);

        // show in color if we did collect the image
        if ((cv::waitKey(delay) & 255) == 27) {
            return -1;
        }
    }

    // END COLLECTION WHILE LOOP.
    cv::destroyWindow("Calibration");
    cout << "\n\n*** CALIBRATING THE CAMERA...\n" << endl;

    /////////// CALIBRATE //////////////////////////////////////////////
    // CALIBRATE THE CAMERA!
    //
    cv::Mat intrinsic_matrix, distortion_coeffs;
    double err = cv::calibrateCamera(
      object_points, image_points, image_size, intrinsic_matrix,
      distortion_coeffs, cv::noArray(), cv::noArray(),
      cv::CALIB_ZERO_TANGENT_DIST | cv::CALIB_FIX_PRINCIPAL_POINT);

    // SAVE THE INTRINSICS AND DISTORTIONS
    cout << " *** DONE!\n\nReprojection error is " << err
       << "\nStoring Intrinsics.xml and Distortions.xml files\n\n";
    cv::FileStorage fs("intrinsics.xml", cv::FileStorage::WRITE);
    fs << "image_width" << image_size.width << "image_height" << image_size.height
     << "camera_matrix" << intrinsic_matrix << "distortion_coefficients"
     << distortion_coeffs;
    fs.release();

    // EXAMPLE OF LOADING THESE MATRICES BACK IN:
    fs.open("intrinsics.xml", cv::FileStorage::READ);
    cout << "\nimage width: " << static_cast<int>(fs["image_width"]);
    cout << "\nimage height: " << static_cast<int>(fs["image_height"]);
    cv::Mat intrinsic_matrix_loaded, distortion_coeffs_loaded;
    fs["camera_matrix"] >> intrinsic_matrix_loaded;
    fs["distortion_coefficients"] >> distortion_coeffs_loaded;
    cout << "\nintrinsic matrix:" << intrinsic_matrix_loaded;
    cout << "\ndistortion coefficients: " << distortion_coeffs_loaded << "\n" << endl;

    // Build the undistort map which we will use for all
    // subsequent frames.
    //
    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(intrinsic_matrix_loaded, distortion_coeffs_loaded,
                              cv::Mat(), intrinsic_matrix_loaded, image_size,
                              CV_16SC2, map1, map2);

    ////////// DISPLAY //////////////////////////////////////////////
    cout << "*****************\nPRESS A KEY TO SEE THE NEXT IMAGE, ESQ TO QUIT\n"
         << "****************\n" << endl;
    board_count = 0;  // resent max boards to read
    for (size_t i = 0; (i < filenames.size()) && (board_count < n_boards); ++i) {
        cv::Mat image, image0 = cv::imread(folder + filenames[i]);
        ++board_count;
        if (!image0.data) {  // protect against no file
            cerr << folder + filenames[i] << ", file #" << i << ", is not an image" << endl;
            continue;
        }
        // Just run the camera to the screen, now showing the raw and
        // the undistorted image.
        //
        cv::remap(image0, image, map1, map2, cv::INTER_LINEAR,
            cv::BORDER_CONSTANT, cv::Scalar());
        cv::imshow("Original", image0);
        cv::imshow("Undistorted", image);
        if ((cv::waitKey(0) & 255) == 27) {
            break;
        }
    }
    return 0;
}
