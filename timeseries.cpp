#include "timeseries.h"
#include <iostream>
#include <vector>
#include <string>

/**
 * load the csv file into the TimeSeries object
 * @param fileName - name of the csv file
 */
void TimeSeries::loadCSV(const char* fileName) {
    map<int, vector<float>> tempMap;
    ifstream infile(fileName);
    vector<float> tempVector;
    string tempLine;
    //reading first line
    getline(infile, tempLine);
    //calling function to split the names of the features
    features = TimeSeries::splitFirst(tempLine, ',');
    vector<string> first = features;
    //gets the number of features
    int sizeOfColums = first.size();
    vector<float> values;
    int numberOfLine = 1;
    //continue reading lines from file
    while (getline(infile, tempLine)) {
        //gets every line in vector string
        vector<string> lineVector = TimeSeries::splitFirst(tempLine, ',');
        numberOfLine += 1;
        //convert word to float
        for (auto i: lineVector) {
            float temp = std::stof(i);
            //push value to temp
            values.push_back(temp);
        }
    }
    this->numberOfLines = numberOfLine; // updating number of lines
    //got everything to vector needs to divide it to the map. (to every vector in map)
    for (int i = 0; i < sizeOfColums; i++) {
        for (int j = 0; j < values.size(); j += sizeOfColums) {
            tempVector.push_back(values[j + i]);
        }
        //insert data into map
        mapData.insert(std::make_pair(first[i], tempVector));
        //clear the vector
        tempVector.clear();
    }
}
/**
 * function that splits the sentence into vector.
 * @param sentence - the string
 * @param delimiter - letter which creating the split
 * @return - vector of split string
 */
vector<string> TimeSeries::splitFirst(string sentence, char delimiter) {
    // creating the tok splitter
    string tok;
    //create an empty vector
    vector<string> firstWords;
    // setting string stream
    std::stringstream ss(sentence);
    string word;
    //cutting words
    while(getline(ss, tok, delimiter)) {
        //push word into vector
        firstWords.push_back(tok);
    }
    //return vector
    return firstWords;
}

/**
 * function to print the data of the map
 */
void TimeSeries::printMap() {
    //iterarating through the map keys
    for (auto m:features) {
        //get the map values (vector of floats)
        auto i = mapData.find(m);
        //iterating through values and print t
        for (auto j:i->second)
            std::cout<<j<<std::endl;
    }
}
/**
 * function which prints the features
 */
void TimeSeries::printProperties() {
    for (auto m:features)
        std::cout<<m<<std::endl;
}

/**
 * function which prints the feature i at the line j
 * @param feature - feature name
 * @param j - index of line in feature vector
 */
void TimeSeries::printValueOfPropertyJinI(string feature,int j) {
  std::cout<<mapData[feature].at(j)<<std::endl;
}

/**
 * function that return the features vector
 * @return - features vector
 */
vector<string> TimeSeries::getFeatures() const {
    return this->features;
}

/**
 * function that return the mapData
 * @return  - mapData
 */
map<string ,vector<float>> TimeSeries::getMap() const {
    return this->mapData;
}

int TimeSeries::getNumberOflines() {
    return this->numberOfLines;
}

