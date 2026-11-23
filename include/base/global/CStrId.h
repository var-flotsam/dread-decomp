#pragma once

// base::global::CStrId: a pooled, reference-counted string id. The name comes from
// reflection: fn_7100009658 (refl_GetType<base::global::CStrId>) describes the values built by
// fn_71000003d4. Reflection descriptors take their type name as one. Members keep
// offset-based names.

#include "types.h"

namespace base::global {

class CRntString;

struct UnkStringData {
    u8 unk_0x0[0x8];
    // A union member: Clang gives union accesses no type-based alias info, so the decrement in
    // CStrId's move-assignment and destructors forces the reloads the original has
    // (CStrId::data and ref_count after `--ref_count`).
    union {
        s32 ref_count;  // 0x8
    };
    u8 unk_0xc[0x4];
    // The string itself: a CRntString in the original (base/global/CRntString.h), of which only
    // the text and the key are read through an id. Its key, the 64-bit hash, is read as two words.
    const char* text;  // 0x10 (CRntString::data)
    u8 unk_0x18[0x18];
    u32 hash[2];  // 0x30 (CRntString::key)
    u8 unk_0x38[0xc];
    // How often the text was read (UnkStrIdBase::Text). A union member for the same reason as
    // ref_count: after the increment the original reloads the id and whatever else it read.
    union {
        s32 reads;  // 0x44
    };

    // The key read whole, as one 64-bit word (the timeline and parasite lookups).
    u64 Key() const { return *reinterpret_cast<const u64*>(hash); }
    // The whole string, for the calls that take the CRntString by reference.
    const CRntString& String() const { return *reinterpret_cast<const CRntString*>(&text); }
};
static_assert(__builtin_offsetof(UnkStringData, text) == 0x10, "UnkStringData layout");
static_assert(__builtin_offsetof(UnkStringData, hash) == 0x30, "UnkStringData layout");
static_assert(__builtin_offsetof(UnkStringData, reads) == 0x44, "UnkStringData layout");

// A pooled string as the hash maps read it (its hash at 0x30): the same layout.
using UnkHashedString = UnkStringData;

struct UnkStringPool {
    u8 unk_0x0[0x10];
    UnkStringData empty;
};

}  // namespace base::global

// The empty text an id with no string reads as (also what the CRntString constructors pass to the
// pool for a string with no text; fn_7100000850, src/base/global/fn_71000003d4.cpp).
extern const char g_unk_710162e2ed[] __attribute__((visibility("hidden")));

// The global string pool (reached through the GOT).
extern base::global::UnkStringPool* g_unk_7101d47108;
extern base::global::UnkStringPool* g_unk_7101d47200;  // CFilePathStrId's pool (base/global/CFilePathStrId.h)
void StringPool_Release(base::global::UnkStringPool* pool, base::global::UnkStringData* data);
u64 StringCrc64(const char* str, u64 length);  // CRC64 of a string
base::global::UnkStringData* StringPool_Find(base::global::UnkStringPool* pool, u64 hash);  // lookup
base::global::UnkStringData* StringPool_Insert(base::global::UnkStringPool* pool, u64 hash, const char* str,
                                           u32 length, bool flag);  // insert
u32 fn_7100080568();
void StringPool_InitHeap();
// A string id's destructor, out of line: what static initialisers register with the atexit stub.
void CStrId_Destroy(void* id);

namespace base::global {

// A weak function-local static (guard and object reached through the GOT) whose constructor
// creates the pool: what the string-id constructor runs when there's no pool yet.
struct UnkStringPoolHolder {
    u8 unk_0x0[0xe8];
    // Defined in src/base/global/UnkStringPoolHolder.cpp.
    UnkStringPoolHolder(u32 a, UnkStringPool** pool) asm("fn_710007fe68");
    ~UnkStringPoolHolder() asm("fn_7100080004");
};

inline void UnkEnsureStringPool() {
    static UnkStringPoolHolder s_holder(fn_7100080568(), &g_unk_7101d47108);
    StringPool_InitHeap();
}

// Tag for the string-id constructor's inline definition. Game code calls it out of line
// (fn_71000003d4); a few base library getters (base::file, base::animation) have it inlined,
// with the string length folded.
struct UnkStrIdInline {};

}  // namespace base::global

namespace base::global {

// What CStrId and UnkStrId share: the pointer and the constructor (fn_71000003d4). Its
// destructor is trivial; each id type releases the string in its own destructor.
class UnkStrIdBase {
public:
    UnkStringData* data;

    UnkStrIdBase(const char* str, bool flag);  // fn_71000003d4
    // The same constructor, defined inline (matches fn_71000003d4 out of line).
    __attribute__((always_inline)) UnkStrIdBase(UnkStrIdInline, const char* str, bool flag) {
        data = nullptr;
        if (!g_unk_7101d47108)
            UnkEnsureStringPool();
        if (str) {
            u64 length = __builtin_strlen(str);
            if (length) {
                u64 hash = StringCrc64(str, length);
                data = StringPool_Find(g_unk_7101d47108, hash);
                if (data)
                    ++data->ref_count;
                else
                    data = StringPool_Insert(g_unk_7101d47108, hash, str, length, flag);
            }
        }
        if (!data)
            data = &g_unk_7101d47108->empty;
    }
    UnkStrIdBase(const UnkStrIdBase& other) = default;

