#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>
#include <fmod.h>
#include <fmod.hpp>
#include <conio.h>


using namespace std;
using namespace cv;
using namespace FMOD;


Mat filtrarObjetos(Mat original, Mat filtrada);
Mat filtrarPalos(Mat original, Mat filtrada, LPCWSTR x);
Mat pisarPedal(Mat original, Mat filtrada);
//El color verde claro para las baquetas
Scalar mini(150, 210, 130);
Scalar maxi(190, 245, 170);
bool pisado = false;


double hu[7];
Scalar sample_mean;
Scalar sample_standard_deviation;

//Programa principal
int main() {
	VideoCapture c;

	c.open(0);
	if (!c.isOpened()) {
		cout << "Error al tratar de inicializar la camara" << endl;
	}
	else {
		Mat frame;
		Mat frameHSV;
		Mat frameBlur;
		namedWindow("Original", 1);
		moveWindow("Original", 50, 100);
		namedWindow("Filtrado", 1);
		moveWindow("Filtrado", 700, 100);
		

		Mat frameMorph;
		while (true) {
			c >> frame;

			cvtColor(frame, frameHSV, CV_BGR2GRAY);
			blur(frameHSV, frameBlur, Size(3, 3));

			//threshold(frameBlur, frameMorph, 180, 255, CV_THRESH_OTSU);
			threshold(frameBlur, frameMorph, 180, 255, 3); //IMPORTANTE PARA VER PUNTOS -- comentar despues

			imshow("Filtrado", frameMorph);
			frame = filtrarObjetos(frame, frameMorph); //IMPORTANTE PARA VER PUNTOS -- comentar cuando se tengan las areas de interes

			imshow("Original", frame);

			//Codigo para dibujar las piezas
			Mat canvas = frame.clone();
			const int thickness = 3;
			const Scalar line_color(0, 255, 255);
			//China
			const Point up_leftChina(100, 60);
			const Point bottom_rightChina(20, 90);
			rectangle(canvas, up_leftChina, bottom_rightChina, line_color, thickness);
			const Rect roiChina(up_leftChina, bottom_rightChina);
			Mat sampleChina = frame(roiChina);

			Mat sampleChina2 = sampleChina.clone();
			inRange(sampleChina, mini, maxi, sampleChina2);
			sampleChina = filtrarPalos(sampleChina, sampleChina2, TEXT("audio\\China.wav"));

			imshow("China", sampleChina);
			//Hat
			const Point up_leftHat(45, 200);
			const Point bottom_rightHat(150, 250);
			rectangle(canvas, up_leftHat, bottom_rightHat, line_color, thickness);
			const Rect roiHat(up_leftHat, bottom_rightHat);
			Mat sampleHat = frame(roiHat);

			Mat sampleHat2 = sampleHat.clone();
			inRange(sampleHat, mini, maxi, sampleHat2);
			if (pisado)
			{
				sampleHat = filtrarPalos(sampleHat, sampleHat2, TEXT("audio\\Hat.wav"));
			}
			else
			{
				sampleHat = filtrarPalos(sampleHat, sampleHat2, TEXT("audio\\Hat2.wav"));
			}

			imshow("Hat", sampleHat);
			//Snare
			const Point up_leftSnare(165, 233);
			const Point bottom_rightSnare(265, 296);
			rectangle(canvas, up_leftSnare, bottom_rightSnare, line_color, thickness);
			const Rect roiSnare(up_leftSnare, bottom_rightSnare);
			Mat sampleSnare = frame(roiSnare);

			Mat sampleSnare2 = sampleSnare.clone();
			inRange(sampleSnare, mini, maxi, sampleSnare2);
			sampleSnare = filtrarPalos(sampleSnare, sampleSnare2, TEXT("audio\\Snare.wav"));

			imshow("Snare", sampleSnare);
			//Tom1
			const Point up_leftTom1(280, 200);
			const Point bottom_rightTom1(340, 300);
			rectangle(canvas, up_leftTom1, bottom_rightTom1, line_color, thickness);
			const Rect roiTom1(up_leftTom1, bottom_rightTom1);
			Mat sampleTom1 = frame(roiTom1);

			Mat sampleTom12 = sampleTom1.clone();
			inRange(sampleTom1, mini, maxi, sampleTom12);
			sampleTom1 = filtrarPalos(sampleTom1, sampleTom12, TEXT("audio\\Tom1.wav"));

			imshow("Tom1", sampleTom1);
			//Tom2
			const Point up_leftTom2(350, 200);
			const Point bottom_rightTom2(410, 300);
			rectangle(canvas, up_leftTom2, bottom_rightTom2, line_color, thickness);
			const Rect roiTom2(up_leftTom2, bottom_rightTom2);
			Mat sampleTom2 = frame(roiTom2);

			Mat sampleTom22 = sampleTom2.clone();
			inRange(sampleTom2, mini, maxi, sampleTom22);
			sampleTom2 = filtrarPalos(sampleTom2, sampleTom22, TEXT("audio\\Tom2.wav"));

			imshow("Tom2", sampleTom2);
			//crash
			const Point up_leftCrash(520, 200);
			const Point bottom_rightCrash(600, 250);
			rectangle(canvas, up_leftCrash, bottom_rightCrash, line_color, thickness);
			const Rect roiCrash(up_leftCrash, bottom_rightCrash);
			Mat sampleCrash = frame(roiCrash);

			Mat sampleCrash2 = sampleCrash.clone();
			inRange(sampleCrash, mini, maxi, sampleCrash2);
			sampleCrash = filtrarPalos(sampleCrash, sampleCrash2, TEXT("audio\\Crash.wav"));

			imshow("Crash", sampleCrash);
			//splash
			const Point up_leftSplash(560, 60);
			const Point bottom_rightSplash(630, 90);
			rectangle(canvas, up_leftSplash, bottom_rightSplash, line_color, thickness);
			const Rect roiSplash(up_leftSplash, bottom_rightSplash);
			Mat sampleSplash = frame(roiSplash);
			meanStdDev(sampleSplash, sample_mean, sample_standard_deviation);
			//VaEnOtros
			Mat sampleSplash2 = sampleSplash.clone();
			inRange(sampleSplash, mini, maxi, sampleSplash2);
			sampleSplash = filtrarPalos(sampleSplash, sampleSplash2, TEXT("audio\\Splash.wav"));
			//FinVaEnOtros
			imshow("Splash", sampleSplash);
			//imshow("Splash2", sampleSplash2);
			//piso
			const Point up_leftPiso(420, 250);
			const Point bottom_rightPiso(505, 350);
			rectangle(canvas, up_leftPiso, bottom_rightPiso, line_color, thickness);
			const Rect roiPiso(up_leftPiso, bottom_rightPiso);
			Mat samplePiso = frame(roiPiso);

			Mat samplePiso2 = samplePiso.clone();
			inRange(samplePiso, mini, maxi, samplePiso2);
			samplePiso = filtrarPalos(samplePiso, samplePiso2, TEXT("audio\\Piso.wav"));

			imshow("Piso", samplePiso);
			//pedal
			const Point up_leftPedal(200, 450);
			const Point bottom_rightPedal(280, 480);
			rectangle(canvas, up_leftPedal, bottom_rightPedal, line_color, thickness);
			const Rect roiPedal(up_leftPedal, bottom_rightPedal);
			Mat samplePedal = frame(roiPedal);

			Mat samplePedal2 = samplePedal.clone();
			inRange(samplePedal, mini, maxi, samplePedal2);
			//samplePedal = filtrarPalos(samplePedal, samplePedal2, TEXT("audio\\Pedal.wav"));
			
			samplePedal = pisarPedal(samplePedal, samplePedal2);

			imshow("Pedal", samplePedal);
			//bombo
			const Point up_leftBombo(380, 450);
			const Point bottom_rightBombo(460, 480);
			rectangle(canvas, up_leftBombo, bottom_rightBombo, line_color, thickness);
			const Rect roiBombo(up_leftBombo, bottom_rightBombo);
			Mat sampleBombo = frame(roiBombo);

			Mat sampleBombo2 = sampleBombo.clone();
			inRange(sampleBombo, mini, maxi, sampleBombo2);
			sampleBombo = filtrarPalos(sampleBombo, sampleBombo2, TEXT("audio\\Bombo.wav"));

			imshow("Bombo", sampleBombo);


			//LEVEL 1
			const string canvas_window = "Canvas";
			imshow(canvas_window, canvas);
			//END LEVEL 1

			//const Rect roiChina(up_leftChina, bottom_rightChina);
			//const Mat sampleChina = frame(roiChina);
			
			//Fin del codigo para dibujar las piezas

			if (waitKey(120) >= 0) {
				break;
			}
		}
		c.release();
	}
	destroyAllWindows();

	return 1;
}

