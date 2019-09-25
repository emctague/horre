#include "ResourceSet.h"

int ResourceSet::cleanup() {
    models.cleanup();
    shaders.cleanup();

    return models.loadedCount() + shaders.loadedCount();
}

ResourceSet::~ResourceSet() {
    // Clean up unused resources before destroying
    int remaining = cleanup();

    // Warn if any objects are still in use
    if (remaining) {
        std::cout << "Warning: ResourceSet is being destroyed, but " << remaining << " resources are still in use."
                  << std::endl << "This may mean a resource-using object (e.g. entity) has been leaked." << std::endl;
    }
}


