/*
 * sudokuDetect.h
 *
 *  Created on: 03-May-2021
 *      Author: abrar
 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;


vector<Point2f> extractGrid(const Mat orig_img)
{
	Mat img = orig_img.clone();
	Mat blurred,gray,threshed;
	imshow("original",img);
	GaussianBlur(img,blurred,Size(5,5),0);
	imshow("blurred",blurred);
	cvtColor( blurred, gray, cv::COLOR_BGR2GRAY ); // Convert the image to Gray

	/*namedWindow("thresholding", WINDOW_AUTOSIZE ); // Create a window to display results
	int threshold_value = 0;
    createTrackbar( "Thresholding value",
                    "thresholding", &threshold_value,
                    255); // Create a Trackbar to choose Threshold value
    while( true )
    {
        threshold( gray, threshed, threshold_value, 255, 1); //1: Binary Inverted
        imshow( "thresholding", threshed);
        waitKey(10);
        //threashold value is 150
    }*/

	//threshold( gray, threshed, 150, 255, cv::THRESH_BINARY_INV ); //1: Binary Inverted
	adaptiveThreshold( gray, threshed,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY_INV,101,5 ); //1: Binary Inverted
	imshow("thresholded",threshed);

	dilate(threshed,threshed,cv::getStructuringElement( cv::MORPH_ELLIPSE,Size( 3, 3 ),Point( 2, 2 )) );
	imshow("dilated",threshed);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( threshed, contours,hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<Point2f> ans;
    Mat boxPts;
    float maxArea = 0;
    for(int i=0; i<contours.size(); i++)
    {
    	double a = contourArea( contours[i],false);
    	if(a<10000) continue;
    	double p = arcLength (contours[i],true);
    	float chk = a/pow(p/4,2);
    	if(chk<0.9 || chk>1.1) continue;
    	if(a < maxArea) continue;
    	else maxArea = a;
    	cout<<"chk value: "<<chk<<endl;
    	RotatedRect r = minAreaRect(contours[i]);
    	cv::boxPoints(r, boxPts);
    	Mat dummy = img.clone(),dummy2;
    	rectangle(dummy, r.boundingRect(),  Scalar(0,255,0),2, 8,0);
    	cv::resize(dummy,dummy2,Size(720,720));
    	imshow("contours",dummy2);
    }
    waitKey(1000);

    cout<<boxPts.size()<<endl;
    cout<<"boxPts"<<" "<<boxPts<<endl;
    if(boxPts.size() != Size(2,4)){
    	cout<<"grid not found"<<endl;
    	return ans;
    }
    else
    	cout<<"grid found"<<endl;
    for(int i=0;i<4; i++)
    {
    	int x = boxPts.at<float>(i,0);
    	int y = boxPts.at<float>(i,1);
    	ans.push_back(Point(x,y));
    }
    destroyAllWindows();
	return ans;

}

Mat getGrid(const vector<Point2f>src, const Mat& img){
	int grid[4][2] = {{0,252},{0,0},{252,0},{252,252}};
	vector<Point2f>dst(4);
	Mat matrix,gridImg;
	for(int i=0;i<4;i++)
		dst[i]=cv::Point2f(grid[i][0],grid[i][1]);
	matrix=cv::getPerspectiveTransform(src,dst);
	cv::warpPerspective(img, gridImg, matrix,Size(252,252));
	imshow("grid image",gridImg);
	waitKey(1000);
	return gridImg;
}


void getAttach(Mat& img,const Mat& soln,const vector<Point2f>dst)
{
	int grid[4][2] = {{0,252},{0,0},{252,0},{252,252}};
	vector<Point2f>src(4);
	Mat matrix,gridImg;
	for(int i=0;i<4;i++)
		src[i]=cv::Point2f(grid[i][0],grid[i][1]);
	matrix=cv::getPerspectiveTransform(src,dst);
	cv::warpPerspective(soln, gridImg, matrix,img.size());
	cv::addWeighted(img,0.7,gridImg,1,1,img);
}

vector<cv::Mat> getDigits(const Mat img)
{
	vector<Mat> ans;
	for(int i=0; i<252; i += 28){
		for(int j=0; j<252; j += 28){
			Mat digit = img(Rect(j, i, 28, 28));
			ans.push_back(digit);
		}
	}
	return ans;
}
