#include "MembersManageService.h"
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <wiringPi.h>

MembersManageService::MembersManageService(ComDev *comDev, View *viewer)
{
    membersEntity = new MembersEntity();
    this->comDev = comDev;
    this->view = viewer;
    membersManagerState = CARD_READER;
    
}

MembersManageService::~MembersManageService()
{

}

void MembersManageService::updateStateEvent(std::string devName)
{
    MemberInfo tempMember;
    static int idcount = 100001;
    static int notcounter = 0;

    switch (membersManagerState)
    {
        case CARD_READER:
            if(devName == "ModeButton")
            {
                membersManagerState = CARD_REGISTER;
                printf("changed to CARD_REGISTER State\n");  
                view->setstate(membersManagerState);    
            }
            view->print_view();
        break;

        case CARD_REGISTER:
            if(devName == "ModeButton")
            {
                membersManagerState = CARD_REMOVE;
                printf("changed to CARD_REMOVE State\n");
                view->setstate(membersManagerState);        
            }
            view->print_view();
        break;

        case CARD_REMOVE:
            if(devName == "ModeButton")
            {
                membersManagerState = CARD_MODIFY;
                printf("changed to CARD_MODIFY State\n");
                view->setstate(membersManagerState);              
            }
            view->print_view();
        break;

        case CARD_MODIFY:
            if(devName == "ModeButton")
            {
                membersManagerState = CARD_FIND;
                printf("changed to CARD_FIND State\n");
                view->setstate(membersManagerState);         
            }
            view->print_view();
        break;

        case CARD_FIND:
        
            if(devName == "ModeButton")
            {
                membersManagerState = CARD_READER;
                printf("changed to CARD_READER State\n");          
            }
        break;
    
    }
}


void MembersManageService::checkCardNumber(int *cardNum)
{
    // MemberInfo tempMember;
    
    switch (membersManagerState)
    {
        case CARD_READER:
            membersEntity->member_reader(cardNum);
            membersEntity->printMemberInfo(cardNum);
        break;

        case CARD_REGISTER:
            membersEntity->member_register(cardNum); 
        break;

        case CARD_REMOVE:
            membersEntity->removeMemberInfo(cardNum);
        break;

        case CARD_MODIFY:
            membersEntity->modifyMemberInfo(cardNum);
        break;

        case CARD_FIND:
             membersEntity->findMemberInfo_name(cardNum);
        break;
    }
}

void MembersManageService::findmember(int *cardNum)
{
    switch (membersManagerState)
    {
        case CARD_FIND:
             membersEntity->findMemberInfo_name(cardNum);
        break;
    }    
}






