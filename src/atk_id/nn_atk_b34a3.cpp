// nn::atk unit: a sound archive ("FSAR") file header, copied out when its magic, byte-order mark and
// version are valid. Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "game/UnkTU99.h"
#include "types.h"
struct UnkAtkFileHeaderHead {
    u32 magic;
    s16 byteOrder;
    u16 headerSize;
    u32 version;
    u32 fileSize;
    u64 field_0x10;
};
struct UnkAtkFileHeaderBlock {
    u64 a, b, c, d;
};
struct UnkAtkFileHeader {
    UnkAtkFileHeaderHead head;
    UnkAtkFileHeaderBlock blocks;
};
UnkAtkFileHeader* fn_71012106d0(UnkAtkFileHeader* out, const UnkAtkFileHeader* in) {
    if (in->head.magic == 0x52415346 && in->head.byteOrder == -257 && in->head.version - 0x10000 <= 0x10500) {
        out->head = in->head;
        out->blocks = in->blocks;
    }
    return out;
}

// An info record found by name: its first word, a value derived from it (fn_710121062c) and its byte at +4.
struct UnkAtkNamedRecord {
    u32 field_0x0;
    u8 field_0x4;
};
struct UnkAtkNamedTable;
const UnkAtkNamedRecord* fn_710120fda0(const UnkAtkNamedTable* table, const char* name);
u32 fn_710121062c(const UnkAtkNamedRecord* record);
struct UnkAtkNamedOut {
    u32 field_0x0;
    u32 field_0x4;
    u8 field_0x8;
};
struct UnkAtkNamedReader {
    u8 pad_0x0[0x40];
    const UnkAtkNamedTable* table;  // 0x40
};
bool fn_7101210a74(const UnkAtkNamedReader* self, const char* name, UnkAtkNamedOut* out) {
    const UnkAtkNamedRecord* record = fn_710120fda0(self->table, name);
    if (!record)
        return false;
    out->field_0x0 = record->field_0x0;
    out->field_0x4 = fn_710121062c(record);
    out->field_0x8 = record->field_0x4;
    return true;
}

// A block reference inside a file (kind at +0x18, offset at +0x1c, -1 for none): the block it points
// to when the kind is 0x2210, else null.
struct UnkAtkBlockRefOwner {
    u8 pad_0x0[0x18];
    u16 kind;
    u8 pad_0x1a[0x2];
    s32 offset;
};
const void* fn_71012104a0(const UnkAtkBlockRefOwner* self) {
    if (self->offset == -1)
        return nullptr;
    if (self->kind != 0x2210)
        return nullptr;
    return reinterpret_cast<const char*>(self) + self->offset;
}

// Two more lookups by name in the same reader's table.
struct UnkAtkNamedRecord2;
struct UnkAtkNamedRecord2Info {
    u32 field_0x0;
    u32 field_0x4;
    u8 field_0x8;
    u8 field_0x9;
};
const UnkAtkNamedRecord2* fn_710120fc5c(const UnkAtkNamedTable* table, const char* name);
const UnkAtkNamedRecord2Info* fn_7101210178(const UnkAtkNamedRecord2* record);
bool fn_7101210b50(const UnkAtkNamedReader* self, const char* name, UnkAtkNamedRecord2Info* out) {
    const UnkAtkNamedRecord2* record = fn_710120fc5c(self->table, name);
    if (!record)
        return false;
    const UnkAtkNamedRecord2Info* info = fn_7101210178(record);
    if (!info)
        return false;
    out->field_0x0 = info->field_0x0;
    out->field_0x4 = info->field_0x4;
    out->field_0x8 = info->field_0x8;
    out->field_0x9 = info->field_0x9;
    return true;
}

