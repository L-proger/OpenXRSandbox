#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <stdexcept>

#pragma comment(lib, "d3d11.lib")

class RendererD3D11 {
public:
    RendererD3D11() {
        createDevice();

    }
    Microsoft::WRL::ComPtr<ID3D11Device> getDevice() {
        return _device;
    }
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> getImmediateContext() {
        return _immediateContext;
    }
private:
    void createDevice() {
         // Create the device and device context objects
        HRESULT hr = D3D11CreateDevice( nullptr,D3D_DRIVER_TYPE_HARDWARE, nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &_device,
            nullptr,
            &_immediateContext);

        if(hr != S_OK){
            throw std::runtime_error("Failed to create device");
        }
    }

    Microsoft::WRL::ComPtr<ID3D11Device> _device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> _immediateContext;
};