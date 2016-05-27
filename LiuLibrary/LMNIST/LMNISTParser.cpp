

#include "LMNISTParser.h"

#include <cstdio>

#define IMAGE_FILE_HEADER_SIZE 16
#define LABEL_FILE_HEADER_SIZE 8

class CMNISTImageFile
{
public:
    /// @brief ���캯��
    CMNISTImageFile(IN const char* pFilePath)
    {
        m_pFile = 0;
        m_imageNum = 0;
        m_imageRow = 0;
        m_imageCol = 0;
    
        if (pFilePath == 0)
            return;

        fopen_s(&m_pFile, pFilePath, "rb");
        if (m_pFile == 0)
            return;

        unsigned char fileHeaderBuffer[IMAGE_FILE_HEADER_SIZE] = {0};
        size_t count = fread_s(fileHeaderBuffer, IMAGE_FILE_HEADER_SIZE, IMAGE_FILE_HEADER_SIZE, 1, m_pFile);
        if (count != 1)
            return;

        // �����ģʽת��ΪС��ģʽ
        for (unsigned int i = 0; i < 16; i += 4)
        {
            unsigned char temp = fileHeaderBuffer[i];
            fileHeaderBuffer[i] = fileHeaderBuffer[i + 3];
            fileHeaderBuffer[i + 3] = temp;

            temp = fileHeaderBuffer[i + 1];;
            fileHeaderBuffer[i + 1] = fileHeaderBuffer[i + 2];
            fileHeaderBuffer[i + 2] = temp;
        }

        m_imageNum = *(unsigned int*)(fileHeaderBuffer + 4);
        m_imageRow = *(unsigned int*)(fileHeaderBuffer + 8);
        m_imageCol = *(unsigned int*)(fileHeaderBuffer + 12);

    }

    /// @brief ��������
    ~CMNISTImageFile()
    {
        if (m_pFile != 0)
        {
            fclose(m_pFile);
            m_pFile = 0;
        }
    }

    /// @brief ��ȡͼ������
    bool GetImageNum(OUT unsigned int& num) const
    {
        if (0 == m_pFile || 0 == m_imageNum)
            return false;

        num = m_imageNum;

        return true;
    }

    /// @brief ��ȡͼ���С
    bool GetImageSize(OUT unsigned int& row, OUT unsigned int& col) const
    {
        if (0 == m_pFile || 0 == m_imageRow || 0 == m_imageCol)
            return false;

        row = m_imageRow;
        col = m_imageCol;

        return true;
    }

    /// @brief ��ȡͼ������
    bool GetImage(IN unsigned int index, OUT unsigned char* pBuffer) const
    {
        return this->GetImage(index, 1, pBuffer);
    }

    /// @brief ��ȡͼ������
    bool GetImage(IN unsigned int startIndex, IN unsigned int imageNum, OUT unsigned char* pBuffer) const
    {
        if (0 == m_pFile || 0 == m_imageNum || 0 == m_imageRow || 0 == m_imageCol)
            return false;

        unsigned int imageSize = m_imageRow * m_imageCol;
        unsigned int offset = IMAGE_FILE_HEADER_SIZE + startIndex * imageSize;
        fseek(m_pFile, offset, SEEK_SET);
        size_t count = fread_s(pBuffer, imageSize * imageNum, imageSize, imageNum, m_pFile);
        if (count != imageNum)
            return false;

        return true;
    }

private:
    FILE* m_pFile; ///< �ļ�ָ��
    unsigned int m_imageNum; ///< ͼƬ����
    unsigned int m_imageRow; ///< ͼƬ��(����)
    unsigned int m_imageCol; ///< ͼƬ��(����)

private:
    /// @brief ��ֹĬ�Ͽ������캯��
    CMNISTImageFile(const CMNISTImageFile&);
    /// @brief ��ֹĬ�ϸ�ֵ������
    CMNISTImageFile& operator = (const CMNISTImageFile&);
};

