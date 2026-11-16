// From unit 1390 of tools/tu_map.py (0x71008efba0-0x71008f2680, COmniLightComponent by its
// reflection): the light component's helper that creates its 0x220-byte scene object (constructor
// fn_710014419c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8; the layout of base/UnkHashMap.h). Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "game/UnkTU290.h"
#include "types.h"

// The scene object's part at +0x28 (constructed by fn_710020c38c) with a virtual update.
struct UnkLightNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
};

// A counted resource reference (base/UnkCountedResource.h), as src/game/UnkEnvironmentFade.cpp's.
struct UnkLightResRef {
    UnkCountedResource* ptr;

    void Set(UnkCountedResource* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
    }
};

// The object's base at +8 (what the map is keyed by and the caller gets back).
struct UnkLightBase {
    u64 field_0x0;
};

// The 0x220-byte scene object.
struct UnkLightObject {
    void* vtable;
    UnkLightBase base;          // 0x8
    UnkLightResRef resource;    // 0x10
    u8 pad_0x18[0x10];
    UnkLightNode node;          // 0x28
    u8 pad_0x30[0x70];
    UnkLightResRef resource2;   // 0xa0
    u8 pad_0xa8[0x178];
};
static_assert(sizeof(UnkLightObject) == 0x220, "UnkLightObject layout");

// Its constructor (the second argument is passed through unused).
void fn_710014419c(UnkLightObject* self, const void* arg);

// The scene objects by their base.
using UnkLightMap = base::global::CRntDictionary<UnkLightBase*, UnkLightObject*>;

// Creates a scene object, hands it the resource (also at +0xa0 and through its node's vf_0x90
// when there is one) and enters it in the map. Stores the object in `out` when given and returns
// its base.
UnkLightBase* fn_71008f2444(UnkLightMap* map, UnkCountedResource* resource, UnkLightObject** out,
                            const void* arg) {
    auto* obj = static_cast<UnkLightObject*>(fn_71000839a0(sizeof(UnkLightObject), 0x38));
    fn_710014419c(obj, arg);
    UnkLightBase* base = nullptr;
    if (obj) {
        UnkLightNode* node = &obj->node;
        fn_710020c634(node);
        obj->resource.Set(resource);
        base = &obj->base;
        if (resource) {
            obj->resource2.Set(resource);
            node->vf_0x90();
        }
        map->Insert(base, obj);
    }
    if (out)
        *out = obj;
    return base;
}
