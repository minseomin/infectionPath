//
//  ifs_element.c
//  InfestPath
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

// ȯ���� ������ �����ϴ� ����ü
typedef struct ifs_ele {
    int index;  //number
    int age;    //age
    unsigned int time;   //time
    place_t place[N_HISTORY];//place[N_HISTORY]
} ifs_ele_t;

/*
    description : ȯ���� ����ü�� ����
    input parameters : index, age, time, history_place (ȯ���� ����)
    return value : void pointer
*/
void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY])
{
    ifs_ele_t* ptr;

    ptr = malloc(sizeof (struct ifs_ele));
    ptr->index = index;
    ptr->age = age;
    ptr->time = detected_time;
    for (int i = 0; i < N_HISTORY; i++)
    {
        ptr->place[i] = history_place[i];
    }

    return ptr;
}

/*
    description : ȯ���� ��ȣ�� ��ȯ
    input parameters : void pointer obj
    return value : int index
*/
int ifctele_getIndex(void* obj)
{
    ifs_ele_t* ptr = (ifs_ele_t*)obj;

    return ptr->index;
}

/*
    description : ȯ���� ���̸� ��ȯ
    input parameters : void pointer obj
    return value : int age
*/
int ifctele_getAge(void* obj)
{
    ifs_ele_t* ptr = (ifs_ele_t*)obj;

    return ptr->age;
}

/*
    description : ȯ���� �̵� ��� �� �ش� index ��°�� ��Ҹ� ��ȯ / index �� ������ ������ index ��°�� ��ȯ
    input parameters : void pointer obj, index
    return value : place index
*/
int ifctele_getHistPlaceIndex(void* obj, int index)
{
    ifs_ele_t* ptr = (ifs_ele_t*)obj;

    return index >= 0 ? ptr->place[index] : ptr->place[N_HISTORY + index];
}

/*
    description : ȯ���� ���� �ð��� ��ȯ
    input parameters : void pointer obj
    return value : unsigned int time
*/
unsigned int ifctele_getinfestedTime(void* obj)
{
    ifs_ele_t* ptr = (ifs_ele_t*)obj;

    return ptr->time;
}

/*
    description : ����� �̸��� ��ȯ
    input parameters : int placeIndex
    return value : char pointer placeName
*/
char* ifctele_getPlaceName(int placeIndex)
{
    return countryName[placeIndex];
}

/*
    description : ȯ���� ������ ���
    input parameters : void pointer obj
    return value : void
*/
void ifctele_printElement(void* obj)
{
    ifs_ele_t* ptr = (ifs_ele_t*)obj;

    printf("=== ȯ�� ���� ===\n");
    printf("��ȣ : %d\n", ptr->index);
    printf("���� : %i\n", ptr->age);
    printf("���� Ȯ�� ���� : %i\n", ptr->time);
    printf("���� ���: ");
    for (int i = 0; i < N_HISTORY; i++)
    {
        printf("%s ", countryName[ptr->place[i]]);
    }
    printf("\n");
}
