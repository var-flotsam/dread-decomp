// CCollisionComponent's unit: applies a collider-state track to every record of a collision
// container: each record's flags gain or lose the masks of the id lists that name it, and changed
// objects get the lost flags cleared and the new ones set. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CStrId;

UnkCollisionObject* fn_7100863814(UnkCollisionContainer* self, const CStrId* id, const u32* extra);
void fn_71008639a8(UnkCollisionContainer* self, const CStrId* id, const u32* extra);

// A list of record ids and the flags it applies to them.
struct UnkCollisionIdMask_l861bb4 {
    base::global::CRntVector<CStrId> ids;
    u64 field_0x28;
    u32 mask;
    u8 pad_0x34[0xc];
};
static_assert(sizeof(UnkCollisionIdMask_l861bb4) == 0x40, "");

struct UnkCollisionFlagRule_l861bb4 {
    UnkCollisionIdMask_l861bb4 add;
    UnkCollisionIdMask_l861bb4 remove;
};

struct UnkCollisionTrack_l861bb4 {
    u8 unk_0x0[0x20];
    UnkCollisionFlagRule_l861bb4 rules[3];
};

// The record's object as seen here: its flags (vf_0x58) and its name at +0x20.
struct UnkCollisionNode_l861bb4 {
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
    u8 unk_0x8[0x18];
    CStrId name;  // 0x20
};

static inline UnkCollisionNode_l861bb4* Node(UnkCollisionContainer* self, u32 i) {
    return reinterpret_cast<UnkCollisionNode_l861bb4*>(fn_71007253b8(&self->records, i).field_0x28);
}

static inline bool Names(const UnkCollisionIdMask_l861bb4& list, const UnkCollisionRecord& record) {
    for (u32 k = 0; k < list.ids.count; ++k) {
        if (list.ids.data[k].data == record.field_0x8.data)
            return true;
    }
    return false;
}

void fn_7100861bb4(UnkCollisionContainer* self, void* track) {
    UnkCollisionTrack_l861bb4* rules = static_cast<UnkCollisionTrack_l861bb4*>(track);
    for (u32 i = 0; i < self->records.count; ++i) {
        u32 old = Node(self, i)->vf_0x58();
        u32 flags = static_cast<u32>(fn_71007253b8(&self->records, i).field_0x20);
        for (u32 r = 0; r < 3; ++r) {
            if (Names(rules->rules[r].add, fn_71007253b8(&self->records, i)))
                flags |= rules->rules[r].add.mask;
        }
        for (u32 r = 0; r < 3; ++r) {
            if (Names(rules->rules[r].remove, fn_71007253b8(&self->records, i)))
                flags &= ~rules->rules[r].remove.mask;
        }
        if (flags == old)
            continue;
        u32 lost = old & ~flags;
        fn_71008639a8(self, &Node(self, i)->name, &lost);
        u32 gained = flags;
        fn_7100863814(self, &Node(self, i)->name, &gained);
    }
}
