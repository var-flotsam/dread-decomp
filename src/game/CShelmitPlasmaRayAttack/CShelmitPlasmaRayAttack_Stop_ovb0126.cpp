// CShelmitPlasmaRayAttack's unit (tools/tu_map.py): fn_71006b46c8 stops the attack. After CAttack's step
// (fn_71003609b8) the owner (+0x8) is handed to the lazily created manager's objects at +0x3f0 and +0x470
// (fn_71006bd4f0); with the owner's character class (+0x220), the link at +0x38 is unhooked from the target
// of its weak handle (+0x50) and the class gets three calls (fn_7101178e08: 0xb, 8 and 0xc); the ray ends
// (fn_71006b4900) when the flag at +0x72 is set; in state 7 (+0x30), with the owner's navigation object
// (+0x298) and its +0x218 object, the navigation gets a kind-5 parameter block carrying that object's vector
// at +0x70 (game_s2/UnkParams.h) and the object a kind-3 block (vtable g_unk_7101a2e208, fn_71010c7e6c), and
// the object's flag at +0xf9 is cleared through its slot 0x260. The state ends at 0x7fffffff. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/UnkManager_71726af468.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_71006bd4f0(void* self, UnkActor* actor);
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);
struct UnkEffectSink;
void fn_71010c7e6c(UnkEffectSink* sink, void* params, u32 flags);
extern void* g_unk_7101a2e208[];

struct CShelmitPlasmaRayAttack;
void fn_71006b4900(CShelmitPlasmaRayAttack* self);

struct UnkPlasmaRayKind3Params_ovb0126 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkPlasmaRaySink_ovb0126 {
    void** vtable;
    u8 pad_0x8[0x68];
    u32 field_0x70[3];
    u8 pad_0x7c[0xf9 - 0x7c];
    u8 field_0xf9;

    void vf_0x260() { reinterpret_cast<void (*)(UnkPlasmaRaySink_ovb0126*)>(vtable[0x260 / 8])(this); }
};

struct UnkPlasmaRayStopOwner_ovb0126 {
    u8 pad_0x0[0x218];
    UnkPlasmaRaySink_ovb0126* field_0x218;
    UnkActorTarget* field_0x220;
    u8 pad_0x228[0x298 - 0x228];
    void* navigation;  // 0x298
};

struct UnkPlasmaRayStop_ovb0126 {
    u8 pad_0x0[0x8];
    UnkPlasmaRayStopOwner_ovb0126* owner;
    u8 pad_0x10[0x20];
    u32 state;  // 0x30
    u8 pad_0x34[4];
    u8 link[0x18];  // 0x38
    UnkWeakHandle* handle;  // 0x50
    u8 pad_0x58[0x1a];
    bool field_0x72;
};

