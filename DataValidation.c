#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "DataValidation.h"

int validateFileName(char* fileName) {
    char str[] = "IMP_CUSTOMER_DATA_";
    int result = 1;
    // Check if the file name starts with "IMP_CUSTOMER_DATA_"
    int i = 0;
    for (i = 0; i < LEN_FILE_NAME; i++) {
        if (str[i] != fileName[i]) {
            printf("Wrong name\n");
            result = 0;
            break;
        }
    }

    //Check if the date part of the file name is valid
    char dateStr[DATE_SIZE_FILENAME];
    strncpy(dateStr, &fileName[LEN_FILE_NAME], DATE_SIZE - 1);
    dateStr[DATE_SIZE - 1] = '\0';
    int year, month, day;
    int res=sscanf(dateStr, "%04d%02d%02d", &year, &month, &day);
    if (sscanf(dateStr, "%04d%02d%02d", &year, &month, &day) == 3 && ((year >= 1900 && year <= 2023) && (month >= 1  && month <= 12))) {
        if (month == 2 && !(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
            if (day > 28) {
                printf("Ivalid date\n");
                result = 0;
            }
        }
        else if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) && day > 29) {
            printf("Invalid date\n");
            result = 0;

        }
        else if ((month == 1 || month == 3 || month == 5 || month == 7 ||
            month == 8 || month == 10 || month == 12) && day > 31) {
            printf("Invalid date\n");
            result = 0;
        }
        else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            printf("Invalid date\n");
            result = 0;
        }
    }
    else {
        return 0 ;
    }

    // Check if the time part of the file name is valid
    char timeStr[TIME_SIZE];
    strncpy(timeStr, &fileName[LEN_FILE_NAME], TIME_SIZE - 1);
    timeStr[TIME_SIZE - 1] = '\0';
    int hour, minute, second;
    if (sscanf(timeStr, "%02d%02d%02d", &hour, &minute, &second) != 3 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        printf("Invalid time\n");
        result = 0;
    }
    return result;
}

int countRowsInFile(FILE* fp) {
    char c;
    int count = 1;

    for (c = getc(fp); c != EOF; c = getc(fp)) {

        if (c == '\n') {
            count++;
        }
    }

    fseek(fp, 0, SEEK_SET);

    return count;
}

int validation(FILE* fp,char cRowForType[],char cRow[]) {
    
    char* token;
    int result = 0;

    token = strtok(cRowForType, ";");

    if (strcmp(token,"01") == 0) {

        result = checkForNull01(cRow);

        if (result >= 0 && result < 4) {
            return validateRow01(fp, result);
        }
        else {
            return 0;
        }
    }
    else if (strcmp(token, "02") == 0) {

        result = checkForNull02And03(cRow);
        
        if (result == 1) {
            return validateRow02(fp, result);
        }
        else {
            return 0;
        }
    }
    else if (strcmp(token, "03") == 0) {

        result = checkForNull02And03(cRow);

        if (result == 1) {
            return validateRow03(fp, result);
        }
        else {
            return 0;
        }
    }
    else {
        printf("Invalid input: %s\n", cRowForType);
    }

    return 0;

}

void fillArray(FILE* fp,char cRow[]) {
    fgets(cRow, MAX_ROW, fp);
}

int checkForNull01(char cRow[]) {

    int iCounter = 0;
    int iResult = 0;
    int iLength = strlen(cRow);
    int i;

    for (i = 0; i < iLength; i++) {
        if (cRow[i] == ';') {
            iCounter++;
        }
        if (cRow[i] == ';' && cRow[i + 1] == ';') {

            if (iCounter == 1) {
                iResult += 1;
            }
            if (iCounter == 6) {
                iResult += 2;
            }
            if (iCounter != 1 && iCounter != 6) {
                iResult = 4;
            }
        }
    }

    // if iResult = 0 - nothing in ";;"
    // if iResult = 1 - intRef is NULL
    // if iResult = 2 - City is NULL
    // if iResult = 3 - intRef and City are NULL
    // if iResult = 4 - NULL in incorrect position

    return iResult; 
}

