// Unit 1629 of tools/tu_map.py (no name hint). A record of a name, a vector of children at +0x20 (each
// child keeps the addresses of the records it belongs to) and a vector of string ids at +0x48: its copy
// constructor (fn_7100acfb14) and destructor (fn_7100ad04c4), the filter that collects the children
// accepted for a pair of arguments (fn_7100ad0380) and the copy assignment of a vector of records
// (fn_7100ad22a4, whose elements are copied, moved and destroyed out of line). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU1629.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_b257g;

// An object that keeps the addresses of the records it belongs to (a vector at +0x20).
struct UnkChild_b257g {
    u8 pad_0x0[0x20];
    UnkRecord_b257g** parents;  // 0x20
    u32 parent_count;  // 0x28
};

struct UnkRecord_b257g {
    u32 field_0x0;
    u32 field_0x4;
    CStrId id;  // 0x8
    u8 field_0x10;
    void* field_0x18;
    base::UnkVector<UnkChild_b257g*> children;  // 0x20
    base::UnkStrIdVec ids;  // 0x48
    void* field_0x70;
    u8 field_0x78;
};
static_assert(__builtin_offsetof(UnkRecord_b257g, ids) == 0x48, "layout");
static_assert(__builtin_offsetof(UnkRecord_b257g, field_0x78) == 0x78, "layout");

// The destructor: the ids and the children vectors are released and freed, then the name.
void fn_7100ad04c4(void* record) {
    UnkRecord_b257g* self = static_cast<UnkRecord_b257g*>(record);
    if (self->ids.data) {
        for (u64 i = 0; i < self->ids.count; ++i)
            self->ids.data[i].Reset();
        self->ids.count = 0;
        self->ids.allocator->vf_0x48(self->ids.data);
        self->ids.capacity = 0;
    }
    self->children.Free();
    self->id.Reset();
}

void fn_7100acfb14(void* record, const void* source) {
    UnkRecord_b257g* self = static_cast<UnkRecord_b257g*>(record);
    const UnkRecord_b257g* other = static_cast<const UnkRecord_b257g*>(source);
    self->field_0x0 = other->field_0x0;
    self->field_0x4 = other->field_0x4;
    new (&self->id) CStrId(other->id);
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    self->children.data = nullptr;
    self->children.count = 0;
    self->children.capacity = 0;
    self->children.step = other->children.step;
    self->children.field_0x14 = other->children.field_0x14;
    self->children.flags = other->children.flags;
    self->children.allocator = other->children.allocator;
    self->children.Reserve(other->children.capacity);
    __builtin_memcpy(self->children.data, other->children.data, (u64)other->children.count * 8);
    self->children.count = other->children.count;
    new (&self->ids) base::UnkStrIdVec(other->ids);
    self->field_0x70 = other->field_0x70;
    self->field_0x78 = other->field_0x78;
}

bool fn_7100ad0294(UnkRecord_b257g* self, bool a, u32 b, UnkChild_b257g* child);

// Appends the children the record accepts for (a, b) to `out`.
void fn_7100ad0380(UnkRecord_b257g* self, bool a, u32 b, base::UnkVector<UnkChild_b257g*>* out) {
    out->count = 0;
    if (!self->children.count)
        return;
    UnkChild_b257g** it = self->children.data;
    UnkChild_b257g** end = it + self->children.count;
    for (; it != end; ++it) {
        UnkChild_b257g* child = *it;
        if (fn_7100ad0294(self, a, b, child))
            out->PushBack(child);
    }
}

// The record as the vectors of records see it: moved, copied and destroyed out of line.

struct UnkRecordElem_b257h {
    u8 bytes[0x80];
    __attribute__((always_inline)) UnkRecordElem_b257h(UnkRecordElem_b257h&& other) { fn_7100acf988(this, &other); }
    __attribute__((always_inline)) UnkRecordElem_b257h(const UnkRecordElem_b257h& other) { fn_7100acfb14(this, &other); }
    __attribute__((always_inline)) ~UnkRecordElem_b257h() { fn_7100ad04c4(this); }
};

using UnkRecordVec_b257h = base::UnkVector<UnkRecordElem_b257h>;

UnkRecordVec_b257h* fn_7100ad22a4(UnkRecordVec_b257h* self, const UnkRecordVec_b257h* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        if (other->count) {
            const UnkRecordElem_b257h* it = other->data;
            const UnkRecordElem_b257h* end = it + other->count;
            for (; it != end; ++it)
                self->PushBackCopy(*it);
        }
    }
    return self;
}
