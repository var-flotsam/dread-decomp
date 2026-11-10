// CCooldownXBossLavaDropsAttack's unit (tools/tu_map.py): clearing the word at +0x2ec and setting the owner's
// (+0x8) timeline (+0x2e8) value for the id g_unk_7101649b18 to the all-bits mask (fn_71004c8330); the same for
// the id g_unk_7101649b08 after clearing +0x2e0, then an effect from the model manager placed at +0x2f0
// (rotation 0, scale 1), enabled, looping at speed -1 and kept at +0x320 (fn_71004c7d10); and the attack's
// destructor (fn_71004c86cc: the vtable, the listener at +0x328 (game_5821e4/CAttackDtor.h), four value
// holders from +0x270 down to +0x120, the member at +0x68, then CAttack's destructor). Names and layouts are
// placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern const u64 g_unk_7101649b18;  // reached through the GOT

struct UnkLavaDropsOwner_ovb0111 {
    u8 pad_0x0[0x2e8];
    UnkTimelineComponent* field_0x2e8;
};

struct UnkLavaDrops_ovb0111 {
    u8 pad_0x0[0x8];
    UnkLavaDropsOwner_ovb0111* owner;
    u8 pad_0x10[0x2dc];
    u32 field_0x2ec;
};

void fn_71004c8330(UnkLavaDrops_ovb0111* self) {
    self->field_0x2ec = 0;
    UnkTimelineComponent* timeline = self->owner->field_0x2e8;
    base::reflection::UnkReflTypeIdArg all;
    all.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &g_unk_7101649b18, &all.id, false, -1.0f);
}

extern const u64 g_unk_7101649b08;  // reached through the GOT
extern const u8 g_unk_71726d7cd0[] __attribute__((visibility("hidden")));  // the effect's path id

void fn_7100146dac(UnkSwitchObject* self, bool enable);

struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0111 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);  // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);  // 0x98
};
static inline UnkFxObjectVtable_ovb0111* FxVtable(UnkFxObject_ovb0041* fx) {
    return *reinterpret_cast<UnkFxObjectVtable_ovb0111**>(fx);
}
UnkFxObject_ovb0041* fn_71003b7db0(UnkModelManager* self, const void* path, bool a, bool b);
void fn_71011a9bb4(UnkModelManager* self, UnkFxObject_ovb0041* fx);


struct UnkLavaDropsFx_ovb0111 {
    u8 pad_0x0[0x8];
    UnkLavaDropsOwner_ovb0111* owner;
    u8 pad_0x10[0x2d0];
    u64 field_0x2e0;
    u8 pad_0x2e8[0x8];
    UnkVec3f field_0x2f0;
    u8 pad_0x2fc[0x24];
    UnkFxObject_ovb0041* fx;  // 0x320
};
static_assert(__builtin_offsetof(UnkLavaDropsFx_ovb0111, fx) == 0x320, "layout");

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_71004c7d10(UnkLavaDropsFx_ovb0111* self) {
    self->field_0x2e0 = 0;
    UnkTimelineComponent* timeline = self->owner->field_0x2e8;
    base::reflection::UnkReflTypeIdArg all;
    all.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &g_unk_7101649b08, &all.id, false, -1.0f);
    UnkModelManager* manager = ModelManager();
    UnkVec3f position = self->field_0x2f0;
    UnkVec3f rotation(0.0f, 0.0f, 0.0f);
    UnkVec3f scale(1.0f, 1.0f, 1.0f);
    UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, g_unk_71726d7cd0, false, true);
    if (fx) {
        FxVtable(fx)->set_position(fx, &position);
        FxVtable(fx)->set_rotation(fx, &rotation);
        FxVtable(fx)->set_scale(fx, &scale);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
        FxVtable(fx)->set_loop(fx, true);
        FxVtable(fx)->set_speed(fx, -1.0f);
        fn_71011a9bb4(manager, fx);
    }
    self->fx = fx;
}

extern void* g_unk_7101a3e6c0[];  // the attack's vtable
extern void* g_unk_7101a3e820[];  // the value holders' vtable
extern void* g_unk_7101a3e878[];  // the member's vtable

struct UnkLavaDropsPair_ovb0111 {
    u64 a, b;
    ~UnkLavaDropsPair_ovb0111() {
        a = 0;
        b = 0;
    }
};

struct UnkLavaDropsValueHolderBase_ovb0111 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    ~UnkLavaDropsValueHolderBase_ovb0111() {
        vtable = reinterpret_cast<char*>(g_unk_7101a3e820) + 0x10;
        fn_71000a1c60(&value);
    }
};

struct UnkLavaDropsValueHolder_ovb0111 : UnkLavaDropsValueHolderBase_ovb0111 {
    UnkLavaDropsPair_ovb0111 pairs[4];
};
static_assert(sizeof(UnkLavaDropsValueHolder_ovb0111) == 0x70, "value holder layout");

struct UnkLavaDropsMember_ovb0111 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkLavaDropsMember_ovb0111() {
        vtable = reinterpret_cast<char*>(g_unk_7101a3e878) + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkLavaDropsAttack_ovb0111 {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkLavaDropsMember_ovb0111 field_0x68;
    u8 pad_0x90[0x90];
    UnkLavaDropsValueHolder_ovb0111 field_0x120;
    UnkLavaDropsValueHolder_ovb0111 field_0x190;
    UnkLavaDropsValueHolder_ovb0111 field_0x200;
    UnkLavaDropsValueHolder_ovb0111 field_0x270;
    u8 pad_0x2e0[0x48];
    UnkAttackListenerRaw listener;
};
static_assert(__builtin_offsetof(UnkLavaDropsAttack_ovb0111, listener) == 0x328, "layout");

void fn_71004c86cc(UnkLavaDropsAttack_ovb0111* self) {
    self->vtable = g_unk_7101a3e6c0 + 2;
    self->listener.Destroy();
    self->field_0x270.~UnkLavaDropsValueHolder_ovb0111();
    self->field_0x200.~UnkLavaDropsValueHolder_ovb0111();
    self->field_0x190.~UnkLavaDropsValueHolder_ovb0111();
    self->field_0x120.~UnkLavaDropsValueHolder_ovb0111();
    self->field_0x68.~UnkLavaDropsMember_ovb0111();
    fn_71003606c0(self);
}
