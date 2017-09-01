/*
Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __KERNELS_H__
#define __KERNELS_H__

//////////////////////////////////////////////////////////////////////
// SHARED_PUBLIC - shared sybols for export
// STITCH_API_ENTRY - export API symbols
#if _WIN32
#define SHARED_PUBLIC extern "C" __declspec(dllexport)
#else
#define SHARED_PUBLIC extern "C" __attribute__ ((visibility ("default")))
#endif

//////////////////////////////////////////////////////////////////////
// common header files
#include <VX/vx.h>
#include <VX/vx_khr_nn.h>
#include <vx_ext_amd.h>
#include <miopen/miopen.h>
#include <iostream>
#include <string.h>
#if __APPLE__
#include <opencl.h>
#else
#include <CL/cl.h>
#endif

//////////////////////////////////////////////////////////////////////
//! \brief The macro for error checking from OpenVX status.
#define ERROR_CHECK_STATUS(call) { vx_status status = (call); if(status != VX_SUCCESS){ vxAddLogEntry(NULL, status, "ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status, __LINE__); return status; }}
//! \brief The macro for error checking from OpenVX object.
#define ERROR_CHECK_OBJECT(obj)  { vx_status status = vxGetStatus((vx_reference)(obj)); if(status != VX_SUCCESS){ vxAddLogEntry((vx_reference)(obj), status, "ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status, __LINE__); return status; }}

#ifndef ERROR_CHECK_MIOPEN_STATUS
#define ERROR_CHECK_MIOPEN_STATUS(call) if(call) { \
    std::cerr << "ERROR: fatal error occured at " __FILE__ << "#" << __LINE__ << std::endl; \
    exit(1); \
    }
#endif

// Debug Print Dims : disabled unless enabled explicitly by setting DEBUG_PRINT_DIMS=1
#ifndef ENABLE_DEBUG_PRINT_DIMS
#define ENABLE_DEBUG_PRINT_DIMS 0
#endif

//////////////////////////////////////////////////////////////////////
//! user kernels
enum nn_additional_library
{
    NN_EXTENSION_LIBRARY = 1,
};
enum user_kernel_e
{
    VX_KERNEL_BATCH_NORMALISATION_LAYER_AMD  = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x001,
    VX_KERNEL_ARGMAX_LAYER_AMD               = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x002,
    VX_KERNEL_CONVERT_IMAGE_TO_TENSOR_AMD    = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x003,
    VX_KERNEL_CONVERT_TENSOR_TO_IMAGE_AMD    = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x004,
    VX_KERNEL_ELEMENTWISE_LAYER_AMD          = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x005,
    VX_KERNEL_CONCAT_LAYER_AMD               = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x006,
    VX_KERNEL_SLICE_LAYER_AMD                = VX_KERNEL_BASE(VX_ID_AMD, NN_EXTENSION_LIBRARY) + 0x007,
};

//////////////////////////////////////////////////////////////////////
//! \brief Common data shared across all nodes in a graph
struct NeuralNetworkCommonHandle {
    int count;
    miopenHandle_t  miopen_handle;
    cl_command_queue cmdq;
    bool exhaustiveSearch;
};

//////////////////////////////////////////////////////////////////////
//! \brief The utility functions
vx_node createNode(vx_graph graph, vx_enum kernelEnum, vx_reference params[], vx_uint32 num);
vx_status createGraphHandle(vx_node node, NeuralNetworkCommonHandle ** pHandle);
vx_status releaseGraphHandle(vx_node node, NeuralNetworkCommonHandle * handle);

//////////////////////////////////////////////////////////////////////
//! \brief The kernel publish functions
vx_status publishConvolutionLayer(vx_context context);
vx_status publishFullyConnectedLayer(vx_context context);
vx_status publishPoolingLayer(vx_context context);
vx_status publishSoftmaxLayer(vx_context context);
vx_status publishNormalizationLayer(vx_context context);
vx_status publishActivationLayer(vx_context context);
vx_status publishROIPoolingLayer(vx_context context);
vx_status publishDeconvolutionLayer(vx_context context);
vx_status publishElementwiseLayer(vx_context context);
vx_status publishBatchNormalizationLayer(vx_context context);
vx_status publishArgmaxLayer(vx_context context);
vx_status publishConcatLayer(vx_context context);
vx_status publishSliceLayer(vx_context context);
vx_status publishImageToTensorConvert(vx_context context);
vx_status publishTensorToImageConvert(vx_context context);
vx_status publishTensorAdd(vx_context context);
vx_status publishTensorSubtraction(vx_context context);
vx_status publishTensorMultiply(vx_context context);
vx_status publishTensorConvertDepth(vx_context context);


//////////////////////////////////////////////////////////////////////
//! \brief The module entry point for publishing/unpublishing kernels
SHARED_PUBLIC vx_status VX_API_CALL vxPublishKernels(vx_context context);
SHARED_PUBLIC vx_status VX_API_CALL vxUnpublishKernels(vx_context context);

#endif //__KERNELS_H__
