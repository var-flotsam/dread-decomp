// CPickableSpringBallComponent's unit (tools/tu_map.py unit 1494): after the base step (fn_71009d2558),
// when the pickable names an item (+0x98) with a non-zero amount (+0xa8) and the player's inventory
// (fn_7100df55e4) already holds some of it (fn_7101142928), the owner's entity state is switched off
// (fn_7100232864). Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkInventory;
f32 fn_7101142928(UnkInventory*, const UnkQualifiedId&);
void* fn_7100df55e4(UnkGameManager*);
struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
struct UnkEntityState;
void fn_7100232864(UnkEntityState*, bool);

struct UnkPickableSpringBall_ov110 {
    u8 pad_0x0[0x30];
    UnkEntityState* owner;  // 0x30
    u8 pad_0x38[0x60];
    CStrId item;  // 0x98
    u8 pad_0xa0[0x8];
    f32 amount;  // 0xa8
};

void fn_71009d2558(UnkPickableSpringBall_ov110* self);

void fn_71009d5260(UnkPickableSpringBall_ov110* self) {
    fn_71009d2558(self);
    if (self->item.data == &g_unk_7101d47108->empty)
        return;
    if (fabsf(self->amount) > 1.1920929e-7f) {
        UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
        if (!inventory)
            return;
        alignas(UnkQualifiedId) char storage[sizeof(UnkQualifiedId)];
        UnkQualifiedId& id = *reinterpret_cast<UnkQualifiedId*>(storage);
        fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(&id), self->item);
        f32 held = fn_7101142928(inventory, id);
        id.id.~CStrId();
        if (held > 0.0f)
            fn_7100232864(self->owner, false);
    }
}
