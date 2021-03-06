//*****************************************************************************
// Copyright 2017-2019 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#pragma once

#include <map>
#include <memory>

#include <CPP/engine.hpp>
#include <CPP/network.hpp>

#include "ngraph/runtime/backend.hpp"

namespace ngraph
{
    namespace runtime
    {
        namespace intelgpu
        {
            class IntelGPUBackend;
            class IntelGPUExecutable;
        }
    }
}

class ngraph::runtime::intelgpu::IntelGPUBackend : public runtime::Backend
{
public:
    IntelGPUBackend();
    std::shared_ptr<ngraph::runtime::Tensor>
        create_tensor(const ngraph::element::Type& element_type,
                      const Shape& shape,
                      void* memory_pointer) override;

    std::shared_ptr<ngraph::runtime::Tensor>
        create_tensor(const ngraph::element::Type& element_type, const Shape& shape) override;

    std::shared_ptr<runtime::Executable> compile(std::shared_ptr<Function> func,
                                                 bool enable_timing = false) override;
    void remove_compiled_function(std::shared_ptr<runtime::Executable> exec) override;

    bool is_supported_property(const Property prop) const override;

private:
    std::shared_ptr<cldnn::engine> cldnn_engine;
    std::map<std::shared_ptr<Function>, std::shared_ptr<runtime::Executable>> cldnn_networks;

    bool m_profile_enable = false;
    long m_profile_lines_limit_count = 10;
    bool m_dump_graph_enable = false;
    bool m_cldnn_graph_optimize = true;
    bool m_cldnn_dump_enable = false;
    bool m_function_cache_disabled = false;
    bool m_disable_backend_optimizations = false;
    std::string m_cldnn_dump_dir = std::string("intelgpu_codegen");
};

class ngraph::runtime::intelgpu::IntelGPUExecutable : public runtime::Executable
{
public:
    IntelGPUExecutable(std::shared_ptr<Function> func,
                       std::shared_ptr<cldnn::network> network,
                       bool enable_timing,
                       bool enable_profile,
                       double compilation_time,
                       double consumed_memory,
                       size_t profile_lines_limit_count);

    bool call(const std::vector<std::shared_ptr<runtime::Tensor>>& outputs,
              const std::vector<std::shared_ptr<runtime::Tensor>>& inputs) override;

    std::vector<PerformanceCounter> get_performance_data() const override;

private:
    std::shared_ptr<Function> m_function;
    std::shared_ptr<cldnn::network> m_cldnn_network = nullptr;
    bool m_performance_counters_enabled = false;
    bool m_profile_enable = false;
    double m_compilation_time = 0.0;
    double m_consumed_memory = 0.0;
    long m_profile_lines_limit_count = 10;
    std::string delim = std::string(":");

    // Statistic related things
    void print_call_performance(const std::shared_ptr<cldnn::network> network,
                                const std::shared_ptr<Function> func,
                                double time_compile,
                                double time_call,
                                double mem_compilation_consumed,
                                double mem_call_consumed,
                                double mem_current) const;
};
