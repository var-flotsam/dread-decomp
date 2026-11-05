// media::CMoviePlayer's unit: slot 0xf8 of the reflected base::global::CSmartPtr<media::CMoviePlayer>
// pointer type (src/reflection/media_CMoviePlayer.cpp): assigns the pointer a typed pointer holds
// (+0x8) to the smart pointer. The object keeps an intrusive count at +0x8 and is destroyed through
// vtable slot 2 when it reaches zero. Names and layouts are placeholders.
#include "types.h"

struct UnkMoviePlayerRef_b59 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 refs;
};

struct UnkMoviePlayerTypedPtr_b59 {
    void* type;
    UnkMoviePlayerRef_b59* ptr;
};

struct UnkMoviePlayerSmartPtr_b59 {
    UnkMoviePlayerRef_b59* ptr;

    void Assign(UnkMoviePlayerRef_b59* p) {
        if (ptr == p)
            return;
        if (p)
            __atomic_fetch_add(&p->refs, 1, __ATOMIC_ACQ_REL);
        UnkMoviePlayerRef_b59* old = ptr;
        if (old) {
            __atomic_fetch_sub(&old->refs, 1, __ATOMIC_ACQ_REL);
            if (__atomic_load_n(&old->refs, __ATOMIC_ACQUIRE) == 0)
                old->vf_0x10();
        }
        ptr = p;
    }
};

void fn_710021e198(void* self, const void* value) {
    static_cast<UnkMoviePlayerSmartPtr_b59*>(self)->Assign(static_cast<const UnkMoviePlayerTypedPtr_b59*>(value)->ptr);
}
