// GUI::CPowerDescriptionButton's unit (tools/tu_map.py unit 1710): shows the button's power on the
// in-game menu's power description composition: its two texts on the composition's labels, the
// music chosen by the power's id, then the composition itself. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkPowerDescCompositionMusic;
struct UnkPowerDescCompositionShow;

void fn_7100b641cc(UnkPowerDescCompositionMusic* self, u32 mode);
void fn_7100b63ca4(UnkPowerDescCompositionShow* self);
void fn_71011f9778(UnkGuiDisplayObject* self, const CStrId& name);
__attribute__((visibility("hidden"))) extern CStrId g_unk_71727656e8;

// The in-game menu's slot holding the power description composition.
struct UnkPowerDescSlot_b62a00 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* composition;
};
struct UnkIngameMenuView_b62a00 {
    u8 pad_0x0[0x488];
    UnkPowerDescSlot_b62a00* power_description;
};

struct UnkPowerDescriptionButton_b62a00 {
    u8 pad_0x0[0x2a8];
    CRntString name;  // 0x2a8
    CRntString text;  // 0x2d8
    u8 pad_0x308[0x8];
    CStrId power;                        // 0x310
    UnkGuiDisplayObject* name_label;     // 0x318
    UnkGuiDisplayObject* text_label;     // 0x320
    UnkGuiDisplayObject* power_object;   // 0x328
};
static_assert(__builtin_offsetof(UnkPowerDescriptionButton_b62a00, power) == 0x310, "layout");

static inline bool HasText(const CRntString& text) {
    return text.data && *text.data;
}

void fn_7100b62a00(UnkPowerDescriptionButton_b62a00* self) {
    UnkIngameMenuView_b62a00* menu = reinterpret_cast<UnkIngameMenuView_b62a00*>(fn_7100deac84(g_unk_7101d47310));
    UnkPowerDescSlot_b62a00* slot = menu->power_description;
    if (!slot)
        return;
    base::reflection::UnkReflObject* composition = slot->composition;
    if (!composition)
        return;
    if (!fn_710009d50c(composition->vf_0x0(), fn_7100b6343c()))
        return;
    if (HasText(self->name) && self->name_label)
        fn_71011ec424(self->name_label, self->name);
    if (HasText(self->text) && self->text_label)
        fn_71011ec424(self->text_label, self->text);
    if (self->power.data != &g_unk_7101d47108->empty && self->power_object) {
        fn_71011f9778(self->power_object, g_unk_71727656e8);
        // The music the composition plays for the power.
        UnkPowerDescCompositionMusic* music = reinterpret_cast<UnkPowerDescCompositionMusic*>(composition);
        switch (self->power.data->Key()) {
        case 0xB1C532D54CD43A33:
            fn_7100b641cc(music, 44);
            break;
        case 0x7C80C924A80A26EE:
            fn_7100b641cc(music, 45);
            break;
        case 0x1C22AFDD119957B7:
            fn_7100b641cc(music, 46);
            break;
        case 0x590BCEAB01A5024A:
            fn_7100b641cc(music, 47);
            break;
        case 0xFCF447BD7A52007B:
            fn_7100b641cc(music, 48);
            break;
        case 0x76A423A842B16D66:
            fn_7100b641cc(music, 49);
            break;
        case 0xB4E37FAE52F6CF88:
            fn_7100b641cc(music, 50);
            break;
        case 0x0E98B2698F516384:
            fn_7100b641cc(music, 51);
            break;
        case 0xC186D81A332A2CDC:
            fn_7100b641cc(music, 52);
            break;
        case 0x0CC0CB393A145B82:
            fn_7100b641cc(music, 53);
            break;
        case 0xE311725B5D4164EC:
            fn_7100b641cc(music, 54);
            break;
        case 0x27A4D01C1A2E20D6:
            fn_7100b641cc(music, 55);
            break;
        case 0xA4FBCD7FD79D9CE1:
            fn_7100b641cc(music, 56);
            break;
        case 0x9ED2F5D271B1EE04:
            fn_7100b641cc(music, 57);
            break;
        case 0x7112B23208BE6DC4:
            fn_7100b641cc(music, 58);
            break;
        case 0x0A05D6C94287DEFE:
            fn_7100b641cc(music, 59);
            break;
        case 0xA672B84B8BFBB5E2:
            fn_7100b641cc(music, 60);
            break;
        case 0x2F5B6974B8099C57:
            fn_7100b641cc(music, 61);
            break;
        case 0x7BD89D866C336119:
            fn_7100b641cc(music, 62);
            break;
        case 0x7FEE347D178AA06C:
            fn_7100b641cc(music, 63);
            break;
        case 0xA4BD6FDB2D7E0B0E:
            fn_7100b641cc(music, 64);
            break;
        case 0x6110B66971F38E81:
            fn_7100b641cc(music, 65);
            break;
        case 0x84AA46E2F7A4A89E:
            fn_7100b641cc(music, 66);
            break;
        case 0xDD422FEA1BAEB6C5:
            fn_7100b641cc(music, 67);
            break;
        case 0xF63250230582EB0B:
            fn_7100b641cc(music, 68);
            break;
        case 0xBFD11FD72AF3FD4F:
            fn_7100b641cc(music, 69);
            break;
        case 0x1094839B222287CC:
            fn_7100b641cc(music, 70);
            break;
        case 0xBD731E937F6FEA48:
            fn_7100b641cc(music, 71);
            break;
        }
    }
    fn_7100b63ca4(reinterpret_cast<UnkPowerDescCompositionShow*>(composition));
}
