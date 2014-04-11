/* 
 * sortheapbinaryclusteredvarianta.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPBINARYCLUSTEREDVARIANTA_HPP
#define	SORTHEAPBINARYCLUSTEREDVARIANTA_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateClusteredBinaryHeapSortVariantA {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr arity = 2;

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void siftDown(ItemType * const a, ssize_t item, ssize_t const count,
                ssize_t clusterStart, ssize_t globalLevelStart,
                ssize_t globalLevelSize) {
            ssize_t constexpr clusterSize =
                    computeClusterSize<clusterLevels>(arity);
            ssize_t constexpr clusterArity =
                    computeClusterLevelSize<clusterLevels>(arity);
            ssize_t constexpr relativeLastLevelStart =
                    computeClusterSize < clusterLevels - 1 > (arity);

            ssize_t relativeItem = item - clusterStart;

            while (item < count) {
                ssize_t relativeEnd = count - clusterStart;

                ItemType * const cluster = a + clusterStart;
                while (relativeItem < relativeLastLevelStart) {
                    ssize_t const relativeLeft = relativeItem * arity + 1;
                    ssize_t const relativeRight = relativeItem * arity + 2;

                    if (relativeRight < relativeEnd) {
                        if (compOp(cluster[relativeItem], Below,
                                cluster[relativeLeft])) {
                            if (compOp(cluster[relativeLeft], Below,
                                    cluster[relativeRight])) {
                                std::swap(cluster[relativeItem],
                                        cluster[relativeRight]);
                                relativeItem = relativeRight;
                            } else {
                                std::swap(cluster[relativeItem],
                                        cluster[relativeLeft]);
                                relativeItem = relativeLeft;
                            }
                        } else {
                            if (compOp(cluster[relativeItem], Below,
                                    cluster[relativeRight])) {
                                std::swap(cluster[relativeItem],
                                        cluster[relativeRight]);
                                relativeItem = relativeRight;
                            } else {
                                return;
                            }
                        }
                    } else {
                        if (relativeLeft < relativeEnd && compOp(
                                cluster[relativeItem], Below,
                                cluster[relativeLeft])) {
                            std::swap(cluster[relativeItem],
                                    cluster[relativeLeft]);
                        }
                        return;
                    }
                }
                item = clusterStart + relativeItem;
                {
                    ssize_t const left = (clusterStart - globalLevelStart)
                            * clusterArity + (relativeItem
                            - relativeLastLevelStart) * arity * clusterSize
                            + globalLevelStart + globalLevelSize;
                    ssize_t const right = left + clusterSize;

                    ssize_t max = item;

                    if (right < count) {
                        if (compOp(a[max], Below, a[left])) {
                            max = left;
                        }
                        if (compOp(a[max], Below, a[right])) {
                            max = right;
                        }
                    } else if (left < count && compOp(a[max], Below, a[left])) {
                        max = left;
                    } else {
                        return;
                    }
                    if (max != item) {
                        std::swap(a[item], a[max]);
                        item = max;
                    } else {
                        return;
                    }

                    clusterStart = item;

                    globalLevelStart += globalLevelSize;
                    globalLevelSize *= clusterArity;
                }
                relativeItem = 0;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void heapify(ItemType * const a, ssize_t const count) {
            ssize_t const end = count;
            ssize_t constexpr clusterSize =
                    computeClusterSize<clusterLevels>(arity);
            ssize_t constexpr clusterArity =
                    computeClusterLevelSize<clusterLevels>(arity);

            ssize_t globalLevelStart = 0;
            ssize_t globalLevelSize = clusterSize;
            while (globalLevelStart + globalLevelSize < end) {
                globalLevelStart += globalLevelSize;
                globalLevelSize *= clusterArity;
            }
            ssize_t item = count - 1;
            ssize_t localClusterStart = item / clusterSize * clusterSize;
            ssize_t localLevelStart = localClusterStart;
            ssize_t localLevelSize = 1;
            ssize_t localLevel = 0;
            while (localLevelStart + localLevelSize <= item) {
                localLevelStart += localLevelSize;
                localLevelSize *= arity;
                localLevel++;
            }
            while (true) {
                while (localLevel >= 0) {
                    while (item >= localLevelStart) {
                        siftDown<ItemType, compOp, clusterLevels>(a, item, end,
                                localClusterStart, globalLevelStart,
                                globalLevelSize);
                        item--;
                    }
                    localLevel--;
                    localLevelSize /= arity;
                    localLevelStart -= localLevelSize;
                }
                if (item < 0) {
                    break;
                }
                if (item < globalLevelStart) {
                    globalLevelSize /= clusterArity;
                    globalLevelStart -= globalLevelSize;
                }
                localClusterStart -= clusterSize;
                localLevel = clusterLevels - 1;
                localLevelSize =
                        computeClusterLevelSize < clusterLevels - 1 > (arity);
                localLevelStart -= localLevelSize;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t last = count - 1;
            while (last > 0) {
                std::swap(a[last], a[0]);
                last--;
                siftDown<ItemType, compOp, clusterLevels>(a, 0, last + 1,
                        0, 0, computeClusterSize<clusterLevels>(arity));
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp, clusterLevels>(a, count);
            drainHeap<ItemType, compOp, clusterLevels>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp,
    ssize_t clusterLevels = 4 >
    void ClusteredBinaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        privateClusteredBinaryHeapSortVariantA::
                heapsort<ItemType, compOp, clusterLevels>(a, count);
    }

    template<typename ItemType>
    void ClusteredBinaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        ClusteredBinaryHeapSortVariantA<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPBINARYCLUSTEREDVARIANTA_HPP */
