#pragma once
#include <unordered_map>
#include <string>


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
    using PtrType = std::shared_ptr<T>;

    ResourceManager() = default;


    /** Get or create a resource with the given name. */
    PtrType getResource(Args&&... args) {
        auto hash = resourceHash(args...);
        if (!resources.count(hash))
            resources[hash] = std::make_shared<T>(args...);
        return resources[hash];
    }


    /** Remove all unused resources from the resources list. */
    void cleanup() {
        for (std::pair<std::size_t, PtrType>& resource : resources) {
            if (resource.second.use_count() <= 1) {
                resources.erase(resource.first);
            }
        }
    }

private:

    /** A list of all resources currently loaded. */
    std::unordered_map<std::size_t, PtrType> resources;
};


