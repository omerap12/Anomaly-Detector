#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include <netinet/in.h>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}
    // you may add additional methods here
};



// you may add here helper classes
class Client{
public:
    vector<AnomalyReport> reports;
    HybridAnomalyDetector* hybridAnomalyDetector;
    int linesNumber;

public:
    Client(){
        hybridAnomalyDetector=new HybridAnomalyDetector();
    }
    HybridAnomalyDetector* getDetector() {
        return this->hybridAnomalyDetector;
    }
    void setAnomalyReport(vector<AnomalyReport> rep) {
        this->reports = std::move(rep);
    }
    vector<AnomalyReport> getReports() {
        return reports;
    }
    void setNumberLines(int number) {
        this->linesNumber = number;
    }
    int getLines() {
        return this->linesNumber;
    }
};

class Command{
public:
    DefaultIO* dio;
    string description;
    Client* client;
public:

    Command(DefaultIO *dio, string description, Client* client){
        this->dio = dio;
        this->description = description;
        this->client = client;
    }

    virtual void execute()=0;
    virtual ~Command(){}

    string getCommand() {
        return this->description;
    }

};

class UploadCSV: public Command {
private:
    string trainCSVfile = "trainCSVfile.csv"; //train.txt
    string testCSVfile = "testCSVfile.csv"; // test.txt
public:
    UploadCSV(DefaultIO *dio, string description,
              Client *client) : Command(dio, description, client) {
        return;
    }


    void execute() override {
        //need to read line by line until "done" and write it to the file
        dio->write("Please upload your local train CSV file.\n");
        string tempLine = "";
        ofstream fw(trainCSVfile);
        while (tempLine != "done") {
            if (tempLine == "done")
                break;
            tempLine = dio->read();
            if (tempLine == "done")
                break;
            fw << tempLine;
            fw << "\n";
        }
        fw.close();
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        tempLine = "";
        ofstream fw1(testCSVfile);
        while (tempLine != "done") {
            if (tempLine == "done")
                break;
            tempLine = dio->read();
            if (tempLine == "done")
                break;
            fw1 << tempLine;
            fw1 << "\n";
        }
        fw1.close();
        dio->write("Upload complete.\n");
    };
};

class AlgorithmSetting: public Command {
public:
    AlgorithmSetting(DefaultIO *dio, string description,
                     Client *client): Command(dio,description,client) {
        return;
    }
    void execute() override {
        HybridAnomalyDetector* hybridAnomalyDetector = this->client->getDetector();
        float currentThreshold = ((floor(hybridAnomalyDetector->highThreshold*1000))/(1000));
        dio->write("The current correlation threshold is "+std::to_string(currentThreshold));
        dio->write("\n");
        dio->write("Type a new threshold\n");
        string userThresholdString;
        while (true) {
            userThresholdString = dio->read();
            float userThreshold = std::stof(userThresholdString);
            if ((userThreshold > 0.0f)&&(userThreshold < 1.0f)) {
                this->client->hybridAnomalyDetector->setHighThreshold(userThreshold);
                break;
            }
            else {
                dio->write("please choose a value between 0 and 1.\n");
                dio->write("The current correlation threshold is "+std::to_string(currentThreshold));
                dio->write("\n");
                dio->write("Type a new threshold\n");
            }
        }
    }
};

class DetectAnomalies: public Command {
public:
    DetectAnomalies(DefaultIO *dio, string description,
            Client *client): Command(dio,description,client) {
        return;
    }
    void execute() override {
        TimeSeries timeSeriresForTraing("trainCSVfile.csv");
        TimeSeries timeSeriresForTest("testCSVfile.csv");
        HybridAnomalyDetector *hybridAnomalyDetector = client->getDetector();
        hybridAnomalyDetector->learnNormal(timeSeriresForTraing);
        vector<AnomalyReport> anomalities = hybridAnomalyDetector->detect(timeSeriresForTest);
        this->client->setAnomalyReport(anomalities);
        this->client->setNumberLines(timeSeriresForTraing.getNumberOflines());
        hybridAnomalyDetector->cf.clear();
        dio->write("anomaly detection complete.\n");
    }
};

