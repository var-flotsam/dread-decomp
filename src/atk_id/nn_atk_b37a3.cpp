// nn::atk unit (unit 2772 of tools/tu_map.py): sound archive info lookups by item id. The id's top byte is the
// item kind and its low 24 bits an index into a table reached through a self-relative offset; each table is a
// count followed by (type, offset) references whose offsets are relative to the table. Names and layouts are
// placeholders. The signatures copy the existing declarations (src/atk_id/nn_atk.cpp, nn_atk_b34a3.cpp; the
// latter pass the id as `const char*`).
#include "types.h"

struct UnkAtkInfoTable;
struct UnkAtkNamedTable;
struct UnkAtkInfoRecord;
struct UnkAtkNamedRecord;
struct UnkAtkNamedRecord2;
struct UnkAtkNamedRecord3;

struct UnkAtkItemTable {
    u32 count;
    struct {
        u32 type;
        s32 offset;
    } items[1];
};

// The id as the 32-bit value the callers pass (the declarations type it as a pointer).
static inline u32 IdOf(const char* name) {
    u32 id = (u32)(u64)name;
    asm("" : "+r"(id));
    return id;
}

template <u32 Kind, u32 TableOffset>
static inline const char* GetInfo(const void* block, u32 id) {
    const char* self = (const char*)block;
    if ((id >> 24) != Kind)
        return nullptr;
    u32 index = id & 0xffffff;
    const UnkAtkItemTable* table = (const UnkAtkItemTable*)(self + *(const s32*)(self + TableOffset));
    if (index >= table->count)
        return nullptr;
    return (const char*)table + table->items[index].offset;
}

__attribute__((noinline)) const UnkAtkNamedRecord2* fn_710120fc5c(const UnkAtkNamedTable* table, const char* name) {
    return (const UnkAtkNamedRecord2*)GetInfo<1, 0x4>(table, IdOf(name));
}
__attribute__((noinline)) const UnkAtkInfoRecord* fn_710120fc98(const UnkAtkInfoTable* table, u32 id) {
    return (const UnkAtkInfoRecord*)GetInfo<3, 0x14>(table, id);
}
const UnkAtkInfoRecord* fn_710120fcd4(const UnkAtkInfoTable* table, u32 id) {
    return (const UnkAtkInfoRecord*)GetInfo<4, 0x2c>(table, id);
}
const UnkAtkNamedRecord3* fn_710120fd1c(const UnkAtkNamedTable* table, const char* name) {
    return (const UnkAtkNamedRecord3*)GetInfo<2, 0xc>(table, IdOf(name));
}
const UnkAtkInfoRecord* fn_710120fd64(const UnkAtkInfoTable* table, u32 id) {
    return (const UnkAtkInfoRecord*)GetInfo<6, 0x24>(table, id);
}
const UnkAtkNamedRecord* fn_710120fda0(const UnkAtkNamedTable* table, const char* name) {
    return (const UnkAtkNamedRecord*)GetInfo<5, 0x1c>(table, IdOf(name));
}
// The one table looked up without a kind check.
const UnkAtkInfoRecord* fn_710120fde8(const UnkAtkInfoTable* block, u32 id) {
    const char* self = (const char*)block;
    u32 index = id & 0xffffff;
    const UnkAtkItemTable* table = (const UnkAtkItemTable*)(self + *(const s32*)(self + 0x34));
    if (index >= table->count)
        return nullptr;
    return (const UnkAtkInfoRecord*)((const char*)table + table->items[index].offset);
}

// Reads the first word of a kind-3 info record of the archive's info block (+0x40).
struct UnkAtkArchiveReader {
    u8 pad_0x0[0x40];
    const UnkAtkInfoTable* info;
};

bool fn_71012108b0(const UnkAtkArchiveReader* self, u32 id, u32* out) {
    const UnkAtkInfoRecord* record = fn_710120fc98(self->info, id);
    if (!record)
        return false;
    *out = *(const u32*)record;
    return true;
}

// A flag word at +0xc: with bit 1 clear the answer is yes; otherwise the byte at +1 of the 4-byte item (at
// +0x10) that bit 0 selects.
struct UnkAtkFlagItem {
    u8 field_0x0;
    u8 field_0x1;
    u8 pad_0x2[0x2];
};

struct UnkAtkFlagSet {
    u32 flags;
    UnkAtkFlagItem items[2];
};

