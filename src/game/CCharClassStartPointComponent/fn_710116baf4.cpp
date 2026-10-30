// Destructors (complete and deleting) of an object with a name string at 0x40 and a string id
// at 0x70: restore the vtable, release the id, destroy the string and run the base destructor
// (fn_71010c4194); the deleting one then frees the object.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/UnkVector.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkBase_0x40 {
    void** vtable;
    u8 unk_0x8[0x38];
    ~UnkBase_0x40();  // fn_71010c4194
};

// A string id whose release decrements and tests the count in two steps.
struct UnkEntryId {
    UnkStringData* data;

    ~UnkEntryId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

extern void* g_unk_7101cb41c0[];

struct UnkObject_710116baf4 : UnkBase_0x40 {
    CRntString field_0x40;
    UnkEntryId field_0x70;

    ~UnkObject_710116baf4() { vtable = &g_unk_7101cb41c0[2]; }
};

void fn_710116ba70(UnkObject_710116baf4& self) {
    self.~UnkObject_710116baf4();
}

void fn_710116baf4(UnkObject_710116baf4& self) {
    delete &self;
}

// The same pair for three more classes, with their own vtables, base destructors, layouts and (for
// two of them) member destructors: a member at `NameAt` destroyed by `NameDtor`, then the base
// destructor, after releasing the string id at `IdAt`.
struct UnkStruct_7100416e80;

void fn_71006b2dc0(void* self);
void fn_71006c0074(void* self);
void fn_7100416e80(UnkStruct_7100416e80* self);
void fn_71011bf1a4(char* self);

extern void* g_unk_7101a63ad0[];
extern void* g_unk_7101a64f60[];
extern void* g_unk_7101a9a200[];

template <u32 NameAt, u32 IdAt, class Name>
struct UnkNamedObject {
    void** vtable;
    u8 unk_0x8[NameAt - 0x8];
    Name name;
    u8 unk_name_end[IdAt - NameAt - sizeof(Name)];
    UnkEntryId id;
};

template <void** Vtable, class Self, class Name, void (*NameDtor)(Name*), class Base, void (*BaseDtor)(Base*)>
static inline void DestroyNamed(Self* self) {
    self->vtable = &Vtable[2];
    self->id.~UnkEntryId();
    NameDtor(&self->name);
    BaseDtor(reinterpret_cast<Base*>(self));
}

struct UnkName_0x10 {
    u8 unk_0x0[0x10];
};

#define UNK_NAMED_DTORS(complete, deleting, vtable, Self, Name, name_dtor, Base, base_dtor) \
    void fn_##complete(Self* self) {                                                       \
        DestroyNamed<vtable, Self, Name, name_dtor, Base, base_dtor>(self);                \
    }                                                                                      \
    void fn_##deleting(Self* self) {                                                       \
        DestroyNamed<vtable, Self, Name, name_dtor, Base, base_dtor>(self);                \
        fn_7100083c00(self);                                                               \
    }

using UnkNamedObject_71006b91a8 = UnkNamedObject<0x510, 0x6a8, UnkName_0x10>;
using UnkNamedObject_71006c54dc = UnkNamedObject<0x510, 0x640, UnkName_0x10>;
using UnkNamedObject_7100b03744 = UnkNamedObject<0x2b0, 0x2e0, CRntString>;

UNK_NAMED_DTORS(71006b91a8, 71006b922c, g_unk_7101a63ad0, UnkNamedObject_71006b91a8, void, fn_71006b2dc0,
                UnkStruct_7100416e80, fn_7100416e80)
UNK_NAMED_DTORS(71006c54dc, 71006c5560, g_unk_7101a64f60, UnkNamedObject_71006c54dc, void, fn_71006c0074,
                UnkStruct_7100416e80, fn_7100416e80)
UNK_NAMED_DTORS(7100b03744, 7100b037c8, g_unk_7101a9a200, UnkNamedObject_7100b03744, CRntString, fn_710007ecd4,
                char, fn_71011bf1a4)
