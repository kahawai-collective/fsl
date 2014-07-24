#pragma once

#include <cstdlib>
#include <algorithm>

#include <boost/regex.hpp>

namespace Fsl {
namespace Estimation {

class Sample : public std::vector<double> {
public:
	Sample(void){}

	template<typename Type>
	Sample(Type data):std::vector<double>(data){};
};

class Samples : public std::vector<Sample> {
private:

	std::vector<std::string> labels_;
	Sample sample_;

public:

	Samples(void){}
	
	template<typename Type>
	Samples(Type data):std::vector<Sample>(data){};

	Sample random(void) {
		uint row = std::rand()%size();
		sample_ = operator[](row);
		return sample_;
	}

	double get(const std::string& label){
		uint column = std::find(labels_.begin(),labels_.end(),label)-labels_.begin();
		return sample_[column];
	}

	/**
	 * Read a file of tab separated values
	 * 
	 * @param  filename [description]
	 * @return          [description]
	 */
	Samples& read(const std::string& filename,bool header=true){
		std::ifstream file(filename);
		std::string line;

		boost::regex rgx("\\s+");
		boost::sregex_token_iterator end;
				
		if(header){
			std::getline(file, line);
			boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
			for( ; iter != end; ++iter) labels_.push_back(*iter);
		}

		while(true){
			std::getline(file, line);
			if(not file.good()) break;
			boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
			Sample sample;
			for( ; iter != end; ++iter) sample.push_back(boost::lexical_cast<float>(*iter));
			push_back(sample);
		}

		return *this;
	}

	void read_ss3(const std::string& filename){
		// Read in posterior
		std::ifstream file(filename);
		std::string line;

		// Get header
		std::getline(file, line);
		boost::regex rgx("\\s+");
		boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
		boost::sregex_token_iterator end;
		for( ; iter != end; ++iter) labels_.push_back(*iter);

		while(true){
			std::getline(file, line);
			if(not file.good()) break;
			boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
			Sample sample;
			for( ; iter != end; ++iter) sample.push_back(boost::lexical_cast<float>(*iter));
			push_back(sample);
		}
	}

	void write(const std::string& filename){
		std::ofstream file(filename);
		file.setf(std::ios::scientific);
		if(labels_.size()>0){
			for(auto label : labels_) file<<label<<"\t";
			file<<std::endl;
		}
		for(auto row : *this){
			for(auto value : row) file<<value<<"\t";
			file<<std::endl;
		}
	}

};

}
}
