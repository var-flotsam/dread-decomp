// The heap objects base/UnkCloneRecords.h's refl_AddFunctionHolder creates (fn_7100af30a0 and its
// copies, one per GUI composition unit), and the functions their vtables hold next to them: the
// destructor (it stores the holder's vtable, then destroys the std::function), the deleting
// destructor, and the clone of the callable the std::function wraps (a vtable and three words,
// allocated with operator new). Each copy names its own vtables. Names and layouts are placeholders.
#include <functional>

#include "types.h"

template <char* Vtable>
struct UnkFnHolderB31 {
    char* vtable;
    u64 field_0x8;
    std::function<void()> fn;
    ~UnkFnHolderB31() { vtable = Vtable + 0x10; }
};

// The callable's captures: a pointer and a pair of words.
struct UnkFnCapturesB31 {
    struct Pair {
        u64 a, b;
    };
    void* owner;
    Pair pair;
};

template <char* Vtable>
struct UnkFnCallableB31 {
    char* vtable;
    UnkFnCapturesB31 captures;
    UnkFnCallableB31(const UnkFnCapturesB31& c) : vtable(Vtable + 0x10), captures(c) {}
};

template <char* Vtable>
__attribute__((always_inline)) static inline void DestroyHolder(UnkFnHolderB31<Vtable>* self) {
    self->~UnkFnHolderB31();
}

template <char* Vtable>
__attribute__((always_inline)) static inline void DeleteHolder(UnkFnHolderB31<Vtable>* self) {
    self->~UnkFnHolderB31();
    operator delete(self);
}

template <char* Vtable>
__attribute__((always_inline)) static inline UnkFnCallableB31<Vtable>* CloneCallable(
    const UnkFnCallableB31<Vtable>* self) {
    return new UnkFnCallableB31<Vtable>(self->captures);
}

extern char g_unk_7101a98070[];
extern char g_unk_7101a980a0[];
void fn_7100af31e0(UnkFnHolderB31<g_unk_7101a98070>* self) { DestroyHolder(self); }
void fn_7100af3220(UnkFnHolderB31<g_unk_7101a98070>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101a980a0>* fn_7100af32d4(const UnkFnCallableB31<g_unk_7101a980a0>* self) { return CloneCallable(self); }
extern char g_unk_7101a9a188[];
extern char g_unk_7101a9a1b8[];
void fn_7100b0280c(UnkFnHolderB31<g_unk_7101a9a188>* self) { DestroyHolder(self); }
void fn_7100b0284c(UnkFnHolderB31<g_unk_7101a9a188>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101a9a1b8>* fn_7100b02904(const UnkFnCallableB31<g_unk_7101a9a1b8>* self) { return CloneCallable(self); }
extern char g_unk_7101a9ce00[];
extern char g_unk_7101a9ce30[];
void fn_7100b4ca30(UnkFnHolderB31<g_unk_7101a9ce00>* self) { DestroyHolder(self); }
void fn_7100b4ca70(UnkFnHolderB31<g_unk_7101a9ce00>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101a9ce30>* fn_7100b4cb20(const UnkFnCallableB31<g_unk_7101a9ce30>* self) { return CloneCallable(self); }
extern char g_unk_7101a9d9c8[];
extern char g_unk_7101a9d9f8[];
void fn_7100b60a40(UnkFnHolderB31<g_unk_7101a9d9c8>* self) { DestroyHolder(self); }
void fn_7100b60a80(UnkFnHolderB31<g_unk_7101a9d9c8>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101a9d9f8>* fn_7100b60b34(const UnkFnCallableB31<g_unk_7101a9d9f8>* self) { return CloneCallable(self); }
extern char g_unk_7101aa02b0[];
extern char g_unk_7101aa02e0[];
void fn_7100b95334(UnkFnHolderB31<g_unk_7101aa02b0>* self) { DestroyHolder(self); }
void fn_7100b95374(UnkFnHolderB31<g_unk_7101aa02b0>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101aa02e0>* fn_7100b95420(const UnkFnCallableB31<g_unk_7101aa02e0>* self) { return CloneCallable(self); }
extern char g_unk_7101aa0328[];
extern char g_unk_7101aa0358[];
void fn_7100b955f0(UnkFnHolderB31<g_unk_7101aa0328>* self) { DestroyHolder(self); }
void fn_7100b95630(UnkFnHolderB31<g_unk_7101aa0328>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101aa0358>* fn_7100b956d0(const UnkFnCallableB31<g_unk_7101aa0358>* self) { return CloneCallable(self); }
extern char g_unk_7101aa03a0[];
extern char g_unk_7101aa03d0[];
void fn_7100b958a0(UnkFnHolderB31<g_unk_7101aa03a0>* self) { DestroyHolder(self); }
void fn_7100b958e0(UnkFnHolderB31<g_unk_7101aa03a0>* self) { DeleteHolder(self); }
UnkFnCallableB31<g_unk_7101aa03d0>* fn_7100b95984(const UnkFnCallableB31<g_unk_7101aa03d0>* self) { return CloneCallable(self); }
