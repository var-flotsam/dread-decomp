// Unit 2530: updates each liquid simulation of a list, gathers the values of those that ran into one
// buffer (at most 0x1ff after its first word), tells each one where its values went, and hands the
// buffer to the render manager. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkRenderManager.h"
#include "types.h"

// The array of counted references that fn_7100da2900 reserves (src/game/GUI/CCustomMarkersDialog/fn_7100afe1d0.cpp).
struct UnkRefPtrArray;
void fn_7100da2900(UnkRefPtrArray* self, u32 capacity);

struct CLiquidSimulationComponent;
bool fn_7100905e9c(CLiquidSimulationComponent* self, f32 dt);
void fn_7100906208(char* p0, u32 p1, u32 p2, u32 p3);
void fn_710021a0f8(UnkRenderManager* self, const u32* values, u32 count);

// A list entry: a reference-counted holder of a liquid simulation.
struct UnkLiquidRef_da2100 {
    u8 pad_0x0[0x10];
    CLiquidSimulationComponent* liquid;  // 0x10
};

// base::global::CRntVector's layout, of the entries.
struct UnkLiquidList_da2100 {
    UnkLiquidRef_da2100** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    void Reserve(u32 n) { fn_7100da2900(reinterpret_cast<UnkRefPtrArray*>(this), n); }
};

// The part of CLiquidSimulationComponent (src/game/CLiquidSimulationComponent.cpp) read here: the values
// it hands over and how many there are.
struct UnkLiquidValues_da2100 {
    u8 pad_0x0[0x54];
    u32 values[0x200];  // 0x54
    u8 pad_0x854[0x800];
    u32 count;  // 0x1054
};

struct UnkLiquidBatch_da2100 {
    UnkLiquidList_da2100 liquids;
    u32 buffer[0x200];  // 0x28
    u32 used;           // 0x828
};

// The element at `index`. When field_0x14 is set, an index past the end first grows the array to the
// next multiple of the step and then to exactly index + 1 elements, the new ones empty.
static inline UnkLiquidRef_da2100*& At(UnkLiquidList_da2100* array, u32 index) {
    if (array->field_0x14 && array->count <= index) {
        u32 capacity = array->capacity;
        if (capacity <= index && array->step != 0)
            capacity = (index / array->step + 1) * array->step;
        array->Reserve(capacity);
        u32 n = index + 1;
        array->Reserve(n);
        for (u32 i = array->count; i < n; ++i)
            new (&array->data[i]) UnkLiquidRef_da2100*();
        array->count = n;
    }
    return array->data[index];
}

static inline CLiquidSimulationComponent* Liquid(UnkLiquidList_da2100* array, u32 index) {
    UnkLiquidRef_da2100* p = At(array, index);
    return p ? p->liquid : nullptr;
}

static inline UnkLiquidValues_da2100* Values(CLiquidSimulationComponent* liquid) {
    return reinterpret_cast<UnkLiquidValues_da2100*>(liquid);
}

void fn_7100da2100(UnkLiquidBatch_da2100* self, f32 dt) {
    u32* values = self->buffer;
    self->used = 1;
    u32* out = &values[1];
    for (u32 i = 0; i < self->liquids.count; ++i) {
        UnkLiquidRef_da2100* p = self->liquids.data[i];
        if (!p || !p->liquid)
            continue;
        if (!fn_7100905e9c(Liquid(&self->liquids, i), dt)) {
            fn_7100906208(reinterpret_cast<char*>(Liquid(&self->liquids, i)), 0, 0, 0);
            continue;
        }
        u32 count = Values(At(&self->liquids, i)->liquid)->count;
        u32 room = 0x1ff - self->used;
        u32 n = count < room ? count : room;
        if (n == 0)
            continue;
        __builtin_memcpy(out, Values(Liquid(&self->liquids, i))->values, (u64)n * sizeof(u32));
        fn_7100906208(reinterpret_cast<char*>(Liquid(&self->liquids, i)), 0x200, n, self->used);
        self->used += n;
        out += n;
    }
    fn_710021a0f8(g_unk_7101d47300, values, self->used);
}
