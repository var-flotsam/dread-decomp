// CCollisionComponent unit (game/UnkCollisionRecord.h). fn_7100861710: while the container is
// active (+0xc8) and either refreshing is allowed (+0xc9) or forced, refreshes
// every record whose flag (the low byte of +0x18) is set through the container's vf_0x170; a record
// whose object reports nothing (its slot 0x58) instead gets its count at +0x1c cleared. Names and
// layouts are placeholders.
#include "game/UnkCollisionRecord.h"

typedef void (*UnkCollisionRefresh_b42)(void* self, UnkCollisionRecord* record, bool force);

struct UnkCollisionQuery_b42 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual s32 vf_0x58();
};

struct UnkCollisionContainer_b42 {
    UnkCollisionRefresh_b42* vtable;
    u8 unk_0x8[0x40];
    base::UnkVector<UnkCollisionRecord> records;  // 0x48
    u8 unk_0x70[0x58];
    bool active;  // 0xc8
    bool refresh;  // 0xc9
};

void fn_7100861710(UnkCollisionContainer_b42* self, bool force) {
    if (self->active && (self->refresh || force)) {
        for (u32 i = 0; i < self->records.count; ++i) {
            UnkCollisionRecord& record = fn_71007253b8(&self->records, i);
            if (static_cast<u8>(record.field_0x18)) {
                if (reinterpret_cast<UnkCollisionQuery_b42*>(record.field_0x28)->vf_0x58())
                    self->vtable[0x170 / 8](self, &record, force);
                else
                    reinterpret_cast<u32*>(&record.field_0x18)[1] = 0;
            }
        }
    }
}
