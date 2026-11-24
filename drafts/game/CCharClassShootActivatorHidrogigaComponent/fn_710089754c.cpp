// CCharClassShootActivatorHidrogigaComponent unit (tools/tu_map.py unit 1347): the update. After the base
// update it ends a hit once the Hydrogiga's cooldown has passed, fades its value toward the target, tints
// the model by how charged it is and, when a shot is pending, restarts the owner's AI with every shot at
// full scale and sends the linked part to state 2. Names and layouts are placeholders.
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "base/UnkDestructors.h"
#include "base/UnkRefHandle.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassShootActivatorHidrogigaComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU437.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::math::CVector4D;
using base::reflection::UnkReflObject;

struct UnkHydrogigaAI_b57a3;
struct UnkSteamJetParts_b49;
struct UnkSteamJetPart_b49;

s32 CountHydrogigaPartsWithValueAtOrAboveThreshold(UnkHydrogigaAI_b57a3* self);
UnkSteamJetPart_b49** fn_71008921a0(UnkSteamJetParts_b49* self, u32 index);

struct UnkHydrogigaShooter_l89754c;
void fn_7100891690(UnkHydrogigaShooter_l89754c* self, f32 dt);  // the base update
f32 fn_71005befec(UnkHydrogigaAI_b57a3* ai);
bool fn_71005befd8(UnkHydrogigaAI_b57a3* ai);

// The owner's AI as this component drives it.
struct UnkShooterAI_l89754c {
    u8 pad_0x0[0xb0];
    u8 source[0x28];  // 0xb0
    u32 state;        // 0xd8
    u8 pad_0xdc[0x4];
    base::UnkRefHandle handle;  // 0xe0
    u8 pad_0xe8[0x168 - 0xe8];
    f32 field_0x168;
};
void fn_7100a68480(UnkShooterAI_l89754c* ai, bool restart);
void fn_7100a6777c(UnkShooterAI_l89754c* ai);

struct UnkHydrogigaState_l89754c {
    u8 pad_0x0[0x10];
    u32 id;
};

struct UnkHydrogigaAI_l89754c : UnkReflObject {
    u8 pad_0x8[0x149c4 - 0x8];
    u32 field_0x149c4;
    u8 pad_0x149c8[0x14cb8 - 0x149c8];
    UnkHydrogigaState_l89754c* field_0x14cb8;
};

struct UnkHydrogigaActor_l89754c {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkHydrogigaHolder_l89754c {
    u8 pad_0x0[0x10];
    UnkHydrogigaActor_l89754c* actor;  // 0x10
};

struct UnkHydrogigaRegistry_l89754c {
    u8 pad_0x0[0x100];
    UnkHydrogigaHolder_l89754c* hydrogiga;  // 0x100
};

struct UnkShot_l89754c {
    void** vtable;

    void SetScale(f32 scale) { reinterpret_cast<void (*)(UnkShot_l89754c*, f32)>(vtable[0x168 / 8])(this, scale); }
};

struct UnkCounted_l89754c : base::UnkRefCounted {
    void* object;  // 0x10
};

// A lazily resolved reference: while the state is 1 the source is looked up into the handle.
struct UnkLazyRef_l89754c {
    u8 source[0x28];
    u32 state;
    u8 pad_0x2c[0x4];
    base::UnkRefHandle handle;

