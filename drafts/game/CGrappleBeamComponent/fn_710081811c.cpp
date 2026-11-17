// CGrappleBeamComponent unit (tools/tu_map.py, unit 1277): the component's destructor. Drops the scene
// objects in the list at +0x740 and frees it, hands the object at +0x738 back to the render manager,
// releases the three string ids, destroys the four states (last first), the shared value at +0x548 and the
// state machine at +0x480, then runs the base destructor (fn_7100ac72a4). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a75230[];  // the class's vtable
extern void* g_unk_7101a755c8[];  // the states' base vtable
extern void* g_unk_7101a75620[];  // the state machine's vtable
void fn_7100ac72a4(char*);

struct UnkGrappleRefObject_l81811c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkGrappleSceneObject_l81811c {
    void** vtable;
    u64 node;  // what the render manager takes
};

struct UnkGrappleState_l81811c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    u64 block[8];

    void Destroy() {
        for (int i = 7; i >= 0; --i)
            block[i] = 0;
        vtable = g_unk_7101a755c8 + 2;
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkGrappleState_l81811c) == 0x70, "layout");

struct UnkGrappleMachine_l81811c {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;

    void Destroy() {
        vtable = g_unk_7101a75620 + 2;
        field_0x10 = 0;
        field_0x18 = 0;
        count = 0;
    }
};

struct UnkGrappleBeam_l81811c {
    void** vtable;
    u8 pad_0x8[0x480 - 0x8];
    UnkGrappleMachine_l81811c machine;              // 0x480
    u8 pad_0x4a8[0x548 - 0x4a8];
    UnkGrappleRefObject_l81811c* shared;            // 0x548
    UnkGrappleState_l81811c states[4];              // 0x550
    u8 pad_0x710[0x720 - 0x710];
    CStrId id_0x720;                                // 0x720
    CStrId id_0x728;                                // 0x728
    CStrId id_0x730;                                // 0x730
    UnkGrappleSceneObject_l81811c* scene_object;    // 0x738
    UnkGrappleSceneObject_l81811c** objects;        // 0x740
    u32 object_count;                               // 0x748
};
static_assert(__builtin_offsetof(UnkGrappleBeam_l81811c, shared) == 0x548, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l81811c, id_0x720) == 0x720, "layout");
static_assert(__builtin_offsetof(UnkGrappleBeam_l81811c, object_count) == 0x748, "layout");

void fn_710081811c(UnkGrappleBeam_l81811c* self) {
    self->vtable = g_unk_7101a75230 + 2;
    for (u32 i = 0; i < self->object_count; i++) {
        if (self->objects[i]) {
            fn_7100219e10(g_unk_7101d47300, reinterpret_cast<u64>(self->objects[i]));
            self->objects[i] = nullptr;
        }
    }
    delete[] self->objects;
    self->objects = nullptr;
    if (self->scene_object) {
        fn_71002199d8(g_unk_7101d47300,
                      reinterpret_cast<UnkItemBase_71011a91c4*>(&self->scene_object->node), 0);
        self->scene_object = nullptr;
    }
    self->id_0x730.Reset();
    self->id_0x728.Reset();
    self->id_0x720.Reset();
    for (int i = 3; i >= 0; --i)
        self->states[i].Destroy();
    if (UnkGrappleRefObject_l81811c* shared = self->shared) {
        if (--shared->ref_count == 0)
            shared->vf_0x10();
    }
    self->machine.Destroy();
    fn_7100ac72a4(reinterpret_cast<char*>(self));
}
