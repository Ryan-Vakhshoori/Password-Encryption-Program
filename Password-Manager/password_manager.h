#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H
#define ENCRYPT_VALUE 100
#include <stdbool.h>
#include <stdio.h>

typedef struct Entry_struct {
  char* site_name;
  char* username;
  char* password;
} Entry;
typedef struct Data_struct {
  int num_of_passwords;
  Entry* data;
} Data;
char* read_any_len_str();
char* append(char val, char* str, int* size);
bool check_master_password_existence();
bool verify_master_password();
char* read_any_len_str_from_file(FILE* fp);
char get_command();
void help();
Data add_entry(Data data);
Data read_into_struct();
void save(Data data);
Data del_entry(Data data);
Data edit_entry(Data data);
void view(Data data);
int find_pos_of_new_entry(Data data, char* site_name);
int binary_search(Data data, char* site_name);
char* string_to_lower(char* str);
char* encrypt_string(char* str);
char* decrypt_string(char* str);
void read_invalid_input();
void list_entries(Data data);

#endif
