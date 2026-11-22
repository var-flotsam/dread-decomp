// GUI::CContinueGameButton (unit 1657): continues the game from the main menu's save slot for the
// current profile, or starts a new game; either way the profile gets a new id (a UUID kept in the
// profile blackboard). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

namespace nn::util {
struct Uuid {
    u8 data[16];
};
Uuid GenerateUuid();
s32 TSNPrintf(char* buffer, u64 size, const char* format, ...);
}  // namespace nn::util

struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};

CRntString fn_7100df703c(u32 slot);  // the name of save slot `slot`
void fn_7100dfecec(UnkGameManager* self, u32 a);

// The main menu's items, four for each save slot: the slot's data is its second, and its fourth
// says whether the slot was just created.
struct UnkMenuItem_lafa6ec {
    u8 pad_0x0[0x10];
    bool is_new;  // 0x10
    u8 pad_0x11[0x3f];
};
static_assert(sizeof(UnkMenuItem_lafa6ec) == 0x50, "layout");

struct UnkMainMenu_lafa6ec {
    u8 pad_0x0[0xb0];
    UnkMenuItem_lafa6ec items[12];
};

// What a slot's data says about the save (built out of line, destroyed inline).
struct UnkSaveSlotInfo_lafa6ec {
    bool valid;
    u8 pad_0x1[0x2b];
    s32 field_0x2c;
    u8 pad_0x30[0x50];
    CRntString profile_id;  // 0x80

    explicit UnkSaveSlotInfo_lafa6ec(const UnkMenuItem_lafa6ec* data) asm("fn_7100afa4c0");
};

// The game manager's current profile name.
struct UnkGameManagerProfile_lafa6ec {
    u8 pad_0x0[0x7f0];
    CRntString profile;
};

struct UnkContinueGameButton_lafa6ec {
    u8 pad_0x0[0x2a4];
    f32 field_0x2a4;
};

static inline const CStrId& IdTableKey(u32 offset) {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}

static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

static inline void FormatUuid(char* buffer, const nn::util::Uuid& u) {
    nn::util::TSNPrintf(buffer, 0x40, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                        u.data[0], u.data[1], u.data[2], u.data[3], u.data[4], u.data[5], u.data[6], u.data[7],
                        u.data[8], u.data[9], u.data[10], u.data[11], u.data[12], u.data[13], u.data[14],
                        u.data[15]);
}

static inline bool Equal(const CRntString& a, const CRntString& b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&a), reinterpret_cast<const UnkCompared_0x30*>(&b));
}

// Stores a string under `name` in a blackboard section.
static inline void SetString(CBlackboard::CSection* section, const UnkQualifiedId& name, CRntString* value) {
    UnkTypedValue v;
    v.type = fn_7100007f84();
    v.value = value;
    fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
}

void fn_7100afa6ec(UnkContinueGameButton_lafa6ec* self) {
    UnkMainMenu_lafa6ec* menu =
        static_cast<UnkMainMenu_lafa6ec*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")));
    const CRntString& profile_name = reinterpret_cast<UnkGameManagerProfile_lafa6ec*>(g_unk_7101d47310)->profile;
    u32 slot;
    if (Equal(profile_name, fn_7100df703c(0)))
        slot = 0;
    else if (Equal(profile_name, fn_7100df703c(1)))
        slot = 1;
    else if (Equal(profile_name, fn_7100df703c(2)))
        slot = 2;
    else
        slot = 0;
    UnkMenuItem_lafa6ec* data = &menu->items[slot * 4 + 1];

    UnkSaveSlotInfo_lafa6ec info(data);
    // The profile gets a new id (a UUID), stored in the profile blackboard and in `info`.
    char text[0x40];
    if (info.valid && self->field_0x2a4 > 0.0f) {
        UnkMenuItem_lafa6ec* state = &data[2];
        if (state->is_new) {
            state->is_new = false;
            UnkGameManager* gm = g_unk_7101d47310;
            CBlackboard* profile = &gm->field_0x820;
            FormatUuid(text, nn::util::GenerateUuid());
            {
                const CStrId& section = IdTableKey(0x1148);
                const UnkQualifiedId& name = Qid(g_unk_71727594b8);
                CRntString value(text);
                if (CBlackboard::CSection* s = fn_71010472e0(profile, section)) {
                    if (!profile->field_0xe8) {
                        SetString(s, name, &value);
                    }
                }
            }
            {
                CRntString id(text);
                CRntString_Assign(&info.profile_id, &id);
            }
            UnkProfileSaveArg arg = {nullptr, nullptr};
            fn_7100df99f8(g_unk_7101d47310, 0, &arg, false);
        }
        fn_7100dfecec(g_unk_7101d47310, 0);
    } else {
        fn_7100000790("Init.CreateAndInitNewGame", "iss", info.field_0x2c, "s010_cave", "StartPoint0");
        UnkGameManager* gm = g_unk_7101d47310;
        CBlackboard* profile = &gm->field_0x820;
        FormatUuid(text, nn::util::GenerateUuid());
        {
            const CStrId& section = IdTableKey(0x1148);
            const UnkQualifiedId& name = Qid(g_unk_71727594b8);
            CRntString value(text);
            if (CBlackboard::CSection* s = fn_71010472e0(profile, section)) {
                if (!profile->field_0xe8) {
                    SetString(s, name, &value);
                }
            }
        }
        CRntString id(text);
        CRntString_Assign(&info.profile_id, &id);
    }
}
