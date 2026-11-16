// UnkTU341 (tools/tu_map.py unit 341, CActor's): when asked, sorts the actor's keyed array of parts
// at +0xa0 (values, their string-id keys, a count) with std::sort (fn_7100248e50, src/game/CActor.cpp).
// Names and layouts are placeholders.
#include <iterator>

#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkSortedObj_7100248e50;

// The value an iterator refers to (state 2), or a moved-out key and value (state 1).
using UnkSortValue_7100248e50 = base::UnkIterValue<UnkSortedObj_7100248e50*, base::UnkStrIdMember>;

// As src/game/CActor.cpp defines it.
struct UnkSortIter_7100248e50 {
    typedef std::random_access_iterator_tag iterator_category;
    typedef UnkSortValue_7100248e50 value_type;
    typedef s64 difference_type;
    typedef UnkSortValue_7100248e50* pointer;
    typedef UnkSortValue_7100248e50& reference;

    UnkSortValue_7100248e50 current;

    UnkSortIter_7100248e50(const base::UnkStrIdMember* index, UnkSortedObj_7100248e50** element) {
        current.SetRef(index, element);
    }
    UnkSortIter_7100248e50(const UnkSortIter_7100248e50& other) {
        current.SetRef(other.current.ref.index, other.current.ref.element);
    }
    UnkSortIter_7100248e50& operator=(const UnkSortIter_7100248e50& other) {
        if (this != &other)
            current.SetRef(other.current.ref.index, other.current.ref.element);
        return *this;
    }

    reference operator*() { return current; }
    UnkSortIter_7100248e50& operator++() {
        ++current.ref.index;
        ++current.ref.element;
        return *this;
    }
    UnkSortIter_7100248e50& operator--() {
        --current.ref.index;
        --current.ref.element;
        return *this;
    }
    UnkSortIter_7100248e50& operator+=(s64 n) {
        current.ref.index += n;
        current.ref.element += n;
        return *this;
    }
    // A new iterator at the position, not a copy that is moved.
    UnkSortIter_7100248e50 operator+(s64 n) const {
        return UnkSortIter_7100248e50(current.ref.index + n, current.ref.element + n);
    }
    s64 operator-(const UnkSortIter_7100248e50& o) const { return current.ref.index - o.current.ref.index; }
    bool operator==(const UnkSortIter_7100248e50& o) const { return current.ref.index == o.current.ref.index; }
    bool operator!=(const UnkSortIter_7100248e50& o) const { return current.ref.index != o.current.ref.index; }
    bool operator<(const UnkSortIter_7100248e50& o) const { return current.ref.index < o.current.ref.index; }
    bool operator>(const UnkSortIter_7100248e50& o) const { return current.ref.index > o.current.ref.index; }
    bool operator>=(const UnkSortIter_7100248e50& o) const { return current.ref.index >= o.current.ref.index; }
};

// std::sort over the array (src/game/CActor.cpp).
extern "C" void fn_7100248e50(UnkSortIter_7100248e50 first, UnkSortIter_7100248e50 last);

// The actor's keyed array of parts.
struct UnkSortedParts_l2323e0 {
    u8 pad_0x0[0xa0];
    UnkSortedObj_7100248e50** values;  // 0xa0
    base::UnkStrIdMember* keys;        // 0xa8
    u32 count;                         // 0xb0

    UnkSortIter_7100248e50 Begin() { return UnkSortIter_7100248e50(keys, values); }
    UnkSortIter_7100248e50 End() { return UnkSortIter_7100248e50(keys + count, values + count); }
};

// Sorts a range, as std::sort hands its iterators on.
inline void SortRange_l2323e0(UnkSortIter_7100248e50 first, UnkSortIter_7100248e50 last) {
    fn_7100248e50(first, last);
}

struct UnkActor_710023259c;

// Signature as src/game/CEntity_2.cpp declares it.
void fn_71002323e0(UnkActor_710023259c* self, bool sort) {
    auto* parts = reinterpret_cast<UnkSortedParts_l2323e0*>(self);
    if (sort)
        SortRange_l2323e0(parts->Begin(), parts->End());
}
