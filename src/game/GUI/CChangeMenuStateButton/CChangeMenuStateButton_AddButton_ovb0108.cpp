// GUI::CChangeMenuStateButton's unit (tools/tu_map.py): adds a button to a paged list. The button is
// created from the template at +0x258 (fn_71011b63c4); when it is a GUI::CButton it is named
// "Button_<n>" after the list's count, shown, placed one step (its +0x90) per entry down the list,
// given the template's value (+0x140, or +0x13c when unset) through its vtable slot 0x78, and added
// to the list (+0x250, fn_71011d968c). Returns the button, or null. Names and layouts are
// placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "../../../game_s7/UnkGuiS7.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CButton.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2744.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;

extern void* g_unk_7101a1b5a0[];
using UnkButtonName_ovb0108 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

struct UnkButtonTemplate_ovb0108 {
    u8 pad_0x0[0x28];
    void* field_0x28;
    u8 pad_0x30[0x10c];
    s32 default_value;  // 0x13c
    s32 value;          // 0x140
};

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

UnkGuiDisplayObject* fn_7100af59bc(UnkButtonScreen_ovb0108* self) {
    UnkGuiDisplayObject* obj = fn_71011b63c4(g_unk_717271e8f8, self->button_def);
    if (!obj)
        return nullptr;
    if (!fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(obj->GetClass()),
                       reinterpret_cast<const base::reflection::CType*>(fn_71011be400())))
        return nullptr;
    auto* button = reinterpret_cast<UnkButton_ovb0108*>(obj);
    s32 index = self->list->count;
    UnkButtonName_ovb0108 name("Button_");
    fn_710007f634(&name, index);
    button->vf_0x1d8(self->button_def->field_0x28);
    {
        UnkButtonId_ovb0108 id(*name.Str());
        button->name = reinterpret_cast<const CStrId&>(id);
    }
    fn_71011cdce8(obj, true);
    fn_71011cc940(obj, 0.0f);
    unk_gui::fn_71011ccb28(obj, button->step * (f32)index);
    s32 value = self->button_def->value;
    if (value == 0x7fffffff)
        value = self->button_def->default_value;
    button->vf_0x78(value, true);
    fn_71011c0158(obj);
    fn_71011d968c(reinterpret_cast<UnkGuiDisplayObject*>(self->list), obj);
    return obj;
}
