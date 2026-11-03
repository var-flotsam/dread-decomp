// CHydrogigaTongueSwirlAttack: its init with the AI component's class data (fn_71005b7910): the
// CHydrogigaAttack init with CCharClassHydrogigaTongueSwirlAttack, then the id "C01_Tentacle".
#include "../CHydrogigaTentacleBashAttack/UnkHydrogigaAttackInit.h"
#include "base/reflection/UnkClassGetters.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

void fn_71005b7910(UnkHydrogigaAttack* self, UnkReflObject* arg) {
    InitHydrogigaAttack<fn_71005b2544>(self, arg);
    PushMove(&self->field_0x28, CStrId("C01_Tentacle", true));
}
