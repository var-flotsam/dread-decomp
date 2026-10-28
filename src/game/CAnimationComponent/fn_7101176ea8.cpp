// Forwards a request record (unless its id is -1) with the entry found by key and the default all-bits mask.
#include "types.h"

struct UnkEntry_7101176ea8 {
    char pad[0x70];
    void* field_0x70;
};

struct UnkMap_7101176ea8 {
    char pad[0xc8];
    UnkEntry_7101176ea8** values;
    u64* keys;
    u32 count;

    UnkEntry_7101176ea8** Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return values + count;
    }
};

struct UnkStruct_7101176ea8 {
    char pad[0x40];
    UnkMap_7101176ea8* field_0x40;
};

struct UnkFlags_7101176ea8 {
    u32 value;
    UnkFlags_7101176ea8(u32 v) : value(v) {}
    UnkFlags_7101176ea8(const UnkFlags_7101176ea8& o) : value(o.value) {}
};
struct UnkS16 { u64 a, b; };
struct UnkS12 { u32 a, b, c; };

void* fn_7101176d34(UnkStruct_7101176ea8* self, void* arg);
bool fn_7101177044(UnkStruct_7101176ea8* self, void* r, s32 idx, void* data, UnkFlags_7101176ea8 flags,
                   const UnkS16& a, const UnkS12& b, UnkFlags_7101176ea8 c);

struct UnkMaskBase_7101176ea8 {
    u8 value;
    u8 unk_0x1[3];
    UnkMaskBase_7101176ea8() : value(0), unk_0x1{} {}
};

inline const UnkMaskBase_7101176ea8& NoBits_7101176ea8() {
    static UnkMaskBase_7101176ea8 s_none;
    return s_none;
}

inline UnkFlags_7101176ea8 AllBits_7101176ea8() {
    static UnkFlags_7101176ea8 s_all = static_cast<u8>(~static_cast<u8>(NoBits_7101176ea8().value));
    return s_all;
}

struct UnkRequest_7101176ea8 {
    void* id;
    UnkS16 a;
    UnkS12 b;
    UnkFlags_7101176ea8 flags;
};

static inline bool Send_7101176ea8(UnkStruct_7101176ea8* self, void* id, u64 key, UnkFlags_7101176ea8 flags,
                                   const UnkS16& a, const UnkS12& b, UnkFlags_7101176ea8 mask) {
    void* r = fn_7101176d34(self, id);
    UnkMap_7101176ea8* m = self->field_0x40;
    UnkEntry_7101176ea8** it = m->Find(key);
    UnkEntry_7101176ea8** end = m->values + m->count;
    void* data = (it != end && *it) ? (*it)->field_0x70 : nullptr;
    return fn_7101177044(self, r, -1, data, flags, a, b, mask);
}

bool fn_7101176ea8(UnkStruct_7101176ea8* self, const UnkRequest_7101176ea8* req, u64 key) {
    if (req->id == reinterpret_cast<void*>(-1))
        return false;
    return Send_7101176ea8(self, req->id, key, req->flags, req->a, req->b, AllBits_7101176ea8());
}