    // The id's text, counting the read (inlined wherever an id is shown or compared as text).
    const char* Text() const {
        ++data->reads;
        return data->text ? data->text : g_unk_710162e2ed;
    }

protected:
    // Leaves `data` for an out-of-line constructor to set (fn_7100000850).
    struct BuiltOutOfLine {};
    explicit UnkStrIdBase(BuiltOutOfLine) {}

    // Inlined into the move-assignments. Unlike the destructors, it decrements and tests in
    // two steps (the original reloads `data` and the count after the decrement). The
    // moved-from id gets the empty string of a freshly loaded pool.
    void MoveFrom(UnkStrIdBase& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
        }
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
};

class CStrId : public UnkStrIdBase {
public:
    using UnkStrIdBase::UnkStrIdBase;
    // Not inherited: an inheriting constructor wouldn't be inlined.
    __attribute__((always_inline)) CStrId(UnkStrIdInline tag, const char* str, bool flag)
        : UnkStrIdBase(tag, str, flag) {}
    // From a string, out of line (fn_7100000850, below).
    CStrId(const CRntString& str, bool flag);
    // The global empty string id (g_unk_7101d470e0, below): default ids are copied from it.
    static CStrId& Empty();

    CStrId(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    CStrId& operator=(CStrId&& other) {
        MoveFrom(other);
        return *this;
    }
    // Matched in refl_RegisterNamed (fn_7100072940): `data` is read before `other.data`, and the
    // released id is reloaded for the release call (ref_count is a union member).
    CStrId& operator=(const CStrId& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = d;
        }
        return *this;
    }
    // The count is decremented, then tested (the dictionaries' clears and move-assignments reload it after
    // the store); where nothing is stored in between, both forms compile the same.
    ~CStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    // The release part of ReleaseAndTake on its own (fn_71002ee65c): the same two-step release,
    // leaving the empty id.
    void Reset() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    // The move assignment of the records that hold an id (fn_71002e1dc0, fn_71003eeca8): the id is
    // released like a destructor would, but in two steps (the original reloads `data` and the
    // count after the decrement), then takes the other's; the other gets the empty id.
    void ReleaseAndTake(CStrId& other) {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
    }
    // ReleaseAndTake as the move of the keyed arrays' iterator values and of their records
    // (fn_7100e558f8, fn_7100e5ae40): the moved-from id gets the empty id through a store with no
    // type-based alias info, so what follows (the pool, counts, array pointers, the holder's kind)
    // is loaded again, as the original does. Elsewhere (fn_71000d8b90, fn_71002d9994) the original
    // keeps those values across the store, so it is a member of its own.
    void ReleaseAndTakeReloading(CStrId& other) {
        Reset();
        data = other.data;
        union Slot {
            UnkStringData* p;
        };
        reinterpret_cast<Slot*>(&other.data)->p = &g_unk_7101d47108->empty;
    }
    // The same take into a slot that holds no id yet (an array's grown or moved-into storage):
    // nothing is released first (fn_71002a6d38, fn_7100e6164c).
    static void MoveConstructReloading(CStrId* slot, CStrId& other) {
        slot->data = other.data;
        union Slot {
            UnkStringData* p;
        };
        reinterpret_cast<Slot*>(&other.data)->p = &g_unk_7101d47108->empty;
    }

    // Ids are equal when they share the pooled string.
    bool operator==(const CStrId& other) const { return data == other.data; }
    // The bucket hash of an id: its string's hash, folded to one word.
    u32 Hash() const {
        const UnkHashedString* string = static_cast<const UnkHashedString*>(data);
        return string->hash[0] ^ string->hash[1];
    }
};

// The id type of a reflected function's name (CFunction+0x30). It is built by the same
// constructor as CStrId but destroyed differently: in fn_71004f01a8 the CStrId passed to
// fn_7100097550 is released by CStrId's destructor (empty id from the pool reloaded after a
// release), while the moved-from temporary assigned to the name folds away only if its
// destructor ends with `data = &pool->empty` on the pool it loaded first. The getter and
// setter initialisers (fn_71004f01a8, fn_7100a19530) match that way. Their release branch
// is dead, so the release part of this destructor is unverified.
class UnkStrId : public UnkStrIdBase {
public:
    using UnkStrIdBase::UnkStrIdBase;
    UnkStrId& operator=(UnkStrId&& other) {
        MoveFrom(other);
        return *this;
    }
    ~UnkStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
            data = &pool->empty;
        }
    }
};

class CRntString;

}  // namespace base::global

// The string-id constructor from a CRntString (fn_7100000850), out of line: the string's cached
// CRC-64 (`key`) is the pool hash, so nothing is rehashed, and a string with no text is passed as
// "". Defined in src/base/global/fn_71000003d4.cpp.
void CStrId_ConstructFromRntString(base::global::UnkStrIdBase* self, const base::global::CRntString& str, bool flag);
inline base::global::CStrId::CStrId(const CRntString& str, bool flag) : UnkStrIdBase(BuiltOutOfLine()) {
    CStrId_ConstructFromRntString(this, str, flag);
}

// The global empty string id: a guarded static that unit 0's initializer (fn_7100001390) builds
// from no string, so it holds the pool's empty entry; its destructor is fn_710000051c. Default
// ids are copied from it and ids are compared with it.
extern base::global::CStrId g_unk_7101d470e0;
inline base::global::CStrId& base::global::CStrId::Empty() { return g_unk_7101d470e0; }
