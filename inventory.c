/**************************************************************************************************
Copyright (c) 2016 Prakash B Hegade, Vishwanath A Telsang

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sub-license, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

***************************************************************************************************/



/**************************************************************************************************
				Modified for exercise questions
Name:		Navdeep Singh Bagga
Organisation:	KLE Technological University
Date:		03 November 2019
em@il:		navdeepsinghbagga1999@gmail.com
***************************************************************************************************/



#include "inventory.h"
#include "settings.h"


//Function:     open_log
//Description:  opens the log file to write the success or failure status
//Input param:  NULL
//Return Type:  NULL
void open_log()
{
    // Open the file
    flog = fopen("log.txt", "a+");


    // If unable to open the file, alert the user
    if(flog == NULL)
        printf("Unable to open log file. No log status will be captured.");
    else
        fprintf(flog, "%s", "_________________________START OF ITERATION________________________");

    fprintf(flog, "%s", "\n\n");
}


//Function:     close_log
//Description:  closes the log file if opened
//Input param:  NULL
//Return Type:  NULL
void close_log()
{
    if(flog != NULL) {
        // Add a message to indicate end of a iteration
        fprintf(flog, "%s", "_________________________END OF ITERATION________________________");
        fprintf(flog, "%s", "\n\n");

        // Close the file pointer
        fclose(flog);
    }
}


//Function:     write_log
//Description:  logs the status with success or failure message
//Input param:  function name, status and message to display
//Return Type:  NULL
void write_log(char * function_name, char *status, char * message)
{
    // Write the appropriate log message with associated run time
    mytime = time(NULL);
    fprintf(flog, "%s %s : %s --> %s\n\n", ctime(&mytime), function_name, status, message);

}


//Function:     file_empty_check
//Description:  Check whether file has content or not
//Input param:  name of the file
//Return Type:  integer
//              success state is returned if file has content
//              FILE_EMPTY_ERROR otherwise
int file_empty_check(char *filename)
{
    // Declaring "stat" a system call to know about content of file.
    struct stat filestat;
    stat (filename,&filestat);

    // Variables to hodl messages
    char message_failure[40];
    char message_success[40];

    strcpy(message_failure, filename);
    strcat(message_failure, " is empty");

    strcpy(message_success, filename);
    strcat(message_success, " has content");

    //check size of file and return appropriate status.
    if( filestat.st_size == 0 ){
        write_log("file_empty_check","FILE_EMPTY_ERROR",message_failure);
        return FILE_EMPTY_ERROR;
    }
    else {
        write_log("file_empty_check","SUCCESS",message_success);
        return SUCCESS;
    }
}


//Function:     inventory_memory_allocation
//Description:  dynamically allocates the memory to the inventory table
//Input param:  NULL
//Return Type:  integer
//              success status on successful memory allocation
//              MEMORY_ALLOCATION_ERROR on failure.
int inventory_memory_allocation()
{
    // Local variables
    int index1;
    int index2;

    // Calculate the required size for keywords and properties.
    // We allocate the the current count plus some additional
    // for the new incoming values for the table.
    property_size = property_size + property_count;
    key_size = key_size + key_count;

    inventory = malloc(property_size * sizeof(char**));
    if(inventory == NULL)
        return MEMORY_ALLOCATION_ERROR;

    for(index1 = 0; index1 < property_size; index1++) {
        inventory[index1] = malloc(key_size * sizeof(char*));
        if(inventory[index1] == NULL)
            return MEMORY_ALLOCATION_ERROR;
        for(index2 = 0; index2 < key_size; index2++) {
            inventory[index1][index2] = malloc(word_length * sizeof(char));
            if(inventory[index1][index2] == NULL)
                return MEMORY_ALLOCATION_ERROR;
        }
    }
    write_log("inventory_memory_allocation", "SUCCESS", "Memory allocation Successful");
    return SUCCESS;
}


