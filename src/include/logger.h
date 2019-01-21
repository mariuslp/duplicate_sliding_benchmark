#pragma once

#include <iostream>
#include <string>

using namespace std;

template<typename ...args>
void Log(args&& ...str) {
	/** Console log
	 *   @param s1, s2, ... : sequence of strings
	 */

	(cout << ... << forward<args>(str)) << endl;
}
