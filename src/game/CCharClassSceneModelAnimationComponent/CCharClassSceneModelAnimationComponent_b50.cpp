// CCharClassSceneModelAnimationComponent unit (tools/tu_map.py, name from reflection): the destructor
// of a class with vtable g_unk_7101a8a658 (fn_71009f1e64). It destroys the 0x50-byte entries of the
// array at +0x60 (fn_71009f26a0), frees it, releases the string id at +0x58 and runs the base
// destructor (fn_71011acd74). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkIdObject;
void fn_71011acd74(UnkIdObject*);
struct UnkEntry_b50a5;
void fn_71009f26a0(UnkEntry_b50a5* self);

struct UnkEntry_b50a5 {
    u8 unk_0x0[0x50];
    ~UnkEntry_b50a5() { fn_71009f26a0(this); }
};

struct UnkEntryArray_b50a5 : base::UnkVector<UnkEntry_b50a5> {
    ~UnkEntryArray_b50a5() {
        Clear();
        Free();
    }
};

extern char g_unk_7101a8a658[];

// fn_71009f26a0: an entry's destructor. The entry holds two string ids and an object owned through
// a std::function deleter (libc++ layout: a 0x20-byte buffer at +0x20 and the callable at +0x40,
// which points into the buffer when stored there); an empty deleter throws bad_function_call
// (fn_71009f2970).
[[noreturn]] void fn_71009f2970();

struct UnkDeleterCallable_b50a5 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void Destroy();             // 0x20: in the buffer
    virtual void DestroyDeallocate();   // 0x28: on the heap
    virtual void Call(void** object);   // 0x30
};

struct UnkEntryBody_b50a5 {
    base::UnkStrIdMember id0;
    base::UnkStrIdMember id1;
    void* object;
    u8 unk_0x18[0x8];
    alignas(16) u8 buffer[0x20];
    UnkDeleterCallable_b50a5* deleter;

    __attribute__((always_inline)) void CallDeleter(void* arg) {
        if (!deleter)
            fn_71009f2970();
        deleter->Call(&arg);
    }

    __attribute__((always_inline)) ~UnkEntryBody_b50a5() {
        void* p = object;
        object = nullptr;
        if (p)
            CallDeleter(p);
        if (static_cast<void*>(deleter) == &buffer)
            deleter->Destroy();
        else if (deleter)
            deleter->DestroyDeallocate();
    }
};

void fn_71009f26a0(UnkEntry_b50a5* self) {
    reinterpret_cast<UnkEntryBody_b50a5*>(self)->~UnkEntryBody_b50a5();
}

struct UnkDtor_fn_71009f1e64;
template <>
struct base::UnkDtorTraits<UnkDtor_fn_71009f1e64> {
    static char* vtable() { return g_unk_7101a8a658; }
    static void base(char* p) { fn_71011acd74(reinterpret_cast<UnkIdObject*>(p)); }
};

template void base::refl_Dtor<UnkDtor_fn_71009f1e64, base::UnkMemberAt<UnkEntryArray_b50a5, 0x60>,
                              base::UnkStrIdAt<0x58>>(char*);