//Function:     load
//Description:  loads the key_count and property_count from the respective file
//              loads the inventory table data from secondary memory to primary
//Input param:  NULL
//Return Type:  integer
//              success status on successful operation
//              respective error status code otherwise
int load()
{

    // Start the log file
    open_log();

    // Use a file pointer to open various files to load the values
    FILE *fp;

    //Local variables
    int index       = 0;
    int key_index   = 0;
    int status      = 0;
    int file_status = 0;


    //check whether key_count.txt file is empty or not.
    file_status = file_empty_check("key_count.txt");
    if (file_status == 1006)
        return FAILURE;

    //check whether property_count.txt file is empty or not.
    file_status = file_empty_check("property_count.txt");
    if (file_status == 1006)
        return FAILURE;

    //check whether inventory_file.txt file is empty or not
    file_status = file_empty_check("inventory_file.txt");
    if (file_status == 1006)
        return FAILURE;

    // Open the key_count file to read the number of keywords
    fp = fopen("key_count.txt","r");
    if(fp == NULL) {
        write_log("load", "FILE_OPEN_ERROR", "Unable to open the key_count file");
        return FAILURE;
    }

    fscanf(fp,"%d", &key_count);
    if(key_count <= 0 ) {
        write_log("load", "FAILURE", "Key count is 0 or less than 0");
        return FAILURE;
    }
    write_log("load", "SUCCESS", "Key count read successfully");
    fclose(fp);

    // Open the property_count file to read the number of properties
    fp = fopen("property_count.txt","r");
    if(fp == NULL) {
        write_log("load", "FILE_OPEN_ERROR", "Unable to open the property_count file");
        return FAILURE;
    }

    fscanf(fp,"%d", &property_count);
    if (property_count <= 0) {
        write_log("load", "FAILURE", "property count is 0 or less than 0");
        return FAILURE;
    }
    write_log("load", "SUCCESS", "Property count read successfully");
    fclose(fp);


    // Open the inventory_file to read the available inventory details
    fp = fopen("inventory_file.txt", "r");
    if (fp == NULL) {
        write_log("load", "FILE_OPEN_ERROR", "Error in opening the inventory_file");
        return FAILURE;
    }

    // Allocate the memory for inventory table
    status = inventory_memory_allocation();

    if(status ==  1002) {
        write_log("load", "MEMORY_ALLOCATION_ERROR", "No memory for inventory table");
        return FAILURE;
    }

    // Load the details from file to main memory
    while(!feof(fp)) {
        for(index = 0; index <= key_count; index++) {
            fscanf(fp, "%s ", inventory[key_index][index]);
        }
        key_index++;
    }
    fclose(fp);

    write_log("load", "SUCCESS", "Inventory Load Successful");
    return SUCCESS;
}


//Function:     add_key
//Description:  reads the data from the file and loads the key and the details
//              into the inventory-table present in main memory
//Input param:  NULL
//Return Type:  integer type
//              success status is returned if key is successfully
//              added to inventory file
//              failure status otherwise
int add_key()
{
    // Local variables
    FILE *fp;
    char key[20];
    char property[20];
    int index=0;
    char filename[256];
    int status = 0;
    int file_status = 0;

    // Read the file name from the user
    printf("\nEnter the file name which contains the keyword and property details:");
    printf("\nNOTE:\n");
    printf("1-The file name has to end with .txt extension\n");
    printf("2-The file will not be validated for error input format\n");
    printf("3-The file has to be placed in the project directory\n");
    printf("4-Keep a backup of file if required.\n");
    printf("  The file placed in directory will be deleted after load\n");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if(fp == NULL) {
        write_log("add_key", "FILE_OPEN_ERROR", "Unable to open the supplied file");
        printf("Invalid file specified. Please check.\n\n");
        return FAILURE;
    }

    // Check whether file having new key is empty
    file_status = file_empty_check(filename);
    if (file_status == 1006) {
        printf("\n\nFile having new key is empty\n");
        return FAILURE;
    }

    // Increment the key count as the new keyword will be added
    key_count++;
    fscanf(fp,"%s",key);
    strcpy(inventory[index][key_count], key);
    index++;
    // Read the data from file and load into the main memory
    while(!feof(fp))
    {
        // A small modification (or can say rectification is done)
        //fscanf(fp,"%s %s", key, property);
        fscanf(fp,"%s",property);
        strcpy(inventory[index][key_count], property);
        index++;
    }

    // Write the success log and close the file
    write_log("add_key", "SUCCESS", "The file contents were successfully copied");
    fclose(fp);

    // Delete the file as, it is no more required
    status = remove(filename);
    if(status == 0 )
        write_log("add_key", "SUCCESS", "The file was successfully deleted");
    else
        write_log("add_key", "FILE_DELETE_ERROR", "The file was not deleted. Requires manual delete.");

    return SUCCESS;
}


