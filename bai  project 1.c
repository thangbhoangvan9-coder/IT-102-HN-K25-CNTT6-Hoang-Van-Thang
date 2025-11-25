#include <stdio.h>
#include <string.h>
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


int findPatient(char cardId[]);
int recordExists(char cardId[], char date[]);
int isDischarged(char cardId[]);


void CreateNewPatient();
void updatePatientInfo();



int main() {
    int choice;

    while (1) {
        printf("-----------------------------------------------\n");
        printf("1. Tiep nhan benh nhan\n");
        printf("2. Cap nhat so dien thoai benh nhan\n");
        printf("3. Xuat vien (xoa benh nhan)\n");
        printf("4. Hien thi danh sach benh nhan\n");
        printf("5. Tim kiem benh nhan\n");
        printf("6. Sap xep benh nhan theo debt\n");
        printf("7. Ghi nhan kham benh\n");
        printf("8. Xem lich su kham benh\n");
        printf("0. Thoat\n");
        printf("-----------------------------------------------\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:{
            	CreateNewPatient();
				break;
			}
            case 2:{
            	updatePatientInfo();
				break;
			}
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
            default: printf("Lua chon khong hop le!\n");
        }
    }

    return 0;
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


void CreateNewPatient() {
    struct Patient patient;

    printf("Nhap ma ho so: ");
    scanf("%s", patient.cardId);

    if (strlen(patient.cardId) == 0 || findPatient(patient.cardId) != -1) {
        printf("ma ho so rong hoac da ton tai!\n");
        return;
    }

    printf("Nhap ten: ");
    getchar();
    fgets(patient.name, 50, stdin);
    patient.name[strcspn(patient.name, "\n")] = 0;

    printf("Nhap so dien thoai: ");
    scanf("%s", patient.phone);

    printf("Nhap cong no ban dau: ");
    scanf("%lf", &patient.debt);
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
    printf("Nhap cardId benh nhan can cap nhat: ");
    scanf("%s", id);

    
    int index = findPatient(id);
    if (index == -1) {
        printf("Khong tim thay benh nhan!\n");
        return;
    }

    
    char newName[50];
    printf("Nhap ten moi (khong duoc rong): ");
    getchar(); 
    fgets(newName, 50, stdin);
    newName[strcspn(newName, "\n")] = 0;  

    if (strlen(newName) == 0) {
        printf("Ten khong duoc rong!\n");
        return;
    }

    
    char newPhone[15];
    printf("Nhap so dien thoai moi: ");
    scanf("%s", newPhone);

    
    if (strlen(newPhone) == 0) {
        printf("So dien thoai khong duoc rong!\n");
        return;
    }

    
    for (int i = 0; newPhone[i] != '\0'; i++) {
        if (newPhone[i] < '0' || newPhone[i] > '9') {
            printf("So dien thoai phai chi chua chu so!\n");
            return;
        }
    }

    
    if (strlen(newPhone) < 9 || strlen(newPhone) > 14) {
        printf("So dien thoai khong hop le!\n");
        return;
    }

    double newDebt;
    printf("Nhap debt moi: ");
    scanf("%lf", &newDebt);

    if (newDebt < 0) {
        printf("Debt khong duoc am!\n");
        return;
    }

   
    strcpy(patients[index].name, newName);
    strcpy(patients[index].phone, newPhone);
    patients[index].debt = newDebt;

    printf("Cap nhat thong tin benh nhan thanh cong!\n");

}



