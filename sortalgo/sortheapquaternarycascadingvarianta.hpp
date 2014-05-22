/* 
 * sortheapquaternarycascadingvarianta.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPQUATERNARYCASCADINGVARIANTA_HPP
#define	SORTHEAPQUATERNARYCASCADINGVARIANTA_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateQuaternaryHeapSortCascadingVariantA {

        ssize_t constexpr Arity = 4;

        ssize_t constexpr QueueSize = 64;

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftUp(ItemType * const a, ssize_t const start) {
            ssize_t current = start;
            while (current >= Arity) {
                ssize_t const parent = current / Arity - 1;
                if (compOp(a[parent], Below, a[current])) {
                    std::swap(a[parent], a[current]);
                    current = parent;
                } else {
                    return;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            for (ssize_t item = Arity; item < count; item++) {
                siftUp<ItemType, compOp>(a, item);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        bool siftDownSingleStep(ItemType * const a, ssize_t const count,
                ssize_t * const queueSlot, ssize_t const child1) {
            ssize_t const root = *queueSlot;
            ssize_t const child2 = child1 + 1;
            ssize_t const child3 = child2 + 1;
            ssize_t const child4 = child3 + 1;

            if (child4 < count) {
                ssize_t const maxChild12 = child1
                        + compOp(a[child1], Below, a[child2]);
                ssize_t const maxChild34 = child3
                        + compOp(a[child3], Below, a[child4]);
                ssize_t const maxChild1234 = compOp(a[maxChild12], Below,
                        a[maxChild34]) ? maxChild34 : maxChild12;
                if (compOp(a[root], Below, a[maxChild1234])) {
                    std::swap(a[root], a[maxChild1234]);
                    *queueSlot = maxChild1234;
                    prefetch(a + std::min((maxChild1234 + 1) * Arity,
                            count - 1));
                    return true;
                }
            } else {
                ssize_t biggest = root;
                if (child1 < count && compOp(a[biggest], Below, a[child1])) {
                    biggest = child1;
                }
                if (child2 < count && compOp(a[biggest], Below, a[child2])) {
                    biggest = child2;
                }
                if (child3 < count && compOp(a[biggest], Below, a[child3])) {
                    biggest = child3;
                }
                std::swap(a[root], a[biggest]);
            }
            return false;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        ssize_t siftDownCascaded(ItemType * const a, ssize_t * const queue,
                ssize_t queueLength, ssize_t const count) {
            ssize_t queueStoreIndex = 0;
            for (ssize_t queueIndex = (queue[0] + 1) * Arity >= count;
                    queueIndex < queueLength; queueIndex++) {
                ssize_t const item = queue[queueIndex];
                queue[queueStoreIndex] = item;
                queueStoreIndex += siftDownSingleStep<ItemType, compOp>(a,
                        count, queue + queueStoreIndex, (item + 1) * Arity);
            }
            queue[queueStoreIndex] = count - 1;
            queueStoreIndex += siftDownSingleStep<ItemType, compOp>(a, count,
                    queue + queueStoreIndex, 0);
            return queueStoreIndex;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t queue[QueueSize];
            ssize_t queueLength = 0;
            for (ssize_t next = count; next > 0; next--) {
                queueLength = siftDownCascaded<ItemType, compOp>(a, queue,
                        queueLength, next);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void QuaternaryHeapSortCascadingVariantA(ItemType * const a,
            ssize_t const count) {
        privateQuaternaryHeapSortCascadingVariantA::heapsort<ItemType,
                compOp>(a, count);
    }

    template<typename ItemType>
    void QuaternaryHeapSortCascadingVariantA(ItemType * const a,
            ssize_t const count) {
        QuaternaryHeapSortCascadingVariantA<ItemType,
                genericComparisonOperator>(a, count);
    }
}

#endif	/* SORTHEAPQUATERNARYCASCADINGVARIANTA_HPP */
