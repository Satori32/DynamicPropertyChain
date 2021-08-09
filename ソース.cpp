#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <tuple>

#define interface struct

template <class T>
interface IProperty
{
	virtual std::string ID() { return "IProperty"; }
	virtual bool Check(const T&) = 0;
	typedef std::shared_ptr<IProperty<T>> SharedProterty;
};

template <class T>
class Filter {
public:

	typedef T Value;

	bool Push(std::shared_ptr<IProperty<T>> In) {//if you need multi type. to use std::tuple<A, B, C, ...>.or vector.
		IP.push_back(In);
		return true;
	}

	std::tuple<size_t, std::size_t> Check(const T& In) {
		std::size_t Tr = 0;
		std::size_t Fa = 0;
		for (auto& o : IP) {
			o->Check(In) ? Tr++ : Fa++;
		}

		return { Tr,Fa };
	}

	std::size_t Size() {
		return IP.size();
	}
	
protected:
	std::vector<std::shared_ptr<IProperty<T>>> IP;
};
template<class T>
class One :public IProperty<T>{
	std::string ID() { return "One"; }
	bool Check(const T& In) { return In % 2; }
};
template<class T>
class Div16 :public IProperty<T>{
	std::string ID() { return "Div16"; }
	bool Check(const T& In) { ; return ((In/16)&1)==1; }
};

template<class T>
class Mod256 :public IProperty<T>{
	std::string ID() { return "Mod256"; }
	bool Check(const T& In) {  return (In&256)==0; }
};

int main() {
	Filter<std::size_t> F;
	IProperty<decltype(F)::Value>::SharedProterty O = std::make_shared<One<decltype(F)::Value>>();
	IProperty<std::size_t>::SharedProterty D = std::make_shared<Div16<std::size_t>>();	
	IProperty<std::size_t>::SharedProterty M = std::make_shared<Mod256<std::size_t>>();

	F.Push(O);
	F.Push(D);
	F.Push(M);
	{
		std::size_t X;
		auto R=F.Check(X);

		std::cout << X << ':' << std::get<0>(R) << ',' << std::get<1>(R) << std::endl;
	}

	return 0;
}