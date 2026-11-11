// CDoorShieldLifeComponent unit (tools/tu_map.py): the constructor without the post-construction call
// (fn_71008de2fc; the same layout as CDoorShieldLifeComponent_Ctor_A5_b71.cpp, its three arrays built as one
// array of three) and the destructor (fn_71008de3e0: clears what the first array's entries point to, empties
// and frees the arrays, destroys the two strings, then the base destructor); fn_71008db6c4 opens or closes the
// shield (animation g_unk_71727317a0 or g_unk_71727317a8, forwards when the state object at +0x430 is in
// state 1, the matching flag on the actor's +0x228 set or cleared, and the state bytes at +0xae8..+0xaed);
// fn_71008de71c fades the shield out over `time` (each mesh slot's colour to alpha 0, or the actor's fader at
// +0x280) and schedules its deletion 0.1 s later; fn_71008de8f4 handles a damage request through virtual
// slots 0x1d8 (accept), 0x1e0 (apply), 0x1e8 (reject) and 0x1c0 (notify), killing the shield (fn_71008de9d4)
// when its life runs out. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/math/CVector4D.h"
#include "game/CActorComponent.h"
#include "game/CDoorShieldLifeComponent.h"
#include "game/CItemLifeComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU2523.h"
#include "types.h"

using base::global::CRntString;
using base::math::CVector4D;

// As src/game/CDoorShieldLifeComponent/CDoorShieldLifeComponent_Ctor_A5_b71.cpp declares them.
struct UnkLifeComponent_A5_b71;
void fn_710007e6c4(void*);
// As src/game/CHyperBeamBlockLifeComponent/CHyperBeamBlockLifeComponent_Dtor_b50.cpp declares it.

struct UnkDoorShieldLifeObj_ovb0001 {
    void** vtable;
    u8 pad_0x8[0x388 - 0x8];
    CRntString name;      // 0x388
    u8 field_0x3b8[0x30];
    f32 field_0x3e8;
    u8 pad_0x3ec[0x4];
    u64 field_0x3f0;
    s64 field_0x3f8;
    u32 field_0x400;
    u8 pad_0x404[0x4];
    base::UnkVector<void**> lists[3];  // 0x408
    bool field_0x480;
    u8 pad_0x481[0x7];
    u64 field_0x488;
    bool field_0x490;
};
static_assert(__builtin_offsetof(UnkDoorShieldLifeObj_ovb0001, field_0x490) == 0x490, "layout");

void fn_71008de2fc(UnkDoorShieldLifeObj_ovb0001* self) {
    fn_71008e04e8(reinterpret_cast<UnkLifeComponent_A5_b71*>(self));
    self->vtable = &g_unk_7101a7ebf8[2];
    new (&self->name) CRntString("");
    fn_710007e6c4(self->field_0x3b8);
    self->field_0x3e8 = 0.15f;
    self->field_0x3f0 = 0;
    self->field_0x3f8 = -1;
    self->field_0x400 = 0;
    for (base::UnkVector<void**>& list : self->lists)
        new (&list) base::UnkVector<void**>(0xf);
    self->field_0x480 = false;
    self->field_0x488 = 0;
    self->field_0x490 = true;
}

void fn_71008de3e0(UnkDoorShieldLifeObj_ovb0001* self) {
    self->vtable = &g_unk_7101a7ebf8[2];
    base::UnkVector<void**>& links = self->lists[0];
    for (void*** it = links.data, ***end = links.data + links.count; it != end; ++it)
        **it = nullptr;
    for (base::UnkVector<void**>& list : self->lists)
        list.Clear();
    self->lists[2].Free();
    self->lists[1].Free();
    self->lists[0].Free();
    fn_710007ecd4(reinterpret_cast<CRntString*>(self->field_0x3b8));
    fn_710007ecd4(&self->name);
    fn_71008e05e8(self);
}

// As src/game/CGooplotAIComponent/CGooplotAIComponent_Reset_b49.cpp declares them.
struct UnkFlagTarget;

