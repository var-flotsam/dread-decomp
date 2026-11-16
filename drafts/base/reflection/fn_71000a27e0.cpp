// Unit 82 (base::reflection): walks a reflected collection for a visitor. When the visitor walks
// the collection, an iterator runs from its begin to its end, handing each element, by its dynamic
// type, to that type's visit slot until one declines; the collection is reported last.
// Names and layouts are placeholders.
#include "base/reflection.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/CPointerType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTunableReflValue.h"
#include "types.h"

using base::reflection::CCollectionType;
using base::reflection::CPointerType;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;

struct UnkCollectionVisitor_l0a27e0;

// An element's type as the walk sees it: its visit slot.
struct UnkVisitedType_l0a27e0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual s32 Visit(UnkReflTypedPtr* value, UnkCollectionVisitor_l0a27e0* visitor);
};

struct UnkCollectionVisitor_l0a27e0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
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
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual s32 BeginCollection(UnkReflTypedPtr* collection);
    virtual void EndCollection(UnkReflTypedPtr* collection);
};

using IteratorFn = void (*)(void* collection, void* iterator);
using GetFn = UnkReflTypedPtr (*)(void* iterator);
using IncrementFn = void (*)(void* iterator);

static inline const CCollectionType* AsCollection(char* type) {
    auto* object = reinterpret_cast<UnkReflObject*>(type);
    return object && fn_710009d50c(object->vf_0x0(), fn_71000986d0())
               ? reinterpret_cast<const CCollectionType*>(object)
               : nullptr;
}

static inline const CPointerType* AsPointer(const char* type) {
    auto* object = reinterpret_cast<const UnkReflObject*>(type);
    return object && fn_710009d50c(object->vf_0x0(), fn_710009bedc())
               ? reinterpret_cast<const CPointerType*>(object)
               : nullptr;
}

// Where a value's data is: inline when it fits in 8 bytes, else through field_0x0.
static inline void* Storage(UnkReflValue* value) {
    if (!value->type)
        return nullptr;
    if (value->field_0x14 == 1 || reinterpret_cast<const CType*>(value->type)->m_size > 8)
        return value->field_0x0;
    return value;
}

// The element an iterator points to, with its dynamic type; none when the iterator's type cannot
// get one.
static inline UnkReflTypedPtr Get(const char* it_type, void* it) {
    const CPointerType* type = AsPointer(it_type);
    if (type->m_get && it_type == reinterpret_cast<const char*>(type))
        return reinterpret_cast<GetFn>(type->m_get)(it);
    UnkReflTypedPtr none;
    none.type = nullptr;
    none.ptr = nullptr;
    return none;
}

static inline void Increment(const char* it_type, void* it) {
    const CPointerType* type = AsPointer(it_type);
    if (type->m_increment && it_type == reinterpret_cast<const char*>(type))
        reinterpret_cast<IncrementFn>(type->m_increment)(it);
}

// Hands the element, by its dynamic type, to that type's visit slot.
static inline s32 VisitElement(UnkCollectionVisitor_l0a27e0* self, const UnkReflTypedPtr& element) {
    UnkReflTypedPtr typed;
    typed.type = fn_710009dcd8(element.type, element.ptr);
    typed.ptr = element.ptr;
    return reinterpret_cast<UnkVisitedType_l0a27e0*>(element.type)->Visit(&typed, self);
}

// Runs an iterator over the collection, visiting each element until one declines.
static inline void VisitElements(UnkCollectionVisitor_l0a27e0* self, const UnkReflTypedPtr& obj) {
    const CCollectionType* collection = AsCollection(obj.type);
    UnkTunableReflValue begin(base::reflection::refl_EmptyValue());
    if (collection->m_iterator && collection->m_begin && obj.type == reinterpret_cast<const char*>(collection)) {
        fn_71000a2014(&begin, collection->m_iterator);
        reinterpret_cast<IteratorFn>(collection->m_begin)(obj.ptr, Storage(&begin));
    }
    const char* it_type = begin.type;
    void* it = Storage(&begin);
    UnkTunableReflValue end(base::reflection::refl_EmptyValue());
    if (collection->m_iterator && collection->m_end && obj.type == reinterpret_cast<const char*>(collection)) {
        fn_71000a2014(&end, collection->m_iterator);
        reinterpret_cast<IteratorFn>(collection->m_end)(obj.ptr, Storage(&end));
    }
    UnkReflTypedPtr last;
    fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(&last), &end);
    while (!fn_71000a1ed4(reinterpret_cast<const UnkCompared_0x18*>(&begin),
                          reinterpret_cast<const UnkCompared_0x18*>(&end))) {
        UnkReflTypedPtr element = Get(it_type, it);
        s32 result = VisitElement(self, element);
        Increment(it_type, it);
        if (result != 1)
            break;
    }
}

bool fn_71000a27e0(UnkCollectionVisitor_l0a27e0* self, const UnkReflTypedPtr* in) {
    char* type = in->type;
    void* ptr = in->ptr;
    UnkReflTypedPtr obj;
    obj.type = fn_710009dcd8(type, ptr);
    obj.ptr = ptr;
    AsCollection(obj.type);
    if (self->BeginCollection(&obj) == 1)
        VisitElements(self, obj);
    self->EndCollection(&obj);
    return true;
}
