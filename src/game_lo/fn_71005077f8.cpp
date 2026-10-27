// Builds a string id for a kind: the kind's base id, extended for kinds 7 and 13 with the
// string of the id at field_0x7a0, then with one of two fixed suffixes for each of two flags.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The pooled data as seen here: the string value at +0x10 and a counter at +0x44 that is
// bumped whenever the string is read.
struct UnkStringData_71005077f8 {
    u8 pad_0x0[0x8];
    union {
        s32 ref_count;
    };
    u8 pad_0xc[0x4];
    CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 field_0x44;
    };
};

// A pooled string id. Its destructor is not needed here (the temporaries are emptied by the
// move and the result is returned); the ref_count/field_0x44 unions keep the reloads the
// original has.
struct UnkStrId_71005077f8 {
    UnkStringData_71005077f8* data;

    UnkStrId_71005077f8(const CRntString& str, bool flag);  // fn_7100000850
    UnkStrId_71005077f8(const UnkStrId_71005077f8& other);

    // Releases `old` (the current id) and takes next's, leaving it the pool's empty id.
    void Replace(UnkStringData_71005077f8* old, UnkStrId_71005077f8&& next) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            UnkStringData_71005077f8* empty = reinterpret_cast<UnkStringData_71005077f8*>(&pool->empty);
            if (old && old != empty) {
                if (--old->ref_count == 0) {
                    fn_71000803a4(pool, reinterpret_cast<UnkStringData*>(old));
                    pool = g_unk_7101d47108;
                }
            }
            data = next.data;
            next.data = reinterpret_cast<UnkStringData_71005077f8*>(&pool->empty);
        } else {
            data = next.data;
            next.data = reinterpret_cast<UnkStringData_71005077f8*>(&g_unk_7101d47108->empty);
        }
    }

    // Replaces the id with the id of its string followed by `suffix`.
    void Append(const CRntString& suffix);
    void Append(const UnkStrId_71005077f8& suffix);

    // The id's string, counting the read.
    const CRntString& Str() const {
        ++data->field_0x44;
        return data->str;
    }
};

UnkStrId_71005077f8 fn_710050a8f8(u32 kind);

inline void UnkStrId_71005077f8::Append(const CRntString& suffix) {
    UnkStringData_71005077f8* old = data;
    ++old->field_0x44;
    Replace(old, UnkStrId_71005077f8(fn_710007eee0(&old->str, &suffix), true));
}

inline void UnkStrId_71005077f8::Append(const UnkStrId_71005077f8& suffix) {
    UnkStringData_71005077f8* old = data;
    ++old->field_0x44;
    Replace(old, UnkStrId_71005077f8(fn_710007eee0(&old->str, &suffix.Str()), true));
}

extern CRntString unk_71726de7d0 __attribute__((visibility("hidden")));
extern CRntString unk_71726de820 __attribute__((visibility("hidden")));

struct UnkStruct_71005077f8 {
    u8 pad_0x0[0x702];
    bool field_0x702;
    bool field_0x703;
    u8 pad_0x704[0x9c];
    UnkStrId_71005077f8 field_0x7a0;
};

void fn_71005096b4(UnkStruct_71005077f8* self, u32 kind);

UnkStrId_71005077f8 fn_71005077f8(UnkStruct_71005077f8* self, u32 kind) {
    UnkStrId_71005077f8 id = fn_710050a8f8(kind);
    if (kind == 13 || kind == 7) {
        fn_71005096b4(self, kind);
        id.Append(self->field_0x7a0);
    }
    if (self->field_0x702)
        id.Append(unk_71726de7d0);
    if (self->field_0x703)
        id.Append(unk_71726de820);
    return id;
}
