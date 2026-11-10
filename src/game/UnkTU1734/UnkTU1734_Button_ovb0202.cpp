// Unit 1734 of tools/tu_map.py (no class hint): a mode refresh (fn_7100b92b3c) and the button add of
// fn_7100af59bc (src/game/GUI/CChangeMenuStateButton/CChangeMenuStateButton_AddButton_ovb0108.cpp) for
// a list and a template passed in: the button is created from the template (fn_71011b63c4), and, when
// it is a GUI::CButton, named "Button_<n>" after the list's count, shown, placed one step (its +0x90)
// per entry down the list, and added to the list (fn_71011d968c). Returns the button, or null. Names
// and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "../../game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

extern void* g_unk_7101a1b5a0[];
using UnkButtonName_ovb0108 = UnkBufferString<0x20, g_unk_7101a1b5a0>;
void fn_710007f634(void* self, u32 value);  // appends a number

struct UnkButtonTemplate_ovb0108 {
    u8 pad_0x0[0x28];
    void* field_0x28;
    u8 pad_0x30[0x10c];
    s32 default_value;  // 0x13c
    s32 value;          // 0x140
};

UnkGuiDisplayObject* fn_71011b63c4(UnkGuiRoot* root, UnkButtonTemplate_ovb0108* def);
void fn_71011cc940(UnkGuiDisplayObject* self, f32 value);
void fn_71011c0158(UnkGuiDisplayObject* self);

struct UnkButtonList_ovb0108 {
    u8 pad_0x0[0x1c8];
    s32 count;  // 0x1c8
};

struct UnkButtonScreen_ovb0108 {
    u8 pad_0x0[0x250];
    UnkButtonList_ovb0108* list;           // 0x250
    UnkButtonTemplate_ovb0108* button_def;  // 0x258
};

// The button as this function sees it.
struct UnkButton_ovb0108 {
    void** vtable;
    u8 pad_0x8[0x88];
    f32 step;  // 0x90
    u8 pad_0x94[0x9c];
    CStrId name;  // 0x130

    void vf_0x1d8(void* a) { reinterpret_cast<void (*)(UnkButton_ovb0108*, void*)>(vtable[0x1d8 / 8])(this, a); }
    void vf_0x78(s32 a, bool b) { reinterpret_cast<void (*)(UnkButton_ovb0108*, s32, bool)>(vtable[0x78 / 8])(this, a, b); }
};

// A string id built from a string (fn_7100000850).
struct UnkButtonId_ovb0108 {
    UnkStringData* data;

    explicit UnkButtonId_ovb0108(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkButtonId_ovb0108() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
};

UnkGuiDisplayObject* fn_7100b93d70(void* unused, UnkButtonList_ovb0108* list, UnkButtonTemplate_ovb0108* def) {
    UnkGuiDisplayObject* obj = fn_71011b63c4(g_unk_717271e8f8, def);
    if (!obj)
        return nullptr;
    if (!fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                       reinterpret_cast<const base::reflection::CType*>(fn_71011be400())))
        return nullptr;
    auto* button = reinterpret_cast<UnkButton_ovb0108*>(obj);
    s32 index = list->count;
    UnkButtonName_ovb0108 name("Button_");
    fn_710007f634(&name, index);
    if (def->field_0x28)
        button->vf_0x1d8(def->field_0x28);
    {
        UnkButtonId_ovb0108 id(*name.Str());
        button->name = reinterpret_cast<const CStrId&>(id);
    }
    fn_71011cdce8(obj, true);
    fn_71011cc940(obj, 0.0f);
    unk_gui::fn_71011ccb28(obj, button->step * (f32)index);
    fn_71011c0158(obj);
    fn_71011d968c(reinterpret_cast<UnkGuiDisplayObject*>(list), obj);
    return obj;
}

// While no value is set (+0x8c is -1) and the owner's definition does not suppress it (flag at +0x552),
// refreshes the mode globals (g_unk_71726bc570) and hands them the definition's flag at +0x551.
struct UnkModeFlags {
    u8 unk_0x0[0x100];
    u32 field_0x100;
};
extern UnkModeFlags* g_unk_71726bc570;
struct UnkRush_b0195;
void fn_7100dcc2cc(UnkRush_b0195* self);
void fn_7100dcc404(UnkRush_b0195* self, bool a, bool b, bool c);

struct UnkDefinition_b0202 {
    u8 pad_0x0[0x551];
    u8 field_0x551;
    u8 field_0x552;
};

struct UnkOwner_b0202 {
    u8 pad_0x0[0x8];
    UnkDefinition_b0202* definition;  // 0x8
};

struct UnkSource_b0202 {
    u8 pad_0x0[0x8];
    UnkOwner_b0202* owner;  // 0x8
    u8 pad_0x10[0x7c];
    s32 field_0x8c;
};

void fn_7100b92b3c(UnkSource_b0202* self) {
    if (self->field_0x8c != -1 || self->owner->definition->field_0x552)
        return;
    u32 value = g_unk_71726bc570->field_0x100;
    fn_7100dcc2cc(reinterpret_cast<UnkRush_b0195*>(g_unk_71726bc570));
    // The flags are passed on as the raw bytes and word they were read as (no bool conversion).
    reinterpret_cast<void (*)(UnkRush_b0195*, u8, u32, u32)>(fn_7100dcc404)(
        reinterpret_cast<UnkRush_b0195*>(g_unk_71726bc570), self->owner->definition->field_0x551, 0, value);
}
