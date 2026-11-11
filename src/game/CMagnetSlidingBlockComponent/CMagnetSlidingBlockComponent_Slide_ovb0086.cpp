// CMagnetSlidingBlockComponent's unit (tools/tu_map.py): the sliding block's states. The rumble it plays
// while sliding (a handle at +0x4e0, stopped with fn_7100072328 as in src/game/CPlayerInfo/
// CPlayerInfo_Pending_ovb0071.cpp), the progress (+0x348) rescaled between the two travel times (+0x334,
// +0x338) by the current speed (between +0x408 and +0x40c by how much of +0x410 is left), the colliders
// switched on or off and the glow material faded with the speed. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CComponent.h"
#include "game/CMagnetSlidingBlockComponent.h"
#include "game/CMaterialFXComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkTU39.h"
#include "game/UnkTU50.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;
using base::color::CColor4F;
using base::math::CVector3D;
using base::math::UnkVector4D;

// A handle to a playing rumble: current while the target's serial is the handle's.
struct UnkHandleTarget_b0086 {
    void* target;
    u64 serial;
};

struct UnkHandle {
    UnkHandleTarget_b0086* target;
    u64 serial;

    void* Get() const {
        if (target && target->serial == serial && serial != ~0ull)
            return target->target;
        return nullptr;
    }
};

// The rumble manager, created on first use.
extern UnkRumbleManager* g_unk_7101d47348;

static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

struct UnkMagnetStringData_b0086 {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

// A file-path id (pool g_unk_7101d47200) whose count is a plain int.
struct UnkMagnetPathId_b0086 {
    UnkStringData* data;

