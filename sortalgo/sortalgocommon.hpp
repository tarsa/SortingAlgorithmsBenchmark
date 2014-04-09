/* 
 * sortalgocommon.hpp -- sorting algorithms benchmark
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

#ifndef SORTALGOCOMMON_HPP
#define	SORTALGOCOMMON_HPP

namespace tarsa {

    enum ComparisonType {
        Below, Equal, Above
    };

    template<typename ItemType>
    using ComparisonOperator = bool (*)(ItemType const, ComparisonType const,
            ItemType const);

    template<typename ItemType>
    bool genericComparisonOperator(ItemType const leftOp,
            ComparisonType const opType, ItemType const rightOp) {
        switch (opType) {
            case Below: return leftOp < rightOp;
            case Equal: return leftOp == rightOp;
            case Above: return leftOp > rightOp;
        }
    }
    
    template<typename ItemType>
    bool genericReverseComparisonOperator(ItemType const leftOp,
            ComparisonType const opType, ItemType const rightOp) {
        return genericComparisonOperator(rightOp, opType, leftOp);
    }
    
    namespace privateClusteredHeapsorts {
        template<ssize_t levels>
        ssize_t constexpr computeClusterSize(ssize_t const arity) {
            return 1 + arity * computeClusterSize<levels - 1>(arity);
        }
        
        template<>
        ssize_t constexpr computeClusterSize<0>(ssize_t const arity) {
            return 0;
        }
        
        template<ssize_t level>
        ssize_t constexpr computeClusterLevelSize(ssize_t const arity) {
            return arity * computeClusterLevelSize<level - 1>(arity);
        }
        
        template<>
        ssize_t constexpr computeClusterLevelSize<0>(ssize_t const arity) {
            return 1;
        }
    }
}

#endif	/* SORTALGOCOMMON_HPP */