int validateRow01(FILE* fp, int result) {

    char cCustomersInternalReference[CUSTOMERS_INT_SIZE];
    char cCustomersExtarnalReference[CUSTOMERS_EXT_SIZE];
    char cName[NAME_SIZE];
    char cLastName[LASTNAME_SIZE];
    char cGender[GENDER_SIZE];
    char cCity[CITY_SIZE];
    char cCountry[COUNTRY_SIZE];
    char cLimit[LIMIT_SIZE];
    char cEmail[EMAIL_SIZE];
    char* token;
   
    if (result == 0) {

        token = strtok(NULL, ";");
        strncpy(cCustomersInternalReference, token, CUSTOMERS_INT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCustomersExtarnalReference, token, CUSTOMERS_EXT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cName, token, NAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLastName, token, LASTNAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cGender, token, GENDER_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCity, token, CITY_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCountry, token, COUNTRY_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLimit, token, LIMIT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cEmail, token, EMAIL_SIZE);

        if (!(checkCustInternalRef(cCustomersInternalReference) && checkCustExternalRef(cCustomersExtarnalReference) &&
            checkName(cName) && checkLastName(cLastName) && checkGender(cGender) && checkCity(cCity) &&
            checkCountry(cCountry) && checkLimit(cLimit) && checkEmail(cEmail))) {
            return 0;
        }
    }
    else if (result == 1) {

        token = strtok(NULL, ";");
        strncpy(cCustomersExtarnalReference, token, CUSTOMERS_INT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cName, token, NAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLastName, token, LASTNAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cGender, token, GENDER_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCity, token, CITY_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCountry, token, COUNTRY_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLimit, token, LIMIT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cEmail, token, EMAIL_SIZE);


        if (!(checkCustExternalRef(cCustomersExtarnalReference) && checkName(cName) && checkLastName(cLastName) && checkGender(cGender) &&
            checkCity(cCity) && checkCountry(cCountry) && checkLimit(cLimit) && checkEmail(cEmail))) {
            return 0;
        }

    }
    else if (result == 2) {

        token = strtok(NULL, ";");
        strncpy(cCustomersInternalReference, token, CUSTOMERS_INT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCustomersExtarnalReference, token, CUSTOMERS_EXT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cName, token, NAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLastName, token, LASTNAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cGender, token, GENDER_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCountry, token, COUNTRY_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLimit, token, LIMIT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cEmail, token, EMAIL_SIZE);

        if (!(checkCustInternalRef(cCustomersInternalReference) && checkCustExternalRef(cCustomersExtarnalReference) && checkName(cName) &&
            checkLastName(cLastName) && checkGender(cGender) && checkCountry(cCountry) && checkLimit(cLimit) && checkEmail(cEmail))) {
            return 0;
        }
    }
    else if (result == 3) {

        token = strtok(NULL, ";");
        strncpy(cCustomersExtarnalReference, token, CUSTOMERS_EXT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cName, token, NAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLastName, token, LASTNAME_SIZE);
        token = strtok(NULL, ";");
        strncpy(cGender, token, GENDER_SIZE);
        token = strtok(NULL, ";");
        strncpy(cCountry, token, COUNTRY_SIZE);
        token = strtok(NULL, ";");
        strncpy(cLimit, token, LIMIT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cEmail, token, EMAIL_SIZE);

        if (!(checkCustExternalRef(cCustomersExtarnalReference) && checkName(cName) && checkLastName(cLastName) && checkGender(cGender) &&
            checkCountry(cCountry) && checkLimit(cLimit) && checkEmail(cEmail))) {
            return 0;
        }
    }
    return 1;
}

