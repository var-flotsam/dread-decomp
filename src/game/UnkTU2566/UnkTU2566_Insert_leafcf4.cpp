// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): inserts a copy of an element (a string id
// and two words) at an index of an array with base::global::CRntVector's layout, moving the later elements up
// one place; growing like PushBack (including its hang). Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTU1689.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

// A string id that can also be moved from (CStrId's copy; its destructor releases in two steps, as
// CStrId::Reset does).
struct UnkMovableStrId_eafcf4 : UnkStrIdBase {
    UnkMovableStrId_eafcf4(const UnkMovableStrId_eafcf4& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkMovableStrId_eafcf4(UnkMovableStrId_eafcf4&& other) : UnkStrIdBase(other) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMovableStrId_eafcf4() {
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

struct UnkIdValues_eafcf4 {
    UnkMovableStrId_eafcf4 id;
    u32 field_0x8;
    u32 field_0xc;

    UnkIdValues_eafcf4(const UnkIdValues_eafcf4& other)
        : id(other.id), field_0x8(other.field_0x8), field_0xc(other.field_0xc) {}
    UnkIdValues_eafcf4(UnkIdValues_eafcf4&& other)
        : id(static_cast<UnkMovableStrId_eafcf4&&>(other.id)), field_0x8(other.field_0x8), field_0xc(other.field_0xc) {}
};

struct UnkIdValuesArray_eafcf4 {
    UnkIdValues_eafcf4* data;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100eafcf4(UnkIdValuesArray_eafcf4* self, const UnkIdValues_eafcf4* value, u32 index) {
    UnkIdValuesArray* array = reinterpret_cast<UnkIdValuesArray*>(self);
    if (index == self->count) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)  // hangs, as CRntVector::PushBack does
            for (;;) {
            }
        fn_7100b48af4(array, new_capacity);
        new (&self->data[self->count]) UnkIdValues_eafcf4(*value);
    } else {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        fn_7100b48af4(array, new_capacity);
        for (u32 i = self->count - 1; i >= index;) {
            UnkIdValues_eafcf4* e = &self->data[i];
            new (&e[1]) UnkIdValues_eafcf4(static_cast<UnkIdValues_eafcf4&&>(*e));
            self->data[i].~UnkIdValues_eafcf4();
            if (i-- == 0)
                break;
        }
        new (&self->data[index]) UnkIdValues_eafcf4(*value);
    }
    ++self->count;
}
