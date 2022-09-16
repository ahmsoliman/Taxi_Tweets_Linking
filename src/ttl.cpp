#include "ttl.h"
#include "utils.h"
#include "stopwatch.h"

#define DEBUG

__attribute__((unused)) bool sort_by_date(const row& a, const row& b) {
  return (GET_DATE(a) < GET_DATE(b));
}

__attribute__((unused)) bool sort_by_lat_long(const row& a, const row& b) {
  float a_squared_length = (GET_LAT(a) * GET_LAT(a)) + (GET_LONG(a) * GET_LONG(a));
  float b_squared_length = (GET_LAT(b) * GET_LAT(b)) + (GET_LONG(b) * GET_LONG(b));
  return (a_squared_length < b_squared_length);
}

bool sort_by_date_then_lat_long(const row& a, const row& b) {
  bool sorted = false;
  float a_squared_length = (GET_LAT(a) * GET_LAT(a)) + (GET_LONG(a) * GET_LONG(a));
  float b_squared_length = (GET_LAT(b) * GET_LAT(b)) + (GET_LONG(b) * GET_LONG(b));
  if(GET_DATE(a) < GET_DATE(b)) {
    sorted = true;
  } else {
    if( (GET_DATE(a) == GET_DATE(b)) &&
         (a_squared_length < b_squared_length) ) {
        sorted = true;
    }
  }
  return sorted;
}

bool sort_by_date_then_lat_long_then_type(const row& a, const row& b) {
  bool sorted = false;
  float a_squared_length = (GET_LAT(a) * GET_LAT(a)) + (GET_LONG(a) * GET_LONG(a));
  float b_squared_length = (GET_LAT(b) * GET_LAT(b)) + (GET_LONG(b) * GET_LONG(b));
  if(GET_DATE(a) < GET_DATE(b)) {
    sorted = true;
  } else {
    if( (GET_DATE(a) == GET_DATE(b)) &&
        (a_squared_length < b_squared_length) ) {
      sorted = true;
    } else {
      if( (GET_DATE(a) == GET_DATE(b)) &&
           (a_squared_length < b_squared_length) ) {
        sorted = (GET_TYPE(a) < GET_TYPE(b));
      }
    }
  }
  return sorted;
}

bool same_date_lat_long(const row& a, const row& b) {
  return (GET_DATE(a)==GET_DATE(b) &&
          GET_LAT(a)==GET_LAT(b) &&
          GET_LONG(a)==GET_LONG(b));
}

/// print the passed in data row tuple for either a taxi or a tweet
/// %os could be std::cout to print to stdout or
/// an ofstream to print to an opened file.
void print_row(row r, ostream& os) {
  string type;
  if(GET_TYPE(r) == TYPE_TWEET) {
    type="TW"; /// "Tweet"
  } else {
    type="TX"; /// "Taxi"
  }
  os << GET_DATE(r) << "\t"
     << fixed
     << setw(9) << setprecision(5) << right << setfill(' ')
     << GET_LONG(r) << "\t"
     << setw(9) << setprecision(5) << right
     << GET_LAT(r) << "\t"
     << setfill(' ') << right
     << type << setw(3) << GET_ID(r) << "\t"
     << GET_LINE(r)
     << endl;
}

__attribute__((unused)) bool cmp_by_value(pair<string,unsigned short int>& a,
                  pair<string,unsigned short int>& b) {
  return a.second > b.second;
}

