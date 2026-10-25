// Sets up a state machine: creates its scene object, then creates its 18 states, each named by
// a key from the id table, and adds them to the machine's keyed list. Names and layouts are
// placeholders.
#include "UnkHandleS7.h"
#include "UnkPropS7.h"
#include "base/global/CStrId.h"
#include "game/UnkRenderManager.h"
#include "game/UnkXformNode.h"

#include <new>

using namespace unk_handle;
using namespace unk_prop;

struct UnkOwner_71010e2278;
struct UnkArg1_71010e2278;
struct UnkArg2_71010e2278;

struct UnkState_71010e2278 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkRefPtr* GetHandleSlot();
    u8 unk_0x8[0x18];
    CStrId name;

    void Init(UnkOwner_71010e2278* owner, const CStrId* key, UnkArg1_71010e2278* a1,
              UnkArg2_71010e2278* a2, f32 f0, f32 f1, f32 f2);  // fn_71010ed5d4
};

// The states, one type per constructor; the sizes are what the setup allocates.
#define STATE(ctor, size, extra)                                    \
    struct UnkState_##ctor : UnkState_71010e2278 {                   \
        u8 unk_0x28[size - 0x28];                                    \
        UnkState_##ctor(); /* fn_##ctor */                           \
        extra                                                        \
    };
STATE(7100934724, 0x3c8, void SetField26c(f32 v) { *reinterpret_cast<f32*>(&unk_0x28[0x244]) = v; })
STATE(71010f06bc, 0x2a0, )
STATE(71010ef0ec, 0xc18, )
STATE(71009ca94c, 0xbe8, )
STATE(71010e81e8, 0x660, )
STATE(7100a20b10, 0x330, )
STATE(71009278cc, 0x338, )
STATE(71007d324c, 0x200, )
STATE(710074ef00, 0x358, )
STATE(71007af56c, 0x1d8, )
STATE(7100722124, 0x248, UnkRefPtr& Field228() { return *reinterpret_cast<UnkRefPtr*>(&unk_0x28[0x200]); })
STATE(71009d63bc, 0x2f8, )
STATE(71007e5dc0, 0x1d8, )
STATE(71007977dc, 0x1e0, void SetField1d8(u8 v) { unk_0x28[0x1b0] = v; })
STATE(710086e7c8, 0x208, )
STATE(7100797cac, 0x300, )
#undef STATE

// The machine's states keyed by name (the layout of base::UnkCloneKeyedArray).
// The values are union members (no type-based alias info): the original reloads the count after
// storing one.
struct UnkStateSlot_71010e2278 {
    union {
        UnkState_71010e2278* ptr;
    };
};

struct UnkStateList_71010e2278 {
    UnkStateSlot_71010e2278* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 grow;
};

void fn_71010e7a04(UnkStateList_71010e2278* list, u32 n);  // reserves

struct UnkSceneBase_71010e2278 {
    virtual void vf_0x0();
};
struct UnkSceneNode_71010e2278 {
    virtual void vf_0x0();
};
struct UnkSceneXform_71010e2278 {
    u8 unk_0x0[0x10];
    // fn_7100205f98. Declared with the floats first: with the pointer first, the argument
    // copies come out in the other order. The real parameter order is unknown.
    void Set(f32 x, f32 y, f32 z, UnkArg1_71010e2278* a1);
};

// The scene object the machine drives.
struct UnkSceneObj_71010e2278 : UnkSceneBase_71010e2278, UnkSceneNode_71010e2278 {
    u8 unk_0x10[0x10];
    UnkSceneXform_71010e2278 xform;
};

extern char g_unk_7172a045f0[] __attribute__((visibility("hidden")));

void fn_7100e13928(void* unk, void* unk2, UnkSceneObj_71010e2278** out, const CStrId* name);
void fn_7100219ae0(UnkRenderManager* mgr, UnkSceneNode_71010e2278* node, u32 layer);
void fn_7100219bb4(UnkRenderManager* mgr, UnkSceneObj_71010e2278* obj, u32 layer);
void fn_71001453c4(UnkSceneObj_71010e2278* obj, UnkArg2_71010e2278* a2);
void fn_7100030fc0(void* dst, const void* src, f32 value);

