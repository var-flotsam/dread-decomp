// CEmmyWakeUpComponent: a message handler. For kind 1 of the message type with hash
// 0xf4061ff25a2163d6 it resolves the lazily resolved handle at +0xb8 (source at +0x88, state at +0xb0:
// 1 until the handle takes its value, 2 after; as
// src/game/CStartCentralUnitCombatLogicAction/CStartCentralUnitCombatLogicAction_b49.cpp) and, when the
// actor it names is a central unit (fn_71007d74b0), calls its vtable slot 0x128 and clears the state of
// the entity at +0x30 (fn_7100232864); it returns false then, true otherwise. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "game/CEmmyWakeUpComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCentralUnitProto;
struct UnkEntityState;

struct UnkEmmyWakeUpMessageType {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkEmmyWakeUpMessage {
    u32 kind;
    u8 pad_0x4[0xc];
    UnkEmmyWakeUpMessageType* type;
};

struct UnkEmmyWakeUpTarget : base::UnkRefCounted {
    void* field_0x10;
};

struct UnkEmmyWakeUpComponent {
    u8 pad_0x0[0x30];
    UnkEntityState* entity;
    u8 pad_0x38[0x50];
    u8 source[0x28];  // 0x88
    u32 state;        // 0xb0
    u32 pad_0xb4;
    base::UnkRefHandle handle;  // 0xb8
};

typedef void (*UnkEmmyWakeUpCall)(UnkCentralUnitProto* unit);

bool fn_71007d73cc(UnkEmmyWakeUpComponent* self, const UnkEmmyWakeUpMessage* msg) {
    if (msg->type->hash != 0xf4061ff25a2163d6ull || msg->kind != 1)
        return true;
    if (self->state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(self->source);
            fn_7100234f78(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        self->state = 2;
    }
    auto* target = static_cast<UnkEmmyWakeUpTarget*>(self->handle.ptr);
    if (!target || !target->field_0x10)
        return true;
    UnkCentralUnitProto* unit = fn_71007d74b0(*reinterpret_cast<void* volatile*>(&target->field_0x10));
    if (!unit)
        return true;
    (*reinterpret_cast<UnkEmmyWakeUpCall**>(unit))[0x128 / 8](unit);
    if (self->entity)
        fn_7100232864(self->entity, false);
    return false;
}
