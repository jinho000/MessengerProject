#pragma once

template <class T>
class Singleton
{
private:
	static T* pInstance;

protected: // default
	Singleton();
	virtual ~Singleton() = 0;

	Singleton(const Singleton& _other) = delete;
	Singleton(Singleton&& _other) = delete;
	Singleton& operator=(const Singleton& _other) = delete;
	Singleton& operator=(const Singleton&& _other) = delete;

public: // member Func
	static void CreateInstance();
	static void Destroy();
	static T* GetInst();
};

template<class T>
T* Singleton<T>::pInstance = nullptr;

template<class T>
inline Singleton<T>::Singleton()
{
}


template<class T>
inline Singleton<T>::~Singleton()
{
}

template<class T>
inline void Singleton<T>::CreateInstance()
{
	if (pInstance != nullptr)
		return;

	pInstance = new T;
}

template<class T>
inline void Singleton<T>::Destroy()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

template<class T>
inline T* Singleton<T>::GetInst()
{
	return pInstance;
}
