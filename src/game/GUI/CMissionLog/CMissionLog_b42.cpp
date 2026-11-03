// GUI::CMissionLog's unit (tools/tu_map.py): the "set element at index" functions (slots 0x140 and
// 0x148 of the reflected collection type) of the array of 0x38-byte mission-log records (a value, a
// string id and an array of ids) that fn_7100b5bfdc copies. An index past the end of a growable array
// first reserves its capacity (grown to the next multiple of the step when the index reaches it) and
// then index + 1 records (fn_7100b5c28c), constructs the new ones (fn_7100b5cbf0) and extends the
// count; the record is then copy-assigned (the value, the id, then the array out of line,
// fn_71000ce19c) or move-assigned (fn_7100b5b31c). The shape of fn_7100c92080
// (src/game/CCutSceneDef/CCutSceneDef_b42.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// The element and array types of src/game/GUI/CMissionLog/fn_7100b5bfdc.cpp (same names, same
// layouts), so the reserve keeps that file's signature.
struct UnkElem38 {
    u8 unk_0x0[0x38];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

using UnkElem38Array = UnkElemArray<UnkElem38>;

void fn_7100b5c28c(UnkElem38Array* self, u32 capacity);
void fn_7100b5cbf0(UnkElem38* self);  // the record's constructor
// As src/game/GUI/CMissionLog/UnkMissionLogRecordMove.cpp defines it.
struct UnkMissionLogRecord;
UnkMissionLogRecord* fn_7100b5b31c(UnkMissionLogRecord* self, UnkMissionLogRecord* other);
// The id array's copy assignment, as src/game/CGameManager.cpp declares it.
struct UnkStruct_0x28;
void fn_71000ce19c(UnkStruct_0x28*, const UnkStruct_0x28*);

struct UnkMissionLogRecord_b42a3 {
    u32 field_0x0;
    CStrId id;
    u8 ids[0x28];
};

static inline void Grow_b42a3(UnkElem38Array* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_7100b5c28c(self, capacity);
    u32 n = index + 1;
    fn_7100b5c28c(self, n);
    for (u32 i = self->count; i < n; ++i)
        fn_7100b5cbf0(&self->data[i]);
    self->count = n;
}

void fn_7100b5bd74(UnkElem38Array* self, const u32* index, const UnkMissionLogRecord_b42a3* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b42a3(self, i);
    UnkElem38* data = self->data;
    u32 v = value->field_0x0;
    auto* e = reinterpret_cast<UnkMissionLogRecord_b42a3*>(&data[i]);
    e->field_0x0 = v;
    e->id = value->id;
    fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(e->ids), reinterpret_cast<const UnkStruct_0x28*>(value->ids));
}

void fn_7100b5beb4(UnkElem38Array* self, const u32* index, UnkMissionLogRecord* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_b42a3(self, i);
    fn_7100b5b31c(reinterpret_cast<UnkMissionLogRecord*>(&self->data[i]), value);
}
