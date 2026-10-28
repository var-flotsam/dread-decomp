// Releases and deletes the record held by a pointer slot (clearing the slot first): its owned
// object at 0x30 is deleted through its virtual slot 0x10, then its string id at 8 is released.
#include <new>
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

struct UnkOwned {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkRecord {
    u8 unk_0x0[8];
    UnkStrIdTwoStep field_0x8;
    u8 unk_0x10[0x20];
    UnkOwned* field_0x30;

    ~UnkRecord() {
        UnkOwned* owned = field_0x30;
        field_0x30 = nullptr;
        if (owned)
            owned->vf_0x10();
    }
};

void fn_71002ecdac(UnkRecord** slot) {
    UnkRecord* record = *slot;
    *slot = nullptr;
    delete record;
}
