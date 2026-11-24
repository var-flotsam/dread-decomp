// CShotManager::CTunableShotManager's unit (tools/tu_map.py unit 2576): the shot manager's constructor. It
// builds its arrays with room for 50 shots (30 effects), clears the per-type tables, loads the projectile
// models and points each shot type's slots at its tunables. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "types.h"

using base::global::CRntVector;

extern void* g_unk_7101caa0d8[];

struct UnkShot;
struct UnkListedObj;
struct UnkProjectiles;
struct UnkShotManager_b0011;
// As src/game/CShotManager.cpp declares it: the element at `i`, growing the array to it.
UnkShot** fn_7100ed4b68(base::global::CRntVector<UnkShot*>* self, u32 i);
// As src/game/CShotManager/CTunableShotManager_Resize_b43.cpp defines it.
void CShotManager_ResizeObj290Vector(base::global::CRntVector<UnkObj290_b43a3>* self, u32 n);
// As src/game/CShotManager/CTunableShotManager/fn_7100ed4cb8.cpp defines it: loads the projectile models.
void CShotManager_LoadProjectileModels(UnkProjectiles* self);
// As src/game/CShotManager/CTunableShotManager/CTunableShotManager_Shots_ovb0011.cpp defines it.
void CShotManager_AcquireShotLights(UnkShotManager_b0011* self);

// As src/game/CShotManager/CTunableShotManager_Resize_b43.cpp defines it.
struct UnkObj290_b43a3 {
    virtual void vf_0x0();  // destroys
    u8 pad_0x8[0x288];
};
static_assert(sizeof(UnkObj290_b43a3) == 0x290, "UnkObj290_b43a3 layout");

// A shot's effect (0xfc bytes).
struct UnkShotEffectBody_led4454 {
    u8 data[0xe4];
};
struct UnkShotEffect_led4454;
void fn_7100ddebbc(UnkShotEffect_led4454* self);  // Constructor.
struct UnkShotEffect_led4454 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    UnkShotEffectBody_led4454 body;  // 0x18

    UnkShotEffect_led4454() { fn_7100ddebbc(this); }
    UnkShotEffect_led4454(const UnkShotEffect_led4454& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          body(other.body) {}
};
static_assert(sizeof(UnkShotEffect_led4454) == 0xfc, "UnkShotEffect_led4454 layout");

// A weak reference to a shot's target.
struct UnkShotWeakRef_led4454 {
    void* target;
    s64 generation;
    ~UnkShotWeakRef_led4454() {}
};

struct UnkShotModel_led4454;
void fn_7100ecf93c(UnkShotModel_led4454* self);  // Constructor.
struct UnkShotModel_led4454 {
    u64 field_0x0;
    UnkShotModel_led4454() { fn_7100ecf93c(this); }
};

struct UnkShotPresetNames_led4454;
struct UnkShotPresets_led4454;
void fn_7100eda574(UnkShotPresetNames_led4454* self);  // Constructor.
void fn_7100ed4adc(UnkShotPresets_led4454* self);      // Constructor.
struct UnkShotPresetNames_led4454 {
    u8 data[0x3c0];
    UnkShotPresetNames_led4454() { fn_7100eda574(this); }
};
struct UnkShotPresets_led4454 {
    u8 data[0xb40];
    UnkShotPresets_led4454() { fn_7100ed4adc(this); }
};

// The per-type and per-variant tables the update fills in.
struct UnkShotTables_led4454 {
    u32 field_0xd8[5][2];
    u64 field_0x100[5][2];
    u64 field_0x150[5][2];
    u64* field_0x1a0[5][2][2];
};

class UnkShotShared_led4454;

// An array with room for 50 elements from the start.
template <class T>
struct UnkReservedVector_led4454 : CRntVector<T> {
    UnkReservedVector_led4454() : CRntVector<T>(0xf) { this->Reserve(50); }
};

// The shots, which move with fn_7100eddc68 and are destroyed through their vtables.
struct UnkShots_led4454 : CRntVector<UnkObj290_b43a3> {
    UnkShots_led4454() : CRntVector<UnkObj290_b43a3>(0xf) { Reserve(50); }

