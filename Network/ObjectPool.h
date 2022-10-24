#pragma once
#include "MemoryPool.h"
#include <queue>
#include <cassert>

// 미리 오브젝트를 할당해놓는 오브젝트 풀
// 오브젝트풀에서 사용되는 오브젝트는 기본 생성자가 있어야함
template<typename ObjectType>
class ObjectPool
{
private: // member var
	MemoryPool				m_allocator;
	CircularQueue			m_objectList;

public: // default
	ObjectPool(size_t _maxObjectCount);
	~ObjectPool();

public: // member Func
	void ReturnToPool(ObjectType* _object);
	ObjectType* GetObject();
};

template<typename ObjectType>
inline ObjectPool<ObjectType>::ObjectPool(size_t _maxObjectCount)
	: m_allocator(sizeof(ObjectType)* _maxObjectCount, sizeof(ObjectType))
	, m_objectList(_maxObjectCount)
{
	for (size_t i = 0; i < _maxObjectCount; i++)
	{
		ObjectType* pObject = reinterpret_cast<ObjectType*>(m_allocator.Allocate());
		new(pObject) ObjectType();

		//ObjectType* pObject = new ObjectType;
		//m_objectList.push(pObject);

		m_objectList.Enqueue(reinterpret_cast<void*>(pObject));
	}
}

template<typename ObjectType>
inline ObjectPool<ObjectType>::~ObjectPool()
{
	while (m_objectList.IsEmpty() == false)
	{
		ObjectType* pObject = reinterpret_cast<ObjectType*>(m_objectList.Front());
		pObject->~ObjectType();

		m_objectList.Dequeue();
	}
}

template<typename ObjectType>
inline ObjectType* ObjectPool<ObjectType>::GetObject()
{
	assert(m_objectList.IsEmpty() == false);
	
	ObjectType* pObject = reinterpret_cast<ObjectType*>(m_objectList.Front());
	m_objectList.Dequeue();

	return pObject;
}

template<typename ObjectType>
inline void ObjectPool<ObjectType>::ReturnToPool(ObjectType* _object)
{
	m_objectList.Enqueue(_object);
}
