// Three more copies of stream 2's functions on arrays of owned-object lists (fn_71004a8bd4,
// fn_71004a8ddc, fn_71004a8f10, fn_71004a9050, fn_71004a9570, fn_71004a972c, fn_71004a9990 and
// fn_71004a9a90 in src/game_s2/), for other types: clear, compare, free, move-assign, erase,
// grow-at, begin and end.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkCollectionIter.h"
#include "game_s2/UnkOwnedList.h"

using UnkLists = base::UnkVector<UnkOwnedList>;
using UnkListIter = base::UnkCollectionIter<UnkOwnedList>;

namespace {

__attribute__((always_inline)) inline bool Equal(const UnkLists* self, const UnkLists* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        const UnkOwnedList& a = other->data[i];
        const UnkOwnedList& b = self->data[i];
        if (a.count != b.count)
            return false;
        const UnkOwnedPtr* pa = a.data;
        const UnkOwnedPtr* pb = b.data;
        u64 n = a.count;
        for (; n; --n, ++pa, ++pb)
            if (pb->ptr != pa->ptr)
                return false;
    }
    return true;
}

__attribute__((always_inline)) inline void EraseAt(UnkLists* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkOwnedList();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkOwnedList* cur = &self->data[i];
            new (cur) UnkOwnedList(static_cast<UnkOwnedList&&>(cur[1]));
            (&self->data[i])[1].~UnkOwnedList();
        }
        self->count = self->count - 1;
    }
}

}  // namespace

// The move-assignments are flattened: with three users in the file, UnkVector's operator= would
// otherwise stay out of line.

void fn_71005da4f8(UnkLists* self) { self->Clear(); }
bool fn_71005da700(const UnkLists* self, const UnkLists* other) { return Equal(self, other); }
void fn_71005da834(UnkLists* self) { self->Free(); }
__attribute__((flatten)) UnkLists* fn_71005da974(UnkLists* self, UnkLists* other) { return &(*self = static_cast<UnkLists&&>(*other)); }
void fn_71005dae94(UnkLists* self, u32 index) { EraseAt(self, index); }
UnkOwnedList* fn_71005db050(UnkLists* self, u32 index) { return &self->GrowAt(index); }
void fn_71005db2b4(const UnkLists* v, UnkListIter* out) { *out = UnkListIter(0, v->data); }
void fn_71005db3b4(const UnkLists* v, UnkListIter* out) { *out = UnkListIter(v->count, v->data + v->count); }

void fn_71005e3778(UnkLists* self) { self->Clear(); }
bool fn_71005e3980(const UnkLists* self, const UnkLists* other) { return Equal(self, other); }
void fn_71005e3ab4(UnkLists* self) { self->Free(); }
__attribute__((flatten)) UnkLists* fn_71005e3bf4(UnkLists* self, UnkLists* other) { return &(*self = static_cast<UnkLists&&>(*other)); }
void fn_71005e4114(UnkLists* self, u32 index) { EraseAt(self, index); }
UnkOwnedList* fn_71005e42d0(UnkLists* self, u32 index) { return &self->GrowAt(index); }
void fn_71005e4534(const UnkLists* v, UnkListIter* out) { *out = UnkListIter(0, v->data); }
void fn_71005e4634(const UnkLists* v, UnkListIter* out) { *out = UnkListIter(v->count, v->data + v->count); }

void fn_71005ee7a4(UnkLists* self) { self->Clear(); }
bool fn_71005ee9ac(const UnkLists* self, const UnkLists* other) { return Equal(self, other); }
void fn_71005eeae0(UnkLists* self) { self->Free(); }
__attribute__((flatten)) UnkLists* fn_71005eec20(UnkLists* self, UnkLists* other) { return &(*self = static_cast<UnkLists&&>(*other)); }
void fn_71005ef140(UnkLists* self, u32 index) { EraseAt(self, index); }
UnkOwnedList* fn_71005ef2fc(UnkLists* self, u32 index) { return &self->GrowAt(index); }
void fn_71005ef560(const UnkLists* v, UnkListIter* out) { *out = UnkListIter(0, v->data); }
void fn_71005ef660(const UnkLists* v, UnkListIter* out) { *out = UnkListIter(v->count, v->data + v->count); }
