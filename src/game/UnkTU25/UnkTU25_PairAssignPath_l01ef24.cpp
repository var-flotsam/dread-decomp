// Unit 25: assigns a string id and file-path id pair from path text (unless it is the pair itself): the ids
// are built from the text (fn_710001f074) and moved into the pair. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// The moved-from id is cleared through a union, so the pool is reloaded after the store.
union UnkIdSlot_l01ef24 {
    UnkStringData* p;
};

struct UnkPairId_l01ef24 : base::global::CStrId {
    UnkPairId_l01ef24& operator=(UnkPairId_l01ef24&& other) {
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
        reinterpret_cast<UnkIdSlot_l01ef24*>(&data)->p = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
};

struct UnkPairPath_l01ef24 : base::global::CFilePathStrId {
    UnkPairPath_l01ef24& operator=(UnkPairPath_l01ef24&& other) {
        UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &pool->empty;
        return *this;
    }
};

struct UnkPair_l01ef24 {
    UnkPairId_l01ef24 id;
    UnkPairPath_l01ef24 path;
};

// The ids the builder fills in place (the members start out unset).
struct UnkBuiltIds_l01ef24 {
    union {
        UnkPairId_l01ef24 id;
    };
    union {
        UnkPairPath_l01ef24 path;
    };

    explicit UnkBuiltIds_l01ef24(const UnkInlineString* source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), source);
    }
    ~UnkBuiltIds_l01ef24() {
        path.~UnkPairPath_l01ef24();
        id.~UnkPairId_l01ef24();
    }
};

UnkPair_l01ef24* fn_710001ef24(UnkPair_l01ef24* self, const UnkInlineString* other) {
    if (reinterpret_cast<const void*>(other) != reinterpret_cast<const void*>(self)) {
        UnkBuiltIds_l01ef24 ids(other);
        self->id = static_cast<UnkPairId_l01ef24&&>(ids.id);
        self->path = static_cast<UnkPairPath_l01ef24&&>(ids.path);
    }
    return self;
}
