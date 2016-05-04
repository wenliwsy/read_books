#include "resource_mgr.h"

int Resource::sCounter = 0;

bool ImageLoader::load()
{
	printf("load exec...\n");
	std::this_thread::sleep_for(chrono::seconds(3));

	return true;
}

ResourceManager::ResourceManager()
{
}

#define DESTROY_VECTOR(vec) \
	for (int k = 0; k < vec.size(); k++) \
	{ \
		if (vec[k]) \
		{ \
			delete vec[k]; \
			vec[k] = 0; \
		} \
	}

ResourceManager::~ResourceManager()
{
	DESTROY_VECTOR(toLoadVec);

	DESTROY_VECTOR(loadedVec);
}

void ResourceManager::removeResource(Resource* res)
{

}

void ResourceManager::addResourceRequest(Resource* res)
{
	mMutex.lock();
	
	toLoadVec.push_back(res);
	res->SetState(Loading);

	mMutex.unlock();
}

void ResourceManager::handleTasks()
{
	// release dead/

	Resource* missions[3];

	int num = 0;

	mMutex.lock();
	for (int k = 0; k < toLoadVec.size(); k++)
	{
		if (num >= 3)
			break;

		missions[num++] = toLoadVec[k];
	}

	toLoadVec.erase(toLoadVec.begin(), toLoadVec.begin() + num);
	mMutex.unlock();

	for (int k = 0; k < num; k++)
	{
		Resource* res = missions[k];
		if (res->getState() != Loading)
		{
			continue;
		}

		if (res->load())
		{
			res->SetState(Loaded);
		}

		loadedVec.push_back(res);
	}

	printf("exit data thread...\n");	// Not safe.

}

void ResourceManager::smart_test()
{
#if 0
	shared_ptr<int> sptr(new int(100));

	weak_ptr<int> wptr(sptr);

	auto ptest = wptr.lock();
	shared_ptr<int> xx = wptr.lock();

	int times = ptest.use_count();	// 3
#endif

	weak_ptr<int> wptr;

	{
		shared_ptr<int> sptr(new int(100));

		int val = *sptr;

		wptr = sptr;		// _Ptr, _Rep, 

		auto ptest = wptr.lock();
		shared_ptr<int> xx = wptr.lock();
		int times = sptr.use_count();

		bool isempty = wptr.expired();

	}

	bool isempty = wptr.expired();

	auto ptest = wptr.lock();
	int times = ptest.use_count();

	int val = *(wptr._Get());		// bad value.

}