/// main entry for linking taxi and tweet datasets
/// @brief  Link taxi and tweet datasets
/// @arg taxi file path
/// @arg tweet file path
/// @return merged file. Each row in the merged file constitutes the following:
/// `taxi_id`, `datetime (unix)`, `longitude`, `latitude`, `ht0`, `ht1`, `ht2`, `ht3`, `ht4`
int main(int argc, char** argv) {
  string filename;
  stringstream ss;
  cout << "Taxi Tweet Linkage" << endl;

  double t[6];

  /// Taxi data fields
  vector<int> Taxi_ID;
  vector<int> Taxi_Epoch;
  vector<double> Taxi_Long;
  vector<double> Taxi_Lat;

  /// Tweet data fields
  vector<int> Tweet_Epoch;
  vector<double> Tweet_Long;
  vector<double> Tweet_Lat;
  vector<int> Tweet_ID;

  stopwatch sw; //instantiating a stopwatch object

  if(argc<4){
      cout<<"Please supply filenames in the program arguments. "
       <<"For example, ./ttl 200_taxi_database.csv tweet_database.csv taxi_tweet_merged_database.csv"
       <<endl;
      exit(1);
  }

  /// Reading Taxi database
  stopwatch::start();
  filename = argv[1];
  cout << "Reading Taxi records/rows from file: "
    << filename << endl;
  size_t taxi_rows = read_taxi_csv(filename,
                                   Taxi_ID,Taxi_Epoch,
                                   Taxi_Long, Taxi_Lat);
  cout << " " << taxi_rows << " taxi entries read in ";
  stopwatch::lap("");

  /// Get number of Taxis
  set<int> set_of_taxi_ids;
  set_of_taxi_ids.clear();
  for(int i=0; i < taxi_rows; ++i)
    {
      set_of_taxi_ids.insert(Taxi_ID[i]);
    }
  cout << " " << set_of_taxi_ids.size() << " unique taxis" << endl;

  /// Reading Tweet database
  stopwatch::start();
  filename=argv[2];
  cout << "Reading Tweet records from file: " << filename << endl;
  size_t tweet_rows = read_tweet_csv(filename,
                                     Tweet_Epoch, Tweet_Long,
                                     Tweet_Lat, Tweet_ID);
  cout << " " << tweet_rows << " tweet entries read in ";
  stopwatch::lap("");

  /// Get number of unique tweets
  set<int> set_of_tweet_ids;
  for(int i=0; i < tweet_rows; ++i)
    {
      set_of_tweet_ids.insert(Tweet_ID[i]);
    }
  size_t n_hash_tags = set_of_tweet_ids.size();
  cout << " " << n_hash_tags << " unique hashtags" << endl;

  /// Form the sequence S1 from the Taxi rows.
  /// Each row constitutes: (Taxi_ID, Taxi_Epoch, Taxi_Long, Taxi_Lat)
  stopwatch::start();
  vector<row> S1;
  S1.resize(Taxi_ID.size());
  for(unsigned long int i=0; i < Taxi_ID.size(); i++)
    {
        S1[i] = make_tuple(Taxi_Epoch[i], Taxi_Long[i], Taxi_Lat[i],
                         TYPE_TAXI, Taxi_ID[i], i+1);
    }
  t[2]=stopwatch::lap("Forming sequence S1 took : ");
  vector<row> S1_orig;
  S1_orig.resize(S1.size());
  std::copy(S1.begin(), S1.end(), S1_orig.begin());
  // Form the sequence S2 from the Tweet rows.
  // Each row constitutes: (Tweet_Epoch, Tweet_Long, Tweet_Lat, Tweet_ID)
  // start=high_resolution_clock::now();
  stopwatch::start();
  vector<row> S2;
  S2.resize(Tweet_ID.size());
  for(unsigned long int i=0; i<Tweet_ID.size(); i++)
      {
          S2[i] = make_tuple(Tweet_Epoch[i], Tweet_Long[i], Tweet_Lat[i],
                           TYPE_TWEET, Tweet_ID[i], i+1);
      }
  t[3]=stopwatch::lap("Forming sequence S2 took : ");

  // sort S1 and S2
  stopwatch::start();
  sort(S1.begin(), S1.end(), sort_by_date_then_lat_long);
  t[4]=stopwatch::lap("Sorting S1 took : ");

  stopwatch::start();
  sort(S2.begin(), S2.end(), sort_by_date_then_lat_long);
  t[4]+=stopwatch::lap("Sorting S2 took : ");

  //Merge S1 and S2 into S
  stopwatch::start();
  vector<row> S;
  S.reserve(S1.size()+S2.size());
  merge(S1.begin(), S1.end(), S2.begin(), S2.end(),
    back_inserter(S), sort_by_date_then_lat_long_then_type);
  t[4]+=stopwatch::lap("Merging S1 and S2 into S took : ");

  #ifdef DEBUG
  ofstream of;
  streambuf *buf;
  ostream out(buf);
  #endif

  /// Write the sorted merged sequence to a file.
  /// For debugging only.
  #ifdef DEBUG
  of.open("merged_sequence.txt");
  /* buf = of.rdbuf(); */
  //int i=0;
  //for(auto it=S.begin();i<100000;++it,++i)
  for(auto & it : S)
    {
      print_row(it, of);
    }
  of.close();
  #endif

  /// Scan S to associate Taxis with Tweets
  stopwatch::start();
  tweet2taxis tw2txs; /// tweet-taxis associations
  tweet2taxi tw2tx; /// tweet-taxi linkages
  vector<row> vec_taxi,vec_tweet; /// taxi and tweet vectors
  row row_taxi, row_tweet;   /// taxi and tweet rows
  mapLine2HashTag ln2ht;  /// Line number (in original taxi database) to hash tag ID mapping
  string str_Taxi_ID;
  string str_Tweet_ID;

  /// Save tweet-to-taxi linkages.
  /// Only for debugging
#ifdef DEBUG
  of.open("linkage_output.csv");
  of << "TAXI_LineNo., TAXI_ID, TWEET_LineNo., HashTag_ID\n";
#endif

  /// Associate potential taxis with tweets
  for(auto it=S.begin();it!=S.end();)
    {
      vec_tweet.clear();
      vec_taxi.clear();
      /// skip tweet rows
      while ( it!=S.end() && GET_TYPE(*it) == TYPE_TWEET)
          ++it;
      if (it!=S.end() && GET_TYPE(*it) == TYPE_TAXI)
        {
          row_taxi = *it;
          vec_taxi.push_back(row_taxi); /// Add first taxi record
          ++it;
        }
      /// get potential taxi records
      while ( it!=S.end() && GET_TYPE(*it) == TYPE_TAXI &&
              same_date_lat_long(*it, row_taxi) )
        {
          vec_taxi.push_back(*it); /// Add other taxi records
          ++it;
        }
      /// get tweet records
      while ( it!=S.end() && GET_TYPE(*it) == TYPE_TWEET &&
              same_date_lat_long(*it, row_taxi) )
        {
          vec_tweet.push_back(*it);
          ++it;
        }

      /// associate all taxis in vec_taxi with all tweets in vec_tweet
      for(auto & it_taxi : vec_taxi)
        {
          row_taxi=it_taxi;
          for(auto & it_tweet : vec_tweet)
            {
              row_tweet=it_tweet;
              /// associate row_tweet with row_taxi
              str_Tweet_ID = to_string(GET_ID(row_tweet));
              str_Taxi_ID  = to_string(GET_ID(row_taxi));
              tw2txs[str_Tweet_ID][str_Taxi_ID]++;
              /// link tweet to taxi
              tw2tx[str_Tweet_ID] = str_Taxi_ID;
              ln2ht[GET_LINE(row_taxi)].insert(GET_ID(row_tweet));
#ifdef DEBUG
              // TAXI_LineNo., TAXI_ID, TWEET_LineNo., HashTag_ID
              of << setw(12) << GET_LINE(row_taxi) << ","
               << setw(8) << str_Taxi_ID << ","
               << setw(14) << GET_LINE(row_tweet) << ","
               << setw(11) << GET_ID(row_tweet) << endl;
#endif
          }
        }
    }

  t[5]=stopwatch::lap("Scanning sequence S took : ");

  stopwatch::total();

  /// Save tweet-to-taxi linkages.
  /// Only for debugging
#ifdef DEBUG
    of.close();
    of.open("tweets_taxi_counts.csv");
    // buf = of.rdbuf(); //
    //int i = 0;
    of << "TWEET_ID   TAXI_ID   Count\n";
    for(auto & tw2tx_it : tw2txs)
      {
        //if (i==10) break;
        of << "TWEET" << setw(3) << right << tw2tx_it.first;
        auto taxi2count = tw2tx_it.second;
        for(auto & it2 : taxi2count)
          {
            of << "  TAXI" << setw(4) << it2.first
             << "  " << setw(2) << it2.second;
          }
          of << endl;
        //++i;
      }
    of.close();
  #endif

  cout << "Total time in seconds: "
    << stopwatch::get_total_seconds()
    << endl;

  string output_file_name = argv[3];
  ofstream  fout;
  fout = ofstream(output_file_name, std::ios_base::out);
  /// print header
  fout << "taxi_id,datetime,longitude,  latitude";
  for(auto& item: set_of_tweet_ids) {
    fout << ",ht" << item;
  }
  fout << endl;

  /// print linked records
  for(unsigned long int i=0; i < Taxi_ID.size(); i++) {
    row r = S1_orig[i];
    fout
      << setw(3)
      << GET_ID(r) << ", "
      << GET_DATE(r) << ", "
      << fixed << setw(9) << setprecision(5) << right << setfill(' ')
      << GET_LONG(r) << ", "
      << fixed << setw(9) << setprecision(5) << right << setfill(' ')
      << GET_LAT(r)
      << setfill(' ') << right;
      /// mark all hashtags associated with current taxi
    set<HashTagT> set_ht = ln2ht[GET_LINE(r)];
    for(auto& ht: set_of_tweet_ids) {
      /// if hash tag is in the set associated with the current taxi line print '100' else print '0'
      fout << ",";
      if (set_ht.find(ht) != set_ht.end()) {
        fout << "100";
      } else {
        fout << "  0";
      }
    }
    fout << endl;
  }
  return 0;
}
