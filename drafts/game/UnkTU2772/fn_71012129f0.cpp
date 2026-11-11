#include "types.h"

struct UnkAtkLoader;
struct UnkAtkArchive;
u32 fn_71012069f8(UnkAtkArchive* archive, u32 id);
void* fn_7101211d30(UnkAtkLoader* self, u32 file_id, void* heap, void* arg, u32 unk);
bool fn_710121166c(UnkAtkLoader* self, u32 id, void* heap, u32 flags, void* arg);

struct UnkAtkLoaderB_b0149 {
    void* vtable;
    UnkAtkArchive* archive;  // 0x8
};

// fn_71012129f0
struct UnkAtkIdPair_b0149 {
    u32 first;
    u32 second;
};
struct UnkAtkIdList_b0149 {
    u32 count;
    UnkAtkIdPair_b0149 entries[1];  // 0x4
};
struct UnkAtkIdLookup_b0149 {
    u32 id;
    u32 pad_0x4;
    u8 flag;  // 0x8
};
void fn_7101215788(void* out, void* in);
UnkAtkIdList_b0149* fn_7101215a38(void* tmp);
bool fn_7101206ed4(UnkAtkArchive* archive, u32 id, UnkAtkIdLookup_b0149* out);
bool fn_7101211e48(UnkAtkLoader* self, u32 a, u32 b, void* heap, void* arg);

bool fn_71012129f0(UnkAtkLoaderB_b0149* self, void* in, void* heap) {
    if (!in)
        return false;
    UnkAtkIdLookup_b0149 lookup;
    u8 tmp[0x18];
    fn_7101215788(tmp, in);
    UnkAtkIdList_b0149* list = fn_7101215a38(tmp);
    bool ok = false;
    if (!list)
        goto done;
    for (u64 i = 0; i < list->count; ++i) {
        u32 a = list->entries[i].first;
        u32 b = list->entries[i].second;
        lookup.id = 0xffffffff;
        lookup.flag = 0;
        if (!fn_7101206ed4(self->archive, a, &lookup))
            goto done;
        if (lookup.flag) {
            if (!fn_7101211e48((UnkAtkLoader*)self, a, b, heap, nullptr))
                goto done;
        } else {
            if (!fn_7101211d30((UnkAtkLoader*)self, fn_71012069f8(self->archive, a), heap, nullptr, 1))
                goto done;
        }
    }
    ok = true;
done:
    return ok;
}

// fn_7101211c20
struct UnkAtkRange_b0149 {
    u32 lo;
    u32 hi;
    u64 pad;
};
bool fn_7101206f40(UnkAtkArchive* archive, u32 id, UnkAtkRange_b0149* out);
u32 fn_7101206a60(UnkAtkArchive* archive, u32 id);
bool fn_7101211880(UnkAtkLoader* self, u32 id, void* heap, u32 flags, void* arg, u32 parent);

bool fn_7101211c20(UnkAtkLoaderB_b0149* self, u32 a, void* heap, u32 flags, void* arg) {
    UnkAtkRange_b0149 range;
    range.lo = 0xffffffff;
    range.hi = 0xffffffff;
    range.pad = 0;
    if (!fn_7101206f40(self->archive, a, &range))
        return false;
    if (range.lo == 0xffffffff)
        goto second;
    switch (fn_7101206a60(self->archive, range.lo)) {
    case 1:
    case 3:
        break;
    default:
        goto second;
    }
    {
    u32 i = range.lo;
    if (i > range.hi)
        goto second;
    do {
        if (!fn_710121166c((UnkAtkLoader*)self, i, heap, flags, arg))
            return false;
        ++i;
    } while (i <= range.hi);
    return true;
}
second : {
    u32 i = range.lo;
    if (i > range.hi)
        return true;
    do {
        if (!fn_7101211880((UnkAtkLoader*)self, i, heap, flags, arg, a))
            return false;
        ++i;
    } while (i <= range.hi);
    return true;
}
}

// fn_710122cf1c
struct UnkAtkCur_b0149 {
    u8 pad_0x0[0x34];
    u32 state;  // 0x34
};
struct UnkAtkPlayer_b0149;
typedef void (*UnkAtkPlayerCallback_b0149)(UnkAtkPlayer_b0149*, u32, void*);
struct UnkAtkPlayer_b0149 {
    u8 pad_0x0[0x1c4];
    s32 count;  // 0x1c4
    UnkAtkPlayerCallback_b0149 callback;  // 0x1c8
    void* user;  // 0x1d0
    u8 active;  // 0x1d8
    u8 flags[2];  // 0x1d9
    u8 pad_0x1db[5];
    UnkAtkCur_b0149* cur;  // 0x1e0
};
struct UnkAtkRegistry_b0147;
UnkAtkRegistry_b0147* fn_710122edb0();
void fn_710122f2a0(UnkAtkRegistry_b0147* reg, UnkAtkPlayer_b0149* player);
bool fn_7101230254(void* track);
void fn_7101230314(void* track);

void fn_710122cf1c(UnkAtkPlayer_b0149* self) {
    if (!self->active || !self->flags[1] || self->count < 1)
        return;
    if (self->cur && self->cur->state - 1 > 1) {
        if (self->callback)
            self->callback(self, 0, self->user);
        goto clear;
    }
    for (s64 i = 0; i < self->count; ++i) {
        if (!fn_7101230254((u8*)self + i * 0xe0))
            goto stop;
    }
    return;
stop:
    for (s64 i = 0; i < self->count; ++i)
        fn_7101230314((u8*)self + i * 0xe0);
    self->count = 0;
    self->active = 0;
    fn_710122f2a0(fn_710122edb0(), self);
    if (self->callback)
        self->callback(self, 3, self->user);
clear:
    __builtin_memset(&self->flags[0], 0, 2);
}
