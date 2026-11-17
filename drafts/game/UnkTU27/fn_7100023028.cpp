// Unit 27: erases an entry from the factory registry's keyed array of 0x48-byte entries (as
// src/base/UnkResFactoryRegistry_Reserve_b49.cpp lays them out): the entry is destroyed, its key released,
// and the last entry and key moved into its place. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTU27.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A pooled id whose moved-from side is left as the pool's empty string.
template <UnkStringPool** Pool>
struct UnkMovedId_l023028 {
    UnkStringData* data;

    UnkMovedId_l023028(UnkMovedId_l023028&& other) : data(other.data) {
        other.data = &(*Pool)->empty;
    }
};

// Copied member by member (the original reloads the source after each store).
struct UnkRegistryCallback_l023028 {
    void* context;
    void (*function)();

    UnkRegistryCallback_l023028(const UnkRegistryCallback_l023028& other)
        : context(other.context), function(other.function) {}
};

// Two file-path ids moved together: both moved-from ids become the pool's empty string.
struct UnkPathIds_l023028 {
    UnkStringData* first;
    UnkStringData* second;

    UnkPathIds_l023028(UnkPathIds_l023028&& other) {
        UnkStringData* empty = &g_unk_7101d47200->empty;
        first = other.first;
        other.first = empty;
        second = other.second;
        other.second = empty;
    }
};

struct UnkRegistryEntryHead_l023028 {
    void* desc;
    void* factory;
};

struct UnkRegistryEntry_l023028 : UnkRegistryEntryHead_l023028 {
    UnkMovedId_l023028<&g_unk_7101d47108> id;
    UnkPathIds_l023028 paths;  // 0x18
    UnkRegistryCallback_l023028 field_0x28;
    UnkRegistryCallback_l023028 field_0x38;

    UnkRegistryEntry_l023028(UnkRegistryEntry_l023028&& other) = default;
};
static_assert(sizeof(UnkRegistryEntry_l023028) == 0x48, "UnkRegistryEntry layout");

struct UnkRegistryArray_l023028 {
    UnkRegistryEntry_l023028* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
};

UnkRegistryEntry_l023028* fn_7100023028(char* array, void* it) {
    UnkRegistryArray_l023028* self = reinterpret_cast<UnkRegistryArray_l023028*>(array);
    u32 index = static_cast<u32>(static_cast<UnkRegistryEntry_l023028*>(it) - self->values);
    fn_7100020164(reinterpret_cast<char*>(&self->values[index]));
    self->keys[index].Reset();
    if (self->count - 1 != index) {
        new (&self->values[index]) UnkRegistryEntry_l023028(static_cast<UnkRegistryEntry_l023028&&>(self->values[self->count - 1]));
        fn_7100020164(reinterpret_cast<char*>(&self->values[self->count - 1]));
        CStrId::MoveConstructReloading(&self->keys[index], self->keys[self->count - 1]);
        self->keys[self->count - 1].Reset();
    }
    --self->count;
    return &self->values[index];
}
