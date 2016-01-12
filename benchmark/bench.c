#include "bkstring.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

uint64_t get_us_time(struct timeval start, struct timeval end) {
  return end.tv_usec - start.tv_usec;
}

uint64_t get_s_time(struct timeval start, struct timeval end) {
  return end.tv_sec - start.tv_sec;
}

void print_time(void* str_arg, struct timeval start, struct timeval end) {
  uint8_t *str = str_arg;
  uint64_t us_time = get_us_time(start, end);
  uint64_t s_time = get_s_time(start, end);

  if (us_time >= 1000000) {
    printf("%s %zus\n", str, s_time);
  } else {
    printf("%s %zuus\n", str, us_time);
  }
}

int main(int argc, char *argv[]) {

  FILE *fp;
  uint8_t line[256];
  uint8_t *filename = argv[1];
  uint8_t *word = argv[2];
  uint64_t dist = atoi(argv[3]);

  fp = fopen(filename, "r");
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }

  uint64_t lines = 100000;
  uint64_t names = 60;
  uint64_t i = 0;
  uint8_t arr[lines][names];

  struct timeval start, end, build_start, build_end, search_start, search_end;

  gettimeofday(&start, NULL);
  while (fgets(line, sizeof(line), fp)) {
    sscanf(line, "%s\n", arr[i]);
    i += 1;
  }

  i = 0;
  BKTree b = init_bktree();
  gettimeofday(&build_start, NULL);
  while (strcmp(arr[i], "\0")) {
    b.Add(arr[i], &b);
    i += 1;
  }
  gettimeofday(&build_end, NULL);


  fclose(fp);
  gettimeofday(&search_start, NULL);
  uint8_t **list = b.Search(word, dist, &b);
  gettimeofday(&search_end, NULL);
  gettimeofday(&end, NULL);
  free(list);
  clear_bktree(&b);


  print_time("build in:", build_start, build_end);
  print_time("search in:", search_start, search_end);

  print_time("total time:", start, end);

  exit(EXIT_SUCCESS);
}
