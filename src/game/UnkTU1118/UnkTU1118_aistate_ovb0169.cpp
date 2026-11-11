// Unit 1118 (no class hint): an AI state object. Names and layouts are placeholders.
#include "game/UnkAIComponent.h"
#include "game/navmesh/SNavMeshPath.h"
#include "types.h"

struct UnkStateOwnerSub_b0169 {
    u8 pad_0x0[0x1f0];
    u8 flag;  // 0x1f0
};

struct UnkStateOwner_b0169 {
    u8 pad_0x0[0x1f0];
    UnkStateOwnerSub_b0169* sub;  // 0x1f0
};

struct __attribute__((packed)) UnkPacked64_b0169 {
    u64 value;
};

// An object the state asks whether it is ready.
struct UnkReadyObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
};

struct UnkAIState_b0169 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x74];
    s32 field_0xac;
    s32 field_0xb0;
    s32 field_0xb4;
    u8 pad_0xb8[4];
    s32 field_0xbc;
    u8 pad_0xc0[8];
    u8 field_0xc8;
    u8 pad_0xc9[0xb];
    UnkPacked64_b0169 field_0xd4;
    UnkPacked64_b0169 field_0xdc;
    UnkPacked64_b0169 field_0xe4;
    u8 pad_0xec[0x4c];
    UnkReadyObject_b0169* field_0x138;
    u8 field_0x140;
    u8 pad_0x141[0xc7];
    u8 member_0x208[8];
};
static_assert(__builtin_offsetof(UnkAIState_b0169, field_0x138) == 0x138, "layout");
static_assert(__builtin_offsetof(UnkAIState_b0169, member_0x208) == 0x208, "layout");

// Resets the tracked values and the owner's flag.
void fn_71006fce60(UnkAIState_b0169* self) {
    self->field_0xac = 0x80000001;
    self->field_0xb4 = 0x80000001;
    self->field_0xbc = 0;
    self->field_0xc8 = 0;
    self->field_0xd4.value = 0;
    self->field_0xdc.value = 0;
    self->field_0xe4.value = 0;
    fn_71002adc8c(self->member_0x208);
    UnkStateOwnerSub_b0169* sub = reinterpret_cast<UnkStateOwner_b0169*>(self->owner)->sub;
    if (sub)
        sub->flag = 0;
}

// A list-like object: ready, size and element accessors.
struct UnkListObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool vf_0x20();
    virtual s32 vf_0x28();
    virtual UnkReadyObject_b0169* vf_0x30(s32 index);
};

struct UnkResultObject_b0169 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void* vf_0x18();
};

struct UnkAIStateSelect_b0169 {
    u8 pad_0x0[0xac];
    s32 current;  // 0xac
    s32 previous;  // 0xb0
    u8 pad_0xb4[4];
    s32 field_0xb8;
    s32 field_0xbc;
    u8 pad_0xc0[8];
    u8 field_0xc8;
    u8 pad_0xc9[0x13];
    u8 vec_0xdc[8];
    u8 pad_0xe4[0x54];
    UnkListObject_b0169* list;  // 0x138
    u8 has_list;  // 0x140
    u8 pad_0x141[0xcf];
    u8 has_alt;  // 0x210
    u8 pad_0x211[7];
    u8 alt_list[8];  // 0x218
};

bool fn_71006fe35c(void* self, void* item);
void* fn_71006fe430(void* self, void* target, s32 index, s32 arg, void* extra);
struct UnkVec3;
void fn_710007b444(void*, const UnkVec3*);

// Selects the item at `index` of the state's list (counted from the end of the second list when negative).
bool fn_71006fe16c(UnkAIStateSelect_b0169* self, s32 index) {
    UnkListObject_b0169* list = nullptr;
    if (self->has_list) {
        list = self->list;
        if (list && !list->vf_0x20())
            list = nullptr;
    }
    UnkReadyObject_b0169* item;
    if (index >= 0) {
        if (!list)
            return false;
        if (!list->vf_0x20())
            return false;
        if (!list->vf_0x28())
            return false;
        list->vf_0x28();
        UnkListObject_b0169* sub = reinterpret_cast<UnkListObject_b0169*>(list->vf_0x30(0));
        if (sub->vf_0x28() <= index)
            return false;
        item = sub->vf_0x30(index);
        if (!item)
            return false;
    } else {
        if (!self->has_alt)
            return false;
        UnkListObject_b0169* alt = reinterpret_cast<UnkListObject_b0169*>(self->alt_list);
        s32 at = alt->vf_0x28() + index;
        if (at < 1 || at >= alt->vf_0x28())
            return false;
        item = alt->vf_0x30(at);
        if (!item)
            return false;
    }
    if (!fn_71006fe35c(self, item))
        return false;
    self->previous = self->current;
    self->current = index;
    UnkListObject_b0169* target = nullptr;
    if (self->has_list) {
        UnkListObject_b0169* object = self->list;
        if (object && object->vf_0x20())
            target = object;
    }
    if (self->field_0xb8 != self->current) {
        void* result = fn_71006fe430(self, target, self->current, self->field_0xbc, nullptr);
        if (result) {
            self->field_0xc8 = 1;
            const UnkVec3* v = static_cast<const UnkVec3*>(static_cast<UnkResultObject_b0169*>(result)->vf_0x18());
            fn_710007b444(self->vec_0xdc, v);
            return true;
        }
    }
    self->field_0xc8 = 0;
    return true;
}