/// @brief MINIST��д���ֱ�ǩ�ļ���
class CMNISTLabelFile
{
public:
    /// @brief ���캯��
    /// @param[in] pFilePath �ļ�·��
    explicit CMNISTLabelFile(IN const char* pFilePath)
    {
        m_pFile = 0;

        if (pFilePath == 0)
            return;

        fopen_s(&m_pFile, pFilePath, "rb");
        if (m_pFile == 0)
            return;

        unsigned char fileHeaderBuffer[LABEL_FILE_HEADER_SIZE] = { 0 };
        size_t count = fread_s(fileHeaderBuffer, LABEL_FILE_HEADER_SIZE, LABEL_FILE_HEADER_SIZE, 1, m_pFile);
        if (count != 1)
            return;

        // �����ģʽת��ΪС��ģʽ
        for (unsigned int i = 0; i < LABEL_FILE_HEADER_SIZE; i += 4)
        {
            unsigned char temp = fileHeaderBuffer[i];
            fileHeaderBuffer[i] = fileHeaderBuffer[i + 3];
            fileHeaderBuffer[i + 3] = temp;

            temp = fileHeaderBuffer[i + 1];;
            fileHeaderBuffer[i + 1] = fileHeaderBuffer[i + 2];
            fileHeaderBuffer[i + 2] = temp;
        }

        m_labelNum = *(unsigned int*)(fileHeaderBuffer + 4);
    }

    /// @brief ��������
    ~CMNISTLabelFile()
    {
        if (m_pFile != 0)
        {
            fclose(m_pFile);
            m_pFile = 0;
        }
    }

    /// @brief ��ȡ��ǩ����
    bool GetLabelNum(OUT unsigned int& num) const
    {
        if (0 == m_pFile || 0 == m_labelNum)
            return false;

        num = m_labelNum;
        return true;
    }

    /// @brief ��ȡ��ǩ����
    bool GetLabel(IN unsigned int index, OUT unsigned char& label) const
    {
        return this->GetLabel(index, 1, &label);
    }

    /// @brief ��ȡ��ǩ����
    bool GetLabel(IN unsigned int startIndex, IN unsigned int labelNum, OUT unsigned char* pBuffer) const
    {
        if (0 == m_pFile || 0 == m_labelNum)
            return false;

        unsigned int offset = LABEL_FILE_HEADER_SIZE + startIndex;
        fseek(m_pFile, offset, SEEK_SET);
        size_t count = fread_s(pBuffer, labelNum, 1, labelNum, m_pFile);
        if (count != labelNum)
            return false;

        return true;
    }

private:
    FILE* m_pFile; ///< �ļ�ָ��
    unsigned int m_labelNum; ///< ͼƬ����

private:
    /// @brief ��ֹĬ�Ͽ������캯��
    CMNISTLabelFile(const LMNISTLabelFile&);
    /// @brief ��ֹĬ�ϸ�ֵ������
    CMNISTLabelFile& operator = (const LMNISTLabelFile&);
};

LMNISTImageFile::LMNISTImageFile(IN const char* pFilePath)
    : m_pMNISTImageFile(0)
{
    m_pMNISTImageFile = new CMNISTImageFile(pFilePath);
}

LMNISTImageFile::~LMNISTImageFile()
{
    if (m_pMNISTImageFile != 0)
    {
        delete m_pMNISTImageFile;
        m_pMNISTImageFile = 0;
    }
}

bool LMNISTImageFile::GetImageNum(OUT unsigned int& num) const
{
    return m_pMNISTImageFile->GetImageNum(num);
}

bool LMNISTImageFile::GetImageSize(OUT unsigned int& row, OUT unsigned int& col) const
{
    return m_pMNISTImageFile->GetImageSize(row, col);
}

bool LMNISTImageFile::GetImage(IN unsigned int index, OUT unsigned char* pBuffer) const
{
    return m_pMNISTImageFile->GetImage(index, pBuffer);
}

bool LMNISTImageFile::GetImage(IN unsigned int startIndex, IN unsigned int imageNum, OUT unsigned char* pBuffer) const
{
    return m_pMNISTImageFile->GetImage(startIndex, imageNum, pBuffer);
}

LMNISTLabelFile::LMNISTLabelFile(IN const char* pFilePath)
    : m_pMNISTLabelFile(0)
{
    m_pMNISTLabelFile = new CMNISTLabelFile(pFilePath);
}

LMNISTLabelFile::~LMNISTLabelFile()
{
    if (m_pMNISTLabelFile != 0)
    {
        delete m_pMNISTLabelFile;
        m_pMNISTLabelFile = 0;
    }
}

bool LMNISTLabelFile::GetLabelNum(OUT unsigned int& num) const
{
    return m_pMNISTLabelFile->GetLabelNum(num);
}

bool LMNISTLabelFile::GetLabel(IN unsigned int index, OUT unsigned char& label) const
{
    return m_pMNISTLabelFile->GetLabel(index, label);
}

bool LMNISTLabelFile::GetLabel(IN unsigned int startIndex, IN unsigned int labelNum, OUT unsigned char* pBuffer) const
{
    return m_pMNISTLabelFile->GetLabel(startIndex, labelNum, pBuffer);
}

