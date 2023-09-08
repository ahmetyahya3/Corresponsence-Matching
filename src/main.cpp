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

int windowSize = 5;


int main(){
    vector<vector<double>> rightUwpMap = readCSV("right_uwp_map.csv");
    vector<vector<double>> leftUwpMap = readCSV("left_uwp_map.csv");

    int rows = rightUwpMap.size(), cols = rightUwpMap[0].size();

    Mat rightUwp(rows, cols, CV_32F);
    Mat leftUwp(rows, cols, CV_32F);
    
    for(int i = 0; i < rows; i++){
        auto rowLeft = leftUwp.ptr<float>(i);
        auto rowRight = rightUwp.ptr<float>(i);
        for(int j = 0; j < cols; j++){
            rowLeft[j] = leftUwpMap[i][j];
            rowRight[j] = rightUwpMap[i][j];
        }
    }

    Mat normalizedRightUwp(rows, cols, CV_32F);
    Mat normalizedLeftUwp(rows, cols, CV_32F);

    normalize(rightUwp, normalizedRightUwp, 0, 1.0, NORM_MINMAX, CV_32F);
    normalize(leftUwp, normalizedLeftUwp, 0, 1.0, NORM_MINMAX, CV_32F);

    Rect imgBoun(0, 0, cols, rows);
    for(int i = 0; i < rows; i++){
        // cout << i << endl;
        for(int j = 0; j < cols; j++){
            int roiX = j-windowSize/2;
            int roiY = i-windowSize/2;
            roiX = max(roiX, 0);
            roiY = max(roiY, 0);
            int roiWidth = min(windowSize, cols-roiX);
            int roiHeight = min(windowSize, rows-roiY);
            Rect roi(roiX, roiY, roiWidth, roiHeight);


            // slower but clearer option
            // Rect roi(j-windowSize/2, i-windowSize/2, windowSize, windowSize);
            // roi = roi&imgBoun; 
            Mat regionLeft = normalizedLeftUwp(roi);

            int minError = INT_MAX;

            for(int k = 0; k < cols; k++){
                // better aproach
                int roiX = k-windowSize/2;
                int roiY = i-windowSize/2;
                roiX = max(roiX, 0);
                roiY = max(roiY, 0);
                int roiWidth = min(windowSize, cols-roiX);
                int roiHeight = min(windowSize, rows-roiY);
                Rect roi(roiX, roiY, roiWidth, roiHeight);

                // slower but clearer option
                // Rect roi(k-windowSize/2, i-windowSize/2, windowSize, windowSize);
                // roi = roi&imgBoun; //too slow
                Mat regionRight = normalizedRightUwp(roi);
                Scalar err = norm(regionLeft, regionRight, NORM_L2);
                minError = min(minError, static_cast<int>(err[0]));
            }
            cout << minError << endl;
        }
    }

    imshow("normilezedRight", normalizedRightUwp);
    imshow("normilezedLeft", normalizedLeftUwp);


    waitKey(0);
}