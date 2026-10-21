// Default constructor of the 16-slot table of path-id list pairs (the copy constructor is
// fn_710103c4e8): every list is empty, growth step 10, allocator flags 0xf.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkPathIdListPair_710103a240 {
    base::UnkVector<CFilePathStrId> first{0xf};
    base::UnkVector<CFilePathStrId> second{0xf};
};


struct UnkPathIdTable_710103c4e8 {
    UnkPathIdListPair_710103a240 slots[16];
};

void fn_7101036a9c(UnkPathIdTable_710103c4e8* self) {
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
}
