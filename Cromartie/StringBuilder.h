#pragma once

#include <string>
#include <sstream>

class StringBuilder
{
public:
    template<typename T>
    StringBuilder &operator <<(const T &value)
    {
        builder << value;

        return (*this);
    }

    operator std::string()
    {
        return (builder.str());
    }

	std::string getString()
	{
		return (builder.str());
	}

private:
    std::ostringstream builder; 
};