int checkCustInternalRef(const char* str) {
    int length = strlen(str);

    if (length > CUSTOMERS_EXT_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }
    
    for (int i = 0; i < length; i++) {
        if (!isdigit(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkCustExternalRef(const char* str) {
    int length = strlen(str);
    
    if (strncmp(str, "EXT", 3) != 0) {
        printf("Invalid input: %s\n", str);
        return 0;
    }
   
    if (length > CUSTOMERS_INT_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }
    if (length <= 3) {
        printf("Invalid input: EXT not finished\n");
        return 0;
    }
    
    for (int i = 3; i < length; i++) {
        if (!isdigit(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkName(const char* str) {
    int length = strlen(str);

    if (length == 0 || length > NAME_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (!isalpha(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkLastName(const char* str) {
    int length = strlen(str);
    
    if (length == 0 || length > LASTNAME_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (!isalpha(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkGender(const char* str) {
    int length = strlen(str);

    if (length != GENDER_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    if (str[0] == 'm' || str[0] == 'f') {
        return 1;
    }
    else {
        printf("Invalid input: %s\n", str);
        return 0;
    }
    return 0;
}

int checkCity(const char* str) {
    int length = strlen(str);

    if (length == 0 || length > CITY_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (!isalpha(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkCountry(const char* str) {
    int length = strlen(str);
    
    if (length == 0 || length > COUNTRY_SIZE - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (!isalpha(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkLimit(const char* str) {
    int length = strlen(str);
   
    if (length == 0 || length > LIMIT_SIZE) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (!isdigit(str[i])) {
            printf("Invalid input: %s\n", str);
            return 0;
        }
    }
    return 1;
}

int checkEmail(const char* str) {
    int length = strlen(str);
    int atIndex = -1;
    int dotIndex = -1;
    
    if (length >= EMAIL_SIZE) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (str[i] == '@') {
            atIndex = i;
        }
        else if (str[i] == '.') {
            dotIndex = i;
        }
    }

    if (atIndex == -1 || dotIndex == -1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    if (dotIndex < atIndex) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    if (dotIndex - atIndex <= 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    if (atIndex <= 0) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    if (dotIndex >= length - 1) {
        printf("Invalid input: %s\n", str);
        return 0;
    }

    return 1;
}

/////////////////////////////////////////////

int checkForNull02And03(char cRow[]) {

    int iCounter = 0;
    int iLength = strlen(cRow);
    int i;

    for (i = 0; i < iLength; i++) {
        
        if (cRow[i] == ';' && cRow[i + 1] == ';') {

            return 0;
        }
    }

    return 1;
}

int validateRow02(FILE* fp, int result) {

    char cCustomersExternalReference[CUSTOMERS_EXT_SIZE];
    char cInvoiceDate[DATE_SIZE];
    char cInvoiceAmount[AMOUNT_SIZE];
    char cInvoiceCurrency[CURRENCY_SIZE];

    char* token;

    if (result == 1) {

        token = strtok(NULL, ";");
        strncpy(cCustomersExternalReference, token, CUSTOMERS_EXT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cInvoiceDate, token, DATE_SIZE);
        token = strtok(NULL, ";");
        strncpy(cInvoiceAmount, token, AMOUNT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cInvoiceCurrency, token, CURRENCY_SIZE);

        if (!(checkCustExternalRef(cCustomersExternalReference) && checkInvoiceDate(cInvoiceDate) &&
            checkInvoiceSum(cInvoiceAmount) && checkInvoiceCurrency(cInvoiceCurrency))) {
            return 0;
        }
    }
    else {
        return 0;
    }

    return 1;
}

int checkInvoiceDate(const char* str) {
    int day, month, year;
    // Extract the day, month, and year from the input string
    sscanf(str, "%2d%2d%4d", &day, &month, &year);

    // Check if the date is valid
    int valid = 1;
    if (year < 1 || year > 9999) {
        valid = 0;
    }
    else if (month < 1 || month > 12) {
        valid = 0;
    }
    else {
        int max_day = 31; // default to 31 days
        if (month == 4 || month == 6 || month == 9 || month == 11) {
            max_day = 30;
        }
        else if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                max_day = 29; // leap year
            }
            else {
                max_day = 28;
            }
        }

        if (day < 1 || day > max_day) {
            valid = 0;
        }
    }

    return valid;
}

int checkInvoiceSum(const char* str) {
    int digitsBeforeDot = 0;
    int digitsAfterDot = 0;
    int dotSeen = 0;
    while (*str != '\0') {
        if (*str == '.') {
            if (dotSeen) {
                // More than one dot seen
                return 0;
            }
            else {
                dotSeen = 1;
            }
        }
        else if (isdigit(*str)) {
            if (dotSeen) {
                digitsAfterDot++;
            }
            else {
                digitsBeforeDot++;
            }
        }
        else {
            // Invalid character encountered
            return 0;
        }

        str++;
    }
    // Check that there are at most 10 digits before the dot and 2 after the dot
    if (digitsBeforeDot > 10 || digitsAfterDot > 2) {
        return 0;
    }

    // Check that the sum is not empty and that a dot was seen or there is no dot (integer value)
    return (digitsBeforeDot + digitsAfterDot > 0) && (dotSeen || digitsAfterDot == 0) ? 1 : 0;
}

int checkInvoiceCurrency(const char* str) {
    int length = strlen(str);
    
    if (length > CURRENCY_SIZE) {
        return 0;
    }
   
    for (int i = 0; i < length - 1; i++) {
        if (!isalpha(str[i])) {
            printf("Invalid input: %s \n", str);
            return 0;
        }
    }
    return 1;
}

/////////////////////////////////////////////

int validateRow03(FILE* fp, int result) {

    char cCustomersExternalReference[CUSTOMERS_EXT_SIZE];
    char cDueDate[DATE_SIZE];
    char cPaymentAmount[AMOUNT_SIZE];
    char cMethodOfPayment[METHOD_OF_PAYMENT_SIZE];
    char cPaymentCurrency[CURRENCY_SIZE];

    char* token;

    if (result == 1) {

        token = strtok(NULL, ";");
        strncpy(cCustomersExternalReference, token, CUSTOMERS_EXT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cDueDate, token, DATE_SIZE);
        token = strtok(NULL, ";");
        strncpy(cPaymentAmount, token, AMOUNT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cMethodOfPayment, token, METHOD_OF_PAYMENT_SIZE);
        token = strtok(NULL, ";");
        strncpy(cPaymentCurrency, token, CURRENCY_SIZE);

        if (!(checkCustExternalRef(cCustomersExternalReference) && checkDueDate(cDueDate) && checkPaymentsAmount(cPaymentAmount) &&
            checkMethodOfPayment(cMethodOfPayment) && checkPaymentCurrency(cPaymentCurrency))) {
            return 0;
        }
    }    
    else {
        return 0;
    }

    return 1;
}

int checkDueDate(const char* str) {
    // Validate the due date string
    if (!checkInvoiceDate(str)) {
        return 0;
    }

    // Parse the due date string into day, month, and year integers
    int day = atoi(str);
    int month = atoi(str + 2);
    int year = atoi(str + 4);

    // Get the current date and time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Set the current date and time to midnight
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    // Calculate the number of seconds from the epoch for the current date and time
    time_t now = mktime(&tm);

    // Set the due date to midnight
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;

    // Calculate the number of seconds from the epoch for the due date
    time_t due = mktime(&tm);

    // Calculate the number of seconds between the due date and the current date and time
    double diff = difftime(due, now);

    // Calculate the number of days left until the due date
    int days_left = diff / (24 * 60 * 60);

    // Check if the due date has already passed
    if (days_left < 0) {
        return 0;
    }

    return 1;
}

int checkPaymentsAmount(const char* str) {
    
    int digitsBeforeDot = 0;
    int digitsAfterDot = 0;
    int dotSeen = 0;
    while (*str != '\0') {
        if (*str == '.') {
            if (dotSeen) {
                // More than one dot seen
                return 0;
            }
            else {
                dotSeen = 1;
            }
        }
        else if (isdigit(*str)) {
            if (dotSeen) {
                digitsAfterDot++;
            }
            else {
                digitsBeforeDot++;
            }
        }
        else {
            // Invalid character encountered
            return 0;
        }

        str++;
    }
    // Check that there are at most 10 digits before the dot and 2 after the dot
    if (digitsBeforeDot > 10 || digitsAfterDot > 2) {
        return 0;
    }

    // Check that the sum is not empty and that a dot was seen
    return (digitsBeforeDot + digitsAfterDot > 0) && dotSeen;
}

int checkMethodOfPayment(const char* str) {
    const char* valid_methods[] = { "cash", "card", "BankTransf" };
    const int num_methods = sizeof(valid_methods) / sizeof(valid_methods[0]);

    for (int i = 0; i < num_methods; ++i) {
        if (strcmp(str, valid_methods[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int checkPaymentCurrency(const char* str) {
    // check if str is not NULL
    if (str == NULL) {
        return 0;
    }

    int len = strlen(str);
    // check if str length is at least 3
    if (len < 3) {
        return 0;
    }

    // check if first two characters are letters
    for (int i = 0; i < 2; i++) {
        if (!isalpha(str[i])) {
            return 0;
        }
    }

    // check if the currency is a valid one
    const char* valid_currencies[] = { "USD", "EUR", "GBP", "JPY", "CAD", "BGN", "CHF" };
    int num_valid_currencies = sizeof(valid_currencies) / sizeof(valid_currencies[0]);
    int is_valid_currency = 0;
    for (int i = 0; i < num_valid_currencies; i++) {
        if (strcmp(str, valid_currencies[i]) == 0) {
            is_valid_currency = 1;
            break;
        }
    }
    return is_valid_currency;

}

void fillCustomersData(CustomersData* cd, char cRow[]) {

    int result = 0;
    int temp = 0;
    int lenght = 0;

    result = checkForNull01(cRow);

    if (result == 0) { // none ;;

        temp = sscanf(cRow, "%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';']", cd->cType, cd->cCustomersInternalReference, cd->cCustomersExternalReference, cd->cName, cd->cLastName, cd->cGender, cd->cCity, cd->cCountry, cd->cLimit, cd->cEmail);
    }
    else if (result == 1) { // cCustomersExternalReference = NULL
        temp = sscanf(cRow, "%[^';'];", cd->cType);
        lenght = strlen(cd->cType);

        strcpy(cd->cCustomersInternalReference, "");

        temp = sscanf(cRow + lenght + 2, "%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';']", cd->cCustomersExternalReference, cd->cName, cd->cLastName, cd->cGender, cd->cCity, cd->cCountry, cd->cLimit, cd->cEmail);
    }
    else if (result == 2) { // cCity = NULL

        temp = sscanf(cRow, "%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';']", cd->cType, cd->cCustomersInternalReference, cd->cCustomersExternalReference, cd->cName, cd->cLastName, cd->cGender);

        lenght = strlen(cd->cType) + strlen(cd->cCustomersInternalReference) + strlen(cd->cCustomersExternalReference) + strlen(cd->cName) + strlen(cd->cLastName) + strlen(cd->cGender);

        strcpy(cd->cCity, "NULL");

        temp = sscanf(cRow + lenght + 7, "%[^';'];%[^';'];%[^';']", cd->cCountry, cd->cLimit, cd->cEmail);
    }
    else if (result == 3) { // cCustomersExternalReference = NULL and cCity = NULL
        temp = sscanf(cRow, "%[^';'];", cd->cType);
        lenght = strlen(cd->cType);
        
        strcpy(cd->cCustomersInternalReference, "");
        temp = sscanf(cRow + lenght + 2, "%[^';'];%[^';'];%[^';'];%[^';']", cd->cCustomersExternalReference, cd->cName, cd->cLastName, cd->cGender);
        strcpy(cd->cCity, "");
        lenght = 0;
        lenght = strlen(cd->cType) + strlen(cd->cCustomersExternalReference) + strlen(cd->cName) + strlen(cd->cLastName) + strlen(cd->cGender);
        temp = sscanf(cRow + lenght + 7, "%[^';'];%[^';'];%[^';']", cd->cCountry, cd->cLimit, cd->cEmail);
    }
}

void fillInvoicesData(InvoicesData* id, char cRow[]) {
    int temp = 0;
    temp = sscanf(cRow, "%[^';'];%[^';'];%[^';'];%[^';'];%[^';']", id->cType, id->cCustomersInternalReference, id->cInvoiceDate, id->cInvoiceAmount, id->cInvoiceCurrency);
}

void fillPaymentsData(PaymentsData* pd, char cRow[]) {
    int temp = 0;
    temp = sscanf(cRow, "%[^';'];%[^';'];%[^';'];%[^';'];%[^';'];%[^';']", pd->cType, pd->cCustomersInternalReference, pd->cDueDate, pd->cPaymentAmount, pd->cMethodOfPayment, pd->cPaymentCurrency);
}
