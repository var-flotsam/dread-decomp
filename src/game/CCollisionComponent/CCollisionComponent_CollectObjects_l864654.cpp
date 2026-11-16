// CCollisionComponent unit (tools/tu_map.py unit 1319): appends to `out` every object whose mask
// (vf_0x58) shares a bit with `mask`, skipping disabled objects unless `all` is set, and refreshing
// each flagged record through the container's vf_0x170 first. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkCollisionShape_l864654 {
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
    virtual u32 vf_0x58();
    u8 unk_0x8[0x62];
    bool enabled;  // 0x6a
};

struct UnkCollisionRecord_l864654 {
    u8 unk_0x0[0x18];
    bool dirty;  // 0x18
    u8 unk_0x19[0xf];
    UnkCollisionShape_l864654* object;  // 0x28
};
static_assert(sizeof(UnkCollisionRecord_l864654) == 0x30, "record size");

struct UnkCollisionContainer_l864654 {
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
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118();
    virtual void vf_0x120();
    virtual void vf_0x128();
    virtual void vf_0x130();
    virtual void vf_0x138();
    virtual void vf_0x140();
    virtual void vf_0x148();
    virtual void vf_0x150();
    virtual void vf_0x158();
    virtual void vf_0x160();
    virtual void vf_0x168();
    virtual void vf_0x170(UnkCollisionRecord_l864654* record, s32 a);
    u8 unk_0x8[0x40];
    UnkCollisionRecord_l864654* records;  // 0x48
    u32 recordCount;                      // 0x50
};

void fn_7100864654(UnkCollisionContainer_l864654* self, u32 mask,
                   base::global::CRntVector<UnkCollisionShape_l864654*>* out, bool all) {
    out->Reserve(self->recordCount);
    for (u64 i = 0; i < self->recordCount; ++i) {
        UnkCollisionRecord_l864654* record = &self->records[i];
        if (!all && !record->object->enabled)
            continue;
        if (!(record->object->vf_0x58() & mask))
            continue;
        if (record->dirty)
            self->vf_0x170(record, 0);
        UnkCollisionShape_l864654* object = record->object;
        out->PushBack(object);
    }
}
