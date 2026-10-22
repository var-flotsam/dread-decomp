// Construction of cached iterator values (base::UnkIterValue) from another one: an owned pair is
// moved or copied, a referenced pair is read through the reference (the value then owns it).
// Moves with string-id elements and 4-byte indices (fn_71000cd580; CFilePathStrId's pool
// fn_71000cfd3c), with string-id keys and 4- or 8-byte elements (fn_71000de484, fn_71000de9bc;
// fn_7100120758, fn_7100120c90, fn_71002424e8, fn_7100242a20, fn_7100251208, fn_71002515ac),
// with two-word elements (fn_7100121610, fn_7100121984) and with 8-byte indices and elements
// (fn_71001f1cd8, fn_71001f1fc8), with string-id keys and CFilePathStrId elements (moved in
// fn_71000da264, copied in fn_71000da8dc), with string-id keys and elements (fn_71000d887c),
// with elements holding a string id and a file path id (moved in fn_710001d334, copied in
// fn_710001d9ac), with reflected-value elements (fn_71000d5178; string-id keys fn_71000d6698,
// fn_71000d7740); copies with string-id elements (fn_71000cda10; CFilePathStrId's
// pool fn_71000d01cc).
#include <new>

#include "base/UnkCollectionIter.h"
#include "base/reflection/UnkReflValue.h"
#include "base/global/CStrId.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::UnkIterValue;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id moved out by taking its data and leaving the pool's empty string.
template <UnkStringPool** Pool>
struct UnkMovedStrIdT {
    UnkStringData* data;

    UnkMovedStrIdT(UnkMovedStrIdT&& other) : data(other.data) { other.data = &(*Pool)->empty; }
};

// A string id copied by adding a reference (unless it is the empty string).
template <UnkStringPool** Pool>
struct UnkCopiedStrIdT {
    UnkStringData* data;

    UnkCopiedStrIdT(const UnkCopiedStrIdT& other) : data(other.data) {
        if (&(*Pool)->empty != data)
            ++data->ref_count;
    }
};

using UnkMovedStrId = UnkMovedStrIdT<&g_unk_7101d47108>;
using UnkMovedFilePathStrId = UnkMovedStrIdT<&g_unk_7101d47200>;
using UnkCopiedStrId = UnkCopiedStrIdT<&g_unk_7101d47108>;
using UnkCopiedFilePathStrId = UnkCopiedStrIdT<&g_unk_7101d47200>;

// Elements moved member by member.
struct UnkWordPair {
    u32 first;
    u32 second;

    UnkWordPair(UnkWordPair&& other) : first(other.first), second(other.second) {}
};

struct UnkWord {
    u32 value;

    UnkWord(UnkWord&& other) : value(other.value) {}
};

// A string id and a file path id, moved or (with no move constructor) copied member by member.
struct UnkMovedIdPair {
    UnkMovedStrId id;
    UnkMovedFilePathStrId path;
};

struct UnkCopiedIdPair {
    UnkCopiedStrId id;
    UnkCopiedFilePathStrId path;
};

// A reflected value copied through fn_71000a18d4 (the value is read in place when it fits in 8
// bytes, unless it is stored through field_0x0).
struct UnkCopiedReflValue {
    void* field_0x0;
    const char* type;
    u32 kind;
    u32 field_0x14;

    UnkCopiedReflValue(const UnkCopiedReflValue& other) {
        type = nullptr;
        field_0x0 = nullptr;
        kind = other.kind;
        const void* value = nullptr;
        if (other.type)
            value = other.field_0x14 == 1 || *reinterpret_cast<const u64*>(other.type + 0x18) >= 9 ? other.field_0x0
                                                                                                  : &other;
        fn_71000a18d4(reinterpret_cast<base::reflection::UnkReflValue*>(this),
                      reinterpret_cast<const base::reflection::CType*>(other.type), value, false);
    }
};

// A counted reference (UnkWeakHandle.h), copied as "null, then assign".
struct UnkCopiedRef {
    UnkRefObject* ptr;

    UnkCopiedRef(const UnkCopiedRef& other) : ptr(nullptr) { AssignRef(&ptr, other.ptr); }
};

// A counted resource (count at 0x54, freed by fn_710001737c), referenced the same way.
struct UnkCounted54 {
    u8 unk_0x0[0x54];
    union {  // (the original reloads around the count, as through a union)
        s32 ref_count;
    };
};
void fn_710001737c(UnkCounted54* obj);

struct UnkCopiedRef54 {
    UnkCounted54* ptr;

