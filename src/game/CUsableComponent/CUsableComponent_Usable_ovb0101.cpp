// CUsableComponent's unit (tools/tu_map.py): the usable's blackboard flags (stored under the unit's
// properties g_unk_7172753170, g_unk_7172753198 and g_unk_71727531c0 in the section the usable's
// vf_0x2d8 names; the name looked up in the key list at +0x4d8, count +0x4e0, values +0x4d0, as in
// CElevatorUsableComponent_b48.cpp), the pop-up message shown through the game manager's GUI entry
// (+0x2258), the use dialog (choice or "can not use"), the check that the user's power bombs and
// missiles are full, and the hand-over to the smart object of the usable's owner. Names and layouts
// are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CInventoryComponent.h"
#include "game/CReserveTankInfo.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2523.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflObject;

// ---- The blackboard flags ----

extern u8 g_unk_7172753170[];  // a UnkProperty
extern u8 g_unk_7172753198[];  // a UnkProperty
extern u8 g_unk_71727531c0[];  // a UnkProperty

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkUsableValue_b0101 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkUsable_b0101 {
    void** vtable;
    u8 pad_0x8[0xf0];
    bool field_0xf8;
    u8 pad_0xf9;
    bool field_0xfa;
    bool field_0xfb;
    u8 pad_0xfc[0x3d4];
    UnkUsableValue_b0101* values;  // 0x4d0
    u64* keys;                     // 0x4d8
    u32 count;                     // 0x4e0

    // The blackboard section the usable's flags are stored in.
    CStrId SectionId() { return reinterpret_cast<CStrId (*)(UnkUsable_b0101*)>(vtable[0x2d8 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkUsable_b0101, values) == 0x4d0, "UnkUsable_b0101 layout");

// Stores true under `property` in the usable's section, when it has one.
static inline void StoreTrue(UnkUsable_b0101* self, u8* property) {
    CStrId section_id = self->SectionId();
    if (section_id.data != &g_unk_7101d47108->empty) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        bool value;
        u64 index = 0;
        u32 count = self->count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)reinterpret_cast<UnkProperty*>(property)->name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkUsableValue_b0101* values = self->values;
        value = true;
        SetSectionBool(blackboard, section_id, &values[index].name, &value);
    }
}

void fn_7100aa0034(UnkUsable_b0101* self) {
    StoreTrue(self, g_unk_7172753170);
}

void fn_7100aa01a4(UnkUsable_b0101* self) {
    self->field_0xf8 = true;
    StoreTrue(self, g_unk_7172753198);
}

// Stores under g_unk_71727531c0 whether the usable is usable: the negation of +0xfb when +0xfa is
// set, true otherwise.
void fn_7100a9f7f0(UnkUsable_b0101* self) {
    CStrId section_id = self->SectionId();
    if (section_id.data != &g_unk_7101d47108->empty) {
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        bool value;
        u64 index = 0;
        u32 count = self->count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)reinterpret_cast<UnkProperty*>(g_unk_71727531c0)->name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkUsableValue_b0101* values = self->values;
        value = self->field_0xfa ? !self->field_0xfb : true;
        SetSectionBool(blackboard, section_id, &values[index].name, &value);
    }
}

// ---- The pop-up message ----

void fn_7100b5fd6c(void* object, base::global::UnkStringData** name, void* block, bool a, bool b);
bool fn_7100df2568(UnkGameManager* manager);

struct UnkPopUpEntry_b0101 {
    u8 pad_0x0[0x10];
    void* composition;
};

// Shows the pop-up `name` (unless it is empty, when it returns false) on the game manager's GUI
// composition, unless fn_7100df2568 says not to.
bool fn_7100a9f94c(void* self, void* context, const char* name) {
    if (!*name)
        return false;
    if (!fn_7100df2568(g_unk_7101d47310)) {
        auto* entry = static_cast<UnkPopUpEntry_b0101*>(g_unk_7101d47310->field_0x2258);
        void* composition;
        if (entry && (composition = entry->composition)) {
            CStrId id(name, true);
            fn_7100b5fd6c(composition, &id.data, &g_unk_7101d470e0, true, false);
        }
    }
    return true;
}

// ---- The use dialog ----

struct UnkUsableDialog_b0101 {
    void** vtable;
    u8 pad_0x8[0xe0];
    UnkWeakHandle* user;  // 0xe8

