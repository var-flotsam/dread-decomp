// CHydrogigaTentacleBashAttack: its init with the AI component's class data (fn_71005b6e40): the
// CHydrogigaAttack init (UnkHydrogigaAttackInit.h) with CCharClassHydrogigaTentacleBashAttack, then
// the ids "R02_Tentacle" and "C01_Tentacle".
#include "UnkHydrogigaAttackInit.h"
#include "base/reflection/UnkClassGetters.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

void fn_71005b6e40(UnkHydrogigaAttack* self, UnkReflObject* arg) {
    InitHydrogigaAttack<fn_71005b2164>(self, arg);
    PushMove(&self->field_0x28, CStrId("R02_Tentacle", true));
    PushMove(&self->field_0x28, CStrId("C01_Tentacle", true));
}
