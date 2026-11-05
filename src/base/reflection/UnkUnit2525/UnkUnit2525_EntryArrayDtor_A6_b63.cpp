// Unit 2525 (base::reflection): a destructor slot with the same code as fn_710025d890
// (src/base/reflection/UnkUnit346/UnkUnit346_GameModelIndexDtor_b60.cpp): an array (base::UnkVector) of 16-byte
// entries, each destroyed through its operations table (the function at +0x40) when it has one, then freed.
// fn_7100f7b200 registers it. Names are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkOpsEntry_A6_b63;

struct UnkOpsEntryOps_A6_b63 {
    u8 pad_0x0[0x40];
    void (*destroy)(UnkOpsEntry_A6_b63* entry);
};

struct UnkOpsEntry_A6_b63 {
    u64 field_0x0;
    UnkOpsEntryOps_A6_b63* ops;

    ~UnkOpsEntry_A6_b63() {
        if (ops) {
            ops->destroy(this);
            ops = nullptr;
        }
    }
};

void fn_7100d8ce3c(void* self) {
    static_cast<base::UnkVector<UnkOpsEntry_A6_b63>*>(self)->Free();
}
