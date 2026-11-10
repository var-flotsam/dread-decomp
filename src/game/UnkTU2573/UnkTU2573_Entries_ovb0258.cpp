// Unit 2573 (no reliable name): a growing array of 0x18-byte records (a counted reference and two ids), and the
// damage request built from an attack. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRefEntry_b0258b {
    base::UnkRefHandle handle;
    u32 value;
};

void fn_7100ecf4c8(base::UnkVector<UnkRefEntry_b0258b>* self, u32 n) {
    if (self->capacity < n) {
        UnkRefEntry_b0258b* p = static_cast<UnkRefEntry_b0258b*>(self->allocator->vf_0x40((u64)n * sizeof(UnkRefEntry_b0258b), 8, self->flags));
        UnkRefEntry_b0258b* src = self->data;
        if (src) {
            UnkRefEntry_b0258b* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst) {
                UnkRefEntry_b0258b* old = src++;
                dst->handle.ptr = nullptr;
                dst->handle.Set(old->handle.ptr);
                dst->value = old->value;
                if (base::UnkRefCounted* r = old->handle.ptr) {
                    if (--r->ref_count == 0)
                        delete r;
                }
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}

struct UnkAttackLink_b0258e {
    u8 pad_0x0[0x10];
    UnkWeakTarget* target;  // 0x10
};

struct UnkAttackData_b0258e {
    u8 pad_0x0[0x10];
    f32 field_0x10;
    f32 scale;  // 0x14
    u32 field_0x18;
    u32 field_0x1c;
    u32 flags;  // 0x20
    u8 pad_0x24[0x4];
    UnkAttackLink_b0258e* field_0x28;
    UnkAttackLink_b0258e* field_0x30;
    u32 words[12];  // 0x38
    u8 pad_0x68[0x8];
    UnkAttackLink_b0258e* field_0x70;
};
static_assert(__builtin_offsetof(UnkAttackData_b0258e, field_0x70) == 0x70, "layout");

struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
void fn_71008c0c74(UnkStruct_71008c0c74* self);
void fn_71008c1558(UnkBase_71003cc474* self);

struct UnkDamageReq_b0258e {
    u8 pad_0x0[0x8];
    union {
        UnkWeakHandle* source;  // 0x8
    };
    union {
        UnkWeakHandle* target;  // 0x10
    };
    u8 pad_0x18[0x48];
    UnkWeakTarget* field_0x60;
    u32 words[12];  // 0x68
    UnkWeakTarget* field_0x98;
    u8 pad_0xa0[0x14];
    u32 field_0xb4;
    u8 pad_0xb8[0x8];
    u32 field_0xc0;
    u32 field_0xc4;
    u8 pad_0xc8[0x10];
    u32 pos[3];  // 0xd8
    u32 dir[3];  // 0xe4
    f32 value;   // 0xf0
    u8 pad_0xf4;
    u8 field_0xf5;
    bool field_0xf6;
    u8 pad_0xf7[9];

    UnkDamageReq_b0258e() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkDamageReq_b0258e() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(__builtin_offsetof(UnkDamageReq_b0258e, field_0xf6) == 0xf6, "layout");

struct UnkCharClassObj_b0258e {
    virtual const base::reflection::CClass* vf_0x0() const;
};

struct UnkDamageActor_b0258e {
    u8 pad_0x0[0x28];
    UnkCharClassObj_b0258e* owner;  // 0x28
};

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);
f32 fn_7100305c24(void* stats);
UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

__attribute__((noinline)) void fn_7100ecdcb0(UnkAttackData_b0258e* a, UnkDamageReq_b0258e* req, UnkDamageActor_b0258e* actor, const u32* pos,
                   const u32* dir, f32 value) {
    f32 scale = a->scale;
    u32 flags = a->flags;
    if (actor && (flags & 0x80)) {
        UnkCharClassObj_b0258e* owner = actor->owner;
        char* cls = nullptr;
        if (owner) {
            if (fn_710009d50c(owner->vf_0x0(), fn_71011abbd4()))
                cls = reinterpret_cast<char*>(owner);
        }
        scale *= fn_7100305c24(cls + 0x98);
    } else if (flags & 0x8000400) {
        scale *= a->field_0x10;
    }
    fn_71008c1538(reinterpret_cast<UnkRequest*>(req), scale);
    req->field_0xc0 = a->field_0x18;
    req->field_0xc4 = a->field_0x1c;
    req->field_0xb4 = a->flags;
    for (u32 i = 0; i < 12; ++i)
        req->words[i] = a->words[i];
    req->field_0x60 = a->field_0x28 ? a->field_0x28->target : nullptr;
    req->field_0x98 = a->field_0x30 ? a->field_0x30->target : nullptr;
    AssignRef(&req->source, fn_7100d78378(a->field_0x70 ? a->field_0x70->target : nullptr));
    AssignRef(&req->target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(actor)));
    req->pos[0] = pos[0];
    req->pos[1] = pos[1];
    req->pos[2] = pos[2];
    req->dir[0] = dir[0];
    req->dir[1] = dir[1];
    req->dir[2] = dir[2];
    req->value = value;
    req->field_0xf6 = true;
}

struct UnkLifeComp_b0258e {
    virtual const base::reflection::CClass* vf_0x0() const;
    u8 pad_0x8[0x68];
    s32 field_0x70;
    u8 pad_0x74[0x1cc];
    u32 field_0x240;
    s32 CallHit(UnkDamageReq_b0258e* req) {
        return reinterpret_cast<s32 (*)(UnkLifeComp_b0258e*, UnkDamageReq_b0258e*)>(
            (*reinterpret_cast<void***>(this))[0x188 / 8])(this, req);
    }
};

struct UnkHitActor_b0258e {
    u8 pad_0x0[0x1b5];
    u8 field_0x1b5;
    u8 pad_0x1b6[2];
    UnkLifeComp_b0258e* life;  // 0x1b8
    u8 pad_0x1c0[0x30];
    UnkCharClassObj_b0258e* movement;  // 0x1f0
};

s32 fn_7100ecdf00(UnkAttackData_b0258e* a, UnkHitActor_b0258e* actor, void*, const u32* pos, const u32* dir, u8* out) {
    if (!actor)
        return 0;
    UnkLifeComp_b0258e* life = actor->life;
    if (!life)
        return 0;
    if (life->field_0x70 == 2)
        return 0;
    if (actor->field_0x1b5)
        return 0;
    if (fn_710009d50c(life->vf_0x0(), fn_71008dff9c()) && (a->flags & life->field_0x240) == 0) {
        if (life->field_0x240 & 0xe)
            return 0;
        UnkCharClassObj_b0258e* movement = actor->movement;
        if (!movement)
            return 0;
        if (__builtin_expect(fn_710009d50c(movement->vf_0x0(), fn_7100ac2434()), 1))
            return 0xb;
        return 0;
    }
    UnkDamageReq_b0258e req;
    s32 result;
    if (fn_710009d50c(life->vf_0x0(), fn_710072b634())) {
        f32 value;
        if (*reinterpret_cast<s32*>(reinterpret_cast<char*>(a) + 8) == 4)
            value = 5.0f;
        else
            value = *reinterpret_cast<s32*>(reinterpret_cast<char*>(a) + 0xc) == 1 ? 30.0f : 10.0f;
        fn_7100ecdcb0(a, &req, reinterpret_cast<UnkDamageActor_b0258e*>(actor), pos, dir, value);
        result = life->CallHit(&req);
    } else {
        const u32* zero = reinterpret_cast<const u32*>(&g_unk_710164779c);
        fn_7100ecdcb0(a, &req, reinterpret_cast<UnkDamageActor_b0258e*>(actor), zero, zero, 0.0f);
        result = life->CallHit(&req);
        if (!req.field_0xf5)
            *out = 0;
    }
    a->words[0] = req.words[0];
    a->words[1] = req.words[1];
    a->words[2] = req.words[2];
    a->words[3] = req.words[3];
    a->words[4] = req.words[4];
    a->words[5] = req.words[5];
    a->words[6] = req.words[6];
    a->words[7] = req.words[7];
    a->words[8] = req.words[8];
    a->words[9] = req.words[9];
    a->words[10] = req.words[10];
    a->words[11] = req.words[11];
    return result;
}
