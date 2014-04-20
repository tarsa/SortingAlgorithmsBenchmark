/* 
 * sortheaphybrid.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPHYBRID_HPP
#define	SORTHEAPHYBRID_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateHybridHeapSort {

        using namespace privateClusteredHeapsorts;

        ssize_t constexpr topClusterStepArity = 3;
        ssize_t constexpr topClusterLevels = 8;
        ssize_t constexpr topClusterSize = computeClusterSize<topClusterLevels>(
                topClusterStepArity);
        ssize_t constexpr topClusterTotalArity = computeClusterLevelSize<
                topClusterLevels - 1 > (topClusterStepArity);
        ssize_t constexpr topClusterLastLevelStart =
                computeClusterSize<topClusterLevels - 1 > (topClusterStepArity);

        ssize_t constexpr smallClusterLevels = 2;
        ssize_t constexpr smallClusterArities[] = {3, 4};
        ssize_t constexpr smallClusterSize = 4 * (3 + 1);
        ssize_t constexpr smallClusterTotalArity = 4 * 3;
        ssize_t constexpr smallClusterSecondLevelStart = 4;

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownFromLongLink(ItemType * const a, ssize_t const end,
                ssize_t root, ssize_t clusterStart, ssize_t relativeChild1,
                ssize_t globalLevelStart, ssize_t globalLevelSize) {
            ssize_t const last = end - 1;
            while (true) {
                ItemType * const cluster = a + clusterStart;
                ssize_t relativeRoot = root - clusterStart;
                ssize_t const relativeLast = last - clusterStart;

                {
                    ssize_t const relativeChild2 = relativeChild1 + 1;
                    ssize_t const relativeChild3 = relativeChild2 + 1;
                    ssize_t const relativeChild4 = relativeChild3 + 1;

                    if (relativeChild4 <= relativeLast) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeChild1])) {
                            if (compOp(cluster[relativeChild1], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeChild2], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild2]);
                                        relativeRoot = relativeChild2;
                                    }
                                }
                            } else {
                                if (compOp(cluster[relativeChild1], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeChild1], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild1]);
                                        relativeRoot = relativeChild1;
                                    }
                                }
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeChild2], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild2]);
                                        relativeRoot = relativeChild2;
                                    }
                                }
                            } else {
                                if (compOp(cluster[relativeRoot], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeRoot], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        return;
                                    }
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = relativeRoot;
                        if (relativeChild1 <= relativeLast && compOp(
                                cluster[biggest], Below,
                                cluster[relativeChild1])) {
                            biggest = relativeChild1;
                        }
                        if (relativeChild2 <= relativeLast && compOp(
                                cluster[biggest], Below,
                                cluster[relativeChild2])) {
                            biggest = relativeChild2;
                        }
                        if (relativeChild3 <= relativeLast && compOp(
                                cluster[biggest], Below,
                                cluster[relativeChild3])) {
                            biggest = relativeChild3;
                        }
                        std::swap(cluster[biggest], cluster[relativeRoot]);
                        return;
                    }
                }
                relativeChild1 = smallClusterSecondLevelStart + relativeRoot
                        * smallClusterArities[0];
                {
                    ssize_t const relativeChild2 = relativeChild1 + 1;
                    ssize_t const relativeChild3 = relativeChild2 + 1;

                    if (relativeChild3 < relativeLast) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeChild1])) {
                            if (compOp(cluster[relativeChild1], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild2]);
                                    relativeRoot = relativeChild2;
                                }
                            } else {
                                if (compOp(cluster[relativeChild1], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild1]);
                                    relativeRoot = relativeChild1;
                                }
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild2]);
                                    relativeRoot = relativeChild2;
                                }
                            } else {
                                if (compOp(cluster[relativeRoot], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    return;
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = relativeRoot;
                        if (relativeChild1 < relativeLast &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeChild1])) {
                            biggest = relativeChild1;
                        }
                        if (relativeChild2 < relativeLast &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeChild2])) {
                            biggest = relativeChild2;
                        }
                        std::swap(cluster[relativeRoot], cluster[biggest]);
                        return;
                    }
                }
                root = relativeRoot + clusterStart;
                clusterStart = (relativeRoot - smallClusterSecondLevelStart)
                        * smallClusterSize + (clusterStart - globalLevelStart)
                        * smallClusterTotalArity + globalLevelStart
                        + globalLevelSize;
                relativeChild1 = 0;
                globalLevelStart += globalLevelSize;
                globalLevelSize *= smallClusterTotalArity;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownFromShortLink(ItemType * const a, ssize_t const end,
                ssize_t root, ssize_t clusterStart, ssize_t relativeChild1,
                ssize_t globalLevelStart, ssize_t globalLevelSize) {
            ssize_t const last = end - 1;
            ItemType * cluster = a + clusterStart;
            ssize_t relativeRoot = root - clusterStart;
            ssize_t relativeLast = last - clusterStart;
            while (true) {
                {
                    ssize_t const relativeChild2 = relativeChild1 + 1;
                    ssize_t const relativeChild3 = relativeChild2 + 1;

                    if (relativeChild3 < relativeLast) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeChild1])) {
                            if (compOp(cluster[relativeChild1], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild2]);
                                    relativeRoot = relativeChild2;
                                }
                            } else {
                                if (compOp(cluster[relativeChild1], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild1]);
                                    relativeRoot = relativeChild1;
                                }
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild2]);
                                    relativeRoot = relativeChild2;
                                }
                            } else {
                                if (compOp(cluster[relativeRoot], Below,
                                        cluster[relativeChild3])) {
                                    std::swap(cluster[relativeRoot],
                                            cluster[relativeChild3]);
                                    relativeRoot = relativeChild3;
                                } else {
                                    return;
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = relativeRoot;
                        if (relativeChild1 < relativeLast &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeChild1])) {
                            biggest = relativeChild1;
                        }
                        if (relativeChild2 < relativeLast &&
                                compOp(cluster[biggest], Below,
                                cluster[relativeChild2])) {
                            biggest = relativeChild2;
                        }
                        std::swap(cluster[relativeRoot], cluster[biggest]);
                        return;
                    }
                }
                root = relativeRoot + clusterStart;
                clusterStart = (relativeRoot - smallClusterSecondLevelStart)
                        * smallClusterSize + (clusterStart - globalLevelStart)
                        * smallClusterTotalArity + globalLevelStart
                        + globalLevelSize;
                relativeChild1 = 0;
                globalLevelStart += globalLevelSize;
                globalLevelSize *= smallClusterTotalArity;

                cluster = a + clusterStart;
                relativeRoot = root - clusterStart;
                relativeLast = last - clusterStart;
                {
                    ssize_t const relativeChild2 = relativeChild1 + 1;
                    ssize_t const relativeChild3 = relativeChild2 + 1;
                    ssize_t const relativeChild4 = relativeChild3 + 1;

                    if (relativeChild4 <= relativeLast) {
                        if (compOp(cluster[relativeRoot], Below,
                                cluster[relativeChild1])) {
                            if (compOp(cluster[relativeChild1], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeChild2], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild2]);
                                        relativeRoot = relativeChild2;
                                    }
                                }
                            } else {
                                if (compOp(cluster[relativeChild1], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeChild1], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild1]);
                                        relativeRoot = relativeChild1;
                                    }
                                }
                            }
                        } else {
                            if (compOp(cluster[relativeRoot], Below,
                                    cluster[relativeChild2])) {
                                if (compOp(cluster[relativeChild2], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeChild2], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild2]);
                                        relativeRoot = relativeChild2;
                                    }
                                }
                            } else {
                                if (compOp(cluster[relativeRoot], Below,
                                        cluster[relativeChild3])) {
                                    if (compOp(cluster[relativeChild3], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild3]);
                                        relativeRoot = relativeChild3;
                                    }
                                } else {
                                    if (compOp(cluster[relativeRoot], Below,
                                            cluster[relativeChild4])) {
                                        std::swap(cluster[relativeRoot],
                                                cluster[relativeChild4]);
                                        relativeRoot = relativeChild4;
                                    } else {
                                        return;
                                    }
                                }
                            }
                        }
                    } else {
                        ssize_t biggest = relativeRoot;
                        if (relativeChild1 <= relativeLast && compOp(
                                cluster[biggest], Below,
                                cluster[relativeChild1])) {
                            biggest = relativeChild1;
                        }
                        if (relativeChild2 <= relativeLast && compOp(
                                cluster[biggest], Below,
                                cluster[relativeChild2])) {
                            biggest = relativeChild2;
                        }
                        if (relativeChild3 <= relativeLast && compOp(
                                cluster[biggest], Below,
                                cluster[relativeChild3])) {
                            biggest = relativeChild3;
                        }
                        std::swap(cluster[biggest], cluster[relativeRoot]);
                        return;
                    }
                }
                relativeChild1 = smallClusterSecondLevelStart + relativeRoot
                        * smallClusterArities[0];
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownTip(ItemType * const a, ssize_t root, ssize_t const end) {
            while (root < topClusterLastLevelStart) {
                ssize_t const first = root * 3 + 1;
                ssize_t const middle = first + 1;
                ssize_t const last = middle + 1;

                if (last < end) {
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
                    if (first < end && compOp(a[biggest], Below, a[first])) {
                        biggest = first;
                    }
                    if (middle < end && compOp(a[biggest], Below, a[middle])) {
                        biggest = middle;
                    }
                    std::swap(a[root], a[biggest]);
                    return;
                }
            }
            if (root >= topClusterLastLevelStart && end >= topClusterSize) {
                siftDownFromLongLink<ItemType, compOp>(a, end, root,
                        (root - topClusterLastLevelStart) * smallClusterSize
                        + topClusterSize, 0, topClusterSize, smallClusterSize
                        * topClusterTotalArity);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            ssize_t const end = count;
            ssize_t const item = count - 1;
            ssize_t topRelativeItem = item - topClusterSize;

            if (topRelativeItem >= 0) {
                ssize_t const smallClustersStart = topClusterSize;
                ssize_t topRelativeGlobalLevelStart = 0;
                ssize_t globalLevelSize = topClusterTotalArity
                        * smallClusterSize;
                while (topRelativeGlobalLevelStart + globalLevelSize
                        <= topRelativeItem) {
                    topRelativeGlobalLevelStart += globalLevelSize;
                    globalLevelSize *= smallClusterTotalArity;
                }
                ssize_t topRelativeClusterStart = topRelativeItem
                        / smallClusterSize * smallClusterSize;
                topRelativeItem = std::min(topRelativeItem,
                        topRelativeClusterStart +
                        smallClusterSecondLevelStart - 1);
                while (topRelativeItem >= topRelativeClusterStart) {
                    siftDownFromLongLink<ItemType, compOp>(a, end,
                            smallClustersStart + topRelativeItem,
                            topRelativeClusterStart + topClusterSize,
                            smallClusterSecondLevelStart + (topRelativeItem
                            - topRelativeClusterStart) * smallClusterArities[0],
                            topRelativeGlobalLevelStart + topClusterSize,
                            globalLevelSize);
                    topRelativeItem--;
                }
                while (topRelativeItem >= 0) {
                    topRelativeClusterStart -= smallClusterSize;
                    if (topRelativeItem < topRelativeGlobalLevelStart) {
                        globalLevelSize /= smallClusterTotalArity;
                        topRelativeGlobalLevelStart -= globalLevelSize;
                    }
                    while (topRelativeItem >= topRelativeClusterStart
                            + smallClusterSecondLevelStart) {
                        siftDownFromLongLink<ItemType, compOp>(a, end,
                                smallClustersStart + topRelativeItem,
                                (topRelativeItem - topRelativeClusterStart -
                                smallClusterSecondLevelStart) * smallClusterSize
                                + (topRelativeClusterStart
                                - topRelativeGlobalLevelStart)
                                * smallClusterTotalArity
                                + topRelativeGlobalLevelStart + globalLevelSize
                                + topClusterSize, 0,
                                topRelativeGlobalLevelStart + topClusterSize,
                                globalLevelSize);
                        topRelativeItem--;
                    }
                    while (topRelativeItem >= topRelativeClusterStart) {
                        siftDownFromShortLink<ItemType, compOp>(a, end,
                                smallClustersStart + topRelativeItem,
                                topRelativeClusterStart + topClusterSize,
                                smallClusterSecondLevelStart + (topRelativeItem
                                - topRelativeClusterStart)
                                * smallClusterArities[0],
                                topRelativeGlobalLevelStart + topClusterSize,
                                globalLevelSize);
                        topRelativeItem--;
                    }
                }
            }
            for (ssize_t start = std::min(topClusterSize - 1, count - 1);
                    start >= 0; start--) {
                siftDownTip<ItemType, compOp>(a, start, count);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t last = count - 1;
            while (last > 0) {
                std::swap(a[last], a[0]);
                last--;
                siftDownTip<ItemType, compOp>(a, 0, last + 1);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void HybridHeapSort(ItemType * const a, ssize_t const count) {
        privateHybridHeapSort::heapsort<ItemType, compOp>(
                a, count);
    }

    template<typename ItemType>
    void HybridHeapSort(ItemType * const a, ssize_t const count) {
        HybridHeapSort<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPHYBRID_HPP */
