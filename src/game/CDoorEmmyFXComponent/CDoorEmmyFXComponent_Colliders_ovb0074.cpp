// CDoorEmmyFXComponent's unit (tools/tu_map.py): shows or hides the door's colliders. The owner's
// collision component (fn_710043acb4 on +0x30) has its "door" collider set to the opposite of `on`
// and its "doorcooldown" collider set to `on` (fn_71008640a4, fn_7100d35a98). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

void* fn_710043acb4(void* key);

struct UnkDoorEmmyFX;

struct UnkDoorEmmyFXColliders_ovb0074 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
};

void fn_71007b5c48(UnkDoorEmmyFX* self_, bool on) {
    auto* self = reinterpret_cast<UnkDoorEmmyFXColliders_ovb0074*>(self_);
    auto* collision = static_cast<UnkCollisionContainer*>(fn_710043acb4(self->owner));
    if (!collision)
        return;
    static CStrId s_door("door", true);
    static CStrId s_cooldown("doorcooldown", true);
    if (UnkCollisionObject* door = fn_71008640a4(collision, &s_door))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(door), !on);
    if (UnkCollisionObject* cooldown = fn_71008640a4(collision, &s_cooldown))
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(cooldown), on);
}
