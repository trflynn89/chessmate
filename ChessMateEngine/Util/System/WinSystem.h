/**
 * Windows declaration of the SocketImpl interface.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version November 11, 2013
 */
#ifndef _SYSTEM_IMPL_H_
#define _SYSTEM_IMPL_H_

namespace Util { namespace System {

class SystemImpl
{
public:
	static void PrintBacktrace(int);
};

}}

#endif // _SYSTEM_IMPL_H_
