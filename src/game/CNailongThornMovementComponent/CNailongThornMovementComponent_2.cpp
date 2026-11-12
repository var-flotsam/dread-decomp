// CNailongThornMovementComponent: fn_71006238ec sets up a damage request (fn_71008c1538) with one of
// two tunable values of g_unk_7172716160 and copies the matching unit-static id into the request's
// id at +0xb8, picked by the word at +0x5dc. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;

struct UnkNailongThornRequest {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
};

struct UnkNailongThornMovement2 {
    u8 pad_0x0[0x5dc];
    u32 field_0x5dc;
};

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726f8138 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f8140 __attribute__((visibility("hidden")));

static inline f32 TunableValue(u32 offset) {
    return *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + offset);
}

void fn_71006238ec(UnkNailongThornMovement2* self, UnkNailongThornRequest* request) {
    if (self->field_0x5dc) {
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), TunableValue(0x420));
        request->id = g_unk_71726f8140;
    } else {
        fn_71008c1538(reinterpret_cast<UnkRequest*>(request), TunableValue(0x29c));
        request->id = g_unk_71726f8138;
    }
}