struct UnkOwner_71010e2278 {
    u8 unk_0x0[0x40];
    UnkSceneObj_71010e2278* scene;
    f32 field_0x48;
    u8 unk_0x4c[0x34];
    UnkStateList_71010e2278 states;
    u8 unk_0xa0[0x38];
    UnkVec3f field_0xd8;
    f32 field_0xe4;
    f32 field_0xe8;
    u8 unk_0xec[0x4];
    u8 field_0xf0[0xb8];
    u8 field_0x1a8[0x8];

    template <class T>
    __attribute__((always_inline)) T* Create(u32 key, UnkArg1_71010e2278* a1,
                                             UnkArg2_71010e2278* a2) {
        T* s = new (fn_71000839a0(sizeof(T), 5)) T;
        if (s)
            s->vf_0x18();
        s->Init(this, &Key(key), a1, a2, field_0xe4, field_0xe8, field_0x48);
        return s;
    }

    __attribute__((always_inline)) void Add(UnkState_71010e2278* s) {
        if (states.count == states.capacity)
            fn_71010e7a04(&states, states.count + states.grow);
        // The name is copied as raw bytes, then retained: the original reloads the count after
        // the copy on both paths.
        CStrId* k = &states.keys[states.count];
        __builtin_memcpy(k, &s->name, sizeof(*k));
        if (&g_unk_7101d47108->empty != k->data)
            ++k->data->ref_count;
        states.values[states.count].ptr = s;
        ++states.count;
    }
};

void fn_71010e2278(UnkOwner_71010e2278* self, UnkArg1_71010e2278* a1, UnkArg2_71010e2278* a2,
                   f32 x, f32 y, f32 z) {
    UnkRenderManager* mgr = g_unk_7101d47300;
    UnkSceneObj_71010e2278* scene = nullptr;
    fn_7100e13928(mgr->field_0x218, nullptr, &scene, &g_unk_7101d470e0);
    fn_7100219ae0(mgr, scene, 0);
    self->scene = scene;
    scene->xform.Set(x, y, z, a1);
    fn_71001453c4(self->scene, a2);
    self->field_0xe4 = x;
    self->field_0xe8 = 0.0f;
    fn_7100030fc0(self->field_0xf0, g_unk_7172a045f0, y);
    fn_7100030fc0(self->field_0x1a8, g_unk_7172a045f0 + 8, z);
    self->field_0xd8.x = x;
    self->field_0xd8.y = y;
    self->field_0xd8.z = z;
    fn_7100219bb4(g_unk_7101d47300, self->scene, 0);

    auto* first = self->Create<UnkState_7100934724>(0x17e0, a1, a2);
    first->SetField26c(85.0f);
    self->Add(first);
    self->Add(self->Create<UnkState_71010f06bc>(0x1cf0, a1, a2));
    self->Add(self->Create<UnkState_71010ef0ec>(0x1080, a1, a2));
    self->Add(self->Create<UnkState_71009ca94c>(0x1c08, a1, a2));
    self->Add(self->Create<UnkState_71010e81e8>(0x7c8, a1, a2));
    self->Add(self->Create<UnkState_7100a20b10>(0x22a8, a1, a2));
    self->Add(self->Create<UnkState_71009278cc>(0x1780, a1, a2));
    self->Add(self->Create<UnkState_71007d324c>(0xee8, a1, a2));
    self->Add(self->Create<UnkState_710074ef00>(0x998, a1, a2));
    self->Add(self->Create<UnkState_71007af56c>(0xd78, a1, a2));
    {
        auto* s = self->Create<UnkState_7100722124>(0x868, a1, a2);
        // Shares the first state's handle, creating it if needed.
        SetRef(s->Field228(), Handle(first));
        self->Add(s);
    }
    self->Add(self->Create<UnkState_71009d63bc>(0x1d30, a1, a2));
    self->Add(self->Create<UnkState_71007e5dc0>(0x17e8, a1, a2));
    {
        auto* s = self->Create<UnkState_71007977dc>(0x17f0, a1, a2);
        s->SetField1d8(3);
        self->Add(s);
    }
    {
        auto* s = self->Create<UnkState_71007977dc>(0x17f8, a1, a2);
        s->SetField1d8(1);
        self->Add(s);
    }
    {
        auto* s = self->Create<UnkState_71007977dc>(0x1800, a1, a2);
        s->SetField1d8(2);
        self->Add(s);
    }
    self->Add(self->Create<UnkState_710086e7c8>(0x1380, a1, a2));
    self->Add(self->Create<UnkState_7100797cac>(0xc10, a1, a2));
}
