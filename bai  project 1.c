#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 1000



struct Patient {            
    char cardId[15];
    char name[50];
    char phone[15];
    double debt;
    int visitDays;
};

struct Record {       
    char recId[20];
    char cardId[15];
    char date[20];
    char status[20];
};

struct Patient patients[10000];   
struct Record records[10000];

int patientCount = 0;
int recordCount = 0;




void readLine(const char *prompt, char *buffer, int size);   
void readNonEmpty(const char *prompt, char *buffer, int size);  
int readInt(const char *prompt); 
double readDouble(const char *prompt); 
int isEmpty(char str[]); 
int findPatient(char cardId[]);
int recordExists(char cardId[], char date[]);
int isDischarged(char cardId[]);
void loadSamplePatients();


void createNewPatient();
void updatePatientInfo();
void dischargePatient();
void showCurrentPatients(struct Patient patients[], int patientCount);
void searchPatient();
void sortbyDebt();
void createMedicalRecord();
void viewMedicalHistory();



int main() {
	loadSamplePatients(); 
    int choice;
    while (1) {
        printf("\n=================================================\n");
        printf("|              QUAN LY BENH NHAN                |\n");
        printf("=================================================\n");
        printf("| F01. Tiep nhan benh nhan                         |\n");
        printf("| F02. Cap nhat thong tin benh nhan                |\n");
        printf("| F03. Xuat vien (xoa benh nhan)                   |\n");
        printf("| F04. Hien thi danh sach benh nhan                |\n");
        printf("| F05. Tim kiem benh nhan                          |\n");
        printf("| F06. Sap xep benh nhan theo debt                 |\n");
        printf("| F07. Ghi nhan kham benh                          |\n");
        printf("| F08. Xem lich su kham benh                       |\n"); 
		printf("| F00. Thoat                                       |\n");                                   
        printf("=================================================\n");
        choice = readInt("Nhap lua chon: ");

        switch (choice) {
            case 1: createNewPatient(); break;
            case 2: updatePatientInfo(); break;
            case 3: dischargePatient(); break;
            case 4: showCurrentPatients(patients, patientCount); break;
            case 5: searchPatient(); break;
            case 6: sortbyDebt(); break;
            case 7: createMedicalRecord(); break;
            case 8: viewMedicalHistory(); break;
            case 0:{
            	printf("Thoat chuong trinh.Tam biet!\n");
				return 0;
			}
            default: printf("Lua chon khong hop le!\n");
        }
    }
    return 0;
}




void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

void readNonEmpty(const char *prompt, char *buffer, int size) {
    do {
        readLine(prompt, buffer, size);
        if (strlen(buffer) == 0)
            printf("Khong duoc de rong. Vui long nhap lai.\n");
    } while (strlen(buffer) == 0);
}

int readInt(const char *prompt) {
    char line[100];
    int value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (strlen(line) == 0) {
            printf("Khong duoc de rong. Vui long nhap lai.\n");
            continue;
        }
        if (sscanf(line, "%d", &value) == 1) return value;
        printf("Nhap sai dinh dang. Vui long nhap so nguyen.\n");
    }
}

double readDouble(const char *prompt) {
    char line[100];
    double value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (strlen(line) == 0) {
            printf("Khong duoc de rong. Vui long nhap lai.\n");
            continue;
        }
        if (sscanf(line, "%lf", &value) == 1) return value;
        printf("Nhap sai dinh dang. Vui long nhap so.\n");
    }
}

int isEmpty(char str[]) {
    return strlen(str) == 0;
}

int findPatient(char cardId[]) {
    for (int i = 0; i < patientCount; i++)
        if (strcmp(patients[i].cardId, cardId) == 0)
            return i;
    return -1;
}

int recordExists(char cardId[], char date[]) {
    for (int i = 0; i < recordCount; i++)
        if (strcmp(records[i].cardId, cardId) == 0 &&
            strcmp(records[i].date, date) == 0)
            return 1;
    return 0;
}

