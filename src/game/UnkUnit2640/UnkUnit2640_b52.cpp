// Unit 2640 (tools/tu_map.py; no reflected class). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkTU2640.h"
#include "types.h"

// Two string ids and an object (+0x10) that fn_7100ffaa88 destroys.
struct UnkIdPairRecord_b52 {
    base::UnkStrIdMember id_0x0;
    base::UnkStrIdMember id_0x8;
    u8 field_0x10[0x8];
};

// The record's destructor: the object, then the ids in reverse order.
void fn_7101005d8c(UnkIdPairRecord_b52* self) {
    fn_7100ffaa88(self->field_0x10);
    self->id_0x8.~UnkStrIdMemberOf();
    self->id_0x0.~UnkStrIdMemberOf();
}
