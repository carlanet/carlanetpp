/*
 * utils.h
 *
 *  Created on: Jul 27, 2022
 *      Author: christian
 */

#ifndef CARLANET_UTILS_H_
#define CARLANET_UTILS_H_

#include <vector>
#include "omnetpp.h"

using namespace omnetpp;

/**
 * @brief Return a vector containing pointers to all submodules of parentModule of type T (from veins)
 */
template <class T>
std::vector<T*> getSubmodulesOfType(cModule* parentModule, bool recurse = false)
{
    std::vector<T*> result;
    for (cModule::SubmoduleIterator iter(parentModule); !iter.end(); iter++) {
        auto mm = dynamic_cast<T*>(*iter);
        if (mm != nullptr) result.push_back(mm);
        if (recurse) {
            for (auto m : getSubmodulesOfType<T>(*iter, recurse)) {
                result.emplace_back(m);
            }
        }
    }
    return result;
}

/**
 * Return the first module found of a specific type
 */
template <class T>
T* getFirstSubmoduleOfType(cModule* parentModule, bool recurse = false){
    return getSubmodulesOfType<T>(parentModule, recurse).front();
}



#endif /* CARLA_OMNET_UTILS_H_ */
