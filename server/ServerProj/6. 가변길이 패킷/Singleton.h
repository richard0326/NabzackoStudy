#pragma once

#define DECLARE_SINGLETON(type)				\
public:										\
static type** GetSingle(void)				\
{											\
	static type* pInstance = new type;		\
		if (pInstance == NULL)				\
			pInstance = new type;			\
	return &pInstance;						\
}											\
static void DestroyInstance()				\
{											\
	type** ppInstance = GetSingle();		\
if (*ppInstance != NULL)					\
	{										\
			delete *ppInstance;				\
			*ppInstance = NULL;				\
	}										\
}

#define SINGLETON(type) (*(type::GetSingle()))
#define DESTROY_SINGLETON(type) { (*(type::GetSingle()))->DestroyInstance(); }