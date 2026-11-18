// GUI::CProfileButton's unit (tools/tu_map.py unit 1712): gaining focus. After CButton's step
// (fn_71011bf558), the "Content.Options" label of the button's container is shown when the button's
// profile slot (on the main menu) is used, hidden otherwise. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;
using base::reflection::CType;

// As src/game/UnkTU2621/UnkTU2621_Save_ovb0235.cpp declares them.
struct alignas(8) UnkEntryData_b0235 {
    u8 bytes[0x80];
    CRntString name;  // 0x80
};
void fn_7100afa4c0(UnkEntryData_b0235* self, const UnkEntryData_b0235* other);

// As src/game/GUI/CButton/CButton_Focus_ovb0000.cpp and src/game/GUI/CChangeMenuStateButton/fn_7100af65b4.cpp
// declare them.
struct UnkGuiButtonFocus_ovb0000;
void fn_71011bf558(UnkGuiButtonFocus_ovb0000* self);
struct UnkObj_7100af65b4;
UnkGuiDisplayObject* fn_71011c068c(UnkObj_7100af65b4* self);

// A profile slot's entry, copied out of line (fn_7100afa4c0): whether the slot is used, then the
// profile's name, the only member that needs destroying.
struct UnkProfileEntryCopy_lb64a24 {
    bool used;
    u8 pad_0x1[0x7f];
    alignas(8) u8 name[sizeof(CRntString)];  // 0x80

    explicit UnkProfileEntryCopy_lb64a24(const UnkEntryData_b0235& other) {
        fn_7100afa4c0(reinterpret_cast<UnkEntryData_b0235*>(this), &other);
    }
    ~UnkProfileEntryCopy_lb64a24() { reinterpret_cast<CRntString*>(name)->~CRntString(); }
};
static_assert(sizeof(UnkProfileEntryCopy_lb64a24) == sizeof(UnkEntryData_b0235), "entry");

// A profile slot is four 0x50-byte records; the slot's entry starts at the second.
struct UnkProfileRecord_lb64a24 {
    u8 bytes[0x50];
};

struct UnkMainMenu_lb64a24 {
    u8 pad_0x0[0xb0];
    UnkProfileRecord_lb64a24 records[3 * 4];  // 0xb0

    // Slots past the third fall back to the first.
    const UnkEntryData_b0235& Entry(u32 index) {
        return reinterpret_cast<const UnkEntryData_b0235&>(records[index < 3 ? static_cast<u64>(index) * 4 + 1 : 1]);
    }
};

struct UnkProfileButton_lb64a24 {
    u8 pad_0x0[0x2a8];
    s8 slot;  // 0x2a8
};

template <const base::reflection::CClass* (*GetClass)()>
static inline UnkGuiDisplayObject* AsClass(UnkGuiDisplayObject* obj) {
    if (obj && !fn_710009d50c(reinterpret_cast<const CType*>(obj->GetClass()), reinterpret_cast<const CType*>(GetClass())))
        obj = nullptr;
    return obj;
}

void fn_7100b64a24(UnkProfileButton_lb64a24* self) {
    fn_71011bf558(reinterpret_cast<UnkGuiButtonFocus_ovb0000*>(self));
    UnkGuiDisplayObject* container =
        AsClass<fn_71011d7b84>(fn_71011c068c(reinterpret_cast<UnkObj_7100af65b4*>(self)));
    UnkGuiDisplayObject* label;
    {
        CRntString path("Content.Options");
        label = AsClass<fn_71011e8dfc>(fn_71011d9508(container, reinterpret_cast<const char* const*>(&path)));
    }
    if (!label)
        return;
    auto* menu = static_cast<UnkMainMenu_lb64a24*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")));
    if (!menu)
        return;
    UnkProfileEntryCopy_lb64a24 entry(menu->Entry(self->slot));
    fn_71011cdce8(label, entry.used);
}
