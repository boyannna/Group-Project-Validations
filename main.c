#include <stdio.h>
#include "DataValidation.h"


int main(int argc, char* argv[]) {

    char cRow[MAX_ROW];
    char cRowForType[MAX_ROW];
    CustomersData cd;
    InvoicesData id;
    PaymentsData pd;

    FILE* fp = fopen("D:\\Codix курс\\Final\\Final_roject\\file01OK.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    
        char* fileName = "IMP_CUSTOMER_DATA_20220432141559.txt";//checking for 30th February
        if (validateFileName(fileName)) {                       //the output should be invalid date
            printf("File name is valid\n");
        }
    

    int result = 0;
    int rows = countRowsInFile(fp);
    int i = 0;
    for (i = 0; i < rows; i++) {
        

        fillArray(fp, cRow);
        strcpy(cRowForType,cRow);

        if (validation(fp, cRowForType, cRow)) {
            printf("Row %d format is correct\n", i + 1);
            printf("----------------------------\n");

            //rename ok
            
            if (strcmp(cRowForType, "01") == 0) {

                fillCustomersData(&cd,cRow);
                //printf("%s", cRow);
            }
            else if (strcmp(cRowForType, "02") == 0) {

                fillInvoicesData(&id, cRow);
            }
            else if (strcmp(cRowForType, "03") == 0) {
               
                fillPaymentsData(&pd, cRow);
            }
        }
        else {
            printf("Row %d format is not correct\n", i + 1);
            printf("----------------------------\n");
            //ko
        }
       
    }

    fclose(fp);

    return 0;
}