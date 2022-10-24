#ifndef __DHT_Data_H__
#define __DHT_Data_H__

class DHT_Data
{
public:
    int RH;
    int RHDec;
    int Temp;
    int TempDec;
    int checksum;
    int error;
};

#endif