// Unit 1102 (CXParasiteAIComponent's neighbourhood, tools/tu_map.py): fn_71006e73f4 picks one of four
// weighted choices at random (the fourth only while the player holds the item at the singleton's +0x4c8),
// fn_71006e75e8 toggles the shape's flag and, when turning on outside state 4, sets three slot values from
// the bindings of the current state before handing over to fn_71006e8208. Names and layouts are placeholders.
#include <stdlib.h>

#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CGunComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "types.h"
#include "game/UnkXParasiteAI.h"

struct UnkStruct_710083e9d4;
struct UnkItem_710083e9d4;

struct UnkXParasitePlayer_ovo0307 {
    u8 pad_0x0[0x2a8];
    UnkStruct_710083e9d4* guns;  // 0x2a8
};

struct UnkXParasitePlayerLink_ovo0307 {
    u8 pad_0x0[0x10];
    UnkXParasitePlayer_ovo0307* player;  // 0x10
};

static inline const u64* ItemId() {
    return reinterpret_cast<const u64*>(reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0x4c8);
}

u32 XParasiteAI_PickWeightedChoice(UnkXParasiteAI* self) {
    UnkXParasiteWeights* w = &self->weights;
    UnkItem_710083e9d4* item = nullptr;
    auto* link = *reinterpret_cast<UnkXParasitePlayerLink_ovo0307**>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x29a0);
    if (link && link->player) {
        if (link->player->guns)
            item = fn_710083e9d4(link->player->guns, ItemId(), false);
    }
    f32 total = 0.0f;
    if (w->mask & 4)
        total = w->weight_2 + 0.0f;
    if (w->mask & 2)
        total += w->weight_1;
    if (w->mask & 1)
        total += w->weight_0;
    if ((w->mask & 8) && item)
        total += w->weight_3;
    f32 pick = total * (f32)((f64)(rand() & 0x7fff) * (1.0 / 32767.0));
    f32 t = pick + 0.0f;
    f32 acc = 0.0f;
    if (w->mask & 4) {
        f32 v = w->weight_2;
        if (v > 0.0f) {
            if (!(t > v))
                return 1;
            acc = v + 0.0f;
        }
    }
    if (w->mask & 2) {
        f32 v = w->weight_1;
        if (v > 0.0f) {
            acc += v;
            if (!(t > acc))
                return 2;
        }
    }
    if (w->mask & 1) {
        f32 v = w->weight_0;
        if (v > 0.0f) {
            acc += v;
            if (!(t > acc))
                return 0;
        }
    }
    if (w->mask & 8) {
        f32 v = w->weight_3;
        acc += v;
        if (v > 0.0f && item != nullptr && !(t > acc))
            return 3;
    }
    return 0;
}

void XParasiteAI_SetEffectActive(UnkXParasiteAI* self, bool on);

void fn_71006e75e8(UnkXParasiteAI* self, bool on) {
    if (UnkXParasiteHolder* holder = ModelHolder(self->owner)) {
        if (holder->shape->field_0x11c)
            *reinterpret_cast<char*>(&holder->shape->field_0x114) = !on;
    }
    if (on) {
        if (self->field_0x14724 != 4) {
            if (UnkXParasiteHolder* holder = ModelHolder(self->owner)) {
                if (UnkXParasiteShape* shape = holder->shape) {
                    base::math::CVector4D* first = &CurrentBinding(self)->first;
                    fn_71001c2764(shape->slots, 0, 0, first, 1, true);
                    base::math::CVector4D* second = &CurrentBinding(self)->second;
                    fn_71001c2764(shape->slots, 0, 1, second, 1, true);
                    base::math::CVector4D zero = {};
                    fn_71001c2764(shape->slots, 0, 2, &zero, 1, true);
                }
            }
        }
    }
    XParasiteAI_SetEffectActive(self, on);
}
