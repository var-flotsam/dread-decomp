// behaviortree::CChaseTargetAction's unit (tools/tu_map.py unit 471): the action's start (fn_710037a4bc) and
// the chase size it hands the owner's AI (fn_710037a680). The start runs the base start, puts the AI in
// state 2 (+0x14518, releasing it through fn_71010fa828 when +0x5a asks), clears the action's vectors,
// refreshes them (fn_710037b5c4, and fn_710037b7b4/fn_710037bb64 or the target's position), sets the size,
// drops the two handles at +0x78/+0x80 and resets +0xb4. The size is the AI's source size (its vf_0x90),
// possibly overridden by the filter (vf_0xb8) when that gives a positive one, at least the minimum at
// +0xcc and replaced by the AI's vf_0x698 while +0x78 has a target; it is stored at +0x68 and in the AI
// at +0x14690. Names and layouts are placeholders.
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);
struct UnkAIComponent;

struct UnkChaseSizeSource_ovb0132 {
    virtual void vf_0x0();
    u8 pad_0x8[0x11c];
    u32 field_0x124;
};

struct UnkChaseSizeQuery_ovb0132 {
    void** vtable;
    void GetSize(void* actor, f32* width, f32* height) {
        reinterpret_cast<void (*)(UnkChaseSizeQuery_ovb0132*, void*, f32*, f32*)>(vtable[0x90 / 8])(this, actor, width,
                                                                                                  height);
    }
};

struct UnkChaseSizeFilter_ovb0132 {
    void** vtable;
    void Filter(u32 arg, f32* width, f32* height) {
        reinterpret_cast<void (*)(UnkChaseSizeFilter_ovb0132*, u32, f32*, f32*)>(vtable[0xb8 / 8])(this, arg, width,
                                                                                                 height);
    }
};

struct UnkChaseSize_ovb0132 {
    f32 width;
    f32 height;
};

// The owner's AI component as the action sees it.
struct UnkChaseMover_ovb0132 {
    void** vtable;
    u8 pad_0x8[0x1138];
    u32 field_0x1140;
    u8 pad_0x1144[4];
    UnkChaseSizeFilter_ovb0132* field_0x1148;
    u8 pad_0x1150[0x4c0];
    UnkChaseSizeQuery_ovb0132* source;  // +0x1610
    u8 pad_0x1618[0x12f00];
    u32 field_0x14518;
    u8 pad_0x1451c[0x174];
    UnkChaseSize_ovb0132 size;  // +0x14690

    f32 vf_0x698() { return reinterpret_cast<f32 (*)(UnkChaseMover_ovb0132*)>(vtable[0x698 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkChaseMover_ovb0132, source) == 0x1610, "layout");
static_assert(__builtin_offsetof(UnkChaseMover_ovb0132, field_0x14518) == 0x14518, "layout");
static_assert(__builtin_offsetof(UnkChaseMover_ovb0132, size) == 0x14690, "layout");

struct UnkChaseRef_ovb0132 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    void* target;  // +0x10
};

struct UnkChaseRefSlot_ovb0132 {
    UnkChaseRef_ovb0132* ptr;
    void Reset() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                ptr->vf_0x10();
            ptr = nullptr;
        }
    }
};

struct UnkChaseVec3_ovb0132 {
    f32 x, y, z;
};

struct UnkChaseAction_ovb0132 {
    u8 pad_0x0[0x18];
    UnkActor* actor;  // +0x18
    u8 pad_0x20[0xc];
    u8 field_0x2c[0x1c];
    u8 pad_0x48[0x11];
    bool field_0x59;
    bool field_0x5a;
    u8 pad_0x5b;
    u32 field_0x5c;
    u32 state;  // +0x60
    u32 field_0x64;
    f32 width;   // +0x68
    f32 height;  // +0x6c
    u8 field_0x70[8];
    UnkChaseRefSlot_ovb0132 field_0x78;
    UnkChaseRefSlot_ovb0132 field_0x80;
    u8 pad_0x88[0x2c];
    u32 field_0xb4;
    u8 pad_0xb8[0x8];
    UnkChaseVec3_ovb0132 field_0xc0;
    f32 field_0xcc;  // the minimum size
    UnkChaseVec3_ovb0132 field_0xd0;
};
static_assert(__builtin_offsetof(UnkChaseAction_ovb0132, field_0xd0) == 0xd0, "layout");

