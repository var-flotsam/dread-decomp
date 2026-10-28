#pragma once

// The tunable and named-property statics a unit's static initialiser builds, shared by both helper
// families (include/game/UnkTUInit.h, and unk_static_init in src/game_5821e4/UnkStaticInit.h). Each
// is a global reached through the GOT, built in place, with its destructor registered with
// __cxa_atexit. A property is linked into the global list g_unk_71726db788 (game/UnkTUSymbols.h)
// as it is built; then it is checked through its vtable (slots 2 and 3) and, if those allow it,
// handed to the unit's own function. Layouts and slot roles are placeholders.
//
// The original links a property into the list and stores its vtable in either order, unit by unit:
// the property and the list head are distinct globals, so the scheduler may, and the order in the
// source steers it. Each property initialiser therefore comes in two forms: the plain one links
// first, the VtableFirst one stores the vtable first.

#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkTUSymbols.h"
#include "types.h"

// A tunable: its base constructor is out of line, then its own vtable is stored and its destructor
// registered.
struct UnkTunable {
    void** vtable;
};

// The base constructor most tunables pass to InitTunable (src/base/tunable/CTunable/fn_7100138090.cpp).
void fn_7100136ecc(UnkTunable* self);

__attribute__((always_inline)) inline void InitTunable(UnkTunable* tunable, void (*ctor)(UnkTunable*), void** vtable,
                                                       void (*dtor)(void*)) {
    ctor(tunable);
    tunable->vtable = vtable + 2;
    fn_7100000250(dtor, tunable, g_unk_7101a19000);
}

// Storage of a property's name: a string id constructed in place, so the property stays trivially
// constructible.
struct UnkPropertyName {
    base::global::UnkStringData* data;
};

// The plain property: its name at 0x10 and its value at 0x18.
struct UnkProperty : UnkPropertyHead {
    UnkPropertyName name;
    const void* value;
    u32 field_0x20;

    __attribute__((always_inline)) UnkProperty(void** vt, UnkPropertyHead*& head, const char* str, const void* v) {
        next = head;
        head = this;
        vtable = vt + 2;
        new (&name) base::global::UnkStrIdBase(str, true);
        value = v;
        field_0x20 = 0;
    }
    bool vf_0x10() { return reinterpret_cast<bool (*)(UnkProperty*)>(vtable[2])(this); }
    bool vf_0x18() { return reinterpret_cast<bool (*)(UnkProperty*)>(vtable[3])(this); }
};

template <class Init>
__attribute__((always_inline)) inline void InitProperty(UnkProperty* p, void** vtable, UnkPropertyHead*& head,
                                                        const char* name, const void* value, Init on_init,
                                                        void (*dtor)(void*)) {
    new (p) UnkProperty(vtable, head, name, value);
    if (p->vf_0x10() && !p->vf_0x18())
        on_init(p);
    fn_7100000250(dtor, p, g_unk_7101a19000);
}

template <class Init>
__attribute__((always_inline)) inline void InitPropertyVtableFirst(UnkProperty* p, void** vtable,
                                                                   UnkPropertyHead*& head, const char* name,
                                                                   const void* value, Init on_init,
                                                                   void (*dtor)(void*)) {
    p->next = head;
    p->vtable = vtable + 2;
    head = p;
    new (&p->name) base::global::CStrId(name, true);
    p->value = value;
    p->field_0x20 = 0;
    if (p->vf_0x10() && !p->vf_0x18())
        on_init(p);
    fn_7100000250(dtor, p, g_unk_7101a19000);
}

// The extended property: its base holds a member at 0x10 that the base's own constructor builds out
// of line (fn_71002ef5bc for the 0x18-byte member, fn_7100ae9cc8 for the 0x160-byte one), between
// the base's vtable and the property's own; then a name, a pointer to the global it controls and a
// word.
void fn_71002ef5bc(void* member);
void fn_7100ae9cc8(void* member);

template <u32 MemberSize = 0x18>
struct UnkPropertyEx : UnkPropertyHead {
    u8 field_0x10[MemberSize];
    UnkPropertyName name;
    void* value;
    u32 field_0x38;  // at 0x38 after the 0x18-byte member, at 0x180 after the 0x160-byte one

    bool vf_0x10() { return reinterpret_cast<bool (*)(UnkPropertyEx*)>(vtable[2])(this); }
    bool vf_0x18() { return reinterpret_cast<bool (*)(UnkPropertyEx*)>(vtable[3])(this); }
};

template <class P, class Init>
__attribute__((always_inline)) inline void InitPropertyEx(P* p, void** base_vtable, UnkPropertyHead*& head,
                                                          void** vtable, void (*member_ctor)(void*), const char* name,
                                                          void* value, Init on_init, void (*dtor)(void*)) {
    p->next = head;
    head = p;
    p->vtable = base_vtable + 2;
    member_ctor(p->field_0x10);
    p->vtable = vtable + 2;
    new (&p->name) base::global::UnkStrIdBase(name, true);
    p->value = value;
    p->field_0x38 = 0;
    if (p->vf_0x10() && !p->vf_0x18())
        on_init(p);
    fn_7100000250(dtor, p, g_unk_7101a19000);
}

template <class P, class Init>
__attribute__((always_inline)) inline void InitPropertyExVtableFirst(P* p, void** base_vtable, UnkPropertyHead*& head,
                                                                     void** vtable, void (*member_ctor)(void*),
                                                                     const char* name, void* value, Init on_init,
                                                                     void (*dtor)(void*)) {
    p->next = head;
    p->vtable = base_vtable + 2;
    head = p;
    member_ctor(p->field_0x10);
    p->vtable = vtable + 2;
    new (&p->name) base::global::CStrId(name, true);
    p->value = value;
    p->field_0x38 = 0;
    if (p->vf_0x10() && !p->vf_0x18())
        on_init(p);
    fn_7100000250(dtor, p, g_unk_7101a19000);
}
