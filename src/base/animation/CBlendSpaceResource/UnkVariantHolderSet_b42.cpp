// Re-pointing a holder's two-state variant at the holder's own value, one copy per unit (two in
// CBlendSpaceResource's unit with the variant's id at +0x20, two in the flag-id unit with it at
// +0x18): the variant is reset (state 1 holds a file-path id and a counted reference, released;
// state 2 a pair of pointers) and, when the value at +8 is set, takes state 2 with the holder and
// that value. The reset is that of src/base/animation/CBlendSpaceResource/fn_710000c2c0.cpp. Names and
// layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkReleasedFilePathId_b42a1 {
    UnkStringData* data;

    ~UnkReleasedFilePathId_b42a1() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkCountedPtr_b42a1 {
    UnkCountedResource* ptr;

    ~UnkCountedPtr_b42a1() {
        if (UnkCountedResource* p = ptr) {
            if (--p->ref_count == 0)
                fn_710001737c(p);
        }
    }
};

template <u32 Skip>
struct UnkIdRef_b42a1 {
    u8 unk_0x0[Skip];
    UnkReleasedFilePathId_b42a1 id;
    UnkCountedPtr_b42a1 ref;
};

struct UnkPtrPair_b42a1 {
    void* first;
    void* second;
};

template <u32 Skip>
struct UnkIdRefVariant_b42a1 {
    union {
        UnkIdRef_b42a1<Skip> id_ref;
        UnkPtrPair_b42a1 pair;
    };
    s32 state;

    __attribute__((always_inline)) void Reset() {
        switch (state) {
        case 1:
            id_ref.~UnkIdRef_b42a1<Skip>();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
    }

    __attribute__((always_inline)) void SetPair(void* first, void* second) {
        // Read through a plain pointer: the original does not merge this read with Reset's.
        s32* p = &state;
        switch (*p) {
        case 1:
            id_ref.~UnkIdRef_b42a1<Skip>();
            state = 0;
            break;
        case 2:
            state = 0;
            break;
        }
        state = 2;
        pair.first = first;
        pair.second = second;
    }
};

template <u32 Skip>
struct UnkVariantHolder_b42a1 {
    u64 field_0x0;
    void* value;
    UnkIdRefVariant_b42a1<Skip> variant;

    __attribute__((always_inline)) void Update() {
        if (void* v = value)
            variant.SetPair(this, v);
        else
            variant.Reset();
    }
};

void fn_710000c3dc(UnkVariantHolder_b42a1<0x10>* self) { self->Update(); }
void fn_710000c878(UnkVariantHolder_b42a1<0x10>* self) { self->Update(); }

// Signature as src/flag_id/fn_7100266c04.cpp declares it (the second argument is unused).
struct UnkStruct_0x10;
void fn_7100266c44(UnkStruct_0x10* self, const UnkStruct_0x10* other) {
    reinterpret_cast<UnkVariantHolder_b42a1<0x8>*>(self)->Update();
}
void fn_71002670e0(UnkVariantHolder_b42a1<0x8>* self) { self->Update(); }