//Function:     string_array_memory_allocation
//Description:  dynamically allocate the memory for array of strings
//Input param:  size indicating number of strings for which the memory
//              needs to be allocated
//Return Type:  starting address on successful allocation
//              NULL on failure
char ** string_array_memory_allocation(int size)
{
    // Increase the size by one as 0th location is used for names
    size = size + 1;

    // First allocate the memory for array of pointers
    char **string_array = malloc(size * sizeof(char *));
    if(string_array == NULL)
        return NULL;

    // We then allocate memory for each of the pointer
    int index;
    for (index = 0; index < size; index++) {
       string_array[index] = (char *)malloc(word_length * sizeof(char));
       if(string_array[index] == NULL)
        return NULL;
    }

    // Return the successfully allocated array
    write_log("string_array_memory_allocation", "SUCCESS", "Memory allocated");
    return string_array;
}


//Function:     gets_keys
//Description:  loads all the keys in the character array
//Input param:  NULL
//Return Type:  pointer to character array
//              on success character array holding all keys
//              NULL otherwise
char ** get_keys()
{
    // Variable to load all the keys
    char **tstring_array;

    // Call the function to allocate the required memory
    tstring_array = string_array_memory_allocation(key_count);
    if(tstring_array == NULL)
    {
        write_log("get_keys", "MEMORY_ALLOCATION_ERROR", "No memory for keys array");
        return NULL;
    }

    // Load all the keys from inventory table
    int index=0;
    for (index = 0; index <= key_count; index++) {
        tstring_array[index] = inventory[0][index];
    }

    write_log("get_keys", "SUCCESS", "Keys successfully loaded to variable");
    return tstring_array;
}


//Function:     get_properties
//Description:  loads all the properties in the string array
//Input param:  NULL
//Return Type:  pointer to character array
//              on success character array holding all properties
//              NULL otherwise
char ** get_properties()
{
    // Variable to load all the properties
    char **tstring_array;

    // Call the function to allocate the required memory
    tstring_array = string_array_memory_allocation(property_count);
    if(tstring_array == NULL)
    {
        write_log("get_properties", "MEMORY_ALLOCATION_ERROR", "No memory for properties array");
        return NULL;
    }

    // Load all the properties from inventory table
    int index=0;
    for (index = 0; index <= property_count; index++) {
        tstring_array[index] = inventory[index][0];
    }

    write_log("get_properties", "SUCCESS", "Properties successfully loaded to variable");
    return tstring_array;
}


//Function:     get_key_properties
//Description:  loads all the properties for the given index of key,
//              from inventory table in the supplied array
//Input param:  key's index number for which all it's properties
//              has to be loaded
//Return Type:  pointer to character array
//              character array is returned holding all properties for given key
//              NULL otherwise
char ** get_key_properties(int index_number)
{
    if (index_number > key_count){
        write_log("get_key_properties", "COUNT_EXCEED_ERROR", "Invalid counter supplied");
        return NULL;
    }

    // Variable to load all the properties
    char **tstring_array;

    // Call the function to allocate the required memory
    tstring_array = string_array_memory_allocation(property_count);
    if(tstring_array == NULL)
    {
        write_log("get_key_properties", "MEMORY_ALLOCATION_ERROR", "No memory for properties array");
        return NULL;
    }

    // Load all the properties from inventory table
    int index=0;
    for (index = 0; index <= property_count; index++) {
        tstring_array[index] = inventory[index][index_number];
    }

    write_log("get_key_properties", "SUCCESS", "Requested Properties successfully loaded to variable");
    return tstring_array;
}


//Function:     get_key_name
//Description:  gets the name of the keyword at the specified location
//Input param:  integer value which mentions the index
//Return Type:  pointer to character array
//              returns the key if index exists
//              COUNT_EXCEED_ERROR otherwise
char * get_key_name(int index_number)
{
    // Check if the input supplied is in valid range
    if (index_number > key_count){
        write_log("get_key_name", "COUNT_EXCEED_ERROR", "Invalid counter supplied");
        return "COUNT_EXCEED_ERROR";
    }
    else {
        write_log("get_key_name", "SUCCESS", "Returned expected value");
        return inventory[0][index_number];
    }
}

