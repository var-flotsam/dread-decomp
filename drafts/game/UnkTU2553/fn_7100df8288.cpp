// UnkTU2553 (the game manager): fills in the unlock flags the user data's section is missing (hard and expert
// mode from the legacy keys and the save slots' rankings, the main menu's state, the rush pop-ups) and saves
// the user data when any was missing. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2544.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU57.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CType;
using unk_static_init::UnkEnumType;

CRntString fn_7100df703c(u32 kind);

// The unit's ids: the names of the flags this fills in and the user data section's id.
struct UnkUnlockIds_ldf8288 {
    CStrId hard_mode;         // hard mode unlocked
    CStrId hard_mode_legacy;  // hard mode unlocked (from the legacy key)
    CStrId expert_mode;       // expert mode unlocked
    CStrId field_0x18;
    CStrId main_menu;         // the main menu was seen
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId section;
};
__attribute__((visibility("hidden"))) extern UnkUnlockIds_ldf8288 g_unk_71727bee38;

extern base::global::UnkStrIdBase g_unk_71727bd268;  // "ShowBossRushPopUp"
extern base::global::UnkStrIdBase g_unk_71727bd280;  // "ShowDreadRushPopUp"

extern UnkPropertyEx<> g_unk_717275ae78;  // "UnlockedRewards"
extern UnkPropertyEx<> g_unk_717275aeb8;
extern UnkEnumType g_unk_7101d60e78;  // the flags' type

// The rewards read from the archive section are converted to a vector of string ids.
struct UnkRewardsTag;
template <>
struct base::reflection::UnkFieldTraits<UnkRewardsTag> {
    static const CType* type() { return &g_unk_7101d5c998; }
};

// The flags read from the archive section are converted to the flags' type.
struct UnkSlotFlagsTag;
template <>
struct base::reflection::UnkFieldTraits<UnkSlotFlagsTag> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_7101d60e78); }
};

// The rewards of a save slot; freed on destruction.
struct UnkRewardList_ldf8288 : CRntVector<CStrId> {
    UnkRewardList_ldf8288() : CRntVector<CStrId>(0xf) {}
    ~UnkRewardList_ldf8288() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~CStrId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// One flag per scenario; freed on destruction.
struct UnkFlagList_ldf8288 : CRntVector<bool> {
    UnkFlagList_ldf8288() : CRntVector<bool>(0xf) {}
    ~UnkFlagList_ldf8288() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// The menus the game manager keeps, by name.
struct UnkMenu_ldf8288 {
    u8 pad_0x0[0x8];
    CRntString name;
    u8 pad_0x38[0xc8];
    bool field_0x100;
    u8 pad_0x101[0x13f];
    bool field_0x240;
    u8 pad_0x241[0x13f];
    bool field_0x380;
};

struct UnkMenuNode_ldf8288 {
    UnkMenu_ldf8288* value;
    UnkMenuNode_ldf8288* next;
};

struct UnkGameManagerMenus_ldf8288 {
    u8 pad_0x0[0x478];
    UnkMenuNode_ldf8288* menus;
};

static inline UnkUserDataOwner* GetOwner() {
    if (!g_unk_7101d47308) {
        UnkUserDataOwner* owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return g_unk_7101d47308;
}

// Stores `*value` under `name` in the user data's section, unless the data is locked.
static inline void StoreBool(CBlackboard* data, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* section = fn_71010472e0(data, g_unk_71727bee38.section)) {
        if (!data->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(section, name, &v);
        }
    }
}

static inline bool ReadBool(CBlackboard* data, const CStrId* name, bool* out) {
    if (CBlackboard::CSection* section = fn_7101047270(data, g_unk_71727bee38.section))
        return fn_710006d11c(section, name, out);
    return false;
}

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The id of the archive section that holds the slot's rewards (a slot of the game's id table).
static inline const CStrId& ArchiveSectionId() {
    return reinterpret_cast<const CStrId&>(GameManagerIdTable().pad_0x1bf0);
}

template <class Tag>
static inline void ReadArchive(CBlackboard* archive, UnkPropertyEx<>& item, void* out) {
    const CStrId& section_id = ArchiveSectionId();
    const UnkQualifiedId& name = Qid(item);
    if (CBlackboard::CSection* section = fn_7101047270(archive, section_id))
        base::refl_GetKeyedValue<Tag>(reinterpret_cast<char*>(section),
                                      reinterpret_cast<const base::UnkStrIdMember*>(&name.id), out);
}

static inline bool Equal(const CRntString* a, const CRntString* b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(a), reinterpret_cast<const UnkCompared_0x30*>(b));
}

static inline UnkMenu_ldf8288* FindMenu(UnkGameManager* manager, const CRntString& name) {
    UnkGameManagerMenus_ldf8288* menus = reinterpret_cast<UnkGameManagerMenus_ldf8288*>(manager);
    for (UnkMenuNode_ldf8288* node = menus->menus; node; node = node->next) {
        if (Equal(&node->value->name, &name))
            return node->value;
    }
    return nullptr;
}