struct UnkDoorShieldLife6_ovb0001;
void fn_71008d0a08(UnkDoorShieldLife6_ovb0001* self, const void* animation, bool reverse, bool loop);

struct UnkDoorShieldFlags_ovb0001 {
    u8 pad_0x0[0x30];
    u64 name;  // 0x30
};

extern UnkDoorShieldFlags_ovb0001* g_unk_7172731790 __attribute__((visibility("hidden")));
extern UnkDoorShieldFlags_ovb0001* g_unk_7172731798 __attribute__((visibility("hidden")));
extern const u64 g_unk_71727317a0 __attribute__((visibility("hidden")));
extern const u64 g_unk_71727317a8 __attribute__((visibility("hidden")));

struct UnkDoorShieldActor6_ovb0001 {
    u8 pad_0x0[0x228];
    UnkFlagTarget* flags;  // 0x228
};

struct UnkDoorShieldState_ovb0001 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkDoorShieldLife6_ovb0001 {
    u8 pad_0x0[0x30];
    UnkDoorShieldActor6_ovb0001* actor;  // 0x30
    u8 pad_0x38[0x3f8];
    UnkDoorShieldState_ovb0001* field_0x430;
    u8 pad_0x438[0xae8 - 0x438];
    bool field_0xae8;
    u8 pad_0xae9;
    bool field_0xaea;
    bool field_0xaeb;
    bool field_0xaec;
    bool field_0xaed;
};

static inline void SetFlag(UnkDoorShieldLife6_ovb0001* self, UnkDoorShieldFlags_ovb0001* flags, bool on) {
    if (UnkFlagTarget* target = self->actor->flags) {
        u64 name = flags->name;
        if (on)
            fn_7101172764(target, reinterpret_cast<const char*>(&name));
        else
            fn_7101172780(target, reinterpret_cast<const char*>(&name));
    }
}

void fn_71008db6c4(UnkDoorShieldLife6_ovb0001* self, bool open) {
    bool ready = self->field_0x430 && self->field_0x430->state == 1;
    if (open) {
        if (ready) {
            fn_71008d0a08(self, &g_unk_71727317a0, false, false);
            SetFlag(self, g_unk_7172731790, true);
            self->field_0xaed = true;
        } else {
            SetFlag(self, g_unk_7172731790, false);
            fn_71008d0a08(self, &g_unk_71727317a0, true, false);
            self->field_0xaed = false;
        }
        self->field_0xaea = true;
    } else {
        if (ready) {
            fn_71008d0a08(self, &g_unk_71727317a8, false, false);
            SetFlag(self, g_unk_7172731798, true);
            self->field_0xaec = true;
        } else {
            SetFlag(self, g_unk_7172731798, false);
            fn_71008d0a08(self, &g_unk_71727317a8, true, false);
            self->field_0xaec = false;
        }
        self->field_0xaeb = true;
    }
    self->field_0xae8 = true;
}

// As src/game/CNailongAIComponent/CNailongAIComponent_b46.cpp declares it.
bool fn_71001c287c(void* target, u32 a, u32 b, CVector4D* colour, u32 c);
// As src/game/CCharClassDeleteEntityEvent/CCharClassDeleteEntityEvent_Run_b49.cpp declares it.
void fn_71001c2ec0(void* target, u32 a, u32 b, CVector4D* colour, u32 c, f32 time);
void fn_7101155124(void* fade, f32 time);

struct UnkDoorShieldMeshInfo_ovb0001 {
    u8 pad_0x0[0x2c];
    s32 count;  // 0x2c
};

struct UnkDoorShieldMesh_ovb0001 {
    u8 pad_0x0[0x78];
    UnkDoorShieldMeshInfo_ovb0001* info;  // 0x78
};

struct UnkDoorShieldSlots_ovb0001 {
    u8 pad_0x0[0x138];
    UnkDoorShieldMesh_ovb0001* mesh;  // 0x138
};

