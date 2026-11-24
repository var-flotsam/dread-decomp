// CCameraComponent (unit 2691): script binding. Reads a controller name, finds the controller the
// game manager has for it and stores its string id in the reflected value `out`. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU56.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkCamCtrl_l0e655c {
    u8 pad_0x0[0x18];
    CStrId id;  // 0x18
};

s32 CCameraComponent_ReadControllerNameToStrIdValue(void*, void* stream, UnkReflValue* out) {
    char name[0x80];
    CRntFile_ReadFormat(stream, "s", name);
    UnkGameManager* manager = g_unk_7101d47310;
    UnkCamCtrl_l0e655c* controller;
    {
        CStrId id(name, true);
        controller = reinterpret_cast<UnkCamCtrl_l0e655c*>(fn_7100df2404(manager, id));
    }
    if (controller) {
        const CStrId* value = &controller->id;
        const CType* t = fn_7100009658();
        const CType* cur = reinterpret_cast<const CType*>(out->type);
        if (cur && out->field_0x14 == 0 &&
            (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                      : fn_710009d52c(t, cur))) {
            fn_71000a1cc8(out, t, value);
        } else {
            fn_71000a1c60(out);
            fn_71000a18d4(out, t, value, false);
        }
    }
    return 0;
}
