#include <atomic>
#include <chrono>
#include <future>
#include <vector>

#include <Util/Concurrency/ConcurrentQueue.h>
#include <Util/Logging/Logger.h>

using std::async;
using std::atomic_bool;
using std::bind;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::future;
using std::launch;
using std::ref;
using std::vector;

using Util::Concurrency::ConcurrentQueue;

//=============================================================================
typedef int Object;
typedef ConcurrentQueue<Object> ObjectQueue;

//=============================================================================
bool RunEmptyQueueUponCreationTest()
{
	ObjectQueue objectQueue;
	ObjectQueue::size_type size = objectQueue.Size();

	if (size != 0)
	{
		LOGC("Empty queue has non-zero size %llu", size);
		return false;
	}
	else if (!objectQueue.IsEmpty())
	{
		LOGC("Empty queue reports as non-empty");
		return false;
	}

	return true;
}

//=============================================================================
bool RunPopFromEmptyQueueTest()
{
	ObjectQueue objectQueue;

	Object obj1;
	Object obj2(1);

	// Make sure pop is initially invalid
	if (objectQueue.Pop(obj1, milliseconds(0)))
	{
		LOGC("Incorrectly popped a valid object from an empty queue");
		return false;
	}

	// Push an item onto the queue and immediately pop it
	objectQueue.Push(obj2);

	if (!objectQueue.Pop(obj1, milliseconds(0)))
	{
		LOGC("Incorrectly did not pop an object from a non-empty queue");
		return false;
	}

	// Make sure popping an item from the no-longer non-empty queue is invalid
	objectQueue.Pop(obj1, milliseconds(0));

	if (objectQueue.Pop(obj1, milliseconds(0)))
	{
		LOGC("Incorrectly popped a valid object from an empty queue");
		return false;
	}

	return true;
}

//=============================================================================
bool DoQueuePush(ObjectQueue &objectQueue, const Object &object, ObjectQueue::size_type expectedSize)
{
	objectQueue.Push(object);
	ObjectQueue::size_type size = objectQueue.Size();

	if (size != expectedSize)
	{
		LOGC("Queue with %llu items has incorrect size %llu", expectedSize, size);
		return false;
	}
	else if (objectQueue.IsEmpty())
	{
		LOGC("Non-empty queue reports as empty");
		return false;
	}

	return true;
}

//=============================================================================
bool DoQueuePop(ObjectQueue &objectQueue, const Object &expectedObject, ObjectQueue::size_type expectedSize)
{
	Object object;
	ObjectQueue::size_type size = 0;

	if (!objectQueue.Pop(object, milliseconds(0)))
	{
		LOGC("Incorrectly did not pop an object from a non-empty queue");
		return false;
	}
	else if ((size = objectQueue.Size()) != expectedSize)
	{
		LOGC("Queue with %llu items has incorrect size %llu", expectedSize, size);
		return false;
	}
	else if (object != expectedObject)
	{
		LOGC("Item popped from queue was not expected object");
		return false;
	}

	return true;
}

//=============================================================================
bool RunSingleThreadedTest()
{
	ObjectQueue objectQueue;
	ObjectQueue::size_type size = 0;

	Object obj1(1);
	Object obj2(2);
	Object obj3(3);

	if (!DoQueuePush(objectQueue, obj1, ++size))
	{
		LOGC("First test push failed");
		return false;
	}

	if (!DoQueuePush(objectQueue, obj1, ++size))
	{
		LOGC("Second test push failed");
		return false;
	}

	if (!DoQueuePop(objectQueue, obj1, --size))
	{
		LOGC("First test pop failed");
		return false;
	}

	if (!DoQueuePush(objectQueue, obj2, ++size))
	{
		LOGC("Third test push failed");
		return false;
	}

	if (!DoQueuePush(objectQueue, obj3, ++size))
	{
		LOGC("Fourth test push failed");
		return false;
	}

	if (!DoQueuePop(objectQueue, obj1, --size))
	{
		LOGC("Second test pop failed");
		return false;
	}

	if (!DoQueuePop(objectQueue, obj2, --size))
	{
		LOGC("Third test pop failed");
		return false;
	}

	if (!DoQueuePop(objectQueue, obj3, --size))
	{
		LOGC("Fourth test pop failed");
		return false;
	}

	return true;
}

