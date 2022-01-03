#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->client = new Client();
    UploadCSV *firstCommand = new UploadCSV(dio,"1.upload a time series csv file\n",client);
    AlgorithmSetting *secondCommand = new AlgorithmSetting(dio,"2.algorithm settings\n",client);
    DetectAnomalies *thirdCommand = new DetectAnomalies(dio,"3.detect anomalies\n",client);
    DisplayResults *fourthCommand = new DisplayResults(dio, "4.display results\n",client);
    UploadAnomalitiesAndAnalyze *fifthCommand = new UploadAnomalitiesAndAnalyze(dio,"5.upload anomalies and analyze results\n",client);
    Exit *sixthCommand = new Exit(dio,"6.exit\n",client);

    commandsList.insert(pair<int,Command*>(1,firstCommand));
    commandsList.insert(pair<int,Command*>(2,secondCommand));
    commandsList.insert(pair<int,Command*>(3,thirdCommand));
    commandsList.insert(pair<int,Command*>(4,fourthCommand));
    commandsList.insert(pair<int,Command*>(5,fifthCommand));
    commandsList.insert(pair<int,Command*>(6,sixthCommand));
}

/**
 * function to print main menu
 */
void CLI::start(){

    while (true) {
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 1; i<7; i++) {
            dio->write(commandsList.at(i)->getCommand());
        }
        string input = dio->read();
        if (input == "6")
            break;
        commandsList.at(stoi(input))->execute();

    }
}


CLI::~CLI() {
    for (int i = 1; i<7; i++) {
        delete commandsList.at(i);
    }
}

