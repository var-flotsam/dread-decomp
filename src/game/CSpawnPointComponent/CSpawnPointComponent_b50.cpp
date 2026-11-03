// Describes a spawn point's settings in a property bag (unk_prop_bag, game_s7/UnkPropBagS7.h).
// Layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"

using namespace unk_prop_bag;

struct UnkObj_7101126e88 {
    UnkName* field_0x0;
    UnkName* field_0x8;
    UnkName* field_0x10;
    UnkName* field_0x18;
    bool field_0x20;
};

void fn_7101126e88(UnkObj_7101126e88* self, UnkPropBag* bag) {
    fn_710050e4dc(bag, Key(0x2140), UseName(self->field_0x0));
    fn_710050e4dc(bag, Key(0x2150), UseName(self->field_0x8));
    fn_710050e4dc(bag, Key(0x1958), UseName(self->field_0x10));
    fn_710050e4dc(bag, Key(0xaa8), UseName(self->field_0x18));
    fn_710050f4ac(bag, Key(0x1170), self->field_0x20);
}
