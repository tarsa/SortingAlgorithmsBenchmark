/* 
 * sortheapternaryclusteredvariantb.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPTERNARYCLUSTEREDVARIANTB_HPP
#define	SORTHEAPTERNARYCLUSTEREDVARIANTB_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateClusteredTernaryHeapSortVariantB {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr arity = 3;

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void siftDownInit(ItemType * const a, ssize_t root,
                ssize_t relativeLeft, ssize_t const count, ssize_t clusterStart,
                ssize_t globalLevelStart, ssize_t globalLevelSize) {
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
                    ssize_t const relativeMiddle = relativeLeft + 1;
                    ssize_t const relativeRight = relativeMiddle + 1;

                    if (relativeRight < relativeClusterEnd) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeLeft])) {
                            if (compOp(cluster[relativeLeft], Below,
                                    cluster[relativeMiddle])) {
                                if (compOp(cluster[relativeMiddle], Below,
                                        cluster[relativeRight])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeRight]);
                                    relativeRoot = relativeRight;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeMiddle]);
                                    relativeRoot = relativeMiddle;
                                }
                            } else {
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
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeMiddle])) {
                                if (compOp(cluster[relativeMiddle], Below,
                                        cluster[relativeRight])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeRight]);
                                    relativeRoot = relativeRight;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeMiddle]);
                                    relativeRoot = relativeMiddle;
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
                        }
                    } else {
                        ssize_t biggest = relativeRoot;
                        if (relativeLeft < relativeClusterEnd &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeLeft])) {
                            biggest = relativeLeft;
                        }
                        if (relativeMiddle < relativeClusterEnd &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeMiddle])) {
                            biggest = relativeMiddle;
                        }
                        std::swap(cluster[relativeRoot], cluster[biggest]);
                        return;
                    }
                    relativeLeft = (relativeRoot + 1) * arity;
                }
                root = clusterStart + relativeRoot;
                {
                    ssize_t const left = (clusterStart - globalLevelStart)
                            * clusterArity + (relativeRoot
                            - relativeLastLevelStart) * clusterSize
                            + globalLevelStart + globalLevelSize;
                    ssize_t const middle = left + 1;
                    ssize_t const right = middle + 1;

                    if (right < count) {
                        if (compOp(a[root], Below, a[left])) {
                            if (compOp(a[left], Below, a[middle])) {
                                if (compOp(a[middle], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    std::swap(a[root], a[middle]);
                                    root = middle;
                                }
                            } else {
                                if (compOp(a[left], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    std::swap(a[root], a[left]);
                                    root = left;
                                }
                            }
                        } else {
                            if (compOp(a[root], Below, a[middle])) {
                                if (compOp(a[middle], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    std::swap(a[root], a[middle]);
                                    root = middle;
                                }
                            } else {
                                if (compOp(a[root], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    return;
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = root;
                        if (left < count &&
                                compOp(a[biggest], Below, a[left])) {
                            biggest = left;
                        }
                        if (middle < count &&
                                compOp(a[biggest], Below, a[middle])) {
                            biggest = middle;
                        }
                        if (biggest != root) {
                            std::swap(a[root], a[biggest]);
                        }
                        return;
                    }

                    clusterStart = left;
                    relativeRoot = root - clusterStart;
                    relativeLeft = (relativeRoot + 1) * arity;

                    globalLevelStart += globalLevelSize;
                    globalLevelSize *= clusterArity;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void siftDownLast(ItemType * const a, ssize_t root,
                ssize_t left, ssize_t const count, ssize_t clusterStart,
                ssize_t globalLevelStart, ssize_t globalLevelSize) {
            ssize_t constexpr clusterSize =
                    computeClusterSize < clusterLevels + 1 > (arity) - 1;
            ssize_t constexpr clusterArity =
                    computeClusterLevelSize<clusterLevels>(arity);
            ssize_t constexpr relativeLastLevelStart =
                    computeClusterSize<clusterLevels>(arity) - 1;

            ssize_t relativeRoot;
            while (true) {
                {
                    ssize_t const middle = left + 1;
                    ssize_t const right = middle + 1;

                    if (right < count) {
                        if (compOp(a[root], Below, a[left])) {
                            if (compOp(a[left], Below, a[middle])) {
                                if (compOp(a[middle], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    std::swap(a[root], a[middle]);
                                    root = middle;
                                }
                            } else {
                                if (compOp(a[left], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    std::swap(a[root], a[left]);
                                    root = left;
                                }
                            }
                        } else {
                            if (compOp(a[root], Below, a[middle])) {
                                if (compOp(a[middle], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    std::swap(a[root], a[middle]);
                                    root = middle;
                                }
                            } else {
                                if (compOp(a[root], Below, a[right])) {
                                    std::swap(a[root], a[right]);
                                    root = right;
                                } else {
                                    return;
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = root;
                        if (left < count &&
                                compOp(a[biggest], Below, a[left])) {
                            biggest = left;
                        }
                        if (middle < count &&
                                compOp(a[biggest], Below, a[middle])) {
                            biggest = middle;
                        }
                        if (biggest != root) {
                            std::swap(a[root], a[biggest]);
                        }
                        return;
                    }

                    clusterStart = left;
                    relativeRoot = root - clusterStart;

                    globalLevelStart += globalLevelSize;
                    globalLevelSize *= clusterArity;
                }

                ssize_t relativeLeft = (relativeRoot + 1) * arity;
                ssize_t relativeClusterEnd = std::min(count - clusterStart,
                        clusterSize);

                ItemType * const cluster = a + clusterStart;
                while (relativeLeft < clusterSize) {
                    ssize_t const relativeMiddle = relativeLeft + 1;
                    ssize_t const relativeRight = relativeMiddle + 1;

                    if (relativeRight < relativeClusterEnd) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeLeft])) {
                            if (compOp(cluster[relativeLeft], Below,
                                    cluster[relativeMiddle])) {
                                if (compOp(cluster[relativeMiddle], Below,
                                        cluster[relativeRight])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeRight]);
                                    relativeRoot = relativeRight;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeMiddle]);
                                    relativeRoot = relativeMiddle;
                                }
                            } else {
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
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeMiddle])) {
                                if (compOp(cluster[relativeMiddle], Below,
                                        cluster[relativeRight])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeRight]);
                                    relativeRoot = relativeRight;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeMiddle]);
                                    relativeRoot = relativeMiddle;
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
                        }
                    } else {
                        ssize_t biggest = relativeRoot;
                        if (relativeLeft < relativeClusterEnd &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeLeft])) {
                            biggest = relativeLeft;
                        }
                        if (relativeMiddle < relativeClusterEnd &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeMiddle])) {
                            biggest = relativeMiddle;
                        }
                        std::swap(cluster[relativeRoot], cluster[biggest]);
                        return;
                    }
                    relativeLeft = (relativeRoot + 1) * arity;
                }
                root = clusterStart + relativeRoot;
                left = (clusterStart - globalLevelStart)
                        * clusterArity + (relativeRoot
                        - relativeLastLevelStart) * clusterSize
                        + globalLevelStart + globalLevelSize;
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

            ssize_t globalLevelStart = 0;
            ssize_t globalLevelSize = clusterSize;
            while (globalLevelStart + globalLevelSize < end) {
                globalLevelStart += globalLevelSize;
                globalLevelSize *= clusterArity;
            }
            ssize_t item = count - 1;
            ssize_t localClusterStart = item / clusterSize * clusterSize;
            ssize_t localLevelStart = localClusterStart;
            ssize_t localLevelSize = arity;
            ssize_t localLevel = 0;
            while (localLevelStart + localLevelSize <= item) {
                localLevelStart += localLevelSize;
                localLevelSize *= arity;
                localLevel++;
            }
            while (true) {
                ssize_t relativeLeft;
                if (localLevel == clusterLevels - 1) {
                    relativeLeft = (localClusterStart
                            - globalLevelStart) * clusterArity + (item
                            - localClusterStart - relativeLastLevelStart)
                            * clusterSize + globalLevelStart + globalLevelSize;
                    while (item >= localLevelStart) {
                        siftDownLast<ItemType, compOp, clusterLevels>(a,
                                item, relativeLeft, count, localClusterStart,
                                globalLevelStart, globalLevelSize);
                        item--;
                        relativeLeft -= clusterSize;
                    }
                    localLevel--;
                    if (localLevel >= 0) {
                        localLevelSize /= arity;
                        localLevelStart -= localLevelSize;
                    }
                }
                relativeLeft = (item - localClusterStart + 1) * arity;
                while (localLevel >= 0) {
                    while (item >= localLevelStart) {
                        siftDownInit<ItemType, compOp, clusterLevels>(a, item,
                                relativeLeft, end,
                                localClusterStart, globalLevelStart,
                                globalLevelSize);
                        item--;
                        relativeLeft -= arity;
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
                localLevelSize = computeClusterLevelSize<clusterLevels>(arity);
                localLevelStart = localClusterStart + relativeLastLevelStart;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp,
        ssize_t clusterLevels>
        void drainHeap(ItemType * const a, ssize_t const count) {
            for (ssize_t i = count - 1; i > 0; i--) {
                siftDownInit<ItemType, compOp, clusterLevels>
                        (a, i, 0, i, 0, 0,
                        computeClusterSize < clusterLevels + 1 > (arity) - 1);
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
    void ClusteredTernaryHeapSortVariantB(ItemType * const a,
            ssize_t const count) {
        privateClusteredTernaryHeapSortVariantB::
                heapsort<ItemType, compOp, clusterLevels>(a, count);
    }

    template<typename ItemType>
    void ClusteredTernaryHeapSortVariantB(ItemType * const a,
            ssize_t const count) {
        ClusteredTernaryHeapSortVariantB<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPTERNARYCLUSTEREDVARIANTB_HPP */
