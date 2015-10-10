
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

/// @brief ����
LTEMPLATE
class LMatrix
{
public:
    /// @brief ����˷�
    /// Ҫ�����A���������ھ���B������
    /// @param[in] A ������
    /// @param[in] B ����
    /// @param[out] C �������
    /// @return �������󷵻�false
    static bool MUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief ������
    /// Ҫ�����A�Ĵ�С���ھ���B�Ĵ�С
    /// @param[in] A ������
    /// @param[in] B ����
    /// @param[out] C �������
    /// @return �������󷵻�false
    static bool DOTMUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief ������
    /// Ҫ�����A�Ĵ�С���ھ���B�Ĵ�С
    /// @param[in] A ������
    /// @param[in] B ����
    /// @return �������
    static LMatrix<Type> DOTMUL(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B);

    /// @brief ��������
    /// @param[in] A ������
    /// @param[in] B ����
    /// @param[out] C �������
    /// @return ����true
    static bool SCALARMUL(IN const LMatrix<Type>& A, IN const Type& B, OUT LMatrix<Type>& C);

    /// @brief ������
    /// Ҫ�����A�Ĵ�С���ھ���B�Ĵ�С
    /// @param[in] A ������
    /// @param[in] B ����
    /// @param[out] C �������
    /// @return �������󷵻�false
    static bool DOTDIV(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief �������
    /// Ҫ�����A�Ĵ�С���ھ���B�Ĵ�С
    /// @param[in] A ������
    /// @param[in] B ����
    /// @param[out] C �������
    /// @return �������󷵻�false
    static bool SUB(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief ����ӷ�
    /// Ҫ�����A�Ĵ�С���ھ���B�Ĵ�С
    /// @param[in] A ������
    /// @param[in] B ����
    /// @param[out] C �������
    /// @return �������󷵻�false
    static bool ADD(IN const LMatrix<Type>& A, IN const LMatrix<Type>& B, OUT LMatrix<Type>& C);

    /// @brief ����ת��
    /// @param[in] A ��Ҫת�õľ���
    /// @param[out] B ת�ú�Ľ������
    /// @return ����true
    static bool T(IN const LMatrix<Type>& A, OUT LMatrix<Type>& B);

public:
    /// @brief Ĭ�Ϲ��캯��
    ///
    /// Ĭ�Ͼ��󳤶�Ϊ0, ��û�����ݿɱ�����
    LMatrix();
    ~LMatrix();

    /// @brief ���캯��
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    LMatrix(unsigned int row, unsigned int col);

    /// @brief �������캯��
    LMatrix(const LMatrix<Type>& rhs);

    /// @brief ��ֵ������
    LMatrix<Type>& operator = (const LMatrix<Type>& rhs);

    /// @brief ����˷�
    /// Ҫ�����������������ھ���B������
    /// @param[in] B ����
    /// @return �������
    LMatrix<Type> operator * (IN const LMatrix<Type>& B) const;

    /// @brief �������
    /// Ҫ����������С���ھ���B�Ĵ�С
    /// @param[in] B ����
    /// @return �������
    LMatrix<Type> operator - (IN const LMatrix<Type>& B) const;

    /// @brief ����ӷ�
    /// Ҫ����������С���ھ���B�Ĵ�С
    /// @param[in] B ����
    /// @return �������
    LMatrix<Type> operator + (IN const LMatrix<Type>& B) const;

    /// @brief []������
    ///
    /// �����ʹ�ø�Ч�ʷ�����ֱ��ʹ��Data
    /// @param[in] row ������
    Type*& operator[](unsigned int row);

    /// @brief []������
    ///
    /// �����ʹ�ø�Ч�ʷ�����ֱ��ʹ��Data
    /// @param[in] row ������
    const Type* operator[](unsigned int row) const;

    /// @brief ����ת��
    /// @return ת�ú�Ľ������
    LMatrix<Type> T() const;

    /// @brief ��������
    /// @param[in] B ����
    /// @return �������
    LMatrix<Type> ScalarMul(IN const Type& B) const;

    /// @brief ��ȡ�����е�һ������
    /// @param[in] row ������
    /// @return ������
    LMatrix<Type> GetRow(unsigned int row) const;

    /// @brief ��ȡ�����е�һ������
    /// @param[in] col ������
    /// @return ������
    LMatrix<Type> GetColumn(unsigned int col) const;

    /// @brief ���þ���
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    void Reset(unsigned int row, unsigned int col);

public:
    unsigned int RowLen; ///< �г���
    unsigned int ColumnLen; ///< �г���
    Type** Data; ///< �����б�
private:
    Type* m_data; ///< ʵ�ʴ洢������
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
