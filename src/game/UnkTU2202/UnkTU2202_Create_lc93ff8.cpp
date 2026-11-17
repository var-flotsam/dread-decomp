// tools/tu_map.py unit 2202: creates a 0x140-byte scene object (constructor fn_7100147eec), prepares
// its holder, hands it the resource (also to its node, which is then updated) and enters it in the
// pool's chained hash map. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntDictionary.h"
#include "types.h"

// The scene object's part at +0x20 with a virtual update.
struct UnkSceneNode_lc93ff8 {
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

// A counted resource reference (base/UnkCountedResource.h).
struct UnkResRef_lc93ff8 {
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

struct UnkHolder_b0202;
void fn_71001c3d80(UnkHolder_b0202* self);

// The 0x140-byte scene object.
struct UnkSceneObject_lc93ff8 {
    void* vtable;
    UnkResRef_lc93ff8 resource;  // 0x8
    u8 pad_0x10[0x10];
    UnkSceneNode_lc93ff8 node;  // 0x20
    u8 pad_0x28[0x70];
    UnkResRef_lc93ff8 resource2;  // 0x98
    u8 pad_0xa0[0x88];
    UnkHolder_b0202* holder;  // 0x128
    u8 pad_0x130[0x10];
};
static_assert(sizeof(UnkSceneObject_lc93ff8) == 0x140, "UnkSceneObject_lc93ff8 layout");

// Its constructor (the second argument is passed through).
void fn_7100147eec(UnkSceneObject_lc93ff8* self, const void* arg);

// The scene objects by themselves.
using UnkSceneMap_lc93ff8 = base::global::CRntDictionary<UnkSceneObject_lc93ff8*, UnkSceneObject_lc93ff8*>;

UnkSceneObject_lc93ff8* fn_7100c93ff8(UnkSceneMap_lc93ff8* map, UnkCountedResource* resource,
                                      UnkSceneObject_lc93ff8** out, const void* arg) {
    auto* obj = static_cast<UnkSceneObject_lc93ff8*>(fn_71000839a0(sizeof(UnkSceneObject_lc93ff8), 0x38));
    fn_7100147eec(obj, arg);
    UnkSceneObject_lc93ff8* result = nullptr;
    if (obj) {
        fn_71001c3d80(obj->holder);
        obj->resource.Set(resource);
        if (resource) {
            UnkSceneNode_lc93ff8* node = &obj->node;
            obj->resource2.Set(resource);
            node->vf_0x90();
        }
        result = obj;
        map->Insert(result, obj);
    }
    if (out)
        *out = obj;
    return result;
}
