// GUI::CGammaAdjustComposition's unit (tools/tu_map.py unit 1678): stores the gamma value (+0x234) under "GammaValue" in
// the user data's "USER_DATA" section (unless the data is locked), tells the user data's owner, then, for the main
// menu or the in-game menu that is current, reacts through fn_7100b35704 or fn_7100b2eaac. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2768.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkUserDataOwner;
CBlackboard* fn_710120384c(UnkUserDataOwner* self);
bool fn_7100deac24(UnkGameManager* self);
UnkReflObject* fn_7100e033a8(UnkGameManager*);
void fn_7100b35704(UnkReflObject*, u32, bool);
struct UnkStateOwner;

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkGammaAdjust {
    u8 pad_0x0[0x234];
    f32 gamma;  // 0x234
};

void fn_7100b20b04(UnkGammaAdjust* self) {
    CBlackboard* data = fn_710120384c(GetOwner());
    {
        UnkTypedValue v;
        CStrId section_id("USER_DATA", true);
        CStrId name("GammaValue", true);
        if (CBlackboard::CSection* section = fn_71010472e0(data, section_id)) {
            if (!data->field_0xe8) {
                f32* gamma = &self->gamma;
                // keeps the address computed before the type getter call
                asm("" : "+r"(gamma));
                v.type = fn_710000abbc();
                v.value = gamma;
                fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &v);
            }
        }
    }
    fn_7101203854(GetOwner());
    if (fn_7100deac24(g_unk_7101d47310)) {
        UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
        if (menu && fn_710009d50c(menu->vf_0x0(), fn_7100b34230()))
            fn_7100b35704(menu, 3, true);
    } else {
        UnkReflObject* menu = reinterpret_cast<UnkReflObject*>(fn_7100deac84(g_unk_7101d47310));
        if (menu && fn_710009d50c(menu->vf_0x0(), fn_7100b2c064()))
            fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(menu), 2, false, true);
    }
}
