// base::reflection::CType (tools/tu_map.py unit 81): runs the class set-up function (+0x70) of every type in
// a class's parent chain on the class, the root first. Names and layouts are placeholders.
#include <string.h>

#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

namespace {

// A vector with its elements in place: a count and room for N elements, which reading past the end
// grows (the new elements zeroed).
template <class T, u32 N>
struct UnkInPlaceVector {
    u32 m_size = 0;
    alignas(4) u8 m_storage[N * sizeof(T)];

    T* data() { return reinterpret_cast<T*>(m_storage); }
    u32 size() const { return m_size; }
    void push_back(T value) {
        data()[m_size] = value;
        ++m_size;
    }
    T& operator[](u32 i) {
        if (i + 1 > m_size) {
            memset(data() + m_size, 0, (i + 1 - m_size) * sizeof(T));
            m_size = i + 1;
        }
        return data()[i];
    }
};

}  // namespace

void fn_710009ce10(CClass* self) {
    UnkInPlaceVector<const CType*, 10> chain;
    if (!self)
        return;
    for (const CType* type = self; type; type = type->m_parent)
        chain.push_back(type);
    for (s32 i = chain.size() - 1; i >= 0; --i) {
        const CType* type = chain[i];
        if (type->m_fn_0x70)
            type->m_fn_0x70(self);
    }
}
