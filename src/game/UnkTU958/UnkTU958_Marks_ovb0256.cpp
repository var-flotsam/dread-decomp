// Unit 958 of tools/tu_map.py (CPoisonFlyAIComponent's neighbourhood, no name hint): a list of 0x30-byte
// marks (a position and a flag, holding a weak handle to the object they belong to) with its growth,
// the mark's construction and re-targeting, and the reset that clears the owner counters. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU958.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkMark_b256 {
    u32 field_0x8, field_0xc, field_0x10;
    f32 x, y, z;
    UnkWeakHandle* handle;  // 0x20
    u8 flag;                // 0x28

    UnkMark_b256(const f32* v, bool f) : field_0x8(0), field_0xc(0), field_0x10(0), x(v[0]), y(v[1]), z(v[2]), handle(nullptr), flag(f) {}
    UnkMark_b256(const UnkMark_b256& o)
        : field_0x8(o.field_0x8), field_0xc(o.field_0xc), field_0x10(o.field_0x10), x(o.x), y(o.y), z(o.z), handle(nullptr) {
        AssignRef(&handle, o.handle);
        __builtin_memcpy(&flag, &o.flag, 1);
    }
    virtual ~UnkMark_b256() {
        if (handle && --handle->ref_count == 0)
            handle->vf_0x10();
    }
};

struct UnkMarkArray_b256 {
    UnkMark_b256* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710063cb28(UnkMarkArray_b256* self, u32 n) {
    if (self->capacity < n) {
        UnkMark_b256* p = static_cast<UnkMark_b256*>(self->allocator->vf_0x40((u64)n * sizeof(UnkMark_b256), 8, self->flags));
        UnkMark_b256* old = self->data;
        if (old) {
            UnkMark_b256* src = old;
            UnkMark_b256* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                new (dst) UnkMark_b256(*src);
                src->~UnkMark_b256();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}

void fn_710063cc8c(UnkMarkArray_b256* self, const UnkMark_b256* value) {
    u32 n = self->count;
    u32 cap = self->capacity;
    u32 nc = cap;
    if (cap <= n && self->step != 0)
        nc = (n / self->step + 1) * self->step;
    if (__builtin_expect((s32)n >= (s32)nc, 0))
        for (;;) {
        }
    fn_710063cb28(self, nc);
    UnkMark_b256* data = self->data;
    u64 index = self->count;
    new (&data[index]) UnkMark_b256(*value);
    ++self->count;
}

void fn_710063c934(UnkMark_b256* self, const f32* v, UnkWeakHandle** owner, u32 flag) {
    new (self) UnkMark_b256(v, flag & 1);
    if (UnkWeakHandle* o = *owner)
        if (UnkWeakTarget* t = o->target)
            AssignRef(&self->handle, GetWeakHandle(t));
}

void fn_710063bf8c(void* unused, UnkWeakTarget* target, UnkMark_b256* mark) {
    ResetRef(&mark->handle);
    AssignRef(&mark->handle, GetWeakHandle(target));
}



extern UnkTunable g_unk_71726fa0f8;

struct UnkFlyComp_b256 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x14768];
    u32 count;  // 0x14770
};

struct UnkFlyOwner_b256 : UnkWeakTarget {
    u8 pad_0x8[0x238];
    UnkFlyComp_b256* comp;  // 0x240
};

struct UnkFlyHandleNode_b256 {
    UnkWeakHandle* handle;
    UnkFlyHandleNode_b256* next;
};

struct UnkFlyMgr_b256 {
    u8 pad_0x0[0x40];
    UnkFlyHandleNode_b256* head;  // 0x40
    u8 pad_0x48[0x60];
    UnkWeakHandle* handle;  // 0xa8
    u8 pad_0xb0[4];
    u32 field_0xb4;
};

static inline UnkFlyComp_b256* AsPoisonFly(UnkFlyComp_b256* comp) {
    return fn_710009d50c(comp->vf_0x0(), fn_7100638be0()) ? comp : nullptr;
}

void fn_710063c670(UnkFlyMgr_b256* self) {
    self->field_0xb4 = *reinterpret_cast<const u32*>(reinterpret_cast<const char*>(&g_unk_71726fa0f8) + 0x4c);
    if (!self->handle)
        return;
    UnkFlyOwner_b256* owner = static_cast<UnkFlyOwner_b256*>(self->handle->target);
    UnkFlyComp_b256* comp = owner ? owner->comp : nullptr;
    if (comp) {
        if (UnkFlyComp_b256* fly = AsPoisonFly(comp)) {
            fly->count = 0;
            for (UnkFlyHandleNode_b256* n = self->head; n; n = n->next) {
                UnkFlyOwner_b256* other = n->handle ? static_cast<UnkFlyOwner_b256*>(n->handle->target) : nullptr;
                UnkWeakTarget* mine = self->handle ? self->handle->target : nullptr;
                if (other == mine)
                    continue;
                UnkFlyComp_b256* c = AsPoisonFly(other->comp);
                ++c->count;
            }
        }
    }
    ResetRef(&self->handle);
}

