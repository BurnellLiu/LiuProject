/// Author:Burnell_Liu Email:674288799@qq.com Date: 2014/08/27
/// Description: �Ż��㷨
/// 
/// �Ŵ��㷨�Ż�
/// ��ɽ�㷨�Ż�
/// ģ���˻��㷨�Ż�
///
/// Ӧ���Ż��㷨����:1.����Ľ����ת��Ϊ�����б� 2. ���ڽ����, ���Ž�Ӧ�ýӽ����������Ž�, ����û���Ż�
/// Others: 
/// Function List: 
///
/// History: 
///  1. Date, Author 
///  Modification
///


#ifndef _LOPTIMIZATION_H_
#define _LOPTIMIZATION_H_

#include "LDataStruct/include/LArray.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief ����Χ�ṹ
struct LOGeneDomain
{
    int Min; ///< ��Сֵ
    int Max; ///< ���ֵ
};

typedef LArray<int> LOGenome; ///< ������
typedef LArray<LOGeneDomain>  LOGenomeDomain; ///< �����鷶Χ
typedef LArray<int> LGOCrossOverSplitPointList; ///< ����ָ���б�

/// @brief ��ṹ
struct LOSolution
{
    static const int MAX_COST; ///< ���ɱ�

    LOGenome* PGenome; ///< ������
    int Cost; ///< ��ɱ�
    
    /// @brief ����<������, ʹ�ýṹ�ɱ�������ʹ��
    bool operator < (const LOSolution& other)
    {
        return this->Cost < other.Cost;
    }
};

/// @brief �������
class LOSolutionMutate
{

public:
    LOSolutionMutate();
    virtual ~LOSolutionMutate();

    /// @brief ���ø���
    /// @param[in] prob �������, ��ΧΪ0~1�ĸ�����
    /// @return �������󷵻�false
    bool SetProb(IN float prob);

    /// @brief ���ò���
    /// @param[in] step ���첽��, ����Ϊ���ڵ���1������
    /// @return �������󷵻�false
    bool SetStep(IN int step);

    /// @brief ����
    /// @param[in] genomeDomain �����鷶Χ
    /// @param[in] solution ��Ҫ����Ľ�
    /// @return �������󷵻�false
    bool Mutate(IN const LOGenomeDomain& genomeDomain, INOUT LOSolution& solution) const;

private:
    const static int ERROR_STEP; ///< ���󲽳�
    const static float ERROR_PROB; ///< �������
private:
    int m_step; ///< ���첽��
    float m_prob; ///< �������
private:
    // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
    LOSolutionMutate(const LOSolutionMutate&);
    LOSolutionMutate& operator = (const LOSolutionMutate&);
};

/// @�⽻����
class LOSolutionCrossOver
{
public:
    LOSolutionCrossOver();
    virtual ~LOSolutionCrossOver();

    /// @brief ���ý����и���б�
    /// 
    /// ����ʹ��Init()����ǰ,���ñ��첽��
    /// @param[in] splitPointList �и���б�
    /// @return true
    bool SetSplitPointList(IN const LGOCrossOverSplitPointList& splitPointList);

    /// @brief ����������н�����������µĽ�
    /// @return true
    bool CrossOver(IN const LOSolution& solutionOld1, IN const LOSolution& solutionOld2, 
        OUT LOSolution& solutionNew1, OUT LOSolution& solutionNew2) const;

private:
    LGOCrossOverSplitPointList m_splitPointList; ///< �����и���б�
private:
    // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
    LOSolutionCrossOver(const LOSolutionCrossOver&);
    LOSolutionCrossOver& operator = (const LOSolutionCrossOver&);
};

/// @brief �ɱ������ӿ�
class LOCostFunInterface
{
public:
    virtual ~LOCostFunInterface(){};

    /// @brief ���������ĳɱ�
    ///
    /// Խ��Ļ�����, ���صĳɱ�Խ��
    /// @param[in] genome ������
    /// @return �ɱ�
    virtual int CalculateGenomeCost(IN LOGenome& genome) = 0;
};

/// @brief �Ŵ��㷨�Ż��������
class LGeneticOptimize
{
public:
    LGeneticOptimize();
    virtual ~LGeneticOptimize();

public:
    /// @brief ������Ⱥ��С
    ///
    /// ����ʹ��Init()����ǰ,������Ⱥ��С
    /// @param[in] popSize ��Ⱥ��С, ��ΧΪ����0������, Ĭ��ֵ(200)
    /// @return �������󷵻�false
    bool SetPopSize(IN int popSize);

    /// @brief ����ÿһ��ֱ��ʤ���ߵı���
    ///
    /// ����ʹ��Init()����ǰ,����ʤ���߱���
    /// @param[in] elitePer ʤ���߱���, ��ΧΪ0~1�ĸ�����, Ĭ��ֵ(0.2)
    /// @return �������󷵻�false
    bool SetElitePercent(IN float elitePer);