class DisplayResults:public Command {
public:
    DisplayResults(DefaultIO *dio, string description,
            Client *client): Command(dio,description,client) {
        return;
    }
    void execute() override {
        vector<AnomalyReport> reports = this->client->getReports();
        for (auto report:reports) {
            dio->write(std::to_string(report.timeStep));
            dio->write("         ");
            dio->write(report.description);
            dio->write("\n");
        }
        dio->write("Done.\n");
    }
};

/**
 * need to complete
 */
class UploadAnomalitiesAndAnalyze:public Command {
public:
    UploadAnomalitiesAndAnalyze(DefaultIO *dio, string description,
                                Client *client) : Command(dio, description, client) {
        return;
    }
    vector<pair<int, int>> ArrangeReports(const int size, const vector<AnomalyReport> vec) {
        vector<pair<int, int>> valueToReturn;
        for (int i=0; i < size; i++) {
            int getFirst = vec[i].timeStep;
            while ((i < size) && (vec[i].description == vec[i+1].description) &&
                   (vec[i+1].timeStep - vec[i].timeStep == 1)) {
                i++;
            }
            valueToReturn.emplace_back(getFirst, vec[i].timeStep);
        }
        return valueToReturn;
    }


    virtual void execute() {
        vector<pair<int, int>> clientDataAnomalies;
        vector<AnomalyReport> data = client->getReports();
        vector<pair<int, int>> dataArrangedFromHybrid = ArrangeReports(data.size(),data);
        dio->write("Please upload your local anomalies file.\n");

        float positive = 0;
        float negative = client->linesNumber;
        string inputFromUser = dio->read();
        while (inputFromUser != "done"){
            int delimiterIndex = inputFromUser.find(",");
            int firstLineIndex = stoi(inputFromUser.substr(0, delimiterIndex));
            int secondLineIndex = stoi(inputFromUser.substr(delimiterIndex + 1));
            clientDataAnomalies.emplace_back(firstLineIndex, secondLineIndex);
            negative -= (secondLineIndex - firstLineIndex);
            positive+=1;
            inputFromUser = dio->read();
        }
        float TP = 0;
        float FP = 0;
        dio->write("Upload complete.\n");
        for (int i=0; i < dataArrangedFromHybrid.size(); i++) {
            bool flag = false;
            for (int j=0; j < clientDataAnomalies.size(); j++) {
                if ((clientDataAnomalies[j].second >= dataArrangedFromHybrid[i].first
                     && dataArrangedFromHybrid[i].second >= clientDataAnomalies[j].second)
                    ||(clientDataAnomalies[j].first >= dataArrangedFromHybrid[i].first
                       && dataArrangedFromHybrid[i].second >= clientDataAnomalies[j].first)
                    ||(clientDataAnomalies[j].first <= dataArrangedFromHybrid[i].first
                       && dataArrangedFromHybrid[i].second <= clientDataAnomalies[j].second)) {
                    TP++;
                    flag = true;
                }
            }
            if (!flag) {
                FP++;
            }
        }
        float TPtoUser = TP / positive;
        float FPtoUser = FP / negative;
        TPtoUser = (floor(TPtoUser * 1000)) / (1000);
        FPtoUser = (floor(FPtoUser * 1000)) / (1000);
        dio->write("True Positive Rate: ");
        dio->write(TPtoUser);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(FPtoUser);
        dio->write("\n");
    }
};

class Exit: public Command {
public:
    Exit(DefaultIO *dio, string description,
            Client *client): Command(dio,description,client) {
        return;
    }
    void execute() override {
        return;
    }
};



#endif /* COMMANDS_H_ */
