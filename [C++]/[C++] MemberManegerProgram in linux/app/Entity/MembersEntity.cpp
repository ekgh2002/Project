#include "MembersEntity.h"
#include <cstring>
#include <iostream>

MembersEntity::MembersEntity()
{ 
    loadMembersInfo();
    int idcount = 100001;
}

MembersEntity::~MembersEntity()
{

}

void MembersEntity::loadMembersInfo()
{
    fpDBData = fopen("memberLists.bin", "r"); // r+ : read도 하면서 write도 할수 있다.
    if (fpDBData == NULL)
    {
        fprintf(stderr, "file open error\n");  //  stderr화면에표시
        return;
    }    
    while(fread(&memberInfo, sizeof(MemberInfo), 1, fpDBData))
    {
        vecMembersList.push_back(memberInfo);
    }
    fclose(fpDBData);
}

void MembersEntity::printMemberInfo(int index)
{
    if (index > (int)vecMembersList.size())
    {
        fprintf(stderr, "out of range member count!\n");
        return;
    }
    printf("%04d, %s, %s, %s, %0x-%0x-%0x-%0x-%0x\n",
    vecMembersList[index].id,
    vecMembersList[index].name,
    vecMembersList[index].address,
    vecMembersList[index].phoneNumber,
    vecMembersList[index].cardNum[0],
    vecMembersList[index].cardNum[1],
    vecMembersList[index].cardNum[2],
    vecMembersList[index].cardNum[3],
    vecMembersList[index].cardNum[4]
    );
}

void MembersEntity::printMemberInfo(std::string name)  // 찾기기능
{
    for(const auto &member : vecMembersList)
    {
        if(strcmp(member.name, name.c_str()) == 0)  //  같으면 0, 다르면 0이 아닌값(참)이 출력
        {
            printMemberInfo(member.id);
            return;
        }
    }
}

void MembersEntity::printMemberInfo(int *cardNum)
{
    for(const auto &member : vecMembersList)
    {
        if(memcmp(member.cardNum, cardNum, sizeof(member.cardNum)) == 0)  //  같으면 0, 다르면 0이 아닌값(참)이 출력
        {
            printf("%04d, %s, %s, %s, %0x-%0x-%0x-%0x-%0x\n",
            member.id,
            member.name,
            member.address,
            member.phoneNumber,
            member.cardNum[0],
            member.cardNum[1],
            member.cardNum[2],
            member.cardNum[3],
            member.cardNum[4]
            );
            return;
        }
    }
}


bool MembersEntity::findMemberInfo(std::string name)  // 찾기기능
{
    for(const auto &member : vecMembersList)
    {
        if(strcmp(member.name, name.c_str()) == 0)  //  같으면 0, 다르면 0이 아닌값(참)이 출력
        {
            return true;
        }
    }
    return false;
}

bool MembersEntity::findMemberInfo(int *cardNum)
{
    for(const auto &member : vecMembersList)
    {
        if(memcmp (member.cardNum, cardNum, sizeof(member.cardNum)) == 0)  //  같으면 0, 다르면 0이 아닌값(참)이 출력
        {
            printMemberInfo(member.id);
            return true;
        }
    }
    return false;
}


void MembersEntity::addMemberInfo(MemberInfo member)
{
    vecMembersList.push_back(member);
}

bool MembersEntity::delMemberInfo(int *cardNum)
{
    std::vector<MemberInfo>::iterator itrMember;
    // itrMember = vecMembersList.begin();  //  이 벡터의 해당주소의 가장 첫번째를 알려줌.

    for (itrMember = vecMembersList.begin(); itrMember!= vecMembersList.end(); itrMember++)  //  itrmember : 배열의 주소를 받을 수 있는 포인터변수라고 생각.
    {
        if(memcmp (itrMember->cardNum, cardNum, sizeof(itrMember->cardNum)) == 0)
        {
            vecMembersList.erase(itrMember);  //  insert 해당데이터 변환
            return true;
        }        
    }
    return false;
}

