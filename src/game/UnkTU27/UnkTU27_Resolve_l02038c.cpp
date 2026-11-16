// Unit 27 of tools/tu_map.py (the resource registry): the resource the ids name. The registry's flat
// map at +0x8 (0x48-byte values, string-id keys at +0x10, count at +0x18) is searched by the ids'
// string id, and the found entry's factory (+0x8) makes the resource (vtable slot 1). The flag and
// the last argument are not read. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkStrIdPathPair.h"
#include "game/UnkTU27.h"
#include "types.h"

// As src/base/core/CAsset/UnkAssetResolve_b53.cpp declares it, with fn_710001737c called, not inlined.
struct UnkAssetResRef {
    UnkCountedResource* ptr;

    UnkAssetResRef() : ptr(nullptr) {}
    UnkAssetResRef& operator=(const UnkAssetResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkAssetResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

struct UnkResFactory_l02038c {
    virtual void vf_0x0();
    virtual UnkAssetResRef Make();
};

struct UnkResFactoryEntry_l02038c {
    u8 pad_0x0[0x8];
    UnkResFactory_l02038c* factory;
    u8 pad_0x10[0x38];
};

struct UnkResFactoryMap_l02038c {
    UnkResFactoryEntry_l02038c* values;
    base::global::UnkStringData** keys;
    u32 count;

    UnkResFactoryEntry_l02038c* Find(base::global::UnkStringData* const& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkResRegistry_l02038c {
    u8 pad_0x0[0x8];
    UnkResFactoryMap_l02038c factories;
};

UnkAssetResRef fn_710002038c(UnkRegistry_7100d92158* registry, const UnkStrIdPathPair* ids, bool a, void* b) {
    UnkResFactoryMap_l02038c& factories = reinterpret_cast<UnkResRegistry_l02038c*>(registry)->factories;
    UnkAssetResRef result;
    UnkResFactoryEntry_l02038c* it = factories.Find(ids->id.data);
    if (it != factories.values + factories.count && it->factory)
        result = it->factory->Make();
    return result;
}
