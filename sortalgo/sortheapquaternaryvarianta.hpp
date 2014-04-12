/* 
 * sortheapquaternaryvarianta.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPQUATERNARYVARIANTA_HPP
#define	SORTHEAPQUATERNARYVARIANTA_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateQuaternaryHeapSortVariantA {

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDown(ItemType * const a, ssize_t const start,
                ssize_t const last) {
            ssize_t root = start;
            while (true) {
                ssize_t const child1 = root * 4 + 1;
                ssize_t const child2 = child1 + 1;
                ssize_t const child3 = child2 + 1;
                ssize_t const child4 = child3 + 1;

                if (child4 <= last) {
                    if (compOp(a[root], Below, a[child1])) {
                        if (compOp(a[child1], Below, a[child2])) {
                            if (compOp(a[child2], Below, a[child3])) {
                                if (compOp(a[child3], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child3]);
                                    root = child3;
                                }
                            } else {
                                if (compOp(a[child2], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child2]);
                                    root = child2;
                                }
                            }
                        } else {
                            if (compOp(a[child1], Below, a[child3])) {
                                if (compOp(a[child3], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child3]);
                                    root = child3;
                                }
                            } else {
                                if (compOp(a[child1], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child1]);
                                    root = child1;
                                }
                            }
                        }
                    } else {
                        if (compOp(a[root], Below, a[child2])) {
                            if (compOp(a[child2], Below, a[child3])) {
                                if (compOp(a[child3], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child3]);
                                    root = child3;
                                }
                            } else {
                                if (compOp(a[child2], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child2]);
                                    root = child2;
                                }
                            }
                        } else {
                            if (compOp(a[root], Below, a[child3])) {
                                if (compOp(a[child3], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    std::swap(a[root], a[child3]);
                                    root = child3;
                                }
                            } else {
                                if (compOp(a[root], Below, a[child4])) {
                                    std::swap(a[root], a[child4]);
                                    root = child4;
                                } else {
                                    return;
                                }
                            }
                        }
                    }
                } else {
                    ssize_t biggest = root;
                    if (child1 <= last &&
                            compOp(a[biggest], Below, a[child1])) {
                        biggest = child1;
                    }
                    if (child2 <= last &&
                            compOp(a[biggest], Below, a[child2])) {
                        biggest = child2;
                    }
                    if (child3 <= last &&
                            compOp(a[biggest], Below, a[child3])) {
                        biggest = child3;
                    }
                    std::swap(a[root], a[biggest]);
                    return;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            for (ssize_t start = count / 4; start >= 0; start--) {
                siftDown<ItemType, compOp>(a, start, count - 1);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            ssize_t last = count - 1;
            while (last > 0) {
                std::swap(a[last], a[0]);
                last--;
                siftDown<ItemType, compOp>(a, 0, last);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void QuaternaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        privateQuaternaryHeapSortVariantA::heapsort<ItemType, compOp>(
                a, count);
    }

    template<typename ItemType>
    void QuaternaryHeapSortVariantA(ItemType * const a,
            ssize_t const count) {
        QuaternaryHeapSortVariantA<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPQUATERNARYVARIANTA_HPP */
