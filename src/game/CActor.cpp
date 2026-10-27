// A function of the CActor unit (unit 341, tools/tu_map.py): the counted weak reference to the actor
// that a reflected property value holds.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game_s2/UnkWeakHandle.h"

using namespace base::reflection;

// A reflected property read of `member` (the first member of a call's arguments).
UnkReflValue fn_7100ad7cc0(void* member);
// The class of the actor (a CClass object, initialised by CActor::refl_GetClass).
extern char g_unk_717269f450[] __attribute__((visibility("hidden")));

namespace {

// Destroys the reflected value when the function returns.
struct ValueGuard {
    UnkReflValue* value;
    ~ValueGuard() { fn_71000a1c60(value); }
};

// A pointer type's function at +0xf0: reads the pointee out of the pointer value, giving its type
// and the object (both zero for a null pointer); returned in memory.
typedef UnkReflTypedPtr (*UnkPointeeFn)(void* pointer);

}  // namespace

// The reference to the object the reflected value `member` reads as (an actor, directly or through
// a pointer), or null.
// NON_MATCHING(minor): same behaviour; the original has a 0x80-byte frame, keeps the pointee pair in
// a stack slot addressed through x23 and lays the two branches out differently.
UnkRefPtr fn_710023b580(void* member) {
    UnkReflValue value = fn_7100ad7cc0(member);
    ValueGuard guard = {&value};
    UnkRefPtr result;
    result.field_0x0 = nullptr;
    UnkWeakTarget* object = nullptr;
    if (value.type) {
        const UnkReflObject* type = reinterpret_cast<const UnkReflObject*>(value.type);
        bool is_pointer = fn_710009d50c(type->vf_0x0(), fn_710009bedc());
        if (!is_pointer) {
            fn_7100227448();
            fn_710009d50c(reinterpret_cast<const CType*>(value.type), reinterpret_cast<const CType*>(g_unk_717269f450));
            char* t = const_cast<char*>(value.type);
            void* addr = !t ? nullptr
                            : (value.field_0x14 != 1 && *reinterpret_cast<u64*>(t + 0x18) < 9) ? static_cast<void*>(&value)
                                                                                                  : value.field_0x0;
            object = static_cast<UnkWeakTarget*>(addr);
        } else {
            char* t = const_cast<char*>(value.type);
            void* addr = !t ? nullptr
                            : (value.field_0x14 != 1 && *reinterpret_cast<u64*>(t + 0x18) < 9) ? static_cast<void*>(&value)
                                                                                                  : value.field_0x0;
            char* pointer_type = nullptr;
            if (fn_710009d50c(reinterpret_cast<const UnkReflObject*>(t)->vf_0x0(), fn_710009bedc()))
                pointer_type = t;
            UnkPointeeFn read = *reinterpret_cast<UnkPointeeFn*>(pointer_type + 0xf0);
            UnkReflTypedPtr pointee;
            if (read && t == pointer_type) {
                pointee = read(addr);
            } else {
                pointee.type = nullptr;
                pointee.bits = 0;
            }
            char* refined = fn_710009dcd8(pointee.type, pointee.ptr);
            fn_7100227448();
            fn_710009d50c(reinterpret_cast<const CType*>(refined), reinterpret_cast<const CType*>(g_unk_717269f450));
            object = static_cast<UnkWeakTarget*>(pointee.ptr);
        }
        if (UnkWeakHandle* handle = GetWeakHandle(object)) {
            result.field_0x0 = handle;
            if (++handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    return result;
}

// std::sort over a keyed array (two parallel arrays: 8-byte values and their string-id keys, as
// fn_7100248ce8 reserves them), called by fn_71002323e0 on the array at +0xa0 of its object: the
// libc++ helpers instantiated for an iterator that refers to one key and its value, largest
// fn_710024fb58 of the value first. The comparator has internal linkage, so every instantiation is
// internal and std::__sort inlines __insertion_sort_3 as the game's copy does. Names and layouts
// are placeholders.
#include <algorithm>
#include <iterator>
#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"

struct UnkSortedObj_7100248e50;
// The sort key of a value: a word of the object its +0x10 points to.
s32 fn_710024fb58(UnkSortedObj_7100248e50* obj);

// The value an iterator refers to (state 2), or a moved-out key and value (state 1).
using UnkSortValue_7100248e50 = base::UnkIterValue<UnkSortedObj_7100248e50*, base::UnkStrIdMember>;

// Its move-assignment is out of line (fn_710024caa0, one of the clones' instantiations).
template <>
UnkSortValue_7100248e50& UnkSortValue_7100248e50::operator=(UnkSortValue_7100248e50&& other) asm("fn_710024caa0");

// The iterator: a value that refers to the key and the value at its position. Copies refer to the
// same position; dereferencing gives the value itself.
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

namespace {

struct UnkSortCmp_7100248e50 {
    bool operator()(const UnkSortValue_7100248e50& a, const UnkSortValue_7100248e50& b) const {
        return fn_710024fb58(a.Element()) > fn_710024fb58(b.Element());
    }
};

}  // namespace

// Not in the game: a direct call, so that the internal instantiations are emitted and their unused
// comparator arguments dropped, as in the game.
void unk_anchor_7100248e50(UnkSortIter_7100248e50 first, UnkSortIter_7100248e50 last) {
    std::sort(first, last, UnkSortCmp_7100248e50());
}

#if defined(__aarch64__)
// The instantiations are internal, so their names are set in assembly (an alias would keep the
// comparator arguments).
asm(".globl fn_7100248e50\n.set fn_7100248e50, _ZNSt3__16__sortIRN12_GLOBAL__N_121UnkSortCmp_7100248e50E22UnkSortIter_7100248e50EEvT0_S5_T_");
asm(".globl fn_710024acd0\n.set fn_710024acd0, _ZNSt3__17__sort3IRN12_GLOBAL__N_121UnkSortCmp_7100248e50E22UnkSortIter_7100248e50EEjT0_S5_S5_T_");
asm(".globl fn_710024b290\n.set fn_710024b290, _ZNSt3__17__sort4IRN12_GLOBAL__N_121UnkSortCmp_7100248e50E22UnkSortIter_7100248e50EEjT0_S5_S5_S5_T_");
asm(".globl fn_710024b780\n.set fn_710024b780, _ZNSt3__17__sort5IRN12_GLOBAL__N_121UnkSortCmp_7100248e50E22UnkSortIter_7100248e50EEjT0_S5_S5_S5_S5_T_");
asm(".globl fn_710024be10\n.set fn_710024be10, _ZNSt3__127__insertion_sort_incompleteIRN12_GLOBAL__N_121UnkSortCmp_7100248e50E22UnkSortIter_7100248e50EEbT0_S5_T_");
#else
// The port: another target inlines the helpers or keeps the comparator argument, so the names
// above may not exist. Each name is then a function of its own over the same instantiation, with
// the arguments the game's callers pass (iterators by value are passed by address).
extern "C" void fn_7100248e50(UnkSortIter_7100248e50 first, UnkSortIter_7100248e50 last) {
    std::sort(first, last, UnkSortCmp_7100248e50());
}
extern "C" unsigned fn_710024acd0(UnkSortIter_7100248e50 a, UnkSortIter_7100248e50 b, UnkSortIter_7100248e50 c) {
    UnkSortCmp_7100248e50 cmp;
    return std::__sort3<UnkSortCmp_7100248e50&>(a, b, c, cmp);
}
extern "C" unsigned fn_710024b290(UnkSortIter_7100248e50 a, UnkSortIter_7100248e50 b, UnkSortIter_7100248e50 c,
                                  UnkSortIter_7100248e50 d) {
    UnkSortCmp_7100248e50 cmp;
    return std::__sort4<UnkSortCmp_7100248e50&>(a, b, c, d, cmp);
}
extern "C" unsigned fn_710024b780(UnkSortIter_7100248e50 a, UnkSortIter_7100248e50 b, UnkSortIter_7100248e50 c,
                                  UnkSortIter_7100248e50 d, UnkSortIter_7100248e50 e) {
    UnkSortCmp_7100248e50 cmp;
    return std::__sort5<UnkSortCmp_7100248e50&>(a, b, c, d, e, cmp);
}
extern "C" bool fn_710024be10(UnkSortIter_7100248e50 first, UnkSortIter_7100248e50 last) {
    UnkSortCmp_7100248e50 cmp;
    return std::__insertion_sort_incomplete<UnkSortCmp_7100248e50&>(first, last, cmp);
}
#endif
