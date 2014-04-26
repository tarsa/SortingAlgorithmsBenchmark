/* 
 * sortheapbinaryclusteredvariantb.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPBINARYCLUSTEREDVARIANTB_HPP
#define	SORTHEAPBINARYCLUSTEREDVARIANTB_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateClusteredBinaryHeapSortVariantB {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr arity = 2;

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void siftDownInit(ItemType * const a, ssize_t root,
                ssize_t relativeLeft, ssize_t const count,
                ssize_t clusterStart) {
            ssize_t constexpr clusterSize =
                    computeClusterSize < clusterLevels + 1 > (arity) - 1;
            ssize_t constexpr clusterArity =
                    computeClusterLevelSize<clusterLevels>(arity);
            ssize_t constexpr relativeLastLevelStart =
                    computeClusterSize<clusterLevels>(arity) - 1;

            ssize_t relativeRoot = root - clusterStart;
            while (true) {
                ssize_t relativeClusterEnd = std::min(count - clusterStart,
                        clusterSize);

                ItemType * const cluster = a + clusterStart;
                while (relativeLeft < clusterSize) {
                    ssize_t const relativeRight = relativeLeft + 1;

                    if (relativeRight < relativeClusterEnd) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeLeft])) {
                            if (compOp(cluster[relativeLeft], Below,
                                    cluster[relativeRight])) {
                                std::swap(cluster[relativeRoot],
                                        cluster[relativeRight]);
                                relativeRoot = relativeRight;
                            } else {
                                std::swap(cluster[relativeRoot],
                                        cluster[relativeLeft]);
                                relativeRoot = relativeLeft;
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeRight])) {
                                std::swap(cluster[relativeRoot],
                                        cluster[relativeRight]);
                                relativeRoot = relativeRight;
                            } else {
                                return;
                            }
                        }
                    } else {
                        if (relativeLeft < relativeClusterEnd &&
                                compOp(cluster[relativeRoot], Below,
                                cluster[relativeLeft])) {
                            std::swap(cluster[relativeRoot],
                                    cluster[relativeLeft]);
                        }
                        return;
                    }
                    relativeLeft = (relativeRoot + 1) * arity;
                }
                root = clusterStart + relativeRoot;
                {
                    ssize_t const left = clusterStart * clusterArity +
                            (relativeRoot - relativeLastLevelStart + 1)
                            * clusterSize;
                    ssize_t const right = left + 1;

                    if (right < count) {
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
                        if (left < count && compOp(a[root], Below, a[left])) {
                            std::swap(a[root], a[left]);
                        }
                        return;
                    }

                    clusterStart = left;
                    relativeRoot = root - clusterStart;
                    relativeLeft = (relativeRoot + 1) * arity;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void siftDownLast(ItemType * const a, ssize_t root,
                ssize_t left, ssize_t const count, ssize_t clusterStart) {
            ssize_t constexpr clusterSize =
                    computeClusterSize < clusterLevels + 1 > (arity) - 1;
            ssize_t constexpr clusterArity =
                    computeClusterLevelSize<clusterLevels>(arity);
            ssize_t constexpr relativeLastLevelStart =
                    computeClusterSize<clusterLevels>(arity) - 1;

            ssize_t relativeRoot;
            while (true) {
                {
                    ssize_t const right = left + 1;

                    if (right < count) {
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
                        if (left < count && compOp(a[root], Below, a[left])) {
                            std::swap(a[root], a[left]);
                        }
                        return;
                    }

                    clusterStart = left;
                    relativeRoot = root - clusterStart;
                }

                ssize_t relativeLeft = (relativeRoot + 1) * arity;
                ssize_t relativeClusterEnd = std::min(count - clusterStart,
                        clusterSize);

                ItemType * const cluster = a + clusterStart;
                while (relativeLeft < clusterSize) {
                    ssize_t const relativeRight = relativeLeft + 1;

                    if (relativeRight < relativeClusterEnd) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeLeft])) {
                            if (compOp(cluster[relativeLeft], Below,
                                    cluster[relativeRight])) {
                                std::swap(cluster[relativeRoot],
                                        cluster[relativeRight]);
                                relativeRoot = relativeRight;
                            } else {
                                std::swap(cluster[relativeRoot],
                                        cluster[relativeLeft]);
                                relativeRoot = relativeLeft;
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeRight])) {
                                std::swap(cluster[relativeRoot],
                                        cluster[relativeRight]);
                                relativeRoot = relativeRight;
                            } else {
                                return;
                            }
                        }
                    } else {
                        if (relativeLeft < relativeClusterEnd &&
                                compOp(cluster[relativeRoot], Below,
                                cluster[relativeLeft])) {
                            std::swap(cluster[relativeRoot],
                                    cluster[relativeLeft]);
                        }
                        return;
                    }
                    relativeLeft = (relativeRoot + 1) * arity;
                }
                root = clusterStart + relativeRoot;
                left = clusterStart * clusterArity +
                        (relativeRoot - relativeLastLevelStart + 1)
                        * clusterSize;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void heapify(ItemType * const a, ssize_t const count) {
            ssize_t const end = count;
            ssize_t constexpr clusterSize =
                    computeClusterSize < clusterLevels + 1 > (arity) - 1;
            ssize_t constexpr clusterArity =
                    computeClusterLevelSize<clusterLevels>(arity);
            ssize_t constexpr relativeLastLevelStart =
                    computeClusterSize<clusterLevels>(arity) - 1;

            ssize_t item = count - 1;
            ssize_t localClusterStart = item / clusterSize * clusterSize;
            ssize_t left = localClusterStart * clusterArity;
            ssize_t relativeLeft = clusterSize - arity;
            if (item >= localClusterStart + relativeLastLevelStart) {
                left += (item - localClusterStart - relativeLastLevelStart + 1)
                        * clusterSize;                
            } else {
                relativeLeft = (item - localClusterStart + 1) * arity;
            }
            while (item >= 0) {
                while (item >= localClusterStart + relativeLastLevelStart) {
                    siftDownLast<ItemType, compOp, clusterLevels>(a,
                            item, left, count, localClusterStart);
                    item--;
                    left -= clusterSize;
                }
                while (item >= localClusterStart) {
                    siftDownInit<ItemType, compOp, clusterLevels>(a, item,
                            relativeLeft, end, localClusterStart);
                    item--;
                    relativeLeft -= arity;
                }
                localClusterStart -= clusterSize;
                relativeLeft = clusterSize - arity;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void drainHeap(ItemType * const a, ssize_t const count) {
            for (ssize_t next = count - 1; next > 0; next--) {
                siftDownInit<ItemType, compOp, clusterLevels>
                        (a, next, 0, next, 0);
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
    ssize_t clusterLevels = 5 >
    void ClusteredBinaryHeapSortVariantB(ItemType * const a,
            ssize_t const count) {
        privateClusteredBinaryHeapSortVariantB::
                heapsort<ItemType, compOp, clusterLevels>(a, count);
    }

    template<typename ItemType>
    void ClusteredBinaryHeapSortVariantB(ItemType * const a,
            ssize_t const count) {
        ClusteredBinaryHeapSortVariantB<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPBINARYCLUSTEREDVARIANTB_HPP */
