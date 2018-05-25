#ifndef AUTOPTR_H
#define AUTOPTR_H
#include <assert.h>

namespace GMapping{

template <class X>
class autoptr{
	protected:
	struct reference{
		unsigned int shares;
		X data;
	};
	
	public:
    inline autoptr();
		inline autoptr(const autoptr<X>& ap);
		inline autoptr& operator=(const autoptr<X>& ap);
		inline ~autoptr();
		inline operator int() const;
		inline X& operator*();
		inline const X& operator*() const;
    inline void create();
	protected:
		reference * m_reference;
};

template <class X>
autoptr<X>::autoptr(){
	m_reference=0;
}

template <class X>
autoptr<X>::autoptr(const autoptr<X>& ap){
	m_reference=0;
	if (ap.m_reference){
		m_reference=ap.m_reference;
		m_reference->shares++;
	}
}

template <class X>
autoptr<X>& autoptr<X>::operator=(const autoptr<X>& ap){
	reference* ref=ap.m_reference;
	if (m_reference==ref){
		return *this;
	}

	if (m_reference && !(--m_reference->shares)){
		delete m_reference;
		m_reference=0;
	}	

	if (ref) {
		m_reference=ref;
		m_reference->shares++;
	} else {
		m_reference=0;
  }
	return *this;
}

template <class X>
autoptr<X>::~autoptr(){
	if (m_reference && !(--m_reference->shares)){
		delete m_reference;
		m_reference=0;
	}	
}

template <class X>
void autoptr<X>::create(){
  if(!m_reference) {
    m_reference=new reference;
    m_reference->shares=1;
  }
}

template <class X>
autoptr<X>::operator int() const{
	return static_cast<bool>(m_reference);
}

template <class X>
X& autoptr<X>::operator*(){
	assert(m_reference);
	return m_reference->data;
}

template <class X>
const X& autoptr<X>::operator*() const{
	assert(m_reference);
	return m_reference->data;
}

};
#endif
