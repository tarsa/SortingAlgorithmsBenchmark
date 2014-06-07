/* 
 * sortheaphybridcascading.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPHYBRIDCASCADING_HPP
#define	SORTHEAPHYBRIDCASCADING_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateCascadingHybridHeapSort {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr TopClusterStepArity = 3;
        ssize_t constexpr TopClusterLevels = 8;
        ssize_t constexpr TopClusterSize = computeClusterSize<TopClusterLevels>(
                TopClusterStepArity);
        ssize_t constexpr TopClusterTotalArity = computeClusterLevelSize<
                TopClusterLevels - 1 > (TopClusterStepArity);
        ssize_t constexpr TopClusterLastLevelStart =
                computeClusterSize<TopClusterLevels - 1 > (TopClusterStepArity);

        ssize_t constexpr SmallClusterLevels = 2;
        ssize_t constexpr SmallClusterArities[] = {3, 4};
        ssize_t constexpr SmallClusterSize = 4 * (3 + 1);
        ssize_t constexpr SmallClusterTotalArity = 4 * 3;
        ssize_t constexpr SmallClusterSecondLevelStart = 4;

        ssize_t constexpr QueueSize = 64 - TopClusterLevels;
        ssize_t constexpr QueueBufferFactor = 5;
        ssize_t constexpr QueueBufferSize = QueueSize * QueueBufferFactor;

        struct QueueBufferInfo {
            ssize_t start;
            ssize_t next;
        };

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftUp(ItemType * const a, ssize_t * const ancestorQueue,
                ssize_t const ancestorQueueLength) {
            assert(ancestorQueueLength > 0);
            ssize_t current = ancestorQueue[0];
            for (ssize_t index = 1; index < ancestorQueueLength; index++) {
                assert(current >= TopClusterSize);
                ssize_t const parent = ancestorQueue[index];
                if (compOp(a[parent], Below, a[current])) {
                    std::swap(a[parent], a[current]);
                    current = parent;
                } else {
                    return;
                }
            }
            assert(current < TopClusterSize);
            while (current >= 1) {
                ssize_t const parent = (current - 1) / TopClusterStepArity;
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
            ssize_t const ancestorQueueBufferSize = 64;
            ssize_t ancestorQueueBuffer[ancestorQueueBufferSize];
            ssize_t ancestorQueueLength = 1;
            ssize_t * ancestorQueue = ancestorQueueBuffer +
                    ancestorQueueBufferSize - ancestorQueueLength;
            ancestorQueue[0] = 1;
            while (ancestorQueue[0] < std::min(TopClusterSize, count)) {
                siftUp<ItemType, compOp>(a, ancestorQueue, ancestorQueueLength);
                ancestorQueue[0]++;
            }
            ancestorQueueLength++;
            ancestorQueue--;
            ancestorQueue[0] = TopClusterSize;
            ancestorQueue[1] = TopClusterLastLevelStart;
            while (ancestorQueue[0] < count) {
                assert((ancestorQueueLength - 2) % SmallClusterLevels == 0);
                ssize_t levelMax = TopClusterSize;
                for (ssize_t i = 0; i <= (ancestorQueueLength - 2)
                        / SmallClusterLevels; i++) {
                    levelMax += TopClusterTotalArity * SmallClusterSize
                            * integerPower(SmallClusterTotalArity, i);
                }
                ssize_t const levelEnd = std::min(levelMax, count);
                while (ancestorQueue[0] < levelEnd) {
                    for (ssize_t i = 0; i < SmallClusterSecondLevelStart; i++) {
                        siftUp<ItemType, compOp>(a, ancestorQueue,
                                ancestorQueueLength);
                        ancestorQueue[0]++;
                        if (ancestorQueue[0] == count) {
                            return;
                        }
                    }
                    ancestorQueueLength++;
                    ancestorQueue--;
                    ancestorQueue[0] = ancestorQueue[1];
                    ancestorQueue[1] -= SmallClusterSecondLevelStart;
                    for (ssize_t i = 0; i < SmallClusterArities[1]; i++) {
                        for (ssize_t j = 0; j < SmallClusterArities[0]; j++) {
                            siftUp<ItemType, compOp>(a, ancestorQueue,
                                    ancestorQueueLength);
                            ancestorQueue[0]++;
                            if (ancestorQueue[0] == count) {
                                return;
                            }
                        }
                        ancestorQueue[1]++;
                    }
                    ancestorQueue[1] = ancestorQueue[0];
                    ancestorQueueLength--;
                    ancestorQueue++;
                    ssize_t ancestorIndex = 1;
                    while (ancestorIndex + 1 < ancestorQueueLength) {
                        ancestorQueue[ancestorIndex]++;
                        if ((ancestorQueue[ancestorIndex] - TopClusterSize)
                                % SmallClusterSize == 0) {
                            ancestorQueue[ancestorIndex] +=
                                    SmallClusterSecondLevelStart;
                            ancestorQueue[ancestorIndex + 1] +=
                                    1 + SmallClusterSize
                                    - SmallClusterSecondLevelStart;
                            assert(ancestorQueue[ancestorIndex + 1]
                                    < ancestorQueue[ancestorIndex]);
                            assert(ancestorQueue[ancestorIndex + 1]
                                    > ancestorQueue[ancestorIndex]
                                    - SmallClusterSize);
                            ancestorIndex += 2;
                        } else {
                            ancestorQueue[ancestorIndex + 1] +=
                                    ((ancestorQueue[ancestorIndex]
                                    - TopClusterSize) % SmallClusterSize
                                    - SmallClusterSecondLevelStart)
                                    % SmallClusterArities[0] == 0;
                            break;
                        }
                    }
                    assert(ancestorIndex < ancestorQueueLength);
                    if (ancestorIndex == ancestorQueueLength - 1) {
                        ancestorQueue[ancestorIndex]++;
                    }
                }
                assert(ancestorQueue[ancestorQueueLength - 1]
                        == TopClusterSize);
                ancestorQueue -= 2;
                for (ssize_t i = 0; i < ancestorQueueLength; i++) {
                    ancestorQueue[i] = ancestorQueue[i + 2];
                }
                ancestorQueueLength += 2;
                ancestorQueue[ancestorQueueLength - 1] =
                        TopClusterLastLevelStart;
                ancestorQueue[ancestorQueueLength - 2] =
                        TopClusterSize;
                ancestorQueue[ancestorQueueLength - 3] =
                        TopClusterSize + SmallClusterSecondLevelStart;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        bool siftDownSingleStep(ItemType * const a, ssize_t const count,
                ssize_t * const parentSlot, ssize_t * const clusterSlot) {
            ssize_t parent = *parentSlot;
            ssize_t const clusterStart = *clusterSlot;
            ssize_t child1 = clusterStart;
            if (count - child1 >= 4) {
                ssize_t const child2 = child1 + 1;
                ssize_t const child3 = child2 + 1;
                ssize_t const child4 = child3 + 1;
                ssize_t const leader12 = child1
                        + compOp(a[child1], Below, a[child2]);
                ssize_t const leader34 = child3
                        + compOp(a[child3], Below, a[child4]);
                ssize_t const leader1234 =
                        compOp(a[leader12], Below, a[leader34])
                        ? leader34 : leader12;
                if (compOp(a[parent], Below, a[leader1234])) {
                    std::swap(a[parent], a[leader1234]);
                    parent = leader1234;
                } else {
                    return true;
                }
            } else {
                ssize_t leader = parent;
                for (ssize_t child = child1; child < count; child++) {
                    if (compOp(a[leader], Below, a[child])) {
                        leader = child;
                    }
                }
                std::swap(a[leader], a[parent]);
                return true;
            }
            child1 = clusterStart + (parent - clusterStart)
                    * SmallClusterArities[0] + SmallClusterSecondLevelStart;
            if (count - child1 >= 3) {
                ssize_t leader = parent;
                ssize_t const child2 = child1 + 1;
                ssize_t const child3 = child2 + 1;
                ssize_t const leader12 = child1
                        + compOp(a[child1], Below, a[child2]);
                if (compOp(a[leader], Below, a[child3])) {
                    leader = child3;
                }
                if (compOp(a[leader], Below, a[leader12])) {
                    leader = leader12;
                }
                if (leader != parent) {
                    std::swap(a[leader], a[parent]);
                    parent = leader;
                } else {
                    return true;
                }
            } else {
                ssize_t leader = parent;
                for (ssize_t child = child1; child < count; child++) {
                    if (compOp(a[leader], Below, a[child])) {
                        leader = child;
                    }
                }
                std::swap(a[leader], a[parent]);
                return true;
            }
            *parentSlot = parent;
            *clusterSlot = (parent - clusterStart - SmallClusterSecondLevelStart
                    + 1) * SmallClusterSize + (clusterStart - 1)
                    * SmallClusterTotalArity - TopClusterLastLevelStart + 1;
            prefetch(a + *clusterSlot);
            return false;
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        bool siftDownTip(ItemType * const a, ssize_t const count,
                ssize_t * const parentSlot, ssize_t * const clusterSlot) {
            ssize_t parent = 0;
            while (parent < TopClusterLastLevelStart) {
                ssize_t const child1 = parent * 3 + 1;
                if (count - child1 >= 3) {
                    ssize_t leader = parent;
                    ssize_t const child2 = child1 + 1;
                    ssize_t const child3 = child2 + 1;
                    ssize_t const leader12 = child1
                            + compOp(a[child1], Below, a[child2]);
                    if (compOp(a[leader], Below, a[child3])) {
                        leader = child3;
                    }
                    if (compOp(a[leader], Below, a[leader12])) {
                        leader = leader12;
                    }
                    if (parent != leader) {
                        std::swap(a[parent], a[leader]);
                        parent = leader;
                    } else {
                        return true;
                    }
                } else {
                    ssize_t leader = parent;
                    for (ssize_t child = child1; child < count; child++) {
                        if (compOp(a[leader], Below, a[child])) {
                            leader = child;
                        }
                    }
                    std::swap(a[parent], a[leader]);
                    return true;
                }
            }
            if (parent >= TopClusterLastLevelStart && count > TopClusterSize) {
                assert(parentSlot);
                assert(clusterSlot);
                *parentSlot = parent;
                *clusterSlot = (parent - TopClusterLastLevelStart)
                        * SmallClusterSize + TopClusterSize;
                return siftDownSingleStep<ItemType, compOp>(a, count,
                        parentSlot, clusterSlot);
            } else {
                return true;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownCascadedNoAdd(ItemType * const a, ssize_t const count,
                ssize_t * const parentsQueue, ssize_t * const clustersQueue,
                QueueBufferInfo * const queueBufferInfo) {
            for (ssize_t queueIndex = queueBufferInfo->start;
                    queueIndex < queueBufferInfo->next; queueIndex++) {
                bool const reachedBottom = siftDownSingleStep<ItemType, compOp>(
                        a, count, parentsQueue + queueIndex,
                        clustersQueue + queueIndex);
                if (reachedBottom) {
                    for (ssize_t toIndex = queueIndex; toIndex
                            > queueBufferInfo->start; toIndex--) {
                        parentsQueue[toIndex] = parentsQueue[toIndex - 1];
                        clustersQueue[toIndex] = clustersQueue[toIndex - 1];
                    }
                    queueBufferInfo->start++;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownCascaded(ItemType * const a, ssize_t const count,
                ssize_t * const parentsQueue, ssize_t * clustersQueue,
                QueueBufferInfo * const queueBufferInfo) {
            if (queueBufferInfo->next == QueueBufferSize) {
                ssize_t const distance = queueBufferInfo->start;
                for (ssize_t toIndex = 0; toIndex + distance
                        < QueueBufferSize; toIndex++) {
                    parentsQueue[toIndex] = parentsQueue[toIndex + distance];
                    clustersQueue[toIndex] = clustersQueue[toIndex + distance];
                }
                queueBufferInfo->start = 0;
                queueBufferInfo->next -= distance;
            }
            queueBufferInfo->next += !siftDownTip<ItemType, compOp>(a, count,
                    parentsQueue + queueBufferInfo->next,
                    clustersQueue + queueBufferInfo->next);
            siftDownCascadedNoAdd<ItemType, compOp>(a, count,
                    parentsQueue, clustersQueue, queueBufferInfo);
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t parentsQueue[QueueBufferSize];
            ssize_t clustersQueue[QueueBufferSize];
            QueueBufferInfo queueBufferInfo;
            queueBufferInfo.start = 0;
            queueBufferInfo.next = 0;
            ssize_t next = count - 1;
            for (; next > TopClusterSize; next--) {
                std::swap(a[next], a[0]);
                siftDownCascaded<ItemType, compOp>(a, next,
                        parentsQueue, clustersQueue, &queueBufferInfo);
            }
            while (queueBufferInfo.start < queueBufferInfo.next) {
                siftDownCascadedNoAdd<ItemType, compOp>(a, next,
                        parentsQueue, clustersQueue, &queueBufferInfo);
            }
            for (; next > 0; next--) {
                std::swap(a[next], a[0]);
                siftDownTip<ItemType, compOp>(a, next, nullptr, nullptr);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void HybridCascadingHeapSort(ItemType * const a, ssize_t const count) {
        privateCascadingHybridHeapSort::heapsort<ItemType, compOp>(
                a, count);
    }

    template<typename ItemType>
    void HybridCascadingHeapSort(ItemType * const a, ssize_t const count) {
        HybridCascadingHeapSort<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPHYBRIDCASCADING_HPP */
