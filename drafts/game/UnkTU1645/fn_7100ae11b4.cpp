// Unit 1645 (tools/tu_map.py, no reliable name): adds a property named `name` that controls `value`
// to an owner's map of properties by name, unless the name is taken (then it returns null). The new
// property is linked into the global list and registered. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using namespace base::global;

// As src/game/UnkTU1645/UnkTU1645_Reserve_ovb0261.cpp defines them (the map's Reserve takes them).
struct UnkReleasedId_b0261 {
    UnkStringData* data;

    UnkReleasedId_b0261(UnkReleasedId_b0261&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkReleasedId_b0261() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkOwnedObj_b0261 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkOwnedSlot_b0261 {
    union {
        UnkOwnedObj_b0261* ptr;
    };
    UnkOwnedSlot_b0261(UnkOwnedSlot_b0261&& other) {
        UnkOwnedObj_b0261* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkOwnedSlot_b0261() {
        UnkOwnedObj_b0261* old = ptr;
        ptr = nullptr;
        if (old)
            old->vf_0x8();
    }
};

using UnkOwnedMap_b0261 = base::global::CRntSmallDictionary<UnkReleasedId_b0261, UnkOwnedSlot_b0261>;

void fn_7100ae3834(UnkOwnedMap_b0261* self, u32 capacity);

extern void* g_unk_7101a97840[];  // the property's vtable
void fn_7100ae3300(UnkPropertyEx<>* property);

struct UnkPropertyOwner_lae11b4 {
    u8 pad_0x0[0x38];
    UnkOwnedMap_b0261 properties;  // owned properties by name
};

UnkOwnedObj_b0261* fn_7100ae11b4(UnkPropertyOwner_lae11b4* self, const CStrId& name, void* value) {
    UnkOwnedMap_b0261& map = self->properties;
    if (map.FindIf([&](const UnkReleasedId_b0261& id) { return name.data == id.data; }) != map.End())
        return nullptr;

    const char* text = name.Text();
    auto* property = static_cast<UnkPropertyEx<>*>(fn_7100083b60(sizeof(UnkPropertyEx<>)));
    property->next = g_unk_71726db788;
    property->vtable = &g_unk_7101a45fe8[2];
    g_unk_71726db788 = property;
    fn_71002ef5bc(property->field_0x10);
    property->vtable = &g_unk_7101a97840[2];
    new (&property->name) UnkStrIdBase(text, true);
    property->value = value;
    property->field_0x38 = 0;
    if (property->vf_0x10() && !property->vf_0x18())
        fn_7100ae3300(property);

    if (map.count == map.capacity)
        fn_7100ae3834(&map, map.count + map.step);
    new (&map.keys[map.count]) CStrId(name);
    map.values[map.count].ptr = reinterpret_cast<UnkOwnedObj_b0261*>(property);
    u32 n = ++map.count;
    UnkOwnedSlot_b0261* end = &map.values[n];
    return n ? end[-1].ptr : end->ptr;
}