struct UnkAtkNamedRecord3 {
    u32 field_0x0;
    u32 field_0x4;
    u8 pad_0x8[0x4];
    s32 offset;  // 0xc, from the record
};
struct UnkAtkNamedRecord3Out {
    u32 field_0x0;
    u32 field_0x4;
    const void* data;
};
const UnkAtkNamedRecord3* fn_710120fd1c(const UnkAtkNamedTable* table, const char* name);
bool fn_7101210920(const UnkAtkNamedReader* self, const char* name, UnkAtkNamedRecord3Out* out) {
    const UnkAtkNamedRecord3* record = fn_710120fd1c(self->table, name);
    if (!record)
        return false;
    out->field_0x0 = record->field_0x0;
    out->field_0x4 = record->field_0x4;
    out->data = reinterpret_cast<const char*>(record) + record->offset;
    return true;
}

// Optional parameters after a flag word at +0x14: one 4-byte slot for each of the low bits that is
// set, in bit order. Bit 3's slot holds a byte and a 16-bit value.
struct UnkAtkOptionalParams {
    u32 flags;
    u32 slots[1];

    bool Has(u32 bit) const { return static_cast<u64>(flags) >> bit & 1; }
    const u8* Slot(u32 bit) const {
        u64 count = 0;
        for (u32 i = 0; i < bit; ++i)
            if (Has(i))
                ++count;
        return reinterpret_cast<const u8*>(reinterpret_cast<const u32*>(this) + count + 1);
    }
};
struct UnkAtkOptionalParamsOwner {
    u8 pad_0x0[0x14];
    UnkAtkOptionalParams params;  // 0x14
};
u32 fn_7101210218(const UnkAtkOptionalParamsOwner* self) {
    const UnkAtkOptionalParams* params = &self->params;
    if (!params->Has(3))
        return 0;
    return params->Slot(3)[0];
}
u16 fn_7101210248(const UnkAtkOptionalParamsOwner* self) {
    const UnkAtkOptionalParams* params = &self->params;
    if (!params->Has(3))
        return 0xffff;
    return *reinterpret_cast<const u16*>(params->Slot(3) + 2);
}

// Appends an object's list node (+0x40) to a list (head at +0x890) while holding the lock at +0x8a0
// of the object fn_71001039f8 returns (fn_71012392d0 locks, fn_71012392e0 unlocks). The insertion is
// a ring splice: the node's own ring goes in before the head.
struct UnkAtkLock;
struct UnkAtkListLink {
    UnkAtkListLink* prev;
    UnkAtkListLink* next;

    void LinkPrev(UnkAtkListLink* node) {
        UnkAtkListLink* last = node->prev;
        node->prev = prev;
        last->next = this;
        prev->next = node;
        prev = last;
    }
};
struct UnkAtkListed {
    u8 pad_0x0[0x40];
    UnkAtkListLink link;  // 0x40
};
struct UnkAtkListOwner2 {
    u8 pad_0x0[0x890];
    UnkAtkListLink head;  // 0x890
};
static inline UnkAtkLock* AtkLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_71001039f8()) + 0x8a0);
}
void fn_710121510c(UnkAtkListOwner2* self, UnkAtkListed* item) {
    fn_71012392d0(AtkLock());
    self->head.LinkPrev(&item->link);
    fn_71012392e0(AtkLock());
}

// The same optional parameters at +0x4 of another record: bit 3's slot as a float, 1 by default.
struct UnkAtkOptionalParamsOwner4 {
    u8 pad_0x0[0x4];
    UnkAtkOptionalParams params;  // 0x4
};
f32 fn_7101230ab4(const UnkAtkOptionalParamsOwner4* self) {
    const UnkAtkOptionalParams* params = &self->params;
    if (!params->Has(3))
        return 1.0f;
    return *reinterpret_cast<const f32*>(params->Slot(3));
}

