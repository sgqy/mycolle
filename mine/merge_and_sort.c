
#include "merge_and_sort.h"

int is_bigger(const int left, const int right)
{
    return (left > right) ? 1 : 0;
}

int __merge_and_sort_internal(
    const int* src, const int srcLen,
    int* dst, const int offset,
    int(*comp)(const int left, const int right)
    )
{
    int i = 0;
    int j = 0;

    for (i = offset; i < offset + srcLen; ++i)
    {
        dst[i] = src[i - offset];
        for (j = i - 1; j >= 0 && comp(dst[j], dst[j + 1]); --j) // insert the new element.  `j >= 0`, not `j >= offset`
        {
            int temp = dst[j];
            dst[j] = dst[j + 1];
            dst[j + 1] = temp;
        }
    }

    return i; // return true length of total sorted part (a new offset)
}

int* merge_and_sort(
    const int* arrA, const int lenA,
    const int* arrB, const int lenB,
    int* arrOut, const int lenOut,
    int(*comp)(const int left, const int right)
    )
{
    // check arguments
    if (lenOut < lenA + lenB
        || lenA < 0 || lenB < 0
        || !arrOut || lenOut <= 0)
    {
        return 0;
    }

    if (!comp)
    {
        comp = is_bigger; // default comparer
    }

    // do sort
    int offset = 0;

    if (arrA && lenA > 0)
    {
        offset = __merge_and_sort_internal(arrA, lenA, arrOut, offset, comp);
    }
    // else (arrA && lenA == 0) || (!arrA && lenA > 0)
    // pay attention to `!arrA && lenA > 0`, offset should not be lenA directly

    if (arrB && lenB > 0)
    {
        offset = __merge_and_sort_internal(arrB, lenB, arrOut, offset, comp);
    }

    return arrOut;
}
