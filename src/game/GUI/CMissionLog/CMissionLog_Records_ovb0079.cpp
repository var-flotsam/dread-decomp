// GUI::CMissionLog's unit (tools/tu_map.py): the array of 0x38-byte mission-log records (a value, a
// string id and an array of ids): its clear (fn_7100b5b8b8), its destructor body (fn_7100b55d4c), its
// copy assignment (fn_7100b5c3e8, through the out-of-line reserve and push-back), the assignment of a
// record held by value or by reference (fn_7100b5b158), and the lookup of the last record with a given
// id (fn_7100b59b58). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkStrIdMember;

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

struct UnkMissionLogRecordDtor_ovb0079 {
    u32 field_0x0;
    UnkStrIdMember id;
    UnkStrIdMember* data;
    u32 count;
    u32 capacity;
    u8 unk_0x20[0x10];
    UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkMissionLogRecordDtor_ovb0079() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkStrIdMember();
            UnkStrIdMember* p = data;
            count = 0;
            allocator->vf_0x48(p);
            capacity = 0;
        }
    }
};

using Rec = UnkMissionLogRecordDtor_ovb0079;

__attribute__((always_inline)) static inline void ClearRecords(UnkElem38Array* self) {
    for (u64 i = 0; i < self->count; ++i)
        reinterpret_cast<Rec*>(&self->data[i])->~Rec();
    self->count = 0;
}

void fn_7100b5b8b8(UnkElem38Array* self) { ClearRecords(self); }

void fn_7100b55d4c(UnkElem38Array* self) {
    if (self->data) {
        ClearRecords(self);
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

void fn_7100b5c28c(UnkElem38Array* self, u32 capacity);
void fn_7100b593d0(UnkElem38Array* self, const UnkElem38* value);

UnkElem38Array* fn_7100b5c3e8(UnkElem38Array* self, const UnkElem38Array* other) {
    if (self != other) {
        ClearRecords(self);
        fn_7100b5c28c(self, other->capacity);
        for (const UnkElem38 *it = other->data, *end = it + other->count; it != end; ++it)
            fn_7100b593d0(self, it);
    }
    return self;
}

using base::global::CStrId;

struct UnkStruct_0x28;

struct UnkMissionLogRecordRef_ovb0079 {
    u32 field_0x0;
    CStrId id;
    u8 ids[0x28];
};

struct UnkMissionLogEntryRef_ovb0079 {
    union {
        struct {
            u32 key;
            UnkMissionLogRecordRef_ovb0079 record;
        } value;
        struct {
            u32* key;
            UnkMissionLogRecordRef_ovb0079* record;
        } ref;
    };
    u32 state;

    u32* Key() { return state == 2 ? ref.key : &value.key; }
    UnkMissionLogRecordRef_ovb0079* Record() { return state == 2 ? ref.record : &value.record; }
};

UnkMissionLogEntryRef_ovb0079* fn_7100b5b158(UnkMissionLogEntryRef_ovb0079* self,
                                             UnkMissionLogEntryRef_ovb0079* other) {
    if (self != other) {
        *self->Key() = *other->Key();
        UnkMissionLogRecordRef_ovb0079* src = other->Record();
        UnkMissionLogRecordRef_ovb0079* dst = self->Record();
        dst->field_0x0 = src->field_0x0;
        dst->id = src->id;
        fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(dst->ids), reinterpret_cast<const UnkStruct_0x28*>(src->ids));
    }
    return self;
}

using base::global::UnkStringData;

struct UnkMissionLogFindEntry_ovb0079 {
    u32 field_0x0;
    UnkStringData* id;
    u8 unk_0x10[0x28];
};

struct UnkMissionLogFind_ovb0079 {
    u8 unk_0x0[0x230];
    UnkMissionLogFindEntry_ovb0079* field_0x230;
    u8 unk_0x238[0x10];
    UnkMissionLogFindEntry_ovb0079* data;  // 0x248
    u32 count;
};

static inline UnkMissionLogFindEntry_ovb0079* FindLast(UnkMissionLogFind_ovb0079* self, const CStrId& key) {
    u32 count = self->count;
    if (count != 0) {
        UnkMissionLogFindEntry_ovb0079* data = self->data;
        for (UnkMissionLogFindEntry_ovb0079* p = data + count; p != data;) {
            --p;
            if (p->id == key.data)
                return p;
        }
    }
    return nullptr;
}

void fn_7100b59b58(UnkMissionLogFind_ovb0079* self, const CStrId* id) {
    self->field_0x230 = nullptr;
    UnkMissionLogFindEntry_ovb0079* found;
    {
        CStrId key(*id);
        found = FindLast(self, key);
    }
    if (found)
        self->field_0x230 = found;
}
