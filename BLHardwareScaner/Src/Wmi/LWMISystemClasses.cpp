#include "LWMISystemClasses.h"

#include "LWMICoreManager.h"

namespace LWMI
{
    LComputerSystemManager::LComputerSystemManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_ComputerSystem");
    }
    LComputerSystemManager::~LComputerSystemManager()
    {
        delete m_pWMICoreManager;
    }

    int LComputerSystemManager::GetComputerSystemCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LComputerSystemManager::GetComputerSystemModel(IN int index, OUT wstring& model)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Model", model);
    }

    bool LComputerSystemManager::GetComputerSystemManufacturer(IN int index, OUT wstring& manufacturer)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Manufacturer", manufacturer);
    }

    bool LComputerSystemManager::GetComputerSystemPCType(IN int index, OUT LCOMPUTER_SYSTEM_TYPE& type)
    {
        type = COMPUTER_SYSTEM_UNKNOWN;

        LUINT16 u16Type;
        bool bRet = m_pWMICoreManager->GetUINT16Property(index, L"PCSystemType", u16Type);
        LUINT16 u16TypeEx;
        bool bRetEx = m_pWMICoreManager->GetUINT16Property(index, L"PCSystemTypeEx", u16TypeEx);

        if (bRet)
        {
            if (1 == u16Type)
                type = COMPUTER_SYSTEM_DESKTOP;

            if (2 == u16Type)
                type = COMPUTER_SYSTEM_NOTE_BOOK;
        }

        if (bRetEx)
        {
            if (8 == u16TypeEx)
                type = COMPUTER_SYSTEM_TABLET;
        }

        return bRet;
    }

    bool LComputerSystemManager::GetComputerSystemType(IN int index, OUT wstring& type)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"SystemType", type);
    }

    LOperatingSystemManager::LOperatingSystemManager()
    {
        m_pWMICoreManager = 0;
        m_pWMICoreManager = new LWMICoreManager();
        bool bRet = m_pWMICoreManager->BaseInit(NAMESPACE_ROOT_CIMV2);
        bRet = m_pWMICoreManager->WQLQuery(L"SELECT * FROM Win32_OperatingSystem");
    }

    LOperatingSystemManager::~LOperatingSystemManager()
    {
        delete m_pWMICoreManager;
    }

    int LOperatingSystemManager::GetOSCount()
    {
        return m_pWMICoreManager->GetObjectsCount();
    }

    bool LOperatingSystemManager::GetOSCaption(IN int index, OUT wstring& caption)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Caption", caption);
    }

    bool LOperatingSystemManager::GetOSArchitecture(IN int index, OUT wstring& architecture)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"OSArchitecture", architecture);
    }

    bool LOperatingSystemManager::GetOSVersion(IN int index, OUT wstring& version)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"Version", version);
    }

    bool LOperatingSystemManager::GetOSSystemDrive(IN int index, OUT wstring& drive)
    {
        return m_pWMICoreManager->GetStringProperty(index, L"SystemDrive", drive);
    }
}