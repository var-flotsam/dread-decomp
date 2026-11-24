// CFrozenAsFrostbiteComponent's unit (tools/tu_map.py): the frostbite component's start. Takes the game
// manager's object (+0x1c28) into the slot at +0x3c8, tints the actor's slots with the tunable's colour,
// clears the delays, hands the old objects back to the manager and fetches one per part of the definition
// (scaled by the part's value), places them, sorts them by the actor's longer side, shows an effect on the
// objects a third and two thirds through the list and runs the frozen component's start. Names and layouts
// are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector4D.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CFXComponent.h"
#include "game/CFrozenAsFrostbiteComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkFxObjectVtable_ovb0041.h"
#include "game/UnkFxObject_ovb0041.h"
#include "game/UnkGameManager.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU127.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU284.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkStruct_71001436f4;
struct UnkSlotOwner_b0025;
struct UnkRinkaControl_ovb0090;
struct UnkRinkaEntry_ruc;

extern const u8 g_unk_7172720e20[] __attribute__((visibility("hidden")));
extern const u8 g_unk_7172720e40[] __attribute__((visibility("hidden")));
extern const u8 g_unk_7172720e48[] __attribute__((visibility("hidden")));

void fn_71007f5db4(void* object, void* slot);
void fn_71007f5f44(void* object, void* slot);
void fn_7100da8f00(UnkSlotOwner_b0025* self, UnkSwitchObject* object);
UnkSwitchObject* fn_7100daaf3c(UnkManager_71726af468* self, const u64* key, bool flag);
void fn_7100146380(UnkSwitchObject* self);
void fn_71001463b0(UnkSwitchObject* self);
UnkSwitchObject*& fn_71007f4d8c(base::global::CRntVector<UnkSwitchObject*>* self, u32 index);
void fn_71007f4514(struct UnkFrozenAsFrostbite_l7f48d8* self);
void fn_71007f6100(UnkSwitchObject** first, UnkSwitchObject** last);
void fn_71007f6b90(UnkSwitchObject** first, UnkSwitchObject** last);
struct UnkFrozenAsFrostbite_l7f48d8;
void fn_71007fa0c4(UnkFrozenAsFrostbite_l7f48d8* self, u32 arg);

// The bounds of the actor's collision, returned in memory.
struct UnkBounds_l7f48d8 {
    f32 min_x, min_y, max_x, max_y;
    UnkBounds_l7f48d8() {}
    UnkBounds_l7f48d8(const UnkBounds_l7f48d8& o) : min_x(o.min_x), min_y(o.min_y), max_x(o.max_x), max_y(o.max_y) {}
};
UnkBounds_l7f48d8 fn_71008653c0(UnkCollisionContainer* container, u32 a, u32 b);

