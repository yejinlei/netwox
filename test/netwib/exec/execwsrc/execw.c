#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winbase.h>

void mygets(char *str);
void mygets(char *str)
{
  int strlen_str;
  char c, *pc;

  pc = fgets(str, 79, stdin);
  if (pc == NULL) {
    /* eof */
    exit(0);
  }
  strlen_str = strlen(str);
  while (1) {
    if (strlen_str == 0) break;
    strlen_str--;
    c = str[strlen_str];
    if (c == '\r') {
      str[strlen_str] = '\0';
    } else if (c == '\n') {
      str[strlen_str] = '\0';
    } else {
      break;
    }
  }
}

int main(int argc, char * argv[])
{
  char line[80];
  int choice;

  if (argc == 1) {
    puts("Unrecognized value : ");
    exit(255);
  }

  choice = atoi(argv[1]);

  switch (choice) {
    case 1:
      break;
    case 2:
      return(123);
      break;
    case 3:
      printf("Hello\n");
      fflush(stdout);
      break;
    case 4:
      mygets(line);
      break;
    case 5:
      mygets(line);
      printf("Read %s\n", line);
      fflush(stdout);
      break;
    case 6:
      printf("Hello\n");
      fflush(stdout);
      mygets(line);
      printf("Read %s\n", line);
      fflush(stdout);
      break;
    case 7:
      Sleep(2000);
      printf("Hello\n");
      fflush(stdout);
      Sleep(2000);
      mygets(line);
      Sleep(2000);
      printf("Read %s\n", line);
      fflush(stdout);
      Sleep(2000);
      break;
    case 8:
      while(1) {
        mygets(line);
        if (line[0] == '\0') break;
        printf("Read %s\n", line);
        fflush(stdout);
      }
      break;
    case 9:
      printf("BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      printf("BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      printf("BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      printf("BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      printf("BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      fflush(stdout);
      fprintf(stderr, "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      fprintf(stderr, "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      fprintf(stderr, "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      fprintf(stderr, "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      fprintf(stderr, "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG\n");
      fflush(stderr);
      break;
  default:
    printf("Unrecognized value : %s", argv[1]);
    exit(255);
  }

  return(0);
}
