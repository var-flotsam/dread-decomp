// behaviortree::CEmmyPatrolAction unit (tools/tu_map.py unit 802): a destructor (vtable 0x7101a47c98) of a
// class holding, in reverse order of release: a node owner at +0x30 (base/UnkNodeOwner.h, vtables
// 0x7101a47c78 and 0x7101a21050, next link at +8), two arrays of plain values at +0xa0 and +0xc8, a
// registration at +0xf8 (removed from its deferred set through fn_7100514b7c), and at +0x120 the object
// fn_7100514880 destroys (vtable 0x7101a47c40: a string id at +8, an array of 0x10-byte polymorphic
// elements at +0x18); then the base's destructor fn_7100393ea0. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkNodeOwner.h"
#include "base/UnkVector.h"
#include "game/behaviortree/CBaseAction.h"
#include "game/behaviortree/CEmmyPatrolAction.h"
#include "types.h"

extern void* g_unk_7101a47c98[];
extern void* g_unk_7101a47c78[];
extern void* g_unk_7101a21050[];
extern void* g_unk_7101a47c40[];

struct UnkPatrolOwnerDtor_A3_b69;
struct UnkPatrolMemberDtor_A3_b69;

template <>
struct base::UnkDtorTraits<UnkPatrolOwnerDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a47c98); }
    static void base(char* self) { fn_7100393ea0(self); }
};

template <>
struct base::UnkDtorTraits<UnkPatrolMemberDtor_A3_b69> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a47c40); }
};

struct UnkPatrolMemberElem_A3_b69 {
    virtual ~UnkPatrolMemberElem_A3_b69();
    void* field_0x8;
};

struct UnkPatrolMemberVector_A3_b69 {
    static void Release(char* self) {
        reinterpret_cast<base::UnkVector<UnkPatrolMemberElem_A3_b69>*>(self + 0x18)->Free();
    }
};

struct UnkPatrolMember_A3_b69 {
    __attribute__((always_inline)) static void Release(char* self) {
        char* member = self + 0x120;
        *reinterpret_cast<char**>(member) = base::UnkDtorTraits<UnkPatrolMemberDtor_A3_b69>::vtable() + 0x10;
        UnkPatrolMemberVector_A3_b69::Release(member);
        base::UnkStrIdAt<0x8>::Release(member);
    }
};

struct UnkPatrolRegistration_A3_b69 {
    void* set;
    u64 field_0x8;
    u64 field_0x10;
    static void Release(char* self) {
        auto* reg = reinterpret_cast<UnkPatrolRegistration_A3_b69*>(self + 0xf8);
        if (reg->set) {
            fn_7100514b7c(reg->set, reg);
            reg->set = nullptr;
        }
        reg->field_0x8 = 0;
        reg->field_0x10 = 0;
    }
};

template <u32 Offset>
struct UnkPatrolValues_A3_b69 {
    static void Release(char* self) { reinterpret_cast<base::UnkVector<u64>*>(self + Offset)->Free(); }
};

__attribute__((always_inline)) inline void UnkPatrolNoNodeDtor_A3_b69(UnkListNode<8>*) {}

struct UnkPatrolNodes_A3_b69 {
    static void Release(char* self) {
        reinterpret_cast<UnkNodeOwner<UnkListNode<8>, UnkPatrolNoNodeDtor_A3_b69>*>(self + 0x30)
            ->Destroy(g_unk_7101a47c78, g_unk_7101a21050);
    }
};

__attribute__((flatten)) void fn_7100514614(char* self) {
    base::refl_Dtor<UnkPatrolOwnerDtor_A3_b69, UnkPatrolMember_A3_b69, UnkPatrolRegistration_A3_b69,
                    UnkPatrolValues_A3_b69<0xc8>, UnkPatrolValues_A3_b69<0xa0>, UnkPatrolNodes_A3_b69>(self);
}
