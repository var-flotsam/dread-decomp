// GUI::CProfileButton's unit (tools/tu_map.py unit 1712): after the base step (fn_71011bf3cc), when the
// main menu is up, copies the button's profile slot entry (slots past the third fall back to the first)
// and plays the sound event for a used or an empty profile. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

using base::global::CRntString;

// As src/game/UnkTU2621/UnkTU2621_Save_ovb0235.cpp declares them.
struct alignas(8) UnkEntryData_b0235 {
    u8 bytes[0x80];
    CRntString name;  // 0x80
};
void fn_7100afa4c0(UnkEntryData_b0235* self, const UnkEntryData_b0235* other);

// As src/game/UnkTU2666/UnkTU2666_lua_ovb0163.cpp declares it.
void fn_7100df9728(UnkGameManager* self);

// As src/game/sound/SGUISoundEventsSelector/fn_7100fcbf50.cpp declares them.
extern unk_static_init::UnkEnumType g_unk_71729f01f0;
extern unk_static_init::UnkEnumType g_unk_71729f01f8;

void fn_71011bf3cc(void* self);

// A profile slot's entry, copied out of line (fn_7100afa4c0); only its name needs destroying.
struct UnkProfileEntryCopy_lb64b64 {
    alignas(8) u8 storage[sizeof(UnkEntryData_b0235)];

    explicit UnkProfileEntryCopy_lb64b64(const UnkEntryData_b0235& other) { fn_7100afa4c0(Data(), &other); }
    ~UnkProfileEntryCopy_lb64b64() { Data()->name.~CRntString(); }
    UnkEntryData_b0235* Data() { return reinterpret_cast<UnkEntryData_b0235*>(storage); }
    bool IsUsed() { return Data()->bytes[0] != 0; }
};

// A profile slot is four 0x50-byte records; the slot's entry starts at the second.
struct UnkProfileRecord_lb64b64 {
    u8 bytes[0x50];
};

struct UnkMainMenu_lb64b64 {
    u8 pad_0x0[0xb0];
    UnkProfileRecord_lb64b64 records[3 * 4];  // 0xb0

    // Slots past the third fall back to the first.
    const UnkEntryData_b0235& Entry(u32 index) {
        u32 slot = index < 3 ? index : 0;
        return reinterpret_cast<const UnkEntryData_b0235&>(records[slot * 4 + 1]);
    }
};

struct UnkProfileButton_lb64b64 {
    u8 pad_0x0[0x2a8];
    s8 slot;  // 0x2a8
};

void fn_7100b64b64(UnkProfileButton_lb64b64* self) {
    fn_71011bf3cc(self);
    auto* menu = static_cast<UnkMainMenu_lb64b64*>(fn_7100deab70(g_unk_7101d47310, CRntString("MAINMENU")));
    if (!menu)
        return;
    fn_7100df9728(g_unk_7101d47310);
    UnkProfileEntryCopy_lb64b64 entry(menu->Entry(self->slot));
    fn_7100fc9d78(g_unk_717275c380,
                  reinterpret_cast<UnkStateKey*>(entry.IsUsed() ? &g_unk_71729f01f8 : &g_unk_71729f01f0), self);
}
