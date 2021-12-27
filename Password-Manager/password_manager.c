#include "password_manager.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char* read_any_len_str() {
  char curr_char;
  char* str = NULL;
  int str_size = 0;
  do {
    scanf("%c", &curr_char);
  } while(isspace(curr_char));
  do {
    str = append(curr_char, str, &str_size);
    scanf("%c", &curr_char);
  } while(!isspace(curr_char));
  str = append('\0', str, &str_size);
  return str;
}

char* read_any_len_str_from_file(FILE* fp) {
  char curr_char;
  char* str = NULL;
  int str_size = 0;
  do {
    fscanf(fp, "%c", &curr_char);
  } while(isspace(curr_char));
  do {
    str = append(curr_char, str, &str_size);
    fscanf(fp, "%c", &curr_char);
  } while(!isspace(curr_char) && !feof(fp));
  str = append('\0', str, &str_size);
  return str;
}

char* append(char val, char* str, int* size) {
  (*size)++;
  str = (char*)realloc(str, *size * sizeof(char));
  str[*size - 1] = val;
  return str;
}

bool check_master_password_existence() {
  FILE* fp = fopen("master_password.txt", "r");
  if (fp == NULL) {
    return false;
  }
  return true;
}

bool verify_master_password() {
  bool status = false;
  char* entered_master_password;
  printf("Enter the master password: ");
  entered_master_password = read_any_len_str();
  for (int i = 0; i < strlen(entered_master_password); ++i) {
    entered_master_password[i] += ENCRYPT_VALUE;
  }
  FILE* fp = fopen("master_password.txt", "r");
  char* master_password = read_any_len_str_from_file(fp);
  if (strcmp(entered_master_password, master_password) == 0) {
    status = true;
  }
  free(entered_master_password);
  free(master_password);
  return status;
}

char get_command() {
  const int num_args_needed = 1;
  int num_args_read;
  char c;
  char command;
  num_args_read = scanf(" %c", &command);
  if (num_args_read == num_args_needed) {
    scanf("%c", &c);
    while (c != '\n') {
      if (!(isspace(c))) {
        read_invalid_input();
        return 'i';
      }
    }
    return command;
  }
  read_invalid_input();
  return 'i';
}

void read_invalid_input() {
  char c;
  scanf("%c", &c);
  while (c != '\n') {
    scanf("%c", &c);
  }
}

void help() {
  printf("type 'q' to quit and save\n");
  printf("type 'a' to add entry\n");
  printf("type 'd' to del entry\n");
  printf("type 'e' to edit entry\n");
  printf("type 'v' to view entry\n");
  printf("type 'l' to list entries\n");
}

Data add_entry(Data data) {
  ++data.num_of_passwords;
  if (data.num_of_passwords == 1) {
    data.data = (Entry*)malloc(data.num_of_passwords * sizeof(Entry));
  } else {
    data.data = (Entry*)realloc(data.data, data.num_of_passwords * sizeof(Entry));
  }
  Entry new;
  printf("Enter name of site: ");
  new.site_name = read_any_len_str();
  printf("Enter username: ");
  new.username = read_any_len_str();
  printf("Enter password: ");
  new.password = read_any_len_str();
  int pos = find_pos_of_new_entry(data, new.site_name);
  for (int i = data.num_of_passwords - 1; i > pos; --i) {
    data.data[i] = data.data[i - 1];
  }
  data.data[pos] = new;
  return data;
}

int find_pos_of_new_entry(Data data, char* site_name) {
  int len;
  for (int i = 0; i < data.num_of_passwords - 1; ++i) {
    if (strlen(site_name) <= strlen(data.data[i].site_name)) {
      len = strlen(site_name);
    } else {
      len = strlen(data.data[i].site_name);
    } for (int j = 0; j < len; ++j) {
      if ((tolower(site_name[j])) < (tolower(data.data[i].site_name[j]))) {
        return i;
      } else if ((tolower(site_name[j])) > (tolower(data.data[i].site_name[j]))) {
        break;
      }
    }
  }
  return (data.num_of_passwords - 1);
}

