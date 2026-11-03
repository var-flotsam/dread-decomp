// CFactionComponent unit (tools/tu_map.py): sets the faction from its name once. While the kind at
// +0x70 is still 0 and the name is not empty, "Players", "Neutral" and "Enemies" give the kinds 1, 2
// and 4 and the name is copied to +0x40 (fn_710007eaa0); returns whether it was taken. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

struct UnkFactionComponent_c3 {
    u8 base_0x0[0x40];
    CRntString name;  // 0x40
    u32 kind;         // 0x70
};

bool fn_710113906c(UnkFactionComponent_c3* self, const CRntString* name) {
    if (self->kind != 0)
        return false;
    if (!name->data || !name->data[0])
        return false;
    if (fn_710007f140(name, "Players")) {
        self->kind = 1;
        fn_710007eaa0(&self->name, name);
        return true;
    }
    u32 kind;
    if (fn_710007f140(name, "Neutral"))
        kind = 2;
    else if (fn_710007f140(name, "Enemies"))
        kind = 4;
    else
        return false;
    self->kind = kind;
    fn_710007eaa0(&self->name, name);
    return true;
}
