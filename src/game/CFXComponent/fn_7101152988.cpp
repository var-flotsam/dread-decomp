// CFXComponent's unit: adds an effect under a name to the effect list at +0x50, dropping the one the name
// held (fn_7101153c6c) and growing the list when it is full (fn_71011579c8); an empty name releases the
// effect instead (fn_710115596c). Names and layouts are placeholders.
#include <new>

#include "game/CFXComponent.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkFXEffect_l152988;
struct UnkFXEffectVtable_l152988 {
    void* unk_0x0[12];
    bool (*is_visible)(UnkFXEffect_l152988* self);  // 0x60
};
struct UnkFXEffect_l152988 {
    UnkFXEffectVtable_l152988* vtable;
};

// What an entry keeps about its effect besides the effect itself.
struct UnkFXEffectInfo_l152988 {
    bool visible;       // 0x0
    bool on;            // 0x1
    u8 field_0x2;
    u8 field_0x3[5];    // unset in the original
    void* field_0x8;
};

struct UnkFXEffectEntry_l152988 {
    UnkFXEffect_l152988* effect;
    UnkFXEffectInfo_l152988 info;  // 0x8
    u8 pad_0x18[8];
};

struct UnkFXList_l1579c8;
void CFXComponent_ReserveEffectListCapacity(UnkFXList_l1579c8* self, u32 capacity);

struct UnkFXEffectList_l152988 {
    UnkFXEffectEntry_l152988* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;

    void Add(const CStrId& key, UnkFXEffect_l152988* effect, const UnkFXEffectInfo_l152988& info) {
        if (count == capacity)
            CFXComponent_ReserveEffectListCapacity(reinterpret_cast<UnkFXList_l1579c8*>(this), count + step);
        new (&keys[count]) CStrId(key);
        UnkFXEffectEntry_l152988& entry = values[count];
        entry.effect = effect;
        entry.info = info;
        ++count;
    }
    UnkFXEffectEntry_l152988* End() { return &values[count]; }
    UnkFXEffectEntry_l152988* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i].data == key.data)
                return &values[i];
        return End();
    }
};

struct UnkFXComponent_l152988 {
    u8 pad_0x0[0x50];
    UnkFXEffectList_l152988 effects;  // 0x50
};

void CFXComponent_AddNamedEffect(UnkRinkaControl_ovb0090* control, UnkFxObject_ovb0041* fx, UnkRinkaEntry_ruc** table,
                   void* a, u32 b) {
    UnkFXComponent_l152988* self = reinterpret_cast<UnkFXComponent_l152988*>(control);
    const CStrId& name = *reinterpret_cast<const CStrId*>(table);
    if (name.data == &g_unk_7101d47108->empty) {
        fn_710115596c(reinterpret_cast<UnkRinkaControl_ruc*>(control), fx);
        return;
    }
    UnkFXEffectEntry_l152988* old = self->effects.Find(name);
    if (old != self->effects.End())
        fn_7101153c6c(self, old->effect);

    UnkFXEffect_l152988* effect = reinterpret_cast<UnkFXEffect_l152988*>(fx);
    UnkFXEffectInfo_l152988 info;
    info.on = b & 1;
    info.visible = effect->vtable->is_visible(effect);
    info.field_0x2 = 0;
    info.field_0x8 = a;

    self->effects.Add(name, effect, info);
}
