
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    return;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    return;
}
/**
 * detect function - using algorithm provided by the stuff
 * @param ts - TimeSeries object
 * @return vectors of anomaly reports
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyReportVector; //initialize of answer vector
    vector<string> copyOfFeature = ts.getFeatures(); //get the features vector
    map <string,vector<float>> copyOfMapData = ts.getMap(); //get the map values
    int linesCounter = copyOfMapData[copyOfFeature[0]].size(); //get number of lines per features
    int sizeOfCorrelatedIndexes = cf.size(); //get the size of the features correlation

    for (int i = 0; i<sizeOfCorrelatedIndexes; i++) { //iterating through the features
        for (int j = 0; j<linesCounter; j++) { //iterate through the lines
            float x = copyOfMapData[cf[i].feature1].at(j); //get x be the value of feature1 i with the line j
            float y = copyOfMapData[cf[i].feature2].at(j); //get y be the value of feature2 i with the line j
            if (isAnomilty(x,y,cf[i])) //if dev of line is bigger than the threshold
                //create and push the anomaly
                anomalyReportVector.push_back(AnomalyReport(cf[i].feature1+"-"+cf[i].feature2,j+1));
        }
    }
    return anomalyReportVector;
}
/**
 * learn normal function - using algorithm provided by the stuff
 * @param ts - the TimeSeries object, containing data
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    map<string,vector<float>> copyOfMapData = ts.getMap(); //get copy of the map
    vector <string> copyOfFeatures = ts.getFeatures(); //get copy of the features
    int N = copyOfFeatures.size(); // get the size of the features
    int sizeOfVector = 0;

    //iterate through the features
    for (int i = 0; i<N; i++) {
        //initialize max , c to 0
        float max = 0;
        float c = -1;
        //initialize key1 and key2 as the features names
        string key1;
        string key2;
        //inner loop - according to algorithm
        for (int j = i+1; j<N;j++) {
            vector<float> fi = copyOfMapData.at(copyOfFeatures[i]); //get fi as the vector of the first feature
            vector<float> fj = copyOfMapData.at(copyOfFeatures[j]); //get fj as the vector of the second feature
            sizeOfVector = fi.size(); //get the size of the vector (lines size)
            float pear = pearson(fi.data(),fj.data(),sizeOfVector); //using pear function
            //algorithm implementation
            if (pear < 0 )
                pear = pear*-1;
            if (pear > max)
            {
                c = j;
                key1 = copyOfFeatures[i]; //set key1 to feature i
                key2 = copyOfFeatures[j]; //set key2 to feature j
                max = pear;
            }
        }
        //algorithm implementation
        if (c!= -1){
            //if max is bigger than high threshold value (using as threshold)
            pushVectorToCF(copyOfMapData, max, key1, key2,ts);
        }
    }
}

void SimpleAnomalyDetector::pushVectorToCF(map<string, vector<float>> &copyOfMapData, float max, string &key1,
                                      string &key2,const TimeSeries& ts) {
    if (max > highThreshold) {
        //creating the linear reg
        Line line = createLine(key1, key2, copyOfMapData[key1].size(), copyOfMapData);
        //creating the threshold
        float threshHold = calculateThreshHold(line, copyOfMapData[key1].data(), copyOfMapData[key2].data(),
                                               copyOfMapData[key2].size());
        //set correlatedFeatures object
        correlatedFeatures clf;
        //set values
        clf.threshold = threshHold;
        clf.feature1 = key1;
        clf.feature2 = key2;
        clf.lin_reg = line;
        clf.corrlation = max;
        //push object into vector
        cf.push_back(clf);
    }
}

/**
 * create line function - return linear reg
 * @param fj - vector of values of feature fi
 * @param fi - vector of values of feature fj
 * @param size - size of the values
 * @param mapData - copy of the map, storing all data
 * @return new line
 */
Line SimpleAnomalyDetector::createLine(string fj, string fi, int size, map<string,vector<float>> mapData) {
    //create array of pointers on heap
    Point** points = (Point**) malloc(size* sizeof(Point*));
    //iterate through the size
    for (int j = 0; j<size; j++) {
        //create new point on heap - x value as vector fi with the line j, y value as the vector fj with the line j
        Point* point = new Point(mapData[fj][j],mapData[fi][j]);
        points[j] = point; //set point into the array
    }
    //create line using aux function
    Line line = linear_reg(points,size);

    //freeing memory and return line
    for (int i = 0; i<size; i++)
        delete points[i];
    delete points;
    return line;
}

/**
 * calculate ThreshHold - return threshold of two correlated features
 * @param line - the reg line
 * @param fi - vector of values of feature fi
 * @param fj - vector of values of feature fj
 * @param size - size of the values
 * @return  - threshold
 */
float SimpleAnomalyDetector::calculateThreshHold(Line line, float* fi, float* fj, int size) {
    float temp = 0;
    //iterate through values of the vectors
    for (int i = 0; i<size; i++) {
        //create new point on heap - x value as vector fi with the line j, y value as the vector fj with the line j
        Point *point = new Point(fi[i], fj[i]);
        float threshTemp = dev(*point, line); //create threshold temp using dev function, gets the max value
        if (threshTemp > temp)
            temp = threshTemp;
        //free memory
        delete point;
    }
    //return max value * 1.1
    return temp*1.1;
}

bool SimpleAnomalyDetector::isAnomilty(float x, float y, correlatedFeatures cf) {
    Point p = Point(x,y);
    return cf.threshold < dev(p,cf.lin_reg);
}

void:: SimpleAnomalyDetector::setHighThreshold(float newThreshold) {
    this->highThreshold = newThreshold;
}
