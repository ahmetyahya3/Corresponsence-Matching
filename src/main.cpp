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

double dMapper(double value, double start, double end, double mappedStart, double mappedEnd){//it maps the segments for doubles
    return (value-start)*(mappedEnd-mappedStart)/(end-start)+mappedStart;
}

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

int main(){
    vector<vector<double>> rightUwpMap = readCSV("right_uwp_map.csv");
    vector<vector<double>> leftUwpMap = readCSV("left_uwp_map.csv");

    int rows = rightUwpMap.size(), colums = rightUwpMap[0].size();
    cout << rows << " " << colums << endl;

    Mat mat(rows, colums, CV_8UC1);
    
    double mak = 0;
    double mik = 0;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < colums; j++){
            mak = max(mak, rightUwpMap[i][j]-leftUwpMap[i][j]);
            mik = min(mik, rightUwpMap[i][j]-leftUwpMap[i][j]);
            mat.at<uchar>(i, j) = dMapper(abs(leftUwpMap[i][j]-rightUwpMap[i][j]), 0, 6.8, 0, 255);
        }
    }
    cout << mak << " " << mik << endl;
    imshow("deneme", mat);
    waitKey(0);
}