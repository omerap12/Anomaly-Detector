#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
protected:
    float lowerThreshold = 0.5;
public:
	HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual bool isAnomilty(float x, float y, correlatedFeatures cf);
    virtual void pushVectorToCF(map<string, vector<float>> &copyOfMapData, float max, string &key1, string &key2, const TimeSeries& ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
