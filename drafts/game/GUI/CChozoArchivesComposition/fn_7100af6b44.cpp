// GUI::CChozoArchivesComposition's unit (tools/tu_map.py unit 1655): the composition's set-up. It reads the
// archives' hidden flags (one per memory and one for the secret), shows each gallery button as hidden or revealed,
// reveals the memories of the scenarios the profile lists and decides how the secret button shows. Names and
// layouts are placeholders.
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
extern UnkPropertyEx<> g_unk_717275adb0;  // "UnlockedArchives"

// The hidden flags read from the profile are converted to the flags' type.
struct UnkArchiveFlagsTag;
template <>
struct base::reflection::UnkFieldTraits<UnkArchiveFlagsTag> {
    static const CType* type() { return reinterpret_cast<const CType*>(&g_unk_7101d60e78); }
};

// The scenarios read from the profile are converted to a list of string ids.
struct UnkScenarioListTag_laf6b44;
template <>
struct base::reflection::UnkFieldTraits<UnkScenarioListTag_laf6b44> {
    static const CType* type() { return &g_unk_7101d5c998; }
};

// A gallery button: a display object container with its id at +0x130.
struct UnkGalleryButton_laf6b44 {
    void** vtable;
    u8 pad_0x8[0x128];
    CStrId name;  // 0x130

    // Plays the named state.
    void PlayState(const char* state) {
        reinterpret_cast<void (*)(UnkGalleryButton_laf6b44*, const char*, bool)>(vtable[0x1e8 / 8])(this, state,
                                                                                                   false);
    }
};

struct UnkChildNode_laf6b44 {
    UnkGuiDisplayObject* item;
    UnkChildNode_laf6b44* next;
};

struct UnkGallery_laf6b44 {
    u8 pad_0x0[0x1d0];
    UnkChildNode_laf6b44* children;  // 0x1d0
};

// A button of the gallery and whether its memory is hidden.
struct UnkGalleryEntry_laf6b44 {
    UnkGuiDisplayObject* button;
    bool hidden;

    ~UnkGalleryEntry_laf6b44() {}
};

struct UnkChozoArchives_laf6b44 {
    u8 pad_0x0[0x248];
    s32 field_0x248;  // the number of memories (the secret's flag comes after theirs)
    s32 field_0x24c;
    u8 pad_0x250[4];
    bool field_0x254;  // every memory is revealed
    bool field_0x255;  // more than one memory is revealed
    u8 pad_0x256[2];
    CRntVector<UnkGalleryEntry_laf6b44> entries;  // 0x258
    CRntVector<u32> revealed;                     // 0x280: the memories revealed just now
    u8 pad_0x2a8[0x10];
    CRntVector<bool> hidden;  // 0x2b8: one flag per memory, then the secret's
};
static_assert(__builtin_offsetof(UnkChozoArchives_laf6b44, hidden) == 0x2b8, "UnkChozoArchives_laf6b44 layout");

void fn_7100af83e0(UnkChozoArchives_laf6b44* self);

// The list of scenarios read from the profile; freed on destruction.
struct UnkScenarioList_laf6b44 : CRntVector<CStrId> {
    UnkScenarioList_laf6b44() : CRntVector<CStrId>(0xf) {}
    ~UnkScenarioList_laf6b44() { Free(); }
};

// The property's qualified id, built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// The id of the profile section that holds the archives' state.
static inline const CStrId& ArchivesSectionId() {
    return GameManagerIdTable().field_0x1be8;
}

static inline bool SameId(const base::global::UnkStrIdBase& a, const base::global::UnkStrIdBase& b) {
    return a.data == b.data;
}

