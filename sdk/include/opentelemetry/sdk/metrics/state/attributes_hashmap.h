// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef ENABLE_METRICS_PREVIEW
#  include "opentelemetry/common/spin_lock_mutex.h"
#  include "opentelemetry/nostd/function_ref.h"
#  include "opentelemetry/version.h"

#  include <functional>
#  include <memory>
#  include <mutex>
#  include <unordered_map>

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{
typedef size_t hashcode_t;

template <class KeyType, class ValueType, size_t(HashGenerator)(const KeyType &)>
class AttributesHashMap
{
public:
  /**
   * @return the pointer to value for given key, nullptr if not present.
   *
   */

  ValueType *Get(const KeyType &attributes) const
  {
    auto hashcode = HashGenerator(attributes);
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    auto it = value_map_.find(hashcode);
    if (it != value_map_.end())
    {
      return it->second.get();
    }
    return nullptr;
  }

  /**
   * @return check if key is present in hash
   *
   */
  bool Has(const KeyType &attributes) const
  {
    auto hashcode = HashGenerator(attributes);
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    return (value_map_.find(hashcode) == value_map_.end()) ? false : true;
  }

  /**
   * @return the pointer to value for given key if present.
   * If not present, it uses the provided callback to generate
   * value and store in the hash
   */

  ValueType *GetOrSetDefault(const KeyType &attributes,
                             std::function<std::unique_ptr<ValueType>()> value_callback)
  {
    auto hashcode = HashGenerator(attributes);
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    auto it = value_map_.find(hashcode);
    if (it != value_map_.end())
    {
      return it->second.get();
    }

    if (key_map_.find(hashcode) == key_map_.end())
    {
      key_map_[hashcode] = attributes;
    }
    value_map_[hashcode] = std::move(value_callback());
    return value_map_[hashcode].get();
  }

  /**
   * Set the value for given key, overwriting the value if already present
   */

  void Set(const KeyType &attributes, std::unique_ptr<ValueType> value)
  {
    auto hashcode = HashGenerator(attributes);
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    if (key_map_.find(hashcode) == key_map_.end())
    {
      key_map_[hashcode] = attributes;
    }
    value_map_[hashcode] = std::move(value);
  }

  /**
   * Iterate the hash to yield key and value stored in hash.
   */

  bool GetAllEnteries(
      nostd::function_ref<bool(hashcode_t, const KeyType &, ValueType &)> callback) const
  {
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    for (auto &kv : value_map_)
    {
      auto hashcode = kv.first;
      auto it       = key_map_.find(hashcode);
      if (it != key_map_.end())
      {
        if (!callback(hashcode, it->second, *(kv.second.get())))
        {
          return false;  // callback is not prepared to consume data
        }
      }
      else
      {
        return false;  // shouldn't reach here
      }
    }
    return true;
  }

  /**
   * Return the size of hash.
   */

  size_t Size()
  {
    std::lock_guard<opentelemetry::common::SpinLockMutex> guard(GetLock());
    return key_map_.size();
  }

private:
  std::unordered_map<hashcode_t, KeyType> key_map_;
  std::unordered_map<hashcode_t, std::unique_ptr<ValueType>> value_map_;

  static opentelemetry::common::SpinLockMutex &GetLock() noexcept
  {
    static opentelemetry::common::SpinLockMutex lock;
    return lock;
  }
};
}  // namespace metrics

}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
#endif
