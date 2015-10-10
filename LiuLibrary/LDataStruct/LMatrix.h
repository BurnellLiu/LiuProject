
#ifndef _DATASTRUCT_LMATRIX_H_
#define _DATASTRUCT_LMATRIX_H_

#ifndef LTEMPLATE
#define LTEMPLATE template<typename Type>
#endif

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief 矩阵
LTEMPLATE
class LMatrix
{
public:
    /// @brief 矩阵乘法
    /// 要求矩阵A的列数等于矩阵B的行数
    /// @param[in] A 被乘数
    /// @param[in] B 乘数
    /// @param[out] C 结果矩阵
    /// @return 参数错误返回false
    static bool MUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief 矩阵点乘
    /// 要求矩阵A的大小等于矩阵B的大小
    /// @param[in] A 被乘数
    /// @param[in] B 乘数
    /// @param[out] C 结果矩阵
    /// @return 参数错误返回false
    static bool DOTMUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief 矩阵点乘
    /// 要求矩阵A的大小等于矩阵B的大小
    /// @param[in] A 被乘数
    /// @param[in] B 乘数
    /// @return 结果矩阵
    static LMatrix<Type> DOTMUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B);

    /// @brief 矩阵数乘
    /// @param[in] A 被乘数
    /// @param[in] B 乘数
    /// @param[out] C 结果矩阵
    /// @return 返回true
    static bool SCALARMUL(IN const LMatrix<Type>& A, IN const Type& B, OUT LMatrix<Type>& C);

    /// @brief 矩阵点除
    /// 要求矩阵A的大小等于矩阵B的大小
    /// @param[in] A 被除数
    /// @param[in] B 除数
    /// @param[out] C 结果矩阵
    /// @return 参数错误返回false
    static bool DOTDIV(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief 矩阵减法
    /// 要求矩阵A的大小等于矩阵B的大小
    /// @param[in] A 被减数
    /// @param[in] B 减数
    /// @param[out] C 结果矩阵
    /// @return 参数错误返回false
    static bool SUB(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief 矩阵加法
    /// 要求矩阵A的大小等于矩阵B的大小
    /// @param[in] A 被加数
    /// @param[in] B 加数
    /// @param[out] C 结果矩阵
    /// @return 参数错误返回false
    static bool ADD(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief 矩阵转置
    /// @param[in] A 需要转置的矩阵
    /// @param[out] B 转置后的结果矩阵
    /// @return 返回true
    static bool T(IN const LMatrix<Type>& A, OUT LMatrix<Type>& B);

public:
    /// @brief 默认构造函数
    ///
    /// 默认矩阵长度为0, 即没有数据可被访问
    LMatrix();
    ~LMatrix();

    /// @brief 构造函数
    /// @param[in] row 矩阵行大小
    /// @param[in] col 矩阵列大小
    LMatrix(unsigned int row, unsigned int col);

    /// @brief 拷贝构造函数
    LMatrix(const LMatrix<Type>& rhs);

    /// @brief 赋值操作符
    LMatrix<Type>& operator = (const LMatrix<Type>& rhs);

    /// @brief 矩阵乘法
    /// 要求自身矩阵的列数等于矩阵B的行数
    /// @param[in] B 乘数
    /// @return 结果矩阵
    LMatrix<Type> operator * (IN const LMatrix<Type>& B) const;

    /// @brief 矩阵减法
    /// 要求自身矩阵大小等于矩阵B的大小
    /// @param[in] B 减数
    /// @return 结果矩阵
    LMatrix<Type> operator - (IN const LMatrix<Type>& B) const;

    /// @brief 矩阵加法
    /// 要求自身矩阵大小等于矩阵B的大小
    /// @param[in] B 加数
    /// @return 结果矩阵
    LMatrix<Type> operator + (IN const LMatrix<Type>& B) const;

    /// @brief []操作符
    ///
    /// 如果想使用高效率访问请直接使用Data
    /// @param[in] row 矩阵行
    Type*& operator[](unsigned int row);

    /// @brief []操作符
    ///
    /// 如果想使用高效率访问请直接使用Data
    /// @param[in] row 矩阵行
    const Type* operator[](unsigned int row) const;

    /// @brief 矩阵转置
    /// @return 转置后的结果矩阵
    LMatrix<Type> T() const;

    /// @brief 矩阵数乘
    /// @param[in] B 乘数
    /// @return 结果矩阵
    LMatrix<Type> ScalarMul(IN const Type& B) const;

    /// @brief 获取矩阵中的一行数据
    /// @param[in] row 行索引
    /// @return 行向量
    LMatrix<Type> GetRow(unsigned int row) const;

    /// @brief 获取矩阵中的一列数据
    /// @param[in] col 列索引
    /// @return 列向量
    LMatrix<Type> GetColumn(unsigned int col) const;

    /// @brief 重置矩阵
    /// @param[in] row 矩阵行大小
    /// @param[in] col 矩阵列大小
    void Reset(unsigned int row, unsigned int col);

public:
    unsigned int RowLen; ///< 行长度
    unsigned int ColumnLen; ///< 列长度
    Type** Data; ///< 数据列表
private:
    Type* m_data; ///< 实际存储的数据
};

LTEMPLATE
LMatrix<Type>::LMatrix()
: RowLen(0), ColumnLen(0), Data(0), m_data(0)
{

}

LTEMPLATE
LMatrix<Type>::LMatrix(unsigned int row, unsigned int col)
: RowLen(row), ColumnLen(col), Data(0), m_data(0)
{
    if (0 == (this->RowLen * this->ColumnLen))
        return;

    this->Data = new Type*[this->RowLen];
    this->m_data = new Type[this->RowLen * this->ColumnLen];
    for (unsigned int i = 0; i < this->RowLen; i++)
    {
        this->Data[i] = &this->m_data[this->ColumnLen * i];
    }
}

LTEMPLATE
LMatrix<Type>::~LMatrix()
{
    if (this->Data)
    {
        delete[] this->Data;
        this->Data = 0;
    }

    if (this->m_data)
    {
        delete[] this->m_data;
        this->m_data = 0;
    }

    this->RowLen = 0;
    this->ColumnLen = 0;
}

LTEMPLATE
LMatrix<Type>::LMatrix(const LMatrix<Type>& rhs)
: RowLen(0), ColumnLen(0), Data(0), m_data(0)
{
    unsigned int totalLen = rhs.RowLen * rhs.ColumnLen;
    if (0 == totalLen)
        return;

    this->RowLen = rhs.RowLen;
    this->ColumnLen = rhs.ColumnLen;

    this->m_data = new Type[totalLen];
    this->Data = new Type*[this->RowLen];


    for (unsigned int i = 0; i < this->RowLen; i++)
    {
        this->Data[i] = &this->m_data[this->ColumnLen * i];

        for (unsigned int j = 0; j < this->ColumnLen; j++)
        {
             this->m_data[i * this->ColumnLen + j] = rhs.m_data[i * this->ColumnLen + j];
        }
    }
}

LTEMPLATE
LMatrix<Type>& LMatrix<Type>::operator = (const LMatrix<Type>& rhs)
{
    if ((this->RowLen != rhs.RowLen) || (this->ColumnLen != rhs.ColumnLen))
    {
        if (this->m_data)
        {
            delete[] this->m_data;
            this->m_data = 0;
        }
        if (this->Data)
        {
            delete[] this->Data;
            this->Data = 0;
        }
        this->RowLen = 0;
        this->ColumnLen = 0;

        if (rhs.RowLen * rhs.ColumnLen > 0)
        {
            this->RowLen = rhs.RowLen;
            this->ColumnLen = rhs.ColumnLen;
            this->Data = new Type*[this->RowLen];
            this->m_data = new Type[this->RowLen * this->ColumnLen];
        }
    }

    for (unsigned int i = 0; i < this->RowLen; i++)
    {
         this->Data[i] = &this->m_data[this->ColumnLen * i];

        for (unsigned int j = 0; j < this->ColumnLen; j++)
        {
            this->m_data[i * this->ColumnLen + j] = rhs.m_data[i * this->ColumnLen + j];
        }
    }

    return *this;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::operator * (IN const LMatrix<Type>& B) const
{
    LMatrix<Type> C;
    MUL(*this, B, C);
    return C;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::operator - (IN const LMatrix<Type>& B) const
{
    LMatrix<Type> C;
    SUB(*this, B, C);
    return C;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::operator + (IN const LMatrix<Type>& B) const
{
    LMatrix<Type> C;
    ADD(*this, B, C);
    return C;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::ScalarMul(IN const Type& B) const
{
    LMatrix<Type> C;
    SCALARMUL(*this, B, C);
    return C;
}

LTEMPLATE
Type*& LMatrix<Type>::operator[](unsigned int row)
{
    return this->Data[row];
}

LTEMPLATE
const Type* LMatrix<Type>::operator[](unsigned int row) const
{
    return this->Data[row];
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::T() const
{
    LMatrix<Type> B;
    T(*this, B);
    return B;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::GetRow(unsigned int row) const
{
    LMatrix<Type> rowVector(1, this->ColumnLen);
    for (unsigned int i = 0; i < this->ColumnLen; i++)
    {
        rowVector.Data[0][i] = this->Data[row][i];
    }

    return rowVector;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::GetColumn(unsigned int col) const
{
    LMatrix<Type> columnVector(this->RowLen, 1);
    for (unsigned int i = 0; i < this->RowLen; i++)
    {
        columnVector.Data[i][0] = this->Data[i][col];
    }

    return columnVector;
}

LTEMPLATE
void LMatrix<Type>::Reset(unsigned int row, unsigned int col)
{
    if ((this->RowLen != row) || this->ColumnLen != col)
    {
        if (this->Data)
        {
            delete[] this->Data;
            this->Data = 0;
        }

        if (this->m_data)
        {
            delete[] this->m_data;
            this->m_data = 0;
        }

        this->RowLen = row;
        this->ColumnLen = col;

        if (row * col > 0)
        {
            this->Data = new Type*[this->RowLen];
            this->m_data = new Type[this->RowLen * this->ColumnLen];
            for (unsigned int i = 0; i < this->RowLen; i++)
            {
                this->Data[i] = &this->m_data[this->ColumnLen * i];
            }
        }
    }
}

LTEMPLATE
bool LMatrix<Type>::MUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C)
{
    if (A.ColumnLen != B.RowLen)
        return false;

    C.Reset(A.RowLen, B.ColumnLen);

    for (unsigned int i = 0; i < C.RowLen; i++)
    {
        for (unsigned int j = 0; j < C.ColumnLen; j++)
        {
            C.Data[i][j] = A.Data[i][0] * B.Data[0][j];
            for (unsigned int k = 1; k < A.ColumnLen; k++)
            {
                C.Data[i][j] += A.Data[i][k] * B.Data[k][j];
            }
        }
    }

    return true;
}

LTEMPLATE
bool LMatrix<Type>::DOTMUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C)
{
    if ((A.RowLen != B.RowLen) || (A.ColumnLen != B.ColumnLen))
        return false;

    C.Reset(A.RowLen, A.ColumnLen);

    for (unsigned int i = 0; i < C.RowLen; i++)
    {
        for (unsigned int j = 0; j < C.ColumnLen; j++)
        {
            C.Data[i][j] = A.Data[i][j] * B.Data[i][j];
        }
    }

    return true;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::DOTMUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B)
{
    LMatrix<Type> C;
    DOTMUL(A, B, C);
    return C;
}

LTEMPLATE
bool LMatrix<Type>::SCALARMUL(IN const LMatrix<Type>& A, IN const Type& B, OUT LMatrix<Type>& C)
{
    C.Reset(A.RowLen, A.ColumnLen);
    for (unsigned int row = 0; row < A.RowLen; row++)
    {
        for (unsigned int col = 0; col < A.ColumnLen; col++)
        {
            C.Data[row][col] = A.Data[row][col] * B;
        }
    }

    return true;
}

LTEMPLATE
bool LMatrix<Type>::SUB(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C)
{
    if ((A.RowLen != B.RowLen) || (A.ColumnLen != B.ColumnLen))
        return false;

    C.Reset(A.RowLen, A.ColumnLen);

    for (unsigned int i = 0; i < C.RowLen; i++)
    {
        for (unsigned int j = 0; j < C.ColumnLen; j++)
        {
            C.Data[i][j] = A.Data[i][j] - B.Data[i][j];
        }
    }

    return true;
}

LTEMPLATE
bool LMatrix<Type>::ADD(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C)
{
    if ((A.RowLen != B.RowLen) || (A.ColumnLen != B.ColumnLen))
        return false;

    C.Reset(A.RowLen, A.ColumnLen);

    for (unsigned int i = 0; i < C.RowLen; i++)
    {
        for (unsigned int j = 0; j < C.ColumnLen; j++)
        {
            C.Data[i][j] = A.Data[i][j] + B.Data[i][j];
        }
    }

    return true;
}

LTEMPLATE
bool LMatrix<Type>::DOTDIV(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C)
{
    if ((A.RowLen != B.RowLen) || (A.ColumnLen != B.ColumnLen))
        return false;

    C.Reset(A.RowLen, A.ColumnLen);

    for (unsigned int i = 0; i < C.RowLen; i++)
    {
        for (unsigned int j = 0; j < C.ColumnLen; j++)
        {
            C.Data[i][j] = A.Data[i][j] / B.Data[i][j];
        }
    }

    return true;
}

LTEMPLATE
 bool LMatrix<Type>::T(IN const LMatrix<Type>& A, OUT LMatrix<Type>& B)
{
    B.Reset(A.ColumnLen, A.RowLen);
    for (unsigned int i = 0; i < A.RowLen; i++)
    {
        for (unsigned int j = 0; j < A.ColumnLen; j++)
            B.Data[j][i] = A.Data[i][j];
    }

    return true;
}




#endif
