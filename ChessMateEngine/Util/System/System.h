/**
 * Static class to provide interface to system calls.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Util/Utilities.h>

namespace Util { namespace System {

class System
{
public:
	/**
	 * Print the backtrace to stderr.
	 *
	 * @param int Number of points to print.
	 */
	static void PrintBacktrace(int);
};

}}

#endif // _SYSTEM_H_
