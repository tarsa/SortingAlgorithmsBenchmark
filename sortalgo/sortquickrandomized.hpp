/* 
 * sortquickrandomized.hpp -- sorting algorithms benchmark
 * 
 * Copyright (C) 2014 Piotr Tarsa ( http://github.com/tarsa )
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the author be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 * 
 */

#ifndef SORTQUICKRANDOMIZED_HPP
#define	SORTQUICKRANDOMIZED_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateRandomizedQuickSort {

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t threshold>
        void quicksort(ItemType * const a, ssize_t const left,
                ssize_t const right) {
            ItemType x;
            if (right - left > threshold) {
                x = a[(rand() % (right - left + 1)) + left];
            } else {
                x = a[left + (right - left + 1) / 2];
            }

            ssize_t i = left;
            ssize_t j = right;

            while (i < j) {
                while (compOp(a[i], Below, x)) i++;
                while (compOp(a[j], Above, x)) j--;
                if (i <= j) {
                    std::swap(a[i], a[j]);
                    i++;
                    j--;
                }
            }

            if (left < j) quicksort<ItemType, compOp, threshold>(a, left, j);
            if (right > i) quicksort<ItemType, compOp, threshold>(a, i, right);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp,
    ssize_t threshold = 100 >
    void RandomizedQuickSort(ItemType * const a, ssize_t const count) {
        privateRandomizedQuickSort::quicksort<ItemType, compOp, threshold>(
                a, 0, count - 1);
    }

    template<typename ItemType, ssize_t threshold = 100 >
    void RandomizedQuickSort(ItemType * const a, ssize_t const count) {
        RandomizedQuickSort<ItemType, genericComparisonOperator, threshold>(
                a, count);
    }
}

#endif	/* SORTQUICKRANDOMIZED_HPP */
