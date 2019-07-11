#include "ReservationCheck.h"

const uint8_t numberOfStall = 10;

uint8_t expireHour[numberOfStall];  // expire time: hour 
uint8_t expireMin[numberOfStall];  // expire time: minute
uint8_t hourDuration = 0;  // parking duration: hour
uint8_t minDuration = 2;  // parking duration: minute

uint8_t alertHour[numberOfStall];  // alert led time: hour
uint8_t alertMin[numberOfStall];  // alert led time: minute
uint8_t alertDuration = 1;  // alert duration before expire: minute


void setExpiration(int n) {
  // set expiration time for stalls
  expireMin[n] = minInt + minDuration;
  if (expireMin[n] >= 60) {
    expireMin[n] -= 60;
    expireHour[n] += 24;
  }
  expireHour[n] = hourInt + hourDuration;
  if (expireHour[n] >= 24) {
    expireHour[n] -= 24;
  }
  // set alert time for stalls
  alertHour[n] = expireHour[n];
  alertMin[n] = expireMin[n] - alertDuration;
  if (alertMin[n] < 0) {
    alertMin[n] += 60;
    alertHour[n] -= 1;
    if (alertHour[n] < 0) {
      alertHour[n] += 24;
    }
  }
  Serial.print("Expired At: ");
  Serial.print(expireHour[n]);
  Serial.print(":");
  Serial.println(expireMin[n]);
}

void resetExpiration(int n) {
  // reset expiration time for new parking
  expireHour[n] = -1;
  expireMin[n] = -1;
  alertMin[n] = -1;
  alertHour[n] = -1;
}

uint8_t checkExpiration(int n) {
  // check expiration: 1 stands for expired; 2 stands for expiring
  if (hourInt == expireHour[n] && minInt == expireMin[n]) {
    resetExpiration(n);
    return 1;
  }
  else if (hourInt == alertHour[n] && minInt == alertMin[n]) {
    return 2; 
  }
  return 0;
}
