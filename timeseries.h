#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class TimeSeries{
    const char* fileName;
    vector<string> features;
    map<string ,vector<float>> mapData;
    int numberOfLines;
public:
    TimeSeries(const char* fileName){
        loadCSV(fileName);
    }
    void loadCSV(const char* fileName);
    vector<string> splitFirst(string sentence,char delimiter);
    void printMap();
    void printProperties();
    void printValueOfPropertyJinI(string feature,int j);
    vector<string> getFeatures() const;
    map<string ,vector<float>> getMap() const;
    int getNumberOflines();

};
#endif /* TIMESERIES_H_ */