//Function:     get_property_name
//Description:  gets the name of the property at the specified location
//Input param:  integer value which mentions the index
//Return Type:  character array
//              Returns the property if, index exists
//              COUNT_EXCEED_ERROR otherwise
char * get_property_name(int index_number)
{
    // Check if the input supplied is in valid range
    if (index_number > property_count) {
        write_log("get_property_name", "COUNT_EXCEED_ERROR", "Invalid counter supplied");
        return "COUNT_EXCEED_ERROR";
    }
    else {
        write_log("get_property_name", "SUCCESS", "Returned expected value");
        return inventory[index_number][0];
    }
}


//Function:     search_key
//Description:  gets the index of the keyword to be searched
//Input param:  character array which holds keyword to be searched
//Return Type:  pointer to integer array
//              returns the keyword index if key exists
//              NULL value otherwise
int * search_key(char tstring[20])
{
    // Local variables
    static int index[2];
    int flag = 0;
    int temp_index = 0;

    // Keyword is always found at location index [0, x]
    // We can initialize the first index to 0
    index[0] = 0;

    // Convert the supplied input to lower cases
    for (temp_index = 0; temp_index < strlen(tstring); temp_index++)
        tstring[temp_index] = tolower(tstring[temp_index]);

    // Search for the required keyword
    for (temp_index = 1; temp_index <= key_count; temp_index++) {
        if((strcmp(inventory[0][temp_index], tstring)) == 0) {
            index[1] = temp_index;
            flag = 1;
            break;
        }
    }

    // Return the status based on the search result
    if(flag ==1){
        write_log("search_key", "SUCCESS", "Key index returned");
        return index;
    }
    else{
        write_log("search_key", "RESULT_NOT_FOUND", "Invalid search Key");
        return NULL;
    }
}


//Function:     search_property
//Description:  gets the index of the property to be searched
//Input param:  character array which holds property to be searched
//Return Type:  pointer to integer array
//              returns the property index if property exists
//              NULL value otherwise
int * search_property(char tstring[20])
{
    static int index[2];
    int flag = 0;
    int temp_index = 0;

    // Keyword is always found at location index [x, 0]
    // We can initialize the second index to 0
    index[1] = 0;

    // Convert the supplied input to lower cases
    for (temp_index = 0; temp_index < strlen(tstring); temp_index++)
        tstring[temp_index] = tolower(tstring[temp_index]);

    // Search for the required keyword
    for (temp_index = 1; temp_index <= property_count; temp_index++) {
        if((strcmp(inventory[temp_index][0], tstring)) == 0) {
            index[0] = temp_index;
            flag = 1;
            break;
        }
    }

    // Return the status based on the search result
    if(flag == 1){
        write_log("search_property", "SUCCESS", "Property index returned");
        return index;
    }
    else{
        write_log("search_property", "RESULT_NOT_FOUND", "Invalid search property");
        return NULL;
    }

}


//Function:     search_any
//Description:  searches for the given word in the entire table, for all
//              the occurrences
//              returns indexes of all occurrences of word if search word exists,
//              returns -1 otherwise.
//              variable to hold indices will be supplied as input param
//Input param:  character array, word to be searched and integer array
//              to hold the indexes of found locations
//Return Type:  NULL
void search_any(char tstring[20], int search_index[100][2])
{
    // Local variables
    int spec_index;
    int key_index;
    int temp_index = 0;

    // Convert the supplied input to lower cases
    for (temp_index = 0; temp_index < strlen(tstring); temp_index++)
        tstring[temp_index] = tolower(tstring[temp_index]);

    // To load indexes of matched word
    temp_index = 1;

    // Search for the word
    for (spec_index = 0; spec_index <= property_count; spec_index++) {
        for (key_index = 0; key_index <= key_count; key_index++) {
            if((strcmp(inventory[spec_index][key_index], tstring)) == 0)  {
                    search_index[temp_index][0] = spec_index;
                    search_index[temp_index][1] = key_index;
                    temp_index++;
            }
        }
    }

    // Load the count of number of occurrences.
    // If no results found, initialize with -1
    if(temp_index == 1) {
        search_index[0][0] = -1;
        write_log("search_any", "RESULT_NOT_FOUND", "Invalid search term");
    }
    else {
        search_index[0][0] = temp_index-1;
        write_log("search_any", "SUCCESS", "Results returned in index");
    }
}


