// A central unit component step (tools/tu_map.py: the CCentralUnitComponent unit): on the first pass
// (state 1 at +0x528) takes the handle fn_7100353be8 builds from +0x500 into the slot at +0x530 and
// moves to state 2; when the slot's target has an entity with a door, calls fn_71008ca700 on its
// +0x30 object's part with 1; runs the unit's update with -1; then, in mode 1 (+0x3f0), calls the scenario's
// "OnProtoEmmyWalkThroughCentralUnitDoor" script function, else sets the entity at +0x6d8 (its +0x10)
// active. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "game/CDoorCentralUnitLifeComponent.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU341.h"
#include "game/UnkTU367.h"
#include "types.h"

struct lua_State;

struct UnkWeakTarget;
struct UnkDoorLock;
struct UnkEntityState;

using UnkRef = base::UnkRefPtrMember;
UnkRef fn_7100353be8(void* source);

struct UnkCentralUnitDoor_b46a2;
UnkCentralUnitDoor_b46a2* fn_71004056fc(void* object);

struct UnkCentralUnitLink_b46a2 {
    u8 pad_0x0[0x10];
    UnkEntityState* entity;
};

struct UnkCentralUnit_b46a2 {
    u8 pad_0x0[0x3f0];
    u32 mode;
    u8 pad_0x3f4[0x500 - 0x3f4];
    u8 field_0x500[0x28];
    u32 state;
    u8 pad_0x52c[4];
    base::UnkRefHandle target;
    u8 pad_0x538[0x6d8 - 0x538];
    UnkCentralUnitLink_b46a2* link;
};

u32 fn_7100776248(UnkCentralUnit_b46a2* self) {
    if (self->state == 1) {
        {
            base::UnkRefPtrMember handle = fn_7100353be8(self->field_0x500);
            fn_710031d208(&self->target, reinterpret_cast<base::UnkRefHandle*>(&handle));
        }
        self->state = 2;
    }
    if (base::UnkRefCounted* target = self->target.ptr) {
        if (*reinterpret_cast<void**>(reinterpret_cast<char*>(target) + 0x10)) {
            asm("" : "+r"(target));
            UnkWeakTarget* entity = *reinterpret_cast<UnkWeakTarget**>(reinterpret_cast<char*>(target) + 0x10);
            if (UnkDoorLock* door = fn_71002af860(entity)) {
                if (UnkCentralUnitDoor_b46a2* d = fn_71004056fc(*reinterpret_cast<void**>(reinterpret_cast<char*>(door) + 0x30)))
                    fn_71008ca700(d, 1);
            }
        }
    }
    fn_71007722f4(self, -1.0f);
    if (self->mode == 1) {
        if (fn_71010c0bb0(g_unk_7172a01d18, "CurrentScenario.OnProtoEmmyWalkThroughCentralUnitDoor")) {
            char out[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 0, out, 0x100, nullptr, nullptr);
        }
    } else {
        fn_7100232864(self->link ? self->link->entity : nullptr, true);
    }
    return 1;
}
