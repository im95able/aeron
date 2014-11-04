/*
 * Copyright 2014 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef INCLUDED_AERON_UTIL_STRING_UTIL_FILE__
#define INCLUDED_AERON_UTIL_STRING_UTIL_FILE__

#include <string>
#include <sstream>
#include <type_traits>
#include "Exceptions.h"


namespace aeron { namespace common { namespace util {

inline std::string trimWSLeft (std::string str, const char* wschars = " \t")
{
    str.erase(0,str.find_first_not_of(wschars));
    return str;
}

inline std::string trimWSRight (std::string str, const char* wschars = " \t")
{
    str.erase(str.find_last_not_of(wschars)+1);
    return str;
}

inline std::string trimWSBoth (std::string str, const char* wschars = " \t")
{
    return trimWSLeft(trimWSRight(str, wschars), wschars);
}

template<class valueType>
valueType parse (const std::string& input)
{
    std::string str = trimWSBoth(input);

    std::istringstream stream (str);
    valueType value;

    if (std::is_integral<valueType>::value && input.length() > 2 && input[0] == '0' && (input[1] == 'x' || input[1] == 'X'))
        stream >> std::hex >> value;
    else
        stream >> value;

    // if we failed extract an valid value or we didnt use up all the chars then throw an error
    if (stream.fail() || !stream.eof())
    {
        throw ParseException(std::string("Failed to parse: ") + input, SOURCEINFO);
    }

    return value;
}

template <typename value_t>
inline std::string toString (const value_t& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

std::string strPrintf (const char* format, ...);

}}}

#endif