struct UnkAtkFlagged {
    u8 pad_0x0[0xc];
    UnkAtkFlagSet set;
};

bool fn_71012105e4(const UnkAtkFlagged* self) {
    const UnkAtkFlagSet* set = &self->set;
    u32 flags = set->flags;
    asm("" : "+r"(set));  // the original addresses the items from the set
    if (!(flags & 2))
        return true;
    return set->items[flags & 1].field_0x1 != 0;
}

// Fills a kind-1 info record's summary from the archive's info block (+0x40): its two words and byte at +8
// copied, the rest from the record's accessors. Takes the id typed as fn_710120fc5c's declaration has it.
struct UnkAtkKind1Summary {
    u32 field_0x0;
    u32 field_0x4;
    u8 field_0x8;
    u8 field_0x9;
    u8 field_0xa;
    u8 field_0xb;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u16 field_0x18;
    bool field_0x1a;
};

struct UnkAtkKind1Record {
    u32 field_0x0;
    u32 field_0x4;
    u8 field_0x8;
    u8 field_0x9;
};

struct UnkAtkNamedReader2 {
    u8 pad_0x0[0x40];
    const UnkAtkNamedTable* table;
};

u32 fn_71012101d8(const UnkAtkKind1Record* record);
u32 fn_71012101f8(const UnkAtkKind1Record* record);
u32 fn_7101210218(const UnkAtkKind1Record* record);
u16 fn_7101210248(const UnkAtkKind1Record* record);
u8 fn_7101210278(const UnkAtkKind1Record* record);
u8 fn_71012102a0(const UnkAtkKind1Record* record);
u32 fn_71012103dc(const UnkAtkKind1Record* record);

bool fn_710121080c(const UnkAtkNamedReader2* self, const char* name, UnkAtkKind1Summary* out) {
    const UnkAtkKind1Record* record = (const UnkAtkKind1Record*)fn_710120fc5c(self->table, name);
    if (!record)
        return false;
    out->field_0x0 = record->field_0x0;
    out->field_0x4 = record->field_0x4;
    out->field_0xa = record->field_0x8;
    out->field_0xc = fn_71012101d8(record);
    out->field_0x10 = fn_71012101f8(record);
    out->field_0x14 = fn_7101210218(record);
    out->field_0x18 = fn_7101210248(record);
    out->field_0x9 = fn_7101210278(record);
    out->field_0x8 = fn_71012102a0(record);
    out->field_0xb = record->field_0x9;
    out->field_0x1a = fn_71012103dc(record) & 1;
    return true;
}

// Optional parameters after a flag word at +0x14 of a kind-1 record: one 4-byte slot for each low bit that is
// set, in bit order (as in src/atk_id/nn_atk_b34a3.cpp). Bit 1's slot holds two bytes.
struct UnkAtkKind1Params {
    u32 flags;
    u32 slots[2];
};

#define KIND1_PARAM_BYTE(record, byte)                                                             do {                                                                                               const UnkAtkKind1Params* params = (const UnkAtkKind1Params*)((const char*)(record) + 0x14);         u32 flags = params->flags;                                                                     asm("" : "+r"(params)); /* the original addresses the slots from the flag word */              if (!(flags & 2))                                                                                  return 0;                                                                                  return ((const u8*)&params->slots[flags & 1])[byte];                                       } while (0)

__attribute__((noinline)) u32 fn_71012101d8(const UnkAtkKind1Record* record) { KIND1_PARAM_BYTE(record, 0); }
__attribute__((noinline)) u32 fn_71012101f8(const UnkAtkKind1Record* record) { KIND1_PARAM_BYTE(record, 1); }

// Appends a node (link at +0x38, state at +0x34 set to 1) to the singly linked list whose head and tail sit
// at +0xc8 and +0xd0.
struct UnkAtkListNode {
    u8 pad_0x0[0x34];
    u32 state;
    UnkAtkListNode* next;
};

struct UnkAtkNodeList {
    u8 pad_0x0[0xc8];
    UnkAtkListNode* head;
    UnkAtkListNode* tail;
};

void fn_710122f578(UnkAtkNodeList* self, UnkAtkListNode* node) {
    node->next = nullptr;
    node->state = 1;
    UnkAtkListNode* tail = self->tail;
    (tail ? tail->next : self->head) = node;
    self->tail = node;
}
