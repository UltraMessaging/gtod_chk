/* gtod.c - Test program to see if gettimeofday() sometimes
 * returns garbage.
 * See https://github.com/fordsfords/gtod
 */
/*
# This code and its documentation is Copyright 2002-2022 Steven Ford
# and licensed "public domain" style under Creative Commons "CC0":
#   http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/gtod
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include "cprt.h"


int o_threads = 3;


void help()
{
  printf("Usage: gtod_chk [-h] [-t threads]\n"
    "where:\n"
    "-h - prints help.\n"
    "-t threads - number of parallel threads to run.\n");
  exit(0);
}  /* help */


void get_options(int argc, char **argv)
{
  int opt;

  while ((opt = cprt_getopt(argc, argv, "ht:")) != EOF) {
    switch (opt) {
      case 't':
        CPRT_ATOI(cprt_optarg, o_threads);
        break;
      case 'h':
        help();
        break;
      case '?':
        fprintf(stderr, "cprt_optopt='%c', Use '-h' for help\n", cprt_optopt);
        exit(1);
        break;
      default:
        fprintf(stderr, "Undefined option '%c'\n", opt);
        exit(1);
    }  /* switch opt */
  }  /* while cprt_getopt */
}  /* get_options */


CPRT_THREAD_ENTRYPOINT my_thread_entrypoint(void *in_arg)
{
  struct timeval tv1;
  struct timeval tv2;

  CPRT_EOK0(gettimeofday(&tv1, NULL));
  printf("tv_sec=%jd\n", (uintmax_t)tv1.tv_sec);
  for (;;) {
    CPRT_EOK0(gettimeofday(&tv2, NULL));
    if ((tv2.tv_sec - tv1.tv_sec != 0) && (tv2.tv_sec - tv1.tv_sec != 1)) {
      fprintf(stderr, "tv2.tv_sec=%ld, tv1.tv_sec=%ld\n", (long)tv2.tv_sec, (long)tv1.tv_sec);
      fflush(stderr);
      exit(1);
    }
    tv1.tv_sec = tv2.tv_sec;
  }

  CPRT_THREAD_EXIT;
  return 0;
}  /* my_thread_entrypoint */


int main(int argc, char **argv)
{
  int i;
  CPRT_THREAD_T my_thread_id;

  get_options(argc, argv);

  for (i = 0; i < o_threads; i++) {
    CPRT_THREAD_CREATE(my_thread_id, my_thread_entrypoint, NULL);
  }

  CPRT_THREAD_JOIN(my_thread_id);

  return 0;
}  /* main */
