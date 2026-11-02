// Assignment of a pair of a value and a record (held in place or by reference, state 2) from
// another such pair, in CEnvironmentData's unit: the same as fn_7100eaee5c
// (src/base/reflection/fn_7100adae44.cpp) with this unit's record.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

template <class X, class Y>
struct UnkEnvValuePairRef {
    union {
        struct {
            X x;
            Y y;
        } values;
        struct {
            X* x;
            Y* y;
        } refs;
    };
    s32 state;

    X& GetX() { return state == 2 ? *refs.x : values.x; }
    Y& GetY() { return state == 2 ? *refs.y : values.y; }
};

template <class X, class Y>
static inline UnkEnvValuePairRef<X, Y>* Assign(UnkEnvValuePairRef<X, Y>* self, UnkEnvValuePairRef<X, Y>* other) {
    if (self != other) {
        self->GetX() = other->GetX();
        self->GetY() = other->GetY();
    }
    return self;
}

// A polymorphic record: an id and three words after the vtable, assigned member by member (the
// vtable pointer is kept).
struct UnkEnvIdRecord {
    struct Words {
        u64 w[3];
    };

    virtual void vf_0x0();
    CStrId id;
    Words words;
};

using UnkEnvU32IdValuesPair = UnkEnvValuePairRef<u32, UnkEnvIdRecord>;

UnkEnvU32IdValuesPair* fn_7100f6d7f4(UnkEnvU32IdValuesPair* self, UnkEnvU32IdValuesPair* other) {
    return Assign(self, other);
}

// The same in unit 2640 (tools/tu_map.py), with the id from CFilePathStrId's pool.
struct UnkEnvFilePathId {
    base::global::UnkStringData* data;

    UnkEnvFilePathId& operator=(const UnkEnvFilePathId& other) {
        using base::global::UnkStringData;
        using base::global::UnkStringPool;
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

// A file-path id and three values, assigned member by member.
struct UnkEnvFilePathIdRecord {
    struct Values {
        u32 v[3];
    };

    UnkEnvFilePathId id;
    Values values;
};

using UnkEnvU32FilePathIdPair = UnkEnvValuePairRef<u32, UnkEnvFilePathIdRecord>;

UnkEnvU32FilePathIdPair* fn_7100ffb044(UnkEnvU32FilePathIdPair* self, UnkEnvU32FilePathIdPair* other) {
    return Assign(self, other);
}
