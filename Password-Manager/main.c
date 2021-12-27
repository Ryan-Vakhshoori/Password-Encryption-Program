#include "password_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  if (check_master_password_existence()) {
    while (!verify_master_password()) {
      printf("Incorrect master password entered.\n");
    }
  } else {
    FILE* fp = fopen("master_password.txt", "w");
    printf("Set master password: ");
    char* entered_master_password = read_any_len_str();
    for (int i = 0; i < strlen(entered_master_password); ++i) {
      fprintf(fp, "%c", entered_master_password[i] + ENCRYPT_VALUE);
    }
    free(entered_master_password);
    fclose(fp);
  }
  Data data = read_into_struct();
  char command;
  while (true) {
    printf("Enter command (type 'h' for help): ");
    command = get_command();
    if (command == 'h') {
      help();
    } else if (command == 'q') {
      save(data);
      exit(0);
    } else if (command == 'a') {
      data = add_entry(data);
    } else if (command == 'd') {
      data = del_entry(data);
    } else if (command == 'e') {
      data = edit_entry(data);
    } else if (command == 'v') {
      view(data);
    } else if (command == 'i') {
      printf("Invalid command\n");
    } else if (command == 'l') {
      list_entries(data);
    }
  }
  return 0;
}
