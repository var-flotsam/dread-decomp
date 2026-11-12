// UnkTU2727 (main menu script unit): the main menu object's sound fade-out (the handle at +0x440 is
// told to fade and emptied), its destructor, the popup set-up that also hands the menu's two
// compositions (+0x68 and +0x70) back to the GUI root, a name lookup over the id table's two
// entries (the slot of the matching 0x140-byte entry), and the id assignment that attaches a menu
// item to its parent. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/snd/UnkSound.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringEntry;

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};
extern void* g_unk_7101cb51b8[];               // the menu's vtable

struct UnkCompared_0x30;
struct UnkPropBag;
void fn_710050e8e4(UnkPropBag*);
struct UnkHandleOwner_0x2280;
void fn_7100e02fdc(UnkHandleOwner_0x2280* self, UnkWeakTarget* target);
bool fn_7101191cc0(void* self, void* arg);

// A reference to a composition: released by its count.
struct UnkCompRef_ovb0186 {
    UnkGuiHandleObj* ptr;
    ~UnkCompRef_ovb0186() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkMenuEntryBase_ovb0186 {
    u8 pad_0x0[0x108];
    ~UnkMenuEntryBase_ovb0186() { fn_710050e8e4(reinterpret_cast<UnkPropBag*>(this)); }
};

struct UnkMenuEntry_ovb0186 : UnkMenuEntryBase_ovb0186 {
    CRntString text;  // 0x108
    u64 field_0x138;
};
static_assert(sizeof(UnkMenuEntry_ovb0186) == 0x140, "entry");

struct UnkMainMenuBase_ovb0186 {
    void** vtable;
    u8 pad_0x8[0x60];
    ~UnkMainMenuBase_ovb0186() { fn_71011867d4(this); }
};

// Takes the composition off the GUI root's list (the object is told through its vf_0x248 first).
static inline void Detach(UnkCompRef_ovb0186& ref) {
    if (ref.ptr) {
        if (UnkGuiDisplayObject* object = ref.ptr->target) {
            (*reinterpret_cast<void (***)(UnkGuiDisplayObject*)>(object))[0x248 / 8](object);
            fn_71011b57c4(g_unk_717271e8f8, ref.ptr ? ref.ptr->target : nullptr);
        }
    }
}

struct UnkMainMenu_ovb0186 : UnkMainMenuBase_ovb0186 {
    UnkCompRef_ovb0186 field_0x68;
    UnkCompRef_ovb0186 field_0x70;
    UnkMenuEntry_ovb0186 entry_0x78;
    UnkMenuEntry_ovb0186 entry_0x1b8;
    UnkMenuEntry_ovb0186 entry_0x2f8;
    u8 pad_0x438[8];
    UnkSoundHandle sound;  // 0x440

    __attribute__((always_inline)) ~UnkMainMenu_ovb0186() {
        vtable = &g_unk_7101cb51b8[2];
        Detach(field_0x68);
        Detach(field_0x70);
        FadeOutSound();
    }

