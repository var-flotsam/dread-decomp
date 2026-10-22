// Queries up to 64 objects around a point and switches each one (by its kind) to state 2 or 1.
// Names and layouts are placeholders.
#include "types.h"

struct UnkRefCounted {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    s32 ref_count;
};

struct UnkRef {
    UnkRefCounted* ptr = nullptr;

    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            ptr->Delete();
    }
};

struct UnkQueryObj;

struct UnkHit {
    UnkQueryObj* obj;
    UnkRef field_0x8;
    UnkRef field_0x10;
    s32 field_0x18 = -1;
    f32 field_0x1c[12] = {};
};

void fn_7100ca257c(void* where, void* filter, u32 flags, u32 max, UnkHit* hits, u32* count);
u32 fn_7100c9e960(UnkQueryObj* obj);
void fn_7100c9ea08(UnkQueryObj* obj);
void fn_7100c9e6ac(UnkQueryObj* obj, u32 state, bool a, bool b);

struct UnkSwitcher {
    virtual void vf_0x0();
    bool vf_0x300(u32 kind) {
        return reinterpret_cast<bool (*)(UnkSwitcher*, u32)>((*reinterpret_cast<void***>(this))[0x300 / 8])(this,
                                                                                                         kind);
    }
};

struct UnkFlags_7100aaac80 {
    u8 unk_0x0[0xdc];
    bool field_0xdc;
};
struct UnkGameManager {
    u8 unk_0x0[0x1af8];
    UnkFlags_7100aaac80* field_0x1af8;
};
extern UnkGameManager* g_unk_7101d47310;

void fn_7100aaac80(UnkSwitcher* self, void* where, void* filter) {
    if (g_unk_7101d47310->field_0x1af8->field_0xdc)
        return;
    u32 count = 0;
    UnkHit hits[64];
    fn_7100ca257c(where, filter, 0, 64, hits, &count);
    for (u32 i = 0; i < count; ++i) {
        UnkQueryObj* obj = hits[i].obj;
        if (!obj)
            continue;
        u32 kind = fn_7100c9e960(obj);
        if (kind == 8)
            continue;
        if (self->vf_0x300(kind)) {
            fn_7100c9ea08(obj);
            fn_7100c9e6ac(obj, 2, false, false);
        } else {
            fn_7100c9e6ac(obj, 1, false, false);
        }
    }
}
