
#ifndef _LDOCCLASSIFY_H_
#define _LDOCCLASSIFY_H_

#include <string>
#include <map>
#include <set>
using std::string;
using std::map;
using std::set;

#include "LDataStruct/LArray.h"


/// @�ĵ����
enum LDOC_CATEGORY
{
    LDOC_CAT_BAD = 0,
    LDOC_CAT_GOOD = 1,
    LDOC_CAT_UNKNOWN
};


/// @brief �ĵ�����
class LDocCategory
{
public:
    LDocCategory();
    virtual ~LDocCategory();
public:
    /// @brief ����ָ������ļ���
    /// @param[in] cat
    /// @return �������󷵻�false
    bool IncCount(LDOC_CATEGORY cat);

    /// @brief ��ȡָ������ļ���
    /// @param[in] LDOC_CATEGORY cat
    /// @return �������󷵻�false
    int GetCount(LDOC_CATEGORY cat);

    /// @brief ��ȡ���з�����ܼ���
    /// @return �ܼ���
    int GetTotalCount();

private:
    int m_goodCounts; ///< GOOD����ļ���
    int m_badCounts; ///< BAD����ļ���
private:
    //LDocCategory(const LDocCategory&);
   // LDocCategory& operator = (const LDocCategory&);
};

/// @brief �ĵ�����
class LDocFeature
{
public:
    LDocFeature();
    virtual ~LDocFeature();

    /// @brief ���Ӹ�������ָ�������еļ���
    /// @param[in] cat ָ������
    /// @return �������󷵻�false
    bool IncCategoryCount(LDOC_CATEGORY cat);

    /// @brief ��ȡ������ָ�������еļ���
    /// @param[in] cat ָ������
    /// @return 
    int GetCategoryCount(LDOC_CATEGORY cat);

    /// @brief ��ȡ���������з����е��ܼ���
    /// @return �ܼ���
    int GetCategoryTotalCount();

private:
    LDocCategory m_docCategory; ///< �����������ĵ�����

private:
    //LDocFeature(const LDocFeature&);
    //LDocFeature& operator = (const LDocFeature&);
};



/// @brief �ĵ�������
class LDocClassifier
{
public:
    LDocClassifier();
    virtual ~LDocClassifier();
    
public:
    /// @brief ѵ��������
    /// @param[in] text �ĵ�(Ҫ�󵥴ʼ��Կո����)
    /// @param[in] cat �ĵ������
    /// @return �������󷵻�false
    bool Train(const string& text, LDOC_CATEGORY cat);

    /// @brief ��ȡָ���ĵ�����ĳ������ĸ���
    /// @param[in] text �ĵ�
    /// @param[in] cat ����
    /// @return ���� �� Pr(Category | Document )
    virtual float GetCatgoryProbInDoc(const string& text, LDOC_CATEGORY cat) = 0;

    /// @brief ���ĵ����з���
    /// @param[in] text
    /// @return �ĵ������
    LDOC_CATEGORY Classify(const string& text);

protected:
    /// @brief �Ӹ������ı��л�ȡ����(����ͬ�ĵ���)
    /// @param[in] text �ı�
    /// @param[in] featuresSet ���صĵ��ʼ�
    /// @return true
    bool GetFeatures(const string& text, set<string>& featuresSet);

    /// @brief ������ָ���ķ����г��ֵĸ���
    /// @param[in] feature ����
    /// @param[in] cat ����
    /// @return ����(��Χ[0, 1]) �� Pr(Feature| Category)
    float GetFeatureProbInCat(const string& feature, LDOC_CATEGORY cat);

    // @brief ������ָ���ķ����г��ֵ�Ȩ�ظ���
    ///
    /// ���и���ֵ����0.5����ʼֵ��ѵ������������������ֵ�仯
    /// @param[in] feature ����
    /// @param[in] cat ����
    /// @return ����(��Χ[0, 1]) �� Pr(Feature| Category)
    float GetFeatureWeightedProbInCat(const string& feature, LDOC_CATEGORY cat);

protected:
    map<string, LDocFeature> m_featureMap; ///< �����ֵ�
    LDocCategory m_docCategoryTotal; ///< �ܵ��ĵ�����

    LArray<LDOC_CATEGORY> m_docCatList; ///< �ĵ������б�

private:
    LDocClassifier(const LDocClassifier&);
    LDocClassifier& operator = (const LDocClassifier&);
};

/// @brief ���ر�Ҷ˹������
class LNaiveBayesClassifier : public LDocClassifier
{
public:
    LNaiveBayesClassifier();
    ~LNaiveBayesClassifier();
public:
    /// @brief ��ȡָ���ĵ�����ĳ������ĸ���
    /// @param[in] text �ĵ�
    /// @param[in] cat ����
    /// @return ���� �� Pr(Category | Document )
    virtual float GetCatgoryProbInDoc(const string& text, LDOC_CATEGORY cat);

private:
    /// @brief ��ȡָ�������³���ĳ���ĵ��ĸ���
    /// @param[in] text �ĵ�
    /// @param[in] cat ����
    /// @return ���� �� Pr(Document | Category)
    float GetDocProbInCat(const string& text, LDOC_CATEGORY cat);

private:
    LNaiveBayesClassifier(const LNaiveBayesClassifier&);
    LNaiveBayesClassifier& operator = (const LNaiveBayesClassifier&);
};

/// @brief  �����������
class LFisherClassifier : public LDocClassifier
{
public:
    LFisherClassifier();
    ~LFisherClassifier();
public:
    /// @brief ��ȡָ���ĵ�����ĳ������ĸ���
    /// @param[in] text �ĵ�
    /// @param[in] cat ����
    /// @return ���� �� Pr(Category | Document )
    virtual float GetCatgoryProbInDoc(const string& text, LDOC_CATEGORY cat);

private:
    /// @brief ��ȡָ����������ĳ������ĸ���
    ///
    /// ��һ������
    /// @param[in] feature ����
    /// @param[in] cat ����
    /// @return ���� �� Pr(Category | Feature )
    float GetCatgoryProbInFea(const string& feature, LDOC_CATEGORY cat);

    /// @brief ��ȡָ����������ĳ�������Ȩ�ظ���
    ///
    /// ��һ������
    /// @param[in] feature ����
    /// @param[in] cat ����
    /// @return ���� �� Pr(Category | Feature )
    float GetCatgoryWeightProbInFea(const string& feature, LDOC_CATEGORY cat);

public:
    /// @brief
    /// @param[in] chi
    /// @param[in] df
    /// @return 
    float Inchi2(float chi, int df);
private:
    LFisherClassifier(const LFisherClassifier&);
    LFisherClassifier& operator = (const LFisherClassifier&);
};

#endif