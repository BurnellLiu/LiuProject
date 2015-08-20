
#define _WIN32_DCOM

#include "LWMICoreManager.h"

#include <assert.h>
#include <stdlib.h>

#include <Windows.h>

#include <WbemIdl.h>
#include <comdef.h>
#pragma comment(lib, "WbemUuid.lib")

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")


#define LSAFE_RELEASE(p) do{if (p){p->Release(); p = 0;}}while(0)
#define LSAFE_DELARRAY(p) do{if (p){delete[] p; p = 0;}}while(0)
#define LSAFE_DELETE(p) do{if (p){delete p; p = 0;}}while(0)





namespace LWMI
{
    /// @brief COM初始化类
    /// 只支持单线程
    class LInitCom
    {
    public:
        LInitCom()
        {
            this->bSuccess = false;
            HRESULT hr = CoInitialize(NULL);
            if (hr == S_OK)
            {
                this->bSuccess = true;
            }
            else
            {
                this->bSuccess = false;
            }
        }

        ~LInitCom()
        {
            if (this->bSuccess)
            {
                CoUninitialize();
            }
        }
    private:
        bool bSuccess; ///< 标识是否初始化成功
    };

    LWMICoreManager::LWMICoreManager()
    {
        m_pWbemLocator = NULL;
        m_pWbemServices = NULL;
        m_pEnumObject = NULL;
        m_pObjectArray = NULL;
        m_objectCount = 0;

        m_pInitComObject = new LInitCom();
    }

    LWMICoreManager::~LWMICoreManager()
    {
        this->BaseCleanUp();
        LSAFE_DELETE(m_pInitComObject);
    }

    bool LWMICoreManager::BaseInit(const wchar_t* pNamespace)
    {
        bool bRet = false;
        HRESULT hr;

        this->BaseCleanUp();

        if (pNamespace == NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, 
            IID_IWbemLocator, (LPVOID*)&m_pWbemLocator);
        if (FAILED(hr)) 
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pWbemLocator->ConnectServer(_bstr_t(pNamespace), NULL, NULL, NULL, 
            0, NULL, NULL, &m_pWbemServices);
        if (FAILED(hr)) 
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = CoSetProxyBlanket(m_pWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
            RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
        if (FAILED(hr)) 
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        bRet = true;

SAFE_EXIT:
        if (!bRet)
        {
            this->BaseCleanUp();
        }
        return bRet;
    }

    bool LWMICoreManager::WQLQuery(const wchar_t* pQuery)
    {
        if (pQuery == NULL || m_pWbemServices == NULL)
            return false;

        for (int i = 0; i < m_objectCount; i ++)
        {
            LSAFE_RELEASE(m_pObjectArray[i]);
        }
        LSAFE_DELARRAY(m_pObjectArray);
        LSAFE_RELEASE(m_pEnumObject);

        HRESULT hr = m_pWbemServices->ExecQuery(_bstr_t(L"WQL"), _bstr_t(pQuery),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &m_pEnumObject);
        if (FAILED(hr)) return false;

        int maxSize = 5;
        m_pObjectArray = new IWbemClassObject*[maxSize];
        ZeroMemory(m_pObjectArray, maxSize * sizeof(IWbemClassObject*));

        ULONG uReturn = 0;
        while(true)
        {
            if (m_objectCount >= maxSize)
            {
                IWbemClassObject** pTempArray = new IWbemClassObject*[maxSize * 2];
                ZeroMemory(pTempArray, maxSize * 2 * sizeof(IWbemClassObject*));
                memcpy(pTempArray, m_pObjectArray, maxSize * sizeof(IWbemClassObject*));
                LSAFE_DELARRAY(m_pObjectArray);
                m_pObjectArray = pTempArray;
                pTempArray = NULL;
                maxSize = maxSize * 2;
            }
            hr = m_pEnumObject->Next(WBEM_INFINITE, 1, 
                &m_pObjectArray[m_objectCount], &uReturn);

            if(0 == uReturn)
            {
                break;
            }

            m_objectCount++;
        }

        return true;
    }

    int LWMICoreManager::GetObjectsCount()
    {
        return m_objectCount;
    }

    bool LWMICoreManager::GetStringProperty(int objectIndex, const wchar_t* pPropertyName, wstring& strProperty)
    {
        bool bRet = false;
        VARIANT vtProp;
        HRESULT hr;

        if(objectIndex < 0 || objectIndex >= m_objectCount)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        if (pPropertyName == NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        strProperty.clear();


        VariantInit(&vtProp);
        hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
        if (hr != WBEM_S_NO_ERROR)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if(vtProp.vt == VT_EMPTY||vtProp.vt == VT_NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        bRet = true;
        strProperty.assign(vtProp.bstrVal);


SAFE_EXIT:
        VariantClear(&vtProp);
        return bRet;
    }

    bool LWMICoreManager::GetUINT16Property(int objectIndex, const wchar_t* pPropertyName, LUINT16& ui16Property)
    {
        bool bRet = false;
        VARIANT vtProp;
        HRESULT hr;

        if(objectIndex <0 || objectIndex >= m_objectCount)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        if (pPropertyName == NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        VariantInit(&vtProp);
        hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
        if (hr != WBEM_S_NO_ERROR)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if(vtProp.vt == VT_EMPTY||vtProp.vt == VT_NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        bRet = true;
        ui16Property = vtProp.uiVal;

SAFE_EXIT:
        VariantClear(&vtProp);
        return bRet;
    }

    bool LWMICoreManager::GetUINT32Property(int objectIndex, const wchar_t* pPropertyName, LUINT& uiProperty)
    {
        bool bRet = false;
        VARIANT vtProp;
        HRESULT hr;

        if(objectIndex <0 || objectIndex >= m_objectCount)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        if (pPropertyName == NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        VariantInit(&vtProp);
        hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
        if (hr != WBEM_S_NO_ERROR)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if(vtProp.vt == VT_EMPTY||vtProp.vt == VT_NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        bRet = true;
        uiProperty = vtProp.uintVal;

SAFE_EXIT:
        VariantClear(&vtProp);
        return bRet;
    }

    bool LWMICoreManager::GetUINT64Property(int objectIndex, const wchar_t* pPropertyName, LUINT64& ui64Property)
    {
        bool bRet = false;
        VARIANT vtProp;
        HRESULT hr;

        if(objectIndex <0 || objectIndex >= m_objectCount)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        if (pPropertyName == NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }


        VariantInit(&vtProp);
        hr = m_pObjectArray[objectIndex]->Get(pPropertyName, 0, &vtProp, 0, 0);
        if (hr != WBEM_S_NO_ERROR)
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if(vtProp.vt == VT_EMPTY||vtProp.vt == VT_NULL)
        {
            bRet = false;
            goto SAFE_EXIT;
        }
        /*
        64位整数以字符串的方式保存在VARIANT中
        */
        bRet = true;
        ui64Property = _wcstoui64(vtProp.bstrVal, L'\0', 0);

SAFE_EXIT:
        VariantClear(&vtProp);
        return bRet;
    }

    void LWMICoreManager::BaseCleanUp()
    {
        for (int i = 0; i < m_objectCount; i ++)
        {
            LSAFE_RELEASE(m_pObjectArray[i]);
        }
        m_objectCount = 0;
        LSAFE_DELARRAY(m_pObjectArray);
        LSAFE_RELEASE(m_pEnumObject);
        LSAFE_RELEASE(m_pWbemServices);
        LSAFE_RELEASE(m_pWbemLocator);

    }
}
