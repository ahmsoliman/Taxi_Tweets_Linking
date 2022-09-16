/// @author Ahmed Soliman
/// @date  Sep 16, 2022, July 25, 2021
/// @file  stopwatch.cpp
/// @details  stopwatch is a high resolution timing class.
/// It is designed to seamlessly measure and report C++ code
/// execution times. Using stopwatch is very simple.*/
#include "stopwatch.h"

#include <iostream>
#include <chrono>

using namespace std::chrono;

time_point<high_resolution_clock> stopwatch::time_start;
nanoseconds stopwatch::total_time_taken_ns;

stopwatch::stopwatch()
  {
    reset();
  }

void stopwatch::reset()
  {
    total_time_taken_ns = std::chrono::nanoseconds(0);
  }

void stopwatch::start()
  {
    time_start = high_resolution_clock::now();
  }

__attribute__((unused)) double stopwatch::lap(TIME_UNIT time_unit, const std::string& msg)
  {
    high_resolution_clock::time_point time_stop;
    time_stop = high_resolution_clock::now();
    total_time_taken_ns += 
        duration_cast<nanoseconds>(time_stop-time_start);
    switch(time_unit)
      {
        case(TIME_UNIT_MS):
          std::cout << msg << " " 
            << duration_cast<milliseconds>(time_stop-time_start).count() 
            << " ms" << std::endl;
          break;
        case(TIME_UNIT_SEC):
        default:
          std::cout << msg << " " 
            << duration_cast<seconds>(time_stop-time_start).count() 
            << " seconds" << std::endl;
          break;
      }
      return((double)duration_cast<milliseconds>(time_stop-time_start).count()/1000.0);
  }

  double stopwatch::lap(const std::string& msg)
    {
      high_resolution_clock::time_point time_stop;
      time_stop = high_resolution_clock::now();
      total_time_taken_ns += 
        duration_cast<nanoseconds>(time_stop-time_start);

      auto  hr = duration_cast<hours>(time_stop-time_start).count();
      auto min = duration_cast<minutes>(time_stop-time_start).count();
      auto sec = duration_cast<seconds>(time_stop-time_start).count();
      auto  ms = duration_cast<milliseconds>(time_stop-time_start).count();
      auto  us = duration_cast<microseconds>(time_stop-time_start).count();
      auto  ns = (time_stop-time_start).count();

      std::cout << msg;
      if(hr>0)  std::cout <<  hr << " hrs ";
      if(min>0) std::cout << min -  hr*60    << " min ";
      if(sec>0) std::cout << sec - min*60    << " sec ";
      if(ms>0)  std::cout <<  ms - sec*1000  << " ms ";
      if(us>0)  std::cout <<  us -  ms*1000  << " µs "
                          <<  ns -  us*1000  << " ns "
                          << std::endl;
      return((double)ms/1000.0);
    }
  
  double stopwatch::total() {

    auto hr = duration_cast<hours>(total_time_taken_ns).count();
    auto min = duration_cast<minutes>(total_time_taken_ns).count();
    auto sec = duration_cast<seconds>(total_time_taken_ns).count();
    auto ms = duration_cast<milliseconds>(total_time_taken_ns).count();
    auto us = duration_cast<microseconds>(total_time_taken_ns).count();
    auto ns = total_time_taken_ns.count();

    std::cout << " Total time : ";
    if (hr > 0) {
      std::cout << hr << " hours ";
      std::cout << min - hr * 60 << " minutes ";
    } else {
      if (min > 0) {
        std::cout << min - hr * 60 << " minutes ";
        std::cout << sec - min * 60 << " seconds ";
      } else {
        if (sec > 0) {
          std::cout << sec - min * 60 << " seconds ";
          std::cout << ms - sec * 1000 << " milliseconds ";
        } else {
          if (ms > 0) {
            std::cout << ms - sec * 1000 << " milliseconds ";
            std::cout << us - ms * 1000 << " microseconds ";
          } else {
            if (us > 0) {
              std::cout << us - ms * 1000 << " microseconds ";
              std::cout << ns - us * 1000 << " nanoseconds ";
            }
          }
        }
      }
    }
    std::cout << std::endl;
    return((double)ms/1000.0);
    }

double stopwatch::get_total_seconds()
    {
      return (double)duration_cast<milliseconds>(total_time_taken_ns).count()/1000.0;
    }