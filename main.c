//
//  main.c
//  infestPath
//
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2

/*
    description : patient_no 환자의 전파자를 추적. 감염된 시간, 장소로 detected_time, place 를 변경
    input parameters : int patient_no (추적할 환자 번호), int* detected_time (감염된 시간), int* place (감염된 장소)
    return value : infester_no (전파자 번호) / -1 (전파자가 없음)
*/
int trackInfester(int patient_no, int *detected_time, int *place) {
    void* patient = ifctdb_getData(patient_no);
    int patient_place, patient_time;

    // 첫째 날 부터 다른 감염자와 동선이 겹친 것이 있는지 확인'
    for (int i=0; i < 4; i++) {
        // 환자의 i 번째 위치와 시간
        patient_place = ifctele_getHistPlaceIndex(patient, i);
        patient_time = ifctele_getinfestedTime(patient) - 4 + i;

        // 다른 감염자와 동선 비교
        for (int j = 0; j < ifctdb_len(); j++) {
            if (j == patient_no) continue;

            // 확인할 다른 감염자
            void* infester = ifctdb_getData(j);
            int infester_detected_time = ifctele_getinfestedTime(infester);

            // TIME_HIDE 이내의 시간만 확인
            for (int time = 0; time < TIME_HIDE; time++) {
                // 만약 시간 & 장소가 같다면
                if (ifctele_getHistPlaceIndex(infester, 4 - time) == patient_place
                    && patient_time == infester_detected_time - time) {
                    // detected_time, place 를 전파자를 기준 으로 변경
                    *detected_time = infester_detected_time - time;
                    *place = patient_place;

                    // 전파자 번호 리턴
                    return ifctele_getIndex(infester);
                }
            }
        }
    }

    // 전파자가 없는경우 (최초 전파자) 인 경우는 -1 를 리턴
    return -1;
}

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    // pIndex, age, time 의 값을 파일 에서 읽어옴
    while ( 3 == fscanf(fp, "%d %d %d ", &pIndex, &age, &time))
    {
        // placeHist 의 값을 N_HISTORY 개 만큼 파일 에서 읽어옴
        for (int i = 0; i < N_HISTORY; i++)
        {
            fscanf(fp, "%d ", &placeHist[i]);
        }

        // 읽어온 값들을 ifct_database 에 각각의 노드로 저장
        ifct_element = ifctele_genElement(pIndex, age, time, placeHist);
        ifctdb_addTail(ifct_element);
    }

    //1-3. FILE pointer close
    fclose(fp);
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
                printf("정보를 확인할 환자의 번호를 입력해 주세요 : ");
                int patient_no;
                scanf("%d", &patient_no);

                void* patient = ifctdb_getData(patient_no);
                // 입력한 환자가 존재하지 않는 경우
                if (patient == NULL)
                {
                    printf("해당 환자는 존재하지 않습니다.\n");
                    break;
                }
                // 입력한 환자가 존재하는 경우
                ifctele_printElement(patient);
                break;
                
            case MENU_PLACE:
                printf("정보를 확인할 장소를 입력해 주세요 : ");
                char placeName[MAX_PLACENAME];
                scanf("%s", placeName);
                printf("=== %s 에서 감염이 확인된 환자 목록 ===\n", placeName);
                for (int i = 0; i < ifctdb_len(); i++)
                {
                    ifct_element = ifctdb_getData(i);
                    if (strcmp(placeName, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY - 1))) == 0)
                    {
                        ifctele_printElement(ifct_element);
                        break;
                    }
                }
                break;
                
            case MENU_AGE:
                printf("최소 나이를 입력해 주세요 : ");
                int minAge, maxAge;
                scanf("%d", &minAge);
                printf("최대 나이를 입력해 주세요 : ");
                scanf("%d", &maxAge);
                printf("=== %d ~ %d 세 사이의 환자 목록 ===\n", minAge, maxAge);
                for (int i = 0; i < ifctdb_len(); i++)
                {
                    void* patient = ifctdb_getData(i);
                    if (ifctele_getAge(patient) >= minAge && ifctele_getAge(patient) <= maxAge)
                    {
                        ifctele_printElement(patient);
                    }
                }
                break;
                
            case MENU_TRACK:
                printf("감염 경로를 확인할 환자의 번호를 입력해 주세요 : ");
                int patient_idx, infester_no;
                scanf("%d", &patient_idx);
                ifct_element = ifctdb_getData(patient_idx);
                if (ifct_element == NULL)
                {
                    printf("해당 환자는 존재하지 않습니다.\n");
                    break;
                }

                printf("=== %d번 환자의 감염 경로 ===\n", patient_idx);
                int detected_time = ifctele_getinfestedTime(ifct_element);
                int place = ifctele_getHistPlaceIndex(ifct_element, -1);
                printf("%d번 환자는 %d일에 %s 에서 감염이 확인 되었습니다.\n", patient_idx, detected_time, ifctele_getPlaceName(place));

                while (1)
                {
                    // infester_no = patient_idx 번 환자의 전파자
                    infester_no = trackInfester(patient_idx, &detected_time, &place);
                    // 초기 전파자가 나오면 while 문 종료
                    if (infester_no == -1) {
                        printf("%d번 환자가 초기 전파자 입니다.\n", patient_idx);
                        break;
                    }
                    printf("%d번 환자는 %d번 환자 에게 %d일에 %s 에서 감염 되었습니다.\n", patient_idx, infester_no, detected_time, ifctele_getPlaceName(place));
                    patient_idx = infester_no;
                }
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);

    return 0;
}
