// navmesh::SNavMeshPath's unit (tools/tu_map.py unit 366): the copy assignment of the whole path (flag,
// record, the 16-bit words and the four entries at +0x12a60, as fn_71002ae5f4 but with only the word copy
// guarded against self-assignment) and the destructors of its point table (the base at +0x10: clears the
// 0x120-byte points through their slot 0x8, releases the refcounted pointer at +0x11968, clears again).
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/UnkNavMeshPath.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

// The members of navmesh::SNavMeshPath the copy touches, as one struct so every access shares its base.
struct UnkNavMeshPathCopy_ovb0095 {
    u8 pad_0x0[8];
    u8 field_0x8;
    u8 pad_0x9[7];
    u8 field_0x10[0x11980 - 0x10];  // record, assigned by fn_71002ad4f4
    u32 word_count;                  // 0x11980
    u16 words[0x86e];                // 0x11984
    UnkWeakHandle* field_0x12a60;
    UnkWeakHandle* field_0x12a68;
    void* field_0x12a70;
    void* field_0x12a78;
};
static_assert(__builtin_offsetof(UnkNavMeshPathCopy_ovb0095, field_0x12a60) == 0x12a60, "layout");

UnkNavMeshPathCopy_ovb0095* fn_71002ad420(UnkNavMeshPathCopy_ovb0095* self, const UnkNavMeshPathCopy_ovb0095* other) {
    UnkNavMeshPath* path = reinterpret_cast<UnkNavMeshPath*>(self);
    self->field_0x8 = other->field_0x8;
    const void* other_record = other->field_0x10;
    fn_71002ad4f4(self->field_0x10, other_record);
    if (self != other) {
        __builtin_memcpy(self->words, other->words, (u64)other->word_count * 2);
        self->word_count = other->word_count;
    }
    fn_71002ad688(path, other->field_0x12a60 ? other->field_0x12a60->target : nullptr);
    fn_71002ad978(path, other->field_0x12a68 ? other->field_0x12a68->target : nullptr);
    fn_71002adbb0(path, other->field_0x12a70);
    self->field_0x12a78 = other->field_0x12a78;
    return self;
}

// A refcounted pointer cleared after the release; the count goes through a plain int pointer.
struct UnkRefPtr_ovb0095 {
    base::UnkRefCounted* ptr;

    ~UnkRefPtr_ovb0095() {
        if (ptr) {
            s32* count = &ptr->ref_count;
            if (--*count == 0)
                delete ptr;
            ptr = nullptr;
        }
    }
};

class UnkPathPoint_ovb0095 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
};

// The point table of navmesh::SNavMeshPath (its base at +0x10): up to 250 0x120-byte points.
struct UnkNavMeshPoints_ovb0095 {
    char* vtable;
    u32 count;                          // 0x8
    u8 points[0x11968 - 0xc];           // 0xc, stride 0x120
    UnkRefPtr_ovb0095 field_0x11968;

    void Clear() {
        for (u64 i = 0; i < count; i++)
            reinterpret_cast<UnkPathPoint_ovb0095*>(points + i * 0x120)->vf_0x8();
        count = 0;
    }
};

void fn_71002ac36c(UnkNavMeshPoints_ovb0095* self) {
    self->vtable = g_unk_7101a21168 + 0x10;
    self->Clear();
    self->field_0x11968.~UnkRefPtr_ovb0095();
    self->Clear();
}

void fn_71002ac454(UnkNavMeshPoints_ovb0095* self) {
    self->vtable = g_unk_7101a21168 + 0x10;
    self->Clear();
    self->field_0x11968.~UnkRefPtr_ovb0095();
    self->Clear();
    __builtin_operator_delete(self);
}
