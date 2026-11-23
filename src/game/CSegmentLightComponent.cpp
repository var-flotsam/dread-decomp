// From unit 1391 of tools/tu_map.py (0x71008f2680-0x71008f52c0, CSegmentLightComponent by its
// reflection): the light component's helper that creates its 0x140-byte scene object (constructor
// fn_7100142b8c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8). The same as COmniLightComponent's fn_71008f2444
// (src/game/COmniLightComponent.cpp) over a smaller object. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "game/UnkSegmentLightBase.h"
#include "game/UnkSegmentLightNode.h"
#include "game/UnkSegmentLightObject.h"
#include "game/UnkSegmentLightResRef.h"
#include "game/UnkTU290.h"
#include "types.h"

// Passes the part's object at +0x108 on (fn_71001bd9f0). Declared by src/game/COmniLightComponent.cpp
// over its own node type.
struct UnkLightNode;

static_assert(sizeof(UnkSegmentLightObject) == 0x140, "UnkSegmentLightObject layout");

// Its constructor (the second argument is the object's name).
void fn_7100142b8c(UnkSegmentLightObject* self, const void* arg);

// The scene objects by their base.
using UnkSegmentLightMap = base::global::CRntDictionary<UnkSegmentLightBase*, UnkSegmentLightObject*>;

// Creates a scene object, hands it the resource (also at +0x98 and through its node's vf_0x90
// when there is one) and enters it in the map. Stores the object in `out` when given and returns
// its base.
UnkSegmentLightBase* fn_71008f5084(UnkSegmentLightMap* map, UnkCountedResource* resource,
                                   UnkSegmentLightObject** out, const void* arg) {
    auto* obj = static_cast<UnkSegmentLightObject*>(fn_71000839a0(sizeof(UnkSegmentLightObject), 0x38));
    fn_7100142b8c(obj, arg);
    UnkSegmentLightBase* base = nullptr;
    if (obj) {
        UnkSegmentLightNode* node = &obj->node;
        fn_710020c634(reinterpret_cast<UnkLightNode*>(node));
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
