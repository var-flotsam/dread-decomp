// Unit 1449 (tools/tu_map.py; no reflected class). fn_710097d71c picks a target: the gun owner's
// (fn_710030c5fc) gun named by the id at +0x1240 of the reflection singleton, when it has one that
// accepts (fn_710081b0d0), else the owner's fallback (fn_710099b5b8). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "types.h"

using base::global::CStrId;

struct UnkAuraGunOwner;
struct CGrappleBeamGun;
CGrappleBeamGun* fn_710030c5fc(UnkAuraGunOwner* self, const CStrId& id);

struct UnkTarget1449;
void fn_7100839ae8(UnkAuraGunOwner* self, bool flag);
bool fn_710081b0d0(UnkTarget1449* self, bool flag);
UnkTarget1449* fn_710081b510(UnkTarget1449* self);

struct UnkActor1449;
UnkTarget1449* fn_710099b5b8(UnkActor1449* self, bool flag);

struct UnkGunSlot1449 {
    u8 pad_0x0[0x10];
    UnkTarget1449* target;
};

struct UnkGun1449 {
    u8 pad_0x0[0x1888];
    UnkGunSlot1449* slot;
};

static inline const CStrId& Name_1449() {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) +
                                            0x1240);
}

UnkTarget1449* fn_710097d71c(char* self) {
    UnkActor1449* actor = *reinterpret_cast<UnkActor1449**>(*reinterpret_cast<char**>(self + 8) + 8);
    char* a = *reinterpret_cast<char**>(reinterpret_cast<char*>(actor) + 0x38);
    UnkAuraGunOwner* owner = *reinterpret_cast<UnkAuraGunOwner**>(*reinterpret_cast<char**>(a + 0x30) + 0x2a8);
    fn_7100839ae8(owner, true);
    if (UnkGun1449* gun = reinterpret_cast<UnkGun1449*>(fn_710030c5fc(owner, Name_1449()))) {
        UnkTarget1449* t = gun->slot ? gun->slot->target : nullptr;
        if (fn_710081b0d0(t, true))
            return fn_710081b510(t);
    }
    return fn_710099b5b8(actor, true);
}
