/*
 * Authors: Matthew A. Handzy, Alex J. Topel
 * memgrind.c
 * Assignment 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.c"
#include "mymalloc.h"

#define malloc(x) mymalloc(x, __FILE__ , __LINE__ )
#define free(x) myfree(x, __FILE__ , __LINE__ )
#define MAX 5000

/*
 * 1000 separate malloc()s of 1 byte, then free() the 1000 1 byte pointers one by one
 */
double testcaseA() {

  int a, b;
  double time_total = 0;
  clock_t start, end;

  // take the average of 100 trials
  for (a = 0; a < 100; a++) {

    // start the timer
    start = clock();

    // array of returned malloc pointers
    char* pointers[1000];

    // malloc 1 byte
    for (b = 0; b < 1000; b++) {

      pointers[b] = (char*)malloc(1);

      // NULL pointer returned on failure, or malloc(0)
      if (pointers[b] == NULL) {
        return -1;
      }

    }

    // free each 1 byte pointer
    for (b = 0; b < 1000; b++) {
      free(pointers[b]);
    }

    // end the timer
    end = clock();

    // calculate the duration of a single malloc/free call and add to total
    time_total += (double)(end - start)/CLOCKS_PER_SEC;

  }

  // return the average time elapsed
  return time_total/100;

}

/*
 * First malloc() 1 byte and immediately free it - do this 1000 times
 */
double testcaseB() {

  int a, b;
  double time_total = 0;
  clock_t start, end;

  // take the average of 100 trials
  for (a = 0; a < 100; a++) {

    // start the timer
    start = clock();
    // malloc/free 1000 times consecutively
    for (b = 0; b < 1000; b++) {

      // allocate 1 byte and immediately free it
      char* pointer = malloc(1);
      free(pointer);

    }

    // end the timer
    end = clock();

    // calculate the duration of a single malloc/free call and add to total
    time_total += (double)(end - start)/CLOCKS_PER_SEC;

  }

  // return the average time elapsed
  return time_total/100;

}

/*
 *  Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer - do this 1000 times
    - Keep track of each operation so that you eventually malloc() 1000 bytes, in total
    - Keep track of each operation so that you eventually free() all pointers
 */
double testcaseC() {

  int a, r, mal_c, free_c;
  mal_c = 0;
  free_c = 0;
  int total = 0;
  double time_total = 0;
  clock_t start, end;

  // take the average of 100 time trials
  for (a = 0; a < 100; a++) {

    char* pointers[1000];
    start = clock();

      total = 0;

      while(total < 1000){
        r = rand() % 2;

        if ((r == 0 && mal_c > 0) || mal_c >= 1000-total) {
          mal_c--;
          free(pointers[mal_c]);
          free_c++;
          total++;
        }

        else if (r == 1 || mal_c == 0) {
          pointers[mal_c] = (char*)malloc(1);

          // NULL pointer returned on failure, or malloc(0)
          if (pointers[mal_c] == NULL) {
            return -1;
          }

          mal_c++;
          total++;
        }
    }

    end = clock();
    time_total += (double)(end-start)/CLOCKS_PER_SEC;
  }

  return time_total/100;
}


/*
    Randomly choose between a randomly-sized malloc() or free()ing a pointer – do this many
    times (see below)
    - Keep track of each malloc so that all mallocs do not exceed your total memory capacity
    - Keep track of each operation so that you eventually malloc() 1000 times
    - Keep track of each operation so that you eventually free() all pointers
    - Choose a random allocation size between 1 and 64 bytes
*/
double testcaseD() {

  int a, r, mal_c, free_c;
  mal_c = 0;
  free_c = 0;

  int total = 0;
  int rand_size = 0;
  double time_total = 0;
  clock_t start, end;

  // take the average of 100 time trials
  for (a = 0; a < 100; a++) {

    char* pointers[1000];
    start = clock();

      //GOAL OF LOOP for now: make sure mal_c is bigger than free_c so that free_c always trails and frees something thats already been allocated.
      //sort of ruins the point of randomness but just dont get why its not working
      total = 0;

      while (total < 1000 && 0 <= mal_c && mal_c < 1000) {

        r = rand() % 2;
        rand_size = rand() % 64 + 1;

        if ((r == 0 && mal_c > 0) || mal_c >= 1000-total) {
          mal_c--;
          free(pointers[mal_c]);
          free_c++;
          total++;
        }

        else if (r == 1 || mal_c == 0) {

          pointers[mal_c] = (char*)malloc(rand_size);

          // NULL pointer returned on failure, or malloc(0)
          if (pointers[mal_c] != NULL) {
            mal_c ++;
            total++;
          }
        }

    }

    end = clock();
    time_total += (double)(end-start)/CLOCKS_PER_SEC;
  }

  return time_total/100;

}

// custom testcase, see testcases.txt for description
double testcaseE() {

    int a, b;
    clock_t start, end;
    double time_total;

    time_total = 0;

    for(a = 0; a < 100; a++){

        start = clock();

        char* pointers[50];

        // allocate 100 bytes (4+96) 50 times to fill dynamic memory
        for (b = 0; b < 50; b++){
            pointers[b] = (char*)malloc(96);
        }

        // free each ptr to empty dynamic memory
        for(b = 0; b < 50; b++){
            free(pointers[b]);

        }

        end = clock();
        time_total+= (double)(end-start)/CLOCKS_PER_SEC;

    }

    return time_total/100;

}

// custmo testcase, see testcases.txt for description
double testcaseF() {

    int a, b;
    clock_t start, end;
    double time_total;

    time_total = 0;

    for (a = 0; a < 100; a++){

        start = clock();

        char* pointers[500];

        // allocate 100 bytes (4+96) 50 times to fill dynamic memory
        for (b = 0; b < 500; b++){
            pointers[b] = (char*)malloc(96);
        }

        // free each ptr to empty dynamic memory
        for(b = 0; b < 500; b++){
            if (pointers[b] != NULL) {
                free(pointers[b]);
            }
        }

        end = clock();
        time_total+= (double)(end-start)/CLOCKS_PER_SEC;

    }

    return time_total/100;

}

// main boiiiiiii
int main(int argc, char** argv) {
  srand(time(NULL));
  printf("Testcase A took on average %.2f microseconds to complete.\n", testcaseA()*1000000);
  printf("Testcase B took on average %.2f microseconds to complete.\n", testcaseB()*1000000);
  printf("Testcase C took on average %.2f microseconds to complete.\n", testcaseC()*1000000);
  printf("Testcase D took on average %.2f microseconds to complete.\n", testcaseD()*1000000);
  printf("Testcase E took on average %.2f microseconds to complete.\n", testcaseE()*1000000);
  printf("Testcase F took on average %.2f microseconds to complete.\n", testcaseF()*1000000);

  return 0;

}
