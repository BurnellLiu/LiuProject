
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
    /// Ĭ�Ͼ��󳤶�Ϊ0, ��û�����ݿɱ�����
    LMatrix();

    /// @brief ��������
    ~LMatrix();

    /// @brief ���캯��
    /// ���row��col����һ��Ϊ0, �����������������Ϊ0
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    LMatrix(IN unsigned int row, IN unsigned int col);

    /// @brief ���캯��, �������, ��ʹ��ָ�����ݳ�ʼ������
    /// ���row��col����һ��Ϊ0, �����������������Ϊ0
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    /// @param[in] initValue ��ʼ������
    LMatrix(IN unsigned int row, IN unsigned int col, IN const Type& initValue);

    /// @brief ���캯��, �������, ��ʹ���������ݳ�ʼ������
    /// ���row��col����һ��Ϊ0, �����������������Ϊ0
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    /// @param[in] pDataList ��������
    LMatrix(IN unsigned int row, IN unsigned int col, IN const Type* pDataList);

    /// @brief �������캯��
    LMatrix(IN const LMatrix<Type>& rhs);

    /// @brief ��ֵ������
    LMatrix<Type>& operator = (IN const LMatrix<Type>& rhs);

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
    /// @param[in] row ������
    Type*& operator[](IN unsigned int row);

    /// @brief []������
    ///
    /// @param[in] row ������
    const Type* operator[](IN unsigned int row) const;

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
    LMatrix<Type> GetRow(IN unsigned int row) const;

	/// @brief ��ȡ�����е�һ������
	/// @param[in] row ������
	/// @param[out] rowVector �洢������
	void GetRow(IN unsigned int row, OUT LMatrix<Type>& rowVector) const;

    /// @brief ��ȡ�����е�һ������
    /// @param[in] col ������
    /// @return ������
    LMatrix<Type> GetColumn(IN unsigned int col) const;

	/// @brief ��ȡ�����е�һ������
	/// @param[in] col ������
	/// @param[out] colVector �洢������
	void GetColumn(IN unsigned int col, OUT LMatrix<Type>& colVector) const;

    /// @brief ���þ���
    /// ���row��col����һ��Ϊ0, �����������������Ϊ0
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    void Reset(IN unsigned int row, IN unsigned int col);

    /// @brief ���þ���, ��ʹ��initValue��ʼ�������е�����ֵ
    /// ���row��col����һ��Ϊ0, �����������������Ϊ0
    /// @param[in] row �����д�С
    /// @param[in] col �����д�С
    /// @param[in] initValue ��ʼ��ֵ
    void Reset(IN unsigned int row, IN unsigned int col, IN const Type& initValue);

public:
    const unsigned int& RowLen; ///< �г�������
    const unsigned int& ColumnLen; ///< �г�������

private:
    Type** m_dataTable; ///< ��ά���ݱ�
    Type* m_dataList; ///< ʵ�ʴ洢�������б�
    unsigned int m_rowLen; ///< �����г���
    unsigned int m_columnLen; ///< �����г���
};

LTEMPLATE
LMatrix<Type>::LMatrix()
: m_rowLen(0), m_columnLen(0), RowLen(m_rowLen), ColumnLen(m_columnLen), m_dataTable(0), m_dataList(0)
{

}

LTEMPLATE
LMatrix<Type>::LMatrix(IN unsigned int row, IN unsigned int col)
: m_rowLen(0), m_columnLen(0), RowLen(m_rowLen), ColumnLen(m_columnLen), m_dataTable(0), m_dataList(0)
{
    this->Reset(row, col);
}

LTEMPLATE
LMatrix<Type>::LMatrix(IN unsigned int row, IN unsigned int col, IN const Type& initValue)
: m_rowLen(0), m_columnLen(0), RowLen(m_rowLen), ColumnLen(m_columnLen), m_dataTable(0), m_dataList(0)
{
    this->Reset(row, col);

    unsigned int size = row * col;
    for (unsigned int i = 0; i < size; i++)
    {
        this->m_dataList[i] = initValue;
    }
}

LTEMPLATE
LMatrix<Type>::LMatrix(IN unsigned int row, IN unsigned int col, IN const Type* pDataList)
: m_rowLen(0), m_columnLen(0), RowLen(m_rowLen), ColumnLen(m_columnLen), m_dataTable(0), m_dataList(0)
{
    this->Reset(row, col);

    unsigned int size = row * col;
    for (unsigned int i = 0; i < size; i++)
    {
        this->m_dataList[i] = pDataList[i];
    }

}

LTEMPLATE
LMatrix<Type>::~LMatrix()
{
    if (this->m_dataTable)
    {
        delete[] this->m_dataTable;
        this->m_dataTable = 0;
    }

    if (this->m_dataList)
    {
        delete[] this->m_dataList;
        this->m_dataList = 0;
    }

    this->m_rowLen = 0;
    this->m_columnLen = 0;
}

