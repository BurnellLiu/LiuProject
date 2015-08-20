
#ifndef _LDATASTRUCT_DEFINE_H_
#define _LDATASTRUCT_DEFINE_H_

#include <assert.h>

#ifndef LTEMPLATE
#define LTEMPLATE template<typename Type>
#endif

#ifndef LTEMPLATE_T
#define LTEMPLATE_T template<typename T>
#endif

#ifndef LSQLIST_MAX_LENGTH
#define LSQLIST_MAX_LENGTH 1024      ///< ˳���б���󳤶�
#endif

#ifndef LSQSTACK_MAX_LENGTH
#define LSQSTACK_MAX_LENGTH 1024    ///< ˳��ջ��󳤶�
#endif

#ifndef LSQQUEUE_MAX_LENGTH
#define LSQQUEUE_MAX_LENGTH 1024   ///< ˳�������󳤶�
#endif

#ifndef LSQTREE_MAX_LENGTH
#define LSQTREE_MAX_LENGTH 1024     ///< ˳������󳤶�
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do {if (p) {delete (p); (p) = NULL;}}while(0)
#endif

#ifndef LOK
#define LOK 0
#endif

#ifndef LFAIL
#define LFAIL -1
#endif

#ifndef NULL
#define NULL 0
#endif

typedef int LIndex;
typedef int LStatus;


#endif