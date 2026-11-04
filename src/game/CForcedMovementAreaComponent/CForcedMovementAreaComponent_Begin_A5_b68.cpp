// CForcedMovementAreaComponent's unit: with the object at +0x200 of the argument, runs fn_71010dae74 on it when
// fn_71007ec92c of +0x18 holds, hands it the hidden object at 0x717271fdd0 (fn_71010db850), resets +0xa0 to the
// constant CVector2D, runs fn_71010dae74 again, then gives the player's +0x1d8 object the hidden object at
// 0x717271fdc8 (fn_7100872858). Names are placeholders.
#include "base/math/CVector.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
extern const base::math::CVector2D g_unk_7101647780;

struct UnkForcedMovementTarget_A5_b68;
void* fn_71007ec92c(void* p);
void fn_71010dae74(UnkForcedMovementTarget_A5_b68* p);
void fn_71010db850(UnkForcedMovementTarget_A5_b68* p, void* q);
void fn_7100872858(void* p, void* q);
extern u8 g_unk_717271fdc8[] __attribute__((visibility("hidden")));
extern u8 g_unk_717271fdd0[] __attribute__((visibility("hidden")));

struct UnkForcedMovementArea_A5_b68 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x80];
    base::math::CVector2D field_0xa0;
};

struct UnkForcedMovementArg_A5_b68 {
    u8 pad_0x0[0x200];
    UnkForcedMovementTarget_A5_b68* field_0x200;
};

void fn_71007ec5e4(UnkForcedMovementArea_A5_b68* self, UnkForcedMovementArg_A5_b68* arg) {
    UnkForcedMovementTarget_A5_b68* target = arg->field_0x200;
    void* r = fn_71007ec92c(self->field_0x18);
    if (target && r)
        fn_71010dae74(target);
    fn_71010db850(target, g_unk_717271fdd0);
    self->field_0xa0.x = g_unk_7101647780.x;
    self->field_0xa0.y = g_unk_7101647780.y;
    if (target)
        fn_71010dae74(target);
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (player && *reinterpret_cast<void**>(reinterpret_cast<char*>(player) + 0x1d8))
        fn_7100872858(*reinterpret_cast<void**>(reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310)) + 0x1d8),
                      g_unk_717271fdc8);
}