LTEMPLATE
LMatrix<Type>::LMatrix(IN const LMatrix<Type>& rhs)
: m_rowLen(0), m_columnLen(0), RowLen(m_rowLen), ColumnLen(m_columnLen), m_dataTable(0), m_dataList(0)
{
    this->Reset(rhs.RowLen, rhs.ColumnLen);

    unsigned int size = rhs.RowLen * rhs.ColumnLen;
    for (unsigned int i = 0; i < size; i++)
    {
        this->m_dataList[i] = rhs.m_dataList[i];
    }

}

LTEMPLATE
LMatrix<Type>& LMatrix<Type>::operator = (IN const LMatrix<Type>& rhs)
{
    this->Reset(rhs.RowLen, rhs.ColumnLen);

    unsigned int size = rhs.RowLen * rhs.ColumnLen;
    for (unsigned int i = 0; i < size; i++)
    {
        this->m_dataList[i] = rhs.m_dataList[i];
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
Type*& LMatrix<Type>::operator[](IN unsigned int row)
{
    return this->m_dataTable[row];
}

LTEMPLATE
const Type* LMatrix<Type>::operator[](IN unsigned int row) const
{
    return this->m_dataTable[row];
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::T() const
{
    LMatrix<Type> B;
    T(*this, B);
    return B;
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::GetRow(IN unsigned int row) const
{
    LMatrix<Type> rowVector(1, this->m_columnLen);
	this->GetRow(row, rowVector);

    return rowVector;
}

LTEMPLATE
void LMatrix<Type>::GetRow(IN unsigned int row, OUT LMatrix<Type>& rowVector) const
{
	rowVector.Reset(1, this->m_columnLen);
	for (unsigned int i = 0; i < this->m_columnLen; i++)
	{
		rowVector.m_dataTable[0][i] = this->m_dataTable[row][i];
	}
}

LTEMPLATE
LMatrix<Type> LMatrix<Type>::GetColumn(IN unsigned int col) const
{
    LMatrix<Type> columnVector(this->m_rowLen, 1);
	this->GetColumn(col, columnVector);

    return columnVector;
}

LTEMPLATE
void LMatrix<Type>::GetColumn(IN unsigned int col, OUT LMatrix<Type>& colVector) const
{
	colVector.Reset(this->m_rowLen, 1);
	for (unsigned int i = 0; i < this->m_rowLen; i++)
	{
		colVector.m_dataTable[i][0] = this->m_dataTable[i][col];
	}
}

LTEMPLATE
void LMatrix<Type>::Reset(IN unsigned int row, IN unsigned int col)
{
    if ((this->m_rowLen != row) || this->m_columnLen != col)
    {
        if (this->m_dataTable)
        {
            delete[] this->m_dataTable;
            this->m_dataTable = 0;
        }

        if (this->m_dataList)
        {
            delete[] this->m_dataList;
            this->m_dataList = 0;
        }

        
        if (row * col > 0)
        {
            this->m_rowLen = row;
            this->m_columnLen = col;

            this->m_dataTable = new Type*[this->m_rowLen];
            this->m_dataList = new Type[this->m_rowLen * this->m_columnLen];
            for (unsigned int i = 0; i < this->m_rowLen; i++)
            {
                this->m_dataTable[i] = &this->m_dataList[this->m_columnLen * i];
            }
        }
        else
        {
            this->m_rowLen = 0;
            this->m_columnLen = 0;
        }
    }
}

LTEMPLATE
void LMatrix<Type>::Reset(IN unsigned int row, IN unsigned int col, IN const Type& initValue)
{
    this->Reset(row, col);

    unsigned int size = row * col;
    for (unsigned int i = 0; i < size; i++)
    {
        this->m_dataList[i] = initValue;
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
            C.m_dataTable[i][j] = A.m_dataTable[i][0] * B.m_dataTable[0][j];
            for (unsigned int k = 1; k < A.ColumnLen; k++)
            {
                C.m_dataTable[i][j] += A.m_dataTable[i][k] * B.m_dataTable[k][j];
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
            C.m_dataTable[i][j] = A.m_dataTable[i][j] * B.m_dataTable[i][j];
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
            C.m_dataTable[row][col] = A.m_dataTable[row][col] * B;
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
            C.m_dataTable[i][j] = A.m_dataTable[i][j] - B.m_dataTable[i][j];
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
            C.m_dataTable[i][j] = A.m_dataTable[i][j] + B.m_dataTable[i][j];
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
            C.m_dataTable[i][j] = A.m_dataTable[i][j] / B.m_dataTable[i][j];
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
            B.m_dataTable[j][i] = A.m_dataTable[i][j];
    }

    return true;
}




#endif