void fn_710037b5c4(UnkChaseAction_ovb0132* self, void* out);
void fn_710037b7b4(UnkChaseAction_ovb0132* self, UnkChaseRefSlot_ovb0132* out);
void fn_710037bb64(UnkChaseAction_ovb0132* self, UnkChaseVec3_ovb0132* a, f32* b, UnkChaseVec3_ovb0132* c);

bool fn_710037a680(UnkChaseAction_ovb0132* self, UnkChaseSizeFilter_ovb0132* filter, u32 arg) {
    UnkActor* actor = self->actor;
    if (!actor)
        return false;
    UnkChaseMover_ovb0132* mover = static_cast<UnkChaseMover_ovb0132*>(actor->field_0x240);
    if (!mover)
        return false;
    UnkChaseSizeQuery_ovb0132* source = mover->source;
    if (!source)
        return false;
    self->field_0x64 = reinterpret_cast<UnkChaseSizeSource_ovb0132*>(source)->field_0x124;
    f32 width = 0.0f;
    f32 height = 0.0f;
    source->GetSize(actor, &width, &height);
    self->width = width;
    self->height = height;
    if (filter) {
        filter->Filter(arg, &self->width, &self->height);
        if (self->width < 0.0f && self->height < 0.0f) {
            self->width = width;
            self->height = height;
        }
    }
    f32 min = self->field_0xcc;
    if (min > 0.0f) {
        self->width = __builtin_fmaxf(self->width, min);
        self->height = __builtin_fmaxf(self->height, min);
    }
    if (self->field_0x78.ptr && self->field_0x78.ptr->target) {
        f32 r = mover->vf_0x698();
        if (r > -0.001f) {
            self->width = r;
            self->height = r;
        }
    }
    mover->size.width = self->width;
    mover->size.height = self->height;
    return true;
}

// A zero whose constructor is not constexpr, so the static is guarded.
struct UnkChaseStaticZero_ovb0132 {
    u32 value;
    UnkChaseStaticZero_ovb0132() : value(0) {}
};

inline const UnkChaseStaticZero_ovb0132& UnkChaseStaticZero() {
    static UnkChaseStaticZero_ovb0132 s_value asm("g_unk_71726b5354");
    return s_value;
}

void fn_710037a4bc(UnkChaseAction_ovb0132* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    self->field_0x5c = 0;
    if (self->state != 2) {
        if (UnkActor* actor = self->actor) {
            if (UnkChaseMover_ovb0132* mover = static_cast<UnkChaseMover_ovb0132*>(actor->field_0x240)) {
                // Untyped: the original reloads the state after this store.
                u32 two = 2;
                __builtin_memcpy(&mover->field_0x14518, &two, 4);
                if (self->field_0x5a && self->state != 2)
                    fn_71010fa828(reinterpret_cast<UnkAIComponent*>(mover));
            }
        }
        self->field_0x5c = 0;
        self->state = 2;
    }
    self->field_0xc0.x = 0.0f;
    self->field_0xc0.y = 0.0f;
    self->field_0xc0.z = 0.0f;
    self->field_0xcc = 0.0f;
    self->field_0xd0.x = 0.0f;
    self->field_0xd0.y = 0.0f;
    self->field_0xd0.z = 0.0f;
    __builtin_memset(self->field_0x2c, 0, 0x1c);
    if (UnkActor* actor = self->actor) {
        if (UnkChaseMover_ovb0132* mover = static_cast<UnkChaseMover_ovb0132*>(actor->field_0x240)) {
            fn_710037b5c4(self, self->field_0x70);
            if (self->field_0x59) {
                fn_710037b7b4(self, &self->field_0x78);
                fn_710037bb64(self, &self->field_0xc0, &self->field_0xcc, &self->field_0xd0);
            } else {
                UnkActor* target = fn_7100d7c990(self->actor) ? fn_7100d7c990(self->actor) : self->actor;
                f32 x = target->field_0x44;
                f32 y = target->field_0x54;
                f32 z = target->field_0x64;
                self->field_0xd0.x = x;
                self->field_0xd0.y = y;
                self->field_0xd0.z = z;
            }
            fn_710037a680(self, mover->field_0x1148, mover->field_0x1140);
        }
    }
    self->field_0x78.Reset();
    self->field_0x80.Reset();
    self->field_0xb4 = UnkChaseStaticZero().value;
}
