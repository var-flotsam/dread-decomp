// Copy constructor of a 16-slot table of path-id list pairs: default-constructs every slot,
// then copy-constructs each one in place from the source (fn_710103a240).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathIdListPair_710103a240 {
    base::UnkVector<CFilePathStrId> first{0xf};
    base::UnkVector<CFilePathStrId> second{0xf};
};

void fn_710103a240(UnkPathIdListPair_710103a240* self, const UnkPathIdListPair_710103a240* other);

struct UnkPathIdTable_710103c4e8 {
    UnkPathIdListPair_710103a240 slots[16];
};

void fn_710103c4e8(UnkPathIdTable_710103c4e8* self, const UnkPathIdTable_710103c4e8* other) {
    new (&self->slots[0]) UnkPathIdListPair_710103a240;
    new (&self->slots[1]) UnkPathIdListPair_710103a240;
    new (&self->slots[2]) UnkPathIdListPair_710103a240;
    new (&self->slots[3]) UnkPathIdListPair_710103a240;
    new (&self->slots[4]) UnkPathIdListPair_710103a240;
    new (&self->slots[5]) UnkPathIdListPair_710103a240;
    new (&self->slots[6]) UnkPathIdListPair_710103a240;
    new (&self->slots[7]) UnkPathIdListPair_710103a240;
    new (&self->slots[8]) UnkPathIdListPair_710103a240;
    new (&self->slots[9]) UnkPathIdListPair_710103a240;
    new (&self->slots[10]) UnkPathIdListPair_710103a240;
    new (&self->slots[11]) UnkPathIdListPair_710103a240;
    new (&self->slots[12]) UnkPathIdListPair_710103a240;
    new (&self->slots[13]) UnkPathIdListPair_710103a240;
    new (&self->slots[14]) UnkPathIdListPair_710103a240;
    new (&self->slots[15]) UnkPathIdListPair_710103a240;
    fn_710103a240(&self->slots[0], &other->slots[0]);
    fn_710103a240(&self->slots[1], &other->slots[1]);
    fn_710103a240(&self->slots[2], &other->slots[2]);
    fn_710103a240(&self->slots[3], &other->slots[3]);
    fn_710103a240(&self->slots[4], &other->slots[4]);
    fn_710103a240(&self->slots[5], &other->slots[5]);
    fn_710103a240(&self->slots[6], &other->slots[6]);
    fn_710103a240(&self->slots[7], &other->slots[7]);
    fn_710103a240(&self->slots[8], &other->slots[8]);
    fn_710103a240(&self->slots[9], &other->slots[9]);
    fn_710103a240(&self->slots[10], &other->slots[10]);
    fn_710103a240(&self->slots[11], &other->slots[11]);
    fn_710103a240(&self->slots[12], &other->slots[12]);
    fn_710103a240(&self->slots[13], &other->slots[13]);
    fn_710103a240(&self->slots[14], &other->slots[14]);
    fn_710103a240(&self->slots[15], &other->slots[15]);
}
