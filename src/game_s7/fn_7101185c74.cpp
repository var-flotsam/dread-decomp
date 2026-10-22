// Copies of fn_7100b4c2e8 (src/game_aca57c/fn_7100b021fc.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7101185c74.
// Weak references to GUI objects: setters that take the focused object of the GUI root (its
// object at 0x230, whose target is at 0x10), and assignments from another object's handle.
#include "game_s2/UnkWeakHandle.h"

struct UnkGuiFocus {
    u8 unk_0x0[0x10];
    UnkWeakTarget* field_0x10;
};

struct UnkGuiRoot {
    u8 unk_0x0[0x230];
    UnkGuiFocus* field_0x230;
};

extern UnkGuiRoot* g_unk_717271e8f8;
void fn_71011cf2a4(void* composition, s32 value);
void fn_71011b6bbc(UnkGuiRoot* root, void* object, s32 value);

// Unless the flag at 0x181 is set, refers to the focused object at 0x318 (or to nothing), passes it
// 0 and this object 1, then hands this object to the root.
struct UnkGuiFocusUser318 {
    u8 unk_0x0[0x181];
    bool field_0x181;
    u8 unk_0x182[0x196];
    UnkWeakHandle* field_0x318;
};

// As fn_7100b021fc with the reference at 0x238, which is only changed when there is a focused
// object.
struct UnkGuiFocusUser238 {
    u8 unk_0x0[0x238];
    UnkWeakHandle* field_0x238;
};

// Refers the reference at 0xf0 to `other`'s weak handle.
struct UnkWeakRefF0 {
    u8 unk_0x0[0xf0];
    UnkWeakHandle* field_0xf0;
};

// A weak reference to a GUI::CBossRushReport, assigned from another weak reference: it refers to
// nothing when the other's object is not one.
void* fn_7100af4354();  // GUI::CBossRushReport::refl_GetClass
bool fn_710009d50c(const void* from, const void* to);

__attribute__((always_inline)) static inline const void* GetClass(UnkWeakTarget* target) {
    return reinterpret_cast<const void* (*)(UnkWeakTarget*)>((*reinterpret_cast<void***>(target))[0])(target);
}

struct UnkReportRef {
    UnkWeakHandle* handle;
};

void* fn_71011c3868();
UnkReportRef* fn_7101185c74(UnkReportRef* self, const UnkReportRef* other) {
    UnkWeakTarget* target = other->handle ? other->handle->target : nullptr;
    UnkWeakHandle* handle = nullptr;
    if (target && fn_710009d50c(GetClass(target), fn_71011c3868()))
        handle = GetWeakHandle(target);
    AssignRef(&self->handle, handle);
    return self;
}

