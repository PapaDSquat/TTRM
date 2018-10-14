// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * Stores a fixed amount of objects of any Type into a bag, to be pulled out randomly.
 * 
 */
template< typename Type >
class TETRIS_API ObjectBag
{
public:
	ObjectBag( bool autoRefill = true );

	// Add copies of passed in 'obj' to the bag
	void Add(const Type& obj, int32 copies = 1);

	// Add copies of every object in passed in 'objs' array to the bag
	void Add(const TArray< Type >& objs, int32 copies = 1);

	// Adds 'count' number of default constructed objects to the bag
	void Add(int32 count);

	const Type& Pull();

	// Refill the bag with 
	void Refill();

	// Empties the bag
	void Empty();

private:
	TArray< Type > m_objectsPool;		// Pool to pull new objects from to place into bag
	TArray< Type > m_objectsPulled;		// All objects currently pulled into bag
	
	bool m_autoRefill;	// If true, the bag will be refilled automatically when all objects are pulled

	Type m_defaultObj;	// Default-constructed object returned if the bag is empty
};

template< typename Type >
ObjectBag<Type>::ObjectBag( bool autoRefill /*= true*/ )
	: m_autoRefill(autoRefill)
{
}

template< typename Type >
void ObjectBag<Type>::Refill()
{
	int32 count = m_objectsPulled.Num();
	if (count == 0)
		return;

	m_objectsPool.Reserve(count);
	while (count-- > 0)
	{
		m_objectsPool.Push(m_objectsPulled.Pop());
	}
}

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
	// Refill bag if empty
	if (m_objectsPool.Num() == 0)
	{
		Refill();
	}

	// Return default obj if the bag is empty
	if (m_objectsPool.Num() == 0)
	{
		return m_defaultObj;
	}

	// Move objects from Pool to Pulled
	const int32 randPoolIdx = FMath::RandRange(0, m_objectsPool.Num() - 1);
	m_objectsPulled.Push(m_objectsPool[randPoolIdx]);
	m_objectsPool.RemoveAt(randPoolIdx);

	return m_objectsPulled.Last();
}

template< typename Type >
void ObjectBag<Type>::Empty()
{
	m_objectsPool.Empty();
	m_objectsPulled.Empty();
}
