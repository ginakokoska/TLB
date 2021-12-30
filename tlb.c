/* tlb.c
 * Autor: Gina Kokoska
 * Datum: 14.11.21
 */


#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <sched.h>
#include <unistd.h>
#include <getopt.h>
#include "tlb.h"

/* request Pagesize with 'getconf PAGESIZE'*/
#define PAGESIZE 4096


struct timespec start, end, result;
struct timespec loop_s, loop_e, loop_r;
struct timespec tlb_s, tlb_e, tlb_r;
struct timespec gettime_s, gettime_e, gettime_r;
struct timespec time_s, time_e;
struct timespec raw_s, raw_e, raw_r;


int main(int argc, char* argv[]) {


        int NUMPAGES = 0;
        int TRIALS = 0;
        int STRAY = NUMPAGES * 0.1;
        int JUMP = PAGESIZE / sizeof(int);
        int opt;


        while((opt = getopt(argc, argv, "hn:l:")) != -1) {
                switch (opt) {
                        case 'h':
                                printf("Flags\n\n-h, --help\tShows all Flags with description!\n"
                                        "-n, --numpages\tDefines number of pages\n"
                                        "-l, --trials\tDefines number of trials in order to get a reasonable mesaurement.\n");
                                exit(1);
                        case 'n':
                                NUMPAGES = atoi(optarg);
                                break;
                        case 'l':
                                TRIALS = atoi(optarg);
                                break;
                        default:
                                fprintf(stderr, "Wrong format! Please type: ./<Program name> -n <Numpage> -l <Trials> \n");
                                exit(1);
                }
        }
       if (NUMPAGES <= 0 || TRIALS <= 0) {
                 fprintf(stderr, "Wrong format! Please type: ./<Program name> -n <Numpage> -l <Trials> \n");
                 exit(1);
        }

        int *a = (int *) malloc(sizeof(int) * (NUMPAGES * JUMP));
                if(a == 0) {
                fprintf(stderr,"clockfailed!\n");
                exit(1);

                }
        int *time = (int *) malloc(sizeof(int) * TRIALS);
                if(time == 0) {
                fprintf(stderr,"clockfailed!\n");
                exit(1);
                }

/*-------------------------------------------------------------functions------------------------------*/


        /*function to calculate elapsed timep*/
        typedef uint64_t(*timeconvert)(struct timespec, struct timespec, struct timespec);

        uint64_t convert(struct timespec start, struct timespec end, struct timespec result){
                result.tv_sec = end.tv_sec - start.tv_sec;
                result.tv_nsec = end.tv_nsec - start.tv_nsec;
        uint64_t res = (result.tv_sec * 1000000000) + result.tv_nsec;
                return(res);
        }



        /*function to measure raw loop*/
        typedef uint64_t(*gettimediff)(struct timespec, struct timespec, struct timespec, int JUMP);

        uint64_t loopfunc(struct timespec loop_s, struct timespec loop_e, struct timespec loop_r, int JUMP) {
                clock_gettime(CLOCK_MONOTONIC, &loop_s);
                for (int i = 0; i < NUMPAGES * JUMP; i+= JUMP) {};
                clock_gettime(CLOCK_MONOTONIC, &loop_e);
                timeconvert loop_c = &convert;
                return(loop_c(loop_s, loop_e, loop_r));

        }


        /*function to set to one cpu*/
        typedef void(*setcpu)();

        void cpuaffinity() {
        cpu_set_t my_set;
        CPU_ZERO(&my_set);
        CPU_SET(7, &my_set);
        sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
        }


        /*calculate average, eliminate overhead*/
        typedef int(*calculate_avg)(int *arr, int TRIALS);

        int getavg(int *arr, int TRIALS) {
                int sum;
                /*cmp needed for qsort*/
                int cmp (const void * a, const void * b) {
                        return ( *(int*)a - *(int*)b );
                }
                /*use quicksort to sort array then cut off strays */
                qsort(arr, TRIALS, sizeof(int), cmp);
                for (int i = STRAY; i < TRIALS-STRAY; i++) {
                      //printf("[%d] %d, ", i, arr[i]);
                        sum += arr[i];
                }

                //printf("sum: %d\n", sum);
                int avg = (sum/(TRIALS - (2*STRAY)) / NUMPAGES);
                return(avg);
        }


/*------start program---------------------------------------------------------------------*/

        /*set CPU*/
        setcpu myaffinity = &cpuaffinity;
        myaffinity();


        /*measurement raw loop*/
        gettimediff rawloop = &loopfunc;
        uint64_t rawloop_r = rawloop(raw_s, raw_e, raw_r, JUMP);
        rawloop_r = rawloop_r/NUMPAGES;
        //printf("raw loop time&: %lu\n", rawloop_r);


        /*clean time for clock_gettime*/
        clock_gettime(CLOCK_MONOTONIC, &gettime_s);
        clock_gettime(CLOCK_MONOTONIC, &time_s);
        clock_gettime(CLOCK_MONOTONIC, &time_e);
        clock_gettime(CLOCK_MONOTONIC, &gettime_e);
        gettimediff rawclock = &loopfunc;
        uint64_t clock = rawclock(gettime_s, gettime_e, gettime_r, JUMP);
        clock = clock/NUMPAGES;
        printf("time of timemeasurement: %lu\n", clock);

        /*outter loop to loop measuring multiple times*/
        for (int i = 0; i < TRIALS; i++) {
                /*inner loop through tlb*/
                clock_gettime(CLOCK_MONOTONIC, &tlb_s);
                for (int j = 0; j < NUMPAGES * JUMP; j+=JUMP) {
                        a[j] = j;
                }
                clock_gettime(CLOCK_MONOTONIC, &tlb_e);
                timeconvert tlbtime_c = &convert;
                uint64_t tlbtime = tlbtime_c(tlb_s, tlb_e, tlb_r);
                if (rawloop_r > tlbtime) {
                 tlbtime = tlbtime;
                } else {
                 tlbtime = tlbtime - rawloop_r - clock;
                }
                time[i] = tlbtime;
                //printf("Time for access per page entry is: %d\n", time[i]);   
                //printf("tlbtime total: %lu\n", tlbtime);
        }

        calculate_avg tlb = &getavg;
        int tlb_avg = tlb(time, TRIALS);
        //printf("tlbtime avg: %d\n", tlb_avg);


        /*save to file*/
        FILE *f = fopen("time.txt", "w");
                if (f == 0) {
                        printf("error opening file!\n");
                }

        fprintf(f, "%d", tlb_avg);
        fclose(f);


free(a);
free(time);
return 0;


}

                                                                                                                                                                                          
