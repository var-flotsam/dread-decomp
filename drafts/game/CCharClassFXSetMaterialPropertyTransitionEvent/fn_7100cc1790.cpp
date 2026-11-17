// CCharClassFXSetMaterialPropertyTransitionEvent's unit (tools/tu_map.py unit 2253): applies the event's colour
// transition to one named slot, or every slot, of the actor's effect part through the material table
// (fn_7100926874), or hands the event to the effects component when the actor has no table. Names and layouts are
// placeholders.
#include "base/color/CColor4F.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CFXComponent.h"
#include "types.h"

using base::color::CColor4F;
using base::global::CStrId;

struct UnkTable;
struct UnkKey;
void fn_7100926874(UnkTable* self, const UnkKey& key, u32 id, const base::math::CVector4D& a,
                   const base::math::CVector4D& b, u32 kind, u32 value, f32 weight);

struct UnkFXComponent_ovb51;
struct UnkFXMessage_ovb51;
void fn_7101156238(UnkFXComponent_ovb51* self, UnkFXMessage_ovb51* msg);

// A material slot's two colour sets (fn_7100926874.cpp's layout).
struct UnkMaterialSlots_lcc1790 {
    u8 pad_0x0[0x14c];
    base::math::CVector4D field_0x14c[10];
    u8 pad_0x1ec[0x54];
    base::math::CVector4D field_0x240[10];
};
static_assert(__builtin_offsetof(UnkMaterialSlots_lcc1790, field_0x240) == 0x240, "layout");

struct UnkMaterialPart_lcc1790 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual u32 SlotCount();                                    // 0xf0
    virtual UnkMaterialSlots_lcc1790* Slot(u32 index);          // 0xf8
    virtual void vf_0x100();
    virtual bool FindSlot(u64 hash, u32* index);                // 0x108
};

// The table key (fn_7100926874.cpp's UnkKey).
struct UnkMaterialKey_lcc1790 {
    u64 field_0x0;
    CStrId name;
    u32 index;
    u32 field_0x14;
    UnkMaterialKey_lcc1790(CStrId name_, u32 index_) : field_0x0(0), name(name_), index(index_), field_0x14(0) {}
};

struct UnkActor_lcc1790 {
    u8 pad_0x0[0x280];
    UnkFXComponent_ovb51* fx;  // 0x280
    UnkTable* table;           // 0x288
};

struct UnkTarget_lcc1790 {
    UnkActor_lcc1790* actor;
};

struct UnkEvent_lcc1790 {
    u8 pad_0x0[0x10];
    CStrId part;      // 0x10
    CStrId slot;      // 0x18
    u32 id;           // 0x20
    u32 kind;         // 0x24
    CColor4F color;   // 0x28
    f32 intensity;    // 0x38
    f32 weight;       // 0x3c
};

static inline const UnkKey& AsKey(const UnkMaterialKey_lcc1790& key) {
    return reinterpret_cast<const UnkKey&>(key);
}

static inline f32 NonNegative(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

static inline CColor4F Clamped(const base::math::CVector4D& v) {
    return CColor4F(NonNegative(v.x), NonNegative(v.y), NonNegative(v.z), NonNegative(v.w));
}

// The slot's current colour of the event's kind, made non-negative (opaque black for another kind).
static inline void CurrentColor(const UnkEvent_lcc1790* self, const UnkMaterialSlots_lcc1790* slots, CColor4F& from) {
    switch (self->kind) {
    case 0:
        from = Clamped(slots->field_0x240[self->id]);
        break;
    case 1:
        from = Clamped(slots->field_0x14c[self->id]);
        break;
    }
}

void fn_7100cc1790(UnkEvent_lcc1790* self, UnkTarget_lcc1790* target) {
    UnkActor_lcc1790* actor = target->actor;
    if (!actor)
        return;
    UnkTable* table = actor->table;
    UnkFXComponent_ovb51* fx = actor->fx;
    if (!table || !fx) {
        if (fx)
            fn_7101156238(fx, reinterpret_cast<UnkFXMessage_ovb51*>(self));
        return;
    }
    auto* part = reinterpret_cast<UnkMaterialPart_lcc1790*>(
        fn_7101152b90(reinterpret_cast<UnkWorld*>(fx), self->part.data->Key()));
    if (!part)
        return;
    f32 k = self->intensity;
    CColor4F color(NonNegative(self->color.r * k), NonNegative(self->color.g * k), NonNegative(self->color.b * k),
                   NonNegative(self->color.a));
    if (self->slot.data->String().length != 0) {
        u32 index = -1;
        if (!part->FindSlot(self->slot.data->Key(), &index))
            return;
        UnkMaterialSlots_lcc1790* slots = part->Slot(index);
        CColor4F from(0.0f, 0.0f, 0.0f, 1.0f);
        CurrentColor(self, slots, from);
        fn_7100926874(table, AsKey(UnkMaterialKey_lcc1790(self->part, index)), self->id, fn_7100030878(&from),
                      fn_7100030878(&color), self->kind, 0, self->weight);
        return;
    }
    for (u32 i = 0; i < part->SlotCount(); ++i) {
        if (UnkMaterialSlots_lcc1790* slots = part->Slot(i)) {
            CColor4F from(0.0f, 0.0f, 0.0f, 1.0f);
            CurrentColor(self, slots, from);
            fn_7100926874(table, AsKey(UnkMaterialKey_lcc1790(self->part, i)), self->id, fn_7100030878(&from),
                          fn_7100030878(&color), self->kind, 0, self->weight);
        }
    }
}
