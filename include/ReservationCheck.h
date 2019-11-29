#include <time.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

int monthInt;
int hourInt;
int minInt;
struct tm * timeinfo;

String getHour(int hours){
  // Convert integer hour to desired hour presentation
  String hour;
  switch(hours){
    case 7:
      hour = "a7";
      break;
    case 8:
      hour = "b8";
      break;
    case 9:
      hour = "c9";
      break;
    case 10:
      hour = "d10";
      break;
    case 11:
      hour = "e11";
      break;
    case 12:
      hour = "f12";
      break;
    case 13:
      hour = "g13";
      break;
    case 14:
      hour = "h14";
      break;
    case 15:
      hour = "i15";
      break;
    case 16:
      hour = "j16";
      break;
    case 17:
      hour = "k17";
      break;
    case 18:
      hour = "l18";
      break;
    default:
      hour = "nothing";
      Serial.println(hour);
      break;
  }
  return hour;
}

String getMonth(int monthNumb) {
  //convert integer month to desired presentation
  String month;
  switch(monthNumb){
    case 0:
      month = "Jan";
      break;
    case 1:
      month = "Feb";
      break;
    case 2:
      month = "Mar";
      break;
    case 3:
      month = "Apr";
      break;
    case 4:
      month = "May";
      break;
    case 5:
      month = "Jun";
      break;
    case 6:
      month = "Jul";
      break;
    case 7:
      month = "Aug";
      break;
    case 8:
      month = "Sep";
      break;
    case 9:
      month = "Oct";
      break;
    case 10:
      month = "Nov";
      break;
    case 11:
      month = "Dec";
      break;
    default:
      Serial.println("No Month matches the case");
      break;
  }
  return month;
}

void getTime(){
  //get current time
  time_t rawtime;
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  monthInt = timeinfo->tm_mon;
  hourInt = timeinfo->tm_hour;
  minInt = timeinfo->tm_min;
}

String setResvStall(String stallName) {
  //Convert time_t data type to integer data type for both month and date

  String dateStr = String(timeinfo->tm_mday);
  String month = getMonth(monthInt);
  String date = month+dateStr;
  String setStall;

  //Error handling for not checking the firebase if it isn't reservation period
  String hour = getHour(hourInt);

  if (hour == "nothing") {
    setStall = "nothing";
  }
  else {
    setStall ="reservation/"+date+"/"+stallName+"/"+hour+"/validated/";
  }

  //Print actual current time
  Serial.printf("Current local time and date: %s", asctime(timeinfo));
  return setStall;
}

bool resValidation(String stallName) {
  //Get validation string data
  bool stallStatus = Firebase.getBool(stallName);
  return stallStatus;
}
