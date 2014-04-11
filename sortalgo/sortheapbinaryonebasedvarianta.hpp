/* 
 * sortheapbinaryonebasedvarianta.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPBINARYONEBASEDVARIANTA_HPP
#define	SORTHEAPBINARYONEBASEDVARIANTA_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateOneBasedBinaryHeapSortVariantA {

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDown(ItemType * const a, ssize_t const start,
                ssize_t const end) {
            ssize_t root = start;
            while (true) {
                ssize_t const left = root * 2;
                ssize_t const right = left + 1;

                if (right <= end) {
                    if (compOp(a[root], Below, a[left])) {
                        if (compOp(a[left], Below, a[right])) {
                            std::swap(a[root], a[right]);
                            root = right;
                        } else {
                            std::swap(a[root], a[left]);
                            root = left;
                        }
                    } else {
                        if (compOp(a[root], Below, a[right])) {
                            std::swap(a[root], a[right]);
                            root = right;
                        } else {
                            return;
                        }
                    }
                } else {
                    if (left == end && compOp(a[root], Below, a[left])) {
                        std::swap(a[root], a[left]);
                    }
                    return;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            for (ssize_t start = count / 2; start >= 1; start--) {
                siftDown<ItemType, compOp>(a, start, count);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t end = count;
            while (end > 1) {
                std::swap(a[end], a[1]);
                end--;
                siftDown<ItemType, compOp>(a, 1, end);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void OneBasedBinaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        privateOneBasedBinaryHeapSortVariantA::heapsort<ItemType, compOp>(
                a - 1, count);
    }

    template<typename ItemType>
    void OneBasedBinaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        OneBasedBinaryHeapSortVariantA<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPBINARYONEBASEDVARIANTA_HPP */
