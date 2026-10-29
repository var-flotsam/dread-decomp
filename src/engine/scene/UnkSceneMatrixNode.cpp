// From unit 208 of tools/tu_map.py (0x710016b51c-0x710016e03c, no class hint): a scene object (0x58
// bytes) that keeps a matrix built from a value and an angle, and the 12-byte entries its owner's
// scene allocates for it on first use. Names and layouts are placeholders.
// Holds fn_710016c760 (the constructor).
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "types.h"

// The base's vtable and the class's (reached through the GOT).
extern char unk_7101a1db18[];
extern char unk_7101a1d9f8[];

// The matrix of `value` rotated by `angle` (returned in memory).
UnkMatrix44 fn_710007966c(const void* value, f32 angle);

// An entry (12 bytes, zeroed).
struct UnkSceneEntry {
    u32 words[3] = {};
};

// The owner: its scene (whose +0x38c is the number of entries and +0x1d8 what this points at) and
// the entries, allocated on first use.
struct UnkSceneEntryOwner {
    u8 pad_0x0[0x8];
    u8* scene;
    u8 pad_0x10[0x10];
    void* entries;  // 0x20, 12 bytes each
};

struct UnkSceneMatrixNode {
    char* vtable;
    UnkSceneEntryOwner* owner;
    UnkMatrix44 matrix;  // 0x10
    u8* field_0x50;      // the owner's scene + 0x1d8
};
static_assert(sizeof(UnkSceneMatrixNode) == 0x58, "UnkSceneMatrixNode placeholder layout");

// The entries are zeroed by constructing each one (an array new's loop, run only when n != 0).
void fn_710016c760(UnkSceneMatrixNode* self, UnkSceneEntryOwner* owner, const void* value, f32 angle) {
    self->vtable = unk_7101a1db18 + 0x10;
    self->owner = owner;
    if (!owner->entries) {
        u32 n = *reinterpret_cast<const u32*>(owner->scene + 0x38c);
        u64 size = (u64)n * sizeof(UnkSceneEntry);
        UnkSceneEntry* p = static_cast<UnkSceneEntry*>(fn_7100083ba0(size, 0x1a));
        if (n) {
            UnkSceneEntry* e = p;
            do {
                new (e) UnkSceneEntry;
                ++e;
            } while (e != p + n);
        }
        owner->entries = p;
    }
    self->vtable = unk_7101a1d9f8 + 0x10;
    new (&self->matrix) UnkMatrix44(fn_710007966c(value, angle));
    self->field_0x50 = self->owner->scene + 0x1d8;
}
