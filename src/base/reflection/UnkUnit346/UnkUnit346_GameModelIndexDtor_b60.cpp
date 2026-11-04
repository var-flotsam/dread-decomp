// Unit 346 (base::reflection): the destructor slot (0x40) of the reflected primitive
// gameeditor::CGameModelIndex (fn_710025d890): an array (base::UnkVector) of 16-byte entries, each
// destroyed through its operations table (the function at +0x40, which takes the entry) when it
// has one, then freed. Names are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkModelIndexEntry_b60;

struct UnkModelIndexEntryOps_b60 {
    u8 pad_0x0[0x40];
    void (*destroy)(UnkModelIndexEntry_b60* entry);
};

struct UnkModelIndexEntry_b60 {
    u64 field_0x0;
    UnkModelIndexEntryOps_b60* ops;

    ~UnkModelIndexEntry_b60() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

void fn_710025d890(base::UnkVector<UnkModelIndexEntry_b60>* self) {
    self->Free();
}
