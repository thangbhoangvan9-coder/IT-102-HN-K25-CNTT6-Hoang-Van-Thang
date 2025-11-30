#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX 1000



struct Patient {
    char cardId[10];
    char name[50];
    char phone[15];
    double debt;
    int visitDays;
};

struct Record {
    char recId[20];
    char cardId[10];
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


void CreateNewPatient();
void updatePatientInfo();
void DischargePatient();
void ShowCurrentPatients(struct Patient patients[], int patientCount);
void SearchPatient();
void SortbyDebt();
void CreateMedicalRecord();

char *strcasestr_custom(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;

    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++;
            n++;
        }

        if (!*n) return (char *)haystack;
    }

    return NULL;
}

int main() {
	loadSamplePatients(); 
    int choice;
    while (1) {
        printf("\n=================================================\n");
        printf("|              QUAN LY BENH NHAN                |\n");
        printf("=================================================\n");
        printf("| 1. Tiep nhan benh nhan                         |\n");
        printf("| 2. Cap nhat thong tin benh nhan                |\n");
        printf("| 3. Xuat vien (xoa benh nhan)                   |\n");
        printf("| 4. Hien thi danh sach benh nhan                |\n");
        printf("| 5. Tim kiem benh nhan                          |\n");
        printf("| 6. Sap xep benh nhan theo debt                 |\n");
        printf("| 7. Ghi nhan kham benh                          |\n");
        printf("| 8. Xem lich su kham benh                       |\n");
        printf("| 0. Thoat                                       |\n");
        printf("=================================================\n");
        choice = readInt("Nhap lua chon: ");

        switch (choice) {
            case 1: CreateNewPatient(); break;
            case 2: updatePatientInfo(); break;
            case 3: DischargePatient(); break;
            case 4: ShowCurrentPatients(patients, patientCount); break;
            case 5: SearchPatient(); break;
            case 6: SortbyDebt(); break;
            case 7: CreateMedicalRecord(); break;
            case 8:
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
        printf("Nhap sai dinh dang. Vui long nhap so (vd: 12.5).\n");
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
        {"01", "Nguyen Van A", "0901234567", 500000, 2},
        {"02", "Tran Thi B", "0912345678", 0, 1},
        {"03", "Le Van C",  "0987654321", 1200000, 3},
        {"04", "Pham Thi D", "0933334444", 300000, 0},
        {"05", "Hoang Van E", "0971112222", 250000, 4},
        {"06", "Do Thi F",    "0969998888", 0, 2},
        {"07", "Vu Van G",    "0955556666", 780000, 1},
        {"08", "Bui Thi H",   "0944445555", 1500000, 5},
        {"09", "Nguyen Thi I", "0922223333", 600000, 2},
        {"10", "Le Van J",     "0933332222", 0, 1},
        {"11", "Pham Van K",   "0944446666", 400000, 3},
        {"12", "Tran Thi L",   "0955557777", 800000, 4}
    };

    for (int i = 0; i < 12; i++) {
        patients[patientCount++] = sample[i];
    }

    printf("Da tai 12 benh nhan mau.\n");
}



void CreateNewPatient() {
    struct Patient patient;
    char buf[100];

    readNonEmpty("Nhap ma ho so: ", buf, sizeof(buf));
    if (strlen(buf) >= sizeof(patient.cardId)) {
        printf("Ma ho so qua dai.\n");
        return;
    }
    strcpy(patient.cardId, buf);

    if (findPatient(patient.cardId) != -1) {
        printf("Ma ho so da ton tai!\n");
        return;
    }

    readNonEmpty("Nhap ten: ", patient.name, sizeof(patient.name));

    readNonEmpty("Nhap so dien thoai: ", patient.phone, sizeof(patient.phone));
    for (int i = 0; patient.phone[i] != '\0'; i++) {
        if (!isdigit((unsigned char)patient.phone[i])) {
            printf("So dien thoai phai chi chua chu so!\n");
            return;
        }
    }

    patient.debt = readDouble("Nhap cong no ban dau: ");
    if (patient.debt < 0) {
        printf("Debt khong hop le!\n");
        return;
    }

    patient.visitDays = 0;
    patients[patientCount++] = patient;

    printf("Tiep nhan benh nhan thanh cong!\n");
}

void updatePatientInfo() {
    char id[10];
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

    readNonEmpty("Nhap SDT moi: ", buf, sizeof(buf));
    strcpy(patients[index].phone, buf);

    double newDebt = readDouble("Nhap no moi: ");
    patients[index].debt = newDebt;

    printf("Cap nhat thanh cong!\n");
}

void DischargePatient() {
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
        printf("Benh nhan con no: %.2lf VND\n", patients[index].debt);
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

void ShowCurrentPatients(struct Patient patients[], int patientCount) {
    if (patientCount == 0) {
        printf("Danh sach benh nhan hien dang trong.\n");
        return;
    }

    int itemsPerPage = 2;     
    int totalPages = (patientCount + itemsPerPage - 1) / itemsPerPage; 
    int page = 1;

    while (1) {
        printf("\n==== DANH SACH BENH NHAN - Trang %d/%d ====\n", page, totalPages);

        int start = (page - 1) * itemsPerPage;  
        int end = start + itemsPerPage - 1;

        if (start >= patientCount) {
            printf("+-----------------------------------------------------------+\n");
            printf("|                    KHONG CO DU LIEU                       |\n");
            printf("+-----------------------------------------------------------+\n");
        } else {
            for (int i = start; i <= end && i < patientCount; i++) {
                printf("+-----------------------------------------------------------+\n");
                printf("| Ma ho so: %-47s |\n", patients[i].cardId);
                printf("| Ho ten: %-49s |\n", patients[i].name);
                printf("| So dien thoai: %-42s |\n", patients[i].phone);
                printf("| So no: %-51.2lf|\n", patients[i].debt);
                printf("| So ngay kham benh: %-39d|\n", patients[i].visitDays);
                printf("+-----------------------------------------------------------+\n");
            }
        }

        
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


void SearchPatient() {
    char keyword[50];
    int continueSearch = 1;

    while (continueSearch) {
        int found = 0;


        do {
            readLine("Nhap ten benh nhan can tim: ", keyword, sizeof(keyword));
            if (strlen(keyword) == 0)
                printf("Khong duoc de rong. Vui long nhap lai!\n");
        } while (strlen(keyword) == 0);

        printf("\n=== Ket qua tim kiem ===\n");

        for (int i = 0; i < patientCount; i++) {                            ////strcasestr_custom
            if (strstr(patients[i].name, keyword) != NULL) {
                printf("Ma ho so: %s\n", patients[i].cardId);
                printf("Ho ten: %s\n", patients[i].name);
                printf("SDT: %s\n", patients[i].phone);
                printf("No: %.2lf\n", patients[i].debt);
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

void SortbyDebt() {
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

    int itemsPerPage = 3;
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

        printf("\n===== DANH SACH SAU KHI SAP XEP  Trang %d/%d =====\n", page, totalPages);
        printf("MaBN - TenBN - SDT - Debt - SoNgayKham\n");
        printf("-----------------------------------------------------------\n");

        for (int i = start; i < end && i < patientCount; i++) {
            printf("%s - %s - %s - %.0lf - %d\n",
                   patients[i].cardId,
                   patients[i].name,
                   patients[i].phone,
                   patients[i].debt,
                   patients[i].visitDays);
        }

        printf("-----------------------------------------------------------\n");
    }
}




int isValidDate(const char *d) {
    int day, month, year;
    if (sscanf(d, "%d/%d/%d", &day, &month, &year) != 3)
        return 0;
    if (year < 1000 || year > 3000)
        return 0;
    if (month < 1 || month > 12)
        return 0;
    if (day < 1 || day > 31)
        return 0;

    return 1;
}

void CreateMedicalRecord() {
    char cardId[20], date[20];
    char buf[100];

    printf("\n===== TAO RECORD KHAM BENH =====\n");

    
    readNonEmpty("Nhap ma benh nhan: ", buf, sizeof(buf));
    strcpy(cardId, buf);

    int index = findPatient(cardId);
    if (index == -1) {
        printf("Khong tim thay benh nhan co ma %s.\n", cardId);
        return;
    }

    while (1) {
        readNonEmpty("Nhap ngay kham (dd/mm/YYYY): ", buf, sizeof(buf));
        strcpy(date, buf);

        if (isValidDate(date)) break;

        printf("Dinh dang ngay kham khong hop le! Vui long nhap dung dinh dang.\n");
    }

    if (recordExists(cardId, date)) {
        printf("Benh nhan %s da duoc ghi nhan lich kham vao ngay %s truoc do.\n",
               cardId, date);
        return;
    }

    patients[index].visitDays++;

    struct Record rec;
    sprintf(rec.recId, "REC%04d", recordCount + 1);
    strcpy(rec.cardId, cardId);
    strcpy(rec.date, date);
    strcpy(rec.status, "kham benh");

    records[recordCount++] = rec;

    printf("Ghi nhan lich kham ngay %s cho benh nhan %s thanh cong.\n",
           date, cardId);
}
