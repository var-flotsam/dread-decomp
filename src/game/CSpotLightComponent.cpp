// From unit 1392 of tools/tu_map.py (0x71008f52c0-0x71008fab30, CSpotLightComponent by its
// reflection): the light component's helper that creates its 0x140-byte scene object (constructor
// fn_7100144bac), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8). The same helper as src/game/CDirLightComponent.cpp's fn_71008ebca8
// (the same object layout) and src/game/COmniLightComponent.cpp's fn_71008f2444. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "game/UnkLightNode.h"
#include "game/UnkSpotLightBase.h"
#include "game/UnkSpotLightObject.h"
#include "game/UnkSpotLightResRef.h"
#include "game/UnkTU290.h"
#include "types.h"

static_assert(sizeof(UnkSpotLightObject) == 0x140, "UnkSpotLightObject layout");

// Its constructor (the second argument is passed through).
void fn_7100144bac(UnkSpotLightObject* self, const void* arg);

// The scene objects by their base.
using UnkSpotLightMap = base::global::CRntDictionary<UnkSpotLightBase*, UnkSpotLightObject*>;

// Creates a scene object, hands it the resource (also at +0x98 and through its node's vf_0x90
// when there is one) and enters it in the map. Stores the object in `out` when given and returns
// its base.
UnkSpotLightBase* fn_71008fa8f8(UnkSpotLightMap* map, UnkCountedResource* resource, UnkSpotLightObject** out,
                                const void* arg) {
    auto* obj = static_cast<UnkSpotLightObject*>(fn_71000839a0(sizeof(UnkSpotLightObject), 0x38));
    fn_7100144bac(obj, arg);
    UnkSpotLightBase* base = nullptr;
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