//M�todo para saber si lo identificado es un l�piz
bool esLapiz(double momentArea, double hu0, double hu1, double hu2, double hu3,
	double hu4, double hu5, double hu6, double hu7) {
	if (momentArea > 4000 && momentArea < 15000 && hu1 > 0.6 && hu1 < 1.4 &&
		hu2 > -0.000 && hu2 < 0.007)
		return true;
	else
		return false;
}

typedef long long long64;
/**Don�t forget to use delete*/
LPCWSTR convStringToLPCWSTR(string String)
{
	char *cString = &String[0u];
	long64 size = strlen(cString);
	wchar_t* wString = new wchar_t[size];
	for (long64 i = 0; i < size; i++)
	{
		wString[i] = cString[i];
	}
	wString[size] = 0; //important, otherwise Windows will print whatever next in memmory until it finds a zero.
	return wString;
}

//M�todo para saber si lo identificado es un celular
bool esCelular(double momentArea, double hu0, double hu1, double hu2,
	double hu3, double hu4, double hu5, double hu6, double hu7) {
	if (momentArea > 40000 && momentArea < 100000 && hu1 > 0.0110 &&
		hu1 < 0.0155 && hu2 > -6.8e-05 && hu2 < 1.7)
		return true;
	else
		return false;
}

//M�todo para identificar todos los objetos umbralizados y mostrar su contorno
//en la imagen original
Mat filtrarObjetos(Mat original, Mat filtrada) {
	Mat frame = original.clone();
	vector<vector<Point> > contornos;
	vector<Vec4i> jerarquia;
	Scalar color;
	findContours(filtrada, contornos, jerarquia, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	vector<Moments>momentos(contornos.size());
	Point2f centro, centroObjeto;


	//cv::Moments mom;
	for (int i = 0; i < contornos.size(); i++) {
		momentos[i] = moments(contornos[i], false);

		//mom = cv::moments(contornos[i]);
		//HuMoments(mom, hu);

		if (momentos[i].m00>4000 && momentos[i].m00<100000) {
			centro = Point2f(momentos[i].m10 / momentos[i].m00,
				momentos[i].m01 / momentos[i].m00);

			centroObjeto = Point2f(momentos[i].m10 / momentos[i].m00,
				momentos[i].m01 / momentos[i].m00 + 15);

			color = Scalar(0, 255, 0);
			drawContours(frame, contornos, i, color, 2, 8, jerarquia, 0, Point());
			circle(frame, centro, 4, color, -1, 8, 0);
			ostringstream area;
			//ostringstream area0, area1, area2, area3;
			//ostringstream area4, area5, area6, area7;

			//Imprime las �reas de los objetos identificados
			area << "meDs(" << sample_mean << ", " << sample_standard_deviation << ")";

			Scalar colorText = Scalar(0, 0, 255);
			putText(frame, area.str(), centro, FONT_HERSHEY_PLAIN, 1,
				colorText, 1, 50);

		}
	}
	return frame;
}

Mat filtrarPalos(Mat sample, Mat filtrada, LPCWSTR x) {
	Mat frame = sample.clone();
	vector<vector<Point> > contornos;
	vector<Vec4i> jerarquia;
	//"C:\\Users\\MarQubit\\Documents\\Visual Studio 2015\\Projects\\OpenCV\\audio\\splash.wav"
	//LPCWSTR path = TEXT(x);
	findContours(filtrada, contornos, jerarquia, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	vector<Moments>momentos(contornos.size());
	for (int i = 0; i < contornos.size(); i++) {
		momentos[i] = moments(contornos[i], false);
		if (momentos[i].m00>0) {
			PlaySound(x, NULL, SND_ASYNC);
		}
	}
	return frame;
}

Mat pisarPedal(Mat sample, Mat filtrada) {
	Mat frame = sample.clone();
	vector<vector<Point> > contornos;
	vector<Vec4i> jerarquia;
	//"C:\\Users\\MarQubit\\Documents\\Visual Studio 2015\\Projects\\OpenCV\\audio\\splash.wav"
	//LPCWSTR path = TEXT(x);
	findContours(filtrada, contornos, jerarquia, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	vector<Moments>momentos(contornos.size());
	for (int i = 0; i < contornos.size(); i++) {
		momentos[i] = moments(contornos[i], false);
		if (momentos[i].m00>0) {
			pisado = true;
		}
		else
		{
			pisado = false;
		}
	}
	return frame;
}