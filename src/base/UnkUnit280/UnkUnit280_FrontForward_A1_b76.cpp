// Unit 280 of tools/tu_map.py: forwards an argument to the first item of a pointer vector, growing
// the vector to one (null) item first when it is empty (UnkVector::GrowAt(0)). fn_71001fcf54 calls
// fn_7100202cf0, fn_71001fd330 calls fn_7100202d24 (unit 281). Names and argument types are
// placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "base/reflection.h"
#include "types.h"

struct UnkUnit280ItemA1;

void fn_7100202cf0(UnkUnit280ItemA1* item, void* arg);
struct UnkItem;

void fn_71001fcf54(base::UnkVector<UnkUnit280ItemA1*>* items, void* arg) {
    fn_7100202cf0(items->GrowAt(0), arg);
}

void fn_71001fd330(base::UnkVector<UnkUnit280ItemA1*>* items, void* arg) {
    fn_7100202d24(reinterpret_cast<UnkItem*>(items->GrowAt(0)), arg);
}
