// GUI::CEndingRewardsComposition's unit (tools/tu_map.py unit 1673): the composition's set-up, as
// CChozoArchivesComposition's does it: the memories' hidden flags, the gallery buttons, the rewards the profile
// lists (each reveals its memory) and the secret button. Names and layouts are placeholders.
#include "../../../game_5821e4/UnkStaticInit.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CBlackboard.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2564.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;
using base::global::CStrId;
using unk_static_init::UnkEnumType;

extern UnkEnumType g_unk_7101d60e78;      // the flags' type

// The hidden flags read from the profile are converted to the flags' type.
struct UnkRewardFlagsTag_lb1ca40;
template <>
struct base::reflection::UnkFieldTraits<UnkRewardFlagsTag_lb1ca40> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_7101d60e78); }
};

// The rewards read from the profile are converted to a list of string ids.
struct UnkRewardListTag_lb1ca40;
template <>
struct base::reflection::UnkFieldTraits<UnkRewardListTag_lb1ca40> {
    static const CType* type() { return &g_unk_7101d5c998; }
};

// A gallery button: a display object container with its id at +0x130.
struct UnkGalleryButton_lb1ca40 {
    void** vtable;
    u8 pad_0x8[0x128];
    CStrId name;  // 0x130

    // Plays the named state.
    void PlayState(const char* state) {
        reinterpret_cast<void (*)(UnkGalleryButton_lb1ca40*, const char*, bool)>(vtable[0x1e8 / 8])(this, state,
                                                                                                   false);
    }
};

struct UnkChildNode_lb1ca40 {
    UnkGuiDisplayObject* item;
    UnkChildNode_lb1ca40* next;
};

struct UnkGallery_lb1ca40 {
    u8 pad_0x0[0x1d0];
    UnkChildNode_lb1ca40* children;  // 0x1d0
};

// A button of the gallery and whether its memory is hidden.
struct UnkGalleryEntry_lb1ca40 {
    UnkGuiDisplayObject* button;
    bool hidden;

    ~UnkGalleryEntry_lb1ca40() {}
};

struct UnkEndingRewards_lb1ca40 {
    u8 pad_0x0[0x248];
    s32 field_0x248;  // the number of memories (the secret's flag comes after theirs)
    s32 field_0x24c;
    u8 pad_0x250[4];
    bool field_0x254;  // every memory is revealed
    bool field_0x255;  // more than one memory is revealed
    u8 pad_0x256[2];
    CRntVector<UnkGalleryEntry_lb1ca40> entries;  // 0x258
    CRntVector<s32> revealed;                     // 0x280: the memories revealed just now
    u8 pad_0x2a8[0x10];
    CRntVector<bool> hidden;  // 0x2b8: one flag per memory, then the secret's
};
static_assert(__builtin_offsetof(UnkEndingRewards_lb1ca40, hidden) == 0x2b8, "UnkEndingRewards_lb1ca40 layout");

void fn_7100af83e0(UnkEndingRewards_lb1ca40* self);

// The list of rewards read from the profile; freed on destruction.
struct UnkRewardList_lb1ca40 : CRntVector<CStrId> {
    UnkRewardList_lb1ca40() : CRntVector<CStrId>(0xf) {}
    ~UnkRewardList_lb1ca40() { Free(); }
};

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

// The id of the profile section that holds the secret's state.
static inline const CStrId& SecretSectionId() {
    return GameManagerIdTable().field_0x1be8;
}

// The id of the profile section that holds the rewards.
static inline const CStrId& RewardsSectionId() {
    return GameManagerIdTable().field_0x1bf0;
}

// The memory a reward reveals: one per difficulty and rating, the first when the id is none of them.
static inline u32 RewardIndex(const CStrId& reward) {
    if (reward == CStrId("0_OK", true))
        return 0;
    if (reward == CStrId("0_GOOD", true))
        return 1;
    if (reward == CStrId("0_EXCELLENT", true))
        return 2;
    if (reward == CStrId("1_OK", true))
        return 3;
    if (reward == CStrId("1_GOOD", true))
        return 4;
    if (reward == CStrId("1_EXCELLENT", true))
        return 5;
    return 0;
}

// A vector's last element.
template <class T>
static inline T& Last(CRntVector<T>& v) {
    return *(v.data + v.count - 1);
}

static inline UnkGuiDisplayObject* IfContainer(UnkGuiDisplayObject* object) {
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(object->GetClass()),
                         reinterpret_cast<const base::reflection::CType*>(fn_71011d7b84()))
               ? object
               : nullptr;
}

