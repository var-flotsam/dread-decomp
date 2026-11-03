// game::logic::collision::CColliderGroup (unit 2504 of tools/tu_map.py, the name from reflection):
// sets the flag at +0x28. When the group is registered with an owner (+0x30) whose tracking flag
// (+0x6a) is set, it is removed from the owner (fn_7100d5ac50) before the change and added back
// (fn_7100d5a304) after it. Names and layouts are placeholders.
#include "types.h"

struct UnkStruct_7100861148_Collider;
struct UnkTracker_71008618cc;
struct UnkReflObject;
void fn_7100d5ac50(void* owner, UnkStruct_7100861148_Collider* group);
void fn_7100d5a304(UnkTracker_71008618cc* self, UnkReflObject* group);

struct UnkColliderGroupFlags {
    u8 unk_0x0[0x28];
    bool flag;
    u8 pad_0x29[0x7];
    void* owner;
    u8 unk_0x38[0x32];
    bool tracked;
};

void fn_7100d45a84(UnkColliderGroupFlags* self, bool value) {
    if (self->flag == value)
        return;
    void* owner = self->owner;
    if (owner && self->tracked) {
        fn_7100d5ac50(owner, (UnkStruct_7100861148_Collider*)self);
        self->flag = value;
        fn_7100d5a304((UnkTracker_71008618cc*)owner, (UnkReflObject*)self);
        return;
    }
    self->flag = value;
}
