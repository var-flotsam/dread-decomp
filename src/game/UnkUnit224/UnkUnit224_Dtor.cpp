// From unit 224 of tools/tu_map.py (0x710019c69c-0x710019dcac, no reflected class): the destructor
// of a class over fn_710019e82c's (vtable unk_7101a1e278): it returns its three pooled objects
// (+0x118, +0x120 and +0x130), frees its block at +0x80 through the default allocator and lets go
// of its three arrays (each with its own allocator). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU174.h"
#include "game/UnkTU182.h"
#include "game/UnkTU224.h"
#include "game/UnkTU226.h"
#include "types.h"

struct UnkObj_7100161b90;
struct UnkObj_71001541a0;
struct UnkStruct_710019e82c;

struct UnkArray224 {
    void* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    base::UnkAllocator* allocator;

    ~UnkArray224() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkBlock224 {
    void* data;
    u64 size;
};

// fn_710019e82c's class, the base.
struct UnkLayerBase224 {
    char* vtable;
    u8 pad_0x8[0x78];

    __attribute__((always_inline)) ~UnkLayerBase224() {
        fn_710019e82c(this);
    }
};

struct UnkLayer224 : UnkLayerBase224 {
    UnkBlock224 block;  // 0x80
    UnkBlock224 field_0x90;
    UnkArray224 arrays[3];  // 0xa0
    UnkObj_7100161b90* field_0x118;
    UnkObj_7100161b90* field_0x120;
    u8 pad_0x128[0x8];
    UnkObj_71001541a0* field_0x130;

    __attribute__((always_inline)) inline ~UnkLayer224();
};
static_assert(__builtin_offsetof(UnkLayer224, field_0x130) == 0x130, "UnkLayer224 layout");

inline UnkLayer224::~UnkLayer224() {
    vtable = g_unk_7101a1e278 + 0x10;
    if (field_0x118) {
        fn_7100161b90(field_0x118);
        field_0x118 = nullptr;
    }
    if (field_0x120) {
        fn_7100161b90(field_0x120);
        field_0x120 = nullptr;
    }
    if (field_0x130) {
        fn_71001541a0(field_0x130);
        field_0x130 = nullptr;
    }
    fn_7100083990()->vf_0x48(block.data);
    field_0x90 = UnkBlock224();
    block = UnkBlock224();
}

void fn_710019c80c(UnkLayer224* self) { self->~UnkLayer224(); }
