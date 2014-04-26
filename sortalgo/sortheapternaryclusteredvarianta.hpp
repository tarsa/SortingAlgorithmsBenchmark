/* 
 * sortheapternaryclusteredvarianta.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPTERNARYCLUSTEREDVARIANTA_HPP
#define	SORTHEAPTERNARYCLUSTEREDVARIANTA_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateClusteredTernaryHeapSortVariantA {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr arity = 3;

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void siftDown(ItemType * const a, ssize_t item, ssize_t const count,
                ssize_t clusterStart) {
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
                    ssize_t const relativeMiddle = relativeItem * arity + 2;
                    ssize_t const relativeRight = relativeItem * arity + 3;

                    if (relativeRight < relativeEnd) {
                        if (compOp(cluster[relativeItem], Below,
                                cluster[relativeLeft])) {
                            if (compOp(cluster[relativeLeft], Below,
                                    cluster[relativeMiddle])) {
                                if (compOp(cluster[relativeMiddle], Below,
                                        cluster[relativeRight])) {
                                    std::swap(cluster[relativeItem],
                                            cluster[relativeRight]);
                                    relativeItem = relativeRight;
                                } else {
                                    std::swap(cluster[relativeItem],
                                            cluster[relativeMiddle]);
                                    relativeItem = relativeMiddle;
                                }
                            } else {
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
                            }
                        } else {
                            if (compOp(cluster[relativeItem], Below,
                                    cluster[relativeMiddle])) {
                                if (compOp(cluster[relativeMiddle], Below,
                                        cluster[relativeRight])) {
                                    std::swap(cluster[relativeItem],
                                            cluster[relativeRight]);
                                    relativeItem = relativeRight;
                                } else {
                                    std::swap(cluster[relativeItem],
                                            cluster[relativeMiddle]);
                                    relativeItem = relativeMiddle;
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
                        }
                    } else {
                        ssize_t biggest = relativeItem;
                        if (relativeLeft < relativeEnd &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeLeft])) {
                            biggest = relativeLeft;
                        }
                        if (relativeMiddle < relativeEnd &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeMiddle])) {
                            biggest = relativeMiddle;
                        }
                        std::swap(cluster[relativeItem], cluster[biggest]);
                        return;
                    }
                }
                item = clusterStart + relativeItem;
                {
                    ssize_t const left = clusterStart * clusterArity + (1 +
                            (relativeItem - relativeLastLevelStart) * arity)
                            * clusterSize;
                    ssize_t const middle = left + clusterSize;
                    ssize_t const right = middle + clusterSize;

                    if (right < count) {
                        if (compOp(a[item], Below, a[left])) {
                            if (compOp(a[left], Below, a[middle])) {
                                if (compOp(a[middle], Below, a[right])) {
                                    std::swap(a[item], a[right]);
                                    item = right;
                                } else {
                                    std::swap(a[item], a[middle]);
                                    item = middle;
                                }
                            } else {
                                if (compOp(a[left], Below, a[right])) {
                                    std::swap(a[item], a[right]);
                                    item = right;
                                } else {
                                    std::swap(a[item], a[left]);
                                    item = left;
                                }
                            }
                        } else {
                            if (compOp(a[item], Below, a[middle])) {
                                if (compOp(a[middle], Below, a[right])) {
                                    std::swap(a[item], a[right]);
                                    item = right;
                                } else {
                                    std::swap(a[item], a[middle]);
                                    item = middle;
                                }
                            } else {
                                if (compOp(a[item], Below, a[right])) {
                                    std::swap(a[item], a[right]);
                                    item = right;
                                } else {
                                    return;
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = item;
                        if (left < count &&
                                compOp(a[biggest], Below, a[left])) {
                            biggest = left;
                        }
                        if (middle < count &&
                                compOp(a[biggest], Below, a[middle])) {
                            biggest = middle;
                        }
                        if (biggest != item) {
                            std::swap(a[item], a[biggest]);
                            item = biggest;
                        } else {
                            return;
                        }
                    }

                    clusterStart = item;
                }
                relativeItem = 0;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void heapify(ItemType * const a, ssize_t const count) {
            ssize_t constexpr clusterSize =
                    computeClusterSize<clusterLevels>(arity);

            for (ssize_t item = count - 1,
                    localClusterStart = item / clusterSize * clusterSize;
                    item >= 0; localClusterStart -= clusterSize) {
                while (item >= localClusterStart) {
                    siftDown<ItemType, compOp, clusterLevels>(a, item, count,
                            localClusterStart);
                    item--;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void drainHeap(ItemType * const a, ssize_t const count) {
            for (ssize_t next = count - 1; next > 0; next--) {
                std::swap(a[next], a[0]);
                siftDown<ItemType, compOp, clusterLevels>(a, 0, next, 0);
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
    ssize_t clusterLevels = 3 >
    void ClusteredTernaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        privateClusteredTernaryHeapSortVariantA::
                heapsort<ItemType, compOp, clusterLevels>(a, count);
    }

    template<typename ItemType>
    void ClusteredTernaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        ClusteredTernaryHeapSortVariantA<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPTERNARYCLUSTEREDVARIANTA_HPP */