struct UnkDoorShieldModel_ovb0001 {
    u8 pad_0x0[0x50];
    UnkDoorShieldSlots_ovb0001* slots;  // 0x50
};

struct UnkDoorShieldActor_ovb0001 {
    u8 pad_0x0[0x228];
    UnkDoorShieldModel_ovb0001* model;  // 0x228
    u8 pad_0x230[0x50];
    void* fade;  // 0x280
};

struct UnkDoorShieldData_ovb0001 {
    u8 pad_0x0[0x11c];
    bool fadeMeshes;  // 0x11c
};

struct UnkDoorShieldLife5_ovb0001 {
    u8 pad_0x0[0x30];
    UnkDoorShieldActor_ovb0001* actor;  // 0x30
    u8 pad_0x38[0x450];
    UnkDoorShieldData_ovb0001* data;  // 0x488
};

void fn_71008de71c(UnkDoorShieldLife5_ovb0001* self, f32 time) {
    if (self->data->fadeMeshes) {
        UnkDoorShieldModel_ovb0001* model = self->actor->model;
        if (model && model->slots) {
            UnkDoorShieldSlots_ovb0001* slots = model->slots;
            s32 count = slots->mesh->info->count;
            for (s32 i = 0; i < count; i++) {
                CVector4D colour;
                colour.x = 1.0f;
                colour.y = 1.0f;
                colour.z = 1.0f;
                colour.w = 1.0f;
                fn_71001c287c(slots->mesh, i, 0, &colour, 1);
                colour.w = 0.0f;
                fn_71001c2ec0(slots->mesh, i, 0, &colour, 1, time);
            }
        }
    } else if (void* fade = self->actor->fade) {
        fn_7101155124(fade, time);
    }
    fn_7100d7b054(self->actor, time + 0.1f);
}

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
struct UnkDoorShieldLife_b48;
void fn_71008de9d4(UnkDoorShieldLife_b48* self);

struct UnkDoorShieldRequest_ovb0001 {
    u8 pad_0x0[0xb4];
    u8 flags;  // 0xb4
};

#define UNK_VF8(n) virtual void vf_##n##_0(); virtual void vf_##n##_1(); virtual void vf_##n##_2();     virtual void vf_##n##_3(); virtual void vf_##n##_4(); virtual void vf_##n##_5();     virtual void vf_##n##_6(); virtual void vf_##n##_7();

class UnkDoorShieldLife3_ovb0001 {
public:
    // Slots 0x0-0x1b8 (56 entries).
    UNK_VF8(0) UNK_VF8(1) UNK_VF8(2) UNK_VF8(3) UNK_VF8(4) UNK_VF8(5) UNK_VF8(6)
    virtual void Notify(u32 kind, UnkDoorShieldRequest_ovb0001* request);  // 0x1c0
    virtual void vf_0x1c8();
    virtual void vf_0x1d0();
    virtual bool CanApply(UnkDoorShieldRequest_ovb0001* request);  // 0x1d8
    virtual s32 Apply(UnkDoorShieldRequest_ovb0001* request);  // 0x1e0
    virtual void Reject(UnkDoorShieldRequest_ovb0001* request);  // 0x1e8

    u8 pad_0x8[0x4c];
    f32 life;  // 0x54
    u8 pad_0x58[0xc];
    bool dead;  // 0x64
    u8 pad_0x65[0xb];
    s32 state;  // 0x70
};

s32 fn_71008de8f4(UnkDoorShieldLife3_ovb0001* self, UnkDoorShieldRequest_ovb0001* request) {
    if (self->dead)
        return 0;
    if (request->flags & 0x20)
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), self->life);
    if (!fn_710024fbc8(self))
        return 0;
    if (self->CanApply(request)) {
        s32 result = self->Apply(request);
        if (self->state == 2 || self->life <= 0.0f)
            fn_71008de9d4(reinterpret_cast<UnkDoorShieldLife_b48*>(self));
        if (result == 1)
            self->Notify(0, request);
        return result;
    }
    self->Reject(request);
    return 0;
}
