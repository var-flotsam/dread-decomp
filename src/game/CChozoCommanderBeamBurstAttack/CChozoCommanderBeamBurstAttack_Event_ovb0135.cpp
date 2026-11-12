// CChozoCommanderBeamBurstAttack's unit (tools/tu_map.py): the attack's animation event. On the first
// event of kind 3 with the track kind 0xf4061ff25a2163d6 and the burst's id (+0x30 of the object at +0x38 of
// the object at +0x10 of its +0x8), it turns the owner's animation (+0x220) towards the target (stopping
// layer 5 and playing 4 while the target is on the left, the other way round while it is on the right),
// drops the weak handle at +0xa0 after unlinking +0x88 from its target, marks itself fired (+0xa8) and
// keeps the event's value (+0x64 of its +0x8). Returns true. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CFXComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, bool, bool, f32);
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);

struct UnkBurstEventId_ovb0135 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkBurstEventInner_ovb0135 {
    u8 pad_0x0[0x38];
    UnkBurstEventId_ovb0135* field_0x38;
};
struct UnkBurstEventData_ovb0135 {
    u8 pad_0x0[0x10];
    UnkBurstEventInner_ovb0135* field_0x10;
    u8 pad_0x18[0x4c];
    u32 field_0x64;
};
struct UnkBurstEventKind_ovb0135 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkBurstEvent_ovb0135 {
    u32 kind;
    u8 pad_0x4[4];
    UnkBurstEventData_ovb0135* field_0x8;
    UnkBurstEventKind_ovb0135* field_0x10;
};

struct UnkBurstEventSelf_ovb0135 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x78];
    u8 field_0x88[0x18];
    UnkWeakHandle* field_0xa0;
    bool fired;  // 0xa8
    u8 pad_0xa9[3];
    u32 field_0xac;
};

bool fn_710042a044(UnkBurstEventSelf_ovb0135* self, const UnkBurstEvent_ovb0135* event) {
    if (self->fired || event->kind != 3 || event->field_0x10->hash != 0xf4061ff25a2163d6 ||
        event->field_0x8->field_0x10->field_0x38->hash != 0xf08bac6b45a8080e)
        return true;
    f32 x = self->owner->field_0x44;
    auto* anim = static_cast<UnkActorTarget*>(self->owner->field_0x220);
    f32 target_x = fn_7100d7c990(self->owner)->field_0x44;
    f32 x2 = self->owner->field_0x44;
    f32 target_x2 = fn_7100d7c990(self->owner)->field_0x44;
    if (x > target_x) {
        if (x2 > target_x2) {
            fn_7101178e08(anim, 5, 0, 0, -1.0f);
            fn_710117662c(anim, 4, 1, false, false, -1.0f);
        }
    } else if (x2 < target_x2) {
        fn_7101178e08(anim, 4, 0, 0, -1.0f);
        fn_710117662c(anim, 5, 1, false, false, -1.0f);
    }
    if (UnkWeakHandle* handle = self->field_0xa0) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->field_0x88);
        ResetRef(&self->field_0xa0);
    }
    self->fired = true;
    self->field_0xac = event->field_0x8->field_0x64;
    return true;
}


// The parameter block of the vtable-0x7101a25b38 kind (as
// src/game/CChozoCommanderLandingSlamAttack/CChozoCommanderLandingSlamAttack_End_A2_b72.cpp builds it, with
// the optional at +0x10 left unset); its reflected value (+0x70) is destroyed afterwards.
using base::reflection::UnkReflValue;
extern void* g_unk_7101a25b38[];

struct UnkBurstParams_ovb0135 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkBurstParams_ovb0135() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        *reinterpret_cast<u64*>(field_0x8) = 0;
        field_0x3c = false;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = true;
        value.kind = 2;
        value.field_0x14 = 1;
        __builtin_memset(field_0x8 + 5, 0, 8);
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkBurstParams_ovb0135() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkTrackBlend;
using base::reflection::UnkTunable;
extern UnkTunable g_unk_71726c56e8;

struct UnkBurstStartOwner_ovb0135 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x70];
    void* navigation;  // 0x298
};

