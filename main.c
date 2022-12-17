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
    description : patient_no ȯ���� �����ڸ� ����. ������ �ð�, ��ҷ� detected_time, place �� ����
    input parameters : int patient_no (������ ȯ�� ��ȣ), int* detected_time (������ �ð�), int* place (������ ���)
    return value : infester_no (������ ��ȣ) / -1 (�����ڰ� ����)
*/
int trackInfester(int patient_no, int *detected_time, int *place) {
    void* patient = ifctdb_getData(patient_no);
    int patient_place, patient_time;

    // ù° �� ���� �ٸ� �����ڿ� ������ ��ģ ���� �ִ��� Ȯ��'
    for (int i=0; i < 4; i++) {
        // ȯ���� i ��° ��ġ�� �ð�
        patient_place = ifctele_getHistPlaceIndex(patient, i);
        patient_time = ifctele_getinfestedTime(patient) - 4 + i;

        // �ٸ� �����ڿ� ���� ��
        for (int j = 0; j < ifctdb_len(); j++) {
            if (j == patient_no) continue;

            // Ȯ���� �ٸ� ������
            void* infester = ifctdb_getData(j);
            int infester_detected_time = ifctele_getinfestedTime(infester);

            // TIME_HIDE �̳��� �ð��� Ȯ��
            for (int time = 0; time < TIME_HIDE; time++) {
                // ���� �ð� & ��Ұ� ���ٸ�
                if (ifctele_getHistPlaceIndex(infester, 4 - time) == patient_place
                    && patient_time == infester_detected_time - time) {
                    // detected_time, place �� �����ڸ� ���� ���� ����
                    *detected_time = infester_detected_time - time;
                    *place = patient_place;

                    // ������ ��ȣ ����
                    return ifctele_getIndex(infester);
                }
            }
        }
    }

    // �����ڰ� ���°�� (���� ������) �� ���� -1 �� ����
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
    // pIndex, age, time �� ���� ���� ���� �о��
    while ( 3 == fscanf(fp, "%d %d %d ", &pIndex, &age, &time))
    {
        // placeHist �� ���� N_HISTORY �� ��ŭ ���� ���� �о��
        for (int i = 0; i < N_HISTORY; i++)
        {
            fscanf(fp, "%d ", &placeHist[i]);
        }

        // �о�� ������ ifct_database �� ������ ���� ����
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
                printf("������ Ȯ���� ȯ���� ��ȣ�� �Է��� �ּ��� : ");
                int patient_no;
                scanf("%d", &patient_no);

                void* patient = ifctdb_getData(patient_no);
                // �Է��� ȯ�ڰ� �������� �ʴ� ���
                if (patient == NULL)
                {
                    printf("�ش� ȯ�ڴ� �������� �ʽ��ϴ�.\n");
                    break;
                }
                // �Է��� ȯ�ڰ� �����ϴ� ���
                ifctele_printElement(patient);
                break;
                
            case MENU_PLACE:
                printf("������ Ȯ���� ��Ҹ� �Է��� �ּ��� : ");
                char placeName[MAX_PLACENAME];
                scanf("%s", placeName);
                printf("=== %s ���� ������ Ȯ�ε� ȯ�� ��� ===\n", placeName);
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
                printf("�ּ� ���̸� �Է��� �ּ��� : ");
                int minAge, maxAge;
                scanf("%d", &minAge);
                printf("�ִ� ���̸� �Է��� �ּ��� : ");
                scanf("%d", &maxAge);
                printf("=== %d ~ %d �� ������ ȯ�� ��� ===\n", minAge, maxAge);
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
                printf("���� ��θ� Ȯ���� ȯ���� ��ȣ�� �Է��� �ּ��� : ");
                int patient_idx, infester_no;
                scanf("%d", &patient_idx);
                ifct_element = ifctdb_getData(patient_idx);
                if (ifct_element == NULL)
                {
                    printf("�ش� ȯ�ڴ� �������� �ʽ��ϴ�.\n");
                    break;
                }

                printf("=== %d�� ȯ���� ���� ��� ===\n", patient_idx);
                int detected_time = ifctele_getinfestedTime(ifct_element);
                int place = ifctele_getHistPlaceIndex(ifct_element, -1);
                printf("%d�� ȯ�ڴ� %d�Ͽ� %s ���� ������ Ȯ�� �Ǿ����ϴ�.\n", patient_idx, detected_time, ifctele_getPlaceName(place));

                while (1)
                {
                    // infester_no = patient_idx �� ȯ���� ������
                    infester_no = trackInfester(patient_idx, &detected_time, &place);
                    // �ʱ� �����ڰ� ������ while �� ����
                    if (infester_no == -1) {
                        printf("%d�� ȯ�ڰ� �ʱ� ������ �Դϴ�.\n", patient_idx);
                        break;
                    }
                    printf("%d�� ȯ�ڴ� %d�� ȯ�� ���� %d�Ͽ� %s ���� ���� �Ǿ����ϴ�.\n", patient_idx, infester_no, detected_time, ifctele_getPlaceName(place));
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
