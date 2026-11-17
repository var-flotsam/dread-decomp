// CChozoRobotSoldierAIComponent unit: keeps a moving robot soldier from running into another registered
// one at nearly the same height (within 100): the horizontal speed at +0x6c of `arg` is cut so that it
// stops 201 short of the other, and copied to +0x60. Returns whether it was cut. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoRobotSoldierAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkEntity {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
};

struct UnkEntry {
    u8 pad_0x0[0x10];
    UnkEntity* field_0x10;
};

struct UnkEntryList {
    UnkEntry** data;
    u32 count;

    UnkEntry** begin() { return data; }
    UnkEntry** end() { return data + count; }
};

extern char g_unk_71726cca00[] __attribute__((visibility("hidden")));
extern __attribute__((visibility("hidden"))) base::reflection::CClass g_unk_71726cca50;  // its class

// Whether an object is a CChozoRobotSoldierAIComponent. The class getter runs for its registration;
// the class itself is compared.
inline bool IsSoldierAI_l4745a8(base::reflection::UnkReflObject* object) {
    const base::reflection::CClass* type = object->vf_0x0();
    fn_710046ea2c();
    return fn_710009d50c(type, &g_unk_71726cca50);
}

struct UnkSoldierMove_l4745a8 {
    u8 pad_0x0[0x60];
    f32 field_0x60;
    u8 pad_0x64[0x8];
    f32 speed;  // 0x6c
};

bool fn_71004745a8(base::reflection::UnkReflObject* component, void* arg) {
    UnkSoldierMove_l4745a8* move = static_cast<UnkSoldierMove_l4745a8*>(arg);
    UnkEntity* self = reinterpret_cast<UnkEntity*>(component);
    bool cut = false;
    if (move->speed == 0.0f)
        return cut;
    base::reflection::UnkReflObject* ai = static_cast<base::reflection::UnkReflObject*>(self->field_0x30->field_0x240);
    if (!ai || !IsSoldierAI_l4745a8(ai))
        return cut;
    {
        UnkEntryList* list = reinterpret_cast<UnkEntryList*>(
            fn_7100de94cc(g_unk_7101d47310, reinterpret_cast<const u64*>(g_unk_71726cca00)));
        for (UnkEntry* entry : *list) {
            UnkEntity* other = entry ? entry->field_0x10 : nullptr;
            if (other == reinterpret_cast<UnkEntity*>(ai))
                continue;
            UnkActor* me = self->field_0x30;
            f32 my_y = me->field_0x54;
            f32 other_y = other->field_0x30->field_0x54;
            f32 lo = my_y + -100.0f;
            f32 hi = my_y + 100.0f;
            if (!(hi >= other_y) || !(lo <= other_y))
                continue;
            f32 other_x = other->field_0x30->field_0x44;
            f32 my_x = me->field_0x44;
            f32 speed;
            if (my_x > other_x) {
                f32 v = move->speed;
                if (!(v < 0.0f))
                    continue;
                f32 overlap = (other_x + 201.0f) - (my_x + v);
                if (!(overlap > 0.0f))
                    continue;
                speed = v + overlap;
                speed = !(speed >= 0.0f) ? speed : 0.0f;
            } else if (my_x < other_x) {
                f32 v = move->speed;
                if (!(v > 0.0f))
                    continue;
                f32 overlap = (my_x + v) - (other_x - 201.0f);
                if (!(overlap > 0.0f))
                    continue;
                speed = v - overlap;
                speed = !(speed < 0.0f) ? speed : 0.0f;
            } else {
                continue;
            }
            move->speed = speed;
            move->field_0x60 = speed;
            cut = true;
        }
    }
    return cut;
}
