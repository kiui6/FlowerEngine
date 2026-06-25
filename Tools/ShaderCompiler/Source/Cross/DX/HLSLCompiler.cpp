#include "HLSLCompiler.h"

#include <dxcapi.h>
#include <d3d12shader.h>
#include <comdef.h>
#include <wrl/client.h>

#include <stdexcept>

using Microsoft::WRL::ComPtr;

std::vector<uint8_t> CompileHLSLToDXBC(const std::string &hlslSource)
{
    const std::wstring& entryPoint = L"main",
    const std::wstring& targetProfile = L"ps_5_0";
    
    ComPtr<IDxcCompiler3> compiler;
    ComPtr<IDxcUtils> utils;
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));

    DxcBuffer sourceBuffer;
    sourceBuffer.Ptr = hlslSource.data();
    sourceBuffer.Size = hlslSource.size();
    sourceBuffer.Encoding = 0;

    std::vector<LPCWSTR> args = {
        L"-T", targetProfile.c_str(),
        L"-E", entryPoint.c_str(),
        L"-Zpc",
        L"-Vd"
    };

    // 4. Compile
    ComPtr<IDxcResult> result;
    HRESULT hr = compiler->Compile(
        &sourceBuffer,
        args.data(),
        (UINT32)args.size(),
        nullptr,
        IID_PPV_ARGS(&result)
    );

    if (FAILED(hr)) {
        throw std::runtime_error("DXC Compile failed with HRESULT: " + std::to_string(hr));
    }

    ComPtr<IDxcBlobUtf8> errors;
    hr = result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
    if (SUCCEEDED(hr) && errors && errors->GetStringLength() > 0 && result->GetOutput(DXC_OUT_OBJECT) == NULL) {
        std::string errorMsg((const char*)errors->GetBufferPointer(), errors->GetStringLength());
        throw std::runtime_error("DXC Errors:\n" + errorMsg);
    }

    ComPtr<IDxcBlob> bytecodeBlob;
    hr = result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&bytecodeBlob), nullptr);
    if (FAILED(hr) || !bytecodeBlob) {
        throw std::runtime_error("Failed to retrieve compiled DXBC bytecode.");
    }

    std::vector<uint8_t> dxbcBytecode(
        (uint8_t*)bytecodeBlob->GetBufferPointer(),
        (uint8_t*)bytecodeBlob->GetBufferPointer() + bytecodeBlob->GetBufferSize()
    );

    return dxbcBytecode;
}