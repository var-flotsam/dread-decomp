// CPlayerLifeComponent's unit (tools/tu_map.py): unless the cutscene playing (g_unk_7101d47330's +0x58) is
// "0003firstcomunicationfakeadam" or that manager's flag at +0xb14 is set, either marks the component
// (+0x518) when the game manager's effect player (+0x1040) answers fn_7101053a0c, or runs fn_7101164cc0.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2656.h"
#include "types.h"

using base::global::CStrId;

struct UnkCutscene_l16595c {
    u8 pad_0x0[0x18];
    base::global::UnkStringData* name;  // 0x18
};

struct UnkCutsceneManager_l16595c {
    u8 pad_0x0[0x58];
    UnkCutscene_l16595c* current;  // 0x58
    u8 pad_0x60[0xab4];
    bool field_0xb14;
};
static_assert(__builtin_offsetof(UnkCutsceneManager_l16595c, field_0xb14) == 0xb14, "layout");

struct UnkPlayerLife_l16595c {
    u8 pad_0x0[0x518];
    bool field_0x518;
};

struct UnkPlayerLife_ovb0049;
void fn_7101164cc0(UnkPlayerLife_ovb0049* self);

static inline UnkCutsceneManager_l16595c* Cutscenes() {
    return static_cast<UnkCutsceneManager_l16595c*>(g_unk_7101d47330);
}

void fn_710116595c(UnkPlayerLife_l16595c* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    if (UnkCutscene_l16595c* current = Cutscenes()->current) {
        CStrId name("0003firstcomunicationfakeadam", true);
        if (current->name == name.data)
            return;
    }
    if (Cutscenes()->field_0xb14)
        return;
    if (fn_7101053a0c(manager->field_0x1040))
        self->field_0x518 = true;
    else
        fn_7101164cc0(reinterpret_cast<UnkPlayerLife_ovb0049*>(self));
}
