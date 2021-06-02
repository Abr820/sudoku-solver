/*
 * createDataForTrain.cpp
 *
 *  Created on: 03-May-2021
 *      Author: abrar
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;

class TrainDataGrid
{
	string inputfile;
	fstream fin;
	vector<string> files;
public:
	TrainDataGrid(){

	}
	TrainDataGrid(string f){
		this->inputfile = f;
		read(f);
	}

	void read(string f){
		this->inputfile = f;
		this->fin.open(f,ios::in);
		string file,temp;
		while (!fin.eof()) {
			getline(fin,file);
			this->files.push_back(file);
		}
	}

	void saveImages(bool train){
		string infiledir;
		if(train)
			infiledir = "./data/train/";
		else
			infiledir = "./data/test/";

		string outfiledir = infiledir+"grids/";
		for(int i=0; i<files.size();i++){
			if(files[i].size()<=4) continue;
			string infile = infiledir + files[i];
			string outfile = outfiledir + files[i];
			cout<<infile<<" "<<outfile<<endl;
			Mat img = cv::imread(infile);
			vector<Point2f> quad = extractGrid(img);
			if(quad.size()<=1) continue;
			Mat grid = getGrid(quad,img);
			/*int a;
			cout<<"\nenter rotation:";cin>>a;
			cv::Point2f pc(grid.cols/2., grid.rows/2.);
			cv::Mat r = cv::getRotationMatrix2D(pc, -90*a, 1.0);
			cv::warpAffine(grid, grid, r, grid.size()); // what size I should use?*/
			cv::imwrite(outfile,grid);
		}
	}
};


class TrainDataDigits
{
	string inputfiledir,outputfiledir,csvfile,gtdir;
	fstream fin;
	vector<string> images;
public:
	TrainDataDigits(string indir, string outdir,string gt,string f){
		this->inputfiledir = indir;
		this->outputfiledir = outdir;
		this->gtdir = gt;
		this->csvfile = f;
		readcsv(f);
	}

	void readcsv(string f){
		this->csvfile = f;
		this->fin.open(f,ios::in);
		string file,temp;
		while (!fin.eof()) {
			getline(fin,file);
			this->images.push_back(file);
		}
	}

	vector<int> readDat(string image)
	{
		string infile = this->gtdir + "/" + image.replace(image.end()-3,image.end(),"dat");
		cout<<infile<<endl;
		fstream fdatin(infile,ios::in);
		string file;
		getline(fdatin,file);
		getline(fdatin,file);
		vector<int> arr;
		int a;
		while (!fdatin.eof()) {
			fdatin >> a;
			cout<<a<<" ";
			arr.push_back(a);
		}
		cout<<endl;
		return arr;
	}

	void saveImages(){
		for(int i=0; i<images.size();i++){
			if(images[i].size()<=4) continue;
			string infile = inputfiledir + "/" + images[i];
			Mat img = imread(infile);
			vector<Mat> matArr = getDigits(img);
			vector<int> valArr = readDat(images[i]);
			for(int j=0;j<matArr.size();j++){
				string savefiledir = outputfiledir + "/" + to_string(valArr[j]) + "/";
				string savefile = savefiledir + to_string(j) + images[i];
				cout<<"saving: "<<savefile<<endl;
				imwrite(savefile,matArr[j]);
			}
		}
	}
};
