// Unit 2640 (tools/tu_map.py; no reflected class): two more copies of fn_7101005d8c
// (src/game/UnkUnit2640/UnkUnit2640_b52.cpp), the destructor of a record of two string ids and an
// object at +0x10. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkTU2640.h"
#include "types.h"

template <int Tag>
struct UnkIdPairRecord_b55 {
    base::UnkStrIdMember id_0x0;
    base::UnkStrIdMember id_0x8;
    u8 field_0x10[0x8];
};

template <int Tag>
static inline void DestroyIdPair_b55(UnkIdPairRecord_b55<Tag>* self) {
    fn_7100ffaa88(self->field_0x10);
    self->id_0x8.~UnkStrIdMemberOf();
    self->id_0x0.~UnkStrIdMemberOf();
}

void fn_7100ff7360(UnkIdPairRecord_b55<0>* self) { DestroyIdPair_b55(self); }
void fn_7100ff7828(UnkIdPairRecord_b55<1>* self) { DestroyIdPair_b55(self); }
