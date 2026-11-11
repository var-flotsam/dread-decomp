// GUI::CPowerDescriptionButton's unit (tools/tu_map.py unit 1710): the description setter (fn_7100b62f9c: the
// string id at +0x308, shown on the button's label and refreshed when not empty) and the destructor
// (fn_7100b6308c: the two ids, the two strings, then GUI::CButton's destructor fn_71011bf1a4). Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/GUI/CButton.h"
#include "game/GUI/CPowerDescriptionButton.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkGuiObj;
struct UnkMissionLogButton_ovb0079;
void fn_71011ecfbc(UnkGuiObj* label, const char32_t* text);
void fn_71011cd5a0(UnkGuiObj* object);

struct UnkPowerDescriptionButton_ovb0097 {
    char* vtable;
    u8 base_0x8[0x2a0];
    CRntString field_0x2a8;
    CRntString field_0x2d8;
    union {
        CStrId description;  // 0x308
        base::UnkStrIdMember description_member;
    };
    union {
        base::UnkStrIdMember field_0x310;
    };
};
static_assert(__builtin_offsetof(UnkPowerDescriptionButton_ovb0097, field_0x310) == 0x310, "layout");

static inline UnkGuiObj* Label(UnkPowerDescriptionButton_ovb0097* self) {
    return fn_71011c08dc(reinterpret_cast<UnkMissionLogButton_ovb0079*>(self));
}

void fn_7100b62f9c(UnkPowerDescriptionButton_ovb0097* self, const CStrId* text) {
    self->description = *text;
    if (self->description.data != &g_unk_7101d47108->empty) {
        fn_71011ecfbc(Label(self), reinterpret_cast<const char32_t*>(&self->description));
        fn_71011cd5a0(Label(self));
    }
}

void fn_7100b6308c(UnkPowerDescriptionButton_ovb0097* self) {
    self->vtable = g_unk_7101a9dcb0 + 0x10;
    self->field_0x310.~UnkStrIdMemberOf();
    self->description_member.~UnkStrIdMemberOf();
    fn_710007ecd4(&self->field_0x2d8);
    fn_710007ecd4(&self->field_0x2a8);
    fn_71011bf1a4(reinterpret_cast<char*>(self));
}
