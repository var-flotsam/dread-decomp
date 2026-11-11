// Unit 1545 (tools/tu_map.py; reflection hint CAllowCoolShinesparkLogicAction): the destructors of a
// class with vtable g_unk_7101a8e340 (fn_7100a33260, and the deleting form fn_7100a3331c). They
// release a string id at +0x60 and free two arrays (+0x38, +0x10), then run the base destructor
// (fn_7100a55ab8). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

struct UnkArrayMember_b50a5 : base::UnkVector<void*> {
    ~UnkArrayMember_b50a5() { Free(); }
};

extern char g_unk_7101a8e340[];
struct UnkLogicActionBase;

// One traits type per destructor: each instantiation's symbol names its address through the tag.
#define DTOR_TRAITS_B50A5(Tag)                                      struct Tag;                                                     template <>                                                     struct base::UnkDtorTraits<Tag> {                                   static char* vtable() { return g_unk_7101a8e340; }              static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }             };
DTOR_TRAITS_B50A5(UnkDtor_fn_7100a33260)
DTOR_TRAITS_B50A5(UnkDtor_fn_7100a3331c)

template void base::refl_Dtor<UnkDtor_fn_7100a33260, base::UnkStrIdAt<0x60>,
                              base::UnkMemberAt<UnkArrayMember_b50a5, 0x38>,
                              base::UnkMemberAt<UnkArrayMember_b50a5, 0x10>>(char*);
template void base::refl_DeletingDtor<UnkDtor_fn_7100a3331c, base::UnkStrIdAt<0x60>,
                                      base::UnkMemberAt<UnkArrayMember_b50a5, 0x38>,
                                      base::UnkMemberAt<UnkArrayMember_b50a5, 0x10>>(char*);
