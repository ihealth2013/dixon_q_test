/* Author: sagaii
 * 2021-07-13
 
 * Implementing a Dixon Q-test Function inspired from
 * Python implementation: https://sebastianraschka.com/Articles/2014_dixon_test.html
 
    Keyword arguments:
        data = A ordered or unordered list of data points (int or float).
        left = Q-test of minimum value in the ordered list if True.
        right = Q-test of maximum value in the ordered list if True.
        q_dict = A dictionary of Q-values for a given confidence level,
            where the dict. keys are sample sizes N, and the associated values
            are the corresponding critical Q values. E.g.,
            {3: 0.97, 4: 0.829, 5: 0.71, 6: 0.625, ...}
 
 * Read about Dixon Q test here: 
 * https://www.statisticshowto.datasciencecentral.com/dixons-q-test/
 */

#include<stdio.h> 
#include "dixonQtest.h"   

 /**

 */
void quicksort(int* number, int ArrayFirstId, int ArrayLastId);

 
/* minimum is 3 NOT zero, i.e., 0.941 is for 3 incoming values */    
static double Q90[] = {
                        0.941,0.765,0.642,0.560,0.507,0.468,0.437,
                        0.412,0.392,0.376,0.361,0.349,0.338,0.329,
                        0.320,0.313,0.306,0.300,0.295,0.290,0.285,
                        0.281,0.277,0.273,0.269,0.266,0.263,0.260
                      };

static double Q95[] = {
                        0.970,0.829,0.710,0.625,0.568,0.526,0.493,
                        0.466,0.444,0.426,0.410,0.396,0.384,0.374,
                        0.365,0.356,0.349,0.342,0.337,0.331,0.326,
                        0.321,0.317,0.312,0.308,0.305,0.301,0.290
                     };

static double Q99[] = {
                        0.994,0.926,0.821,0.740,0.680,0.634,0.598,
                        0.568,0.542,0.522,0.503,0.488,0.475,0.463,
                        0.452,0.442,0.433,0.425,0.418,0.411,0.404,
                        0.399,0.393,0.388,0.384,0.380,0.376,0.372
                     };



//===================================================
//
//  @input an array to sort. 
//     quicksort needs two starting points, usualy the
//    fisrtand the last element.
// 
//===================================================

void quicksort(int* number, int ArrayFirstId, int ArrayLastId) {
    int i, j, pivot, temp;
    if (ArrayFirstId < ArrayLastId) {
        pivot = ArrayFirstId;
        i = ArrayFirstId;
        j = ArrayLastId;
        while (i < j) {
            while (number[i] <= number[pivot] && i < ArrayLastId)
                i++;
            while (number[j] > number[pivot])
                j--;
            if (i < j) {
                temp = number[i];
                number[i] = number[j];
                number[j] = temp;
            }
        }
        temp = number[pivot];
        number[pivot] = number[j];
        number[j] = temp;
        quicksort(number, ArrayFirstId, j - 1);
        quicksort(number, j + 1, ArrayLastId);
    }
}




//===================================================
//
// the function discard the small or big value in array
// 
// /* queueLength within [3,31] */ 
//
/* return -1 the array length out range
 * return 0 if there is no outlier in the current data
 * return 1 there have one outlier,and discard it,
 * return 2 thre have two oulier , and discard them.
 */
 //===================================================
 
int discard_1or2outliers_dixon(int *in_values, int *out_values, int queueLength){
     
    int discardCount = -1;

    double q_small=0.0;
    double q_big=0.0;
    double critical_value = 0.0;  //confidence level to compare with

    int sortValues[32] = { 0 };

    if (queueLength >= 3 && queueLength <= 31) {

        //0.set the discard count = 0 
        discardCount++;

        //1. copy in array value to out array 
        for (int i = 0; i < queueLength; i++) {
            sortValues[i] = in_values[i];
        }

        //2. sort the out array 
        quicksort(sortValues, 0, queueLength - 1); /* sort a NEW array */
         
        //3. /* calc Denominator = (x[n]-x[1]) */
        int denom = sortValues[queueLength - 1] - sortValues[0];
       // printf("\nQ_last=%d - Q_0=%d --> denominator=%d\n", sortValues[queueLength - 1], sortValues[0], denom);
         
        if (denom != 0) { /* avoid division by zero */

            //4. calc q_small /*  Lower Outlier: Q = (x[2]-x[1])/(x[n]-x[1]) */		
            q_small = (sortValues[1] - sortValues[0]) /(double) denom;
            //printf("\nQ_1=%d - Q_0=%d / denom=%d --> q_small: %f\n", sortValues[1], sortValues[0], denom, q_small);

            //5. calc q_big /* Higher Outlier: Q = (x[n]-x[n-1])/(x[n]-x[1]) */
            q_big = (sortValues[queueLength - 1] - sortValues[queueLength - 2]) / (double)denom;
           // printf("\nsort[last]=%d - sort[last-1]=%d / denom=%d --> q_big: %f\n", sortValues[queueLength - 1], sortValues[queueLength - 2], denom, q_big);
        }

        //6. get the critical value
        if (CONFIDENCE_LEVEL == 0) {
            critical_value = Q90[queueLength - 3];
        }
        else if (CONFIDENCE_LEVEL == 1) {
            critical_value = Q95[queueLength - 3];
        }
        else if (CONFIDENCE_LEVEL == 2) {
            critical_value = Q99[queueLength - 3];
        }
       // printf("Critical value to compare qxx: %f\n", critical_value);

        /* first confidence for n==3 */
        if (q_small > critical_value) {
           // printf("\nQ_small is an outlier\n");
            discardCount++;
            //7. discard the small outlier
            for (int i = 0; i < queueLength - 1; i++) {
                sortValues[i] = sortValues[i + 1];
            }
            sortValues[queueLength - 1] = 0;

            if (q_big > critical_value) {
               // printf("\nQ_big is an outlier\n");
                discardCount++;
                //8. discard the big outlier(before have discard the small outlier)
                sortValues[queueLength - 2] = 0;
            }
        }
        else {
            if (q_big > critical_value) {
               // printf("\nQ_big is an outlier\n");
                discardCount++;
                //9. only have big outlier ,so discard one
                sortValues[queueLength - 1] = 0;
            }
        } 

        //10. copy sortvalue to out_value
        for (int i = 0; i < queueLength; i++) {
            out_values[i] = sortValues[i];
        }

    } 
          
    return discardCount;
}
 





//===================================================
//
//   remove all outliers in array
//       
// 
//===================================================
 
int discard_all_outliers_dixon(int* in_values, int* out_values, int queueLength) {
   
    int discardCount = 0;

    if (queueLength >= 3 && queueLength <= 31) {

        int arrayLen = queueLength;
        int ret = 0;
         
        //1. copy in value to  out value array
        for (int i = 0; i < queueLength; i++) {
            out_values[i] = in_values[i];
        }
         
        do
        {
           arrayLen = arrayLen - ret;
           ret= discard_1or2outliers_dixon(out_values, out_values, arrayLen);
           discardCount = discardCount + ret;

        } while (ret>0);
              
    }
    else {
        //out_value same with in_values
        for (int i = 0; i < queueLength; i++) {
            out_values[i] = in_values[i];
        }
    }

    return discardCount;
      
}