    /// @brief ���ñ������
    ///
    /// ����ʹ��Init()����ǰ,���ñ������
    /// @param[in] mutateProb �������, ��ΧΪ0~1�ĸ�����, Ĭ��ֵ(0.02)
    /// @return �������󷵻�false
    bool SetMutateProb(IN float mutateProb);

    /// @brief ���ñ��첽��
    ///
    /// ����ʹ��Init()����ǰ,���ñ��첽��
    /// @param[in] mutateStep ���첽��, ����Ϊ���ڵ���1������, Ĭ��ֵ(1)
    /// @return �������󷵻�false
    bool SetMutateStep(IN int mutateStep);

    /// @brief ���ý����и���б�
    /// 
    /// ����ʹ��Init()����ǰ,���ñ��첽��
    /// @param[in] splitPointList �и���б�
    /// @return true
    bool SetCrossOverSplitPointList(IN const LGOCrossOverSplitPointList& splitPointList);

    /// @brief ��ʼ��
    /// @param[in] pCostFun �ɱ�����
    /// @param[in] genomeDomain �����鷶Χ
   /// @return �������󷵻�false
    virtual bool Init(IN LOCostFunInterface* pCostFun, IN const LOGenomeDomain& genomeDomain) = 0;

protected:
    /// @brief ��ʼ��Ⱥ
    /// @return true
    bool InitPopulation();

    /// @brief ��Ⱥ����
    /// @return true
    bool PopulationCompete();

    /// @brief ����ѡ��
    /// @return ��������
    int RouletteWheelSelection();

    /// @brief ������Դ
    /// @return true
    bool CleanUp();

protected:
    typedef LArray<LOSolution> LSolutionList; // ���б�

    LOSolutionMutate m_solutionMutate; ///< �����
    LOSolutionCrossOver m_solutionCrossOver; ///< �⽻��

    float m_elitePer; ///< ʤ���߱���
    int m_popSize; ///< ��Ⱥ��С

    int m_genomeLength; ///< �����鳤��
    LSolutionList m_solutionList; ///< ���б�(��Ⱥ)
    LSolutionList m_solutionListCopy; ///< ���б���
    LOGenomeDomain m_genomeDomain; ///< �����鷶Χ

    LOCostFunInterface* m_pCostFun; ///< �ɱ������ӿ�ָ��

    bool m_bInitSuccess; ///< ��ʼ���ɹ�

private:
    // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
    LGeneticOptimize(const LGeneticOptimize&);
    LGeneticOptimize& operator = (const LGeneticOptimize&);
};

/// @brief ����ʽ�Ŵ��㷨
///
/// ����ʽ�Ŵ��㷨�Գɱ������ӿ�ָ�벻��Ҫ��
/// ����ɱ������ӿ�ָ��Ϊ��ʱ, ��ô����Breed()֮ǰʹ��GetSolution(int)�ֶ����óɱ�
class LOpenedGenetic : public LGeneticOptimize
{
public:
    /// @brief ��ʼ��
    /// @param[in] pCostFun �ɱ������ӿ�ָ��, ����Ϊ��
    /// @param[in] genomeDomain �����鷶Χ, �����鷶Χ��ÿ�����򶼱���MinС�ڵ���Max
    /// @return �������󷵻�false
    virtual bool Init(IN LOCostFunInterface* pCostFun, IN const LOGenomeDomain& genomeDomain);

    /// @brief ��Ⱥ�ݻ�һ��
    /// @return ��ʼ�����󷵻�false
    bool Breed();

    /// @brief ��ȡ��Ⱥ�е�һ����
    /// @param[in] �������, ��ע�������ķ�Χ(0 ~ ��Ⱥ��С-1), �ú�������������Χ���м��
    /// @return �������
    LOSolution& GetSolution(IN int index);

};

/// @brief ���ʽ�Ŵ��㷨
class LClosedGenetic : public LGeneticOptimize
{
public:
    /// @brief ��ʼ��
    /// @param[in] pCostFun �ɱ������ӿ�ָ��, ������Ϊ��
    /// @param[in] genomeDomain �����鷶Χ, �����鷶Χ��ÿ�����򶼱���MinС�ڵ���Max
    /// @return �������󷵻�false
    virtual bool Init(IN LOCostFunInterface* pCostFun, IN const LOGenomeDomain& genomeDomain);

    /// @brief ��Ⱥ�ݻ����
    ///
    /// �ú������ܺܺ�ʱ(�����ݸ��Ӷ��Լ��ɱ�����Ч�ʵ�Ӱ��)
    /// @param[in] times �ݻ�����, �ݻ�����Ϊ���ڵ���1������
    /// @param[out] bestSolution ���Ž�, ��������Ž���Ҫ�Ŀռ�
    /// @return ����������߳�ʼ�����󷵻�false
    bool BreedEx(IN int times, OUT LOSolution&bestSolution);
};



