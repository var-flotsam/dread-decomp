// nn::atk (unit 2772 of tools/tu_map.py): small setters. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkDirtyFlags {
    u8 pad_0x0[0x1db];
    bool field_0x1db;
    u8 pad_0x1dc[0xc];
    u16 dirty;  // 0x1e8
};

// Sets the flag at +0x1db, marking bit 1 dirty when it changes.
void fn_710122d024(UnkAtkDirtyFlags* self, bool value) {
    if (self->field_0x1db != value) {
        self->field_0x1db = value;
        self->dirty |= 2;
    }
}

// A reference to data elsewhere in a sound archive block: a type id, a byte offset from the start
// of the table that holds it, and a size.
struct UnkAtkReferenceWithSize {
    u16 type;
    u16 pad_0x2;
    s32 offset;
    u32 size;
};

struct UnkAtkReferenceTable {
    u32 count;
    UnkAtkReferenceWithSize items[1];
};

struct UnkAtkTableHolder {
    u32 field_0x0;
    s32 table;  // 0x4, byte offset from this object
};

// The data the table's item `index` refers to, or null for index -1 or a missing table.
const void* fn_710120fae8(const UnkAtkTableHolder* self, s32 index) {
    if (index == -1)
        return nullptr;
    const char* table = reinterpret_cast<const char*>(self) + self->table;
    if (table == nullptr)
        return nullptr;
    const UnkAtkReferenceTable* t = reinterpret_cast<const UnkAtkReferenceTable*>(table);
    return table + t->items[index].offset;
}

// An info record found by id: its first word and a value derived from it (fn_710121060c).
struct UnkAtkInfoRecord {
    u32 field_0x0;
};
struct UnkAtkInfoTable;
const UnkAtkInfoRecord* fn_710120fcd4(const UnkAtkInfoTable* table, u32 id);
u32 fn_710121060c(const UnkAtkInfoRecord* record);

struct UnkAtkInfoOut {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkAtkInfoReader {
    u8 pad_0x0[0x40];
    const UnkAtkInfoTable* table;  // 0x40
};

// Reads the record of `id` into `out`; false when the table has none.
bool fn_71012108e4(const UnkAtkInfoReader* self, u32 id, UnkAtkInfoOut* out) {
    const UnkAtkInfoRecord* record = fn_710120fcd4(self->table, id);
    if (!record)
        return false;
    out->field_0x0 = record->field_0x0;
    out->field_0x4 = fn_710121060c(record);
    return true;
}

// An intrusive list whose nodes sit at +0x288 of their owners; destroys every owner
// (fn_710122cf1c), reading the next node first.
struct UnkAtkListNode {
    UnkAtkListNode* prev;
    UnkAtkListNode* next;
};
struct UnkAtkListOwner;
void fn_710122cf1c(UnkAtkListOwner* owner);

struct UnkAtkOwnerList {
    u8 pad_0x0[0x8];
    UnkAtkListNode root;  // 0x8
};

void fn_710122f2f4(UnkAtkOwnerList* self) {
    UnkAtkListNode* end = &self->root;
    for (UnkAtkListNode* node = self->root.next; end != node;) {
        UnkAtkListNode* next = node->next;
        fn_710122cf1c(reinterpret_cast<UnkAtkListOwner*>(reinterpret_cast<char*>(node) - 0x288));
        node = next;
    }
}
