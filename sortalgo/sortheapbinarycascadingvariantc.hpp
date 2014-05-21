/* 
 * sortheapbinarycascadingvariantc.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPBINARYCASCADINGVARIANTC_HPP
#define	SORTHEAPBINARYCASCADINGVARIANTC_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateBinaryHeapSortCascadingVariantC {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr arity = 2;

        ssize_t constexpr TopLevels = 12;
        ssize_t constexpr TopSize = computeClusterSize<TopLevels>(arity);
        ssize_t constexpr TopLastLevelStart =
                computeClusterSize<TopLevels - 1 > (arity);
        ssize_t constexpr CountedLevelStart =
                computeClusterSize<TopLevels - 2 > (arity);
        ssize_t constexpr CountedPrefixesNumber =
                computeClusterLevelSize<TopLevels - 2 > (arity);

        ssize_t constexpr QueueSize = 64 - TopLevels;
        ssize_t constexpr QueueBufferFactor = 5;
        ssize_t constexpr QueueBufferSize = QueueSize * QueueBufferFactor;

        struct QueueBufferInfo {
            ssize_t start;
            ssize_t next;
        };

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownStraight(ItemType * const a, ssize_t const start,
                ssize_t const end, ItemType const item) {
            assert(start >= 1);
            assert(start <= end);
            ssize_t index = start;
            while (index * 2 + 1 <= end) {
                ssize_t const left = index * 2;
                ssize_t const right = left + 1;
                prefetch<1, 0>(a + std::min(left * 8, end));
                ssize_t const newIndex = index * 2
                        + compOp(a[left], Below, a[right]);
                a[index] = a[newIndex];
                index = newIndex;
            }
            if (index * 2 == end) {
                ssize_t const newIndex = index * 2;
                a[index] = a[newIndex];
                index = newIndex;
            }
            while (index / 2 >= start && compOp(a[index / 2], Below, item)) {
                a[index] = a[index / 2];
                index /= 2;
            }
            a[index] = item;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            for (ssize_t item = count / 2; item >= 1; item--) {
                siftDownStraight<ItemType, compOp>(a, item, count, a[item]);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        ssize_t siftDownTop(ItemType * const a,
                int8_t * const prefixesCounters) {
            ssize_t constexpr start = 1;
            ItemType const item = a[start];
            ssize_t index = start;
            for (ssize_t level = 1; (level + 1 < TopLevels) || ((level + 1 ==
                    TopLevels) && (prefixesCounters[index - CountedLevelStart
                    - 1] == 0)); level++) {
                ssize_t const left = index * 2;
                ssize_t const right = left + 1;
                ssize_t const newIndex = index * 2
                        + compOp(a[left], Below, a[right]);
                a[index] = a[newIndex];
                index = newIndex;
            }
            while (index / 2 >= start && compOp(a[index / 2], Below, item)) {
                a[index] = a[index / 2];
                index /= 2;
            }
            a[index] = item;
            return index;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        bool siftDownSingleStepImmediate(ItemType * const a,
                ssize_t const end, ssize_t * const indexSlot) {
            ssize_t const root = *indexSlot;
            ssize_t const left = root * 2;
            ssize_t const right = left + 1;

            if (right <= end) {
                ssize_t const maxChild = root * 2
                        + compOp(a[left], Below, a[right]);
                if (compOp(a[root], Below, a[maxChild])) {
                    std::swap(a[root], a[maxChild]);
                    *indexSlot = maxChild;
                    prefetch(a + std::min(maxChild * 2, end));
                    return false;
                }
            } else if (left == end && compOp(a[root], Below, a[left])) {
                std::swap(a[root], a[left]);
                *indexSlot = left;
            }
            return true;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownBacktrack(ItemType * const a, ssize_t const start,
                ItemType const item) {
            ssize_t index = start;
            while (index / 2 - 1 >= CountedLevelStart // TopLastLevelStart
                    && compOp(a[index / 2], Below, item)) {
                a[index] = a[index / 2];
                index /= 2;
            }
            a[index] = item;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        bool siftDownSingleStepBacktracking(ItemType * const a,
                ssize_t const end, ssize_t * const indexSlot) {
            ssize_t const root = *indexSlot;
            ssize_t const left = root * 2;
            ssize_t const right = left + 1;

            if (right <= end) {
                ssize_t const maxChild = root * 2
                        + compOp(a[left], Below, a[right]);
                a[root] = a[maxChild];
                *indexSlot = maxChild;
                prefetch(a + std::min(maxChild * 2, end));
                return false;
            } else if (left == end) {
                a[root] = a[left];
                *indexSlot = left;
                return false;
            } else {
                return true;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownCascadedNoAdd(ItemType * const a, ssize_t const end,
                ItemType * const itemCopiesQueue, ssize_t * const indexesQueue,
                ssize_t * const prefixesQueue,
                QueueBufferInfo * const queueBufferInfo,
                int8_t * const prefixesCounters) {
            for (ssize_t queueIndex = queueBufferInfo->start;
                    queueIndex < queueBufferInfo->next; queueIndex++) {
                ssize_t const prefix = prefixesQueue[queueIndex];
                bool reachedBottom;
                if (prefixesCounters[prefix - CountedLevelStart - 1] > 1) {
                    reachedBottom = siftDownSingleStepImmediate<ItemType,
                            compOp>(a, end, indexesQueue + queueIndex);
                } else {
                    assert(prefixesCounters[prefix - CountedLevelStart - 1]
                            == 1);
                    reachedBottom = siftDownSingleStepBacktracking<ItemType,
                            compOp>(a, end, indexesQueue + queueIndex);
                    if (reachedBottom) {
                        assert(indexesQueue[queueIndex] <= end);
                        siftDownBacktrack<ItemType, compOp>(a,
                                indexesQueue[queueIndex],
                                itemCopiesQueue[queueIndex]);
                    }
                }
                prefixesCounters[prefix - CountedLevelStart - 1] -=
                        reachedBottom;
                if (reachedBottom) {
                    for (ssize_t toIndex = queueIndex; toIndex - 1
                            >= queueBufferInfo->start; toIndex--) {
                        itemCopiesQueue[toIndex] = itemCopiesQueue[toIndex - 1];
                        indexesQueue[toIndex] = indexesQueue[toIndex - 1];
                        prefixesQueue[toIndex] = prefixesQueue[toIndex - 1];
                    }
                    queueBufferInfo->start++;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownCascaded(ItemType * const a, ssize_t const end,
                ItemType * const itemCopiesQueue, ssize_t * const indexesQueue,
                ssize_t * const prefixesQueue,
                QueueBufferInfo * const queueBufferInfo,
                int8_t * const prefixesCounters) {
            ssize_t const itemIndex = siftDownTop<ItemType, compOp>(a,
                    prefixesCounters);
            if (itemIndex - 1 >= CountedLevelStart) {
                assert(itemIndex - 1 < TopSize);
                if (queueBufferInfo->next == QueueBufferSize) {
                    ssize_t const distance = queueBufferInfo->start;
                    for (ssize_t toIndex = 0; toIndex + distance
                            < QueueBufferSize; toIndex++) {
                        itemCopiesQueue[toIndex] =
                                itemCopiesQueue[toIndex + distance];
                        indexesQueue[toIndex] =
                                indexesQueue[toIndex + distance];
                        prefixesQueue[toIndex] =
                                prefixesQueue[toIndex + distance];
                    }
                    queueBufferInfo->start = 0;
                    queueBufferInfo->next -= distance;
                }
                ssize_t const newItemIndex = queueBufferInfo->next;
                itemCopiesQueue[newItemIndex] = a[itemIndex];
                indexesQueue[newItemIndex] = itemIndex;
                if (itemIndex - 1 >= TopLastLevelStart) {
                    ssize_t const itemPrefix = itemIndex >> 1;
                    prefixesQueue[newItemIndex] = itemPrefix;
                    prefixesCounters[itemPrefix - CountedLevelStart - 1]++;
                } else {
                    ssize_t const itemPrefix = itemIndex;
                    prefixesQueue[newItemIndex] = itemPrefix;
                    prefixesCounters[itemPrefix - CountedLevelStart - 1]++;
                    if (prefixesCounters[itemPrefix - CountedLevelStart - 1]
                            == 2) {
                        ssize_t collidingItemIndex = -1;
                        for (ssize_t i = queueBufferInfo->start;
                                i < queueBufferInfo->next; i++) {
                            if (prefixesQueue[i] == itemPrefix) {
                                assert(collidingItemIndex == -1);
                                collidingItemIndex = i;
                            }
                        }
                        assert(collidingItemIndex != -1);
                        siftDownBacktrack<ItemType, compOp>(a,
                                indexesQueue[collidingItemIndex],
                                itemCopiesQueue[collidingItemIndex]);
                        siftDownSingleStepImmediate<ItemType, compOp>(a, end,
                                indexesQueue + newItemIndex);
                    }
                }
                assert(prefixesQueue[newItemIndex] - CountedLevelStart >= 1);
                assert(prefixesQueue[newItemIndex] - CountedLevelStart - 1
                        < CountedPrefixesNumber);
                queueBufferInfo->next++;
                assert(queueBufferInfo->next <= QueueBufferSize);
            }
            siftDownCascadedNoAdd<ItemType, compOp>(a, end,
                    itemCopiesQueue, indexesQueue, prefixesQueue,
                    queueBufferInfo, prefixesCounters);
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t next = count;
            if (next > TopSize) {
                ItemType itemCopiesQueue[QueueBufferSize];
                ssize_t indexesQueue[QueueBufferSize];
                ssize_t prefixesQueue[QueueBufferSize];
                QueueBufferInfo queueBufferInfo;
                queueBufferInfo.start = 0;
                queueBufferInfo.next = 0;
                int8_t prefixesCounters[CountedPrefixesNumber];
                for (ssize_t i = 0; i < CountedPrefixesNumber; i++) {
                    prefixesCounters[i] = 0;
                }
                for (; next > TopSize; next--) {
                    if (queueBufferInfo.next > queueBufferInfo.start
                            && indexesQueue[queueBufferInfo.start] == next
                            && prefixesCounters[prefixesQueue[queueBufferInfo
                            .start] - CountedLevelStart - 1] == 1) {
                        siftDownBacktrack<ItemType, compOp>(a, next,
                                itemCopiesQueue[queueBufferInfo.start]);
                        prefixesCounters[prefixesQueue[queueBufferInfo.start]
                                - CountedLevelStart - 1]--;
                        queueBufferInfo.start++;
                    }
                    std::swap(a[next], a[1]);
                    siftDownCascaded<ItemType, compOp>(a, next - 1,
                            itemCopiesQueue, indexesQueue, prefixesQueue,
                            &queueBufferInfo, prefixesCounters);
                }
                while (queueBufferInfo.start < queueBufferInfo.next) {
                    siftDownCascadedNoAdd<ItemType, compOp>(a, next - 1,
                            itemCopiesQueue, indexesQueue, prefixesQueue,
                            &queueBufferInfo, prefixesCounters);
                }
            }
            for (; next > 1; next--) {
                ItemType item = a[next];
                a[next] = a[1];
                siftDownStraight<ItemType, compOp>(a, 1, next - 1, item);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void BinaryHeapSortCascadingVariantC(ItemType * const a,
            ssize_t const count) {
        privateBinaryHeapSortCascadingVariantC::heapsort<ItemType, compOp>(
                a - 1, count);
    }

    template<typename ItemType>
    void BinaryHeapSortCascadingVariantC(ItemType * const a,
            ssize_t const count) {
        BinaryHeapSortCascadingVariantC<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPBINARYCASCADINGVARIANTC_HPP */
