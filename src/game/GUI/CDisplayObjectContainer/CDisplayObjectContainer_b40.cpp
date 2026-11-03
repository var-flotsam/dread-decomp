// GUI::CDisplayObjectContainer unit (tools/tu_map.py, the name from reflection): reflected functions
// of the container at 8 that return a child (a pointer, described by fn_7100fc036c): the first child
// in the list at +0x1d0 whose id at +0x130 is the argument's, or what fn_71011d9780 finds for a u32
// and a second argument (null when it has no type); and one that passes the child at the u32 index
// given to the GUI root's fn_71011b57c4 and returns true. Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiRoot.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;

const CType* fn_7100fc036c();
void* fn_71011d9780(char* self, u32 index, void* arg);

static inline __attribute__((always_inline)) void* ArgPtr_b40a3(UnkReflValue* arg) {
    if (!arg->type)
        return nullptr;
    if (arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8)
        return arg->field_0x0;
    return arg;
}

static inline __attribute__((always_inline)) UnkReflValue ReturnPtr_b40a3(void*& v) {
    const CType* type = fn_7100fc036c();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &v, false);
    return r;
}

struct UnkChildLink_b40a3 {
    char* child;
    UnkChildLink_b40a3* next;
};

UnkReflValue fn_71011dc494(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* a = args[0];
    const u64* id = static_cast<const u64*>(ArgPtr_b40a3(a));
    UnkChildLink_b40a3* it = *reinterpret_cast<UnkChildLink_b40a3**>(target + 0x1d0);
    char* v;
    if (!it) {
        v = nullptr;
    } else {
        do {
            v = it->child;
            it = it->next;
            if (*reinterpret_cast<u64*>(v + 0x130) != *id)
                v = nullptr;
        } while (it && !v);
    }
    void* found = v;
    return ReturnPtr_b40a3(found);
}

UnkReflValue fn_71011dc794(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    u32 index = *base::reflection::refl_ArgValue<u32>(&a[0]);
    void* p1 = ArgPtr_b40a3(&a[1]);
    void* v = fn_71011d9780(target, index, p1);
    return ReturnPtr_b40a3(v);
}

static inline char* ChildAt_b40a3(char* self, u32 index) {
    UnkChildLink_b40a3* it = *reinterpret_cast<UnkChildLink_b40a3**>(self + 0x1d0);
    for (u32 i = 0; i < index; ++i)
        it = it->next;
    return it->child;
}

UnkReflValue fn_71011dc848(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* a = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    u32 index = *base::reflection::refl_ArgValue<u32>(a);
    fn_71011b57c4(g_unk_717271e8f8, ChildAt_b40a3(target, index));
    bool done = true;
    const CType* type = fn_710000d228();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &done, false);
    return r;
}
