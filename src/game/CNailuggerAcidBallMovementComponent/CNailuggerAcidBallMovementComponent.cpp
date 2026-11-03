// CNailuggerAcidBallMovementComponent: fn_7100626c90 sets up a damage request (fn_71008c1538) with the
// tunable value at +0x41c of g_unk_7172716160 and copies a unit-static id into the request's id at
// +0xb8, like fn_71006238ec (CNailongThornMovementComponent_2.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;

struct UnkNailuggerAcidBallRequest {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
};

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726f8838 __attribute__((visibility("hidden")));
void fn_71008c1538(UnkRequest* request, f32 value);

void fn_7100626c90(void* self, UnkNailuggerAcidBallRequest* request) {
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0x41c));
    request->id = g_unk_71726f8838;
}
