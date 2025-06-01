#include "faceDetection.h"

faceDetection::faceDetection() {
	if (!faceCascade.load("face-detection/haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading face cascade\n";
    }
}

faceDetection::~faceDetection() {}

cv::Mat faceDetection::detectFromImage(cv::Mat image) {

  cv::Mat backup = image.clone();

  // Create a black and white copy
  // Checking that the image is not already in black and white
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  // Better for contrast apparently
  cv::equalizeHist(gray, gray);

  // List of the detected faces
  std::vector<cv::Rect> faces;
  faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

  // Rectangles displayed around a face
  for (const auto& face : faces) {
        cv::rectangle(image, face, cv::Scalar(255, 0, 0), 2);
  }

  // Creation of the window
  std::string windowName = "FaceDetection";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Display the image
  cv::imshow(windowName, image);

  // Wait for user input
  cv::waitKey(0);

  // Close the window
  cv::destroyWindow(windowName);

  return image;

}

cv::Mat faceDetection::detectFromWebcam() {


  cv::VideoCapture cap = cv::VideoCapture(0);

  // Check that webcam is open
  if(!cap.isOpened()){
    std::cout << "Error opening video stream or file" << std::endl;
  }

  cv::Mat result;
  std::string windowName = "FaceDetection";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  while(true) {


      cv::Mat frame;

    // Capture frame-by-frame
      cap >> frame;

      // If the frame is empty, break immediately
      if(frame.empty()){
        break;
      }

      cv::Mat gray;
      if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
      } else {
        gray = frame;
      }

      // Apparently better for contrast
      cv::equalizeHist(gray, gray);

      // Creation of the list of rectangles
      std::vector<cv::Rect> faces;
      faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

      // Rectangles around the faces
      for (const auto& face : faces) {
        cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
      }

      // Display the resulting frame
      imshow(windowName, frame);

      // Press  ESC on keyboard to exit
      char c = (char) cv::waitKey(25);
      if(c==27) {
        result = frame;
        break;
      }

  }

  // Close the window
  cv::destroyWindow(windowName);

  return result;
}

cv::Mat faceDetection::detectFacesGUI(cv::Mat image) {
  cv::Mat result = image.clone();

  // Check if the image is in black and white and convert it if not
  cv::Mat gray;
  if (result.channels() == 3) {
    cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = result;
  }

  // For better contrast
  cv::equalizeHist(gray, gray);

  // List of the rectangles
  std::vector<cv::Rect> faces;
  faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

  for (const auto& face : faces) {
    cv::rectangle(result, face, cv::Scalar(255, 0, 0), 2);
  }

  return result;
}

