/// @file LMNISTParser.h
/// @brief 本文声明了MNIST手写数字数据文件解析类
/// Detail:
/// MNIST手写数字数据文件可在如下URL下载: http://yann.lecun.com/exdb/mnist/
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

/// @brief MINIST手写数字图像文件类
class LMNISTImageFile
{
public:
    /// @brief 构造函数
    /// @param[in] pFilePath 文件路径
    explicit LMNISTImageFile(IN const char* pFilePath);

    /// @brief 析构函数
    ~LMNISTImageFile();

    /// @brief 获取图像总数
    /// @param[out] num 存储图像数量
    /// @return 成功返回true, 失败返回false
    bool GetImageNum(OUT unsigned int& num) const;

    /// @brief 获取图像大小
    /// @param[out] row 存储图像行大小(像素)
    /// @param[out] col 存储图像列大小(像素)
    /// @return 成功返回true, 失败返回false
    bool GetImageSize(OUT unsigned int& row, OUT unsigned int& col) const;

    /// @brief 获取图像数据
    /// 图像中每个像素占用一个字节
    /// @param[in] index 图像索引
    /// @param[out] pBuffer 存储图像数据
    /// @return 成功返回true, 失败返回false
    bool GetImage(IN unsigned int index, OUT unsigned char* pBuffer) const;

    /// @brief 获取图像数据
    /// 图像中每个像素占用一个字节
    /// @param[in] startIndex 图像开始索引
    /// @param[in] imageNum 图像数量
    /// @param[out] pBuffer 存储图像数据
    /// @return 成功返回true, 失败返回false
    bool GetImage(IN unsigned int startIndex, IN unsigned int imageNum,OUT unsigned char* pBuffer) const;

private:
    CMNISTImageFile* m_pMNISTImageFile; ///< 图像文件实现对象

private:
    /// @brief 禁止默认拷贝构造函数
    LMNISTImageFile(const LMNISTImageFile&);
    /// @brief 禁止默认赋值操作符
    LMNISTImageFile& operator = (const LMNISTImageFile&);
};

class CMNISTLabelFile;

/// @brief MINIST手写数字标签文件类
class LMNISTLabelFile
{
public:
    /// @brief 构造函数
    /// @param[in] pFilePath 文件路径
    explicit LMNISTLabelFile(IN const char* pFilePath);

    /// @brief 析构函数
    ~LMNISTLabelFile();

    /// @brief 获取标签总数
    /// @param[out] num 存储图像数量
    /// @return 成功返回true, 失败返回false
    bool GetLabelNum(OUT unsigned int& num) const;

    /// @brief 获取标签数据
    /// @param[in] index 标签索引
    /// @param[out] label 存储标签数据
    /// @return 成功返回true, 失败返回false
    bool GetLabel(IN unsigned int index, OUT unsigned char& label) const;

    /// @brief 获取标签数据
    /// 每个标签占用一个字节
    /// @param[in] startIndex 标签开始索引
    /// @param[in] labelNum 标签数量
    /// @param[out] pBuffer 存储标签数据
    /// @return 成功返回true, 失败返回false
    bool GetLabel(IN unsigned int startIndex, IN unsigned int labelNum, OUT unsigned char* pBuffer) const;

private:
    CMNISTLabelFile* m_pMNISTLabelFile; ///< 标签文件实现对象

private:
    /// @brief 禁止默认拷贝构造函数
    LMNISTLabelFile(const LMNISTLabelFile&);
    /// @brief 禁止默认赋值操作符
    LMNISTLabelFile& operator = (const LMNISTLabelFile&);
};

#endif



