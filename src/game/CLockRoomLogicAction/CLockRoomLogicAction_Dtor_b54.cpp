// CLockRoomLogicAction's unit (tools/tu_map.py unit 2531): the destructor. It first clears the
// back-pointer each element of the +0x58 array points at and empties the three arrays, then
// releases the members in reverse order (the arrays, the string id at +0x50, the callback list at
// +0x10 over g_unk_7101a33fa0/g_unk_7101a20998) and runs the logic action base destructor
// (fn_7100a55ab8). Layout from src/game/CLockRoomLogicAction/CLockRoomLogicAction_Ctor_b49.cpp.
// Names are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDestructors.h"
#include "game/CTriggerLogicAction.h"
#include "types.h"

struct UnkLogicActionBase;

extern char g_unk_7101aa9180[];   // CLockRoomLogicAction's vtable
extern void* g_unk_7101a20998[];  // the callback list's base vtable
extern void* g_unk_7101a33fa0[];  // the callback list's vtable

struct UnkLockRoomCallbacks_b54;
template <>
struct base::UnkCallbackTraits<UnkLockRoomCallbacks_b54> {
    static char* vtable() { return reinterpret_cast<char*>(g_unk_7101a33fa0); }
    static char* base_vtable() { return reinterpret_cast<char*>(g_unk_7101a20998); }
};
using UnkLockRoomCallbackList_b54 = base::UnkCallbackListShort<UnkLockRoomCallbacks_b54>;

struct UnkLockRoomArray_b54 : base::UnkVector<void**> {
    ~UnkLockRoomArray_b54() { Free(); }
};

// The destructor's own body: clears what the first array's elements point at, then empties all three.
struct UnkLockRoomUnlink_b54 {
    static void Release(char* self) {
        auto* a = reinterpret_cast<UnkLockRoomArray_b54*>(self + 0x58);
        for (void*** p = a->data, ***end = a->data + a->count; p != end; ++p)
            **p = nullptr;
        a->count = 0;
        reinterpret_cast<UnkLockRoomArray_b54*>(self + 0x80)->count = 0;
        reinterpret_cast<UnkLockRoomArray_b54*>(self + 0xa8)->count = 0;
    }
};

struct UnkDtor_fn_7100da3120;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_7100da3120> {
    static char* vtable() { return g_unk_7101aa9180; }
    static void base(char* p) { fn_7100a55ab8(reinterpret_cast<UnkLogicActionBase*>(p)); }
};

template void base::refl_Dtor<UnkDtor_fn_7100da3120, UnkLockRoomUnlink_b54,
                              base::UnkMemberAt<UnkLockRoomArray_b54, 0xa8>,
                              base::UnkMemberAt<UnkLockRoomArray_b54, 0x80>,
                              base::UnkMemberAt<UnkLockRoomArray_b54, 0x58>, base::UnkStrIdAt<0x50>,
                              base::UnkMemberAt<UnkLockRoomCallbackList_b54, 0x10>>(char*);
