// CScorpiusDefensiveSpikeBallPrickAttack (reflection: size 0x58): its move assignment (fn_7100687848). It copies the plain
// words first and tests for self-assignment only before sharing the listener owner's weak handle.
// Near copy of fn_71006338dc (src/game/CPoisonFlyDiveAttack/CPoisonFlyDiveAttack.cpp).
#include "game_s2/UnkWeakHandle.h"

struct UnkBlock32_7100687848 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkWords_7100687848 {
    u64 field_0x0;
    u64 field_0x8;
};

struct CScorpiusDefensiveSpikeBallPrickAttack_View {
    void** vtable;
    UnkBlock32_7100687848 field_0x8;
    u64 field_0x28;
    u64 field_0x30;
    void** listener_vtable;
    UnkWords_7100687848 field_0x40;
    UnkWeakHandle* field_0x50;
};

CScorpiusDefensiveSpikeBallPrickAttack_View* fn_7100687848(CScorpiusDefensiveSpikeBallPrickAttack_View* self, const CScorpiusDefensiveSpikeBallPrickAttack_View* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x40 = other->field_0x40;
    if (self != other)
        CopyWeakRef(&self->field_0x50, other->field_0x50);
    return self;
}
