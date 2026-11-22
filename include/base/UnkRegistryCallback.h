#pragma once

#include "types.h"

// Copied member by member (the original reloads the source after each store).
struct UnkRegistryCallback {
    void* context;
    void (*function)();

    UnkRegistryCallback(void* context, void (*function)()) : context(context), function(function) {}
    UnkRegistryCallback(const UnkRegistryCallback& other) : context(other.context), function(other.function) {}
};
