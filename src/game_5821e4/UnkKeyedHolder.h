#pragma once

// A holder that is empty (kind 0), holds a key and a record (1), or refers to a key and a record
// held elsewhere (2). Its move constructor turns both non-empty kinds into an owned value,
// moving the record (fn_710061bc74 and others in this folder).

#include "types.h"

#include <new>
#include <utility>

template <class K, class R>
struct UnkKeyedValue {
    K key;
    R record;

    UnkKeyedValue(const K& key, R&& record) : key(key), record(std::move(record)) {}
    UnkKeyedValue(UnkKeyedValue&& other) = default;
};

template <class K, class R>
struct UnkKeyedRefs {
    K* key;
    R* record;
};

template <class K, class R>
struct UnkKeyedHolder {
    union {
        UnkKeyedValue<K, R> value;
        UnkKeyedRefs<K, R> refs;
    };
    u32 kind;

    UnkKeyedHolder(UnkKeyedHolder&& other) {
        kind = 0;
        switch (other.kind) {
        case 1:
            kind = 1;
            new (&value) UnkKeyedValue<K, R>(std::move(other.value));
            break;
        case 2:
            kind = 1;
            new (&value) UnkKeyedValue<K, R>(*other.refs.key, std::move(*other.refs.record));
            break;
        }
    }
};
