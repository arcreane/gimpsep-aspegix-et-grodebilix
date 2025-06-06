#include "faceDetection.h"

faceDetection::faceDetection() {
	if (!faceCascade.load("face-detection/haarcascade_frontalface_default.xml")) {
        std::cerr << "Error loading face cascade\n";
    }
}

faceDetection::~faceDetection() {}

cv::Mat faceDetection::detectFromImage(cv::Mat image) {

  cv::Mat backup = image.clone();

  // On crée une copie en noir et blanc
  // Vérif de sécurité pour éviter les bugs quand veux transformer une image noir & blanc en noir & blanc
  cv::Mat gray;
  if (image.channels() == 3) {
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = image;
  }

  // Askip c'est mieux pour le contrast
  cv::equalizeHist(gray, gray);

  // Liste des visages détectés
  std::vector<cv::Rect> faces;
  faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

  // Rectangle qu'on affiche autour d'un visage
  for (const auto& face : faces) {
        cv::rectangle(image, face, cv::Scalar(255, 0, 0), 2);
  }

  // Création de la fenêtre
  std::string windowName = "FaceDetection";
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

  // Affichage de l'image
  cv::imshow(windowName, image);

  // Attente de l'input de l'utilisateur
  cv::waitKey(0);

  // Fermer la fenêtre
  cv::destroyWindow(windowName);

  return image;

}

cv::Mat faceDetection::detectFromWebcam() {


  cv::VideoCapture cap = cv::VideoCapture(0);

  // Vérifier que la caméra est ouverte
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

      // Askip c'est mieux pour le contrast
      cv::equalizeHist(gray, gray);

      // Création de la liste des visages détectés (les rectangles autours mais c'est pareil sayez là)
      std::vector<cv::Rect> faces;
      faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

      // Rectangles autour des visages
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

  // Fermeture de la fenêtre
  cv::destroyWindow(windowName);

  return result;
}