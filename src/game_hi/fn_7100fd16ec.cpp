// Flushes a pending request: if the dirty flag is set, clears it, submits the two stored
// requests to the manager singleton with the full mask, empties both path lists and resets
// the two string ids to the global default and the priority to the maximum.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

template <base::global::UnkStringPool** Pool>
struct UnkPooledId {
    base::global::UnkStringData* data;

    ~UnkPooledId() {
        if (base::global::UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPathEntry_0x18 {
    UnkPooledId<&g_unk_7101d47200> field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};

struct UnkKind {
    u32 value;
    UnkKind(u32 v) : value(v) {}
    UnkKind(const UnkKind& o) : value(o.value) {}
};

inline const UnkMask& MaskNone() {
    static UnkMask s;
    return s;
}

inline const UnkMask& MaskAll() {
    static UnkMask s(~MaskNone().bits & 0x1ff);
    return s;
}

struct UnkRequest {
    u8 data[0xc];
};

struct UnkMgr_0xf18 {
    u8 data[0xf18];
};

extern UnkMgr_0xf18* g_unk_71726bc560;

void* fn_71000839a0(u64 size, u32 flags);
void fn_7100fd3804(UnkMgr_0xf18*);
void fn_7100fd7c3c(UnkMgr_0xf18*, u32, UnkRequest*, f32, UnkKind, UnkMask);

static inline UnkMgr_0xf18* GetMgr() {
    if (!g_unk_71726bc560) {
        UnkMgr_0xf18* m = static_cast<UnkMgr_0xf18*>(fn_71000839a0(0xf18, 0x3f));
        fn_7100fd3804(m);
        g_unk_71726bc560 = m;
    }
    return g_unk_71726bc560;
}

struct UnkStruct_0x150 {
    u8 pad_0x0[0x30];
    bool field_0x30;
    u8 pad_0x31[0x57];
    UnkRequest field_0x88;
    f32 field_0x94;
    u8 pad_0x98[0x18];
    base::UnkVector<UnkPathEntry_0x18> field_0xb0;
    u8 pad_0xd8[0x8];
    base::global::CStrId field_0xe0;
    UnkRequest field_0xe8;
    f32 field_0xf4;
    u8 pad_0xf8[0x18];
    base::UnkVector<UnkPathEntry_0x18> field_0x110;
    u8 pad_0x138[0x8];
    base::global::CStrId field_0x140;
    u32 field_0x148;
};

void fn_7100fd16ec(UnkStruct_0x150* self) {
    if (!self->field_0x30)
        return;
    self->field_0x30 = false;
    fn_7100fd7c3c(GetMgr(), 0, &self->field_0x88, self->field_0x94, UnkKind(2), MaskAll());
    fn_7100fd7c3c(GetMgr(), 1, &self->field_0xe8, self->field_0xf4, UnkKind(2), MaskAll());
    self->field_0xb0.Clear();
    self->field_0x110.Clear();
    self->field_0xe0 = g_unk_7101d470e0;
    self->field_0x140 = g_unk_7101d470e0;
    self->field_0x148 = 0x7fffffff;
}
