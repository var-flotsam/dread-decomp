// GUI::CSlotOptionsComposition: its event handler. After the container's own, an event of kind 2 titles
// the options with the selected file, labels the two copy buttons with the other two slots (empty or in use)
// and the delete button. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkGuiEvent_b49 {
    u8 pad_0x0[0x20];
    u64 kind;  // 0x20
};

struct UnkSlotOptions_lb7bea4 : UnkGuiDisplayObject {
    u8 pad_0x198[0x98];
    s32 slot;  // 0x230
};
static_assert(__builtin_offsetof(UnkSlotOptions_lb7bea4, slot) == 0x230, "layout");

// A copy or delete button: the slot it acts on and the one it copies to.
struct UnkSlotButton_lb7bea4 : UnkGuiDisplayObject {
    u8 pad_0x198[0x128];
    s32 source;  // 0x2c0
    s32 target;  // 0x2c4
};
static_assert(__builtin_offsetof(UnkSlotButton_lb7bea4, target) == 0x2c4, "layout");

struct UnkSaveEntry_lb7bea4 {
    u8 pad_0x0[0x50];
};

// The main menu's save entries, four for each of the three slots.
struct UnkMainMenu_lb7bea4 {
    u8 pad_0x0[0xb0];
    UnkSaveEntry_lb7bea4 entries[12];  // 0xb0
};

// What a save entry says about its slot: whether it is used, and its text.
struct UnkSlotSummary_lb7bea4 {
    bool in_use;
    u8 pad_0x1[0x7f];
    CRntString text;  // 0x80

    UnkSlotSummary_lb7bea4(const UnkSaveEntry_lb7bea4* entry) asm("fn_7100afa4c0");
};

struct UnkMissionLogButton_ovb0079;

// The object when it is of the class `Type` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Type)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* o) {
    return o && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(o->GetClass()),
                              reinterpret_cast<const base::reflection::CType*>(Type()))
               ? o
               : nullptr;
}

void fn_7100b7bea4(UnkSlotOptions_lb7bea4* self, UnkGuiEvent_b49* event) {
    fn_71011d99e4(self, event);
    if (event->kind != 2)
        return;
    UnkGuiDisplayObject* content = As<fn_71011d7b84>(fn_71011d94d4(self, CStrId("Content", true)));
    if (!content)
        return;
    if (UnkGuiDisplayObject* title = As<fn_71011e8dfc>(fn_71011d94d4(content, CStrId("FileSelect", true)))) {
        CRntString text("#GUI_FILESELECT_OPTIONS_TITLE_FILE_");
        fn_710007f634(&text, self->slot + 1);
        fn_71011ec424(title, text);
    }
    CStrId first("FirstSlotButton", true);
    CStrId second("SecondSlotButton", true);
    if (self->slot == 1) {
        first = CStrId("SecondSlotButton", true);
        second = CStrId("FirstSlotButton", true);
    }
    UnkGuiDisplayObject* list = As<fn_71011fe904>(fn_71011d94d4(content, CStrId("ListComposition", true)));
    if (!list)
        return;
    // The two copy buttons, labelled with the slots one and two after the selected one.
    const CStrId* names[2] = {&first, &second};
    for (s32 i = 0; i < 2; i++) {
        UnkGuiDisplayObject* object = fn_71011d94d4(list, *names[i]);
        if (!object || !fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(object->GetClass()),
                                      reinterpret_cast<const base::reflection::CType*>(fn_7100afadc8())))
            continue;
        UnkSlotButton_lb7bea4* button = static_cast<UnkSlotButton_lb7bea4*>(object);
        s32 slot = (self->slot + i + 1) % 3;
        CRntString text("#GUI_FILESELECT_OPTIONS_LABEL_COPY_");
        const char* state = "EMPTY_";
        if (auto* menu = static_cast<UnkMainMenu_lb7bea4*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")))) {
            u64 index = (u32)slot < 3 ? (u32)slot : 0;
            if (UnkSlotSummary_lb7bea4(&menu->entries[index * 4 + 1]).in_use)
                state = "INUSE_";
        }
        CRntString_AppendCString(&text, state);
        fn_710007f634(&text, slot + 1);
        button->target = slot;
        button->source = self->slot;
        fn_71011ec424(fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(button)), text);
    }
    if (UnkGuiDisplayObject* object = As<fn_7100b00714>(fn_71011d94d4(list, CStrId("DeleteSlotButton", true)))) {
        UnkSlotButton_lb7bea4* button = static_cast<UnkSlotButton_lb7bea4*>(object);
        button->source = self->slot;
        UnkGuiDisplayObject* label = fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(button));
        fn_71011ec424(label, CRntString("#GUI_FILESELECT_OPTIONS_LABEL_DELETE"));
    }
}
