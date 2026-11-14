// Unit 1442 of tools/tu_map.py (no name hint): a state-machine class built on the base of unit 1440
// (fn_7100969000; cf. src/game/UnkUnit1442/UnkUnit1442_Refresh_b50.cpp and unit 1444). Its destructor, the
// checks and steps around the 0x100-byte object at +0xb48 and the owner's navigation, and the transform
// copy of a turn. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityShinespark.h"
#include "game/CEnemyMovement.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkTU1130.h"
#include "game/UnkTU1440.h"
#include "game/UnkTU1442.h"
#include "game/UnkTU376.h"
#include "game/UnkTU55.h"
#include "game/malloc.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "base/math/CVector.h"
#include "game/UnkAIComponent.h"
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkAnimationComponent.h"
#include "base/reflection/UnkClassGetters.h"

// ---- a110 ----
extern void* g_unk_7101a85218[];

struct UnkUnit1444Base;

struct UnkLink_1442 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    UnkWeakTarget* target;  // 0x10
};

struct UnkListener_1442 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLink_1442* link;  // 0x18
};

struct UnkUnit1442Dtor {
    void** vtable;
    u8 pad_0x8[0x138];
    u32 count;  // 0x140
    u8 elements[0xa04];  // 0x144, stride 0x100
    UnkObject_0x100 object;  // 0xb48
    u8 pad_0xc48[8];
    UnkListener_1442 listener;  // 0xc50
};
static_assert(__builtin_offsetof(UnkUnit1442Dtor, listener) == 0xc50, "UnkUnit1442 layout");

void fn_710096a110(void* raw) {
    UnkUnit1442Dtor* self = static_cast<UnkUnit1442Dtor*>(raw);
    self->vtable = &g_unk_7101a85218[2];
    self->listener.vtable = &g_unk_7101a22d48[2];
    if (UnkLink_1442* link = self->listener.link) {
        if (link->target) {
            fn_710117b3f0(link->target, &self->listener);
            link = self->listener.link;
        }
        if (link && --link->ref_count == 0)
            link->Release();
    }
    fn_71002b6094(&self->object);
    for (u64 i = 0; i < self->count; ++i)
        fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(self->elements + i * 0x100));
    self->count = 0;
    fn_7100969058(reinterpret_cast<UnkUnit1444Base*>(self));
}

// ---- ccb4 ----
struct UnkRecord_1442 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkRecordHead_1442 {
    u8 pad_0x0[0x10];
    UnkRecord_1442* record;  // 0x10
};

struct UnkRecordTable_1442 {
    UnkRecordHead_1442* head;  // 0x0
    char* kind1;  // 0x8
};

struct UnkNav_1442 {
    u8 pad_0x0[0xb0];
    UnkRecordTable_1442* table;  // 0xb0
};

struct UnkUnit1442Near {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x54];
    f32 x;  // 0x8c
    f32 y;  // 0x90
    u8 pad_0x94[0xab4];
    u8 object[0x100];  // 0xb48
    u8 pad_0xc48[0x10];
    u16 pad_0xc58;
};

static inline bool Accepts(UnkAIComponent* ai, void* object) {
    typedef bool (*Fn)(void*, void*);
    return (*reinterpret_cast<Fn**>(ai))[0x5b0 / 8](ai, object);
}

bool fn_710096ccb4(char* raw) {
    UnkUnit1442Near* self = reinterpret_cast<UnkUnit1442Near*>(raw);
    UnkAIComponent* ai = static_cast<UnkAIComponent*>(self->owner->field_0x240);
    if (ai && !Accepts(ai, self->object))
        return false;
    if (*reinterpret_cast<s16*>(self->object + 2) == -1)
        return false;
    if (*reinterpret_cast<u16*>(self->object + 0x1e) == 0xffff)
        return false;
    if (!ai)
        return true;
    UnkNav_1442* nav = reinterpret_cast<UnkNav_1442*>(fn_71010fccf8(ai));
    if (!nav)
        return true;
    UnkRecordTable_1442* table = nav->table;
    if (!table->head || !table->head->record || table->head->record->kind != 1 || !table->kind1)
        return true;
    const f32* p = reinterpret_cast<const f32*>(table->kind1 + 0x3810c);
    base::math::CVector2D out;
    base::math::CVector3D v;
    v.x = p[0];
    v.y = p[1];
    v.z = p[2];
    fn_710007b430(&out, &v);
    f32 dx = out.x - self->x;
    f32 dy = out.y - self->y;
    return dx * dx + dy * dy > 100.0f;
}

