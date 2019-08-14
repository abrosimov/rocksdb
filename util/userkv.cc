//
// Created by kabrosimov on 12.08.19.
//

#include "util/coding.h"
#include "rocksdb/userkv.h"

namespace userkv {
	class Uint64ComparatorImpl : public rocksdb::Comparator {
	public:
		Uint64ComparatorImpl() = default;

		const char *Name() const override { return "userkv::UInt64Comparator"; }

		int Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const override {
			assert(a.size() == sizeof(uint64_t) && b.size() == sizeof(uint64_t));
			const auto *left = reinterpret_cast<const uint64_t *>(a.data());
			const auto *right = reinterpret_cast<const uint64_t *>(b.data());
			uint64_t leftValue;
			uint64_t rightValue;
			rocksdb::GetUnaligned(left, &leftValue);
			rocksdb::GetUnaligned(right, &rightValue);
			if (leftValue == rightValue) {
				return 0;
			} else if (leftValue < rightValue) {
				return -1;
			} else {
				return 1;
			}
		}

		void FindShortestSeparator(std::string * /*start*/, const rocksdb::Slice & /*limit*/) const override { }

		void FindShortSuccessor(std::string * /*key*/) const override { }
	};

	class UserKVMergeOperator : public rocksdb::AssociativeMergeOperator {
	public:
		bool Merge(const rocksdb::Slice &/*key*/, const rocksdb::Slice */*existing_value*/, const rocksdb::Slice &value, std::string *new_value, rocksdb::Logger */*logger*/) const override {

			if (new_value == nullptr) {
				return false;
			}

			new_value->assign(value.data(), value.size());
			return true;
		}

		const char *Name() const override {
			return "userkv::MergeOperator";
		}
	};

	const rocksdb::Comparator *Uint64Comparator() {
		static userkv::Uint64ComparatorImpl uint64Comparator;
		return &uint64Comparator;
	}

	std::shared_ptr<rocksdb::MergeOperator> CreateUserKVMergeOperator() {
		return std::make_shared<UserKVMergeOperator>();
	}
}
