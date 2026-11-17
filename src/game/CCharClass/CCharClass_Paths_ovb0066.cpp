// CCharClass's unit (tools/tu_map.py unit 2740): the constructors, destructor and reset
// of a class above the base at fn_7100254d6c (destructor fn_7100254e84, reset fn_7100255158). It
// holds a table of counted resources and string ids at +0xd0 (cleared by fn_71011ac8e8), a counted
// resource at +0xf8 and a list of file-path ids at +0x100. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/UnkTU56.h"
#include "game/UnkTU63.h"
#include "types.h"
#include "base/UnkVtableList.h"

using base::global::CFilePathStrId;
using base::global::CRntFile;
using base::global::UnkStringData;
using base::global::UnkStringPool;

extern void* g_unk_7101cb57b8[];

struct UnkResIdTable;
void fn_71011ac8e8(UnkResIdTable* self);
void fn_7100254d6c(void* self);
void fn_7100254e84(void* self);
void fn_7100255158(void* self);

struct UnkPathNode_ovb0066 {
    UnkPathIdTwoStep id;
    UnkPathNode_ovb0066* next;
};

struct UnkCharClass_ovb0066 {
    void** vtable;
    u8 base[0xc8];
    UnkCountedResource** refs;  // 0xd0
    void* ids;                  // 0xd8
    u64 count;                  // 0xe0
    u32 field_0xe8;
    u32 field_0xec;
    base::UnkAllocator* table_allocator;  // 0xf0
    UnkCountedResource* counted;          // 0xf8
    base::UnkVtableList<UnkPathNode_ovb0066> paths;            // 0x100
    u32 field_0x130;
    f32 field_0x134;
    bool field_0x138;
    u32 field_0x13c;
    u32 field_0x140;
    f32 field_0x144;
    u64 field_0x148;
    u64 field_0x150;
    u64 field_0x158;

    // The virtual at slot 0x18 (initialisation).
    void Init() { reinterpret_cast<void (*)(UnkCharClass_ovb0066*)>(vtable[3])(this); }
};

__attribute__((always_inline)) static inline void Construct(UnkCharClass_ovb0066* self) {
    fn_7100254d6c(self);
    self->vtable = g_unk_7101cb57b8 + 2;
    self->refs = nullptr;
    self->ids = nullptr;
    self->count = 0;
    self->field_0xe8 = 10;
    self->field_0xec = 10;
    self->table_allocator = fn_7100083990();
    self->counted = nullptr;
    self->paths.vtable = g_unk_7101a1b030 + 2;
    self->paths.count = 0;
    self->paths.head = nullptr;
    self->paths.tail = nullptr;
    self->paths.allocator = fn_7100083990();
    self->paths.flags = 0x13;
    self->field_0x130 = 1;
    self->field_0x134 = -1.0f;
    self->field_0x138 = false;
    self->field_0x13c = 0;
    self->field_0x140 = 0;
    self->field_0x144 = 1.0f;
    self->field_0x148 = 0;
    self->field_0x150 = 0;
    self->field_0x158 = 0;
}

void fn_71011abe2c(UnkCharClass_ovb0066* self) { Construct(self); }

void fn_71011ac838(UnkCharClass_ovb0066* self) {
    Construct(self);
    self->Init();
}

void fn_71011ac024(UnkCharClass_ovb0066* self) {
    self->paths.ClearDestroying();
    fn_71011ac8e8(reinterpret_cast<UnkResIdTable*>(&self->refs));
    fn_7100255158(self);
}

void fn_71011abec4(UnkCharClass_ovb0066* self) {
    self->vtable = g_unk_7101cb57b8 + 2;
    self->paths.vtable = g_unk_7101a1b030 + 2;
    self->paths.ClearDestroying();
    if (UnkCountedResource* counted = self->counted) {
        if (--counted->ref_count == 0)
            fn_710001737c(counted);
    }
    fn_71011ac8e8(reinterpret_cast<UnkResIdTable*>(&self->refs));
    if (self->ids)
        self->table_allocator->vf_0x48(self->ids);
    if (self->refs)
        self->table_allocator->vf_0x48(self->refs);
    fn_7100254e84(self);
}