    explicit UnkMagnetPathId_b0086(const base::global::UnkFilePathStrIdBase& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++reinterpret_cast<UnkMagnetStringData_b0086*>(data)->ref_count;
    }
    UnkMagnetPathId_b0086& operator=(const base::global::UnkFilePathStrIdBase& other) {
        if (other.data != data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++reinterpret_cast<UnkMagnetStringData_b0086*>(d)->ref_count;
            if (data != &pool->empty) {
                if (--reinterpret_cast<UnkMagnetStringData_b0086*>(data)->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
    ~UnkMagnetPathId_b0086() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--reinterpret_cast<UnkMagnetStringData_b0086*>(data)->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// What a rumble is played from (the UnkRumbleParams of src/game/CRumbleComponent.cpp).
struct UnkMagnetRumbleParams_b0086 {
    UnkMagnetPathId_b0086 path;
    u64 field_0x8;
    u32 field_0x10;
    CVector3D field_0x14;
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkMagnetRumbleParams_b0086()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff), field_0x14(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};
static_assert(sizeof(UnkMagnetRumbleParams_b0086) == 0x48, "");

struct UnkRumbleParams;
UnkHandle fn_7100074354(UnkRumbleManager* self, const UnkRumbleParams* params);

// The block's sliding rumble file.
extern base::global::CFilePathStrId g_unk_71727375f0 __attribute__((visibility("hidden")));

// As src/game/CCollisionComponent/fn_71008651c8.cpp declares it.
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::UnkVector<UnkStruct_71008651c8_Entry*>;
void fn_71008651c8(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);
extern void* const g_unk_71727375d0 __attribute__((visibility("hidden")));  // the block's colliders' key

struct CMagnetSlidingBlockComponent;

struct UnkHandleOwner_71010c4230;

// The block's timeline key.
extern const u64 g_unk_710164b248 __attribute__((visibility("hidden")));

struct UnkMagnetState_b0086 {
    u8 pad_0x0[0x10];
    s32 kind;
};

struct UnkMagnetOwner_b0086 {
    u8 pad_0x0[0x2a0];
    UnkStruct_71008651c8* colliders;  // 0x2a0
};

struct UnkMagnetSlidingBlock_b0086 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkMagnetOwner_b0086* owner;  // 0x30
    u8 pad_0x38[0x290 - 0x38];
    UnkMagnetState_b0086* state;  // 0x290
    u8 pad_0x298[0x330 - 0x298];
    f32 field_0x330;
    f32 field_0x334;
    f32 field_0x338;
    u8 pad_0x33c[8];
    f32 field_0x344;
    f32 field_0x348;
    u8 pad_0x34c[4];
    f32 field_0x350;
    u8 pad_0x354[4];
    u8 timer[0xb0];  // 0x358
    f32 field_0x408;
    f32 field_0x40c;
    f32 field_0x410;
    f32 speed;  // 0x414
    u8 pad_0x418[0x4e0 - 0x418];
    UnkHandle rumble;  // 0x4e0

    f32 vf_0x1d0() { return reinterpret_cast<f32 (*)(UnkMagnetSlidingBlock_b0086*)>(vtable[0x1d0 / 8])(this); }

    // Between +0x408 and +0x40c by how much of +0x410 is left of vf_0x1d0.
    f32 Lerp() {
        f32 t = 1.0f - field_0x410 / vf_0x1d0();
        f32 lo = field_0x408;
        f32 range = field_0x40c - lo;
        f32 c = !(t <= 0.0f) ? (!(t >= 1.0f) ? t : 1.0f) : 0.0f;
        return lo + range * c;
    }

    void StopRumble() {
        if (rumble.Get() != g_unk_7101d586c0.Get())
            fn_7100072328(rumble.Get(), 0.2f, 0.0f);
    }
};
static_assert(__builtin_offsetof(UnkMagnetSlidingBlock_b0086, rumble) == 0x4e0, "layout");

// Stops the rumble, then fn_71010c4230.
void fn_710091fddc(UnkMagnetSlidingBlock_b0086* self) {
    self->StopRumble();
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
}

void fn_710091feb4(UnkMagnetSlidingBlock_b0086* self) {
    self->field_0x344 = self->field_0x330;
    self->StopRumble();
}

// Stops the rumble and starts the sliding one (0.3 strength, from the block's file).
void fn_71009201a8(UnkMagnetSlidingBlock_b0086* self) {
    self->StopRumble();
    UnkMagnetRumbleParams_b0086 params;
    params.path = g_unk_71727375f0;
    params.field_0x20[2] = 0.3f;
    params.field_0x10 = 0;
    self->rumble = fn_7100074354(RumbleManager(), reinterpret_cast<const UnkRumbleParams*>(&params));
}

// Entering state 5 (+0x344 halved): +0x348 rescaled from the +0x338 range to the +0x334 one, then the
// timeline key and the rumble.
void fn_71009200a8(UnkMagnetSlidingBlock_b0086* self, s32 state) {
    if (state == 5)
        self->field_0x344 = self->field_0x330 * 0.5f;
    f32 speed = self->Lerp();
    f32 full = self->field_0x338 / speed;
    f32 kept = 1.0f - self->field_0x348 / full;
    f32 speed2 = self->Lerp();
    self->field_0x348 = kept * (self->field_0x334 / speed2);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b248, false);
    fn_71009201a8(self);
}

// The other way (state 3): from the +0x334 range to the +0x338 one, and the rumble stops.
void fn_7100920a30(UnkMagnetSlidingBlock_b0086* self, s32 state) {
    if (state == 3)
        self->field_0x344 = self->field_0x330;
    f32 speed = self->Lerp();
    f32 full = self->field_0x334 / speed;
    f32 kept = 1.0f - self->field_0x348 / full;
    f32 speed2 = self->Lerp();
    self->field_0x348 = kept * (self->field_0x338 / speed2);
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b248, false);
    self->StopRumble();
}

void fn_7100921248(UnkMagnetSlidingBlock_b0086* self, f32 target) {
    if (self->field_0x40c != target) {
        self->field_0x40c = target;
        self->field_0x410 = self->vf_0x1d0();
        if (UnkMagnetState_b0086* state = self->state) {
            switch (state->kind) {
            case 5: {
                f32 speed = self->Lerp();
                f32 full = self->field_0x338 / speed;
                self->field_0x348 = self->field_0x350 * full;
                return;
            }
            case 3: {
                f32 speed = self->Lerp();
                f32 full = self->field_0x334 / speed;
                self->field_0x348 = (1.0f - self->field_0x350) * full;
                return;
            }
            }
        }
        self->field_0x410 = 0.0f;
        self->field_0x408 = self->field_0x40c;
    }
}

// Turns the block's colliders on or off and applies the matching speed (zero while the timer at
// +0x358 runs, as fn_710092103c).
void fn_7100920f40(UnkMagnetSlidingBlock_b0086* self, bool on) {
    UnkOut_71008651c8 found(0xf);
    fn_71008651c8(self->owner->colliders, &g_unk_71727375d0, &found);
    for (UnkStruct_71008651c8_Entry** it = found.data; it != found.data + found.count; ++it)
        fn_7100d35a98(*reinterpret_cast<UnkEntry**>(reinterpret_cast<char*>(*it) + 0x28), on);
    self->speed = on ? 1.0f : 0.0f;
    if (fn_710002fb70(self->timer))
        self->speed = 0.0f;
    fn_71009210d4(reinterpret_cast<CMagnetSlidingBlockComponent*>(self), self->speed);
    found.Free();
}

bool fn_7100199d88(void* map, const u64* key, u32* index);

struct UnkMagnetMaterial_b0086 {
    u8 pad_0x0[0x14c];
    f32 r, g, b, a;  // 0x14c
};

struct UnkMagnetMaterials_b0086 {
    u8 pad_0x0[0x48];
    UnkMagnetMaterial_b0086** entries;  // 0x48
};

struct UnkMagnetModelRes_b0086 {
    u8 pad_0x0[0x78];
    UnkMagnetMaterials_b0086* materials;  // 0x78
};

struct UnkMagnetModel_b0086 {
    u8 pad_0x0[0x138];
    UnkMagnetModelRes_b0086* res;  // 0x138
};

struct UnkMagnetModelComponent_b0086 {
    u8 pad_0x0[0x50];
    UnkMagnetModel_b0086* model;  // 0x50
};

// A material colour change as fn_71009272c4 builds it (kind at +0, a string id at +0x38).
struct UnkMagnetColorChange_b0086 {
    u32 kind;
    u8 pad_0x4[0x34];
    union {
        base::UnkStrIdMember id;  // 0x38
    };
    u8 pad_0x40[0x18];

    UnkMagnetColorChange_b0086() {}  // built by fn_71009272c4
    ~UnkMagnetColorChange_b0086() { id.~UnkStrIdMemberOf(); }
};

struct UnkMagnetFx_b0086;
void fn_71009272c4(UnkMagnetColorChange_b0086* self, u32 index, bool flag, const UnkVector4D* from,
                   const UnkVector4D* to, f32 time);

struct UnkMagnetOwner4_b0086 {
    u8 pad_0x0[0x228];
    UnkMagnetModelComponent_b0086* model;  // 0x228
    u8 pad_0x230[0x58];
    UnkMagnetFx_b0086* fx;  // 0x288
};

struct UnkMagnetSlidingBlock4_b0086 {
    u8 pad_0x0[0x30];
    UnkMagnetOwner4_b0086* owner;
};

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

// Fades the block's glow material (key 0x117cd46281e7f67a) to its colour with alpha `speed`.
// Out of line in the original (fn_7100920f40 calls it).
__attribute__((noinline)) void fn_71009210d4(CMagnetSlidingBlockComponent* component, f32 speed) {
    UnkMagnetSlidingBlock4_b0086* self = reinterpret_cast<UnkMagnetSlidingBlock4_b0086*>(component);
    u64 key = 0x117cd46281e7f67a;
    UnkMagnetModelComponent_b0086* model = self->owner->model;
    UnkMagnetFx_b0086* fx = self->owner->fx;
    if (!model || !fx)
        return;
    UnkMagnetModel_b0086* m = model->model;
    if (!m)
        return;
    u32 index;
    if (!fn_7100199d88(m->res->materials, &key, &index))
        return;
    u32 i = index;
    UnkMagnetMaterial_b0086* mat = m->res->materials->entries[i];
    CColor4F to(NonNegative(mat->r), NonNegative(mat->g), NonNegative(mat->b), NonNegative(mat->a));
    CColor4F from(NonNegative(mat->r), NonNegative(mat->g), NonNegative(mat->b), speed);
    UnkMagnetColorChange_b0086 change;
    UnkVector4D vto = fn_7100030878(&to);
    UnkVector4D vfrom = fn_7100030878(&from);
    fn_71009272c4(&change, i, false, &vto, &vfrom, 0.1f);
    change.kind = 2;
    fn_7100926aa0(fx, &change);
}
