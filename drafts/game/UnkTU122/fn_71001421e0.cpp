// Unit 122: destroys every object in a list of tagged entries (the entry's top byte says which kind of
// object it holds), then empties the list. Names and layouts are placeholders.
#include "types.h"

struct UnkTaggedObject;
struct UnkTaggedObject124;
struct UnkHolder_b0187a;

void fn_71001429bc(UnkTaggedObject* self);
void fn_7100142aac(UnkTaggedObject124* self);
void fn_7100147900(UnkTaggedObject* self);
void fn_7100147b5c(UnkTaggedObject* self);
void fn_7100147ca4(UnkTaggedObject* self);
void fn_7100147df4(UnkHolder_b0187a* self);
void fn_7100141d7c(UnkTaggedObject* self);
void fn_7100147f48(UnkTaggedObject* self);
void fn_7100143244(UnkTaggedObject* self);
void fn_71001419b4(UnkTaggedObject* self);
void fn_7100141c8c(UnkTaggedObject* self);
void fn_7100141b04(UnkTaggedObject* self);

// An object with a virtual destructor and a release method in slot 3.
struct UnkPolyObject_1421e0 {
    virtual ~UnkPolyObject_1421e0();
    virtual void vf_0x10();
    virtual void Release();
};

struct UnkEntry_1421e0 {
    u32 bits;  // the kind in the top byte
    u8 pad_0x4[0xc];
    void* object;  // 0x10
};

struct UnkEntryList_1421e0 {
    void* data;
    u32 count;  // 0x8
};

struct UnkEntryOwner_1421e0 {
    u8 pad_0x0[0x88];
    UnkEntryList_1421e0 entries;  // 0x88
};

UnkEntry_1421e0** fn_7100142648(UnkEntryList_1421e0* list, u32 index);

template <typename T>
static inline void DestroyAndFree(void* object, void (*destroy)(T*)) {
    if (object) {
        destroy(static_cast<T*>(object));
        operator delete(object);
    }
}

void fn_71001421e0(UnkEntryOwner_1421e0* self) {
    for (u32 i = 0; i < self->entries.count; ++i) {
        UnkEntry_1421e0* entry = *fn_7100142648(&self->entries, i);
        switch (entry->bits >> 24) {
        case 0:
            DestroyAndFree(entry->object, fn_71001429bc);
            break;
        case 1:
            DestroyAndFree(entry->object, fn_7100142aac);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 19:
        case 24:
            delete static_cast<UnkPolyObject_1421e0*>(entry->object);
            break;
        case 20:
            if (UnkPolyObject_1421e0* object = static_cast<UnkPolyObject_1421e0*>(entry->object))
                object->Release();
            break;
        case 7:
            DestroyAndFree(entry->object, fn_7100147900);
            break;
        case 8:
            DestroyAndFree(entry->object, fn_7100147b5c);
            break;
        case 9:
            DestroyAndFree(entry->object, fn_7100147ca4);
            break;
        case 10:
            DestroyAndFree(entry->object, fn_7100147df4);
            break;
        case 11:
            DestroyAndFree(entry->object, fn_7100141d7c);
            break;
        case 12:
            DestroyAndFree(entry->object, fn_7100147f48);
            break;
        case 18:
            DestroyAndFree(entry->object, fn_7100143244);
            break;
        case 21:
            DestroyAndFree(entry->object, fn_71001419b4);
            break;
        case 22:
            DestroyAndFree(entry->object, fn_7100141c8c);
            break;
        case 23:
            DestroyAndFree(entry->object, fn_7100141b04);
            break;
        }
    }
    self->entries.count = 0;
}
