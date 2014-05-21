/* 
 * sortheapbinarycascadingvariantd.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPBINARYCASCADINGVARIANTD_HPP
#define	SORTHEAPBINARYCASCADINGVARIANTD_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateBinaryHeapSortCascadingVariantD {

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

        struct QueuesLinksInfo {
            ssize_t * const listLinks;
            ssize_t cascadeQueueHead;
            ssize_t cascadeQueueLast;
            ssize_t freeQueueHead;

            QueuesLinksInfo(ssize_t * const listLinks,
                    ssize_t const cascadeQueueHead,
                    ssize_t const cascadeQueueLast,
                    ssize_t const freeQueueHead) :
            listLinks(listLinks),
            cascadeQueueHead(cascadeQueueHead),
            cascadeQueueLast(cascadeQueueLast),
            freeQueueHead(freeQueueHead) {
            }

            void advanceHead() {
                ssize_t const oldHead = cascadeQueueHead;
                assert(oldHead != -1);
                if (oldHead == cascadeQueueLast) {
                    cascadeQueueHead = -1;
                    cascadeQueueLast = -1;
                } else {
                    cascadeQueueHead = listLinks[oldHead];
                }
                listLinks[oldHead] = freeQueueHead;
                freeQueueHead = oldHead;
                sanityCheck();
            }

            void advanceLast() {
                ssize_t const newLast = freeQueueHead;
                assert(newLast != -1);
                freeQueueHead = listLinks[freeQueueHead];
                listLinks[newLast] = -1;
                if (cascadeQueueLast != -1) {
                    listLinks[cascadeQueueLast] = newLast;
                }
                cascadeQueueLast = newLast;
                if (cascadeQueueHead == -1) {
                    cascadeQueueHead = newLast;
                }
                sanityCheck();
            }

            ssize_t removeAndFollow(ssize_t const previous,
                    ssize_t const current) {
                ssize_t const next = listLinks[current];
                listLinks[current] = freeQueueHead;
                freeQueueHead = current;
                if (previous == -1) {
                    cascadeQueueHead = next;
                } else {
                    listLinks[previous] = next;
                }
                if (current == cascadeQueueLast) {
                    cascadeQueueLast = previous;
                }
                sanityCheck();
                return next;
            }

            void sanityCheck() {
#ifndef NDEBUG
                ssize_t counters[QueueSize];
                for (ssize_t i = 0; i < QueueSize; i++) {
                    counters[i] = 0;
                }
                for (ssize_t freeNodeIndex = freeQueueHead; freeNodeIndex != -1;
                        freeNodeIndex = listLinks[freeNodeIndex]) {
                    assert(freeNodeIndex >= 0);
                    assert(freeNodeIndex < QueueSize);
                    counters[freeNodeIndex]++;
                }
                for (ssize_t nodeIndex = cascadeQueueHead; nodeIndex != -1;
                        nodeIndex = listLinks[nodeIndex]) {
                    assert(nodeIndex >= 0);
                    assert(nodeIndex < QueueSize);
                    counters[nodeIndex]++;
                }
                for (ssize_t index = 0; index < QueueSize; index++) {
                    assert(counters[index] == 1);
                }
                assert(cascadeQueueLast == -1
                        || listLinks[cascadeQueueLast] == -1);
#endif
            }
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
                ssize_t * const prefixesQueue, int8_t * const prefixesCounters,
                QueuesLinksInfo * const queuesEnds) {
            ssize_t previous = -1;
            ssize_t current = queuesEnds->cascadeQueueHead;
            while (current != -1) {
                ssize_t const prefix = prefixesQueue[current];
                bool reachedBottom;
                if (prefixesCounters[prefix - CountedLevelStart - 1] > 1) {
                    reachedBottom = siftDownSingleStepImmediate<
                            ItemType, compOp>(a, end, indexesQueue + current);
                } else {
                    assert(prefixesCounters[prefix - CountedLevelStart - 1]
                            == 1);
                    reachedBottom = siftDownSingleStepBacktracking<
                            ItemType, compOp>(a, end, indexesQueue + current);
                    if (reachedBottom) {
                        assert(indexesQueue[current] <= end);
                        siftDownBacktrack<ItemType, compOp>(a, indexesQueue[
                                current], itemCopiesQueue[current]);
                    }
                }
                if (reachedBottom) {
                    prefixesCounters[prefix - CountedLevelStart - 1] -= 1;
                    current = queuesEnds->removeAndFollow(previous, current);
                } else {
                    previous = current;
                    current = queuesEnds->listLinks[current];
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownCascaded(ItemType * const a, ssize_t const end,
                ItemType * const itemCopiesQueue, ssize_t * const indexesQueue,
                ssize_t * const prefixesQueue, int8_t * const prefixesCounters,
                QueuesLinksInfo * const queuesEnds) {
            ssize_t const itemIndex = siftDownTop<ItemType, compOp>(a,
                    prefixesCounters);
            if (itemIndex - 1 >= CountedLevelStart) {
                assert(itemIndex - 1 < TopSize);
                queuesEnds->advanceLast();
                ssize_t newItemQueueIndex = queuesEnds->cascadeQueueLast;
                itemCopiesQueue[newItemQueueIndex] = a[itemIndex];
                indexesQueue[newItemQueueIndex] = itemIndex;
                if (itemIndex - 1 >= TopLastLevelStart) {
                    ssize_t const itemPrefix = itemIndex >> 1;
                    prefixesQueue[newItemQueueIndex] = itemPrefix;
                    prefixesCounters[itemPrefix - CountedLevelStart - 1]++;
                } else {
                    ssize_t const itemPrefix = itemIndex;
                    prefixesQueue[newItemQueueIndex] = itemPrefix;
                    prefixesCounters[itemPrefix - CountedLevelStart - 1]++;
                    if (prefixesCounters[itemPrefix - CountedLevelStart - 1]
                            == 2) {
                        ssize_t collidingItemIndex = -1;
                        for (ssize_t nodeQueueIndex = queuesEnds->
                                cascadeQueueHead; nodeQueueIndex !=
                                newItemQueueIndex; nodeQueueIndex =
                                queuesEnds->listLinks[nodeQueueIndex]) {
                            if (prefixesQueue[nodeQueueIndex] == itemPrefix) {
                                assert(collidingItemIndex == -1);
                                collidingItemIndex = nodeQueueIndex;
                            }
                        }
                        assert(collidingItemIndex != -1);
                        siftDownBacktrack<ItemType, compOp>(a,
                                indexesQueue[collidingItemIndex],
                                itemCopiesQueue[collidingItemIndex]);
                        siftDownSingleStepImmediate<ItemType, compOp>(a, end,
                                indexesQueue + newItemQueueIndex);
                    }
                }
                assert(prefixesQueue[newItemQueueIndex] - CountedLevelStart
                        >= 1);
                assert(prefixesQueue[newItemQueueIndex] - CountedLevelStart
                        <= CountedPrefixesNumber);
            }
            siftDownCascadedNoAdd<ItemType, compOp>(a, end,
                    itemCopiesQueue, indexesQueue, prefixesQueue,
                    prefixesCounters, queuesEnds);
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t next = count;
            if (next > TopSize) {
                ssize_t listLinks[QueueSize];
                for (ssize_t i = 1; i < QueueSize; i++) {
                    listLinks[i] = i - 1;
                }
                listLinks[0] = -1;
                QueuesLinksInfo queuesEnds(listLinks, -1, -1, QueueSize - 1);
                ItemType itemCopiesQueue[QueueSize];
                ssize_t indexesQueue[QueueSize];
                ssize_t prefixesQueue[QueueSize];
                int8_t prefixesCounters[CountedPrefixesNumber];
                for (ssize_t i = 0; i < CountedPrefixesNumber; i++) {
                    prefixesCounters[i] = 0;
                }
                for (; next > TopSize; next--) {
                    ssize_t const head = queuesEnds.cascadeQueueHead;
                    if (head != -1 && indexesQueue[head] == next
                            && prefixesCounters[prefixesQueue[head]
                            - CountedLevelStart - 1] == 1) {
                        siftDownBacktrack<ItemType, compOp>(a, next,
                                itemCopiesQueue[head]);
                        prefixesCounters[prefixesQueue[head]
                                - CountedLevelStart - 1]--;
                        queuesEnds.advanceHead();
                    }
                    std::swap(a[next], a[1]);
                    siftDownCascaded<ItemType, compOp>(a, next - 1,
                            itemCopiesQueue, indexesQueue, prefixesQueue,
                            prefixesCounters, &queuesEnds);
                }
                while (queuesEnds.cascadeQueueHead != -1) {
                    siftDownCascadedNoAdd<ItemType, compOp>(a, next - 1,
                            itemCopiesQueue, indexesQueue, prefixesQueue,
                            prefixesCounters, &queuesEnds);
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
    void BinaryHeapSortCascadingVariantD(ItemType * const a,
            ssize_t const count) {
        privateBinaryHeapSortCascadingVariantD::heapsort<ItemType, compOp>(
                a - 1, count);
    }

    template<typename ItemType>
    void BinaryHeapSortCascadingVariantD(ItemType * const a,
            ssize_t const count) {
        BinaryHeapSortCascadingVariantD<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPBINARYCASCADINGVARIANTD_HPP */
