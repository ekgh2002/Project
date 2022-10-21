#include "CardReader.h"

CardReader::CardReader(SPI* spi) : mfrc522(spi) // 
{

}

CardReader::~CardReader()
{

}

bool CardReader::isCard()
{

    uint8_t byte = mfrc522_request(PICC_REQALL, rfidData);
    if (byte == CARD_FOUND)
    {   
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
DeviceData CardReader::getCardNumber()
{
     mfrc522_get_card_serial(rfidData);
     cardData.devName = "cardReader";
     cardData.devData.clear(); //  std::vector<int> devData;
     for (const auto data : rfidData)
     {
        cardData.devData.push_back((int)data);
        
        // cardData.devData.emplace_back((int)data); 위와 똑같이 동작 push back : 변수를 받아서 back / emplace back : 상수화 시켜서 back, 속도가 좀더 빠름
     }
     return cardData;
     
}


