#include "ResourceSet.h"

int ResourceSet::cleanup() {
    models.cleanup();
    shaders.cleanup();
    textures.cleanup();

    return models.loadedCount() + shaders.loadedCount() + textures.loadedCount();
}

ResourceSet::~ResourceSet() {
    // Clean up unused resources before destroying
    int remaining = cleanup();

    // Warn if any objects are still in use
    if (remaining) {
        std::cout << "Warning: ResourceSet is being destroyed, but " << remaining << " resources are still in use."
                  << std::endl << "This may mean a resource-using object (e.g. entity) has been leaked." << std::endl
                  << "shaders  = " << shaders.loadedCount() << std::endl
                  << "models   = " << models.loadedCount() << std::endl
                  << "textures = " << textures.loadedCount() << std::endl;
    }
}


