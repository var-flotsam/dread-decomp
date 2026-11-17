// Unit 1115 (tools/tu_map.py, no class name): the registry forgets one id list filed under a key, marks the
// objects of those ids as unowned, and drops the key once it has no lists left. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

using base::global::CRntVector;

extern u32 g_unk_7101a6b7f0;

// An inner array of plain values, freed with its owner.
struct UnkIdList_l6fb820 {
    CRntVector<u32> ids;

    ~UnkIdList_l6fb820() { ids.Free(); }
};

// A record of the table: an array of arrays and a word.
struct UnkRecord_l6fb820 {
    CRntVector<UnkIdList_l6fb820> lists;  // 0x0
    u32 field_0x28;                       // 0x28

    UnkRecord_l6fb820(UnkRecord_l6fb820&& other)
        : lists(static_cast<CRntVector<UnkIdList_l6fb820>&&>(other.lists)), field_0x28(other.field_0x28) {}
    ~UnkRecord_l6fb820() { lists.Free(); }
};
static_assert(sizeof(UnkRecord_l6fb820) == 0x30, "layout");

// Records with their keys alongside in a second array.
struct UnkRecordTable_l6fb820 {
    UnkRecord_l6fb820* records;  // 0x0
    u32* keys;                   // 0x8
    u32 count;                   // 0x10
};

UnkRecord_l6fb820* fn_71006fb820(UnkRecordTable_l6fb820* self, UnkRecord_l6fb820* it);

// An object of the registry, as far as this function writes it.
struct UnkRegObj_l6fa0fc {
    u8 pad_0x0[0xc];
    u32 owner;  // 0xc
};

// Objects with their ids alongside in a second array.
struct UnkObjTable_l6fa0fc {
    UnkRegObj_l6fa0fc** objects;  // 0x0
    u32* keys;                    // 0x8
    u32 count;                    // 0x10
};

struct UnkRegistry_l6fa0fc {
    u8 pad_0x0[0x18];
    UnkRecordTable_l6fb820 records;  // 0x18
    u8 pad_0x30[0x10];
    UnkObjTable_l6fa0fc objects;     // 0x40
    u8 pad_0x58[0x40];
    CRntVector<u32> keys;            // 0x98
};
static_assert(__builtin_offsetof(UnkRegistry_l6fa0fc, keys) == 0x98, "layout");

// The record filed under `key`, or the place past the last one.
inline UnkRecord_l6fb820& RecordOf(UnkRecordTable_l6fb820& table, u32 key) {
    u64 i = 0;
    for (; i < table.count; ++i)
        if (table.keys[i] == key)
            break;
    return table.records[i];
}

// The place of the record filed under `key`, or the end when there is none.
inline UnkRecord_l6fb820* FindRecord(UnkRecordTable_l6fb820& table, u32 key) {
    for (u64 i = 0; i < table.count; ++i)
        if (table.keys[i] == key)
            return &table.records[i];
    return &table.records[table.count];
}

// The object filed under `key`, or the slot past the last one.
inline UnkRegObj_l6fa0fc* ObjectOf(UnkObjTable_l6fa0fc& table, u32 key) {
    u64 i = 0;
    for (; i < table.count; ++i)
        if (table.keys[i] == key)
            break;
    return table.objects[i];
}

// The index of the list equal to `ids`, or the count when there is none.
inline u64 IndexOfList(const CRntVector<UnkIdList_l6fb820>& lists, const CRntVector<u32>& ids) {
    u64 i = 0;
    for (; i < lists.count; ++i)
        if (lists.data[i].ids == ids)
            break;
    return i;
}

void fn_71006fa0fc(UnkRegistry_l6fa0fc* self, u32 key, const CRntVector<u32>* ids) {
    if (key == g_unk_7101a6b7f0 || ids->count == 0)
        return;
    if (FindRecord(self->records, key) == self->records.records + self->records.count)
        return;

    CRntVector<UnkIdList_l6fb820>& found = RecordOf(self->records, key).lists;
    if (IndexOfList(found, *ids) != found.count) {
        CRntVector<UnkIdList_l6fb820>& lists = RecordOf(self->records, key).lists;
        u32 index = IndexOfList(lists, *ids);
        if (index < lists.count) {
            // The list's elements have no move constructor: move the arrays themselves down.
            lists.data[index].~UnkIdList_l6fb820();
            for (u64 i = index; i < lists.count - 1; ++i) {
                new (&lists.data[i].ids) CRntVector<u32>(static_cast<CRntVector<u32>&&>(lists.data[i + 1].ids));
                lists.data[i + 1].~UnkIdList_l6fb820();
            }
            lists.count = lists.count - 1;
        }
        for (u32 id : *ids) {
            UnkRegObj_l6fa0fc* obj = ObjectOf(self->objects, id);
            if (obj != nullptr)
                obj->owner = g_unk_7101a6b7f0;
        }
    }

    if (RecordOf(self->records, key).lists.count == 0) {
        UnkRecord_l6fb820* it = FindRecord(self->records, key);
        if (it != self->records.records + self->records.count)
            fn_71006fb820(&self->records, it);
        self->keys.Remove(key);
    }
}
