/// @file LMNISTParser.h
/// @brief ����������MNIST��д���������ļ�������
/// Detail:
/// MNIST��д���������ļ���������URL����: http://yann.lecun.com/exdb/mnist/
/// @author Burnell_Liu Email:burnell_liu@outlook.com
/// @version   
/// @date 2016/05/27

#ifndef _LMNIST_PARSER_H_
#define _LMNIST_PARSER_H_

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class CMNISTImageFile;

/// @brief MINIST��д����ͼ���ļ���
class LMNISTImageFile
{
public:
    /// @brief ���캯��
    /// @param[in] pFilePath �ļ�·��
    explicit LMNISTImageFile(IN const char* pFilePath);

    /// @brief ��������
    ~LMNISTImageFile();

    /// @brief ��ȡͼ������
    /// @param[out] num �洢ͼ������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetImageNum(OUT unsigned int& num) const;

    /// @brief ��ȡͼ���С
    /// @param[out] row �洢ͼ���д�С(����)
    /// @param[out] col �洢ͼ���д�С(����)
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetImageSize(OUT unsigned int& row, OUT unsigned int& col) const;

    /// @brief ��ȡͼ������
    /// ͼ����ÿ������ռ��һ���ֽ�
    /// @param[in] index ͼ������
    /// @param[out] pBuffer �洢ͼ������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetImage(IN unsigned int index, OUT unsigned char* pBuffer) const;

    /// @brief ��ȡͼ������
    /// ͼ����ÿ������ռ��һ���ֽ�
    /// @param[in] startIndex ͼ��ʼ����
    /// @param[in] imageNum ͼ������
    /// @param[out] pBuffer �洢ͼ������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetImage(IN unsigned int startIndex, IN unsigned int imageNum,OUT unsigned char* pBuffer) const;

private:
    CMNISTImageFile* m_pMNISTImageFile; ///< ͼ���ļ�ʵ�ֶ���

private:
    /// @brief ��ֹĬ�Ͽ������캯��
    LMNISTImageFile(const LMNISTImageFile&);
    /// @brief ��ֹĬ�ϸ�ֵ������
    LMNISTImageFile& operator = (const LMNISTImageFile&);
};

class CMNISTLabelFile;

/// @brief MINIST��д���ֱ�ǩ�ļ���
class LMNISTLabelFile
{
public:
    /// @brief ���캯��
    /// @param[in] pFilePath �ļ�·��
    explicit LMNISTLabelFile(IN const char* pFilePath);

    /// @brief ��������
    ~LMNISTLabelFile();

    /// @brief ��ȡ��ǩ����
    /// @param[out] num �洢ͼ������
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetLabelNum(OUT unsigned int& num) const;

    /// @brief ��ȡ��ǩ����
    /// @param[in] index ��ǩ����
    /// @param[out] label �洢��ǩ����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetLabel(IN unsigned int index, OUT unsigned char& label) const;

    /// @brief ��ȡ��ǩ����
    /// ÿ����ǩռ��һ���ֽ�
    /// @param[in] startIndex ��ǩ��ʼ����
    /// @param[in] labelNum ��ǩ����
    /// @param[out] pBuffer �洢��ǩ����
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool GetLabel(IN unsigned int startIndex, IN unsigned int labelNum, OUT unsigned char* pBuffer) const;

private:
    CMNISTLabelFile* m_pMNISTLabelFile; ///< ��ǩ�ļ�ʵ�ֶ���

private:
    /// @brief ��ֹĬ�Ͽ������캯��
    LMNISTLabelFile(const LMNISTLabelFile&);
    /// @brief ��ֹĬ�ϸ�ֵ������
    LMNISTLabelFile& operator = (const LMNISTLabelFile&);
};

#endif