    void Reserve(u32 n) {
        if (capacity < n) {
            UnkObj290_b43a3* p =
                static_cast<UnkObj290_b43a3*>(allocator->vf_0x40((u64)n * sizeof(UnkObj290_b43a3), 8, flags));
            if (UnkObj290_b43a3* old = data) {
                UnkObj290_b43a3* dst = p;
                for (u32 i = 0; i < count; ++i) {
                    CShotManager_CopyConstructElement290(dst, old);
                    old->vf_0x0();
                    ++dst;
                    ++old;
                }
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
    }

    // Grows (fn_7100ede9e4) or shrinks to `n` shots.
    void Resize(u32 n) {
        if (count < n) {
            CShotManager_ResizeObj290Vector(this, n);
        } else if (count != n) {
            for (u32 i = n; i < count; ++i)
                data[i].vf_0x0();
            count = n;
        }
    }
};

// Gives the effects room for `n`, copying them over to the new array.
static void ReserveEffects(CRntVector<UnkShotEffect_led4454>& effects, u32 n) {
    if (effects.capacity >= n)
        return;
    UnkShotEffect_led4454* p = static_cast<UnkShotEffect_led4454*>(
        effects.allocator->vf_0x40((u64)n * sizeof(UnkShotEffect_led4454), 8, effects.flags));
    if (effects.data) {
        UnkShotEffect_led4454* src = effects.data;
        UnkShotEffect_led4454* dst = p;
        for (u32 i = 0; i < effects.count; ++i, ++dst, ++src)
            new (dst) UnkShotEffect_led4454(*src);
        effects.allocator->vf_0x48(effects.data);
    }
    effects.capacity = n;
    effects.data = p;
}

// Grows or shrinks the effects to `n`, constructing the new ones in place.
static void ResizeEffects(CRntVector<UnkShotEffect_led4454>& effects, u32 n) {
    u32 old = effects.count;
    if (old < n) {
        ReserveEffects(effects, n);
        old = effects.count;
        for (u32 i = old; i < n; ++i)
            new (&effects.data[i]) UnkShotEffect_led4454();
    } else if (old == n) {
        return;
    }
    effects.count = n;
}

namespace {

// The manager itself (0x1278 bytes); fn_7100ed4454 is its constructor.
struct UnkShotManager_led4454 {
    void** vtable = g_unk_7101caa0d8 + 2;
    UnkShots_led4454 field_0x8;
    CRntVector<UnkShotEffect_led4454> field_0x30{0xf};
    UnkReservedVector_led4454<UnkShot*> field_0x58;
    u32 field_0x80 = 0;
    UnkShotModel_led4454 field_0x88[5][2];
    UnkShotTables_led4454 tables;  // 0xd8
    void* field_0x240[5][5];
    UnkShotPresetNames_led4454 field_0x308;
    UnkShotPresets_led4454 field_0x6c8;
    f32 field_0x1208 = 0.0f;
    CRntVector<UnkShotWeakRef_led4454> field_0x1210{0xf};
    UnkReservedVector_led4454<UnkListedObj*> field_0x1238;  // the shot lights
    u32 field_0x1260 = 0;
    UnkShotShared_led4454* field_0x1268 = nullptr;
    bool field_0x1270 = false;
};
static_assert(__builtin_offsetof(UnkShotManager_led4454, field_0x240) == 0x240, "UnkShotManager_led4454 layout");
static_assert(__builtin_offsetof(UnkShotManager_led4454, field_0x1210) == 0x1210, "UnkShotManager_led4454 layout");
static_assert(sizeof(UnkShotManager_led4454) == 0x1278, "UnkShotManager_led4454 layout");

}  // namespace

// The shot tunables: 14 blocks of 0x278 bytes from +0x28.
struct UnkShotTunable_led4454 {
    u8 data[0x278];
};
struct UnkStruct_71729cf280 {
    u8 pad_0x0[0x28];
    UnkShotTunable_led4454 field_0x28[14];
};
extern UnkStruct_71729cf280 g_unk_71729cf280;

void fn_7100ed4454(void* self) {
    UnkShotManager_led4454* manager = new (self) UnkShotManager_led4454;

    manager->field_0x8.Resize(50);
    ResizeEffects(manager->field_0x30, 30);
    manager->field_0x58.Resize(50);
    manager->field_0x1210.Reserve(50);
    for (u32 i = 0; i < manager->field_0x58.count; ++i)
        *fn_7100ed4b68(&manager->field_0x58, i) = nullptr;

    manager->tables = UnkShotTables_led4454();
    CShotManager_LoadProjectileModels(reinterpret_cast<UnkProjectiles*>(manager));

    UnkShotTunable_led4454* tunables = g_unk_71729cf280.field_0x28;
    manager->field_0x240[0][0] = &tunables[0];
    manager->field_0x240[0][1] = &tunables[1];
    manager->field_0x240[0][2] = &tunables[2];
    manager->field_0x240[0][3] = &tunables[13];
    manager->field_0x240[0][4] = &tunables[12];
    manager->field_0x240[1][0] = &tunables[3];
    manager->field_0x240[1][1] = &tunables[4];
    manager->field_0x240[1][2] = &tunables[5];
    manager->field_0x240[1][3] = &tunables[13];
    manager->field_0x240[1][4] = &tunables[12];
    manager->field_0x240[2][0] = &tunables[6];
    manager->field_0x240[2][1] = &tunables[7];
    manager->field_0x240[2][2] = &tunables[8];
    manager->field_0x240[2][3] = &tunables[13];
    manager->field_0x240[2][4] = &tunables[12];
    manager->field_0x240[3][0] = &tunables[11];
    manager->field_0x240[3][3] = &tunables[13];
    manager->field_0x240[3][4] = &tunables[12];
    manager->field_0x240[4][0] = &tunables[9];
    manager->field_0x240[4][1] = &tunables[9];
    manager->field_0x240[4][2] = &tunables[9];
    manager->field_0x240[4][3] = &tunables[13];
    manager->field_0x240[4][4] = &tunables[10];

    CShotManager_AcquireShotLights(reinterpret_cast<UnkShotManager_b0011*>(manager));
}
