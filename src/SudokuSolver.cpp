//============================================================================
// Name        : SudokuSolver.cpp
// Author      : Abrar Hossain
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <opencv2/opencv.hpp>
#include "solver.hpp"
#include "sudokuDetect.h"
#include "createDataForTrain.h"
#include "CPython.h"
using namespace cv;
using namespace std;
int main() {
	Mat image;
	VideoCapture cap(0);
	cap >> image;
	if (!image.data) {
		printf("No image data \n");
		return -1;
	}
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);
	waitKey(1000);

	Mat img = cv::imread("./data/test/image1005.jpg");

	vector<Point2f> quad = extractGrid(img);

	Mat grid = getGrid(quad,img);

	/* TO-DO
		 * implement sudoku detector - DONE
		 * implement digit recogniser
		 */

	//getAttach(img,grid,quad);
	//imshow("new real image",img);
	//waitKey(5000);

	//TrainDataGrid data("./data/train.csv");
	//data.saveImages(true);

	//TrainDataDigits ddata("./data/train/grids","./data/train/datasets","./data/train","./data/train.csv");
	//ddata.saveImages();

	vector<Mat> digits = getDigits(grid);
	CPython cp;
	vector<int> pred = cp.predict(digits);
	cout<<"size: "<<pred.size()<<endl;
	for(int i=0;i<pred.size();i++) cout<<pred[i]<<" "; cout<<endl;

	int dum[9][9] = {{5,3,0,0,7,0,0,0,0},
					 {6,0,0,1,9,5,0,0,0},
					 {0,9,8,0,0,0,0,6,0},
					 {8,0,0,0,6,0,0,0,3},
					 {4,0,0,8,0,3,0,0,1},
					 {7,0,0,0,2,0,0,0,6},
					 {0,6,0,0,0,0,2,8,0},
					 {0,0,0,4,1,9,0,0,5},
					 {0,0,0,0,8,0,0,7,9}};
	vector< vector<int> > mat(9,vector<int>(9,0));
	FOR(i,9) FOR(j,9) mat[i][j] = dum[i][j];
	Solver s;
	s.setMat(mat);
	s.solve(0);
	mat = s.getMat();

	FOR(i,9){
		FOR(j,9) cout<<mat[i][j]<<" ";
		cout<<endl;
	}
	return 0;
}
