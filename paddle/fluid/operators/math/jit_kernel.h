/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once
#include <functional>
#include <memory>  // for shared_ptr
#include <string>
#include <unordered_map>
#include "paddle/fluid/platform/macros.h"

// Note: Only support on CPU yet.
namespace paddle {
namespace operators {
namespace math {
namespace jitkernel {

class Kernel {
 public:
  Kernel() {}
  virtual ~Kernel() = default;

 private:
  DISABLE_COPY_AND_ASSIGN(Kernel);
};

class KernelPool {
 public:
  static KernelPool& Instance();

  template <typename Ker, typename... ARGS>
  const std::shared_ptr<Ker> Get(ARGS... args);

 private:
  KernelPool() = default;
  std::unordered_map<std::string, std::shared_ptr<Kernel>> kers_;

  DISABLE_COPY_AND_ASSIGN(KernelPool);
};

template <typename T>
class LSTMKernel : public Kernel {
 public:
  explicit LSTMKernel(int d, const std::string& act_gate,
                      const std::string& act_cand, const std::string& act_cell);

  void ComputeCtHt(T* gates, const T* ct_1, T* ct);
  void ComputeCtHt_NoC0H0(T* gates, const T* ct_1, T* ct);

 private:
  int d_;
  std::function<void(const int, const T *, T *)> act_gate_, act_cell_,
      act_cand_;
};

}  // namespace jitkernel
}  // namespace math
}  // namespace operators
}  // namespace paddle

#include "paddle/fluid/operators/math/jit_kernel_impl.h"