void CEndingRewardsComposition_InitGalleryFromProfileRewards(UnkEndingRewards_lb1ca40* self) {
    fn_7100af83e0(self);
    self->field_0x248 = 6;
    self->field_0x24c = 3;
    CBlackboard* profile = &g_unk_7101d47310->field_0x820;
    self->hidden.Clear();
    {
        const CStrId& section_id = RewardsSectionId();
        const UnkQualifiedId& name = Qid(g_unk_717275aeb8);
        if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
            base::refl_GetKeyedValue<UnkRewardFlagsTag_lb1ca40>(reinterpret_cast<char*>(section),
                                                         reinterpret_cast<const base::UnkStrIdMember*>(&name.id),
                                                         &self->hidden);
    }
    // Everything starts hidden when the profile has no flags yet.
    if (self->hidden.count == 0)
        for (s32 i = 0; i <= self->field_0x248; ++i)
            self->hidden.PushBack(true);

    self->entries.Clear();
    UnkGuiDisplayObject* gallery;
    {
        CRntString path("Content.Gallery");
        gallery = fn_71011d9508(reinterpret_cast<UnkGuiDisplayObject*>(self),
                                reinterpret_cast<const char* const*>(&path));
        gallery = gallery ? IfContainer(gallery) : nullptr;
    }
    s32 shown = 0;
    if (gallery) {
        bool* hidden = self->hidden.data;
        for (UnkChildNode_lb1ca40* node = reinterpret_cast<UnkGallery_lb1ca40*>(gallery)->children; node;
             node = node->next, ++hidden) {
            UnkGuiDisplayObject* object = node->item;
            if (!object || !IfContainer(object))
                continue;
            UnkGalleryButton_lb1ca40* button = reinterpret_cast<UnkGalleryButton_lb1ca40*>(object);
            bool is_hidden = *hidden;
            self->entries.EmplaceBack(UnkGalleryEntry_lb1ca40{object, is_hidden});
            if (button->name == CStrId("GalleryButton_Secret", true))
                break;
            fn_71011cdce8(object, true);
            fn_71011cdc30(object, true);
            if (is_hidden) {
                button->PlayState("Memory_Hidden");
            } else {
                ++shown;
                button->PlayState("Memory_Revealed");
            }
        }
    }

    // Each reward earned reveals its memory.
    self->revealed.Clear();
    UnkRewardList_lb1ca40 rewards;
    {
        const CStrId& section_id = RewardsSectionId();
        const UnkQualifiedId& name = Qid(g_unk_717275ae78);
        if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
            base::refl_GetKeyedValue<UnkRewardListTag_lb1ca40>(
                reinterpret_cast<char*>(section), reinterpret_cast<const base::UnkStrIdMember*>(&name.id),
                &rewards);
    }
    for (CStrId* it = rewards.data; it != rewards.data + rewards.count; ++it) {
        CStrId reward = *it;
        u32 index = RewardIndex(reward);
        bool& flag = self->hidden.data[index];
        self->revealed.PushBack(index);
        ++shown;
        flag = false;
    }

    self->field_0x255 = shown > 1;
    self->field_0x254 = false;
    if (shown < self->field_0x248) {
        // The secret stays hidden until every memory is revealed.
        UnkGalleryEntry_lb1ca40& secret = Last(self->entries);
        secret.hidden = true;
        fn_71011cdce8(secret.button, false);
        Last(self->hidden) = true;
    } else {
        self->field_0x254 = true;
        UnkGalleryEntry_lb1ca40& secret = Last(self->entries);
        secret.hidden = false;
        bool& secret_hidden = Last(self->hidden);
        bool visited = false;
        {
            const CStrId& section_id = SecretSectionId();
            const UnkQualifiedId& name = Qid(g_unk_717275aef8);
            if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
                fn_710006d11c(section, &name.id, &visited);
        }
        if (visited) {
            fn_71011cdce8(secret.button, true);
            reinterpret_cast<UnkGalleryButton_lb1ca40*>(secret.button)->PlayState("SecretMemory_Revealed");
        } else if (secret_hidden) {
            fn_71011cdce8(secret.button, false);
            self->revealed.PushBack(self->field_0x248);
        } else {
            fn_71011cdce8(secret.button, true);
            reinterpret_cast<UnkGalleryButton_lb1ca40*>(secret.button)->PlayState("SecretMemory_Hidden");
        }
        secret_hidden = false;
    }
}