void MembersEntity::memoryToDB()
{
    // file open;
    // vecMembersList의 배열을 하나씩 read
    // vecMembersList의 값을 file에 vecMembersList의 끝까지 write
    // file close;
    fpDBData = fopen("memberLists.bin", "w"); // r+ : read도 하면서 write도 할수 있다.
    FILE *fpDBData2 = fopen("memberLists.txt", "w");
    if (fpDBData == NULL)
    {
        fprintf(stderr, "file open error\n");  //  stderr화면에표시
        return;
    } 
    for(const auto &member : vecMembersList)
    {   
        fwrite(&member, sizeof(member), 1, fpDBData);
        fprintf(fpDBData2, "%05d, %s\n", member.id, member.name);
    }
    fclose(fpDBData);
    fclose(fpDBData2);

}

void MembersEntity::member_register(int *cardNum)
{
    MemberInfo tempMember;
    tempMember.id = idcount;
    printf("이름을 입력하세요 : ");
    scanf("%s", &tempMember.name);
    printf("\n주소를 입력하세요 : ");
    scanf("%s", &tempMember.address);
    printf("\n번호를 입력하세요 : ");
    scanf("%s", &tempMember.phoneNumber);
    memcpy(tempMember.cardNum, cardNum, sizeof(tempMember.cardNum));
    addMemberInfo(tempMember);
    printf("member registered\n");
    idcount++;
}

void MembersEntity::member_reader(int*cardNum)
{
    if(findMemberInfo(cardNum))
    {
        printf("Registered Member\n");
        printMemberInfo(cardNum);  
    }
    else
    {
        printf("Not Registered Member\n");
    }  
}

// void MembersEntity::findMemberInfo_PhoneNumber()
// {
//     MemberInfo tempMember3;
//     printf("휴대폰 번호를 입력하세요 : ");
//     scanf("%s", &tempMember3.phoneNumber);
//     static int not_member_count = 0;
//     for(const auto &member : vecMembersList)
//     {
//         if(strcmp(member.phoneNumber, tempMember3.phoneNumber) == 0)
//         {
//             printf("registed memeber\n");
//         }
//         else
//         {
//             not_member_count++;
//         }
//         if(not_member_count >= idcount)
//         {
//             printf("not registered member\n");
//         }
//     }
// }

void MembersEntity::searchMemberInfo(int *cardNum)
{
    for(const auto &member : vecMembersList)
    {
        if(memcmp(member.cardNum, cardNum, sizeof(member.cardNum)) == 0)
        {
            printf("%s", member.name);
        }
    }
}

int MembersEntity::searchMemberID(int *cardNum)
{
    for(const auto &member : vecMembersList)
    {
        if(memcmp(member.cardNum, cardNum, sizeof(member.cardNum)) == 0)
        {
            return member.id;
        }
    }
}

void MembersEntity::modifyMemberInfo(int *cardNum)
{
    MemberInfo tempMember;
    tempMember.id = searchMemberID(cardNum);

    std::vector<MemberInfo>::iterator itrMember;    
    itrMember = vecMembersList.begin();             
    for(itrMember; itrMember != vecMembersList.end() ; itrMember++)
    {
        if(memcmp(itrMember->cardNum, cardNum, sizeof(itrMember->cardNum)) == 0)
        {
            vecMembersList.erase(itrMember);
            
            printf("Name : ");
            scanf("%s", &tempMember.name);
            printf("address : ");
            scanf("%s", &tempMember.address);
            printf("PhoneNumber : ");
            scanf("%s", &tempMember.phoneNumber);
            memcpy(tempMember.cardNum, cardNum, sizeof(tempMember.cardNum));
            
            vecMembersList.insert(itrMember, tempMember);
            printf("%s's information is updated\n", tempMember.name);
        }
    }
    
}

void MembersEntity::removeMemberInfo(int *cardNum)
{
    if(findMemberInfo(cardNum) == 0)
    {
        printf("Not registered member\n");
    }    
    searchMemberInfo(cardNum);
    
    if(delMemberInfo(cardNum) == 1)
    {
        printf("  removed\n");
    }    
}

void MembersEntity::findMemberInfo_name(int *cardNum)
{
    MemberInfo tempMember;
    printf("\n이름을 입력하세요 : ");
    scanf("%s", &tempMember.name);
    for(const auto &member : vecMembersList)
    {
        if(strcmp(member.name, tempMember.name) == 0)
        {
            printMemberInfo(cardNum);
        }
    }
}

