#ifndef DIXONQTEST_H
#define DIXONQTEST_H
  
#define CONFIDENCE_LEVEL 1 // 0 = q90, 1 = q95, 2 = q99 

int discard_1or2outliers_dixon(int* in_values, int* out_values, int queueLength);

int discard_all_outliers_dixon(int* in_values, int* out_values, int queueLength);

#endif
