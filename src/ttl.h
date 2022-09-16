//
// Created by ahs16102 on 9/15/22.
//

#ifndef TTL_TTL_H
#define TTL_TTL_H

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <bits/stdc++.h>
#include <map>
#include <iterator>

/// definitions of data rows
#define TYPE_TAXI  0
#define TYPE_TWEET 1

/// Taxi/Tweet data row tuples
/// datetime, longitude, latitude, TYPE_TAXI/TYPE_TWEET, taxi_id/hashtag_id, line_number
///      int,     float,    float,                 bool,                int,         int
///        0,         1,        2,                    3,                  4,           5
typedef std::tuple<int, float, float, bool, int, int> row;

///  Macros for accessing data row tuples
#define GET_DATE    get<0>
#define GET_LONG    get<1>
#define GET_LAT     get<2>
#define GET_TYPE    get<3>
#define GET_ID      get<4>
#define GET_LINE    get<5>

//taxi-to-count mappings
typedef std::map<std::string,unsigned short int> taxi2cnt;
//Associations, tweet-to-taxi2cnt mappings
typedef std::map<std::string, taxi2cnt> tweet2taxis;
//Linkages, tweet-to-taxi mappings
typedef std::map<std::string, std::string> tweet2taxi;

typedef unsigned long int LineNumberT;
typedef unsigned short HashTagT;
/// map from original taxi line number to set of hash tags
typedef std::map<LineNumberT, std::set<HashTagT>> mapLine2HashTag;

using namespace std;
using namespace std::chrono;

#endif //TTL_TTL_H
