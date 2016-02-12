#pragma once

#include <string>
#include <type_traits>
#include "hspace.hpp"
#include "vspace.hpp"

class RuntimeRefBase {
public:
	enum class Spacer {
		None,
		HSpacer,
		VSpacer
	};
protected:
	Spacer spacer = Spacer::None;
public:
	Spacer getSpacer() const { return spacer; }
	virtual std::string toString() { return std::string(); }
	virtual ~RuntimeRefBase() {}
};

template <class Type>
class RuntimeRef : public RuntimeRefBase {
	Type& ref;
public:
	RuntimeRef(Type& ref) :
		ref(ref)
	{}
	
	virtual ~RuntimeRef() {}
	std::string toString() {
		std::stringstream s;
		s << ref;
		return s.str();
	}
};

template <class Type>
class ConstRuntimeRef : public RuntimeRefBase {
private:
	Type ref;
	
public:
	virtual ~ConstRuntimeRef() {}
	
	ConstRuntimeRef(const Type& ref) :
		ref(ref)
	{ 
		if(std::is_same<Type, HSpace>::value) {
			spacer = Spacer::HSpacer;
		}
		if(std::is_same<Type, VSpace>::value) {
			spacer = Spacer::VSpacer;
		}
	}
	
	std::string toString() {
		std::stringstream s;
		s << ref;
		return s.str();
	}
	
	Type& getRef() {
		return ref;
	};
};