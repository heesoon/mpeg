/**
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org>
*/

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <tuple>

#include "tsfilterDebug.h"
#include "tsfilter.h"

// implementation of debug functions
namespace debug_tsfilterDebug {

	uint32_t i 		= 1;
	bool isInLoop 	= true;
	// add debug menus
	std::vector< std::tuple<uint32_t, std::string, std::function<void()>> > dv {
		std::make_tuple<uint32_t, std::string, std::function<void()>>(i++, "ts open in File", debug_tsfilter_file),
		std::make_tuple<uint32_t, std::string, std::function<void()>>(i++, "ts open in IP", debug_tsfilter_ip),
		std::make_tuple<uint32_t, std::string, std::function<void()>>(i++, "exit", debug_tsfilter_exit)
	};

	void debug_tsfilter_main() {
		uint32_t opt = 0;

		while(isInLoop) {
			std::string s;
			std::stringstream ss;

			std::cout << " --------------- Debug Menu for TS filter --------------" << std::endl;
			for(auto& d : dv) {
				std::cout << std::get<0>(d) << ". " << std::get<1>(d) << std::endl;
			}

			std::cout << "-- choose menu --" << std::endl;
			std::cin >> s;
			ss.str(s);

			if(ss.fail()) {
				std::cout << "invalid input parameter .. " << std::endl;
				ss.clear();
				continue;
			}

			ss >> opt;

			// excute deubug actual menu
			std::get<2>(dv[opt-1])();
		}
	}

	void debug_tsfilter_file() {

		std::unique_ptr<CTsFilter> up = CTsFilterFactory::createNewTsFilter(MediaType::MEDIA_TYPE_FILE);
		up->openMedia();
		up->readMedia();
		up->closeMedia();
	}

	void debug_tsfilter_ip() {
		std::unique_ptr<CTsFilter> up = CTsFilterFactory::createNewTsFilter(MediaType::MEDIA_TYPE_IP);
		up->openMedia();
		up->readMedia();
		up->closeMedia();
	}

	void debug_tsfilter_exit() {
		std::cout << "... Bye ..." << std::endl;
		isInLoop = false;
	}
}