/// @brief �Ż��㷨�������
class LOptimize
{
public:
    LOptimize();
    virtual ~LOptimize();
public:
    /// @brief ���ò���
    ///
    /// ����ʹ��Init()����ǰ,���ò���
    /// �������㷨������ռ���ٶȳɷ���, ���Ǵ�Ĳ������ܻ��������Ž�
    /// @param[in] mutateStep ����, ����Ϊ���ڵ���1������, Ĭ��ֵ(1) 
    /// @return �������󷵻�false
    bool SetStep(IN int step);

    /// @brief ��ʼ��
    /// @param[in] pCostFun �ɱ������ӿ�, �ɱ������ӿ�ָ�벻��Ϊ��
    /// @param[in] genomeDomain �����鷶Χ, �����鷶Χ��ÿ������ı���MinС�ڵ���Max
    /// @return �������󷵻�false
    bool Init(IN LOCostFunInterface* pCostFun, IN const LOGenomeDomain& genomeDomain);

    /// @brief �������Ž�
    ///
    /// �ú������ܺܺ�ʱ(�����ݸ��Ӷ��Լ��ɱ�����Ч�ʵ�Ӱ��)
    /// @param[out] bestSolution ���Ž�,  ������������Ҫ�Ŀռ�, 
    /// @return ����������߳�ʼ�����󷵻�false
    virtual bool Search(INOUT LOSolution& bestSolution) = 0; 

protected:
    int m_step; ///< ����
    int m_genomeLength; ///< �����鳤��
    LOGenomeDomain m_genomeDomain; ///< �����鷶Χ
    LOCostFunInterface* m_pCostFun; ///< �ɱ������ӿ�ָ��

    bool m_bInitSuccess; ///< ��ʼ���ɹ�

private:
    // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
    LOptimize(const LOptimize&);
    LOptimize& operator = (const LOptimize&);
};

/// @brief ��ɽ���Ż�
class LClimbHillOptimize : public LOptimize
{
public:
    /// @brief �������Ž�
    ///
    /// �ú������ܺܺ�ʱ(�����ݸ��Ӷ��Լ��ɱ�����Ч�ʵ�Ӱ��)
    /// @param[out] bestSolution ���Ž�,  ��������Ž���Ҫ�Ŀռ�, 
    /// @return ����������߳�ʼ�����󷵻�false
    virtual bool Search(OUT LOSolution& bestSolution); 

    /// @brief �����ɽ�������Ž�
    ///
    /// �ú������ܺܺ�ʱ(�����ݸ��Ӷ��Լ��ɱ�����Ч�ʵ�Ӱ��)
    /// @param[in] times ��������, ����Ϊ���ڵ���1������
    /// @param[out] bestSolution ���Ž�, ��������Ž���Ҫ�Ŀռ�
    /// @return ����������߳�ʼ�����󷵻�false
    bool SearchEx(IN int times, OUT LOSolution&bestSolution);
};

/// @ģ���˻��㷨�Ż�
class LAnnealingOptimize : public LOptimize
{
public:
    LAnnealingOptimize();
    virtual ~LAnnealingOptimize();

    /// @brief ���ó�ʼ�¶�
    /// @param[in] startTemp ��ʼ�¶�, Ҫ�����0.1�ĸ�������, Ĭ��ֵ(10000.0)
    /// @return �������󷵻�false
    bool SetStartTemperature(IN float startTemp);

    /// @brief �����¶���ȴ�ٶ�
    ///
    /// ��ȴ�ٶ�Խ��, �¶Ƚ���Խ��
    /// @param[in] coolSpeed ��ȴ�ٶ�, Ҫ�����0С��1�ĸ�����, Ĭ��ֵ(0.05)
    /// @return �������󷵻�false
    bool SetCoolSpeed(IN float coolSpeed);

    /// @brief �������Ž�
    ///
    /// �ú������ܺܺ�ʱ(�����ݸ��Ӷ��Լ��ɱ�����Ч�ʵ�Ӱ��)
    /// @param[out] bestSolution ���Ž�,  ��������Ž���Ҫ�Ŀռ�, 
    /// @return ����������߳�ʼ�����󷵻�false
    virtual bool Search(OUT LOSolution& bestSolution); 

private:
    float m_coolSpeed; ///< �¶ȵ���ȴ�ٶ�
    float m_startTemp; ///< ��ʼ�¶�
private:
    // ��ֹĬ�ϸ�ֵ�������Ϳ������캯��
    LAnnealingOptimize(const LAnnealingOptimize&);
    LAnnealingOptimize& operator = (const LAnnealingOptimize&);
};

#endif