static inline u32 ScenarioIndex(const CStrId& scenario) {
    if (SameId(scenario, g_unk_71727c7fe8))
        return 0;
    if (SameId(scenario, g_unk_71727c7ff0))
        return 1;
    if (SameId(scenario, g_unk_71727c7ff8))
        return 2;
    if (SameId(scenario, g_unk_71727c8000))
        return 3;
    if (SameId(scenario, g_unk_71727c8008))
        return 4;
    if (SameId(scenario, g_unk_71727c8010))
        return 5;
    if (SameId(scenario, g_unk_71727c8018))
        return 6;
    if (SameId(scenario, g_unk_71727c8020))
        return 7;
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

void fn_7100af6b44(UnkChozoArchives_laf6b44* self) {
    fn_7100af83e0(self);
    self->field_0x248 = 8;
    self->field_0x24c = 4;
    CBlackboard* profile = &g_unk_7101d47310->field_0x820;
    self->hidden.Clear();
    {
        const CStrId& section_id = ArchivesSectionId();
        const UnkQualifiedId& name = Qid(g_unk_717275adf0);
        if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
            base::refl_GetKeyedValue<UnkArchiveFlagsTag>(reinterpret_cast<char*>(section),
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
        for (UnkChildNode_laf6b44* node = reinterpret_cast<UnkGallery_laf6b44*>(gallery)->children; node;
             node = node->next, ++hidden) {
            UnkGuiDisplayObject* object = node->item;
            if (!object || !IfContainer(object))
                continue;
            UnkGalleryButton_laf6b44* button = reinterpret_cast<UnkGalleryButton_laf6b44*>(object);
            bool is_hidden = *hidden;
            self->entries.EmplaceBack(UnkGalleryEntry_laf6b44{object, is_hidden});
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

    // The scenarios finished since the last visit reveal their memories.
    self->revealed.Clear();
    UnkScenarioList_laf6b44 scenarios;
    {
        const CStrId& section_id = ArchivesSectionId();
        const UnkQualifiedId& name = Qid(g_unk_717275adb0);
        if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
            base::refl_GetKeyedValue<UnkScenarioListTag_laf6b44>(
                reinterpret_cast<char*>(section), reinterpret_cast<const base::UnkStrIdMember*>(&name.id),
                &scenarios);
    }
    if (scenarios.count != 0) {
        self->revealed.Clear();
        for (CStrId* it = scenarios.data; it != scenarios.data + scenarios.count; ++it) {
            CStrId scenario = *it;
            u32 index = ScenarioIndex(scenario);
            bool& flag = self->hidden.data[index];
            if (flag) {
                self->revealed.PushBack(index);
                ++shown;
                flag = false;
            }
        }
    }

    self->field_0x255 = shown > 1;
    self->field_0x254 = false;
    if (shown < self->field_0x248) {
        // The secret stays hidden until every memory is revealed.
        UnkGalleryEntry_laf6b44& secret = Last(self->entries);
        secret.hidden = true;
        fn_71011cdce8(secret.button, false);
        Last(self->hidden) = true;
    } else {
        self->field_0x254 = true;
        UnkGalleryEntry_laf6b44& secret = Last(self->entries);
        secret.hidden = false;
        bool& secret_hidden = Last(self->hidden);
        bool visited = false;
        {
            const CStrId& section_id = ArchivesSectionId();
            const UnkQualifiedId& name = Qid(g_unk_717275ae30);
            if (CBlackboard::CSection* section = fn_7101047270(profile, section_id))
                fn_710006d11c(section, &name.id, &visited);
        }
        if (visited) {
            fn_71011cdce8(secret.button, true);
            reinterpret_cast<UnkGalleryButton_laf6b44*>(secret.button)->PlayState("SecretMemory_Revealed");
        } else if (secret_hidden) {
            fn_71011cdce8(secret.button, false);
            self->revealed.PushBack(self->field_0x248);
        } else {
            fn_71011cdce8(secret.button, true);
            reinterpret_cast<UnkGalleryButton_laf6b44*>(secret.button)->PlayState("SecretMemory_Hidden");
        }
        secret_hidden = false;
    }
}