    // Tells the playing sound to fade out and forgets it.
    __attribute__((always_inline)) void FadeOutSound() {
        UnkSoundRef* ref = sound.ref;
        if (ref && ref->serial == sound.serial && sound.serial != -1 && ref->object) {
            fn_7100105a80(ref->object, 0.0f, 0.0f);
            sound = g_unk_7108b6c460;
        }
    }
};
static_assert(__builtin_offsetof(UnkMainMenu_ovb0186, sound) == 0x440, "layout");

void fn_7101190700(UnkMainMenu_ovb0186* self) {
    UnkSoundRef* ref = self->sound.ref;
    if (ref && ref->serial == self->sound.serial && self->sound.serial != -1 && ref->object) {
        fn_7100105a80(ref->object, 0.0f, 0.0f);
        self->sound = g_unk_7108b6c460;
    }
}

void fn_710119056c(UnkMainMenu_ovb0186* self) {
    self->~UnkMainMenu_ovb0186();
}

struct UnkGameManagerPopup_ovb0186 {
    u8 pad_0x0[0x2268];
    UnkWeakHandle* field_0x2268;
};

bool fn_7101190fa4(UnkMainMenu_ovb0186* self, void* arg) {
    Detach(self->field_0x68);
    Detach(self->field_0x70);
    bool result = fn_7101191cc0(self, arg);
    UnkGameManagerPopup_ovb0186* manager = reinterpret_cast<UnkGameManagerPopup_ovb0186*>(g_unk_7101d47310);
    if (!manager->field_0x2268 || !manager->field_0x2268->target) {
        UnkGuiDisplayObject* popup = nullptr;
        {
            UnkGuiRoot* root = g_unk_717271e8f8;
            CRntString name("popupgeneric");
            UnkGuiHandle handle = fn_71011b69d8(root, name);
            if (UnkGuiDisplayObject* object = handle.Get())
                popup = fn_710009d50c(object->GetClass(), fn_7100b60d58()) ? object : nullptr;
        }
        if (popup) {
            fn_71011cdce8(popup, false);
            fn_71011cf2a4(popup, false);
            fn_71011cf364(popup, -1);
            (*reinterpret_cast<void (***)(UnkGuiDisplayObject*, s32, bool)>(popup))[0x78 / 8](popup, -2000, true);
            fn_71011cde44(popup, 2);
            fn_71011d968c(g_unk_717271e8f8->menu, popup);
            fn_7100e02fdc(reinterpret_cast<UnkHandleOwner_0x2280*>(g_unk_7101d47310),
                          reinterpret_cast<UnkWeakTarget*>(popup));
        }
    }
    fn_71011b6b5c(g_unk_717271e8f8, 0);
    return result;
}

// Two string ids of the id table (game/UnkGameManagerIdTable.h), at +0x1cb0 and +0x1cb8, name the entries
// the lookup tells apart.
inline UnkGameManagerIdTable* IdTable_ovb0186() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return &s_table;
}

// Takes a reference on the name (an untyped store: the table's pointer is read again after it).
#define NAME_OF(slot)                                                           \
    ({                                                                          \
        UnkGameManagerIdTable* table = IdTable_ovb0186();                       \
        UnkStringEntry* entry = reinterpret_cast<UnkStringEntry*>(table->slot.data); \
        u32 count = entry->tail.field_0x44 + 1;                                 \
        __builtin_memcpy(&entry->tail.field_0x44, &count, 4);                   \
        reinterpret_cast<const UnkCompared_0x30*>(                              \
            &reinterpret_cast<UnkStringEntry*>(table->slot.data)->buffer);      \
    })

struct UnkMenuEntries_ovb0186 {
    u8 pad_0x0[0x78];
    u8 field_0x78[0x140];
    u8 field_0x1b8[0x140];
    u8 field_0x2f8[0x140];
};

// The name is passed by value: both callers copy the string for the call.
void* fn_7101190890(UnkMenuEntries_ovb0186* self, CRntString name) {
    const UnkCompared_0x30* key = reinterpret_cast<const UnkCompared_0x30*>(&name);
    if (fn_710007f120(key, NAME_OF(field_0x1cb0)))
        return self->field_0x1b8;
    if (fn_710007f120(key, NAME_OF(field_0x1cb8)))
        return self->field_0x2f8;
    return self->field_0x78;
}

struct UnkMenuItem_ovb0186 {
    UnkGuiDisplayObject* Object() { return reinterpret_cast<UnkGuiDisplayObject*>(this); }
    u8 pad_0x0[0x130];
    CStrId id;  // 0x130
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[0x2];
    bool field_0x187;
    bool field_0x188;
};

void fn_7101192420(UnkGuiRoot* root, UnkMenuItem_ovb0186* item, const CStrId* id, UnkGuiDisplayObject* parent) {
    if (!item)
        return;
    item->id = *id;
    if (parent)
        fn_71011cd424(item->Object(), parent);
    if (!item->field_0x187 &&
        (item->field_0x188 || item->field_0x184 ||
         (fn_71011cde14(item->Object())->field_0x3c &&
          (fn_71011ceac0(item->Object()) || fn_71011ceb34(item->Object())))))
        fn_71011b5598(root, item->Object());
}
