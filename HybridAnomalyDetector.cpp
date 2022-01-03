#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {


}

HybridAnomalyDetector::~HybridAnomalyDetector() {

}

bool HybridAnomalyDetector::isAnomilty(float x, float y, correlatedFeatures cf) {
    Point p = Point(x,y);
    Point p1 = Point(cf.c_x, cf.c_y);
    if ((cf.corrlation > lowerThreshold && cf.corrlation < highThreshold) && (dist(p,p1) > cf.threshold)
            || (cf.corrlation >= highThreshold && SimpleAnomalyDetector::isAnomilty(x,y,cf))) {
        return true;
    }
    return false;
 //if cf[i].thresh_hold is bigger than 0.9 and simplesays it's
}
void HybridAnomalyDetector::pushVectorToCF(map<string, vector<float>> &copyOfMapData, float max, string &key1,
                                           string &key2,const TimeSeries& ts) {
    SimpleAnomalyDetector::pushVectorToCF(copyOfMapData,max,key1,key2,ts);
    if ((max > lowerThreshold)&&(max<highThreshold)) {
        float* x = copyOfMapData[key1].data();
        float* y = copyOfMapData[key2].data();
        int size = copyOfMapData[key1].size();
        Point** points = (Point**) malloc(size* sizeof(Point*));
        //iterate through the size
        for (int j = 0; j<size; j++) {
            //create new point on heap - x value as vector fi with the line j, y value as the vector fj with the line j
            Point* point = new Point(*(x+j),*(y+j));
            points[j] = point; //set point into the array
        }

        Circle c = findMinCircle(points,size);
        //creating the linear reg
        Line line = createLine(key1, key2, copyOfMapData[key1].size(), copyOfMapData);
        //creating the threshold
        float threshHold = c.radius*1.1;
        //set correlatedFeatures object
        correlatedFeatures clf;
        //set values
        clf.threshold = threshHold;
        clf.feature1 = key1;
        clf.feature2 = key2;
        clf.corrlation = max;
        clf.c_x = c.center.x;
        clf.c_y = c.center.y;
        clf.lin_reg = line;
        //push object into vector
        cf.push_back(clf);
        //freeing memory and return line
        for (int i = 0; i<size; i++)
            delete points[i];
        delete points;
    }
}