int isDischarged(char cardId[]) {
    int lastIdx = -1;
    for (int i = 0; i < recordCount; i++)
        if (strcmp(records[i].cardId, cardId) == 0)
            lastIdx = i;
    if (lastIdx == -1) return 0;
    return strcmp(records[lastIdx].status, "da xuat vien") == 0;
}



void loadSamplePatients() {
    struct Patient sample[12] = {
        {"01234567", "Nguyen Van An",   "0901234567", 500000, 2},
        {"01234568", "Tran Thi Binh",   "0912345678", 230000, 1},
        {"01234569", "Le Van Cuong",    "0987654321", 1200000, 3},
        {"01234570", "Pham Thi Dao",    "0933334444", 0, 0},
        {"01234571", "Hoang Van Em",    "0971112222", 250000, 4},
        {"01234572", "Do Thi Ha",       "0969998888", 100000, 2},
        {"01234573", "Vu Van Giang",    "0955556666", 780000, 1},
        {"01234574", "Bui Thi Hoa",     "0944445555", 1500000, 5},
        {"01234575", "Nguyen Thi I",    "0922223333", 0, 2},
        {"01234576", "Le Van Khoa",     "0933332222", 240000, 1},
        {"01234577", "Pham Van Linh",   "0944446666", 400000, 3},
        {"01234578", "Tran Thi Ly",     "0955557777", 0, 4}
    };

    for (int i = 0; i < 12; i++) {
        patients[patientCount++] = sample[i];
    }

}

