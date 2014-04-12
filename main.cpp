/* 
 * main.cpp -- sorting algorithms benchmark
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

#define NDEBUG

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <utility>

int64_t counter;

#include "sortalgo/sortheapbinaryclusteredvarianta.hpp"
#include "sortalgo/sortheapbinaryclusteredvariantb.hpp"
#include "sortalgo/sortheapbinaryonebasedvarianta.hpp"
#include "sortalgo/sortheapbinaryonebasedvariantb.hpp"
#include "sortalgo/sortheapquaternaryvarianta.hpp"
#include "sortalgo/sortheapquaternaryvariantb.hpp"
#include "sortalgo/sortheapternaryclusteredvarianta.hpp"
#include "sortalgo/sortheapternaryclusteredvariantb.hpp"
#include "sortalgo/sortheapternaryonebasedvarianta.hpp"
#include "sortalgo/sortheapternaryonebasedvariantb.hpp"
#include "sortalgo/sortquickrandomized.hpp"

using namespace tarsa;

typedef int32_t typ;

template<typename ItemType>
bool countingComparisonOperator(ItemType leftOp, ComparisonType opType,
        ItemType rightOp) {
    counter++;
    return genericComparisonOperator(leftOp, opType, rightOp);
}

#define ComparisonOperator genericComparisonOperator
//#define ComparisonOperator countingComparisonOperator

template<typename ItemType>
void testFunction(std::string name, ItemType const * const original,
        ItemType * const work, ssize_t const size,
        std::function<void() > functionInTest) {
    std::cout << name << std::endl;
    clock_t clockz;

    std::copy(original, original + size, work);
    counter = 0;
    clockz = clock();
    functionInTest();
    std::cout << counter << " comparisons" << std::endl;
    std::cout << (clock() - clockz) << " clock ticks" << std::endl;

    for (ssize_t i = 0; i + 1 < size; i++) {
        if (work[i] > work[i + 1]) {
            std::cerr << "Error in verification!" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << std::endl;
}

int main(int argc, char** argv) {
    ssize_t size = 12345678;

    srand(7);
    typ * original = new typ[size];
    for (ssize_t i = 0; i < size; i++) {
        original[i] = rand();
    }

    typ * work = new typ[size];

    testFunction("StdSort", original, work, size, [&]() {
        std::sort(work, work + size); });

    testFunction("ClusteredBinaryHeapSortVariantA", original, work, size,
            [&]() {
                ClusteredBinaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("ClusteredBinaryHeapSortVariantB", original, work, size,
            [&]() {
                ClusteredBinaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedBinaryHeapSortVariantA", original, work, size,
            [&]() {
                OneBasedBinaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedBinaryHeapSortVariantB", original, work, size,
            [&]() {
                OneBasedBinaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("QuaternaryHeapSortVariantA", original, work, size,
            [&]() {
                QuaternaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("QuaternaryHeapSortVariantB", original, work, size,
            [&]() {
                QuaternaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("ClusteredTernaryHeapSortVariantA", original, work, size,
            [&]() {
                ClusteredTernaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });
            
    testFunction("ClusteredTernaryHeapSortVariantB", original, work, size,
            [&]() {
                ClusteredTernaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });            

    testFunction("OneBasedTernaryHeapSortVariantA", original, work, size,
            [&]() {
                OneBasedTernaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedTernaryHeapSortVariantB", original, work, size,
            [&]() {
                OneBasedTernaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("RandomizedQuickSort", original, work, size,
            [&]() {
                RandomizedQuickSort<typ, ComparisonOperator>(
                        work, size);
            });

    std::cout << "Great success!" << std::endl;

    return EXIT_SUCCESS;
}
