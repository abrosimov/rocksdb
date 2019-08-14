//
// Created by kabrosimov on 12.08.19.
//

#ifndef ROCKSDB_USERKV_H
#define ROCKSDB_USERKV_H

#include "rocksdb/comparator.h"
#include "rocksdb/merge_operator.h"

namespace userkv {
	extern const rocksdb::Comparator *Uint64Comparator();
	std::shared_ptr<rocksdb::MergeOperator> CreateUserKVMergeOperator();
}

#endif //ROCKSDB_USERKV_H
