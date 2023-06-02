#pragma once
#include <Ticker.h>

class Time {
  public:
    int sec_;
    int min_;
    int hour_;
    int day_;
    String month_;
    int year_;
    String monthes [12] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
  
    Time();
    void plus();
    String getTime();
    String getTimeHM();
    long int getSecOfDay();
    String getTimeMinHour();
    String getTimeSec();
    int getSec();
    String getData();
    void setTime(int year__, int month__, int day__, int hour__, int min__, int sec__);
    void setHour(int hour__);
    void corrSec(int secpls);
    /* void minus();
      void delta();
      int getSec_char();
      int getMin_char();
      int getHour_char();*/
};

Time::Time() {
  sec_ = 00;
  min_ = 00;
  hour_ = 00;
  day_ = 01;
  month_ = "july";
  year_ = 2018;
}

void Time::plus() {
  if (sec_++ >= 59) {
    sec_ = 0;
    if (min_++ >= 59) {
      min_ = 0;
      if (hour_++ >= 23) {
        hour_ = 0;
        day_++;

        if ((day_ == 32)&&(month_ == monthes[11])){
          day_ = 1;
          month_= monthes[0];
          year_++;
        }
        
        if ((day_ == 32) && ((month_ == monthes[0]) || (month_ == monthes[2]) || (month_ == monthes[4]) || (month_ == monthes[6]) || (month_ == monthes[7]) || (month_ == monthes[9]))) {
          day_ = 1;
          for (int i = 10; i >= 0; i--) {
            if (month_ == monthes[i]) {
              month_ = monthes [i + 1];
            }
          }
        }

        if ((day_ == 31) && ((month_ == monthes[3]) || (month_ == monthes[5]) || (month_ == monthes[8]) || (month_ == monthes[10]))) {
          day_ = 1;
          for (int i = 10; i >= 0; i--) {
            if (month_ == monthes[i]) {
              month_ = monthes [i + 1];
              break;
            }
          }
        }

        if ((day_ == 29) && (month_ == monthes[1]) && ((year_ - 2004) % 4 != 0 )) {
          day_ = 1;
          month_ = monthes [2];
        }

        if ((day_ == 30) && (month_ == monthes[1]) && (year_ - 2004) % 4 == 0 ) {
          day_ = 1;
          month_ = monthes [2];
        }
      }
    }
  }
}

String Time::getTimeSec() {
  String temp;
  if (sec_ >= 10) {
    temp = String(sec_);
  }
  else {
    temp = "0" + String(sec_);
  }
  return temp;
}

long int Time::getSecOfDay() {
  return 60*((60 * hour_) + min_) + sec_;
}

String Time::getTimeMinHour() {
  String temp = String(hour_) + ":";
 
  if (min_ >= 10) {
    temp += String(min_);
  }
  else {
    temp += "0" + String(min_);
  }
  return temp;
}

String Time::getTime() {
  String temp = "";
  if (hour_ >= 10) {
    temp += String(hour_) + ":";
  }
  else
  {
    temp += "0" + String(hour_) + ":";
  }
  
  if (min_ >= 10) {
    temp += String(min_) + ":";
  }
  else {
    temp += "0" + String(min_) + ":";
  }

  if (sec_ >= 10) {
    temp += String(sec_);
  }
  else {
    temp += "0" + String(sec_);
  }
  return temp;
}


String Time::getTimeHM() {
  String temp = "";
  if (hour_ >= 10) {
    temp += String(hour_);
  }
  else
  {
    temp += "0" + String(hour_);
  }
  
  if (sec_%2)
  {
    temp += " ";
  }
  else
  {
    temp += ":";
  }
  
  if (min_ >= 10) {
    temp += String(min_);
  }
  else {
    temp += "0" + String(min_);
  }
  return temp;
}

String Time::getData() {
  String temp;
  if (day_ < 10) {
    temp = "0" + String(day_);
  }
  else {
    temp = String(day_);
  }
  temp += " " + month_ + " " + String (year_);
  return temp;
}

 int Time::getSec(){
  return sec_;
 }
/*  void Time::minus();
  void Time::delta();
  int Time::getSec_char();
  int Time::getMin_char();
  int Time::getHour_char();
  char* Time::getTime_char();*/

void Time::setTime(int year__, int month__, int day__, int hour__, int min__, int sec__){
  year_ = year__;
  month_= monthes[month__ - 1];
  day_ = day__;
  hour_ = hour__;
  min_ = min__;
  sec_ = sec__;
  Serial.println("###################Set Done");
}

void Time::setHour(int hour__){
  hour_ = hour__;
  Serial.println("###################Set Hour Done");
}


void Time::corrSec(int secpls)
{
  for (int i = 0; i < secpls; i++){
    plus();
  }
}
