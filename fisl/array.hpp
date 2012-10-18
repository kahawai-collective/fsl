#pragma once

#include <vector>
#include <algorithm>

namespace Fisl {
    
template<class Type=double, int Dimension1=0, int Dimension2=0, int Dimension3=0, int Dimension4=0, int Dimension5=0> class Array;

template<class Type>
class Array<Type> {
public:
	std::vector<Type> values;
	
	Array(unsigned int size=0){
		resize(size);
	}
	
	void operator=(std::initializer_list<Type> values_){
		unsigned int index = 0;
		for(auto iter=values_.begin();iter!=values_.end();iter++){
			if(index<size()) values[index] = *iter;
			else values.push_back(*iter);
			index++;
		}
	}
	
	unsigned int size(void) const {
		return values.size();
	}
	
	void resize(unsigned int size) {
		return values.resize(size);
	}
	
	void append(const Type& item) {
		return values.push_back(item);
	}
	
	Type& operator()(int index){
		return values[index];
	}
	
	const Type& operator()(int index) const {
		return values[index];
	}
    
    double sum(void){
        double sum = 0;
        std::for_each(values.begin(),values.end(),[&](double value){
            sum += value;
        });
        return sum;
    }
};

template<class Type,int Dimension1>
class Array<Type,Dimension1> {
public:
	Type values[Dimension1];
	
	void operator=(std::initializer_list<Type> values_){
		unsigned int index = 0;
		for(auto iter=values_.begin();iter!=values_.end();iter++){
			values[index++] = *iter;
		}
	}
	
	unsigned int size(void) const {
		return Dimension1;
	}
	
	Type& operator()(int index){
		return values[index];
	}
	
	const Type& operator()(int index) const {
		return values[index];
	}
    
    double sum(void){
        double sum = 0;
        std::for_each(values,values+size(),[&](double value){
            sum += value;
        });
        return sum;
    }
};

template<class Type,int Dimension1, int Dimension2>
class Array<Type,Dimension1,Dimension2> {
private:
	Type values[Dimension1][Dimension2];
	
public:
	unsigned int size(void) const {
		return Dimension1*Dimension2;
	}
	
	Type& operator()(int index1,int index2){
		return values[index1][index2];
	}
	
	const Type& operator()(int index1,int index2) const {
		return values[index1][index2];
	}
    
    double sum(void){
        double sum = 0;
        std::for_each(values,values+size(),[&](double value){
            sum += value;
        });
        return sum;
    }
};

}

