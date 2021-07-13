#include "stdio.h"
#include "dixonQtest.h"
 
void main(){
     
 int  dxion_vals[] = {242, 5993 ,5997 ,5980 ,5995 ,6010 ,6400,8932 };
 
 /* if dixon_vals is float or double , you can mult 1000 or more ,convert to int */

 int  outVals[32] = { 0 };
 
 int  arrayLen = sizeof(dxion_vals) / sizeof(dxion_vals[0]);

         printf("source value:\r\n");
         for (int i = 0; i < arrayLen; i++) {
             printf("%d ", dxion_vals[i]);
         }
         printf("\r\n");

         int ret= discard_all_outliers_dixon(dxion_vals, outVals, arrayLen);

         printf("discard value: remove(%d) \r\n",ret);
         for (int i = 0; i < arrayLen-ret; i++) {
             printf("%d ", outVals[i]);
         }
         printf("\r\n");



}

