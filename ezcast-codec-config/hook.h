#pragma once

#include <Windows.h>

#include "PolyHook.hpp"
#include "logger.h"

namespace {
	template <class CLASS_TYPE, class FUNC_TYPE>
	HRESULT hookVirtualFunction(CLASS_TYPE *pInstance, int vFuncIndex, LPVOID hookFunc, FUNC_TYPE *originalFunc, std::shared_ptr<PLH::VFuncDetour> VFuncDetour_Ex) {
		if (VFuncDetour_Ex->GetOriginal<FUNC_TYPE>() != NULL) {
			*originalFunc = VFuncDetour_Ex->GetOriginal<FUNC_TYPE>();
			return S_OK;
		}
		VFuncDetour_Ex->SetupHook(*(BYTE***)pInstance, vFuncIndex, (BYTE*)hookFunc);
		if (!VFuncDetour_Ex->Hook()) {
			LOG(LL_ERR, VFuncDetour_Ex->GetLastError().GetString());
			return E_FAIL;
		}
		*originalFunc = VFuncDetour_Ex->GetOriginal<FUNC_TYPE>();
		return S_OK;
	}
	
	template <class FUNC_TYPE>
	HRESULT hookNamedFunction(LPCSTR dllname, LPCSTR funcName, LPVOID hookFunc, FUNC_TYPE *originalFunc, std::shared_ptr<PLH::IATHook> IATHook_ex) {
		if (IATHook_ex->GetOriginal<FUNC_TYPE>() != NULL) {
			*originalFunc = IATHook_ex->GetOriginal<FUNC_TYPE>();
			return S_OK;
		}
		IATHook_ex->SetupHook(dllname, funcName, (BYTE*)hookFunc);
		if (!IATHook_ex->Hook()) {
			LOG(LL_ERR, IATHook_ex->GetLastError().GetString());
			return E_FAIL;
		}
		*originalFunc = IATHook_ex->GetOriginal<FUNC_TYPE>();
		return S_OK;
	}

	template <class FUNC_TYPE>
	HRESULT hookX86Function(LPVOID func, LPVOID hookFunc, FUNC_TYPE *originalFunc, std::shared_ptr<PLH::X86Detour> X86Detour_ex) {
		/*if (X86Detour_ex->GetOriginal<FUNC_TYPE>() != NULL) {
			*originalFunc = X86Detour_ex->GetOriginal<FUNC_TYPE>();
			return E_ABORT;
		}*/
		X86Detour_ex->SetupHook((BYTE*)func, (BYTE*)hookFunc);
		if (!X86Detour_ex->Hook()) {
			LOG(LL_ERR, X86Detour_ex->GetLastError().GetString());
			return E_FAIL;
		}
		*originalFunc = X86Detour_ex->GetOriginal<FUNC_TYPE>();
		return S_OK;
	}
}