    template <class F>
    F Slot(u32 offset) { return reinterpret_cast<F>(vtable[offset / 8]); }
    bool CanUse() { return Slot<bool (*)(UnkUsableDialog_b0101*)>(0x188)(this); }
    CRntString ChoiceText() { return Slot<CRntString (*)(UnkUsableDialog_b0101*)>(0x220)(this); }
    CRntString CanNotUseText() { return Slot<CRntString (*)(UnkUsableDialog_b0101*)>(0x228)(this); }
    void ShowDialog(const char* event, const char* text) {
        Slot<void (*)(UnkUsableDialog_b0101*, const char*, const char*)>(0x260)(this, event, text);
    }
    void OnChoiceShown() { Slot<void (*)(UnkUsableDialog_b0101*)>(0x298)(this); }
    void OnCanNotUseShown() { Slot<void (*)(UnkUsableDialog_b0101*)>(0x2a8)(this); }
};

static inline const char* Text(const CRntString& s) {
    return s.data ? s.data : g_unk_710162e2ed;
}

// Keeps a weak handle to `user` and shows the choice dialog, or the "can not use" one.
bool fn_7100aa19cc(UnkUsableDialog_b0101* self, UnkWeakTarget* user) {
    if (!user)
        return false;
    AssignRef(&self->user, fn_7100d78378(user));
    if (self->CanUse()) {
        {
            CRntString text = self->ChoiceText();
            self->ShowDialog("ShowDialogChoice", Text(text));
        }
        self->OnChoiceShown();
    } else {
        {
            CRntString text = self->CanNotUseText();
            self->ShowDialog("ShowDialogCanNotUse", Text(text));
        }
        self->OnCanNotUseShown();
    }
    return false;
}

// ---- Whether the user's ammunition is full ----

struct UnkInventory;
struct UnkReserveTankInfo_b49a6;

struct UnkUsableUserActor_b0101 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;
};

struct UnkUsableUserHandle_b0101 {
    u8 pad_0x0[0x10];
    UnkUsableUserActor_b0101* actor;
};

struct UnkUsableUser_b0101 {
    u8 pad_0x0[0xe8];
    UnkUsableUserHandle_b0101* user;
};

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// Whether the user's power bombs (when it has any capacity) and missiles are full and the reserve
// tank checks (the info at +0x1f0 of the game manager) pass.
bool fn_7100aa3418(UnkUsableUser_b0101* self) {
    if (!self->user)
        return false;
    UnkUsableUserActor_b0101* actor = self->user->actor;
    if (!actor)
        return false;
    UnkInventory* inventory = actor->inventory;
    f32 bomb_max = fn_7101142928(inventory, GetQualifiedId(g_unk_717275a298));
    if (!(bomb_max < 0.0f)) {
        f32 bombs = fn_7101142928(inventory, GetQualifiedId(g_unk_717275a258));
        if (bombs != bomb_max)
            return false;
    }
    f32 missile_max = fn_7101142928(inventory, GetQualifiedId(g_unk_717275a218));
    f32 missiles = fn_7101142928(inventory, GetQualifiedId(g_unk_717275a1d8));
    if (missile_max == missiles) {
        auto* info = reinterpret_cast<UnkReserveTankInfo_b49a6*>(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x1f0);
        if (!fn_7100f10960(info))
            return true;
        return fn_7100f10a2c(info);
    }
    return false;
}

// ---- The smart object ----

struct UnkSmartObject_b0101 : UnkReflObject {
    u8 pad_0x8[0x58];
    void* field_0x60;
};

struct UnkSmartObjectEntry_b0101 {
    u8 pad_0x0[0x10];
    UnkSmartObject_b0101* object;  // 0x10
};

// The smart object components (fn_7100a03058 gives the list).
struct UnkSmartObjectList_b0101 {
    UnkSmartObjectEntry_b0101** data;
    u32 count;
};

struct UnkUsableOwner_b0101 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};

struct UnkUsableComponent_b0101 {
    u8 pad_0x0[0x18];
    UnkUsableOwner_b0101* owner;  // 0x18
};

UnkSmartObjectList_b0101* fn_7100a03058();

typedef void (*UnkSmartObjectUse_b0101)(UnkSmartObject_b0101* self, void* arg);

// Hands `arg` to the last smart object component whose +0x60 is the owner's +0x30 (its vf_0x180).
bool fn_7100aa3658(UnkUsableComponent_b0101* self, void* arg) {
    UnkSmartObjectList_b0101* list = fn_7100a03058();
    UnkSmartObject_b0101* found = nullptr;
    for (UnkSmartObjectEntry_b0101 **it = list->data, **end = it + list->count; it != end; ++it) {
        UnkSmartObjectEntry_b0101* entry = *it;
        if (!entry)
            continue;
        UnkSmartObject_b0101* object = entry->object;
        if (!object)
            continue;
        if (!fn_710009d50c(object->vf_0x0(), fn_7100a00014()))
            continue;
        UnkUsableOwner_b0101* owner = self->owner;
        if (object->field_0x60 == owner->field_0x30)
            found = object;
    }
    if (found)
        (*reinterpret_cast<UnkSmartObjectUse_b0101**>(found))[0x180 / 8](found, arg);
    return true;
}
