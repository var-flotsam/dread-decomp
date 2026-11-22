// From unit 1390 of tools/tu_map.py (0x71008efba0-0x71008f2680, COmniLightComponent by its
// reflection): the light component's helper that creates its 0x220-byte scene object (constructor
// fn_710014419c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8; the layout of base/UnkHashMap.h). Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "game/UnkLightBase.h"
#include "game/UnkLightNode.h"
#include "game/UnkLightObject.h"
#include "game/UnkLightResRef.h"
#include "game/UnkTU290.h"
#include "types.h"

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
