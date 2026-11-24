// CDemolitionBlockSonarTargetComponent's unit: the start. After the component base start (fn_71010c4228) it keeps
// a weak handle (+0x40) to the owner's demolition block (fn_710031cd8c) and, for every collider the owner's
// collision container (+0x2a0) lists under g_unk_71727438f8, appends a target built from the collider's
// shape (fn_7100a0ed5c) to the list at +0x48. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// As in src/game/CCollisionComponent/fn_71008651c8.cpp.
struct UnkStruct_71008651c8_Entry;
struct UnkStruct_71008651c8;
using UnkOut_71008651c8 = base::global::CRntVector<UnkStruct_71008651c8_Entry*>;
void CCollisionComponent_CollectRecordsBySecondaryId(UnkStruct_71008651c8* self, void* const* key, UnkOut_71008651c8* out);

struct UnkRefl_fn_710031cd8c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(void*);

extern void* const g_unk_71727438f8 __attribute__((visibility("hidden")));  // the colliders' key
extern void* g_unk_71727438d0;

struct UnkSonarTargetEntry_la0f964;
struct UnkSonarOwner_la0f964;

void fn_7100a0ed5c(UnkSonarTargetEntry_la0f964* self, UnkSonarOwner_la0f964* owner, void* shape, void* value);

// A collider the container lists: its shape at +0x28.
struct UnkSonarCollider_la0f964 {
    u8 pad_0x0[0x28];
    void* shape;  // 0x28
};

struct UnkSonarTargetEntry_la0f964 {
    u8 pad_0x0[0x20];
};

struct UnkSonarTargets_la0f964;
void fn_7100a102e8(UnkSonarTargets_la0f964* self, u32 capacity);

// The targets: a vector of 0x20-byte entries growing by `step` (fn_7100a102e8 moves them over).
struct UnkSonarTargets_la0f964 {
    UnkSonarTargetEntry_la0f964* data;
    u32 count;
    u32 capacity;
    u32 step;

    // The arguments are taken by reference: they are read only once the room is there.
    void Append(UnkSonarOwner_la0f964* const& owner, void* const& shape, void* const& value) {
        u32 n = count;
        if (n == capacity) {
            u32 new_capacity = n;
            if (step != 0)
                new_capacity = (n / step + 1) * step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            fn_7100a102e8(this, new_capacity);
            n = count;
        }
        fn_7100a0ed5c(&data[n], owner, shape, value);
        ++count;
    }
};

struct UnkSonarOwner_la0f964 {
    u8 pad_0x0[0x2a0];
    UnkStruct_71008651c8* collision;  // 0x2a0
};

struct UnkSonarTarget_la0f964 {
    u8 pad_0x0[0x30];
    UnkSonarOwner_la0f964* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkWeakHandle* block;            // 0x40
    UnkSonarTargets_la0f964 targets;  // 0x48
};
static_assert(__builtin_offsetof(UnkSonarTarget_la0f964, targets) == 0x48, "layout");

void fn_7100a0f964(UnkSonarTarget_la0f964* self) {
    fn_71010c4228(reinterpret_cast<UnkParent*>(self));
    UnkWeakTarget* block = reinterpret_cast<UnkWeakTarget*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_710031cd8c>(self->owner));
    if (!block)
        return;
    AssignRef(&self->block, GetWeakHandle(block));
    UnkStruct_71008651c8* collision = self->owner->collision;
    UnkOut_71008651c8 found(0xf);
    CCollisionComponent_CollectRecordsBySecondaryId(collision, &g_unk_71727438f8, &found);
    for (UnkStruct_71008651c8_Entry* entry : found) {
        auto* collider = reinterpret_cast<UnkSonarCollider_la0f964*>(entry);
        self->targets.Append(self->owner, collider->shape, g_unk_71727438d0);
    }
    found.Free();
}
