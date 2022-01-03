
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1;
    string feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float c_x;
    float c_y;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
    float highThreshold = 0.9;

    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    vector<correlatedFeatures> cf;
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

    float calculateThreshHold(Line line, float* fi, float* fj, int size);
    Line createLine(string fj, string fi, int size, map<string,vector<float>> mapData);
    virtual bool isAnomilty(float x, float y, correlatedFeatures cf);
    virtual void pushVectorToCF(map<string, vector<float>> &copyOfMapData, float max, string &key1, string &key2,const TimeSeries& ts);
    void setHighThreshold(float newThreshold);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
