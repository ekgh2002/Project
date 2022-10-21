#ifndef CARDREADER_H
#define CARDREADER_H

#include <iostream>
#include "mfrc522.h"
#include "DeviceData.h"


class CardReader : public mfrc522  //  상속 (public 이하로 상속하겠다. )
{
private:
    uint8_t rfidData[16];
    DeviceData cardData;


public:
    CardReader(SPI* spi);
    virtual ~CardReader();
    bool isCard();
    DeviceData getCardNumber();



};

#endif