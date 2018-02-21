#ifdef _WIN32

#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
# define MP_REUSE_PORT_FLAG SO_REUSEADDR
# define MP_TYPE char

namespace mp
{
	template <class T>
	inline T	__cast(int input)
	{
		return static_cast<T>(input);
	}

	inline void	__initSocket()
	{
		WSADATA WSAData;
		WSAStartup(MAKEWORD(2, 0), &WSAData);
	}

	inline void	__shutdown(int socket)
	{
		shutdown(socket, SD_SEND);
	}

	inline void	__closeSocket(int socket)
	{
		closesocket(socket);
		WSACleanup();
	}
}
#else
#include <arpa/inet.h>
#include <unistd.h>
# define MP_REUSE_PORT_FLAG SO_REUSEPORT
# define MP_TYPE char

namespace mp
{
	template <class T>
	inline T	__cast(int input)
	{
		return static_cast<T>(input);
	}

	inline void	__initSocket()
	{
	}

	inline void	__shutdown(int socket)
	{
		shutdown(socket, SHUT_RDWR);
	}

	inline void	__closeSocket(int socket)
	{
		close(socket);
	}
}

#endif // _WIN32