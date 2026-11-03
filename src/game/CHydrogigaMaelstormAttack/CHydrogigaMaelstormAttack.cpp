// CHydrogigaMaelstormAttack: its init with the AI component's class data (fn_71005b4998): the
// CHydrogigaAttack init with CCharClassHydrogigaMaelstormAttack, then the ids "L01_Tentacle",
// "L02_Tentacle", "R01_Tentacle" and "R02_Tentacle".
#include "../CHydrogigaTentacleBashAttack/UnkHydrogigaAttackInit.h"
#include "base/reflection/UnkClassGetters.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

void fn_71005b4998(UnkHydrogigaAttack* self, UnkReflObject* arg) {
    InitHydrogigaAttack<fn_71005ad6a4>(self, arg);
    PushMove(&self->field_0x28, CStrId("L01_Tentacle", true));
    PushMove(&self->field_0x28, CStrId("L02_Tentacle", true));
    PushMove(&self->field_0x28, CStrId("R01_Tentacle", true));
    PushMove(&self->field_0x28, CStrId("R02_Tentacle", true));
}
