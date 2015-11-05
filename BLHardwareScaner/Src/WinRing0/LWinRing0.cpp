
#include "LWinRing0.h"

#include <Windows.h>

// DLL
typedef DWORD (WINAPI *_GetDllStatus) ();
typedef DWORD (WINAPI *_GetDllVersion) (PBYTE major, PBYTE minor, PBYTE revision, PBYTE release);
typedef DWORD (WINAPI *_GetDriverVersion) (PBYTE major, PBYTE minor, PBYTE revision, PBYTE release);
typedef DWORD (WINAPI *_GetDriverType) ();

typedef BOOL (WINAPI *_InitializeOls) ();
typedef VOID (WINAPI *_DeinitializeOls) ();

// CPU
typedef BOOL (WINAPI *_IsCpuid) ();
typedef BOOL (WINAPI *_IsMsr) ();
typedef BOOL (WINAPI *_IsTsc) ();

typedef BOOL  (WINAPI *_Hlt) ();
typedef DWORD (WINAPI *_Rdmsr) (DWORD index, PDWORD eax, PDWORD edx);
typedef DWORD (WINAPI *_Wrmsr) (DWORD index, DWORD eax, DWORD edx);
typedef DWORD (WINAPI *_Rdpmc) (DWORD index, PDWORD eax, PDWORD edx);
typedef DWORD (WINAPI *_Cpuid) (DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx);
typedef DWORD (WINAPI *_Rdtsc) (PDWORD eax, PDWORD edx);

