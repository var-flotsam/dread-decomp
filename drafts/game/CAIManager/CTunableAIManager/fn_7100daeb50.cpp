// CTunableAIManager's unit: for every entry of a refcounted list whose object lies near a position (within 700
// in x, 200 in y) and on the side of it that a direction points to (same sign in x), runs fn_71002334fc with
// the flag's low bit. Names and layouts are placeholders.
#include "game/UnkTU341.h"
#include "types.h"

struct UnkTarget_ldaeb50 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
};

struct UnkTargetHolder_ldaeb50 {
    u8 pad_0x0[0x18];
    UnkTarget_ldaeb50* target;  // 0x18
};

struct UnkEntry_ldaeb50 {
    virtual void vf_0x0();
    virtual ~UnkEntry_ldaeb50();
    s32 ref_count;  // 0x8
    u8 pad_0xc[4];
    UnkTargetHolder_ldaeb50* holder;  // 0x10
};

struct UnkEntryRef_ldaeb50 {
    UnkEntry_ldaeb50* ptr;

    UnkEntryRef_ldaeb50(const UnkEntryRef_ldaeb50& other) : ptr(other.ptr) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkEntryRef_ldaeb50() {
        if (UnkEntry_ldaeb50* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkEntryList_ldaeb50 {
    UnkEntryRef_ldaeb50* data;
    u32 count;
};

UnkEntryList_ldaeb50* fn_71007b5d90();

static inline f32 Sign(f32 v) {
    return (v > 0) - (v < 0);
}

void fn_7100daeb50(void* self, bool flag, const f32* pos, const f32* dir) {
    UnkEntryList_ldaeb50* list = fn_71007b5d90();
    for (UnkEntryRef_ldaeb50* it = list->data, *end = list->data + list->count; it != end; ++it) {
        if (!it->ptr)
            continue;
        UnkEntryRef_ldaeb50 entry = *it;
        UnkTargetHolder_ldaeb50* holder = entry.ptr->holder;
        if (!holder || !holder->target)
            continue;
        UnkTarget_ldaeb50* t = holder->target;
        f32 dx = t->x - pos[0];
        f32 ay = __builtin_fabsf(t->y - pos[1]);
        f32 ax = __builtin_fabsf(t->x - pos[0]);
        if (ay > 200.0f || ax > 700.0f)
            continue;
        if (Sign(dx) != Sign(dir[0]))
            continue;
        fn_71002334fc(reinterpret_cast<UnkStruct_71002334fc*>(entry.ptr), flag);
    }
}
