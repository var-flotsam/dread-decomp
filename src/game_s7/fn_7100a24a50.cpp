// Copies of fn_7100e7cdfc (src/game_aca57c/fn_7100ca3be4.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100a24a50, fn_71009bd8c0.
// The destructors of reflected property types (vtable 0x7101aa52d0, used for
// CBreakableTileManager's "PB_AND_SM" and "VIGNETTE" properties, and 0x7101ca9ea0): the string
// id at 0x170 is released, then the base property (vtable 0x7101a889c8) destroys its value
// holder at 0x10 (a member at 0xe0, then a name).
#include <new>

#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_71009c172c(void* self);
void fn_710007ed40(void* self);

extern void* g_unk_7101a889c8[];
extern void* g_unk_7101aa52d0[];
extern void* g_unk_7101a88970[];
extern void* g_unk_7101a8d830[];

// A string id whose destructor reloads the id after the decrement.
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The property's value holder at 0x10: a name cleaned up out of line (fn_710007ed40) before it
// is destroyed, and a member at 0xd0.
struct UnkNameHolder {
    CRntString name;
    u64 unk_0x30[0x14];

    ~UnkNameHolder() { fn_710007ed40(this); }
};

struct UnkHolderMember {
    u64 unk_0x0[0x12];

    ~UnkHolderMember() { fn_71009c172c(this); }
};

struct UnkValueHolder : UnkNameHolder {
    UnkHolderMember field_0xd0;
};

struct UnkPropertyBase {
    void** vtable;
    u64 field_0x8;
    UnkValueHolder holder;

    ~UnkPropertyBase() { *reinterpret_cast<void***>(this) = g_unk_7101a889c8 + 2; }
};

template <void** Vtable>
struct UnkIdPropertyT : UnkPropertyBase {
    UnkReleasedStrId id;

    ~UnkIdPropertyT() { *reinterpret_cast<void***>(this) = Vtable + 2; }
};
using UnkIdProperty = UnkIdPropertyT<g_unk_7101aa52d0>;
static_assert(sizeof(UnkIdProperty) == 0x178, "UnkIdProperty layout");

// The same destructor for two other property types (vtables 0x7101a8d830 and 0x7101a88970).
using UnkIdProperty2 = UnkIdPropertyT<g_unk_7101a8d830>;
using UnkIdProperty3 = UnkIdPropertyT<g_unk_7101a88970>;

void fn_7100a24a50(UnkIdProperty2* self) { self->~UnkIdPropertyT(); }

void fn_71009bd8c0(UnkIdProperty3* self) { self->~UnkIdPropertyT(); }

