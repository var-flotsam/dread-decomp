// CCharClassAttackComponent (unit 1185): a reflected setter of the string id at +0x60 of the object at 8;
// it bumps the new name's use count and caches the mask fn_7100414070 gives for its text at +0x68, and
// returns the empty value. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CCharClassAIComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflValue;

struct UnkAttackMask_l78d904 {
    u8 pad_0x0[0x60];
    CStrId name;
    u32 mask;
};

UnkReflValue fn_710078d904(void*, char* obj, UnkReflValue* const* args) {
    auto* target = *reinterpret_cast<UnkAttackMask_l78d904**>(obj + 8);
    UnkReflValue* arg = args[0];
    // The original reads the id even when the argument has no type (then through a null pointer).
    const CStrId* id = arg->type ? base::reflection::refl_ArgValue<CStrId>(arg) : nullptr;
    target->name = *id;
    target->mask = fn_7100414070(id->Text());
    return base::reflection::refl_EmptyValue();
}
