// Unit 271 (base::reflection): adds an entry's object to a list unless the sorted entries a cursor walks
// already hold it there; the cursor first skips the entries ordered before it. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkSortedEntry_l1f181c {
    void* object;  // 0x0
    u64 key;       // 0x8
    s32 order;     // 0x10
};

// The bounds-checked element access of the original's RntVector.h.
static inline const UnkSortedEntry_l1f181c& At(const base::global::CRntVector<UnkSortedEntry_l1f181c>& v, u32 i) {
    if (i >= v.count)
        __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
    return v.data[i];
}

struct UnkObjectRef_l1f181c {
    void* object;

    explicit UnkObjectRef_l1f181c(void* o) : object(o) {}
    ~UnkObjectRef_l1f181c() {}
};

struct UnkSortedCursor_l1f181c {
    void* source;                                              // 0x0: the cursor walks only when set
    base::global::CRntVector<UnkObjectRef_l1f181c>* list;      // 0x8
    const base::global::CRntVector<UnkSortedEntry_l1f181c>* entries;  // 0x10
    u32 end;                                                   // 0x18
    u32 index;                                                 // 0x1c

    const UnkSortedEntry_l1f181c* Current() const { return index < end ? &At(*entries, index) : nullptr; }
};

void fn_71001f181c(UnkSortedCursor_l1f181c* self, const UnkSortedEntry_l1f181c* entry) {
    if (!self->source) {
        self->list->EmplaceBack(UnkObjectRef_l1f181c(entry->object));
        return;
    }
    u32 i = self->index;
    if (i < self->end) {
        u64 key = entry->key;
        s32 order = entry->order;
        do {
            const UnkSortedEntry_l1f181c& e = At(*self->entries, i);
            if (!(e.order < order || (e.order == order && e.key < key)))
                break;
            self->index = ++i;
        } while (i < self->end);
    }
    const UnkSortedEntry_l1f181c* e = self->Current();
    if (e && e->order == entry->order && e->object == entry->object)
        return;
    self->list->EmplaceBack(UnkObjectRef_l1f181c(entry->object));
}