typedef BOOL  (WINAPI *_HltTx) (DWORD_PTR threadAffinityMask);
typedef DWORD (WINAPI *_RdmsrTx) (DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (WINAPI *_WrmsrTx) (DWORD index, DWORD eax, DWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (WINAPI *_RdpmcTx) (DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (WINAPI *_CpuidTx) (DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR threadAffinityMask);
typedef DWORD (WINAPI *_RdtscTx) (PDWORD eax, PDWORD edx, DWORD_PTR threadAffinityMask);

typedef BOOL  (WINAPI *_HltPx)   (DWORD_PTR processAffinityMask);
typedef DWORD (WINAPI *_RdmsrPx) (DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (WINAPI *_WrmsrPx) (DWORD index, DWORD eax, DWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (WINAPI *_RdpmcPx) (DWORD index, PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (WINAPI *_CpuidPx) (DWORD index, PDWORD eax, PDWORD ebx, PDWORD ecx, PDWORD edx, DWORD_PTR processAffinityMask);
typedef DWORD (WINAPI *_RdtscPx) (PDWORD eax, PDWORD edx, DWORD_PTR processAffinityMask);

// I/O
typedef BYTE  (WINAPI *_ReadIoPortByte) (WORD address);
typedef WORD  (WINAPI *_ReadIoPortWord) (WORD address);
typedef DWORD (WINAPI *_ReadIoPortDword) (WORD address);

typedef BOOL (WINAPI *_ReadIoPortByteEx) (WORD address, PBYTE value);
typedef BOOL (WINAPI *_ReadIoPortWordEx) (WORD address, PWORD value);
typedef BOOL (WINAPI *_ReadIoPortDwordEx) (WORD address, PDWORD value);

typedef VOID (WINAPI *_WriteIoPortByte) (WORD address, BYTE value);
typedef VOID (WINAPI *_WriteIoPortWord) (WORD address, WORD value);
typedef VOID (WINAPI *_WriteIoPortDword) (WORD address, DWORD value);

typedef BOOL (WINAPI *_WriteIoPortByteEx) (WORD address, BYTE value);
typedef BOOL (WINAPI *_WriteIoPortWordEx) (WORD address, WORD value);
typedef BOOL (WINAPI *_WriteIoPortDwordEx) (WORD address, DWORD value);

// PCI
typedef VOID (WINAPI *_SetPciMaxBusIndex) (BYTE max);

typedef BYTE  (WINAPI *_ReadPciConfigByte) (DWORD pciAddress, BYTE regAddress);
typedef WORD  (WINAPI *_ReadPciConfigWord) (DWORD pciAddress, BYTE regAddress);
typedef DWORD (WINAPI *_ReadPciConfigDword) (DWORD pciAddress, BYTE regAddress);

typedef BOOL (WINAPI *_ReadPciConfigByteEx) (DWORD pciAddress, DWORD regAddress, PBYTE value);
typedef BOOL (WINAPI *_ReadPciConfigWordEx) (DWORD pciAddress, DWORD regAddress, PWORD value);
typedef BOOL (WINAPI *_ReadPciConfigDwordEx) (DWORD pciAddress, DWORD regAddress, PDWORD value);

typedef VOID (WINAPI *_WritePciConfigByte) (DWORD pciAddress, BYTE regAddress, BYTE value);
typedef VOID (WINAPI *_WritePciConfigWord) (DWORD pciAddress, BYTE regAddress, WORD value);
typedef VOID (WINAPI *_WritePciConfigDword) (DWORD pciAddress, BYTE regAddress, DWORD value);

typedef BOOL (WINAPI *_WritePciConfigByteEx) (DWORD pciAddress, DWORD regAddress, BYTE value);
typedef BOOL (WINAPI *_WritePciConfigWordEx) (DWORD pciAddress, DWORD regAddress, WORD value);
typedef BOOL (WINAPI *_WritePciConfigDwordEx) (DWORD pciAddress, DWORD regAddress, DWORD value);

typedef DWORD (WINAPI *_FindPciDeviceById) (WORD vendorId, WORD deviceId, BYTE index);
typedef DWORD (WINAPI *_FindPciDeviceByClass) (BYTE baseClass, BYTE subClass, BYTE programIf, BYTE index);

namespace LWinRing0
{
    static HMODULE s_hModule = NULL; ///< ¶¯Ì¬¿âÄ£¿é¾ä±ú

    // DLL
    _GetDllStatus GetDllStatus = NULL;
    _GetDllVersion GetDllVersion = NULL;
    _GetDriverVersion GetDriverVersion = NULL;
    _GetDriverType GetDriverType = NULL;

    _InitializeOls InitializeOls = NULL;
    _DeinitializeOls DeinitializeOls = NULL;

    // CPU
    _IsCpuid IsCpuid = NULL;
    _IsMsr IsMsr = NULL;
    _IsTsc IsTsc = NULL;

    _Hlt Hlt = NULL;
    _Rdmsr Rdmsr = NULL;
    _Wrmsr Wrmsr = NULL;
    _Rdpmc Rdpmc = NULL;
    _Cpuid Cpuid = NULL;
    _Rdtsc Rdtsc = NULL;

    _HltTx HltTx = NULL;
    _RdmsrTx RdmsrTx = NULL;
    _WrmsrTx WrmsrTx = NULL;
    _RdpmcTx RdpmcTx = NULL;
    _CpuidTx CpuidTx = NULL;
    _RdtscTx RdtscTx = NULL;

    _HltPx HltPx = NULL;
    _RdmsrPx RdmsrPx = NULL;
    _WrmsrPx WrmsrPx = NULL;
    _RdpmcPx RdpmcPx = NULL;
    _CpuidPx CpuidPx = NULL;
    _RdtscPx RdtscPx = NULL;

    // I/O
    _ReadIoPortByte ReadIoPortByte = NULL;
    _ReadIoPortWord ReadIoPortWord = NULL;
    _ReadIoPortDword ReadIoPortDword = NULL;

    _ReadIoPortByteEx ReadIoPortByteEx = NULL;
    _ReadIoPortWordEx ReadIoPortWordEx = NULL;
    _ReadIoPortDwordEx ReadIoPortDwordEx = NULL;

    _WriteIoPortByte WriteIoPortByte = NULL;
    _WriteIoPortWord WriteIoPortWord = NULL;
    _WriteIoPortDword WriteIoPortDword = NULL;

    _WriteIoPortByteEx WriteIoPortByteEx = NULL;
    _WriteIoPortWordEx WriteIoPortWordEx = NULL;
    _WriteIoPortDwordEx WriteIoPortDwordEx = NULL;

    // PCI
    _SetPciMaxBusIndex SetPciMaxBusIndex = NULL;

    _ReadPciConfigByte ReadPciConfigByte = NULL;
    _ReadPciConfigWord ReadPciConfigWord = NULL;
    _ReadPciConfigDword ReadPciConfigDword = NULL;

    _ReadPciConfigByteEx ReadPciConfigByteEx = NULL;
    _ReadPciConfigWordEx ReadPciConfigWordEx = NULL;
    _ReadPciConfigDwordEx ReadPciConfigDwordEx = NULL;

    _WritePciConfigByte WritePciConfigByte = NULL;
    _WritePciConfigWord WritePciConfigWord = NULL;
    _WritePciConfigDword WritePciConfigDword = NULL;

    _WritePciConfigByteEx WritePciConfigByteEx = NULL;
    _WritePciConfigWordEx WritePciConfigWordEx = NULL;
    _WritePciConfigDwordEx WritePciConfigDwordEx = NULL;

    _FindPciDeviceById FindPciDeviceById = NULL;
    _FindPciDeviceByClass FindPciDeviceByClass = NULL;

    bool InitializeWinRing0(const wstring& filePath)
    {
        bool bRet = false;

        s_hModule = LoadLibraryW(filePath.c_str());
        if (s_hModule == NULL)
        {
            goto SAFE_EXIT;
            bRet = false;
        }

        //-----------------------------------------------------------------------------
        // GetProcAddress
        //-----------------------------------------------------------------------------
        // DLL
        GetDllStatus =			(_GetDllStatus)			GetProcAddress (s_hModule, "GetDllStatus");
        GetDllVersion =			(_GetDllVersion)		GetProcAddress (s_hModule, "GetDllVersion");
        GetDriverVersion =		(_GetDriverVersion)		GetProcAddress (s_hModule, "GetDriverVersion");
        GetDriverType =			(_GetDriverType)		GetProcAddress (s_hModule, "GetDriverType");

        InitializeOls =			(_InitializeOls)		GetProcAddress (s_hModule, "InitializeOls");
        DeinitializeOls =		(_DeinitializeOls)		GetProcAddress (s_hModule, "DeinitializeOls");

        // CPU
        IsCpuid =				(_IsCpuid)				GetProcAddress (s_hModule, "IsCpuid");
        IsMsr =					(_IsMsr)				GetProcAddress (s_hModule, "IsMsr");
        IsTsc =					(_IsTsc)				GetProcAddress (s_hModule, "IsTsc");
        Hlt =					(_Hlt)					GetProcAddress (s_hModule, "Hlt");
        Rdmsr =					(_Rdmsr)				GetProcAddress (s_hModule, "Rdmsr");
        Wrmsr =					(_Wrmsr)				GetProcAddress (s_hModule, "Wrmsr");
        Rdpmc =					(_Rdpmc)				GetProcAddress (s_hModule, "Rdpmc");
        Cpuid =					(_Cpuid)				GetProcAddress (s_hModule, "Cpuid");
        Rdtsc =					(_Rdtsc)				GetProcAddress (s_hModule, "Rdtsc");
        HltTx =					(_HltTx)				GetProcAddress (s_hModule, "HltTx");
        RdmsrTx =				(_RdmsrTx)				GetProcAddress (s_hModule, "RdmsrTx");
        WrmsrTx =				(_WrmsrTx)				GetProcAddress (s_hModule, "WrmsrTx");
        RdpmcTx =				(_RdpmcTx)				GetProcAddress (s_hModule, "RdpmcTx");
        CpuidTx =				(_CpuidTx)				GetProcAddress (s_hModule, "CpuidTx");
        RdtscTx =				(_RdtscTx)				GetProcAddress (s_hModule, "RdtscTx");
        HltPx =					(_HltPx)				GetProcAddress (s_hModule, "HltPx");
        RdmsrPx =				(_RdmsrPx)				GetProcAddress (s_hModule, "RdmsrPx");
        WrmsrPx =				(_WrmsrPx)				GetProcAddress (s_hModule, "WrmsrPx");
        RdpmcPx =				(_RdpmcPx)				GetProcAddress (s_hModule, "RdpmcPx");
        CpuidPx =				(_CpuidPx)				GetProcAddress (s_hModule, "CpuidPx");
        RdtscPx =				(_RdtscPx)				GetProcAddress (s_hModule, "RdtscPx");

        // I/O
        ReadIoPortByte =		(_ReadIoPortByte)		GetProcAddress (s_hModule, "ReadIoPortByte");
        ReadIoPortWord =		(_ReadIoPortWord)		GetProcAddress (s_hModule, "ReadIoPortWord");
        ReadIoPortDword =		(_ReadIoPortDword)		GetProcAddress (s_hModule, "ReadIoPortDword");

        ReadIoPortByteEx =		(_ReadIoPortByteEx)		GetProcAddress (s_hModule, "ReadIoPortByteEx");
        ReadIoPortWordEx =		(_ReadIoPortWordEx)		GetProcAddress (s_hModule, "ReadIoPortWordEx");
        ReadIoPortDwordEx =		(_ReadIoPortDwordEx)	GetProcAddress (s_hModule, "ReadIoPortDwordEx");

        WriteIoPortByte =		(_WriteIoPortByte)		GetProcAddress (s_hModule, "WriteIoPortByte");
        WriteIoPortWord =		(_WriteIoPortWord)		GetProcAddress (s_hModule, "WriteIoPortWord");
        WriteIoPortDword =		(_WriteIoPortDword)		GetProcAddress (s_hModule, "WriteIoPortDword");

        WriteIoPortByteEx =		(_WriteIoPortByteEx)	GetProcAddress (s_hModule, "WriteIoPortByteEx");
        WriteIoPortWordEx =		(_WriteIoPortWordEx)	GetProcAddress (s_hModule, "WriteIoPortWordEx");
        WriteIoPortDwordEx =	(_WriteIoPortDwordEx)	GetProcAddress (s_hModule, "WriteIoPortDwordEx");

        // PCI
        SetPciMaxBusIndex =		(_SetPciMaxBusIndex)	GetProcAddress (s_hModule, "SetPciMaxBusIndex");

        ReadPciConfigByte =		(_ReadPciConfigByte)	GetProcAddress (s_hModule, "ReadPciConfigByte");
        ReadPciConfigWord =		(_ReadPciConfigWord)	GetProcAddress (s_hModule, "ReadPciConfigWord");
        ReadPciConfigDword =	(_ReadPciConfigDword)	GetProcAddress (s_hModule, "ReadPciConfigDword");

        ReadPciConfigByteEx =	(_ReadPciConfigByteEx)	GetProcAddress (s_hModule, "ReadPciConfigByteEx");
        ReadPciConfigWordEx =	(_ReadPciConfigWordEx)	GetProcAddress (s_hModule, "ReadPciConfigWordEx");
        ReadPciConfigDwordEx =	(_ReadPciConfigDwordEx)	GetProcAddress (s_hModule, "ReadPciConfigDwordEx");

        WritePciConfigByte =	(_WritePciConfigByte)	GetProcAddress (s_hModule, "WritePciConfigByte");
        WritePciConfigWord =	(_WritePciConfigWord)	GetProcAddress (s_hModule, "WritePciConfigWord");
        WritePciConfigDword =	(_WritePciConfigDword)	GetProcAddress (s_hModule, "WritePciConfigDword");

        WritePciConfigByteEx =	(_WritePciConfigByteEx)	GetProcAddress (s_hModule, "WritePciConfigByteEx");
        WritePciConfigWordEx =	(_WritePciConfigWordEx)	GetProcAddress (s_hModule, "WritePciConfigWordEx");
        WritePciConfigDwordEx =	(_WritePciConfigDwordEx)GetProcAddress (s_hModule, "WritePciConfigDwordEx");

        FindPciDeviceById =		(_FindPciDeviceById)	GetProcAddress (s_hModule, "FindPciDeviceById");
        FindPciDeviceByClass =	(_FindPciDeviceByClass)	GetProcAddress (s_hModule, "FindPciDeviceByClass");

        //-----------------------------------------------------------------------------
        // Check Functions
        //-----------------------------------------------------------------------------
        if(!(
                    GetDllStatus
            &&	GetDllVersion
            &&	GetDriverVersion
            &&	GetDriverType
            &&	InitializeOls
            &&	DeinitializeOls
            &&	IsCpuid
            &&	IsMsr
            &&	IsTsc
            &&	Hlt
            &&	HltTx
            &&	HltPx
            &&	Rdmsr
            &&	RdmsrTx
            &&	RdmsrPx
            &&	Wrmsr
            &&	WrmsrTx
            &&	WrmsrPx
            &&	Rdpmc
            &&	RdpmcTx
            &&	RdpmcPx
            &&	Cpuid
            &&	CpuidTx
            &&	CpuidPx
            &&	Rdtsc
            &&	RdtscTx
            &&	RdtscPx
            &&	ReadIoPortByte
            &&	ReadIoPortWord
            &&	ReadIoPortDword
            &&	ReadIoPortByteEx
            &&	ReadIoPortWordEx
            &&	ReadIoPortDwordEx
            &&	WriteIoPortByte
            &&	WriteIoPortWord
            &&	WriteIoPortDword
            &&	WriteIoPortByteEx
            &&	WriteIoPortWordEx
            &&	WriteIoPortDwordEx
            &&	SetPciMaxBusIndex
            &&	ReadPciConfigByte
            &&	ReadPciConfigWord
            &&	ReadPciConfigDword
            &&	ReadPciConfigByteEx
            &&	ReadPciConfigWordEx
            &&	ReadPciConfigDwordEx
            &&	WritePciConfigByte
            &&	WritePciConfigWord 
            &&	WritePciConfigDword
            &&	WritePciConfigByteEx
            &&	WritePciConfigWordEx 
            &&	WritePciConfigDwordEx
            &&	FindPciDeviceById
            &&	FindPciDeviceByClass
            ))
        {
            bRet = false;
            goto SAFE_EXIT;
        }

        if (InitializeOls() == TRUE)
            bRet = true;
        else
            bRet = false;

SAFE_EXIT:
        if (!bRet)
        {
            DeinitializeWinRing0();
        }


        return bRet;
    }

    void DeinitializeWinRing0()
    {
        if (DeinitializeOls != NULL)
        {
            DeinitializeOls();
        }

        // DLL
        GetDllStatus =			                         NULL;
        GetDllVersion =		                             NULL;
        GetDriverVersion =	                             NULL;
        GetDriverType =		                             NULL;

        InitializeOls =		                             NULL;
        DeinitializeOls =		                             NULL;

        // CPU                                           
        IsCpuid =				                             NULL;
        IsMsr =					                             NULL;
        IsTsc =					                             NULL;
        Hlt =				                                     NULL;
        Rdmsr =				                             NULL;
        Wrmsr =				                             NULL;
        Rdpmc =				                             NULL;
        Cpuid =					                             NULL;
        Rdtsc =					                             NULL;
        HltTx =					                             NULL;
        RdmsrTx =				                             NULL;
        WrmsrTx =			                             NULL;
        RdpmcTx =			                             NULL;
        CpuidTx =				                             NULL;
        RdtscTx =				                             NULL;
        HltPx =					                             NULL;
        RdmsrPx =				                             NULL;
        WrmsrPx =				                             NULL;
        RdpmcPx =			                             NULL;
        CpuidPx =				                             NULL;
        RdtscPx =				                             NULL;

        // I/O                                                
        ReadIoPortByte =	                             NULL;
        ReadIoPortWord =	                             NULL;
        ReadIoPortDword =	                             NULL;
        ReadIoPortByteEx =		                     NULL;
        ReadIoPortWordEx =		                     NULL;
        ReadIoPortDwordEx =		                     NULL;
        WriteIoPortByte =		                         NULL;
        WriteIoPortWord =		                         NULL;
        WriteIoPortDword =	                         NULL;
        WriteIoPortByteEx =	                         NULL;
        WriteIoPortWordEx =	                         NULL;
        WriteIoPortDwordEx =	                         NULL;

        // PCI                                          
        SetPciMaxBusIndex =		                     NULL;
        ReadPciConfigByte =		                     NULL;
        ReadPciConfigWord =		                     NULL;
        ReadPciConfigDword =	                     NULL;
        ReadPciConfigByteEx =	                     NULL;
        ReadPciConfigWordEx =	                     NULL;
        ReadPciConfigDwordEx =                     NULL;
        WritePciConfigByte =	                         NULL;
        WritePciConfigWord =	                     NULL;
        WritePciConfigDword =	                     NULL;
        WritePciConfigByteEx =	                     NULL;
        WritePciConfigWordEx =	                     NULL;
        WritePciConfigDwordEx=                     NULL;

        FindPciDeviceById =		                     NULL;
        FindPciDeviceByClass =	                     NULL;
     

        if (s_hModule != NULL)
        {
            FreeLibrary(s_hModule);
            s_hModule = NULL;
        }
    }
}
