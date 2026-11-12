// CChozoCommanderSentenceSphereMovementComponent's unit (the name from reflection): with the movement
// component as second argument, sets its state (+0xc8) to 1, gives its damage request the tunable's value
// (+0xc0 of g_unk_7172716160, fn_71008c1538) and copies the unit's static string id into +0xb8.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTU1370.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726ca0f0 __attribute__((visibility("hidden")));

struct UnkSentenceSphereMovement_A2_b70 {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 state;  // 0xc8
};

void fn_710045c930(void*, UnkSentenceSphereMovement_A2_b70* self) {
    self->state = 1;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self),
                  *reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + 0xc0));
    self->id = g_unk_71726ca0f0;
}
