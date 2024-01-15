#ifndef DATA_VAL_H
#define DATA_VAL_H

#pragma warning(disable : 4996)

#define TYPE_SIZE 3
#define CUSTOMERS_EXT_SIZE 61
#define CUSTOMERS_INT_SIZE 21
#define NAME_SIZE 61
#define LASTNAME_SIZE 61
#define GENDER_SIZE 2
#define CITY_SIZE 51
#define COUNTRY_SIZE 31
#define LIMIT_SIZE 11
#define EMAIL_SIZE 51
#define DATE_SIZE_FILENAME 15
#define AMOUNT_SIZE 11
#define CURRENCY_SIZE 5
#define METHOD_OF_PAYMENT_SIZE 11
#define LEN_FILE_NAME 18
#define DATE_SIZE 9
#define TIME_SIZE 7
#define MAX_ROW 100
#define MAX_ROW2 100

typedef struct CustomersData {
    char cType[TYPE_SIZE];
    char cCustomersInternalReference[CUSTOMERS_INT_SIZE];
    char cCustomersExternalReference[CUSTOMERS_EXT_SIZE];
    char cName[NAME_SIZE];
    char cLastName[LASTNAME_SIZE];
    char cGender[GENDER_SIZE];
    char cCity[CITY_SIZE];
    char cCountry[COUNTRY_SIZE];
    char cLimit[LIMIT_SIZE];
    char cEmail[EMAIL_SIZE];
}CustomersData;

typedef struct InvoicesData {
    char cType[TYPE_SIZE];
    char cCustomersInternalReference[CUSTOMERS_INT_SIZE];
    char cInvoiceDate[DATE_SIZE];
    char cInvoiceAmount[AMOUNT_SIZE];
    char cInvoiceCurrency[CURRENCY_SIZE];
}InvoicesData;

typedef struct PaymentsData {
    char cType[TYPE_SIZE];
    char cCustomersInternalReference[CUSTOMERS_INT_SIZE];
    char cDueDate[DATE_SIZE];
    char cPaymentAmount[AMOUNT_SIZE];
    char cMethodOfPayment[METHOD_OF_PAYMENT_SIZE];
    char cPaymentCurrency[CURRENCY_SIZE];
}PaymentsData;

int validateFileName(char* fileName);
int countRowsInFile(FILE* fp);

int validation(FILE* fp, char cRowForType[], char cRow[]);
void fillArray(FILE* fp, char cRow[]);
int checkForNull01(char cRow[]);

int validateRow01(FILE* fp, int result);
int checkCustInternalRef(const char* str);
int checkCustExternalRef(const char* str);
int checkName(const char* str);
int checkLastName(const char* str);
int checkGender(const char* str);
int checkCity(const char* str);
int checkCountry(const char* str);
int checkLimit(const char* str);
int checkEmail(const char* str);

int checkForNull02And03(char cRow[]);
int validateRow02(FILE* fp, int result);
int checkInvoiceDate(const char* str);
int checkInvoiceSum(const char* str);
int checkInvoiceCurrency(const char* str);

int validateRow03(FILE* fp, int result);
int checkDueDate(const char* str);
int checkPaymentsAmount(const char* str);
int checkMethodOfPayment(const char* str);
int checkPaymentCurrency(const char* str);

void fillCustomersData(CustomersData* cd, char cRow[]);
void fillInvoicesData(InvoicesData* id, char cRow[]);
void fillPaymentsData(PaymentsData* pd, char cRow[]);

#endif