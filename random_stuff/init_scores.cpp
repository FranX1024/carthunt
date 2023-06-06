#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>

#define MAX_NAME_LENGTH 25

struct Scoreboard_row {
  wchar_t date[11];
  wchar_t name[MAX_NAME_LENGTH];
  int score;
};

Scoreboard_row scores[10];

int main() {
  for(int i = 0; i < 10; i++) {
    memcpy(scores[i].date, L"--/--/----", 11 * sizeof(wchar_t));
    swprintf(scores[i].name, MAX_NAME_LENGTH, L"Entry empty");
    scores[i].score = 0;
  }
  FILE* sbfile = fopen("../bin/scores", "wb");
  if(sbfile == NULL) {
    printf("ERROR: can't open file!\n");
  } else {
    fwrite((char*) scores, 1, sizeof(scores), sbfile);
  }
  printf("Program finished!\n");
  return 0;
}
