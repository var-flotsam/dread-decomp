// CGrappleBeamComponent unit (1277): appends a record to the fixed-capacity list (u32 count at +0x8, up to 16
// records of 0x48 bytes from +0xc) and returns a status through a result struct: 2 when full, else 3 when the
// list is now full and 1 when not. Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "types.h"

void fn_7100331c7c(void* record, UnkNoArg);

struct UnkGrappleRecord_l81cc58 {
    u8 pad_0x0[0x48];
};

struct UnkRecords_l81cc58 {
    u32 count;
    UnkGrappleRecord_l81cc58 items[16];
    s32 Remaining() const { return 16 - count; }
};

struct UnkGrappleRecordList_l81cc58 {
    void* pad_0x0;
    UnkRecords_l81cc58 records;
};

struct UnkAddResult_l81cc58 {
    u32 status;
    UnkAddResult_l81cc58(const UnkAddResult_l81cc58& other) : status(other.status) {}
    explicit UnkAddResult_l81cc58(u32 s) : status(s) {}
};

UnkAddResult_l81cc58 fn_710081cc58(UnkGrappleRecordList_l81cc58* self) {
    UnkRecords_l81cc58& records = self->records;
    s32 room = records.Remaining();
    if (room < 1)
        return UnkAddResult_l81cc58(2);
    fn_7100331c7c(&records.items[records.count], UnkNoArg());
    records.count++;
    return UnkAddResult_l81cc58(room == 1 ? 3 : 1);
}