/// Case 1:
void createNewPatient() {
    struct Patient patient;
    char buf[100];

    
    while (1) {
        readNonEmpty("Nhap ma ho so(cccd): ", buf, sizeof(buf));

        if (findPatient(buf) != -1) {
            printf("Ma ho so da ton tai! Vui long nhap lai.\n");
            continue;
        }
        
         int num;
         if (sscanf(buf, "%d", &num) == 1 && num < 0) {
             printf("Ma ho so khong duoc la so am! Vui long nhap lai.\n");
             continue;
    }

    strcpy(patient.cardId, buf);
    break;
}

    

   
    readNonEmpty("Nhap ten: ", patient.name, sizeof(patient.name));

    
    while (1) {
        readNonEmpty("Nhap so dien thoai: ", patient.phone, sizeof(patient.phone));
        int len = strlen(patient.phone);
        if (len < 9) {
            printf("So dien thoai khong hop le!\n");
            continue;
        }
        int valid = 1;
        for (int i = 0; i < len; i++) {
            if (!isdigit((unsigned char)patient.phone[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("So dien thoai chi duoc chua chu so!\n");
            continue;
        }
        break;
    }

  
    patient.debt = readDouble("Nhap cong no ban dau: ");
    if (patient.debt < 0) {
        printf("Debt khong hop le!\n");
        return;
    }

    patient.visitDays = 0;
    patients[patientCount++] = patient;

    printf("Tiep nhan benh nhan thanh cong!\n");
    printf("Tong so benh nhan hien co: %d\n", patientCount);
}


// case 2
void updatePatientInfo() {
    char id[15];
    char buf[100];

    readNonEmpty("Nhap ma ho so benh nhan: ", buf, sizeof(buf));
    strcpy(id, buf);

    int index = findPatient(id);
    if (index == -1) {
        printf("Khong tim thay benh nhan!\n");
        return;
    }

    readNonEmpty("Nhap ten moi: ", buf, sizeof(buf));
    strcpy(patients[index].name, buf);

      while (1) {
        readNonEmpty("Nhap SDT moi: ", buf, sizeof(buf));
        int len = strlen(buf);
        if (len < 9) {
            printf("So dien thoai khong hop le! Phai co it nhat 9 chu so.\n");
            continue;
        }
        int valid = 1;
        for (int i = 0; i < len; i++) {
            if (!isdigit((unsigned char)buf[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("So dien thoai chi duoc chua chu so!\n");
            continue;
        }
        strcpy(patients[index].phone, buf);
        break;
    }

    printf("Cap nhat thong tin thanh cong!\n");
    
}

    


/// case 3:
void dischargePatient() {
    char id[10];
    char buf[100];

    readNonEmpty("Nhap ma ho so benh nhan can xuat vien: ", buf, sizeof(buf));
    if (strlen(buf) >= sizeof(id)) {
        printf("Ma ho so qua dai.\n");
        return;
    }
    strcpy(id, buf);

    int index = findPatient(id);
    if (index == -1) {
        printf("Khong tim thay benh nhan!\n");
        return;
    }

    if (isDischarged(id)) {
        printf("Benh nhan da xuat vien truoc do\n");
        return;
    }

  if (patients[index].debt > 0) {
        printf("Benh nhan con no: %.0lf VND\n", patients[index].debt);
        readNonEmpty("Ban co chac muon xuat vien? (y/n): ", buf, sizeof(buf));
        if (!(buf[0] == 'y' || buf[0] == 'Y')) {
            printf("benh nhan chua xuat vien.\n");
            return;
        }
    }

    for (int i = index; i < patientCount - 1; i++) {
        patients[i] = patients[i + 1];
    }
    patientCount--;


    struct Record rec;
    strcpy(rec.cardId, id);
    sprintf(rec.recId, "REC%04d", recordCount + 1);
    strcpy(rec.date, "today");
    strcpy(rec.status, "da xuat vien");
    records[recordCount++] = rec;

    printf("Xuat vien thanh cong\n");
}

/// case 4:
void showCurrentPatients(struct Patient patients[], int patientCount) {
    if (patientCount == 0) {
        printf("Danh sach benh nhan hien dang trong.\n");
        return;
    }

    int itemsPerPage = 3;     
    int totalPages = (patientCount + itemsPerPage - 1) / itemsPerPage; 
    int page = 1;

    while (1) {
        printf("\n===== DANH SACH BENH NHAN - Trang %d/%d =====\n", page, totalPages);
        printf("+------------+---------------------------+--------------+-------------+------------------+\n");
        printf("| Ma BN      | Ten BN                    | SDT          | Debt        | So ngay kham     |\n");
        printf("+------------+---------------------------+--------------+-------------+------------------+\n");

        int start = (page - 1) * itemsPerPage;
        int end = start + itemsPerPage;

        for (int i = start; i < end && i < patientCount; i++) {
            printf("| %-10s | %-25s | %-12s | %11.0lf | %-16d |\n",
                   patients[i].cardId,
                   patients[i].name,
                   patients[i].phone,
                   patients[i].debt,
                   patients[i].visitDays);
        }

        printf("+------------+---------------------------+--------------+-------------+------------------+\n");

        
        char input[20];
        int nextPage;
        while (1) {
            printf("Nhap so trang tiep theo (0 de thoat): ");
            if (!fgets(input, sizeof(input), stdin)) continue;
            input[strcspn(input, "\n")] = 0; 

            if (strlen(input) == 0) {
                printf("Khong duoc de trong. Vui long nhap lai.\n");
                continue;
            }

            if (sscanf(input, "%d", &nextPage) != 1) {
                printf("Nhap sai dinh dang. Vui long nhap so.\n");
                continue;
            }

            if (nextPage < 0 || nextPage > totalPages) {
                printf("So trang khong hop le! (0-%d)\n", totalPages);
                continue;
            }

            break; 
        }

        if (nextPage == 0) {
            printf("Thoat xem danh sach.\n");
            break;
        }

        page = nextPage;
    }
}


//// case 5:
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}


void searchPatient() {
    char keyword[50];
    int continueSearch = 1;

    while (continueSearch) {
        int found = 0;

        do {
            readLine("Nhap ten benh nhan can tim: ", keyword, sizeof(keyword));
            if (strlen(keyword) == 0)
                printf("Khong duoc de rong. Vui long nhap lai!\n");
        } while (strlen(keyword) == 0);
        /// KO PHÂN BIỆT HOA THƯỜNG
        
		char keyLower[50];
        strcpy(keyLower, keyword);
        toLowerStr(keyLower);

        printf("\n=== Ket qua tim kiem ===\n");

        for (int i = 0; i < patientCount; i++) {
        	char nameLower[50];
            strcpy(nameLower, patients[i].name);
            toLowerStr(nameLower);
            
            
            if (strstr(nameLower, keyLower) != NULL) {              /// patients[i].name, keyword
                printf("Ma ho so: %s\n", patients[i].cardId);
                printf("Ho ten: %s\n", patients[i].name);
                printf("SDT: %s\n", patients[i].phone);
                printf("No: %.0lf\n", patients[i].debt);
                printf("So ngay kham benh: %d\n", patients[i].visitDays);
                printf("-----------------------------\n");
                found = 1;
            }
        }

        if (!found) {
            printf("Khong tim thay benh nhan nao phu hop!\n");
        }

        char input[10];
        while (1) {
            printf("Ban co muon tim tiep benh nhan? (0 de thoat, bat ky phim nao de tiep tuc): ");
            if (!fgets(input, sizeof(input), stdin)) continue;
            input[strcspn(input, "\n")] = 0;

            if (strlen(input) == 0) continue;

            if (strcmp(input, "0") == 0) {
                continueSearch = 0;
            }
            break;
        }
    }
}

/// case 6: 
void sortbyDebt() {
    if (patientCount == 0) { 
        printf("Danh sach benh nhan hien dang trong.\n");
        return;  
    }

    int choice;
    printf("Chon kieu sap xep:\n");
    printf("1. Tang dan theo debt\n");
    printf("2. Giam dan theo debt\n");

    do {
        choice = readInt("Nhap lua chon (1-2, nhap 0 de thoat): ");
        if (choice == 0) return;
        if (choice != 1 && choice != 2)
            printf("Lua chon khong hop le. Vui long nhap lai.\n");
    } while (choice != 1 && choice != 2);

    
    for (int i = 0; i < patientCount - 1; i++) {
        for (int j = i + 1; j < patientCount; j++) {
            int needSwap = 0;
            if (choice == 1 && patients[i].debt > patients[j].debt)
                needSwap = 1;
            if (choice == 2 && patients[i].debt < patients[j].debt)
                needSwap = 1;
            if (needSwap) {
                struct Patient temp = patients[i];
                patients[i] = patients[j];
                patients[j] = temp;
            }
        }
    }

    int itemsPerPage = 12;
    int totalPages = (patientCount + itemsPerPage - 1) / itemsPerPage;
    int page;

    
    while (1) {

        printf("\nNhap so trang muon xem (1-%d), nhap 0 de thoat: ", totalPages);
        page = readInt("> ");

        if (page == 0) {
            printf("Thoat hien thi.\n");
            return;
        }

        if (page < 1 || page > totalPages) {
            printf("So trang khong hop le! Vui long nhap lai.\n");
            continue;
        }

        
        int start = (page - 1) * itemsPerPage;
        int end = start + itemsPerPage;

        printf("\n===== DANH SACH BENH NHAN SAU SAP XEP - Trang %d/%d =====\n", page, totalPages);
        printf("+--------+-------------------------------+--------------+-------------+------------------+\n");
        printf("| Ma BN  | Ten BN                        | SDT          | Cong no     | So ngay kham     |\n");
        printf("+--------+-------------------------------+--------------+-------------+------------------+\n");

        for (int i = start; i < end && i < patientCount; i++) {
            printf("| %-6s | %-29s | %-12s | %-11.0lf | %-16d |\n",
                   patients[i].cardId,
                   patients[i].name,
                   patients[i].phone,
                   patients[i].debt,
                   patients[i].visitDays);
        }

        printf("+--------+-------------------------------+--------------+-------------+------------------+\n");
    }

}




//// case 7:
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isValidDate(const char *d) {
    int day, month, year;

     if (sscanf(d, "%d/%d/%d", &day, &month, &year) != 3) {
        if (sscanf(d, "%d %d %d", &day, &month, &year) != 3) {
            return 0; 
        }
    }

    if (year < 1000 || year > 3000) return 0;
    if (month < 1 || month > 12) return 0;

    int maxDay;
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            maxDay = 31; break;
        case 4: case 6: case 9: case 11:
            maxDay = 30; break;
        case 2:
            maxDay = isLeapYear(year) ? 29 : 28; break;
        default:
            return 0;
    }

    if (day < 1 || day > maxDay) return 0;

    return 1; 
}


void createMedicalRecord() {
    char cardId[20], date[20];
    char buf[100];

    printf("\n===== TAO RECORD KHAM BENH =====\n");

    while (1) {
        readNonEmpty("Nhap ma benh nhan (0 de thoat): ", buf, sizeof(buf));
        if (strcmp(buf, "0") == 0) return;

        int index = findPatient(buf);
        if (index == -1) {
            printf("Khong tim thay benh nhan voi ma %s.\n", buf);
            printf("Ban co muon tao benh nhan moi? (0 de huy, bat ky phim de tao moi): ");
            char choice[10];
            readLine("", choice, sizeof(choice));
            if (strcmp(choice, "0") == 0) return;
            createNewPatient(); 
            index = findPatient(buf);
        }

        strcpy(cardId, buf);

        int continueAdding = 1; // Bien dieu khien them lich kham
        while (continueAdding) {
            while (1) {
                readNonEmpty("Nhap ngay kham (dd/mm/YYYY): ", date, sizeof(date));
                if (isValidDate(date)) break;
                printf("Dinh dang ngay kham khong hop le!\n");
            }

            if (recordExists(cardId, date)) {
                printf("Benh nhan %s da duoc ghi nhan vao ngay nay roi.\n", cardId);
            } else {
                int statusChoice;
                printf("Chon trang thai:\n1. Kham benh\n2. Tai kham\n3. Theo doi\n");
                do {
                    statusChoice = readInt("Nhap lua chon (1-3): ");
                    if (statusChoice < 1 || statusChoice > 3)
                        printf("Lua chon khong hop le.\n");
                } while (statusChoice < 1 || statusChoice > 3);

                char status[20];
                switch (statusChoice) {
                    case 1: strcpy(status, "kham benh"); patients[index].visitDays++; break;
                    case 2: strcpy(status, "tai kham"); break;
                    case 3: strcpy(status, "theo doi"); break;
                }

                struct Record rec;
                sprintf(rec.recId, "REC%04d", recordCount + 1);
                strcpy(rec.cardId, cardId);
                strcpy(rec.date, date);
                strcpy(rec.status, status);

                records[recordCount++] = rec;

                printf("Ghi nhan lich kham ngay %s cho benh nhan %s thanh cong.\n", date, cardId);
            }

            char addMore[10];
            while (1) {
                readLine("Ban co muon them ngay kham moi cho benh nhan nay khong? (y/n): ", addMore, sizeof(addMore));
                if (strlen(addMore) == 0) continue;

                if (addMore[0] == 'y' || addMore[0] == 'Y') {
                    continueAdding = 1;
                    break;
                } else if (addMore[0] == 'n' || addMore[0] == 'N') {
                    continueAdding = 0;
                    break;
                } else {
                    printf("Nhap sai. Vui long nhap y (co) hoac n (khong).\n");
                }
            }
        }
        return; 
    }
}




void viewMedicalHistory() {
    char cardId[15];

  
    do {
        readLine("Nhap ma the benh nhan: ", cardId, sizeof(cardId));
        if (strlen(cardId) == 0)
            printf("Khong duoc de rong. Vui long nhap lai!\n");
    } while (strlen(cardId) == 0);

    
    int found = 0;
    printf("\n=== LICH SU KHAM BENH ===\n");
    printf("+--------+----------------+----------------+\n");
    printf("| Rec ID | Ngay kham      | Trang thai     |\n");
    printf("+--------+----------------+----------------+\n");

    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].cardId, cardId) == 0) {
            printf("| %-6s | %-14s | %-14s |\n",
                   records[i].recId,
                   records[i].date,
                   records[i].status);
            found = 1;
        }
    }

    printf("+--------+----------------+----------------+\n");

    if (!found) {
        printf("Khong co lich su kham cho benh nhan nay!\n");
    }
}