//=============================================================================
unsigned int WriterThread(ObjectQueue &objectQueue)
{
	unsigned int numWrites = 100;

	for (unsigned int i = 0; i < numWrites; ++i)
	{
		std::this_thread::sleep_for(milliseconds(10));

		Object object(i);
		objectQueue.Push(object);
	}

	return numWrites;
}

//=============================================================================
unsigned int ReaderThread(ObjectQueue &objectQueue, atomic_bool &finishedWrites)
{
	unsigned int numReads = 0;

	while (!finishedWrites.load() || !objectQueue.IsEmpty())
	{
		Object object;

		if (objectQueue.Pop(object, seconds(1)))
		{
			++numReads;
		}
	}

	return numReads;
}

//=============================================================================
bool RunMultiThreadedTest(unsigned int numWriters, unsigned int numReaders)
{
	ObjectQueue objectQueue;

	vector<future<unsigned int>> writerFutures;
	vector<future<unsigned int>> readerFutures;

	atomic_bool finishedWrites(false);

	for (unsigned int i = 0; i < numWriters; ++i)
	{
		auto func = bind(&WriterThread, ref(objectQueue));
		writerFutures.push_back(async(launch::async, func));
	}

	for (unsigned int i = 0; i < numReaders; ++i)
	{
		auto func = bind(&ReaderThread, ref(objectQueue), ref(finishedWrites));
		readerFutures.push_back(async(launch::async, func));
	}

	unsigned int numWrites = 0;
	unsigned int numReads = 0;

	for (auto &future : writerFutures)
	{
		if (future.valid())
		{
			numWrites += future.get();
		}
		else
		{
			LOGC("Invalid writer future found");
			return false;
		}
	}

	finishedWrites.store(true);

	for (auto &future : readerFutures)
	{
		if (future.valid())
		{
			numReads += future.get();
		}
		else
		{
			LOGC("Invalid reader future found");
			return false;
		}
	}

	LOGC("Finished %u writer / %u reader test with %u writes / %u reads",
		numWriters, numReaders, numWrites, numReads);

	return (numWrites == numReads);
}

//=============================================================================
int main()
{
	int numErrors = 0;

	if (RunEmptyQueueUponCreationTest())
	{
		LOGC("Empty queue upon creation test passed");
	}
	else
	{
		LOGC("Empty queue upon creation test failed");
		++numErrors;
	}

	if (RunPopFromEmptyQueueTest())
	{
		LOGC("Pop from empty queue test passed");
	}
	else
	{
		LOGC("Pop from empty queue test failed");
		++numErrors;
	}

	if (RunSingleThreadedTest())
	{
		LOGC("Single threaded test passed");
	}
	else
	{
		LOGC("Single threaded test failed");
		++numErrors;
	}

	if (RunMultiThreadedTest(1, 1))
	{
		LOGC("Single writer, single reader test passed");
	}
	else
	{
		LOGC("Single writer, single reader test failed");
		++numErrors;
	}

	if (RunMultiThreadedTest(1, 100))
	{
		LOGC("Single writer, multi reader test passed");
	}
	else
	{
		LOGC("Single writer, multi reader test failed");
		++numErrors;
	}

	if (RunMultiThreadedTest(100, 1))
	{
		LOGC("Multi writer, single reader test passed");
	}
	else
	{
		LOGC("Multi writer, single reader test failed");
		++numErrors;
	}

	if (RunMultiThreadedTest(100, 100))
	{
		LOGC("Multi writer, multi reader test passed");
	}
	else
	{
		LOGC("Multi writer, multi reader test failed");
		++numErrors;
	}

	return numErrors;
}