struct UnkBurstStart_ovb0135 {
    u8 pad_0x0[0x8];
    UnkBurstStartOwner_ovb0135* owner;  // 0x8
    u8 pad_0x10[0x4];
    bool field_0x14;
    u8 pad_0x15[0x43];
    u32 field_0x58;
    u8 pad_0x5c[0x10];
    u32 field_0x6c;
};

void fn_7100426de4(UnkBurstStart_ovb0135* self, u32 a, f32 b, f32 c, f32 d);

// Starts the burst: sends the owner's animation the burst request (mode 0), sets its state (+0x58) to 1,
// hands the owner's navigation (+0x298) a parameter block and starts the aim (fn_7100426de4) over the
// tunable's time (+0x2c).
void fn_7100428af4(UnkBurstStart_ovb0135* self) {
    void* animation = self->owner->animation;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, 0xb891d3278bf21139, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                  &g_unk_7172769758, &mask);
    self->field_0x58 = 1;
    self->field_0x14 = false;
    UnkBurstParams_ovb0135 params;
    fn_71006f4b0c(self->owner->navigation, &params, 0);
    f32 time = *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71726c56e8) + 0x2c);
    fn_7100426de4(self, 0, 1.0f, time, time);
    self->field_0x6c = 0;
}

// The id table (fn_7100031894 builds it; as src/game/CAbilityEnergyWave/CAbilityEnergyWave_Effect_ovb0022.cpp).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

using base::global::CStrId;

struct UnkEntity;
struct UnkObject;
struct UnkComponent;
struct UnkFxObject_ovb0041;
UnkComponent* fn_7101152b90(UnkObject*, u64);
bool fn_710114e668(UnkObject*, u8);
void fn_7100146d84(UnkFxObject_ovb0041* fx, const UnkVec3f* position);
void fn_7100146d98(UnkFxObject_ovb0041* fx, const UnkVec3f* angle);
extern "C" f32 atan2f(f32 y, f32 x);

struct UnkBurstBeamFx_ovb0135 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x115];
    bool active;  // 0x121
};

struct UnkBurstAimOwner_ovb0135 {
    u8 pad_0x0[0x228];
    UnkEntity* entity;  // 0x228
    u8 pad_0x230[0x50];
    UnkObject* model;  // 0x280
};
struct UnkBurstAim_ovb0135 {
    u8 pad_0x0[0x8];
    UnkBurstAimOwner_ovb0135* owner;  // 0x8
};
struct UnkBurstAimPoint_ovb0135 {
    u8 pad_0x0[0x14];
    UnkVec3f pos;  // 0x14
};

// Places the beam effect (fn_7101152b90, switched on when off) at the point's x and y and the beam node's
// depth, turned a quarter turn past the node's x axis (its transform from fn_71011735e8 with the id
// table's id at +0xd58).
void fn_710042995c(UnkBurstAim_ovb0135* self, const UnkBurstAimPoint_ovb0135* point) {
    UnkObject* model = self->owner->model;
    UnkEntity* entity = self->owner->entity;
    if (!model || !entity)
        return;
    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    const CStrId& id =
        *reinterpret_cast<const CStrId*>(reinterpret_cast<u8*>(&GameManagerIdTable()) + 0xd58);
    {
        u64 key = *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
        fn_71011735e8(entity, &key, &m, 0);
    }
    f32 ax = m.m[0], ay = m.m[4];
    f32 len2 = ax * ax + ay * ay + 0.0f;
    u32 z;
    __builtin_memcpy(&z, &m.m[11], 4);
    f32 len = __builtin_sqrtf(len2);
    auto* fx = reinterpret_cast<UnkBurstBeamFx_ovb0135*>(fn_7101152b90(model, 0x28d8e7efe4423be0ull));
    if (!fx || !fn_710114e668(model, fx->kind))
        return;
    f32 inv = 1.0f / (len + 0.0001f);
    f32 nx = ax * inv;
    f32 ny = ay * inv;
    if (!fx->active)
        fn_7101152c0c(model, fx, 1, 1);
    UnkVec3f pos = point->pos;
    __builtin_memcpy(&pos.z, &z, 4);
    fn_7100146d84(reinterpret_cast<UnkFxObject_ovb0041*>(fx), &pos);
    UnkVec3f angle(0.0f, 0.0f, atan2f(ny, nx) + 1.5707964f);
    fn_7100146d98(reinterpret_cast<UnkFxObject_ovb0041*>(fx), &angle);
}
