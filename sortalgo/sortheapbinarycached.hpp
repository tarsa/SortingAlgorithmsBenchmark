/* 
 * sortheapbinarycached.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPBINARYCACHED_HPP
#define	SORTHEAPBINARYCACHED_HPP

#include "sortalgocommon.hpp"

namespace tarsa {
    namespace privateCachedComparisonsBinaryHeapSort {
        
        using namespace privateClusteredHeapsorts;

        ssize_t constexpr Arity = 2;
        ssize_t constexpr ClusterLevels = 3;
        ssize_t constexpr ClusterLastLevelStart =
                computeClusterSize<ClusterLevels>(Arity) - 1;
        ssize_t constexpr ClusterLastLevelSize =
                computeClusterLevelSize<ClusterLevels>(Arity);
        ssize_t constexpr ClusterSize =
                ClusterLastLevelStart + ClusterLastLevelSize;
        ssize_t constexpr ClusterArity = ClusterLastLevelSize;
        
        using namespace compileTimeConstArrays;

        int8_t constexpr compMax2(ssize_t cachedComparisons) {
            return ((cachedComparisons & 1) + 1) * Arity + 
                    ((cachedComparisons >> ((cachedComparisons & 1) + 1)) & 1);
        }

        int8_t constexpr compMax3(ssize_t cachedComparisons) {
            return (compMax2(cachedComparisons) + 1) * Arity + 
                    ((cachedComparisons >> (compMax2(cachedComparisons) 
                    + 1)) & 1);
        }
        
        int8_t constexpr compFilteredComparisons(ssize_t cachedComparisons) {
            return cachedComparisons & ~((1) | (1 << ((cachedComparisons & 1)
                    + 1)) | (1 << (compMax2(cachedComparisons) + 1)));
        }
        
        ComputedArray<1 << 7, int8_t> getMax2(gen_seq<1 << 7>(), compMax2);
        ComputedArray<1 << 7, int8_t> getMax3(gen_seq<1 << 7>(), compMax3);        
        ComputedArray<1 << (7 + 2), int8_t> getFilteredComparisons(
                gen_seq<1 << (7 + 2)>(), compFilteredComparisons);

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDownUncached(ItemType * const a, ssize_t const count,
                ssize_t clusterStart, ssize_t current, ssize_t level) {
            assert(clusterStart % ClusterSize == 0);
            assert(current >= 0);
            assert(current < ClusterSize);
            assert(level >= 1);
            assert(level <= ClusterLevels);
            while (clusterStart <= count - ClusterSize) {
                ItemType * const cluster = a + clusterStart;
                while (level < ClusterLevels) {
                    ssize_t const child1 = (current + 1) * Arity;
                    ssize_t const child2 = child1 + 1;
                    ssize_t const leader12 = child1
                            + compOp(cluster[child1], Below, cluster[child2]);
                    if (compOp(cluster[current], Below, cluster[leader12])) {
                        std::swap(cluster[current], cluster[leader12]);
                        current = leader12;
                    } else {
                        return;
                    }
                    level++;
                }
                {
                    assert(current >= ClusterLastLevelStart);
                    assert(current < ClusterSize);
                    assert(level == ClusterLevels);
                    ssize_t const childClusterStart = (current
                            - ClusterLastLevelStart + 1) * ClusterSize
                            + clusterStart * ClusterArity;
                    assert(childClusterStart % ClusterSize == 0);
                    if (childClusterStart > count - Arity) {
                        if (childClusterStart == count - 1) {
                            if (compOp(cluster[current], Below,
                                    a[childClusterStart])) {
                                std::swap(cluster[current],
                                        a[childClusterStart]);
                            }
                        }
                        return;
                    }
                    ItemType * const childCluster = a + childClusterStart;
                    ssize_t const child1 = 0;
                    ssize_t const child2 = child1 + 1;
                    ssize_t const leader12 = child1 + compOp(
                            childCluster[child1], Below, childCluster[child2]);
                    if (compOp(cluster[current], Below,
                            childCluster[leader12])) {
                        std::swap(cluster[current], childCluster[leader12]);
                        current = leader12;
                        clusterStart = childClusterStart;
                    } else {
                        return;
                    }
                }
                level = 1;
            }
            assert(level == 1 || clusterStart == 0);
            {
                ItemType * const cluster = a + clusterStart;
                while (level < ClusterLevels) {
                    ssize_t const child1 = (current + 1) * Arity;
                    ssize_t const child2 = child1 + 1;
                    if (clusterStart + child2 < count) {
                        ssize_t const leader12 = child1 + compOp(
                                cluster[child1], Below, cluster[child2]);
                        if (compOp(cluster[current], Below,
                                cluster[leader12])) {
                            std::swap(cluster[current], cluster[leader12]);
                            current = leader12;
                        } else {
                            return;
                        }
                        level++;
                    } else if (clusterStart + child1 < count) {
                        if (compOp(cluster[current], Below, cluster[child1])) {
                            std::swap(cluster[current], cluster[child1]);
                        }
                        return;
                    } else {
                        return;
                    }
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            ssize_t const last = count - 1;
            ssize_t positionInCluster = last % ClusterSize;
            ssize_t clusterStart = last - positionInCluster;
            while (clusterStart >= 0) {
                while (positionInCluster >= Arity + Arity * Arity) {
                    siftDownUncached<ItemType, compOp>(a, count,
                            clusterStart, positionInCluster, 3);
                    positionInCluster--;
                }
                while (positionInCluster >= Arity) {
                    siftDownUncached<ItemType, compOp>(a, count,
                            clusterStart, positionInCluster, 2);
                    positionInCluster--;
                }
                while (positionInCluster >= 0) {
                    siftDownUncached<ItemType, compOp>(a, count,
                            clusterStart, positionInCluster, 1);
                    positionInCluster--;
                }
                positionInCluster = ClusterSize - 1;
                clusterStart -= ClusterSize;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void initCachedComparisons(ItemType * const a, ssize_t const count,
                int8_t * const cachedComparisons) {
            ssize_t const last = count - 1;
            ssize_t cacheIndex = last / ClusterSize;
            ssize_t clusterStart = cacheIndex * ClusterSize;
            ssize_t positionInCluster = last - clusterStart;
            if (positionInCluster == 0) {
                cachedComparisons[cacheIndex] = 0;
                cacheIndex--;
                clusterStart -= ClusterSize;
                positionInCluster = ClusterSize - 1;
            } else if (positionInCluster % 2 == 0) {
                positionInCluster--;
            }
            while (cacheIndex >= 0) {
                assert(positionInCluster % 2 == 1);
                ItemType * const cluster = a + clusterStart;
                int8_t localCachedComparisons = 0;
                while (positionInCluster > 0) {
                    localCachedComparisons |= compOp(
                            cluster[positionInCluster - 1], Below,
                            cluster[positionInCluster])
                            << (positionInCluster / 2);
                    positionInCluster -= 2;
                }
                assert(positionInCluster == -1);
                cachedComparisons[cacheIndex] = localCachedComparisons;
                cacheIndex--;
                clusterStart -= ClusterSize;
                positionInCluster = ClusterSize - 1;
            }
        }

#ifndef NDEBUG

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void checkHeapState(ItemType * const a, ssize_t const count,
                int8_t * const cachedComparisons) {
            ssize_t const last = count - 1;
            ssize_t cacheIndex = last / ClusterSize;
            ssize_t clusterStart = cacheIndex * ClusterSize;
            ssize_t positionInCluster = last - clusterStart;
            assert(cachedComparisons[cacheIndex]
                    < (1 << ((positionInCluster + 1) / 2)));
            while (clusterStart >= 0) {
                int8_t const cachedComparison = cachedComparisons[cacheIndex];
                ItemType * const cluster = a + clusterStart;
                while (positionInCluster >= Arity + Arity * Arity) {
                    ssize_t const childClusterStart = (positionInCluster
                            - ClusterLastLevelStart + 1) * ClusterSize
                            + clusterStart * ClusterArity;
                    ssize_t const child1 = 0;
                    ssize_t const child2 = child1 + 1;
                    ItemType * const childCluster = a + childClusterStart;
                    assert(childClusterStart + child1 > last
                            || !compOp(cluster[positionInCluster],
                            Below, childCluster[child1]));
                    assert(childClusterStart + child2 > last
                            || !compOp(cluster[positionInCluster],
                            Below, childCluster[child2]));
                    if (childClusterStart < count) {
                        int8_t const childCachedComparison =
                                cachedComparisons[childClusterStart
                                / ClusterSize];
                        if (childClusterStart + child1 == last) {
                            assert(!(childCachedComparison & 1));
                        }
                        if (childClusterStart + child2 <= last) {
                            assert((childCachedComparison & 1)
                                    == compOp(childCluster[child1], Below,
                                    childCluster[child2]));
                        }
                    }
                    positionInCluster--;
                }
                while (positionInCluster >= 0) {
                    ssize_t const child1 = (positionInCluster + 1) * Arity;
                    ssize_t const child2 = child1 + 1;
                    assert(clusterStart + child1 > last || !compOp(
                            cluster[positionInCluster], Below,
                            cluster[child1]));
                    assert(clusterStart + child2 > last || !compOp(
                            cluster[positionInCluster], Below,
                            cluster[child2]));
                    if (clusterStart + child1 == last) {
                        assert(!((cachedComparison >> (child1 / 2)) & 1));
                    }
                    if (clusterStart + child2 <= last) {
                        assert(((cachedComparison >> (child1 / 2)) & 1)
                                == compOp(cluster[child1], Below,
                                cluster[child2]));
                    }
                    positionInCluster--;
                }
                cacheIndex--;
                clusterStart -= ClusterSize;
                positionInCluster = ClusterSize - 1;
            }
            if (count >= 2) {
                assert((cachedComparisons[0] & 1) == compOp(a[0], Below, a[1]));
            }
        }
#endif

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDown(ItemType * const a, ssize_t const next,
                int8_t * const cachedComparisonsTable) {
            if (next < 2) {
                if (next == 1 && compOp(a[1], Below, a[0])) {
                    std::swap(a[0], a[1]);
                }
                return;
            }
            ItemType const itemCopy = a[next];
            ssize_t const count = next;
            ssize_t const lastCacheIndex = (count - 1) / ClusterSize;

            ssize_t cacheIndex = 0;
            ssize_t clusterStart = cacheIndex * ClusterSize;
            int8_t cachedComparisons = cachedComparisonsTable[cacheIndex];
            ssize_t max1 = cachedComparisons & 1;
            ItemType * cluster = a + clusterStart;
            a[next] = cluster[max1];

            ssize_t max2 = getMax2[cachedComparisons];
            ssize_t max3 = getMax3[cachedComparisons];
            int8_t filteredCachedComparisons = 
                    getFilteredComparisons[cachedComparisons];
            ssize_t childCacheIndex = (max3 - ClusterLastLevelStart + 1)
                    + cacheIndex * ClusterArity;
            while (childCacheIndex <= lastCacheIndex) {
                ssize_t childClusterStart = childCacheIndex * ClusterSize;
                int8_t childCachedComparisons = cachedComparisonsTable[
                        childCacheIndex];
                ssize_t const childMax1 = childCachedComparisons & 1;
                ItemType * const childCluster = a + childClusterStart;
                if (!compOp(itemCopy, Below, childCluster[childMax1])) {
                    break;
                }
                cluster[max1] = cluster[max2];
                cluster[max2] = cluster[max3];
                cluster[max3] = childCluster[childMax1];
                cachedComparisonsTable[cacheIndex] = filteredCachedComparisons
                        | (compOp(cluster[0], Below, cluster[1]))
                        | (compOp(cluster[(max1 + 1) * 2], Below,
                        cluster[(max1 + 1) * 2 + 1]) << (max1 + 1))
                        | (compOp(cluster[(max2 + 1) * 2], Below,
                        cluster[(max2 + 1) * 2 + 1]) << (max2 + 1));
                clusterStart = childClusterStart;
                cluster = childCluster;
                cacheIndex = childCacheIndex;
                cachedComparisons = childCachedComparisons;
                max1 = childMax1;
                max2 = getMax2[cachedComparisons];
                max3 = getMax3[cachedComparisons];
                filteredCachedComparisons = 
                        getFilteredComparisons[cachedComparisons];
                childCacheIndex = (max3 - ClusterLastLevelStart + 1)
                        + cacheIndex * ClusterArity;
            }
            if (clusterStart < count - ClusterSize) {
                ssize_t current = max1;
                ssize_t parent = current;
                if (!compOp(cluster[max2], Below, itemCopy)) {
                    current = max2;
                }
                cluster[parent] = cluster[current];
                parent = current;
                if (!compOp(cluster[max3], Below, itemCopy)) {
                    current = max3;
                }
                cluster[parent] = cluster[current];
                cluster[current] = itemCopy;
                cachedComparisonsTable[cacheIndex] = filteredCachedComparisons
                        | (compOp(cluster[0], Below, cluster[1]))
                        | (compOp(cluster[(max1 + 1) * 2], Below,
                        cluster[(max1 + 1) * 2 + 1]) << (max1 + 1))
                        | (compOp(cluster[(max2 + 1) * 2], Below,
                        cluster[(max2 + 1) * 2 + 1]) << (max2 + 1));
            } else {
                assert((count | max1) == 0 || max1 < count - clusterStart);
                ssize_t current = max1;
                if (max2 < count - clusterStart
                        && !compOp(cluster[max2], Below, itemCopy)) {
                    cluster[max1] = cluster[max2];
                    current = max2;
                    if (max3 < count - clusterStart
                            &&!compOp(cluster[max3], Below, itemCopy)) {
                        cluster[max2] = cluster[max3];
                        current = max3;
                    }
                }
                cluster[current] = itemCopy;
                int8_t adjustedCachedComparisons = filteredCachedComparisons;
                if (clusterStart + 1 < count) {
                    adjustedCachedComparisons |= compOp(cluster[0], Below,
                            cluster[1]);
                }
                if (clusterStart + (max1 + 1) * 2 + 1 < count) {
                    adjustedCachedComparisons |= compOp(cluster[(max1 + 1) * 2],
                            Below, cluster[(max1 + 1) * 2 + 1]) << (max2 / 2);
                }
                if (clusterStart + (max2 + 1) * 2 + 1 < count) {
                    adjustedCachedComparisons |= compOp(cluster[(max2 + 1) * 2],
                            Below, cluster[(max2 + 1) * 2 + 1]) << (max3 / 2);
                }
                cachedComparisonsTable[cacheIndex] = adjustedCachedComparisons;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count,
                int8_t * const cachedComparisons) {
            ssize_t next = count - 1;
            ssize_t cacheIndex = next / ClusterSize;
            ssize_t clusterStart = cacheIndex * ClusterSize;
            ssize_t positionInCluster = next - clusterStart;
            if ((positionInCluster & 1) == 0) {
                siftDown<ItemType, compOp>(a, next, cachedComparisons);
                next--;
                positionInCluster--;
            }
            while (clusterStart >= 0) {
                assert((positionInCluster & 1) == 1);
                while (positionInCluster >= 0) {
                    cachedComparisons[cacheIndex] &=
                            (1 << (positionInCluster / 2)) - 1;
                    siftDown<ItemType, compOp>(a, next, cachedComparisons);
                    next--;
                    cachedComparisons[cacheIndex] &=
                            (1 << ((positionInCluster / 2) - 1)) - 1;
                    siftDown<ItemType, compOp>(a, next, cachedComparisons);
                    next--;
                    positionInCluster -= 2;
                }
                assert(positionInCluster == -1);
                cacheIndex--;
                clusterStart -= ClusterSize;
                positionInCluster = ClusterSize - 1;
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count,
                int8_t * const cachedComparisons) {
            heapify<ItemType, compOp>(a, count);
            initCachedComparisons<ItemType, compOp>(a, count,
                    cachedComparisons);
#ifndef NDEBUG     
            checkHeapState<ItemType, compOp>(a, count, cachedComparisons);
#endif
            drainHeap<ItemType, compOp>(a, count, cachedComparisons);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void CachedComparisonsBinaryHeapSort(ItemType * const a,
            ssize_t const count, int8_t * const scratchpad) {
        privateCachedComparisonsBinaryHeapSort::heapsort<ItemType, compOp>(
                a, count, scratchpad);
    }

    template<typename ItemType>
    void CachedComparisonsBinaryHeapSort(ItemType * const a,
            ssize_t const count, int8_t * const scratchpad) {
        CachedComparisonsBinaryHeapSort<ItemType, genericComparisonOperator>(
                a, count, scratchpad);
    }
}

#endif	/* SORTHEAPBINARYCACHED_HPP */
