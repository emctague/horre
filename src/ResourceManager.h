#pragma once
#include <unordered_map>
#include <string>
#include <memory>


/* Note: resourceHash code is derived from: https://gist.github.com/gyakoo/1f77661d670876e6d0c1 */

/** The base case of a function that generates a hash for a set of resources. */
template<typename T>
inline std::size_t resourceHash(const T& arg) {
    return std::hash<T>()(arg);
}

/** Generates a hash given a set of resources, via recursion. */
template<typename T, typename ...Args>
inline std::size_t resourceHash(const T& first, Args ...args) {
    return std::hash<T>()(first) & (resourceHash(args...) << 1);
}


/** A ResourceManager keeps track of resources of a given type. */
template<class T, class... Args>
class ResourceManager {
public:
    ResourceManager() = default;

    /** Get or create a resource with the given name. */
    std::shared_ptr<T> getResource(Args&&... args) {
        auto hash = resourceHash(args...);
        if (!resources.count(hash))
            resources[hash] = std::make_shared<T>(args...);
        return resources[hash];
    }


    /** Remove all unused resources from the resources list. */
    void cleanup() {
        auto iterator = resources.begin();
        while (iterator != resources.end()) {
            if (iterator->second.use_count() <= 1) iterator = resources.erase(iterator);
            else iterator++;
        }
    }

    /** Get the number of loaded resources. */
    int loadedCount() {
        return resources.size();
    }

private:

    /** A list of all resources currently loaded. */
    std::unordered_map<std::size_t, std::shared_ptr<T>> resources;
};