    UnkCopiedRef54(const UnkCopiedRef54& other) : ptr(nullptr) {
        if (UnkCounted54* p = other.ptr) {
            ++p->ref_count;
            if (UnkCounted54* old = ptr) {
                if (--old->ref_count == 0)
                    fn_710001737c(old);
            }
            ptr = p;
        }
    }
};

// A file path id (moved or copied) and a counted resource, after 8 plain bytes or not.
template <class Path>
struct UnkPathRef {
    Path path;
    UnkCopiedRef54 shared;
};

template <class Path>
struct UnkWordPathRef {
    u64 field_0x0;
    Path path;
    UnkCopiedRef54 shared;
};

template <class T, class K>
static inline void CopyConstruct(UnkIterValue<T, K>* self, const UnkIterValue<T, K>* other) {
    using Value = typename UnkIterValue<T, K>::Value;
    self->state = 0;
    if (other->state == 1) {
        self->state = 1;
        new (&self->value) Value(other->value);
    } else if (other->state == 2) {
        self->state = 1;
        typename UnkIterValue<T, K>::Ref r = other->ref;
        new (&self->value) Value{*r.index, *r.element};
    }
}

#define UNK_ITER_MOVE(addr, T, K)                                                                 \
    __attribute__((flatten)) void fn_##addr(void* dst, const void* src) {                         \
        base::refl_IterValueMove<T, K>(dst, src);                                                 \
    }

UNK_ITER_MOVE(71000cd580, UnkMovedStrId, u32)
UNK_ITER_MOVE(71000cfd3c, UnkMovedFilePathStrId, u32)
UNK_ITER_MOVE(71000de484, UnkWord, UnkMovedStrId)
UNK_ITER_MOVE(71000de9bc, UnkWord, UnkMovedStrId)
UNK_ITER_MOVE(7100120758, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100120c90, u64, UnkMovedStrId)
UNK_ITER_MOVE(71002424e8, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100242a20, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100251208, u64, UnkMovedStrId)
UNK_ITER_MOVE(71002515ac, u64, UnkMovedStrId)
UNK_ITER_MOVE(7100121610, UnkWordPair, u32)
UNK_ITER_MOVE(7100121984, UnkWordPair, u32)
UNK_ITER_MOVE(71001f1cd8, u64, u64)
UNK_ITER_MOVE(71001f1fc8, u64, u64)
UNK_ITER_MOVE(71000da264, UnkMovedFilePathStrId, UnkMovedStrId)
UNK_ITER_MOVE(71000da8dc, UnkCopiedFilePathStrId, UnkMovedStrId)
UNK_ITER_MOVE(71000d887c, UnkMovedStrId, UnkMovedStrId)
UNK_ITER_MOVE(710001d334, UnkMovedIdPair, u32)
UNK_ITER_MOVE(710001d9ac, UnkCopiedIdPair, u32)

void fn_71000cda10(UnkIterValue<UnkCopiedStrId>* self, const UnkIterValue<UnkCopiedStrId>* other) {
    CopyConstruct(self, other);
}

void fn_71000d01cc(UnkIterValue<UnkCopiedFilePathStrId>* self, const UnkIterValue<UnkCopiedFilePathStrId>* other) {
    CopyConstruct(self, other);
}

// The same move written as a placement new (the move constructor isn't inlined otherwise).
#define UNK_ITER_MOVE_NEW(addr, T, K)                                                             \
    __attribute__((flatten)) void fn_##addr(UnkIterValue<T, K>* dst, UnkIterValue<T, K>* src) {   \
        new (dst) UnkIterValue<T, K>(static_cast<UnkIterValue<T, K>&&>(*src));                    \
    }

UNK_ITER_MOVE_NEW(71000d5178, UnkCopiedReflValue, u32)
UNK_ITER_MOVE_NEW(71000d6698, UnkCopiedReflValue, UnkMovedStrId)
UNK_ITER_MOVE_NEW(71000d7740, UnkCopiedReflValue, UnkMovedStrId)
UNK_ITER_MOVE_NEW(7100240df0, UnkCopiedRef, u32)
UNK_ITER_MOVE_NEW(71002411d0, UnkCopiedRef, u32)
UNK_ITER_MOVE_NEW(7100267598, UnkPathRef<UnkMovedFilePathStrId>, u32)
UNK_ITER_MOVE_NEW(7100267a00, UnkPathRef<UnkCopiedFilePathStrId>, u32)
UNK_ITER_MOVE_NEW(710000d938, UnkWordPathRef<UnkMovedFilePathStrId>, u32)
UNK_ITER_MOVE_NEW(710000ddb8, UnkWordPathRef<UnkCopiedFilePathStrId>, u32)