struct UnkFrostbitePartName_l7f48d8 {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkFrostbitePart_l7f48d8 {
    u8 pad_0x0[0x8];
    UnkFrostbitePartName_l7f48d8* name;  // 0x8
    u8 pad_0x10[0x8];
    f32 scale;  // 0x18
};

struct UnkFrostbiteDef_l7f48d8 {
    u8 pad_0x0[0xc8];
    UnkFrostbitePart_l7f48d8** parts;  // 0xc8
    u32 partCount;                     // 0xd0
};

struct UnkFrostbiteSlotsOwner_l7f48d8 {
    u8 pad_0x0[0x50];
    UnkStruct_71001436f4* slots;
};

struct UnkFrostbiteActor_l7f48d8 {
    u8 pad_0x0[0x228];
    UnkFrostbiteSlotsOwner_l7f48d8* field_0x228;
    u8 pad_0x230[0x280 - 0x230];
    UnkRinkaControl_ovb0090* field_0x280;
    u8 pad_0x288[0x2a0 - 0x288];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkFrostbiteGame_l7f48d8 {
    u8 pad_0x0[0x1c28];
    u8 field_0x1c28[1];
};

struct UnkFrostbiteTunable_l7f48d8 {
    u8 pad_0x0[0x24];
    f32 r, g, b;   // 0x24
    f32 alphaMin;  // 0x30
    f32 alphaMax;  // 0x34
};

struct UnkFrozenAsFrostbite_l7f48d8 {
    u8 pad_0x0[0x30];
    UnkFrostbiteActor_l7f48d8* actor;  // 0x30
    u8 pad_0x38[0x3b0 - 0x38];
    UnkFrostbiteDef_l7f48d8* def;  // 0x3b0
    u8 pad_0x3b8[0x3c4 - 0x3b8];
    f32 field_0x3c4;
    void* slot;  // 0x3c8
    u8 pad_0x3d0[0x3e0 - 0x3d0];
    f32 field_0x3e0;
    u8 pad_0x3e4[0x4];
    base::global::CRntVector<UnkSwitchObject*> objects;  // 0x3e8
};
static_assert(__builtin_offsetof(UnkFrozenAsFrostbite_l7f48d8, objects) == 0x3e8, "layout");

static inline f32 NonNegative(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

// Tints the actor's slots with the tunable's frost colour, its alpha at `ratio` between the two ends.
static inline void Tint(UnkFrozenAsFrostbite_l7f48d8* self, f32 ratio) {
    UnkStruct_71001436f4* slots = self->actor->field_0x228->slots;
    const UnkFrostbiteTunable_l7f48d8* tunable = reinterpret_cast<const UnkFrostbiteTunable_l7f48d8*>(&g_unk_7172720dc0);
    f32 alpha = tunable->alphaMin + ratio * (tunable->alphaMax - tunable->alphaMin);
    base::math::CVector4D colour = {NonNegative(tunable->r), NonNegative(tunable->g), NonNegative(tunable->b),
                                    NonNegative(alpha)};
    fn_71001436f4(slots, 2, &colour, 1);
}

// The manager, created on first use.
static inline UnkManager_71726af468* Manager() {
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    return g_unk_71726af468;
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// Hands every object back to the manager and empties the list.
static inline void ReleaseObjects(UnkFrozenAsFrostbite_l7f48d8* self) {
    for (UnkSwitchObject** it = self->objects.data, **end = it + self->objects.count; it != end; ++it) {
        if (UnkSwitchObject* object = *it)
            fn_7100da8f00(reinterpret_cast<UnkSlotOwner_b0025*>(Manager()), object);
    }
    self->objects.count = 0;
}

// Shows an effect from `path` on the object at `index`, taking its position.
static inline void ShowEffect(UnkFrozenAsFrostbite_l7f48d8* self, UnkRinkaControl_ovb0090* control, u32 index,
                              const u8* path, const u8* table) {
    UnkFxObject_ovb0041* fx = fn_71003b7db0(ModelManager(), path, false, true);
    if (!fx)
        return;
    if (!fn_71007f4d8c(&self->objects, index))
        return;
    fn_7101152988(control, fx, reinterpret_cast<UnkRinkaEntry_ruc**>(const_cast<u8*>(table)), nullptr, 0);
    const f32* m = reinterpret_cast<const f32*>(fn_71007f4d8c(&self->objects, index));
    UnkVec3f position(m[0x4c / 4], m[0x5c / 4], m[0x6c / 4]);
    fx->vtable->set_position(fx, &position);
    fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(fx), true);
}

void fn_71007f48d8(UnkFrozenAsFrostbite_l7f48d8* self, u32 arg) {
    UnkFrostbiteGame_l7f48d8* game = reinterpret_cast<UnkFrostbiteGame_l7f48d8*>(g_unk_7101d47310);
    void** slot = &self->slot;
    if (*slot) {
        fn_71007f5db4(*slot, slot);
        *slot = nullptr;
    }
    *slot = game->field_0x1c28;
    fn_71007f5f44(*slot, slot);
    Tint(self, 1.0f);
    self->field_0x3e0 = 0.0f;
    self->field_0x3c4 = 0.0f;
    ReleaseObjects(self);
    UnkFrostbiteDef_l7f48d8* def = self->def;
    for (UnkFrostbitePart_l7f48d8** part = def->parts, **end = part + def->partCount; part != end; ++part) {
        UnkManager_71726af468* manager = Manager();
        alignas(16) u64 key = (*part)->name->key;
        UnkSwitchObject* object = fn_7100daaf3c(manager, &key, false);
        if (!object)
            continue;
        self->objects.PushBack(object);
        f32 scale = (*part)->scale;
        UnkVec3f size(scale, scale, scale);
        fn_7100207be8(reinterpret_cast<UnkXformNode*>(object->field_0x28), &size);
        fn_7100146380(object);
        fn_71001463b0(object);
    }
    fn_71007f4514(self);
    UnkFrostbiteActor_l7f48d8* actor = self->actor;
    if (UnkCollisionContainer* container = actor->field_0x2a0) {
        if (UnkRinkaControl_ovb0090* control = actor->field_0x280) {
            UnkBounds_l7f48d8 bounds = fn_71008653c0(container, 0x80, 0);
            UnkSwitchObject** first = self->objects.data;
            UnkSwitchObject** last = first + self->objects.count;
            if (bounds.max_y - bounds.min_y > bounds.max_x - bounds.min_x)
                fn_71007f6100(first, last);
            else
                fn_71007f6b90(first, last);
            u32 third = self->objects.count / 3;
            ShowEffect(self, control, third, g_unk_7172720e20, g_unk_7172720e40);
            ShowEffect(self, control, third * 2, g_unk_7172720e20, g_unk_7172720e48);
        }
    }
    fn_71007fa0c4(self, arg);
}