// ---- b8fc ----
struct UnkEffectSink;

bool fn_710096ccb4(char* self);

struct UnkEvent_1442 {
    void** vtable;
    u8 pad_0x8[4];
    bool field_0xc;
    u8 pad_0xd[7];
    bool field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 mode;  // 0x38
    u8 pad_0x3c[4];
    alignas(8) u8 object[0x100];  // 0x40
    bool field_0x140;
    u8 pad_0x141;
    bool field_0x142;
    bool field_0x143;
    bool field_0x144;

    UnkEvent_1442() {
        field_0xc = false;
        field_0x14 = false;
        mode = 0;
        __builtin_memset(field_0x18, 0, 13);
        __builtin_memset(field_0x28, 0, 13);
        vtable = g_unk_7101a6bff8 + 2;
        fn_71002b57bc(Object());
    }
    ~UnkEvent_1442() {
        vtable = g_unk_7101a6bff8 + 2;
        fn_71002b6094(Object());
    }
    UnkObject_0x100* Object() { return reinterpret_cast<UnkObject_0x100*>(object); }
};

struct UnkUnit1442Check {
    u8 pad_0x0[0x38];
    UnkEffectSink* sink;  // 0x38
    u8 pad_0x40[0xb08];
    UnkObject_0x100 object;  // 0xb48
};

struct UnkTemp_1442 {
    alignas(8) u8 raw[0x100];
    UnkTemp_1442() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(raw)); }
    ~UnkTemp_1442() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(raw)); }
};

void fn_710096b8fc(char* raw) {
    UnkUnit1442Check* self = reinterpret_cast<UnkUnit1442Check*>(raw);
    if (!fn_710096ccb4(raw))
        return;
    UnkEvent_1442 ev;
    ev.field_0x140 = false;
    ev.field_0x142 = false;
    ev.field_0x144 = false;
    ev.mode = 4;
    fn_71002b5c90(ev.Object(), &self->object);
    ev.field_0x140 = true;
    ev.field_0x143 = true;
    ev.field_0x144 = true;
    fn_71010c7e6c(self->sink, &ev, 1);
    UnkTemp_1442 tmp;
    fn_71002b5c90(&self->object, reinterpret_cast<UnkObject_0x100*>(tmp.raw));
}

// ---- aadc ----
struct CVector3D;
struct UnkCharacterMovement;
struct UnkCollisionHit;
struct UnkEntityCollided;
struct UnkRecord_aadc;

bool fn_7100977e50(UnkCharacterMovement* self, f32 max_dot, f32 extra, f32 step, f32 back, u32 mask,
                   const CVector3D* pos, CVector3D* out_point, UnkCollisionHit* out_hit, const CVector3D* dir);
UnkVec3f fn_7100d7c7a0(const UnkEntityCollided* self);

struct UnkRef_aadc {
    UnkRefObject* ptr;

    UnkRef_aadc() : ptr(nullptr) {}
    ~UnkRef_aadc() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHit_aadc {
    UnkRef_aadc object;
    UnkRef_aadc field_0x8;
    s32 id;
    f32 values[12];

    UnkHit_aadc() : id(-1), values() {}
};

struct UnkMovement_aadc {
    void** vtable;
    u8 pad_0x8[0x120];
    u8 record[0x48];  // 0x128
    u8 pad_0x170[0x260];
    u32 mask;  // 0x3d0