static inline UnkManager_71726af468* Manager() {
    UnkManager_71726af468* m = g_unk_71726af468;
    if (!m) {
        m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return m;
}

void fn_71006b46c8(UnkPlasmaRayStop_ovb0126* self) {
    fn_71003609b8(reinterpret_cast<UnkAttackBase*>(self));
    fn_71006bd4f0(reinterpret_cast<u8*>(Manager()) + 0x3f0, reinterpret_cast<UnkActor*>(self->owner));
    fn_71006bd4f0(reinterpret_cast<u8*>(Manager()) + 0x470, reinterpret_cast<UnkActor*>(self->owner));
    if (self->owner) {
        if (UnkActorTarget* target = self->owner->field_0x220) {
            if (UnkWeakHandle* handle = self->handle) {
                if (UnkWeakTarget* t = handle->target)
                    fn_710117b3f0(t, self->link);
                if (UnkWeakHandle* p = self->handle) {
                    if (--p->ref_count == 0)
                        p->vf_0x10();
                    self->handle = nullptr;
                }
            }
            fn_7101178e08(target, 0xb, 1, 0, -1.0f);
            fn_7101178e08(target, 8, 1, 0, -1.0f);
            fn_7101178e08(target, 0xc, 1, 0, -1.0f);
        }
    }
    if (self->field_0x72)
        fn_71006b4900(reinterpret_cast<CShelmitPlasmaRayAttack*>(self));
    if (self->state == 7) {
        UnkPlasmaRayStopOwner_ovb0126* owner = self->owner;
        if (owner && owner->navigation) {
            if (UnkPlasmaRaySink_ovb0126* sink = owner->field_0x218) {
                UnkParams params;
                __builtin_memset(params.field_0x8, 0, 13);
                params.field_0x30 = 0;
                __builtin_memset(params.field_0x20, 0, 16);
                params.vtable = g_unk_7101a25b10 + 2;
                params.field_0x34 = 5;
                *reinterpret_cast<u16*>(&params.pad_0x3c) = 0x100;
                params.field_0x39 = 0;
                params.field_0x3b = 0;
                params.field_0x18 = 0;
                params.field_0x1c = 1;
                u32* v = reinterpret_cast<u32*>(params.field_0x8);
                v[0] = sink->field_0x70[0];
                v[1] = sink->field_0x70[1];
                v[2] = sink->field_0x70[2];
                params.field_0x8[12] = 1;
                fn_71006f4b0c(owner->navigation, &params, 1);
                UnkPlasmaRayKind3Params_ovb0126 p3;
                p3.field_0xc = 0;
                p3.field_0x14 = 0;
                __builtin_memset(p3.field_0x18, 0, 13);
                __builtin_memset(p3.field_0x28, 0, 13);
                p3.vtable = g_unk_7101a2e208 + 2;
                p3.field_0x3d = 0;
                p3.field_0x38 = 3;
                fn_71010c7e6c(reinterpret_cast<UnkEffectSink*>(sink), &p3, 1);
                if (sink->field_0xf9) {
                    sink->field_0xf9 = 0;
                    sink->vf_0x260();
                }
            }
        }
    }
    self->state = 0x7fffffff;
}

// fn_71006b41b4: sends the owner's navigation (+0x298) on. With a target (fn_7100d7c990) closer than 20
// horizontally and the flag given, it sets the flag at +0x71 and hands the navigation a kind-5 block with the
// vector at +0x70 of the owner's +0x218 object; otherwise it clears the flag and hands it a kind-6 block (as
// src/game/behaviortree/CGoToSideAction/CGoToSideAction_Side_ovb0043.cpp builds one) toward the target's
// side (the key g_unk_710164a6d0 when the target is to the left, g_unk_710164a6dc otherwise), called back
// through fn_71006b6dc8 (src/game/CShelmitPlasmaRayAttack/CShelmitPlasmaRayAttack_Nav_b65a4.cpp).
extern void* g_unk_7101a25fb0[];
struct UnkPlasmaRayKey_ovb0126 {
    u8 bytes[12];
};
extern const UnkPlasmaRayKey_ovb0126 g_unk_710164a6d0 __attribute__((visibility("hidden")));
extern const UnkPlasmaRayKey_ovb0126 g_unk_710164a6dc __attribute__((visibility("hidden")));
struct UnkPlasmaRayAttack_b65a4;
void fn_71006b6dc8(UnkPlasmaRayAttack_b65a4* self);

struct UnkPlasmaRayKind6Params_ovb0126 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    void* object;  // 0x20
    void (*fn)(UnkPlasmaRayAttack_b65a4*);  // 0x28
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 field_0x38;
    u8 field_0x39;
};

struct UnkPlasmaRayPosed_ovb0126 {
    u8 pad_0x0[0x44];
    f32 x;
};

struct UnkPlasmaRayNavOwner_ovb0126 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0x218 - 0x48];
    UnkPlasmaRaySink_ovb0126* field_0x218;
    u8 pad_0x220[0x298 - 0x220];
    void* navigation;  // 0x298
};

struct UnkPlasmaRayNav_ovb0126 {
    u8 pad_0x0[0x8];
    UnkPlasmaRayNavOwner_ovb0126* owner;
    u8 pad_0x10[0x61];
    bool field_0x71;
};

void fn_71006b41b4(UnkPlasmaRayNav_ovb0126* self, bool flag) {
    UnkPlasmaRayNavOwner_ovb0126* owner = self->owner;
    if (!owner)
        return;
    void* navigation = owner->navigation;
    auto* target = reinterpret_cast<UnkPlasmaRayPosed_ovb0126*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(owner)));
    owner = self->owner;
    UnkPlasmaRaySink_ovb0126* sink = owner ? owner->field_0x218 : nullptr;
    if (!navigation || !target)
        return;
    f32 tx = target->x;
    f32 ox = owner->x;
    if (__builtin_fabsf(tx - ox) < 20.0f && flag) {
        self->field_0x71 = true;
        UnkParams params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x34 = 5;
        params.field_0x18 = 0;
        params.field_0x1c = 1;
        *reinterpret_cast<u16*>(&params.pad_0x3c) = 0x100;
        u32* v = reinterpret_cast<u32*>(params.field_0x8);
        v[0] = sink->field_0x70[0];
        v[1] = sink->field_0x70[1];
        v[2] = sink->field_0x70[2];
        params.field_0x8[12] = 1;
        fn_71006f4b0c(navigation, &params, 1);
    } else {
        f32 d = tx - ox;
        self->field_0x71 = false;
        UnkPlasmaRayKind6Params_ovb0126 params;
        __builtin_memset(params.field_0x8, 0, 13);
        params.field_0x30 = 0;
        params.object = nullptr;
        params.fn = nullptr;
        params.vtable = g_unk_7101a25fb0 + 2;
        params.field_0x34 = 6;
        params.field_0x38 = d < 0.0f;
        params.field_0x39 = 1;
        params.field_0x18 = 1.0f;
        params.field_0x1c = 1;
        __builtin_memcpy(params.field_0x8, d < 0.0f ? &g_unk_710164a6d0 : &g_unk_710164a6dc, 12);
        params.field_0x8[12] = 1;
        params.object = self;
        params.fn = fn_71006b6dc8;
        params.field_0x30 = 1;
        fn_71006f4b0c(navigation, &params, 1);
    }
}