// Loads an item's data (flag 1, through fn_7101211d30 with the file id fn_71012069f8 gives) and the
// items it refers to (flags 4 and 8: up to four ids fn_7101206b4c fills in, -1 for none, each loaded
// by fn_7101211a64). False when a step fails.
struct UnkAtkArchive;
struct UnkAtkItemRefs {
    u32 field_0x0;
    u32 ids[4];  // 0x4
    u32 field_0x14;
    u16 field_0x18;
};
struct UnkAtkLoader {
    u8 pad_0x0[0x8];
    UnkAtkArchive* archive;  // 0x8
};
bool fn_7101211a64(UnkAtkLoader* self, u32 id, void* heap, u32 flags, void* arg);
bool fn_710121166c(UnkAtkLoader* self, u32 id, void* heap, u32 flags, void* arg) {
    if (flags & 1) {
        if (!fn_7101211d30(self, fn_71012069f8(self->archive, id), heap, arg, 0))
            return false;
    }
    if (flags & 0xc) {
        UnkAtkItemRefs refs;
        refs.field_0x0 = 0;
        refs.field_0x14 = 0;
        refs.field_0x18 = 0;
        refs.ids[0] = refs.ids[1] = refs.ids[2] = refs.ids[3] = 0xffffffff;
        if (!fn_7101206b4c(self->archive, &refs, id))
            return false;
        for (u32 i = 0; i < 4; ++i) {
            if (refs.ids[i] != 0xffffffff && !fn_7101211a64(self, refs.ids[i], heap, flags, arg))
                return false;
        }
    }
    return true;
}

// More of the optional parameters at +0x14: bit 17's slot's low bit (0 by default) and bit 31's
// as a word (-1 by default).
u32 fn_71012103dc(const UnkAtkOptionalParamsOwner* self) {
    const UnkAtkOptionalParams* params = &self->params;
    if (!params->Has(17))
        return false;
    return *reinterpret_cast<const u32*>(params->Slot(17)) & 1;
}
u32 fn_71012102cc(const UnkAtkOptionalParamsOwner* self) {
    const UnkAtkOptionalParams* params = &self->params;
    if (!params->Has(31))
        return 0xffffffff;
    return *reinterpret_cast<const u32*>(params->Slot(31));
}

// Queues a task (fn_710123a3d0 on the queue at +0x18, then fn_710123a3e0) under the lock at +0x960 of
// fn_71001039f8's object. With flag 1, when +0x955 is set and +0xa66 is not, it waits (5 ms sleeps,
// fn_7101238f90) until the task's state (fn_710123a3f0) is 1; otherwise it runs fn_7101214040 and
// aborts (fn_71012390d0) on a failing result.
struct UnkAtkTask;
struct UnkAtkTaskManager {
    u8 pad_0x0[0x18];
    u8 field_0x18[0x8];  // a task queue (fn_710123a3d0)
    u8 pad_0x20[0x935];
    bool field_0x955;
    u8 pad_0x956[0x110];
    bool field_0xa66;
};
void fn_710123a3d0(void* queue, UnkAtkTask* task, void* a, void* b);
void fn_710123a3e0(UnkAtkTask* task);
u32 fn_710123a3f0(UnkAtkTask* task);
void fn_7101238f90(s64 nanoseconds);
u64 fn_7101214040(UnkAtkTaskManager* self);
static inline UnkAtkLock* AtkTaskLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_71001039f8()) + 0x960);
}
static inline u32 AtkTaskState(UnkAtkTask* task) {
    fn_71012392d0(AtkTaskLock());
    u32 state = fn_710123a3f0(task);
    fn_71012392e0(AtkTaskLock());
    return state;
}
void fn_7101213f20(UnkAtkTaskManager* self, UnkAtkTask* task, void* a, void* b, u32 flags) {
    fn_71012392d0(AtkTaskLock());
    fn_710123a3d0(self->field_0x18, task, a, b);
    fn_710123a3e0(task);
    fn_71012392e0(AtkTaskLock());
    if ((flags & 1) && self->field_0x955 && !self->field_0xa66) {
        if (AtkTaskState(task) == 1)
            return;
        do {
            fn_7101238f90(5000000);
        } while (AtkTaskState(task) != 1);
        return;
    }
    alignas(8) u32 result = fn_7101214040(self);  // an 8-byte aligned slot, as in the original
    if (result != 0)
        fn_71012390d0("", "", "", 0, &result, "");
}

