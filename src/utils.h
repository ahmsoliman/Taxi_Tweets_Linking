#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <vector>

using namespace std;

size_t read_taxi_csv(string filename, vector<int> &Taxi_ID, vector<int> &Taxi_Epoch, vector<double> &Taxi_Long,
                     vector<double> &Taxi_Lat);

size_t read_tweet_csv(string filename, vector<int> &Tweet_Epoch, vector<double> &Tweet_Long,
                      vector<double> &Tweet_Lat, vector<int> &Tweet_ID);


#endif