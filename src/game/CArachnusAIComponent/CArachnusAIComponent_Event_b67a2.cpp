// CArachnusAIComponent unit (tools/tu_map.py, the name from reflection): an event result. The base handler's
// (fn_7101105e44) when non-zero; else 2 when the event's flags (+0xb4) have none of bits 1-3; else, with the
// owner's (+0x30) collision container (+0x2a0), 2 when the id it maps the event's +0x60 to is the one at
// 0x71726bca40, otherwise 0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCollisionComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CSluggerAIComponent/CSluggerAIComponent.cpp and src/game/CScorpiusAIComponent/CScorpiusAIComponent_b51.cpp
// declare them.
struct UnkSluggerAI;
struct UnkSluggerEvent;
struct UnkCollisionContainer;

extern CStrId g_unk_71726bca40 __attribute__((visibility("hidden")));

struct UnkArachnusOwner_b67a2 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkArachnusAI_b67a2 {
    u8 pad_0x0[0x30];
    UnkArachnusOwner_b67a2* owner;
};

struct UnkArachnusEvent_b67a2 {
    u8 pad_0x0[0x60];
    void* field_0x60;
    u8 pad_0x68[0xb4 - 0x68];
    u8 flags;
};

u32 fn_71003b2090(UnkArachnusAI_b67a2* self, UnkArachnusEvent_b67a2* event) {
    u32 result = fn_7101105e44(reinterpret_cast<UnkSluggerAI*>(self), reinterpret_cast<UnkSluggerEvent*>(event));
    if (result != 0)
        return result;
    if ((event->flags & 0xe) == 0)
        return 2;
    UnkCollisionContainer* collision = self->owner->collision;
    if (collision == nullptr)
        return result;
    CStrId* id = fn_7100864018(collision, event->field_0x60);
    return (id->data == g_unk_71726bca40.data) << 1;
}