// Pushes a voice's parameters to its output (+0x20) unless the state at +0xa8 is 1 (or 2 while
// fn_710123a690 says no): the value at +0x2c clamped to [-128, 128], the one at +0x30 to
// [1/16, 16], the mix at +0x95, then fn_710122f9e0 and fn_710122faa0. Names are placeholders.
struct UnkAtkVoiceOut;
struct UnkAtkVoiceMix {
    u8 field_0x0;
    u16 field_0x2;
    u16 field_0x4;
    u16 field_0x6;
    u16 field_0x8;
    u16 field_0xa;
};
struct UnkAtkVoiceParams;
struct UnkAtkVoice {
    u8 pad_0x0[0x20];
    u8 out[0xc];  // 0x20
    f32 field_0x2c;
    f32 field_0x30;
    u8 field_0x34[0x61];
    u8 mix_0x95;
    u16 mix_0x96;
    u16 mix_0x98;
    u16 mix_0x9a;
    u16 mix_0x9c;
    u16 mix_0x9e;
    u8 pad_0xa0[0x8];
    u32 state;  // 0xa8
};
bool fn_710123a610(void* out);
u32 fn_710123a690(void* out);
void fn_710123a6a0(void* out, f32 value);
void fn_710123a6b0(void* out, f32 value);
void fn_710123a6c0(void* out, u32 index, const UnkAtkVoiceMix* mix);
void fn_710122f9e0(UnkAtkVoice* self, f32* params);
void fn_710122f79c(UnkAtkVoice* self, u32 arg) {
    void* out = self->out;
    if (!fn_710123a610(out))
        return;
    if (self->state == 1)
        return;
    if (self->state == 2 && !fn_710123a690(out))
        return;
    f32 a = self->field_0x2c;
    f32 a_min = __builtin_fminf(a, 128.0f);
    f32 a_clamped = -128.0f;
    if (a >= -128.0f)
        a_clamped = a_min;
    fn_710123a6a0(out, a_clamped);
    f32 b = self->field_0x30;
    f32 b_max = b <= 0.0625f ? 0.0625f : b;
    fn_710123a6b0(out, b > 16.0f ? 16.0f : b_max);
    UnkAtkVoiceMix mix;
    mix.field_0x8 = self->mix_0x9c;
    mix.field_0xa = self->mix_0x9e;
    mix.field_0x2 = self->mix_0x96;
    mix.field_0x4 = self->mix_0x98;
    mix.field_0x6 = self->mix_0x9a;
    mix.field_0x0 = self->mix_0x95;
    fn_710123a6c0(out, 0, &mix);
    fn_710122f9e0(self, &self->field_0x2c);
    fn_710122faa0(self, self->field_0x34, arg);
}

// A lookup by name that only answers for records of kind 3 (fn_710120ff8c): two words of the
// record's info (fn_7101210154) and two flags derived from it (fn_71012104d8, fn_71012104f0).
struct UnkAtkKind3Info {
    u32 field_0x0;
    u32 field_0x4;
};
struct UnkAtkKind3Out {
    u32 field_0x0;
    u32 field_0x4;
    u8 field_0x8;
    bool field_0x9;
};
u32 fn_710120ff8c(const UnkAtkNamedRecord2* record);
const UnkAtkKind3Info* fn_7101210154(const UnkAtkNamedRecord2* record);
u8 fn_71012104d8(const UnkAtkKind3Info* info);
u8 fn_71012104f0(const UnkAtkKind3Info* info);
bool fn_7101210fcc(const UnkAtkNamedReader* self, const char* name, UnkAtkKind3Out* out) {
    const UnkAtkNamedRecord2* record = fn_710120fc5c(self->table, name);
    if (!record)
        return false;
    if (fn_710120ff8c(record) != 3)
        return false;
    const UnkAtkKind3Info* info = fn_7101210154(record);
    out->field_0x0 = info->field_0x0;
    out->field_0x4 = info->field_0x4;
    out->field_0x8 = fn_71012104d8(info);
    out->field_0x9 = fn_71012104f0(info) != 0;
    return true;
}
