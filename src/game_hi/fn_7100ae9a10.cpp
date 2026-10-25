// Builds a qualified id for an object: joins a prefix id, the object's group id (vf_0x20) and
// its own id (vf_0x28) with ':' (skipping empty parts), and stores the result and the object.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"

using base::global::CStrId;
using base::global::UnkStringData;

// A string with 0x80 bytes of inline storage (the string part is 0x30 bytes, followed by
// its inline allocator).
struct UnkInlineString {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;
};

extern void* g_unk_7101a1b750[];
void fn_710007e6e4(UnkInlineString* self, void* allocator);  // string constructor
void fn_710007f3b4(UnkInlineString* self, const void* str);  // append a string
void fn_710007f5ac(UnkInlineString* self, char c);           // append a character
void fn_710007ed40(UnkInlineString* self);
void fn_7100000850(UnkStringData** out, UnkInlineString* str, bool flag);  // id from a string

struct UnkIdSource {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual const CStrId& vf_0x20();
    virtual const CStrId& vf_0x28();
};

struct UnkQualifiedId {
    CStrId id;
    UnkIdSource* source;
};

// The string data's use count, a union member like ref_count (no type-based alias info, so
// the id is reloaded after the increment).
struct UnkStringUse {
    u8 unk_0x0[0x44];
    union {
        u32 use_count;
    };
};

static inline const void* UseStr(const CStrId& id) {
    ++reinterpret_cast<UnkStringUse*>(id.data)->use_count;
    return reinterpret_cast<char*>(id.data) + 0x10;
}

// Compared as integers: a pointer comparison puts the operands the other way round.
static inline bool HasValue(const CStrId& id) {
    return (u64)id.data != (u64)g_unk_7101d47108 + 0x10;
}

void fn_7100ae9a10(UnkQualifiedId* self, UnkIdSource* source, const CStrId& prefix) {
    UnkInlineString s;
    fn_710007e6e4(&s, &s.vtable);
    s.vtable = &g_unk_7101a1b750[2];
    s.field_0xb8 = 0;
    s.field_0xc0 = 0;
    self->source = source;
    if (HasValue(prefix)) {
        fn_710007f3b4(&s, UseStr(prefix));
        fn_710007f5ac(&s, ':');
    }
    if (HasValue(source->vf_0x20())) {
        fn_710007f3b4(&s, UseStr(source->vf_0x20()));
        fn_710007f5ac(&s, ':');
    }
    fn_710007f3b4(&s, UseStr(source->vf_0x28()));
    {
        UnkStringData* tmp;
        fn_7100000850(&tmp, &s, true);
        // Move-assign: release the old id, then take over the new one.
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (self->id.data && self->id.data != &pool->empty) {
                --self->id.data->ref_count;
                if (self->id.data->ref_count == 0) {
                    fn_71000803a4(pool, self->id.data);
                    pool = g_unk_7101d47108;
                }
            }
            self->id.data = &pool->empty;
    }
        self->id.data = tmp;
    }
    fn_710007ed40(&s);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&s));
}
