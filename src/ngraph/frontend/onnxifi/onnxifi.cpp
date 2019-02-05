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

#include <cstddef>
#include <cstdint>
#include <onnxifi.h>
#include <stdexcept>

#include "backend_manager.hpp"
#include "event_manager.hpp"
#include "exceptions.hpp"

using namespace ngraph::onnxifi;

extern "C" {

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI
    onnxGetBackendIDs(onnxBackendID* backendIDs, std::size_t* numBackends)
{
    try
    {
        BackendManager::get_backend_ids(backendIDs, numBackends);
        return ONNXIFI_STATUS_SUCCESS;
    }
    catch (const status::runtime& e)
    {
        return e.get_status();
    }
    catch (const std::bad_alloc&)
    {
        return ONNXIFI_STATUS_NO_SYSTEM_MEMORY;
    }
    catch (...)
    {
        return ONNXIFI_STATUS_INTERNAL_ERROR;
    }
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI
    onnxReleaseBackendID(onnxBackendID backendID)
{
    try
    {
        BackendManager::release_backend_id(backendID);
        return ONNXIFI_STATUS_SUCCESS;
    }
    catch (const status::runtime& e)
    {
        return e.get_status();
    }
    catch (...)
    {
        return ONNXIFI_STATUS_INTERNAL_ERROR;
    }
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxGetBackendInfo(
    onnxBackendID backendID, onnxBackendInfo infoType, void* infoValue, std::size_t* infoValueSize)
{
    try
    {
        BackendManager::get_backend_info(backendID, infoType, infoValue, infoValueSize);
        return ONNXIFI_STATUS_SUCCESS;
    }
    catch (const status::runtime& e)
    {
        return e.get_status();
    }
    catch (const std::bad_alloc&)
    {
        return ONNXIFI_STATUS_NO_SYSTEM_MEMORY;
    }
    catch (const std::out_of_range&)
    {
        return ONNXIFI_STATUS_INVALID_ID;
    }
    catch (...)
    {
        return ONNXIFI_STATUS_INTERNAL_ERROR;
    }
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxGetBackendCompatibility(
    onnxBackendID backendID, std::size_t onnxModelSize, const void* onnxModel)
{
    return ONNXIFI_STATUS_BACKEND_UNAVAILABLE;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxInitBackend(
    onnxBackendID backendID, const uint64_t* /* auxPropertiesList */, onnxBackend* backend)
{
    try
    {
        if (backend != nullptr)
        {
            *backend = nullptr;
        }
        // The 'auxPropertiesList' is not used in this version of the nGraph ONNXIFI backend.
        BackendManager::init_backend(backendID, backend);
        return ONNXIFI_STATUS_SUCCESS;
    }
    catch (const status::runtime& e)
    {
        return e.get_status();
    }
    catch (const std::out_of_range&)
    {
        return ONNXIFI_STATUS_INVALID_ID;
    }
    catch (const std::bad_alloc&)
    {
        return ONNXIFI_STATUS_NO_SYSTEM_MEMORY;
    }
    catch (...)
    {
        return ONNXIFI_STATUS_INTERNAL_ERROR;
    }
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxReleaseBackend(onnxBackend backend)
{
    try
    {
        BackendManager::release_backend(backend);
        return ONNXIFI_STATUS_SUCCESS;
    }
    catch (const status::runtime& e)
    {
        return e.get_status();
    }
    catch (const std::out_of_range&)
    {
        return ONNXIFI_STATUS_INVALID_BACKEND;
    }
    catch (...)
    {
        return ONNXIFI_STATUS_INTERNAL_ERROR;
    }
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxInitEvent(onnxBackend backend,
                                                                         onnxEvent* event)
{
    try
    {
        if (event != nullptr)
        {
            *event = nullptr;
        }
        EventManager::init_event(backend, event);
        return ONNXIFI_STATUS_SUCCESS;
    }
    catch (const status::runtime& e)
    {
        return e.get_status();
    }
    catch (const std::bad_alloc&)
    {
        return ONNXIFI_STATUS_NO_SYSTEM_MEMORY;
    }
    catch (...)
    {
        return ONNXIFI_STATUS_INTERNAL_ERROR;
    }
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxSignalEvent(onnxEvent event)
{
    return ONNXIFI_STATUS_BACKEND_UNAVAILABLE;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxWaitEvent(onnxEvent event)
{
    return ONNXIFI_STATUS_BACKEND_UNAVAILABLE;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxReleaseEvent(onnxEvent event)
{
    return ONNXIFI_STATUS_INTERNAL_ERROR;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI
    onnxInitGraph(onnxBackend backend,
                  const uint64_t* auxPropertiesList,
                  std::size_t onnxModelSize,
                  const void* onnxModel,
                  uint32_t weightsCount,
                  const onnxTensorDescriptorV1* weightDescriptors,
                  onnxGraph* graph)
{
    return ONNXIFI_STATUS_BACKEND_UNAVAILABLE;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI
    onnxSetGraphIO(onnxGraph graph,
                   std::uint32_t inputsCount,
                   const onnxTensorDescriptorV1* inputDescriptors,
                   std::uint32_t outputsCount,
                   const onnxTensorDescriptorV1* outputDescriptors)
{
    return ONNXIFI_STATUS_BACKEND_UNAVAILABLE;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxRunGraph(
    onnxGraph graph, const onnxMemoryFenceV1* inputFence, onnxMemoryFenceV1* outputFence)
{
    return ONNXIFI_STATUS_BACKEND_UNAVAILABLE;
}

ONNXIFI_PUBLIC ONNXIFI_CHECK_RESULT onnxStatus ONNXIFI_ABI onnxReleaseGraph(onnxGraph graph)
{
    return ONNXIFI_STATUS_INTERNAL_ERROR;
}

} /* extern "C" */
