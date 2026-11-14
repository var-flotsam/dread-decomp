// GUI::CChozoArchivesComposition's unit (tools/tu_map.py unit 1655): the composition's event handler (after the
// gallery's own, fn_7100af8450, the main menu shows its rewards when it has any, or the composition closes
// through its slot 0x278) and the reveal of a secret entry (unless the profile's "SecretVisited" flag is set:
// the flag is set, the profile saved, and the entry plays "SecretMemory_Revealed"). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/GUI/CMainMenu.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::reflection::UnkReflObject;
struct UnkGallery_A5_b75;
struct UnkGuiEvent_b49;
void fn_7100af8450(UnkGallery_A5_b75* self, UnkGuiEvent_b49* event);

struct UnkArchivesMenuPart_ovb0124 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkArchivesMainMenu_ovb0124 {
    u8 pad_0x0[0x528];
    u8 part[8];  // 0x528: an UnkArchivesMenuPart_ovb0124
    UnkReflObject* rewards;            // 0x530
};

struct UnkArchivesRewards_ovb0124 {
    u8 pad_0x0[0x288];
    u32 count;
};

struct UnkArchivesComposition_ovb0124 {
    void** vtable;
    u8 pad_0x8[0x280];
    u32 field_0x288;
};

void fn_7100af77dc(UnkArchivesComposition_ovb0124* self, UnkGuiEvent_b49* event) {
    fn_7100af8450(reinterpret_cast<UnkGallery_A5_b75*>(self), event);
    if (self->field_0x288 != 0)
        return;
    UnkReflObject* menu = fn_7100e033a8(g_unk_7101d47310);
    if (menu)
        menu = fn_710009d50c(menu->vf_0x0(), fn_7100b34230()) ? menu : nullptr;
    UnkArchivesMainMenu_ovb0124* m = reinterpret_cast<UnkArchivesMainMenu_ovb0124*>(menu);
    reinterpret_cast<UnkArchivesMenuPart_ovb0124*>(m->part)->vf_0x10();
    UnkReflObject* rewards = m->rewards;
    auto* r = reinterpret_cast<UnkArchivesRewards_ovb0124*>(
        fn_710009d50c(rewards->vf_0x0(), fn_7100af7fb4()) ? rewards : nullptr);
    if (r->count != 0) {
        fn_7100b35704(menu, 0xb, true);
        f32 one = 1.0f;
        fn_7100219294(g_unk_7101d47300, 0xede73d1953baf654ull, &one, 2);
    } else {
        reinterpret_cast<void (*)(UnkArchivesComposition_ovb0124*)>(self->vtable[0x278 / 8])(self);
    }
}

using base::global::CStrId;

struct UnkProfileSaveArg {
    void* field_0x0;
    void* field_0x8;
};

static inline const CStrId& IdTableKey(u32 offset) {
    return *reinterpret_cast<const CStrId*>(GameManagerIdTable().pad_0x0 + offset);
}

// The property's qualified id (game/UnkGameBlackboard.h), built on first use.
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = reinterpret_cast<UnkQualifiedId&>(item.field_0x10);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, reinterpret_cast<UnkIdSource*>(&item), g_unk_7101d470e0);
    return qid;
}

struct UnkArchivesEntryItem_ovb0124 {
    void** vtable;

    void vf_0x1e8(const char* name, bool b) {
        reinterpret_cast<void (*)(UnkArchivesEntryItem_ovb0124*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, b);
    }
};

// Stores `value` under `name` in the section `section` names (unless the blackboard is locked).
static inline void SetBool_ovb0124(CBlackboard* blackboard, const CStrId& section, const UnkQualifiedId& name,
                                   const bool& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = const_cast<bool*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&name)), &v);
        }
    }
}

void fn_7100af79a0(void* self, UnkArchivesEntryItem_ovb0124* item) {
    UnkGameManager* gm = g_unk_7101d47310;
    CBlackboard* profile = reinterpret_cast<CBlackboard*>(reinterpret_cast<char*>(gm) + 0x820);
    bool visited = false;
    {
        const CStrId& section = IdTableKey(0x1be8);
        const UnkQualifiedId& name = Qid(g_unk_717275ae30);
        if (CBlackboard::CSection* s = fn_7101047270(profile, section))
            fn_710006d11c(s, &name.id, &visited);
    }
    if (visited)
        return;
    SetBool_ovb0124(profile, IdTableKey(0x1be8), Qid(g_unk_717275ae30), true);
    UnkProfileSaveArg arg = {nullptr, nullptr};
    fn_7100df99f8(g_unk_7101d47310, 0, &arg, 0);
    item->vf_0x1e8("SecretMemory_Revealed", false);
}
