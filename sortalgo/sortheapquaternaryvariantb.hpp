/* 
 * sortheapquaternaryvariantb.hpp -- sorting algorithms benchmark
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

#ifndef SORTHEAPQUATERNARYVARIANTB_HPP
#define	SORTHEAPQUATERNARYVARIANTB_HPP

#include "sortalgocommon.hpp"

namespace tarsa {

    namespace privateQuaternaryHeapSortVariantB {

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void siftDown(ItemType * const a, ssize_t root, ssize_t child1,
                ssize_t const last) {
            while (true) {
                ssize_t const child2 = child1 + 1;
                ssize_t const child3 = child2 + 1;
                ssize_t const child4 = child3 + 1;
                ssize_t biggest = root;

                if (child4 <= last) {
                    if (compOp(a[biggest], Below, a[child1])) {
                        biggest = child1;
                    }
                    if (compOp(a[biggest], Below, a[child2])) {
                        biggest = child2;
                    }
                    if (compOp(a[biggest], Below, a[child3])) {
                        biggest = child3;
                    }
                    if (compOp(a[biggest], Below, a[child4])) {
                        biggest = child4;
                    }
                } else if (child1 <= last) {
                    if (compOp(a[biggest], Below, a[child1])) {
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
                } else {
                    return;
                }
                if (biggest != root) {
                    std::swap(a[root], a[biggest]);
                    root = biggest;
                    child1 = (root + 1) * 4;
                } else {
                    return;
                }
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapify(ItemType * const a, ssize_t const count) {
            for (ssize_t start = count / 4; start >= 0; start--) {
                siftDown<ItemType, compOp>(a, start, start * 4 + 4, count - 1);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void drainHeap(ItemType * const a, ssize_t const count) {
            for (ssize_t i = count - 1; i > 0; i--) {
                siftDown<ItemType, compOp>(a, i, 0, i - 1);
            }
        }

        template<typename ItemType, ComparisonOperator<ItemType> compOp>
        void heapsort(ItemType * const a, ssize_t const count) {
            heapify<ItemType, compOp>(a, count);
            drainHeap<ItemType, compOp>(a, count);
        }
    }

    template<typename ItemType, ComparisonOperator<ItemType> compOp>
    void QuaternaryHeapSortVariantB(ItemType * const a,
            ssize_t const count) {
        privateQuaternaryHeapSortVariantB::heapsort<ItemType, compOp>(
                a, count);
    }

    template<typename ItemType>
    void QuaternaryHeapSortVariantB(ItemType * const a,
            ssize_t const count) {
        QuaternaryHeapSortVariantB<ItemType, genericComparisonOperator>(
                a, count);
    }
}

#endif	/* SORTHEAPQUATERNARYVARIANTB_HPP */
