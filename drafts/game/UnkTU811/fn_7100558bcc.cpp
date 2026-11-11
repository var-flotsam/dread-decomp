#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

#include <new>

struct UnkPlayer;
struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;

UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

struct UnkLavaDropsShape_A2_b71;
void fn_7100d6169c(UnkLavaDropsShape_A2_b71* self, const void* source, f32 size);
void fn_7100d61750(UnkLavaDropsShape_A2_b71* self);
bool fn_7100d57ca0(UnkLavaDropsShape_A2_b71* self, void* body, void* other, void* out, u32 flags);
void fn_710056a010(void* set, const void* value);

struct UnkLavaDropsShape_A2_b71 {
    u8 pad_0x0[0x8];
    u8 body[0xa8];

    UnkLavaDropsShape_A2_b71(const void* source, f32 size) { fn_7100d6169c(this, source, size); }
    ~UnkLavaDropsShape_A2_b71() { fn_7100d61750(this); }
};

struct UnkFarSettings_b0194 {
    u8 pad_0x0[0x184];
    s32 mode;  // 0x184
    f32 radius;  // 0x188
};

struct UnkFarSource_b0194 {
    void** vtable;
    UnkFarSettings_b0194* vf_0xd8() {
        return reinterpret_cast<UnkFarSettings_b0194* (*)(UnkFarSource_b0194*)>(vtable[0xd8 / 8])(this);
    }
};

struct UnkFarEntity_b0194 {
    u8 pad_0x0[0x418];
    bool field_0x418;
};

struct UnkFarBlock_b0194 {
    f32 range0;  // 0x0
    u8 pad_0x4[0x70 - 0x4];
    f32 range1;  // 0x70
    u8 pad_0x74[0xe0 - 0x74];
    f32 range2;  // 0xe0
    u8 pad_0xe4[0x150 - 0xe4];
    f32 range3;  // 0x150
    u8 pad_0x154[0x1e0 - 0x154];
    f32 stamp;  // 0x1e0
};

struct UnkFarAI_b0194 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x149b0 - 0x38];
    UnkFarBlock_b0194 block;
    u8 pad_0x14b94[0x29ee0 - 0x149b0 - sizeof(UnkFarBlock_b0194)];
    UnkFarSource_b0194* source;  // 0x29ee0
    u8 pad_0x29ee8[0xb8 - 0x8];
    u32 field_0x29f98;
};

bool fn_71005436e8(UnkFarAI_b0194* self);
bool fn_710054ad2c(UnkFarAI_b0194* self);
bool fn_710054aecc(UnkFarAI_b0194* self);
bool fn_7100543b30(UnkFarAI_b0194* self);
void fn_7100546a30(UnkFarAI_b0194* self, bool flag, UnkPlayer* player, const UnkVec3f* position);
UnkVec3f fn_71008db1d8(UnkFarEntity_b0194* entity);

struct UnkPos2_b0194 {
    f32 x, y;
};
static inline UnkPos2_b0194 PosOf2(const UnkActor* actor) {
    UnkPos2_b0194 p;
    p.x = actor->field_0x44;
    p.y = actor->field_0x54;
    return p;
}
static inline UnkPos2_b0194 operator-(const UnkPos2_b0194& a, const UnkPos2_b0194& b) {
    UnkPos2_b0194 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

struct UnkPos3_b0194 {
    f32 x, y, z;
};
static inline UnkPos3_b0194 PosOf(const UnkActor* actor) {
    UnkPos3_b0194 p;
    p.x = actor->field_0x44;
    p.y = actor->field_0x54;
    p.z = actor->field_0x64;
    return p;
}
static inline UnkPos3_b0194 operator-(const UnkPos3_b0194& a, const UnkPos3_b0194& b) {
    UnkPos3_b0194 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

void fn_7100558bcc(UnkFarAI_b0194* self, UnkFarEntity_b0194* entity) {
    u32 stage = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x14b48);
    if (stage != 0) {
        if (stage != 1)
            return;
        f32 now = *reinterpret_cast<f32*>(reinterpret_cast<u8*>(g_unk_7101d47328) + 0x2c);
        if (!(now - self->block.stamp >= 2.0f))
            return;
    }
    s32 mode = self->source->vf_0xd8()->mode;
    if (mode != 2) {
        if (mode != 1)
            return;
        if (!entity->field_0x418)
            return;
    }
    UnkVec3f target_pos = fn_71008db1d8(entity);
    UnkPos2_b0194 o = PosOf2(self->owner);
    UnkPos2_b0194 t = {target_pos.x, target_pos.y};
    UnkPos2_b0194 d = t - o;
    f32 distance = d.x * d.x + d.y * d.y;
    f32 dz = target_pos.z - self->owner->field_0x64;
    distance += dz * dz;
    f32 radius = self->source->vf_0xd8()->radius;
    if (distance < radius * self->source->vf_0xd8()->radius) {
        UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
        fn_7100d7c990(self->owner);
        UnkActor* actor = fn_7100d7c990(self->owner);
        void* body = nullptr;
        if (actor) {
            void* cache = *reinterpret_cast<void**>(reinterpret_cast<u8*>(actor) + 0x200);
            if (cache)
                body = fn_71010d7a6c(static_cast<UnkPlayerMovementCache*>(cache));
        }
        // The shape's storage is declared before the position it is built from, so it sits above it.
        union ShapeStorage {
            UnkLavaDropsShape_A2_b71 shape;
            ShapeStorage() {}
            ~ShapeStorage() {}
        } storage;
        UnkVec3f position(self->owner->field_0x44, self->owner->field_0x54, self->owner->field_0x64);
        f32 size;
        if (fn_710054ad2c(self))
            size = self->block.range0;
        else if (fn_71005436e8(self))
            size = self->block.range3;
        else if (fn_710054aecc(self))
            size = self->block.range1;
        else if (fn_7100543b30(self))
            size = self->block.range2;
        else
            size = 0.0f;
        new (&storage.shape) UnkLavaDropsShape_A2_b71(&position, size);
        UnkLavaDropsShape_A2_b71& shape = storage.shape;
        if (!fn_7100d57ca0(&shape, shape.body, body, nullptr, 0) && self->field_0x29f98)
            fn_710056a010(reinterpret_cast<u8*>(self) + 0x29f90, &target_pos);
        else
            fn_7100546a30(self, true, player, &target_pos);
        shape.~UnkLavaDropsShape_A2_b71();
    }
}
