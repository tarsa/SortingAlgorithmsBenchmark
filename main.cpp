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
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <utility>

int64_t counter;

#include "sortalgo/sortheapbinaryaheadsimplevarianta.hpp"
#include "sortalgo/sortheapbinaryaheadsimplevariantb.hpp"
#include "sortalgo/sortheapbinarycascadingvarianta.hpp"
#include "sortalgo/sortheapbinarycascadingvariantb.hpp"
#include "sortalgo/sortheapbinarycascadingvariantc.hpp"
#include "sortalgo/sortheapbinarycascadingvariantd.hpp"
#include "sortalgo/sortheapbinaryclusteredvarianta.hpp"
#include "sortalgo/sortheapbinaryclusteredvariantb.hpp"
#include "sortalgo/sortheapbinaryonebasedvarianta.hpp"
#include "sortalgo/sortheapbinaryonebasedvariantb.hpp"
#include "sortalgo/sortheaphybrid.hpp"
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

#if 01
#define ComparisonOperator genericComparisonOperator
#else
#define ComparisonOperator countingComparisonOperator
#endif

template<typename ItemType>
void testFunction(std::string name, ItemType const * const original,
        ItemType * const work, ItemType const * const reference,
        ssize_t const size, std::function<void() > functionInTest) {
    std::cout << name << std::endl;

    std::copy(original, original + size, work);
    counter = 0;
    clock_t clocks = clock();
    functionInTest();
    clocks = clock() - clocks;
    std::cout << counter << " comparisons" << std::endl;
    std::cout << clocks << " clock ticks" << std::endl;

    if (reference != nullptr) {
        for (ssize_t i = 0; i + 1 < size; i++) {
            if (work[i] != reference[i]) {
                std::cerr << "Error in verification!" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    std::cout << std::endl;
}

int main(int argc, char** argv) {
    ssize_t size = 12345678;

    srand(7);
    typ * original = new typ[size];
    typ * sorted = new typ[size];
    for (ssize_t i = 0; i < size; i++) {
        sorted[i] = original[i] = rand();
    }

    typ * work = new typ[size];

    testFunction("StdSort", original, work, (typ*) nullptr, size, [&]() {
        std::sort(sorted, sorted + size); });

    testFunction("BinaryHeapSortAheadSimpleVariantA",
            original, work, sorted, size, [&]() {
                BinaryHeapSortAheadSimpleVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("BinaryHeapSortAheadSimpleVariantB",
            original, work, sorted, size, [&]() {
                BinaryHeapSortAheadSimpleVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("BinaryHeapSortCascadingVariantA",
            original, work, sorted, size, [&]() {
                BinaryHeapSortCascadingVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("BinaryHeapSortCascadingVariantB",
            original, work, sorted, size, [&]() {
                BinaryHeapSortCascadingVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("BinaryHeapSortCascadingVariantC",
            original, work, sorted, size, [&]() {
                BinaryHeapSortCascadingVariantC<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("BinaryHeapSortCascadingVariantD",
            original, work, sorted, size, [&]() {
                BinaryHeapSortCascadingVariantD<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("ClusteredBinaryHeapSortVariantA",
            original, work, sorted, size, [&]() {
                ClusteredBinaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("ClusteredBinaryHeapSortVariantB",
            original, work, sorted, size, [&]() {
                ClusteredBinaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedBinaryHeapSortVariantA",
            original, work, sorted, size, [&]() {
                OneBasedBinaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedBinaryHeapSortVariantB",
            original, work, sorted, size, [&]() {
                OneBasedBinaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("HybridHeapSort",
            original, work, sorted, size, [&]() {
                HybridHeapSort<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("QuaternaryHeapSortVariantA",
            original, work, sorted, size, [&]() {
                QuaternaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("QuaternaryHeapSortVariantB",
            original, work, sorted, size, [&]() {
                QuaternaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("ClusteredTernaryHeapSortVariantA",
            original, work, sorted, size, [&]() {
                ClusteredTernaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("ClusteredTernaryHeapSortVariantB",
            original, work, sorted, size, [&]() {
                ClusteredTernaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedTernaryHeapSortVariantA",
            original, work, sorted, size, [&]() {
                OneBasedTernaryHeapSortVariantA<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("OneBasedTernaryHeapSortVariantB",
            original, work, sorted, size, [&]() {
                OneBasedTernaryHeapSortVariantB<typ, ComparisonOperator>(
                        work, size);
            });

    testFunction("RandomizedQuickSort",
            original, work, sorted, size, [&]() {
                RandomizedQuickSort<typ, ComparisonOperator>(
                        work, size);
            });

    std::cout << "Great success!" << std::endl;

    return EXIT_SUCCESS;
}