    void* Object() {
        if (state == 1) {
            {
                base::UnkRefPtrMember fresh = fn_7100353be8(source);
                fn_710031d208(&handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            state = 2;
        }
        auto* counted = static_cast<UnkCounted_l89754c*>(handle.ptr);
        return counted && counted->object ? counted->object : nullptr;
    }
};

struct UnkHydrogigaModelTint_l89754c {
    u8 pad_0x0[0x30];
    CVector4D* color;  // 0x30
};

struct UnkHydrogigaModel_l89754c {
    u8 pad_0x0[0x14c];
    CVector4D color;  // 0x14c
    u8 pad_0x15c[0x1f0 - 0x15c];
    UnkHydrogigaModelTint_l89754c* tint;  // 0x1f0
};

struct UnkHydrogigaShooter_l89754c {
    u8 pad_0x0[0x30];
    u64 owner;  // 0x30
    u8 pad_0x38[0x3d8 - 0x38];
    u8 shots[0x8];  // 0x3d8
    u32 shot_count;  // 0x3e0
    u8 pad_0x3e4[0x448 - 0x3e4];
    UnkHydrogigaModel_l89754c* model;  // 0x448
    u8 pad_0x450[0x4b0 - 0x450];
    f32 field_0x4b0;
    u8 pad_0x4b4[0x8];
    f32 field_0x4bc;
    u8 pad_0x4c0[0x588 - 0x4c0];
    UnkLazyRef_l89754c target;  // 0x588
    u8 pad_0x5c0[0x8];
    UnkLazyRef_l89754c part;  // 0x5c8
    bool shot_pending;  // 0x600
    bool hit;           // 0x601
    u8 pad_0x602[0x1];
    bool tinted;  // 0x603
    f32 value;    // 0x604
    f32 from;     // 0x608
    f32 to;       // 0x60c
    f32 duration;  // 0x610
    f32 time;      // 0x614
};
static_assert(__builtin_offsetof(UnkHydrogigaShooter_l89754c, part) == 0x5c8, "layout");
static_assert(__builtin_offsetof(UnkHydrogigaShooter_l89754c, time) == 0x614, "layout");

static inline UnkHydrogigaRegistry_l89754c* Registry() {
    if (!g_unk_71726b2398) {
        char* instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return reinterpret_cast<UnkHydrogigaRegistry_l89754c*>(g_unk_71726b2398);
}

static inline UnkShooterAI_l89754c* OwnerAI(UnkHydrogigaShooter_l89754c* self) {
    return reinterpret_cast<UnkShooterAI_l89754c*>(fn_71005c1008(self->owner));
}

// The AI's hit-recovery time: shorter once the Hydrogiga is below half health.
static inline f32 RecoveryTime(UnkHydrogigaAI_b57a3* ai) {
    return fn_71005befec(ai) < 0.5f ? 6.0f : 3.0f;
}

static inline void Restart(UnkHydrogigaShooter_l89754c* self, UnkShooterAI_l89754c* ai) {
    if (void* object = self->target.Object())
        if (UnkShooterAI_l89754c* target = reinterpret_cast<UnkShooterAI_l89754c*>(fn_71005c1008(reinterpret_cast<u64>(object))))
            fn_7100a6777c(target);
    (void)ai;
}

void fn_710089754c(UnkHydrogigaShooter_l89754c* self, f32 dt) {
    fn_7100891690(self, dt);

    UnkReflObject* ai = nullptr;
    UnkHydrogigaHolder_l89754c* holder = Registry()->hydrogiga;
    if (holder && holder->actor)
        ai = holder->actor->ai;
    bool found = false;
    UnkHydrogigaAI_l89754c* hydrogiga = nullptr;
    if (ai) {
        found = fn_710009d50c(ai->vf_0x0(), fn_71005ba098());
        hydrogiga = found ? static_cast<UnkHydrogigaAI_l89754c*>(ai) : nullptr;
        if (found && self->hit) {
            auto* hydrogiga_ai = reinterpret_cast<UnkHydrogigaAI_b57a3*>(hydrogiga);
            f32 elapsed = self->field_0x4b0 - self->field_0x4bc;
            f32 limit = fn_71005befec(hydrogiga_ai) < 0.5f ? 0.3f : 1.0f;
            if (elapsed >= limit || self->field_0x4bc <= 0.0f) {
                u64 owner = self->owner;
                self->hit = false;
                if (auto* owner_ai = reinterpret_cast<UnkShooterAI_l89754c*>(fn_71005c1008(owner))) {
                    owner_ai->field_0x168 = RecoveryTime(hydrogiga_ai);
                    fn_7100a68480(owner_ai, false);
                    Restart(self, owner_ai);
                }
            }
        }
    }

    self->time += dt;
    f32 value;
    if (self->time < self->duration) {
        f32 t = self->time / self->duration;
        t = !(t >= 1.0f) ? t : 1.0f;
        value = self->from + t * (self->to - self->from);
    } else {
        value = self->to;
    }
    self->value = value;

    if (self->tinted) {
        f32 charge = self->field_0x4bc / self->field_0x4b0;
        f32 red, green;
        f32 blue = 0.0f;
        if (charge > 0.0f) {
            f32 c = charge >= 1.0f ? 1.0f : charge;
            if (charge >= 1.0f || charge > 0.5f) {
                red = 1.0f;
                green = 1.0f - (c - 0.5f) * 2.0f;
            } else {
                red = charge * 2.0f;
                green = 1.0f;
            }
        } else {
            red = blue;
            green = 1.0f;
        }
        f32 k = value * (value * (value * value));
        UnkHydrogigaModel_l89754c* model = self->model;
        CVector4D color = {red * k, green * k, k * blue, 1.0f};
        fn_710007b518(&model->color, &color);
        if (model->tint)
            *model->tint->color = color;
    }

    if (UnkShooterAI_l89754c* owner_ai = OwnerAI(self)) {
        if (owner_ai->state == 1) {
            {
                base::UnkRefPtrMember fresh = fn_7100353be8(owner_ai->source);
                fn_710031d208(&owner_ai->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            owner_ai->state = 2;
        }
    }

    if (!found || !self->shot_pending)
        return;
    auto* shooter_ai = reinterpret_cast<UnkHydrogigaAI_b57a3*>(hydrogiga);
    if (!fn_71005befd8(shooter_ai) || static_cast<u32>(CountHydrogigaPartsWithValueAtOrAboveThreshold(shooter_ai)) <= 1) {
        if (hydrogiga->field_0x14cb8 && hydrogiga->field_0x14cb8->id == 2)
            hydrogiga->field_0x149c4 = 0;
        if (UnkShooterAI_l89754c* owner_ai = OwnerAI(self)) {
            owner_ai->field_0x168 = RecoveryTime(shooter_ai);
            fn_7100a68480(owner_ai, true);
            self->shot_pending = false;
            for (u32 i = 0; i < self->shot_count; ++i) {
                auto* shot = reinterpret_cast<UnkShot_l89754c*>(
                    *fn_71008921a0(reinterpret_cast<UnkSteamJetParts_b49*>(self->shots), i));
                shot->SetScale(1.0f);
            }
            Restart(self, owner_ai);
        }
    }
    if (void* object = self->part.Object()) {
        if (auto* part = reinterpret_cast<UnkSmOwner_A5_b73<0x40>*>(fn_7100897b0c(object)))
            RequestState_A5_b73<15, 5>(&part->machine, 2);
    }
}
