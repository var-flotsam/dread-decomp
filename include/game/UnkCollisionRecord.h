#pragma once

// A collision container (the object at +0x2a0 of an actor's owner) and the records of its array at
// +0x48 (src/game/CCollisionComponent/fn_71008618cc.cpp registers them, fn_7100863a8c.cpp unregisters one by its
// id, fn_7100861148.cpp resets the objects): 0x30 bytes, three string ids, two 8-byte values and
// the collision object registered under them. UnkVector's out-of-line GrowAt over them is
// fn_71007253b8 (src/game/CBillboardCollisionComponent/fn_71007253b8.cpp). The names and the members past the ids are
// placeholders.

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A collision object (a collider or a collider group), as far as the container calls it: slot
// 0x40 takes the state value 4 when it is reset or unregistered.
struct UnkCollisionObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40(s32 value);
};

struct UnkCollisionRecord {
    base::UnkStrIdMember field_0x0;
    base::UnkStrIdMember field_0x8;
    base::UnkStrIdMember field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    UnkCollisionObject* field_0x28;

    UnkCollisionRecord();   // fn_710072676c
    ~UnkCollisionRecord();  // fn_7100726674
    UnkCollisionRecord(UnkCollisionRecord&& other) = default;
};
static_assert(sizeof(UnkCollisionRecord) == 0x30, "UnkCollisionRecord placeholder layout");

// The element at `index` of the container's records, growing the array to cover it first
// (UnkVector::GrowAt, out of line). Defined in src/game/CBillboardCollisionComponent/fn_71007253b8.cpp.
UnkCollisionRecord& fn_71007253b8(base::UnkVector<UnkCollisionRecord>* self, u32 index);

// The container as fn_7100863a8c sees it: the records at +0x48 and the registered objects at +0x98.
struct UnkCollisionContainer {
    u8 unk_0x0[0x48];
    base::UnkVector<UnkCollisionRecord> records;
    u8 unk_0x70[0x28];
    base::UnkVector<UnkCollisionObject*> objects;
};

// Unregisters the object stored under the string id `id`: detaches it from the collision tracker,
// removes its record and hands it to the manager. Defined in src/game/CCollisionComponent/fn_7100863a8c.cpp.
void fn_7100863a8c(UnkCollisionContainer* self, const base::global::CStrId* id);

// Looks the object registered under the string id `name` up in the records (refreshing the record
// through the container's vf_0x170 when its flag at +0x18 is set) and returns it. The callers reach
// the container through their owner's +0x2a0 and look components up by name ("damage").
// tools/tu_map.py puts it in the CCollisionComponent unit. Not decompiled yet.
UnkCollisionObject* fn_71008640a4(UnkCollisionContainer* self, const base::global::CStrId* name);
