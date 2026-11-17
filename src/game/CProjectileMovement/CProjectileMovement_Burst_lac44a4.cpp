// CProjectileMovement's unit (tools/tu_map.py, unit 1623): spawns the movement's burst effect (+0x488, or the
// given path when it is set) through the model manager at a point raised by 20, pushes its node along the given
// direction and sets a scaled record on it, then sends the definition's event (+0x5c8, +0xf0) if it has one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/UnkCloneHelpers.h"
#include "game/CAutclastChargeAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CProjectileMovement/CProjectileMovement_Spawn_ovb0046.cpp declares it.
struct UnkFxObject_ovb0041;
struct UnkFxObjectVtable_ovb0041 {
    void* unk_0x0[2];
    void (*set_position)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x10
    void (*set_rotation)(UnkFxObject_ovb0041* self, const UnkVec3f* v);  // 0x18
    void (*set_scale)(UnkFxObject_ovb0041* self, const UnkVec3f* v);     // 0x20
    void* unk_0x28[13];
    void (*set_loop)(UnkFxObject_ovb0041* self, bool loop);               // 0x90
    void (*set_speed)(UnkFxObject_ovb0041* self, f32 speed);              // 0x98
};
struct UnkFxObject_ovb0041 {
    UnkFxObjectVtable_ovb0041* vtable;
};

// As src/game/UnkTU308/fn_710021688c.cpp defines it.
struct UnkScaledRecord {
    void* vtable;
    f32 scale;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 vec[3];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    u32 pad_0x34;
    s64 scaled;
    s32 fixed[3];
    u8 isZero;
    u8 field_0x4d;
};

void fn_710021688c(f32 scale, UnkScaledRecord* self, const u32* vec);
void fn_7100209394(UnkXformNode* self, const UnkScaledRecord* record);

struct UnkBurstFx_lac44a4 {
    u8 pad_0x0[0x28];
    UnkXformNode node;  // 0x28
};

struct UnkBurstSpawned_lac44a4 {
    u8 pad_0x0[0x108];
    f32 field_0x108;
    u8 pad_0x10c[0x4e];
    bool field_0x15a;
};

struct UnkBurstDef_lac44a4 {
    u8 pad_0x0[0xf0];
    base::UnkStrIdMemberOf<g_unk_7101d47108> event;  // 0xf0
};

typedef base::UnkStrIdMemberOf<g_unk_7101d47200> UnkBurstPath_lac44a4;

struct UnkProjectileBurst_lac44a4 {
    u8 pad_0x0[0x40];
    u8 field_0x40[0x448];
    UnkBurstPath_lac44a4 effect;  // 0x488
    u8 pad_0x490[0x10];
    f32 height;  // 0x4a0
    f32 push;    // 0x4a4
    u8 pad_0x4a8[0x120];
    UnkBurstDef_lac44a4* def;  // 0x5c8
};
static_assert(__builtin_offsetof(UnkProjectileBurst_lac44a4, def) == 0x5c8, "layout");

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void fn_7100ac44a4(UnkProjectileBurst_lac44a4* self, const UnkVec3f* at, const UnkVec3f* angle,
                   const UnkVec3f* dir, const UnkBurstPath_lac44a4* path) {
    if (self->effect.data != &g_unk_7101d47200->empty) {
        UnkVec3f angles = *angle;
        angles.z += self->height;
        UnkModelManager* manager = ModelManager();
        const UnkBurstPath_lac44a4* effect = path->data == &g_unk_7101d47200->empty ? &self->effect : path;
        UnkVec3f position(at->x + 0.0f, at->y + 0.0f, at->z + 20.0f);
        UnkVec3f rotation = angles;
        UnkVec3f scale(1.0f, 1.0f, 1.0f);
        UnkFxObject_ovb0041* fx = fn_71003b7db0(manager, effect, false, true);
        if (fx) {
            fx->vtable->set_position(fx, &position);
            fx->vtable->set_rotation(fx, &rotation);
            fx->vtable->set_scale(fx, &scale);
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(fx), true);
            fx->vtable->set_loop(fx, false);
            fx->vtable->set_speed(fx, 1.0f);
            fn_71011a9bb4(manager, fx);
            UnkBurstSpawned_lac44a4* spawned = reinterpret_cast<UnkBurstSpawned_lac44a4*>(fx);
            spawned->field_0x15a = false;
            spawned->field_0x108 = 1.0f;
            UnkXformNode* node = &reinterpret_cast<UnkBurstFx_lac44a4*>(fx)->node;
            {
                f32 push = self->push;
                UnkVec3f offset(push * dir->x, push * dir->y, push * dir->z);
                fn_7100208fa4(node, &offset);
            }
            UnkScaledRecord record;
            UnkVec3f origin(at->x + 0.0f, at->y + 0.0f, at->z + 20.0f);
            fn_710021688c(200.0f, &record, reinterpret_cast<const u32*>(&origin));
            fn_7100209394(node, &record);
            fn_7100216934(reinterpret_cast<char*>(&record));
        }
    }
    if (self->def->event.data != &g_unk_7101d47108->empty)
        fn_7100e038b0(g_unk_7101d47310, &self->def->event, reinterpret_cast<UnkVec*>(self->field_0x40), 0);
}
