#ifndef H_PixyUtility_H
#define H_PixyUtility_H

#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <malloc.h>

namespace Pixy {

	class BadConversion : public std::runtime_error {
	public:
		inline BadConversion(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	class BadStream : public std::runtime_error {
	public:
		inline BadStream(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	class UnknownEvent : public std::runtime_error {
	public:
		inline UnknownEvent(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	class UnknownEventSender : public std::runtime_error {
	public:
		inline UnknownEventSender(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	class BadEvent : public std::runtime_error {
	public:
		inline BadEvent(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	class UnassignedProfile : public std::runtime_error {
	public:
		inline UnassignedProfile(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	class UnknownSpell : public std::runtime_error {
	public:
		inline UnknownSpell(const std::string& s)
		: std::runtime_error(s)
		{ }
	};

	template<typename T>
	inline std::string stringify(const T& x)
	{
		std::ostringstream o;
		if (!(o << x))
			throw BadConversion(std::string("stringify(")
								+ typeid(x).name() + ")");
		return o.str();
	}

	// helper; converts an integer-based type to a string
	template<typename T>
	inline void convert(const std::string& inString, T& inValue,
						bool failIfLeftoverChars = true)
	{
		std::istringstream _buffer(inString);
		char c;
		if (!(_buffer >> inValue) || (failIfLeftoverChars && _buffer.get(c)))
			throw BadConversion(inString);
	}

	template<typename T>
	inline T convertTo(const std::string& inString,
					   bool failIfLeftoverChars = true)
	{
		T _value;
		convert(inString, _value, failIfLeftoverChars);
		return _value;
	}

	inline std::string raceToString(Pixy::RACE inRace) {
		return
		(inRace == EARTH) ? "Earth" :
		(inRace == AIR) ? "Air" :
		(inRace == FIRE) ? "Fire" : "Water";
	}

	inline Pixy::RACE raceFromString(std::string inRace) {
		return
		(inRace == "Earth") ? EARTH :
		(inRace == "Air") ? AIR :
		(inRace == "Fire") ? FIRE : WATER;
	}

	inline std::string alignmentToString(Pixy::PUPPET_ALIGNMENT inAlignment) {
		return
		(inAlignment == H_NEUTRAL) ? "Neutral" :
		(inAlignment == H_GOOD) ? "Good" : "Evil";
	}

	inline Pixy::PUPPET_ALIGNMENT alignmentFromString(std::string inAlignment) {
		return
		(inAlignment == "Neutral") ? H_NEUTRAL :
		(inAlignment == "Good") ? H_GOOD : H_EVIL;
	}

	class Utility {
	public:
	static inline log4cpp::Category& getLogger() {
		return log4cpp::Category::getInstance(PIXY_LOG_CATEGORY);
	}

  /* splits a string s using the delimiter delim */
  inline static
  std::vector<std::string> split(const std::string &s, char delim) {
      std::vector<std::string> elems;
      std::stringstream ss(s);
      std::string item;
      while(std::getline(ss, item, delim)) {
          elems.push_back(item);
      }
      return elems;
  }

  inline static
  std::string join(const std::vector<std::string> &strings, char delim) {
    std::string str="";
    std::vector<std::string>::const_iterator itr;
    for (itr = strings.begin(); itr != strings.end(); ++itr)
      str += (*itr) + delim;

    return str.erase(str.size()-1,1);
  }

  inline static
  bool fileToStream(char **out, const char* path) {
    FILE* fh = fopen( path, "rb" );
    if (fh==NULL) {
      std::cerr
        << "file error! couldn't open file for reading @ "
        << path << ". err: " << stderr << "\n";
      return false;
    }
    // obtain file size:
    fseek (fh , 0 , SEEK_END);
    long lSize = ftell (fh);
    rewind (fh);

    (*out) = (char*) malloc(sizeof(char)*lSize);
    if (!(*out)) {
      std::cerr
        << "not enough memory to allocate buffer for reading @ "
        << path << ". err: " << stderr << "\n";

      fclose(fh);
      return false;
    }

    size_t bytesRead = fread (*out,1,lSize,fh);
    if (bytesRead != lSize) {
      std::cerr << "reading error: " << stderr << "\n";
      free(*out);
      fclose(fh);
      return false;
    }

    fclose(fh);
    return true;
  }

	};
}

#endif
