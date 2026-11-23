// From unit 1388 of tools/tu_map.py (0x71008ea2cc-0x71008ebee0, CDirLightComponent by its
// reflection): the light component's helper that creates its 0x140-byte scene object (constructor
// fn_7100143b3c), gives it the resource, and enters it in its owner's chained hash map (keyed by
// the object's base at +8; the layout of base/UnkHashMap.h). The same helper as
// src/game/COmniLightComponent.cpp's fn_71008f2444 for a smaller object. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "game/UnkDirLightBase.h"
#include "game/UnkDirLightObject.h"
#include "game/UnkDirLightResRef.h"
#include "game/UnkLightNode.h"
#include "game/UnkTU290.h"
#include "types.h"

static_assert(sizeof(UnkDirLightObject) == 0x140, "UnkDirLightObject layout");

// Its constructor (the second argument is passed through).
void fn_7100143b3c(UnkDirLightObject* self, const void* arg);

// The scene objects by their base.
using UnkDirLightMap = base::global::CRntDictionary<UnkDirLightBase*, UnkDirLightObject*>;

// Creates a scene object, hands it the resource (also at +0x98 and through its node's vf_0x90
// when there is one) and enters it in the map. Stores the object in `out` when given and returns
// its base.
UnkDirLightBase* fn_71008ebca8(UnkDirLightMap* map, UnkCountedResource* resource, UnkDirLightObject** out,
                               const void* arg) {
    auto* obj = static_cast<UnkDirLightObject*>(fn_71000839a0(sizeof(UnkDirLightObject), 0x38));
    fn_7100143b3c(obj, arg);
    UnkDirLightBase* base = nullptr;
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
