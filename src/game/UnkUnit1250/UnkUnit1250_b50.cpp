// Unit 1250 of tools/tu_map.py (no class name). fn_71007df12c opens or closes a door: the "door"
// collision object of the owner's (+0x30) collision container (+0x2a0) gets the opposite of `open`
// (fn_7100d35a98), and the owner's object (fn_71002b3b84) gets one of two ids (fn_71009c0f1c).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);

struct UnkDoorOwner_b50 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkDoor_b50 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_b50* owner;
};

static inline UnkCollisionObject* Find_b50(UnkCollisionContainer* collision, const CStrId& name) {
    return fn_71008640a4(collision, &name);
}

void fn_71007df12c(UnkDoor_b50* self, bool open) {
    if (UnkCollisionContainer* collision = self->owner->collision) {
        UnkCollisionObject* object = Find_b50(collision, CStrId("door", true));
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), !open);
    }
    if (void* object = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(object, open ? 0x6909d27d2c787fbaull : 0xe43bbd07b9e307b9ull);
}
