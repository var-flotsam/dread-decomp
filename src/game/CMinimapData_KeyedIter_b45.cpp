#include "../base/reflection/UnkTempIterValue_b44.h"
#include "types.h"

struct UnkNamedIds;
void fn_71009ba4ac(UnkNamedIds* self);
struct UnkIdWordList;
void fn_7100b480a0(UnkIdWordList* self);
struct UnkIdsArraysRecord;
void fn_7100e4d63c(UnkIdsArraysRecord* self);
struct UnkStrIdTriple;
void fn_7100c896a4(UnkStrIdTriple* self);

struct UnkKeyX {
    u64 a;
    ~UnkKeyX() { fn_71009ba4ac(reinterpret_cast<UnkNamedIds*>(this)); }
};
struct UnkKeyY {
    u64 a;
    ~UnkKeyY() { fn_7100b480a0(reinterpret_cast<UnkIdWordList*>(this)); }
};
struct UnkKeyZ {
    u64 a;
    ~UnkKeyZ() { fn_7100e4d63c(reinterpret_cast<UnkIdsArraysRecord*>(this)); }
};
struct UnkKeyW {
    u64 a;
    ~UnkKeyW() { fn_7100c896a4(reinterpret_cast<UnkStrIdTriple*>(this)); }
};
template <int N>
struct Blob { u64 w[N]; };

template <class K, class T>
struct UnkArr {
    T* values;
    K* keys;
    u32 count;
};

#define BEGIN_END(BEGIN, END, K, T)                                                          \
    void BEGIN(UnkArr<K, T>* self, UnkTempIterValue<T, K>* out) {                            \
        *out = UnkTempIterValue<T, K>(self->keys, self->values);                             \
    }                                                                                        \
    void END(UnkArr<K, T>* self, UnkTempIterValue<T, K>* out) {                              \
        *out = UnkTempIterValue<T, K>(self->keys + self->count, self->values + self->count); \
    }

BEGIN_END(fn_7100e5e1e0, fn_7100e5e244, UnkKeyX, Blob<5>)
BEGIN_END(fn_7100eb1d08, fn_7100eb1d6c, UnkKeyY, Blob<5>)
BEGIN_END(fn_7100e61988, fn_7100e619ec, UnkKeyZ, Blob<16>)
BEGIN_END(fn_7100c90e94, fn_7100c90ef8, UnkKeyW, Blob<3>)
