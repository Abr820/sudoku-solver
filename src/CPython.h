/*
 * CPython.h
 *
 *  Created on: 05-May-2021
 *      Author: abrar
 */

#include <Python.h>
#include "pyhelper.hpp"
#include "numpy/arrayobject.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class CPython
{
	CPyObject pName;
	CPyObject pModule;
	CPyObject pFunc;
public:
	CPython(){
		CPyInstance pyInstance;
		pName = PyUnicode_FromString("pyMain");
		pModule = PyImport_Import(pName);
		pFunc = PyObject_GetAttrString(pModule, "predict");
	}

	vector<int> predict(vector<Mat> images){
		vector<int> ans;
		if(pModule)
		{
			if(pFunc && PyCallable_Check(pFunc))
			{
				for(int i=0; i<images.size();i++){
					// total number of elements (here it's a grayscale 640x480)
				      int nElem = 28*28*3;

				      // create an array of apropriate datatype
				      uchar* m = new uchar[nElem];

				      // copy the data from the cv::Mat object into the array
				      std::memcpy(m, images[i].data, nElem * sizeof(uchar));

				      // the dimensions of the matrix
				      npy_intp mdim[] = { 28,28,3 };

				      // convert the cv::Mat to numpy.array
				      CPyObject mat = PyArray_SimpleNewFromData(3, mdim, NPY_UINT8, (void*) m);

				      CPyObject pValue = PyObject_CallObject(pFunc, mat);
				      ans.push_back( PyLong_AsLong(pValue));
				}
			}
			else
			{
				printf("ERROR: function getInteger()\n");
			}

		}
		else
		{
			printf("ERROR: Module not imported\n");
		}
		return ans;
	}

};