//Function:     print
//Description:  displays all the entries in inventory table
//              formatting might not look cleaner if the table
//              size is large
//Input param:  NULL
//Return Type:  NULL
void print()
{
    // Local variables indexing the key and properties
    int kindex = 0;
    int pindex = 0;

    // Run the loop and print the inventory table details
    for (pindex = 0; pindex <= property_count; pindex++) {
        for (kindex = 0; kindex <= key_count; kindex++) {
            printf("%s\t\t", inventory[pindex][kindex]);
        }
        printf("\n");
    }
}


//Function:     inventory_memory_deallocation
//Description:  free the allocated memory for inventory table
//Input param:  NULL
//Return Type:  NULL
void inventory_memory_deallocation()
{
    // Local variables
    int index1;
    int index2;
    /**
        LOOK HERE
        why property and key size are more than what is required?
    **/
    //printf("%d %d\n",property_size,key_size);
    for(index1 = 0; index1 < property_size; index1++) {
        for(index2 = 0; index2 < key_size; index2++) {
            free(inventory[index1][index2]);
        }
        free(inventory[index1]);
    }
    free(inventory);

    write_log("inventory_memory_deallocation", "SUCCESS", "Memory deallocation Successful");
}


//Function:     dump
//Description:  dumps the key_count and property_count of respective file,
//              dumps the inventory table data from main to secondary memory
//Input param:  NULL
//Return Type:  integer
//              Success status on successful operation
//              otherwise respective error status code
int dump()
{
    // Local variables
    int spec_index;
    int key_index;

    // File pointer
    FILE *fp;
    FILE *tfp;

    // Open the file in write mode and dump the latest key count
    fp = fopen("key_count.txt","w+");
    if(fp == NULL) {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the key_count file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s","\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "key_count.txt\n");
        fprintf(tfp,"%d", key_count);
        fclose(tfp);
        return FAILURE;
    }
    fprintf(fp,"%d", key_count);
    write_log("dump", "SUCCESS", "Key count dumped successfully");
    fclose(fp);

    // Open the file in write mode and dump the latest properties count
    fp = fopen("property_count.txt","w+");
    if(fp == NULL) {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the property_count file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s","\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "property_count.txt\n");
        fprintf(tfp,"%d", property_count);
        fclose(tfp);
        return FAILURE;
    }
    fprintf(fp,"%d", property_count);
    write_log("dump", "SUCCESS", "Property count dumped successfully");
    fclose(fp);

    // Open the inventory file and update the values from the main memory
    // Into the secondary storage
    fp = fopen("inventory_file.txt", "w+");
    if (fp == NULL) {
        write_log("dump", "FILE_OPEN_ERROR", "Unable to open the inventory_file");
        tfp = fopen("tempfile.txt", "a+");
        fprintf(tfp, "%s","\nFollowing contents to be added in said file");
        fprintf(tfp, "%s", "inventory_file.txt\n");
        for (spec_index = 0; spec_index <= property_count; spec_index++) {
            for (key_index = 0; key_index <= key_count; key_index++) {
                fprintf(tfp, "%s ", inventory[spec_index][key_index]);
            }
            fprintf(tfp, "%s", "\n");
        }
        fclose(tfp);
        return FAILURE;
    }

    for (spec_index = 0; spec_index <= property_count; spec_index++) {
        for (key_index = 0; key_index <= key_count; key_index++) {
            fprintf(fp, "%s ", inventory[spec_index][key_index]);
        }
        fprintf(fp, "%s", "\n");
    }
    fclose(fp);

    // Free the memory for inventory
    inventory_memory_deallocation();

    write_log("dump", "SUCCESS", "Inventory table dumped successfully");

    // Close the log file
    close_log();
    return SUCCESS;
}


/// EXERCISE

