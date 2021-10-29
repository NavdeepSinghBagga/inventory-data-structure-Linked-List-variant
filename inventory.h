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



#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED

// Including all the required header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>


// Variable to hold the inventory data containing the
// keywords and properties
char ***inventory;


// Variable to capture the time details
time_t mytime;


// Keeps a log of activity. Any errors can be checked in this file
FILE *flog;


//Structure for exercise Question 8

struct rows{
    struct columns *col;
    struct rows *down;
};

struct columns{
    char property[20];
    struct columns *next;
};

// Function Declarations
int load();
int dump();
void open_log();
int inventory_memory_allocation();
void close_log();
void write_log(char * , char *, char *);
int file_empty_check(char *);
int load();
int inventory_memory_allocation();
int add_key();
char ** string_array_memory_allocation(int);
char ** get_keys();
char ** get_properties();
char ** get_key_properties(int);
char * get_key_name(int);
char * get_property_name(int);
int * search_key(char array[]);
int * search_property(char property[]);
void search_any(char c_array[], int i_array[100][2]);
void print();
void inventory_memory_deallocation();
int dump();
void update_at_index();
void file_check();
void covert_to_integer();
int covert_to_number();
int isnum(char *);
void query_processor();
int str_cmp(char *,char*);
void new_property();

///structured link list
int main_structure();
int load_structure();
void menu_structure();
void print_structure_list();
int inventory_structure_memory_allocation();
void init_list();
#endif // INVENTORY_H_INCLUDED
