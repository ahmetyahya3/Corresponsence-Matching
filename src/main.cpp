#include <iostream>
#include <vector>
#include <cmath>
#include <numbers>
#include <fstream>
#include <algorithm>
#include <string>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

vector<vector<double>> readCSV(string fileName){
    ifstream inFile("../../src/"+fileName);
    vector<vector<double>> matrix;
    string line, value;

    while (getline(inFile, line)) {
        vector<double> row;
        stringstream ss(line);
        while (getline(ss, value, ',')) {
            row.push_back(stod(value));
        }
        matrix.push_back(row);
    }
    return matrix;
}

int kernelSize = 101;


int main(){
    vector<vector<double>> rightUwpMap = readCSV("right_uwp_map.csv");
    vector<vector<double>> leftUwpMap = readCSV("left_uwp_map.csv");

    int rows = rightUwpMap.size(), colums = rightUwpMap[0].size();

    Mat rightUwp(rows, colums, CV_32F);
    Mat leftUwp(rows, colums, CV_32F);
    
    for(int i = 0; i < rows; i++){
        auto rowLeft = leftUwp.ptr<float>(i);
        auto rowRight = rightUwp.ptr<float>(i);
        for(int j = 0; j < colums; j++){
            rowLeft[j] = leftUwpMap[i][j];
            rowRight[j] = rightUwpMap[i][j];
        }
    }

    Mat normalizedRightUwp(rows, colums, CV_32F);
    Mat normalizedLeftUwp(rows, colums, CV_32F);

    normalize(rightUwp, normalizedRightUwp, 0, 1.0, NORM_MINMAX, CV_32F);
    normalize(leftUwp, normalizedLeftUwp, 0, 1.0, NORM_MINMAX, CV_32F);


    for(int i = kernelSize/2; i < rows-kernelSize/2; i++){
        for(int k = kernelSize/2; k < colums-kernelSize/2; k++){
            Rect roi(k-kernelSize/2, i-kernelSize/2, kernelSize, kernelSize);
            Mat regionLeft = normalizedLeftUwp(roi);
        }

    }

    imshow("normilezedRight", normalizedRightUwp);
    imshow("normilezedLeft", normalizedLeftUwp);


    waitKey(0);
}