///Question 2
void update_at_index()
{
    int i,j,k_ct,p_ct;
    char str[10],chch;
    printf("Enter the 2D index where you want to update\n");
    scanf("%d%d",&i,&j);
    FILE *fp1,*fp2;
    fp1=fopen("key_count.txt", "r");
    fp2=fopen("property_count.txt", "r");
    fscanf(fp1,"%d",&k_ct);
    fscanf(fp2,"%d",&p_ct);
    fclose(fp1);
    fclose(fp2);
    if(i>k_ct)
    {
        printf("Row count is %d and given input row is %d exceeding it\nreturning to main...\n",k_ct,i);
        return;
    }
    if(j>p_ct)
    {
        printf("column count is %d and given input column is %d exceeding it\nreturning to main...\n",p_ct,j);
        return;
    }
    printf("Enter the string to be replaced\n");                                                                                                fflush(stdin);
    gets(str);
    printf("%s will be replaced by %s\n",inventory[i][j],str );
    printf("Do you still want to update it?(Y/N)\n");                                                                                           fflush(stdin);
    scanf("%c",&chch);
    if(chch=='y'||chch=='Y')
    {
        strcpy(inventory[i][j],str);
        printf("Updated successfully\n");
    }
    else
    {
        printf("Terminated as per user request\n");
    }
}

///Question 3
int isnum(char *stringg)
{
    int i=0;
    while(stringg[i])
        if(!isdigit(stringg[i++]))
            return 0;

    return 1;
}

void covert_to_integer()
{
    int i,j,numb;
    char chch;
    printf("%d %d\n",property_count,key_count);
    for(i=0;i<=property_count;i++)
    {
        for(j=0;j<=key_count;j++)
        {
            if(isnum(inventory[i][j]))
            {
                printf("Index %d %d contains integer value in string format\n",i,j);
            }
        }
    }
    printf("Do you want to convert any number from string to integer(Y/N) : ");                                                                                 fflush(stdin);
    scanf("%c",&chch);
    if(chch=='Y' || chch=='y'){
        printf("Which index value you want to use for further operations\n");
        printf("Note: give only valid input\n-->");
        scanf("%d%d",&i,&j);
        while(!isnum(inventory[i][j]) || j>key_count || i>property_count){
            printf("Invalid input please enter the correct indices to proceed\n-->");
            scanf("%d%d",&i,&j);
        }
        int numb=convert_to_number(i,j);
        printf("Converted number is %d of data-type integer of size %d\n",numb,sizeof(numb));
    }
    else
        printf("Returning to main\n");
}

int convert_to_number(int i,int j)
{
    int numb=0,k=0;
    while(inventory[i][j][k]){
        numb=numb*10+(inventory[i][j][k++]-48);
    }
    return numb;
}

///Question 4
void file_check()
{
    int abba=32;
    printf("%c\n",abba);
    FILE *fp,*fp1;
    char filename[256];
    int file_status = 0;
    printf("\nEnter the file name which contains the keyword and property details:");
    printf("\nNOTE:\n");
    printf("1-The file name has to end with .txt extension\n");
    printf("2-The file will not be validated for error input format\n");
    printf("3-The file has to be placed in the project directory\n");
    printf("4-Keep a backup of file if required.\n");
    printf("  The file placed in directory will be deleted after load\n");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if(fp == NULL) {
        write_log("add_key", "FILE_OPEN_ERROR", "Unable to open the supplied file");
        printf("Invalid file specified. Please check.\n\n");
        return FAILURE;
    }

    // Check whether file having new key is empty
    file_status = file_empty_check(filename);
    if (file_status == 1006) {
        printf("\n\nFile having new key is empty\n");
        return FAILURE;
    }
    //Now checks for no. of rows and no. of columns and words etc.
    char str[10];
    int w_ct=0;
    while(!feof(fp)){
        fscanf(fp,"%s",&str);
        w_ct++;
    }
    fclose(fp);
    if( (w_ct-1)!=property_count)
        printf("\n\nData is not in correct format\n");
    else
        printf("\n\nData is in correct format\n");
}


///Question 5
int str_cmp(char *quer,char*querr)
{   int i=0;
    if(strlen(quer)!=strlen(querr))
        return 0;
    while(quer[i]){
        if(quer[i]!=querr[i++])
            return 1;
    }
    return 0;
}

