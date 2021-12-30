/* tlb.h
 * Autor: Gina Kokoska
 * Datum: 14.11.21
 */


#ifndef FUNC_H
#define FUNC_H


uint64_t convert(struct timespec start, struct timespec end, struct timespec result);

uint64_t loopfunc(struct timespec loop_s, struct timespec loop_e, struct timespec loop_r, int TRIALS, int JUMP);

void cpuaffinity();

int getavg(int *arr, int TRIALS);

#endif
