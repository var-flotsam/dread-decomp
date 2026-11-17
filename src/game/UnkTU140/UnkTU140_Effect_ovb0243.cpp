// Unit 140 of tools/tu_map.py (see game/UnkSceneObject.h): the render manager's pool (game/UnkRenderManager.h)
// creating an effect object (0x210 bytes: a node at +8 that the pool's set keys by, a named
// sub-object at +0x20 that holds the counted resource, and a member at +0x138), as fn_710021d3d4
// (src/base/spatial/UnkModelInstanceSet.cpp) creates a model instance. The result is the object's node
// (null without an object); `out` gets the object. Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSceneObject.h"
#include "game/UnkTU140.h"
#include "game/UnkTU32.h"
#include "types.h"

void fn_71001fae8c(void* sub, const base::global::CStrId* name);

// A counted reference to a resource, set as base::core::CAssetLink's (src/base/core/CAssetLink.cpp).
static inline void SetCountedRef(UnkCountedResource*& ref, UnkCountedResource* res) {
    if (ref != res) {
        if (res)
            ++res->ref_count;
        UnkCountedResource* old = ref;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        ref = res;
    }
}

// The polymorphic sub-object at +0x20: the resource it draws at +0x78; vf_0x90 updates it.
struct UnkEffectSub_9930 {
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

    u8 pad_0x8[0x70];
    UnkCountedResource* resource;  // 0x78
};

struct UnkEffect_9930 {
    void** vtable;
    u32 flags;  // 0x8
    u32 tag;    // 0xc
    UnkCountedResource* resource;  // 0x10
    UnkEffect_9930* self;          // 0x18
    alignas(8) u8 sub[0x108];      // 0x20
    u64 field_0x128;
    u64 field_0x130;
    u8 member[0xd8];  // 0x138

    UnkEffectSub_9930* Sub() { return reinterpret_cast<UnkEffectSub_9930*>(sub); }
    UnkSceneNode* Node() { return reinterpret_cast<UnkSceneNode*>(&flags); }
};
static_assert(sizeof(UnkEffect_9930) == 0x210, "UnkEffect_9930 layout");

// The pool as a chained hash set of the objects (as in UnkModelInstanceSet.cpp).
using UnkEffectSet_9930 = base::global::CRntDictionary<u64, u64>;

void* fn_7100149930(UnkRenderPool* pool, void* a, UnkSceneObject** out, base::global::CStrId* name) {
    UnkEffectSet_9930* set = reinterpret_cast<UnkEffectSet_9930*>(pool);
    UnkCountedResource* resource = static_cast<UnkCountedResource*>(a);
    UnkEffect_9930* effect = static_cast<UnkEffect_9930*>(fn_71000839a0(sizeof(UnkEffect_9930), 0x38));
    effect->flags = 0xff09f000;
    effect->tag = 0x600d600d;
    effect->resource = nullptr;
    effect->vtable = reinterpret_cast<void**>(g_unk_7101a1cbe8 + 0x10);
    fn_71001fae8c(effect->sub, name);
    effect->field_0x130 = 0;
    fn_71000255c4(effect->member);
    effect->self = effect;
    effect->flags = (effect->flags & 0xfffffe) | (0x13u << 24) | 1;
    effect->field_0x130 = effect->field_0x128;
    fn_7100025884(effect->member, effect);
    UnkSceneNode* node = nullptr;
    if (effect) {
        SetCountedRef(effect->resource, resource);
        if (resource) {
            SetCountedRef(effect->Sub()->resource, resource);
            effect->Sub()->vf_0x90();
        }
        node = effect->Node();
        set->InsertChecked((u64)node, (u64)effect);
    }
    if (out)
        *out = reinterpret_cast<UnkSceneObject*>(effect);
    return node;
}
