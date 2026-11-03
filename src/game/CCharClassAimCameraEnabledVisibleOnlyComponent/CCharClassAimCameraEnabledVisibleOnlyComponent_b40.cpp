// CCharClassAimCameraEnabledVisibleOnlyComponent unit (tools/tu_map.py unit 1244): fn_71007d68ac shows
// the owner's object at +0x228 (fn_710024fa0c) while the camera's controller is a CAnimatedCameraCtrl or
// the player's gun (+0x2a8, a CSamusGunComponent) reports so (fn_71008598c0), and hides it otherwise.
// Nothing happens without a player or a gun. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkPlayer;
struct UnkStruct_710024fa0c;
UnkPlayer* fn_7100dec788(UnkGameManager*);
void fn_710024fa0c(UnkStruct_710024fa0c*, bool);
UnkReflObject* fn_71010e37f4(void* camera);
bool fn_71008598c0(UnkReflObject* gun);

struct UnkPlayer_ac {
    u8 pad_0x0[0x2a8];
    UnkReflObject* gun;
};

struct UnkOwner_ac {
    u8 pad_0x0[0x228];
    UnkStruct_710024fa0c* field_0x228;
};

struct UnkAimCameraVisible_ac {
    u8 pad_0x0[0x30];
    UnkOwner_ac* owner;
};

void fn_71007d68ac(UnkAimCameraVisible_ac* self) {
    UnkPlayer_ac* player = reinterpret_cast<UnkPlayer_ac*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkReflObject* ctrl = nullptr;
    if (void* camera = fn_7100defcec(g_unk_7101d47310)) {
        if (UnkReflObject* c = fn_71010e37f4(camera))
            ctrl = fn_710009d50c(c->vf_0x0(), fn_71010e7f90()) ? c : nullptr;
    }
    UnkReflObject* gun = player->gun;
    if (!gun || !fn_710009d50c(gun->vf_0x0(), fn_7100857564()))
        return;
    if (UnkStruct_710024fa0c* target = self->owner->field_0x228) {
        bool shown = fn_71008598c0(gun);
        fn_710024fa0c(target, (ctrl != nullptr) | shown);
    }
}
