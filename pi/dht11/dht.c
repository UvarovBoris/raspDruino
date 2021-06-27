#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wiringPi.h>
    
#define DATA_PIN 0

#define REQUEST_TIME 18

//DHT11 low time is 24
#define LOW_MIN_TIME 9
#define LOW_MAX_TIME 39

//DHT11 high time is 70
#define HIGH_MIN_TIME 55
#define HIGH_MAX_TIME 85

#define READ_MAX_TIME 100

#define RAW_DATA_SIZE 5
#define HT_DATA_SIZE 4

int readDataSomeTime(bool value, int maxTime) {
    int time = 0;
    int start = micros();
    while(digitalRead(DATA_PIN) == value) {
        time = micros() - start;
        if(maxTime != -1 && time > maxTime) return -1;
    }
    return time;
}

int readData(bool value) {
    return readDataSomeTime(value, READ_MAX_TIME);
}

int readHumidityAndTemperature(unsigned char* ht) {
    unsigned char data[RAW_DATA_SIZE];

    for(int byteIndex = 0; byteIndex < RAW_DATA_SIZE; byteIndex++) {
        for(int bitIndex = 7; bitIndex >= 0 ; bitIndex--) {
            if(readData(false) == -1) return 1;

            int highTime = readData(true);
            if(highTime == -1) return 1;

            if(highTime >= LOW_MIN_TIME && highTime <= LOW_MAX_TIME) {
                data[byteIndex] &= ~(1 << bitIndex);
            } else if(highTime >= HIGH_MIN_TIME && highTime <= HIGH_MAX_TIME) {
                data[byteIndex] |= 1 << bitIndex;
            }
        }
    }

    //Check data is correct by checksum
    if(data[0] + data[1] + data[2] + data[3] != data[4]) {
        return 1;
    }

    for(int i = 0; i < HT_DATA_SIZE; i++) {
        ht[i] = data[i];
    }

    return 0;
}


int main(int argc, char** argv) {
    
    unsigned char ht[HT_DATA_SIZE];
    wiringPiSetup();
    
    //Request
    pinMode(DATA_PIN, OUTPUT);    
    digitalWrite(DATA_PIN, HIGH);

    digitalWrite(DATA_PIN, LOW);

    delay(REQUEST_TIME);

    //Prepare for response
    pullUpDnControl(DATA_PIN, PUD_UP);
    pinMode(DATA_PIN, INPUT);

    //Read response preamble
    readData(true);
    if(readData(false) == -1) return 1;
    if(readData(true) == -1) return 1;

    //Start read data
    int status = readHumidityAndTemperature(ht);
    if(status != 0) {
        return 1;
    }

    printf("h:%d.%d|t:%d.%d\n", ht[0], ht[1], ht[2], ht[3]);

    return 0;
}