void query_processor()
{
    char str[20],quer[3],quer1[2]="<",quer2[2]=">",quer3[3]="==",quer4[3]="<=",quer5[3]=">=",quer6[3]="!=";
    int value,i,j,found=0,i_in,j_in;
    printf("Enter the property\n");
    fflush(stdin);
    scanf("%s",str);
    if(!isnum(str)){
        printf("Property is not a numerical value\treturning to main....\n");
        return;
    }
    for(i=1;i<=property_count;i++){
        for(j=1;j<=key_count;j++)
        {
            if(!strcmp(str,inventory[i][j])){
                found=1;
                i_in=i;
                j_in=j;
                break;
            }
        }
        if(found)
            break;
    }
    if(!found){
        printf("Property not found\t returning to main....\n ");
        return;
    }
    printf("Enter the condition\n");
    fflush(stdin);
    scanf("%s",quer);
    if(strcmp(quer,quer1) && strcmp(quer,quer2) && strcmp(quer,quer3) && strcmp(quer,quer4) && strcmp(quer,quer5) && strcmp(quer,quer6) )
    {
        printf("Condition incorrect \t returning to main....\n");
        return;
    }
    printf("Enter the value to process\n");
    fflush(stdin);
    scanf("%d",&value);
    int numb=convert_to_number(i_in,j_in);
    if(!strcmp(quer,quer1)){
       if(numb<value)
            printf("property %d is %s value %d\n",numb,quer1,value);
        else
            printf("property %d is not %s value %d\n",numb,quer1,value);
    }
    else if(!strcmp(quer,quer2)){
        if(numb>value)
            printf("property %d is %s value %d\n",numb,quer2,value);
        else
            printf("property %d is not %s value %d\n",numb,quer2,value);
    }
    else if(!strcmp(quer,quer3)){
        if(numb==value)
            printf("property %d is %s value %d\n",numb,quer3,value);
        else
            printf("property %d is not %s value %d\n",numb,quer3,value);
    }
    else if(!strcmp(quer,quer4)){
        if(numb<=value)
            printf("property %d is %s value %d\n",numb,quer4,value);
        else
            printf("property %d is not %s value %d\n",numb,quer4,value);
    }
    else if(!strcmp(quer,quer5)){
        if(numb>=value)
            printf("property %d is %s value %d\n",numb,quer5,value);
        else
            printf("property %d is not %s value %d\n",numb,quer5,value);
    }
    else if(!strcmp(quer,quer6)){
        if(numb!=value)
            printf("property %d is %s value %d\n",numb,quer6,value);
        else
            printf("property %d is not %s value %d\n",numb,quer6,value);
    }
}


///Question 6

void new_property()
{
    property_count++;
    dump();

    char str[20],cpy[20]="NULL";
    load();
    printf("Enter the New property\n");
    fflush(stdin);
    gets(str);
    printf("%s %s\n%d %d\n",str,cpy,property_count,key_count);
    strcpy(inventory[property_count][0],str);
    for(int i=1;i<=key_count;i++){
        strcpy(inventory[property_count][i],cpy);
    }
    dump();
    load();
}


///Question 8



void init_list()
{
    head=NULL;
}

int inventory_structure_memory_allocation()
{
    init_list();
    int i,j;
    struct rows *p,*pp=NULL;
    struct columns *q,*qq;
    for(i=0;i<=property_count_structure;i++){
        qq=NULL;
        //printf("%d \n",i);
        p=(struct rows *)malloc(sizeof(struct rows));
        if(!p)
            return MEMORY_ALLOCATION_ERROR;
        if(!head)
            head=p;
            p->col=NULL;
            p->down=NULL;
        if(pp)
            pp->down=p;
        pp=p;
        for(j=0;j<=key_count_structure;j++){
                //printf("%d ",j);
            q=(struct columns *)malloc(sizeof(struct columns));
            if(!q)
                return MEMORY_ALLOCATION_ERROR;
            if(!p->col)
                p->col=q;
            q->next=NULL;
            if(qq)
                qq->next=q;
            qq=q;
        }
        //printf("\n");
    }
    return SUCCESS;
}

int inventory_structure_memory_deallocation()
{
    struct rows *p=head,*pp;
    struct columns *q,*qq;
    while(p){
        q=p->col;
        while(q){
            qq=q;
            q=q->next;
            free(qq);
        }
        pp=p;
        p=p->down;
        free(pp);
    }
    init_list();
}

