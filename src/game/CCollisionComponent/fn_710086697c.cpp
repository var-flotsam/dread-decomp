// A collider group read from the collision file (unit 1319, CCollisionComponent): its name, then the
// names of its members, each looked up among the records already read and linked both ways; the
// group is appended to the list of groups. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A collider group or a collider as far as the link touches them: the groups a collider is in, the
// colliders a group holds, a flag word the members' vf_0x58 adds to and two flags passed on.
struct UnkColliderNode {
    void** vtable;
    u8 pad_0x8[0x20];
    u8 field_0x28;
    u8 pad_0x29[0x3b];
    u32 flags;  // 0x64
    u8 pad_0x68[3];
    bool field_0x6b;
    u8 pad_0x6c[0xb8];
    u32 child_count;  // 0x124
    UnkColliderNode* children[7];  // 0x128
    u32 pad_0x160;
    u32 parent_count;  // 0x164 (in a collider)
    UnkColliderNode* parents[1];   // 0x168

    u32 vf_0x58() { return reinterpret_cast<u32 (*)(UnkColliderNode*)>(vtable[0x58 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkColliderNode, child_count) == 0x124, "UnkColliderNode placeholder layout");
static_assert(__builtin_offsetof(UnkColliderNode, parent_count) == 0x164, "UnkColliderNode placeholder layout");

// The collision manager (src/game/CCollisionComponent/fn_7100863a8c.cpp) and the group it creates
// for an owner under a name.
struct UnkManager_7100863a8c;
void fn_7100d46804(UnkManager_7100863a8c* self);
extern UnkManager_7100863a8c* g_unk_71726a4788;
UnkColliderNode* fn_71008625a0(UnkManager_7100863a8c* self, void* owner, const CStrId* name);

static inline UnkManager_7100863a8c* GetManager() {
    if (!g_unk_71726a4788) {
        UnkManager_7100863a8c* m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return g_unk_71726a4788;
}

// The file's reads: a string (its address in the file's buffer) and raw bytes.
s64 fn_710007cee4(void* stream, const char** out);
struct UnkEntry;
void fn_7100d35a98(UnkEntry* entry, u32 flag);  // as src/game/CAutectorAIComponent/fn_71003bb408.cpp declares it

struct UnkCollisionComponentOwner {
    u8 pad_0x0[0x10];
    void* owner;
};

// NON_MATCHING(minor): the same calls in the same order; the locals sit in other stack slots (the
// frame is 0x10 bytes smaller) and the loops are scheduled differently.
void fn_710086697c(UnkCollisionComponentOwner* self, base::global::CRntFile* stream,
                   base::UnkVector<UnkCollisionRecord>* records, base::UnkVector<UnkColliderNode*>* groups) {
    const char* name;
    fn_710007cee4(stream, &name);
    UnkManager_7100863a8c* manager = GetManager();
    UnkColliderNode* group;
    {
        CStrId id(name, true);
        group = fn_71008625a0(manager, self->owner, &id);
    }
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(group), 1);
    u32 count = 0;
    fn_710007c714(stream, &count, 4, 1);
    for (u32 i = 0; i < count; ++i) {
        const char* member;
        fn_710007cee4(stream, &member);
        for (UnkCollisionRecord* r = records->data, *end = r + records->count; r != end; ++r) {
            s64 key;
            {
                CRntString text(member);
                key = text.key;
            }
            const auto& id = *reinterpret_cast<const CStrId*>(&r->field_0x0);
            if (base::reflection::refl_RntStringOf(id).key != key)
                continue;
            if (auto* collider = reinterpret_cast<UnkColliderNode*>(r->field_0x28)) {
                group->children[group->child_count++] = collider;
                collider->parents[collider->parent_count++] = group;
                group->flags |= collider->vf_0x58();
                group->field_0x6b = group->field_0x6b ? true : collider->field_0x6b;
                group->field_0x28 = collider->field_0x28;
            }
            break;
        }
    }
    groups->PushBack(group);
}
