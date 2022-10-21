#ifndef MEMBERSMANAGESERVICE_H
#define MEMBERSMANAGESERVICE_H

#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "MembersEntity.h"
#include "Memberinfo.h"
#include "ComDev.h"
#include "View.h"



class MembersManageService
{
private:
    // time_t curTime;
    int membersManagerState;
    std::vector<MemberInfo> vecMembersList;
    MembersEntity *membersEntity;
    ComDev *comDev;
    View *view;
    

public:
    MembersManageService(ComDev *comDev, View *viewer);
    virtual ~MembersManageService();
    void updateStateEvent(std::string devName);
    void checkCardNumber(int *cardNum);
    void findMemberInfo_name();
    void findmembername();
    void findmember(int *cardNum);
};

#endif