void add_key_in_structure()
{
    // Local variables
    FILE *fp;
    char key[20];
    char property[20];
    int index=0;
    char filename[256];
    int status = 0;
    int file_status = 0;

    // Read the file name from the user
    printf("\n\tEnter the file name which contains the keyword and property details:");
    printf("\n\tNOTE:\n");
    printf("\t1-The file name has to end with .txt extension\n");
    printf("\t2-The file will not be validated for error input format\n");
    printf("\t3-The file has to be placed in the project directory\n");
    printf("\t4-Keep a backup of file if required.\n");
    printf("\t  The file placed in directory will be deleted after load\n\t");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("Invalid file specified. Please check.\n\n");
        return FAILURE;
    }

    // Check whether file having new key is empty
    file_status = file_empty_check(filename);
    if (file_status == 1006) {
        printf("\n\nFile having new key is empty\n");
        return FAILURE;
    }

    // Increment the key count as the new keyword will be added
    key_count_structure++;
    struct rows *p=head;
    struct columns *q,*qq;
    // Read the data from file and load into the main memory
    while(!feof(fp))
    {
        q=p->col;
        while(q!=NULL && q->next!=NULL)
            q=q->next;
        qq=(struct columns *)malloc(sizeof(struct columns));
        qq->next=NULL;
        q->next=qq;
        fscanf(fp,"%s",qq->property);
        p=p->down;
    }

    fclose(fp);
    return SUCCESS;
}
void print_structure_list()
{
    if(!head)
    {
        printf("\tEmpty list\n");
        return;
    }
    struct rows *p=head;
    struct columns *q;
    for(int i=0;i<=property_count_structure && p;i++) {
        q=p->col;
        for(int j = 0; j <= key_count_structure && q; j++) {
            printf("%s\t\t",q->property);
            q=q->next;
        }
        printf("\n");
        p=p->down;
    }

}

int load_structure()
{

    // Use a file pointer to open various files to load the values
    FILE *fp;

    //Local variables
    int index       = 0;
    int key_index   = 0;
    int status      = 0;
    int file_status = 0;


    //check whether key_count.txt file is empty or not.
    file_status = file_empty_check("key_count.txt");
    if (file_status == 1006)
        return FAILURE;

    //check whether property_count.txt file is empty or not.
    file_status = file_empty_check("property_count.txt");
    if (file_status == 1006)
        return FAILURE;

    //check whether inventory_file.txt file is empty or not
    file_status = file_empty_check("inventory_file.txt");
    if (file_status == 1006)
        return FAILURE;

    // Open the key_count file to read the number of keywords
    fp = fopen("key_count.txt","r");
    if(fp == NULL) {
        return FAILURE;
    }

    fscanf(fp,"%d", &key_count_structure);
    if(key_count <= 0 ) {
        return FAILURE;
    }
    fclose(fp);

    // Open the property_count file to read the number of properties
    fp = fopen("property_count.txt","r");
    if(fp == NULL) {
        return FAILURE;
    }

    fscanf(fp,"%d", &property_count_structure);
    if (property_count <= 0) {
        return FAILURE;
    }
    fclose(fp);


    // Allocate the memory for inventory table
    status = inventory_structure_memory_allocation();

    if(status ==  1002) {
        return FAILURE;
    }

    // Open the inventory_file to read the available inventory details

    fp = fopen("inventory_file.txt", "r");
    if (fp == NULL) {
        return FAILURE;
    }


    // Load the details from file to main memory
    char ssttrr[20];
    struct rows *p=head;
    struct columns *q=p->col;

    for(int i=0;i<=property_count_structure;i++){
            q=p->col;
        for(index = 0; index <= key_count_structure; index++) {
            fscanf(fp, "%s ", ssttrr);
            strcpy(q->property,ssttrr);
            q=q->next;
        }
        p=p->down;
    }
    fclose(fp);

    return SUCCESS;

}

void menu_structure()
{
    printf("\t----------------------------------\n");
    printf("\n\t\tMENU_STRUCTURE\n");
    printf("\t----------------------------------\n");
    printf("\t0-exit\n");
    printf("\t1-Print Table\n");
    printf("\t2-Add a New Key\n");
    printf("\t--------------------------------\n");
}


int main_structure(){
    int statt =load_structure();
    if(statt != 1){
        printf("Load Failed\n");
        return 0;
    }
    int cho;
    while(1)
    {
        menu_structure();
        printf("\tEnter your choice\n\t");
        scanf("%d", &cho);
        switch(cho)
        {
        case 0:inventory_structure_memory_deallocation();
                printf("\tcalling print list again\n");
                print_structure_list();
                printf("\tReturning to main.....\n");
                return 0;
            break;
        case 1:print_structure_list();
            break;
        case 2:add_key_in_structure();
            break;
        }
    }
}
///LOOK HERE line no. 641
