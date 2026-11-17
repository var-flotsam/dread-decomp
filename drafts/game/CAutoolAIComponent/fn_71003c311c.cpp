// CAutoolAIComponent's unit (tools/tu_map.py unit 570): the component's destructor. It destroys the
// embedded listener at +0x14798, drops the counted references at +0x14768 to +0x14758, frees two arrays,
// drops the reference at +0x14700, frees the array of entries at +0x146d8 and runs the base's destructor.
// Names and layouts are placeholders.
#include "../../game_5821e4/CAttackDtor.h"
#include "base/global/CRntVector.h"
#include "game/CRobotAIComponent.h"
#include "types.h"

extern void* g_unk_7101a2d0f8[];  // the component's vtable

struct UnkAutoolEntry_l3c311c {
    virtual ~UnkAutoolEntry_l3c311c();
    u8 pad_0x8[0x38];
};

// Counted references and their keys: two arrays from one allocator.
struct UnkAutoolRefTable_l3c311c {
    UnkAttackRefPtrRaw* values;
    u32* keys;
    u32 count;
    u32 field_0x14;
    u64 field_0x18;
    UnkAttackAllocator* allocator;  // 0x20

    // Releases the references, then frees both arrays.
    void Destroy() {
        if (values)
            for (u64 i = 0; i < count; ++i)
                values[i].Release();
        count = 0;
        if (keys)
            allocator->Free(keys);
        if (values)
            allocator->Free(values);
    }
};

struct UnkAutoolAI_l3c311c {
    void** vtable;
    u8 pad_0x8[0x146d0];
    base::global::CRntVector<UnkAutoolEntry_l3c311c> entries;  // 0x146d8
    UnkAttackRefPtrRaw field_0x14700;
    UnkAutoolRefTable_l3c311c refs;                            // 0x14708
    base::global::CRntVector<u32> values;                      // 0x14730
    UnkAttackRefPtrRaw field_0x14758;
    UnkAttackRefPtrRaw field_0x14760;
    UnkAttackRefPtrRaw field_0x14768;
    u8 pad_0x14770[0x28];
    UnkAttackListenerRaw listener;  // 0x14798
};
static_assert(__builtin_offsetof(UnkAutoolAI_l3c311c, listener) == 0x14798, "layout");

void fn_71003c311c(UnkAutoolAI_l3c311c* self) {
    self->vtable = g_unk_7101a2d0f8 + 2;
    self->listener.Destroy();
    self->field_0x14768.Release();
    self->field_0x14760.Release();
    self->field_0x14758.Release();
    self->values.Free();

    self->refs.Destroy();
    self->field_0x14700.Release();
    self->entries.Free();
    fn_7100661914(reinterpret_cast<UnkAutsharpAIBase_b47*>(self));
}
