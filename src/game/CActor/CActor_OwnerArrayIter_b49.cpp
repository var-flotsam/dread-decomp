// CActor's unit (tools/tu_map.py unit 341): Begin and End (slots 0x160 and 0x168) of a reflected
// array of owning pointers, as base::refl_Begin and refl_End (base/UnkCollectionIter.h). An owner
// lets go of its object, which is told first (vf_0x90) and then deleted. Names are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkActorOwnedObject_b49a1 {
    virtual void vf_0x0();
    virtual ~UnkActorOwnedObject_b49a1();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();  // told before it is deleted
};

struct UnkActorOwner_b49a1 {
    UnkActorOwnedObject_b49a1* ptr;

    ~UnkActorOwner_b49a1() {
        UnkActorOwnedObject_b49a1* p = ptr;
        ptr = nullptr;
        if (p) {
            p->vf_0x90();
            delete p;
        }
    }
};

void fn_7100244148(const base::UnkVector<UnkActorOwner_b49a1>* v, base::UnkCollectionIter<UnkActorOwner_b49a1>* out) {
    base::refl_Begin(v, out);
}

void fn_7100244290(const base::UnkVector<UnkActorOwner_b49a1>* v, base::UnkCollectionIter<UnkActorOwner_b49a1>* out) {
    base::refl_End(v, out);
}
