// This program will create 2 threads: one thread increases the variable continuously and 
// then sleep 1 second. One thread decreases the variable to 1 continuously and then sleep 2 seconds

#include <iostream>
#include <unistd.h>

using namespace std;

class Mutex
{
private:
	int mVar;
	// pthread_t myThread[2];
	pthread_mutex_t mMutex;
public:
	void CreateThread(pthread_t *threadID, void* (*threadFunc)(void*));
	Mutex(int mVarIn);
	void CreateMutex();
	void* IncreaseVar();
	static void* StaticIncreaseVar(void *);
	void DecreaseVar();
};

void* Mutex::StaticIncreaseVar(void *context)
{
	return ((Mutex *) context)->IncreaseVar();
}


void* Mutex::IncreaseVar()
{
	for(int i = 0; i < 5; i++)
	{
		// lock the mutex
		pthread_mutex_lock(&mMutex);

		// increase variable
		mVar++;

		cout << "[IncreaseVar]: mVar = " << mVar << endl;
		sleep(1);

		// unlock the mutex
		pthread_mutex_unlock(&mMutex);
	}
	
	pthread_exit(NULL);
}

void Mutex::CreateThread(pthread_t *threadID, void* (*threadFunc)(void*))
{
	pthread_create(threadID, NULL, threadFunc, NULL);
}

Mutex::Mutex(int mVarIn)
{
	this->mVar = mVarIn;
}

void Mutex::CreateMutex()
{
	pthread_mutex_init(&mMutex, NULL);
}


void Mutex::DecreaseVar()
{
	// lock the mutex
	pthread_mutex_lock(&mMutex);

	// decrease variable
	mVar--;

	cout << "[DecreaseVar]: mVar = " << mVar << endl;

	// unlock the mutex
	pthread_mutex_unlock(&mMutex);
}


void* DoThread(void *)
{
	cout << "hello\n";

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int retVal;
	pthread_t myThread[2];
	Mutex *myMutex;
	myMutex = new Mutex(3);

	myMutex->CreateMutex();

	pthread_create(&myThread[0], NULL, &Mutex::StaticIncreaseVar, NULL);

	//retVal = pthread_join(myThread[0], NULL);

	// if(retVal != 0)
	// {
	// 	cout << "error\n";
	// }

	return 0;
}

