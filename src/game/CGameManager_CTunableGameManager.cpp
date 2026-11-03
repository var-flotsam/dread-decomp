// Unit 2553 (CGameManager::CTunableGameManager by reflection hint): an object holding a list of
// strings behind a mutex. Names and layouts are placeholders inferred from the code.

#include <new>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

void fn_7100fc71ac(void* loader);

struct UnkStringList {
    u64 field_0x0;
    u64 field_0x8;
    base::global::CRntString* data;  // the base::UnkVector fields from here
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x24;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkStringList()
        : field_0x0(0), field_0x8(0), data(nullptr), count(0), capacity(0), step(1),
          field_0x24(true), flags(0xf), allocator(fn_7100083990()) {}
    base::UnkVector<base::global::CRntString>& Strings() {
        return *reinterpret_cast<base::UnkVector<base::global::CRntString>*>(&data);
    }
};

struct UnkStringListHolder {
    u16 field_0x0;
    UnkStringList list;  // +0x8
    u32 field_0x40;
    alignas(8) char field_0x48[0x40];
    nn::os::MutexType mutex;  // +0x88
};
static_assert(__builtin_offsetof(UnkStringListHolder, mutex) == 0x88, "layout");

// Builds the holder: an empty list with room for five strings, then clears it under the lock and
// stores `value`.
__attribute__((flatten)) void fn_7100dfb050(UnkStringListHolder* self, u32 value) {
    new (&self->list) UnkStringList();
    self->list.Strings().Reserve(5);
    fn_7100fc71ac(self->field_0x48);
    fn_71000668f8(&self->mutex);
    fn_710006691c(&self->mutex);
    self->field_0x0 = 0;
    self->field_0x40 = 0x7fffffff;
    self->list.field_0x0 = 0;
    self->list.field_0x8 = 0;
    self->list.Strings().Clear();
    fn_7100066920(&self->mutex);
    self->field_0x40 = value;
}