Data del_entry(Data data) {
  printf("Enter name of site: ");
  char* site_name = read_any_len_str();
  int entry_row = binary_search(data, site_name);
  free(site_name);
  if (entry_row == -1) {
    printf("Entry not found\n");
    return data;
  }
  for (int i = entry_row; i < data.num_of_passwords - 1; ++i) {
    data.data[i] = data.data[i + 1];
  }
  --data.num_of_passwords;
  data.data = (Entry*)realloc(data.data, data.num_of_passwords * sizeof(Entry));
  return data;
}

Data edit_entry(Data data) {
  printf("Enter name of site: ");
  char* site_name = read_any_len_str();
  int entry_row = binary_search(data, site_name);
  free(site_name);
  if (entry_row == -1) {
    printf("Entry not found\n");
    return data;
  }
  printf("type 'u' to change username or 'p' to change password: ");
  char choice = get_command();
  if (choice == 'u') {
    printf("New username: ");
    free(data.data[entry_row].username);
    data.data[entry_row].username = read_any_len_str();
  } else if (choice == 'p') {
    printf("New password: ");
    free(data.data[entry_row].password);
    data.data[entry_row].password = read_any_len_str();
  } else if (choice == 'i') {
    printf("Invalid command\n");
  }
  return data;
}

Data read_into_struct() {
  FILE* fp = fopen("passwords.txt", "r");
  if (fp == NULL) {
    fp = fopen("passwords.txt", "w");
    Data data;
    data.num_of_passwords = 0;
    fclose(fp);
    return data;
  }
  Data data;
  fscanf(fp, "%d", &data.num_of_passwords);
  if (data.num_of_passwords == 0) {
    return data;
  }
  data.data = (Entry*)malloc(data.num_of_passwords * sizeof(Entry));
  Entry new;
  for (int i = 0; i < data.num_of_passwords; ++i) {
    new.site_name = decrypt_string(read_any_len_str_from_file(fp));
    new.username = decrypt_string(read_any_len_str_from_file(fp));
    new.password = decrypt_string(read_any_len_str_from_file(fp));
    data.data[i] = new;
  }
  fclose(fp);
  return data;
}

char* decrypt_string(char* str) {
  for (int i = 0; i < strlen(str); ++i) {
    str[i] -= ENCRYPT_VALUE;
  }
  return str;
}

void save(Data data) {
  FILE* fp = fopen("passwords.txt", "w");
  fprintf(fp, "%d\n", data.num_of_passwords);
  for (int i = 0; i < data.num_of_passwords; ++i) {
    fprintf(fp, "%s ", encrypt_string(data.data[i].site_name));
    fprintf(fp, "%s ", encrypt_string(data.data[i].username));
    fprintf(fp , "%s\n", encrypt_string(data.data[i].password));
  }
  fclose(fp);
}

char* encrypt_string(char* str) {
  for (int i = 0; i < strlen(str); ++i) {
    str[i] += ENCRYPT_VALUE;
  }
  return str;
}

void view(Data data) {
  printf("Enter name of site: ");
  char* site_name = read_any_len_str();
  int entry_row = binary_search(data, site_name);
  if (entry_row == -1) {
    printf("Entry not found\n");
  } else {
    printf("Password for %s is %s and username is %s\n", site_name, data.data[entry_row].password, data.data[entry_row].username);
  }
  free(site_name);
}

int binary_search(Data data, char* site_name) {
  char* source;
  char* target = string_to_lower(site_name);
  int bot = 0;
  int mid;
  int top = data.num_of_passwords - 1;
  while (bot <= top) {
    mid = (bot + top) / 2;
    source = string_to_lower(data.data[mid].site_name);
    if (strcmp(source, target) == 0) {
      return mid;
    } else if (strcmp(source, target) > 0) {
      top = mid - 1;
    } else if (strcmp(source, target) < 0) {
      bot = mid + 1;
    }
  }
  return -1;
}

char* string_to_lower(char* str) {
  for (int i = 0; i < strlen(str); ++i) {
    str[i] = tolower(str[i]);
  }
  return str;
}

void list_entries(Data data) {
  for (int i = 0; i < data.num_of_passwords; ++i) {
    printf("%s\n", data.data[i].site_name);
  }
}
