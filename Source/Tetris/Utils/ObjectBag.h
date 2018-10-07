// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template< typename Type >
class TETRIS_API ObjectBag
{
public:
	ObjectBag() = default;

	// Add copies of passed in 'obj' to the bag
	void Add(const Type& obj, int32 copies = 1);

	// Add copies of every object in passed in 'objs' array to the bag
	void Add(const TArray< Type >& objs, int32 copies = 1);

	// Adds 'coun't number of default constructed objects to the bag
	void Add(int32 count);

	const Type& Pull();

private:

	TArray< Type > m_objectsPool;		// Pool to pull new objects from to place into bag
	TArray< Type > m_objectsPulled;		// All objects currently pulled into bag

	Type m_defaultObj;
};

template< typename Type >
void ObjectBag<Type>::Add(const Type& obj, int32 copies /*= 1*/)
{
	while (copies-- > 0)
	{
		m_objectsPool.Push(obj);
	}
}


template< typename Type >
void ObjectBag<Type>::Add(const TArray< Type >& objs, int32 copies /*= 1*/)
{
	while (copies-- > 0)
	{
		for (const Type& obj : objs)
		{
			m_objectsPool.Push(obj);
		}
	}
}

template< typename Type >
void ObjectBag<Type>::Add(int32 count)
{
	while (count-- > 0)
	{
		m_objectsPool(Type());
	}
}

template< typename Type >
const Type& ObjectBag<Type>::Pull()
{
	// Re-fill bag
	if (m_objectsPool.Num() == 0)
	{
		int32 count = m_objectsPulled.Num();
		m_objectsPool.Reserve(count);
		while (count-- > 0)
		{
			m_objectsPool.Push(m_objectsPulled.Pop());
		}
	}

	if (m_objectsPool.Num() == 0)
	{
		return m_defaultObj;
	}

	const int32 randIdx = FMath::RandRange(0, m_objectsPool.Num() - 1);

	m_objectsPulled.Push(m_objectsPool[randIdx]);
	m_objectsPool.RemoveAt(randIdx);

	const Type& outObject = m_objectsPulled.Last();

	

	return outObject;
}