    UnkCharacterMovement* vf_0x2a8() {
        return reinterpret_cast<UnkCharacterMovement* (*)(UnkMovement_aadc*)>(vtable[0x2a8 / 8])(this);
    }
    void vf_0x218(UnkHit_aadc* hit) {
        reinterpret_cast<void (*)(UnkMovement_aadc*, UnkHit_aadc*)>(vtable[0x218 / 8])(this, hit);
    }
};

struct UnkContext_aadc {
    u8 pad_0x0[0x30];
    UnkEntityCollided* entity;  // 0x30
    UnkMovement_aadc* movement;  // 0x38
};

struct UnkCapture_aadc {
    UnkContext_aadc* context;
    u8* found;
};

void fn_710096aadc(UnkRecord_aadc* out, UnkCapture_aadc* capture) {
    UnkContext_aadc* context = capture->context;
    if (!*capture->found) {
        UnkHit_aadc hit;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(reinterpret_cast<u8*>(context->entity) + 0x38));
        UnkVec3f dir(-m.m[1], -m.m[5], -m.m[9]);
        UnkVec3f pos = fn_7100d7c7a0(context->entity);
        if (context->movement) {
            UnkCharacterMovement* probe = context->movement->vf_0x2a8();
            if (fn_7100977e50(probe, 1.0f, 50.0f, 3.4028235e38f, 0.0f, context->movement->mask,
                              reinterpret_cast<CVector3D*>(&pos), nullptr, reinterpret_cast<UnkCollisionHit*>(&hit),
                              reinterpret_cast<CVector3D*>(&dir))) {
                context->movement->vf_0x218(&hit);
                *capture->found = 1;
            }
        }
    }
    fn_7100331c7c(out, reinterpret_cast<const UnkRecord_aadc*>(context->movement->record));
}

// ---- eee8 ----
struct UnkVec3;
void fn_710096dfb8(char* self, u32 flag);

struct UnkAI_eee8 {
    void** vtable;
    u8 pad_0x8[0x68];
    u32 count;  // 0x70
    u8 pad_0x74[0x20];
    u8 field_0x94;
};

struct UnkOwner_eee8 {
    u8 pad_0x0[0x240];
    UnkAI_eee8* ai;  // 0x240
};

struct UnkUnit1442Step {
    u8 pad_0x0[0x30];
    UnkOwner_eee8* owner;  // 0x30
    u8 pad_0x38[0x51];
    u8 reversed;  // 0x89
    u8 pad_0x8a[0x12];
    u32 index;  // 0x9c
    u8 pad_0xa0[8];
    u8 swapping;  // 0xa8
    u8 pad_0xa9[7];
    u32 field_0xb0;
    u32 field_0xb4;
    u8 pad_0xb8[0x14];
    u8 segment_a[0x14];  // 0xcc
    u8 segment_b[0x14];  // 0xe0
    u8 pad_0xf4[0x10];
    u32 field_0x104;
    u32 field_0x108;
    u32 field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x14];
    u32 field_0x128;
    u32 field_0x12c;
};

