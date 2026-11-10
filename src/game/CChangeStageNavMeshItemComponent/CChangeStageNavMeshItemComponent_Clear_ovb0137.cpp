// CChangeStageNavMeshItemComponent's unit (tools/tu_map.py): emptying the component's parallel id and id-list arrays.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// Two parallel arrays sharing a count: a list of ids for each entry (0x28-byte arrays) and an id.
struct UnkStageNavMeshItems_ovb0137 {
    base::UnkVector<base::UnkStrIdMember>* lists;  // 0x0
    base::UnkStrIdMember* ids;                     // 0x8
    u32 count;                                     // 0x10
};

// Empties both arrays (the ids released, each list's array freed), keeping their storage.
void fn_71009ba68c(UnkStageNavMeshItems_ovb0137* self) {
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdMemberOf();
    }
    if (self->lists) {
        for (u64 i = 0; i < self->count; ++i)
            self->lists[i].Free();
    }
    self->count = 0;
}
