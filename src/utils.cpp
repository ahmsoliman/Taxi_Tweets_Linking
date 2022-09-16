/// @file utils.cpp
/// @author Ahmed Soliman
/// @date Sep 15, 2022
#include "utils.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define BUFFER_SIZE (16 * 1024)

///  read taxi csv file with the following fields/columns:
///    1. Taxi_ID - Identifier
///    2. Taxi_Epoch - Datetime stamp
///    3. Taxi_Long - Longitude
///    4. Taxi_Lat  - Latitude
///    Derived in part from coreutils/wc
///        https://github.com/coreutils/coreutils/blob/master/src/wc.c
///    and the question below:
///        https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file
size_t 
read_taxi_csv(string filename, vector<int> &Taxi_ID, vector<int> &Taxi_Epoch, vector<double> &Taxi_Long,
              vector<double> &Taxi_Lat)
{
  size_t bytes_read;
  uintmax_t lines=0;
  char buf[BUFFER_SIZE+1];

  int fd = open (filename.c_str(), O_RDONLY);
  if (fd == -1)
    {
      cout << "Can not open file: " << filename << endl;
      exit(1);
    } 
    
  while ((bytes_read = read (fd, buf, BUFFER_SIZE)) > 0)
    {
      char *p = buf;
      char *end = buf + bytes_read;
      while (p != end)
        lines += *p++ == '\n';
    }
  
  //lines--; //exclude header row

  Taxi_ID.resize (lines);
  Taxi_Epoch.resize (lines);
  Taxi_Long.resize (lines);
  Taxi_Lat.resize (lines);

  lseek (fd, 0, SEEK_SET);

  char *p = buf;
  char f[64];
  char j=0; //field character index
  //string f=""; //field string
  int i=1; //column/field index
  // i = 0 : state of reading and skipping header line
  // i = 1  : state of reading Taxi_ID field
  // i = 2  : state of reading Taxi_Epoch field
  // i = 3  : state of reading Taxi_Long field
  // i = 4  : state of reading Taxi_Lat field
  char d=','; //column delimiter
  char *end = buf + bytes_read;
  uintmax_t r=0; //row

  while ( (bytes_read = read (fd, buf, BUFFER_SIZE)) > 0 )
    {
      char *p = buf;
      char *end = buf + bytes_read;
      while (p != end)
        {
          if (*p == d)
            {
              switch (i)
              {
                case 0:
                  // skip header line
                  i++;
                  j=0;
                  f[0]='\0'; //'\0'; //f.clear();
                  d = ',';
                  break;
                case 1:
                  f[j]='\0';
                  Taxi_ID[r]=stoi(f);
                  j=0;
                  //f[0]='\0';
                  i++;
                  break;
                case 2:
                  f[j]='\0';
                  Taxi_Epoch[r]=stoi(f);
                  j=0;
                  //f[0]='\0';
                  i++;
                  break;
                case 3:
                  f[j]='\0';
                  Taxi_Long[r]=stod(f);
                  j=0;
                  //f[0]='\0';
                  i++;
                  d = '\n';
                  break;
                case 4:
                  f[j]='\0';
                  Taxi_Lat[r++]=stod(f);
                  j=0;
                  //f[0]='\0';
                  i=1;
                  d = ',';
                  break;
              }
            }
          else
            {
              f[j++] = *p;  
            }
          p++;
        }
    }
  close(fd);
  return(lines);
}

///  read tweet csv files with the following fields/columns:
///    1. Tweet_Epoch -  Datetime stamp
///    2. Tweet_Long  -  Longitude
///    3. Tweet_Lat   -  Latitude
///    4. Tweet_ID    -  Identifier
///    Derived in part from coreutils/wc
///        https://github.com/coreutils/coreutils/blob/master/src/wc.c
///    and the question below:
///        https://stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file
size_t read_tweet_csv(string filename, vector<int> &Tweet_Epoch, vector<double> &Tweet_Long,
                      vector<double> &Tweet_Lat, vector<int> &Tweet_ID)
{
  size_t bytes_read;
  uintmax_t lines=0;
  char buf[BUFFER_SIZE+1];

  int fd = open (filename.c_str(), O_RDONLY);
  if (fd == -1)
  {
    cout << "Can not open file: " << filename << endl;
    exit(1);
  }

  while ((bytes_read = read (fd, buf, BUFFER_SIZE)) > 0)
  {
    char *p = buf;
    char *end = buf + bytes_read;
    while (p != end)
      lines += *p++ == '\n';
  }

  //lines--; //exclude header row

  Tweet_Epoch.resize (lines);
  Tweet_Long.resize (lines);
  Tweet_Lat.resize (lines);
  Tweet_ID.resize (lines);

  lseek (fd, 0, SEEK_SET);

  char *p = buf;
  char f[64];
  char j=0; //field character index
  //string f=""; //field string
  int i=1; //column/field index
  // i = 0 : state of reading and skipping header line
  // i = 1  : state of reading Tweet_Epoch field
  // i = 2  : state of reading Tweet_Long field
  // i = 3  : state of reading Tweet_Lat field
  // i = 4  : state of reading Tweet_ID field
  char d=','; //column delimiter
  char *end = buf + bytes_read;
  uintmax_t r=0; //row

  while ( (bytes_read = read (fd, buf, BUFFER_SIZE)) > 0 )
  {
    char *p = buf;
    char *end = buf + bytes_read;
    while (p != end)
    {
      if (*p == d)
      {
        switch (i)
        {
          case 0:
            // skip header line
            i++;
            j=0;
            f[0]='\0'; //'\0'; //f.clear();
            d = ',';
            break;
          case 1:
            f[j]='\0';
            Tweet_Epoch[r]=stoi(f);
            j=0;
            //f[0]='\0';
            i++;
            break;
          case 2:
            f[j]='\0';
            Tweet_Long[r]=stod(f);
            j=0;
            //f[0]='\0';
            i++;
            d = ',';
            break;
          case 3:
            f[j]='\0';
            Tweet_Lat[r]=stod(f);
            j=0;
            //f[0]='\0';
            i++;
            d = '\n';
            break;
          case 4:
            f[j]='\0';
            Tweet_ID[r++]=stoi(f);
            j=0;
            //f[0]='\0';
            i=1;
            d = ',';
            break;
        }
      }
      else
      {
        f[j++] = *p;
      }
      p++;
    }
  }
  close(fd);
  return(lines);
}
