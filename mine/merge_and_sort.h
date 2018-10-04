#pragma once

#ifndef _MERGE_AND_SORT_HDR_
#define _MERGE_AND_SORT_HDR_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    /**
    merge_and_sort function:
      merge two random array into one sorted array.

      input:
        arrA  : random array A
        lenA  : length of array A
        arrB  : random array B
        lenB  : length of array B
        arrOut: target merged array
        lenOut: space size of arrOut
        comp  : how to compare elements, default is `bigger`

      return:
        return `arrOut`, the sorted merged array
    */
    int* merge_and_sort(
        const int* arrA, const int lenA,
        const int* arrB, const int lenB,
        int* arrOut, const int lenOut,
        int(*comp)(const int left, const int right)
        );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _MERGE_AND_SORT_HDR_
