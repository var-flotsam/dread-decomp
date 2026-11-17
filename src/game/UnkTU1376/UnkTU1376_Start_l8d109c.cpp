// CDoorLifeComponent's unit (tools/tu_map.py unit 1376; the name is a hint): starts the door: runs the base
// step, hands the owner's name to the registry at +0x660, enters state 2, registers with the game manager under
// g_unk_7172731758 and joins the target's two deferred sets. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU372.h"
#include "types.h"

using base::global::CStrId;

struct UnkDoorOwner_l8d109c {
    u8 pad_0x0[0x30];
    CStrId name;
};

typedef base::UnkDeferredSet<void> UnkDoorSet_l8d109c;

struct UnkDoorTarget_l8d109c {
    u8 pad_0x0[0x48];
    UnkDoorSet_l8d109c first;
    UnkDoorSet_l8d109c second;
};

struct UnkDoorLife_l8d109c {
    u8 pad_0x0[0x18];
    UnkDoorOwner_l8d109c* owner;
    u8 pad_0x20[0x30 - 0x20];
    UnkWeakTarget* target;
    u8 pad_0x38[0x420 - 0x38];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x4d0[0x624 - 0x4d0];
    f32 field_0x624;
    u8 pad_0x628[0x660 - 0x628];
    u8 registry[0x9c8 - 0x660];
    UnkDoorSet_l8d109c* first_set;
    u8 pad_0x9d0[0x9e0 - 0x9d0];
    UnkDoorSet_l8d109c* second_set;
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d109c, machine) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d109c, second_set) == 0x9e0, "layout");

void fn_71008e087c(char*);
void fn_71008d1294(void* registry, CStrId name);
void fn_71008dd1d0(UnkDoorSet_l8d109c* set, void* item);
void fn_71008dd384(UnkDoorSet_l8d109c* set, void* item);
extern base::UnkStrIdMember g_unk_7172731758 __attribute__((visibility("hidden")));

void fn_71008d109c(UnkDoorLife_l8d109c* self) {
    fn_71008e087c(reinterpret_cast<char*>(self));
    fn_71008d1294(self->registry, self->owner->name);
    RequestState_A5_b73<15, 0x7fffffff>(&self->machine, 2);
    fn_7100de9540(g_unk_7101d47310, reinterpret_cast<const u64*>(&g_unk_7172731758),
                  reinterpret_cast<UnkWeakTarget*>(self));
    self->field_0x624 = -1.0f;
    auto* target = static_cast<UnkDoorTarget_l8d109c*>(fn_71002b3b84(self->target));
    if (target) {
        if (self->first_set) {
            fn_71008dd384(self->first_set, &self->first_set);
            self->first_set = nullptr;
        }
        self->first_set = &target->first;
        fn_71008dd1d0(self->first_set, &self->first_set);
        if (self->second_set) {
            fn_71008dd384(self->second_set, &self->second_set);
            self->second_set = nullptr;
        }
        self->second_set = &target->second;
        fn_71008dd1d0(self->second_set, &self->second_set);
    }
}
