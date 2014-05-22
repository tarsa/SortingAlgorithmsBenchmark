/* 
 * sortheapternarycascadingvarianta.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPTERNARYCASCADINGVARIANTA_HPP
#define	SORTHEAPTERNARYCASCADINGVARIANTA_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateTernaryHeapSortCascadingVariantA {

        ssize_t constexpr QueueSize = 64;

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDown(ItemType * const a, ssize_t const start,
                ssize_t const end) {
            ssize_t root = start;
            while (true) {
                ssize_t const first = root * 3 - 1;
                ssize_t const middle = first + 1;
                ssize_t const last = middle + 1;

                if (last <= end) {
                    if (compOp(a[root], Below, a[first])) {
                        if (compOp(a[first], Below, a[middle])) {
                            if (compOp(a[middle], Below, a[last])) {
                                std::swap(a[root], a[last]);
                                root = last;
                            } else {
                                std::swap(a[root], a[middle]);
                                root = middle;
                            }
                        } else {
                            if (compOp(a[first], Below, a[last])) {
                                std::swap(a[root], a[last]);
                                root = last;
                            } else {
                                std::swap(a[root], a[first]);
                                root = first;
                            }
                        }
                    } else {
                        if (compOp(a[root], Below, a[middle])) {
                            if (compOp(a[middle], Below, a[last])) {
                                std::swap(a[root], a[last]);
                                root = last;
                            } else {
                                std::swap(a[root], a[middle]);
                                root = middle;
                            }
                        } else {
                            if (compOp(a[root], Below, a[last])) {
                                std::swap(a[root], a[last]);
                                root = last;
                            } else {
                                return;
                            }
                        }
                    }
                } else {
                    ssize_t biggest = root;
                    if (first <= end && compOp(a[biggest], Below, a[first])) {
                        biggest = first;
                    }
                    if (middle <= end && compOp(a[biggest], Below, a[middle])) {
                        biggest = middle;
                    }
                    std::swap(a[root], a[biggest]);
                    return;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            for (ssize_t item = count / 3 + 1; item >= 1; item--) {
                siftDown<ItemType, compOp>(a, item, count);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        bool siftDownSingleStep(ItemType * const a, ssize_t const end,
                ssize_t * const queueSlot) {
            ssize_t const root = *queueSlot;
            ssize_t const left = root * 3 - 1;
            ssize_t const middle = left + 1;
            ssize_t const right = middle + 1;

            if (right <= end) {
                ssize_t const maxChild12 = left
                        + compOp(a[left], Below, a[middle]);
                ssize_t const maxChild123 = compOp(a[maxChild12], Below,
                        a[right]) ? right : maxChild12;
                if (compOp(a[root], Below, a[maxChild123])) {
                    std::swap(a[root], a[maxChild123]);
                    *queueSlot = maxChild123;
                    prefetch(a + std::min(maxChild123 * 3, end));
                    return true;
                }
            } else {
                ssize_t biggest = root;
                if (left <= end && compOp(a[biggest], Below, a[left])) {
                    biggest = left;
                }
                if (middle <= end && compOp(a[biggest], Below, a[middle])) {
                    biggest = middle;
                }
                std::swap(a[root], a[biggest]);
            }
            return false;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        ssize_t siftDownCascaded(ItemType * const a, ssize_t const start,
                ssize_t * const queue, ssize_t queueLength, ssize_t const end) {
            queue[queueLength] = start;
            queueLength++;
            ssize_t queueStoreIndex = 0;
            for (ssize_t queueIndex = queue[0] * 3 - 1 > end;
                    queueIndex < queueLength; queueIndex++) {
                queue[queueStoreIndex] = queue[queueIndex];
                queueStoreIndex += siftDownSingleStep<ItemType, compOp>(a, end,
                        queue + queueStoreIndex);
            }
            return queueStoreIndex;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t queue[QueueSize];
            ssize_t queueLength = 0;
            for (ssize_t next = count; next > 1; next--) {
                std::swap(a[next], a[1]);
                queueLength = siftDownCascaded<ItemType, compOp>(a, 1, queue,
                        queueLength, next - 1);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void TernaryHeapSortCascadingVariantA(ItemType * const a,
            ssize_t const count) {
        privateTernaryHeapSortCascadingVariantA::heapsort<ItemType, compOp>(
                a - 1, count);
    }

    template<typename ItemType>
    void TernaryHeapSortCascadingVariantA(ItemType * const a,
            ssize_t const count) {
        TernaryHeapSortCascadingVariantA<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPTERNARYCASCADINGVARIANTA_HPP */