void fn_710096eee8(UnkUnit1442Step* self) {
    self->reversed ^= 1;
    if (self->swapping) {
        UnkSegment2D saved;
        const f32* a = reinterpret_cast<const f32*>(self->segment_a);
        saved.start.x = a[0];
        saved.start.y = a[1];
        saved.end.x = a[2];
        saved.end.y = a[3];
        saved.field_0x10 = -1.0f;
        fn_7100125b44(reinterpret_cast<UnkSegment2D*>(self->segment_a), reinterpret_cast<UnkSegment2D*>(self->segment_b));
        fn_7100125b44(reinterpret_cast<UnkSegment2D*>(self->segment_b), &saved);
        UnkAI_eee8* ai = self->owner->ai;
        u32 index = self->index;
        if (self->reversed) {
            if (index == 0) {
                u32 count = ai->count;
                bool open = ai->field_0x94 == 0;
                index = count - ((count != 0) & open);
            }
            index -= 1;
        } else {
            u32 count = ai->count;
            bool open = ai->field_0x94 == 0;
            index = count + ~(u32)((count != 0) & open) == index ? 0 : index + 1;
        }
        self->index = index;
        u32 t = self->field_0xb0;
        self->field_0xb0 = self->field_0xb4;
        self->field_0xb4 = t;
        alignas(8) u32 v[3];
        v[0] = self->field_0x104;
        v[1] = self->field_0x108;
        v[2] = 0;
        self->field_0x104 = self->field_0x10c;
        self->field_0x108 = self->field_0x110;
        fn_710007b444(&self->field_0x10c, reinterpret_cast<const UnkVec3*>(v));
        self->field_0x12c = self->field_0x128;
    } else {
        u32 t = self->field_0x128;
        self->field_0x128 = self->field_0x12c;
        self->field_0x12c = t;
        fn_710096dfb8(reinterpret_cast<char*>(self), 0);
    }
    fn_710096cdec(reinterpret_cast<char*>(self), false);
    if (UnkAI_eee8* ai = self->owner->ai)
        reinterpret_cast<void (*)(UnkAI_eee8*)>(ai->vtable[0x5d0 / 8])(ai);
}

// ---- b720 ----
bool fn_71010ca5d8(void* target);
struct UnkEnemyMovement_f;
bool fn_71010caa64(UnkEnemyMovement_f* self);

struct UnkTarget_b720 {
    void** vtable;
    u8 pad_0x8[0x27c];
    UnkXform rotation_source;  // 0x284
    u8 pad_0x2e0[0x58];
    UnkXform rotation_target;  // 0x338

    bool vf_0x340(bool flag) { return reinterpret_cast<bool (*)(UnkTarget_b720*, bool)>(vtable[0x340 / 8])(this, flag); }
};
static_assert(__builtin_offsetof(UnkTarget_b720, rotation_target) == 0x338, "UnkTarget_b720 layout");

struct UnkOwner_b720 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkTemp_b720 {
    alignas(8) u8 raw[0x100];
    UnkTemp_b720() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(raw)); }
    ~UnkTemp_b720() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(raw)); }
    UnkObject_0x100* get() { return reinterpret_cast<UnkObject_0x100*>(raw); }
};

struct UnkUnit1442Turn {
    u8 pad_0x0[0x30];
    UnkOwner_b720* owner;  // 0x30
    UnkTarget_b720* target;  // 0x38
    u8 pad_0x40[0xb08];
    UnkObject_0x100 object;  // 0xb48
};

void fn_710096b720(UnkUnit1442Turn* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!fn_71010ca5d8(self->target))
        return;
    if (fn_71010caa64(reinterpret_cast<UnkEnemyMovement_f*>(self->target))) {
        if (!fn_710117b348(animation, fn_7100bf6df8()))
            return;
    }
    if (fn_710117b348(animation, fn_7100bfc588()))
        return;
    fn_710096cdec(reinterpret_cast<char*>(self), true);
    u32 out;
    u64 a = 0;
    UnkTemp_b720 found;
    fn_710096d788(reinterpret_cast<char*>(self), &a, reinterpret_cast<char*>(found.raw), &out, false);
    u8* raw = found.raw;
    if (*reinterpret_cast<s16*>(raw + 2) != -1 && *reinterpret_cast<s16*>(raw + 0x1e) != -1) {
        *(reinterpret_cast<u8*>(self) + 0x13d) = 0;
        fn_71002b5c90(&self->object, found.get());
        if (fn_710096ccb4(reinterpret_cast<char*>(self)))
            return;
    }
    if (!self->target->vf_0x340(false)) {
        fn_710096cdec(reinterpret_cast<char*>(self), false);
        return;
    }
    UnkTarget_b720* target = self->target;
    UnkVec3f angle = target->rotation_source.GetAngle();
    target->rotation_target.SetAngle(angle);
    UnkTemp_b720 empty;
    fn_71002b5c90(&self->object, empty.get());
}
