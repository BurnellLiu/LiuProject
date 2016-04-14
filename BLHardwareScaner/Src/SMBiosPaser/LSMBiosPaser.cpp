

#include "LSMBiosPaser.h"

///@ brief BIOS��Ϣ����ֵ
#define BIOS_INFOR_TYPE 0

///@ brief ϵͳ��Ϣ����ֵ
#define SYSTEM_INFOR_TYPE 1

///@ brief ��������ֵ
#define BASEBOARD_INFOR_TYPE 2

/// @brief SMBios���ͽṹ
struct SMBiosType 
{
    unsigned int Type; ///< ����
    vector<unsigned char> TypeData; ///< ����
};

/// @brief SMBios������
class CSMBiosPaser
{
public:
    CSMBiosPaser(IN const vector<unsigned char>& smBiosData)
    {
        // ����Type����SMBios��Ϣ

        // Type�ṹ��ʼ����λ��
        unsigned typeStartIndex = 0;
        while (typeStartIndex < smBiosData.size())
        {
            SMBiosType newType;

            // ��ȡTypeֵ
            newType.Type = (unsigned int)smBiosData[typeStartIndex];

            // ��ȡ��ʽ���򳤶�
            unsigned int length = (unsigned int)smBiosData[typeStartIndex + 1];

            // �����ַ����������λ��
            unsigned int typeEndIndex = 0;
            for (unsigned int i = length + typeStartIndex; i < smBiosData.size()-1; i++)
            {
                if (smBiosData[i] == 0 && smBiosData[i + 1] == 0)
                {
                    typeEndIndex = i + 1;
                    break;
                }
            }
            if (typeEndIndex == 0)
                break;

            newType.TypeData.assign(
                smBiosData.begin() + typeStartIndex, 
                smBiosData.begin() + typeEndIndex + 1);

            typeStartIndex = typeEndIndex + 1;

            m_smBiosTypeList.push_back(newType);

        }
    }

    ~CSMBiosPaser()
    {

    }

    /// @brief ��ȡBIOS��Ϣ
    /// @param[out] biosInfor �洢BIOS��Ϣ
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetBiosInfor(OUT SMBiosBIOSInfor& biosInfor)
    {
        bool success = false;
        unsigned int biosInforIndex = 0;
        for (unsigned int i = 0; i < m_smBiosTypeList.size(); i++)
        {
            if (m_smBiosTypeList[i].Type == BIOS_INFOR_TYPE)
            {
                success = true;
                biosInforIndex = i;
                break;
            }
        }

        if (!success)
            return false;

        const SMBiosType& typeInfor = m_smBiosTypeList[biosInforIndex];

        vector<string> stringList;
        this->GetTypeStringList(typeInfor, stringList);

        biosInfor.Vendor = stringList[typeInfor.TypeData[0x04]-1];
        biosInfor.Version = stringList[typeInfor.TypeData[0x05]-1];
        biosInfor.ReleaseDate = stringList[typeInfor.TypeData[0x08]-1];
        biosInfor.RomSize = 64 * ((unsigned int)typeInfor.TypeData[0x09] + 1);

        return true;
    }

    bool GetSystemInfor(OUT SMBiosSystemInfor& systemInfor)
    {
        bool success = false;
        unsigned int systemInforIndex = 0;
        for (unsigned int i = 0; i < m_smBiosTypeList.size(); i++)
        {
            if (m_smBiosTypeList[i].Type == SYSTEM_INFOR_TYPE)
            {
                success = true;
                systemInforIndex = i;
                break;
            }
        }

        if (!success)
            return false;

        const SMBiosType& typeInfor = m_smBiosTypeList[systemInforIndex];

        vector<string> stringList;
        this->GetTypeStringList(typeInfor, stringList);

        systemInfor.Manufacturer = stringList[typeInfor.TypeData[0x04]-1];
        systemInfor.ProductName = stringList[typeInfor.TypeData[0x05]-1];
        systemInfor.Version = stringList[typeInfor.TypeData[0x06]-1];
        systemInfor.SerialNumber = stringList[typeInfor.TypeData[0x07]-1];

        for (unsigned int i = 0; i < 16; i++)
        {
            systemInfor.UUID[i] = typeInfor.TypeData[0x08 + i];
        }

        return true;
    }

    bool GetBaseBoardInfor(OUT SMBiosBaseBoardInfor& baseBoardInfor)
    {
        bool success = false;
        unsigned int baseBoardInforIndex = 0;
        for (unsigned int i = 0; i < m_smBiosTypeList.size(); i++)
        {
            if (m_smBiosTypeList[i].Type == BASEBOARD_INFOR_TYPE)
            {
                success = true;
                baseBoardInforIndex = i;
                break;
            }
        }

        if (!success)
            return false;

        const SMBiosType& typeInfor = m_smBiosTypeList[baseBoardInforIndex];

        vector<string> stringList;
        this->GetTypeStringList(typeInfor, stringList);

        baseBoardInfor.Manufacturer = stringList[typeInfor.TypeData[0x04]-1];
        baseBoardInfor.Product = stringList[typeInfor.TypeData[0x05]-1];
        baseBoardInfor.Version = stringList[typeInfor.TypeData[0x06]-1];
        baseBoardInfor.SerialNumber = stringList[typeInfor.TypeData[0x07]-1];

        return true;
    }

private:
    /// @brief ��Type�ṹ�л�ȡ�ַ��б�
    /// @param[in] type Type�ṹ
    /// @param[out] strList �洢�ַ����б�
    void GetTypeStringList(IN const SMBiosType& typeInfor, OUT vector<string>& strList)
    {
        strList.clear();

        unsigned int structLength = (unsigned int)typeInfor.TypeData[0x01];
        for (unsigned int i = structLength; i < typeInfor.TypeData.size(); )
        {
            if (typeInfor.TypeData[i] == 0)
                break;

            string str((char*)&(typeInfor.TypeData[i]));
            strList.push_back(str);

            i += str.length() + 1;
        }
    }

private:
    vector<SMBiosType> m_smBiosTypeList; ///< SMBIOS��ϢType�б�
};

LSMBiosPaser::LSMBiosPaser(IN const vector<unsigned char>& smBiosData)
{
    m_pSMBiosPaser = new CSMBiosPaser(smBiosData);
}

LSMBiosPaser::~LSMBiosPaser()
{
    if (m_pSMBiosPaser != 0)
    {
        delete m_pSMBiosPaser;
        m_pSMBiosPaser = 0;
    }
}

bool LSMBiosPaser::GetBiosInfor(OUT SMBiosBIOSInfor& biosInfor)
{
    return m_pSMBiosPaser->GetBiosInfor(biosInfor);
}

bool LSMBiosPaser::GetSystemInfor(OUT SMBiosSystemInfor& systemInfor)
{
    return m_pSMBiosPaser->GetSystemInfor(systemInfor);
}

bool LSMBiosPaser::GetBaseBoardInfor(OUT SMBiosBaseBoardInfor& baseBoardInfor)
{
    return m_pSMBiosPaser->GetBaseBoardInfor(baseBoardInfor);
}