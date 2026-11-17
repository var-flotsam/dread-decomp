// CCharClassSetMaterialPropertyBonePositionEvent's unit (tools/tu_map.py, unit 1854): finds the target actor
// (the event's own when it names none), reads the named bone's position and writes it, with two weights, into
// the material table entry of each of the target's slots (or of the one named slot). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CFXComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU218.h"
#include "game/UnkTU253.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

struct UnkTable;
struct UnkKey;
void fn_7100926874(UnkTable* self, const UnkKey& key, u32 id, const CVector4D& a, const CVector4D& b, u32 kind,
                   u32 value, f32 weight);
bool fn_71001c1694(char* self, const void* name, f32* transform);
bool fn_71001c17e4(char* self, const void* name, f32* transform);
void* fn_71011986a8(void* gui, const CStrId* id);

static inline bool IsEmpty_lbd802c(const CStrId& id) {
    return &g_unk_7101d47108->empty == id.data;
}

// The key of a material table entry: a named slot, or the slot at an index when no name is given.
struct UnkMaterialKey_lbd802c {
    u64 field_0x0;
    CStrId field_0x8;
    u32 field_0x10;
    u32 field_0x14;
    UnkMaterialKey_lbd802c(u32 index) : field_0x0(0), field_0x8(g_unk_7101d470e0), field_0x10(index), field_0x14(1) {}
    UnkMaterialKey_lbd802c(CStrId name, u32 index) : field_0x0(0), field_0x8(name), field_0x10(index), field_0x14(0) {}
};

struct UnkSlotList_lbd802c {
    u8 pad_0x0[0x2c];
    u32 count;
};

struct UnkModel_lbd802c {
    u8 pad_0x0[0x78];
    UnkSlotList_lbd802c* slots;
};

struct UnkShape_lbd802c {
    u8 pad_0x0[0x138];
    UnkModel_lbd802c* model;
};

struct UnkActorSub_lbd802c {
    u8 pad_0x0[0x50];
    UnkShape_lbd802c* shape;
};

struct UnkActor_lbd802c {
    u8 pad_0x0[0x228];
    UnkActorSub_lbd802c* field_0x228;
    u8 pad_0x230[0x50];
    UnkWorld* world;  // 0x280
    UnkTable* materials;  // 0x288
};

struct UnkPartVtable_lbd802c {
    u8 pad_0x0[0xf0];
    u32 (*vf_0xf0)(UnkPart* self);
    u8 pad_0xf8[0x10];
    bool (*vf_0x108)(UnkPart* self, u64 hash, u32* slot);
};

static inline const UnkPartVtable_lbd802c* Vtable_lbd802c(UnkPart* part) {
    return *reinterpret_cast<UnkPartVtable_lbd802c**>(part);
}

struct UnkGuiRoot_lbd802c {
    u8 pad_0x0[0x58];
    void* field_0x58;
};

struct UnkEventContext_lbd802c {
    UnkActor_lbd802c* actor;
};

struct UnkSetMaterialEvent_lbd802c {
    u8 pad_0x0[0x10];
    CStrId bone;  // 0x10
    CStrId target;  // 0x18
    CStrId part;  // 0x20
    CStrId slot;  // 0x28
    u32 field_0x30;
    u32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 weight;  // 0x40
    bool field_0x44;
};

// The actor named `name`: through the GUI root when it has one, else through the game manager.
static inline void* FindActor_lbd802c(const CStrId& name) {
    auto* gui = static_cast<UnkGuiRoot_lbd802c*>(g_unk_7101d47330);
    if (gui->field_0x58)
        return fn_71011986a8(gui, &name);
    UnkGameManager* manager = g_unk_7101d47310;
    u64 id = name.data->Key();
    alignas(8) u32 flags = g_unk_71727bebb8;
    return fn_7100deba8c(manager, &id, &flags);
}

void fn_7100bd802c(UnkSetMaterialEvent_lbd802c* self, UnkEventContext_lbd802c* context) {
    UnkActor_lbd802c* actor = context->actor;
    if (!actor)
        return;
    UnkActor_lbd802c* target;
    if (IsEmpty_lbd802c(self->target))
        target = actor;
    else
        target = static_cast<UnkActor_lbd802c*>(FindActor_lbd802c(self->target));
    if (!target)
        return;

    UnkActorSub_lbd802c* sub = actor->field_0x228;
    UnkTable* table = target->materials;
    if (!sub || !table)
        return;
    UnkActorSub_lbd802c* targetSub = target->field_0x228;
    UnkWorld* world = target->world;
    if (!((targetSub && IsEmpty_lbd802c(self->part) && targetSub->shape) || (world && !IsEmpty_lbd802c(self->part))))
        return;

    UnkShape_lbd802c* shape = sub->shape;
    u32 count;
    if (IsEmpty_lbd802c(self->part)) {
        count = targetSub->shape->model->slots->count;
    } else {
        UnkPart* part = fn_7101152b90(world, self->part.data->Key());
        count = part ? Vtable_lbd802c(part)->vf_0xf0(part) : 0;
    }
    if (!shape)
        return;

    f32 m[16] = {};
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    if (self->field_0x44) {
        u64 bone = self->bone.data->Key();
        if (!fn_71001c17e4(reinterpret_cast<char*>(shape->model), &bone, m))
            return;
    } else {
        u64 bone = self->bone.data->Key();
        if (!fn_71001c1694(reinterpret_cast<char*>(shape->model), &bone, m))
            return;
    }
    CVector4D a = {m[3], m[7], m[11], self->field_0x38};
    CVector4D b = {m[3], m[7], m[11], self->field_0x3c};

    if (self->slot.data->String().length != 0) {
        u32 slot = static_cast<u32>(-1);
        bool found;
        if (UnkPart* part = fn_7101152b90(world, self->part.data->Key())) {
            found = Vtable_lbd802c(part)->vf_0x108(part, self->slot.data->Key(), &slot);
        } else {
            u64 key = self->slot.data->Key();
            found = fn_7100199d88(targetSub->shape->model->slots, &key, &slot);
        }
        if (!found)
            return;
        CVector4D colour = {1.0f, 1.0f, 1.0f, 1.0f};
        fn_71001c287c(shape->model, slot, self->field_0x30, &colour, self->field_0x34);
        if (IsEmpty_lbd802c(self->part)) {
            UnkMaterialKey_lbd802c key(slot);
            fn_7100926874(table, reinterpret_cast<const UnkKey&>(key), self->field_0x30, b, b, self->field_0x34, 0,
                          self->weight);
        } else {
            UnkMaterialKey_lbd802c key(self->part, slot);
            fn_7100926874(table, reinterpret_cast<const UnkKey&>(key), self->field_0x30, b, b, self->field_0x34, 0,
                          self->weight);
        }
        return;
    }

    for (u32 i = 0; i != count; ++i) {
        if (IsEmpty_lbd802c(self->part)) {
            UnkMaterialKey_lbd802c key(i);
            fn_7100926874(table, reinterpret_cast<const UnkKey&>(key), self->field_0x30, a, b, self->field_0x34, 0,
                          self->weight);
        } else {
            UnkMaterialKey_lbd802c key(self->part, i);
            fn_7100926874(table, reinterpret_cast<const UnkKey&>(key), self->field_0x30, a, b, self->field_0x34, 0,
                          self->weight);
        }
    }
}