static inline UnkPropertyName* Name(CStrId& id) {
    return reinterpret_cast<UnkPropertyName*>(&id);
}

void fn_7100df8288(UnkGameManager* self) {
    CBlackboard* data = fn_710120384c(GetOwner());
    bool has_hard = fn_71010474f4(data, g_unk_71727bee38.section, g_unk_71727bee38.hard_mode);
    bool has_hard_legacy = fn_71010474f4(data, g_unk_71727bee38.section, g_unk_71727bee38.hard_mode_legacy);
    bool has_expert = fn_71010474f4(data, g_unk_71727bee38.section, g_unk_71727bee38.expert_mode);
    bool has_3 = fn_71010474f4(data, g_unk_71727bee38.section, g_unk_71727bee38.field_0x18);
    bool has_main_menu = fn_71010474f4(data, g_unk_71727bee38.section, g_unk_71727bee38.main_menu);

    if (!has_hard) {
        bool value = false;
        StoreBool(data, Name(g_unk_71727bee38.hard_mode), &value);
    }

    if (!has_hard_legacy) {
        bool value = false;
        {
            CStrId key("HardModeUnlocked", true);
            ReadBool(data, &key, &value);
        }
        StoreBool(data, Name(g_unk_71727bee38.hard_mode_legacy), &value);
    }

    if (!has_expert) {
        // Unlocked when a save slot has a good enough ranking, or misses one of the last three scenarios' flags.
        bool expert = false;
        for (u8 slot = 0; slot < 3 && !expert; ++slot) {
            if (fn_7100df78cc(self, fn_7100df703c(slot), false, false))
                continue;
            CBlackboard* archive = &g_unk_7101d47310->field_0x820;
            bool unlocked = false;
            {
                UnkRewardList_ldf8288 rewards;
                ReadArchive<UnkRewardsTag>(archive, g_unk_717275ae78, &rewards);
                for (const CStrId* it = rewards.data, *end = it + rewards.count; it != end; ++it) {
                    if (*it == CStrId("1_OK", true) || *it == CStrId("1_GOOD", true) ||
                        *it == CStrId("1_EXCELLENT", true)) {
                        unlocked = true;
                        break;
                    }
                }
            }
            if (!unlocked) {
                UnkFlagList_ldf8288 flags;
                ReadArchive<UnkSlotFlagsTag>(archive, g_unk_717275aeb8, &flags);
                for (u32 i = 3; i < 6 && i < flags.count; ++i) {
                    if (!flags.data[i]) {
                        unlocked = true;
                        break;
                    }
                }
            }
            expert = unlocked;
        }
        StoreBool(data, Name(g_unk_71727bee38.expert_mode), &expert);

        if (expert) {
            bool already = false;
            {
                CStrId key("ExpertModeUnlocked", true);
                ReadBool(data, &key, &already);
            }
            if (!already) {
                {
                    CStrId key("ExpertModeUnlocked", true);
                    bool value = true;
                    StoreBool(data, Name(key), &value);
                }
                {
                    CStrId key("ShowExpertModePopUp", true);
                    bool value = true;
                    StoreBool(data, Name(key), &value);
                }
            }
        }
    }

    if (!has_3) {
        bool value = false;
        StoreBool(data, Name(g_unk_71727bee38.field_0x18), &value);
    }

    if (!has_main_menu) {
        bool value = false;
        if (UnkMenu_ldf8288* menu = FindMenu(g_unk_7101d47310, CRntString("MAINMENU")))
            value = menu->field_0x100 || menu->field_0x240 || menu->field_0x380;
        StoreBool(data, Name(g_unk_71727bee38.main_menu), &value);
    }

    bool boss_rush_shown = false;
    if (!ReadBool(data, reinterpret_cast<const CStrId*>(&g_unk_71727bd268), &boss_rush_shown)) {
        bool easy = false;
        bool normal = false;
        {
            CStrId key("EasyModeCompleted", true);
            ReadBool(data, &key, &easy);
        }
        {
            CStrId key("NormalModeCompleted", true);
            ReadBool(data, &key, &normal);
        }
        if (easy || normal) {
            {
                bool value = true;
                StoreBool(data, &g_unk_71727bd270, &value);
            }
            {
                bool value = true;
                StoreBool(data, reinterpret_cast<UnkPropertyName*>(&g_unk_71727bd268), &value);
            }
        }
    }

    bool dread_rush_shown = false;
    if (!ReadBool(data, reinterpret_cast<const CStrId*>(&g_unk_71727bd280), &dread_rush_shown)) {
        bool expert_done = false;
        {
            CStrId key("ExpertModeCompleted", true);
            ReadBool(data, &key, &expert_done);
        }
        if (expert_done) {
            {
                bool value = true;
                StoreBool(data, &g_unk_71727bd288, &value);
            }
            {
                bool value = true;
                StoreBool(data, reinterpret_cast<UnkPropertyName*>(&g_unk_71727bd280), &value);
            }
        }
    }

    if (!(has_hard && has_hard_legacy && has_expert && has_3 && has_main_menu))
        fn_7101203854(GetOwner());
}
