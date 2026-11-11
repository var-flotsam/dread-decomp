// CQuetzoaAIComponent: after the base step, sends the owner's target object (+0x298) a query whose
// direction is +1 or -1 along x, the sign of the third element of the owner's matrix.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "game_s2/UnkParams.h"
#include "types.h"

extern void* g_unk_7101a24b58[];

struct UnkQuery {
    void** vtable;
    f32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkQuery() {
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(&field_0x8, 0, 13);
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
    }
};

struct UnkQuetzoaAIComponent {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_710064a838(UnkQuetzoaAIComponent* self, void* arg) {
    fn_7101108124(self, arg);
    UnkActor* owner = self->owner;
    void* target = owner->field_0x298;
    if (!target)
        return;
    UnkQuery query;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    query.field_0x8 = m.m[2] >= 0.0f ? 1.0f : -1.0f;
    query.field_0xc = 0;
    query.field_0x10 = 0;
    query.field_0x14 = 1;
    fn_71006f4b0c(target, &query, 0);
}
