#pragma once

#define XR_USE_GRAPHICS_API_D3D11

#include <openxr\openxr.h>
//#include <windows.h>
#include <d3d11.h>
#include <openxr\openxr_platform.h>
#include <vector>
#include <string>
#include <iostream>

#include "RendererD3D11.h"

//XR_KHR_D3D11_enable - ext

class OpenXRContext {
public:

    OpenXRContext(Microsoft::WRL::ComPtr<ID3D11Device> device) {
        enumerateApiLayers();
        enumerateExtensions();
        useExtension(XR_KHR_D3D11_ENABLE_EXTENSION_NAME);

        if(!createInstance()){
            std::cout << "Failed to create instance" << std::endl;
            return;
        }

        if(!getSystem()){
            std::cout << "Failed to getSystem" << std::endl;
            return;
        }

        if(!createSession(device)){
            std::cout << "Failed to createSession" << std::endl;
            return;
        }
    }

    bool useExtension(const char* sExtName)
	{
		for (const auto& rExt : _supportedExtensions){
			if (strcmp(rExt.extensionName, sExtName) == 0){
				_requiredExtensions.push_back(sExtName);
				return true;
			}
        }

		return false;
	}

    bool createSession(Microsoft::WRL::ComPtr<ID3D11Device> device)
	{
		// Graphics requirements
		XrGraphicsRequirementsD3D11KHR req{ XR_TYPE_GRAPHICS_REQUIREMENTS_D3D11_KHR };

		// Magic code block....
		// link error if call xrGetOpenGLGraphicsRequirementsKHR() directlly
		PFN_xrGetD3D11GraphicsRequirementsKHR func;
		if (check(xrGetInstanceProcAddr(_xrInstance, "xrGetD3D11GraphicsRequirementsKHR", (PFN_xrVoidFunction*)&func), "xrGetInstanceProcAddr"))
		{
			if (check(func(_xrInstance, _xrSystem, &req), "PFN_xrGetD3D11GraphicsRequirementsKHR"))
			{
                ID3D11Device* devicePtrRaw = device.Get();
				XrGraphicsBindingD3D11KHR gbD3D11{ XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR , nullptr, devicePtrRaw};

				XrSessionCreateInfo infoSession{ XR_TYPE_SESSION_CREATE_INFO, &gbD3D11, 0, _xrSystem };
				return check(xrCreateSession(_xrInstance, &infoSession, &_xrSession), "xrCreateSession");
			}
		}
		return false;
	}

    void enumerateApiLayers()
	{
		uint32_t uAPILayerNum = 0;
		if (check(xrEnumerateApiLayerProperties(0, &uAPILayerNum, nullptr), "enumerateApiLayers") && uAPILayerNum > 0)
		{
			_supportedApiLayers.resize(uAPILayerNum, { XR_TYPE_API_LAYER_PROPERTIES });
			check(xrEnumerateApiLayerProperties(uAPILayerNum, &uAPILayerNum, _supportedApiLayers.data()), "enumerateApiLayers2");
		}
	}
    void enumerateExtensions()
	{
		uint32_t uExtensionNum = 0;
		if (check(xrEnumerateInstanceExtensionProperties(nullptr, 0, &uExtensionNum, nullptr), "enumerateExtensions-1") && uExtensionNum > 0)
		{
			_supportedExtensions.resize(uExtensionNum, { XR_TYPE_EXTENSION_PROPERTIES });
			check(xrEnumerateInstanceExtensionProperties(nullptr, uExtensionNum, &uExtensionNum, _supportedExtensions.data()), "enumerateExtensions2");
		}
	}

    bool createInstance()
	{
		XrInstanceCreateInfo infoCreate;
		infoCreate.type = XR_TYPE_INSTANCE_CREATE_INFO;
		infoCreate.next = nullptr;
		infoCreate.createFlags = 0;
		infoCreate.applicationInfo = { "OpenXRSandbox", 1, "NoEngine", 1, XR_CURRENT_API_VERSION };
		infoCreate.enabledApiLayerCount = 0;
		infoCreate.enabledApiLayerNames = {};
		infoCreate.enabledExtensionCount = (uint32_t)_requiredExtensions.size();
		infoCreate.enabledExtensionNames = _requiredExtensions.data();

		return check(xrCreateInstance(&infoCreate, &_xrInstance), "xrCreateInstance");
	}

    bool getSystem()
	{
		XrSystemGetInfo infoSysId{ XR_TYPE_SYSTEM_GET_INFO, nullptr,XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY };
		return check(xrGetSystem(_xrInstance, &infoSysId, &_xrSystem), "xrGetSystem");
	}

private:

    bool check(const XrResult& rs, const std::string& sExtMsg)
	{
		if (rs == XR_SUCCESS)
			return true;

		char sMsg[XR_MAX_RESULT_STRING_SIZE];
		if (xrResultToString(_xrInstance, rs, sMsg) == XR_SUCCESS)
			std::cout << "Error: " << sMsg << "\n  " << sExtMsg << std::endl;
		else
			std::cout << "Error: " << rs << "\n  " << sExtMsg << std::endl;
		return false;
	}

    XrInstance	_xrInstance;
    XrSystemId	_xrSystem = XR_NULL_SYSTEM_ID;
	XrSession	_xrSession;

    std::vector<XrApiLayerProperties> _supportedApiLayers;
	std::vector<XrExtensionProperties> _supportedExtensions;
    std::vector<const char*> _requiredExtensions;
};
