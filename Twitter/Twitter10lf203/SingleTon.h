#pragma once
#include <memory>
template <class T>
class SingleTon final
{
public:
	//disallow copy and move for this class, not needed
	SingleTon(const SingleTon&) = delete;
	SingleTon(SingleTon&&) = delete;
	SingleTon& operator=(const SingleTon&) = delete;
	SingleTon& operator=(SingleTon&&) = delete;
	static std::shared_ptr<T> GetInstance();
	~SingleTon() = default;
private:
	SingleTon() = default;
};

template<class T>
inline std::shared_ptr<T> SingleTon<T>::GetInstance()
{
	static std::shared_ptr<T> m_instance{ new T };
	return m_instance;
}
