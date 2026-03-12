/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:42:49 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/12 15:24:49 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const std::string &name, int port, const std::string &passwd)
: _name(name), _port(port), _passwd(passwd)
{
	return ;
}

Server::Server(const Server &other) :
_name(other._name), _port(other._port), _passwd(other._passwd)
{
	return ;
}

Server	&Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_port = other._port;
		this->_passwd = other._passwd;
	}
	return (*this);
}

Server::~Server(void)
{
	return ;
}

const std::string	&Server::getName(void) const
{
	return (this->_name);
}

int	Server::getPort(void) const
{
	return (this->_port);
}

const std::string	&Server::getPasswd(void) const
{
	return (this->_passwd);
}

void	Server::start(void)
{
	initSocket();
	acceptConnection();
}

void	Server::initSocket(void)
{
	struct sockaddr_in	address;
	int					opt;

	opt = 1;
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd < 0)
		throw (std::runtime_error("Failed to create socket"));

	setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (bind(_socketFd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw (std::runtime_error("Failed to bind"));
	if (listen(_socketFd, 1024) < 0)
		throw (std::runtime_error("Failed to listen"));
}

void	Server::acceptConnection(void)
{
	char				buffer[1024] = {0};
	struct sockaddr_in	address;
	socklen_t			addrlen = sizeof(address);
	int					new_socket;

	new_socket = accept(_socketFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_socket < 0)
		throw (std::runtime_error("Faild to accept connection"));
	while (read(new_socket, buffer, 1024))
	{
		std::cout << "Message from client: " << buffer << std::endl;
		memset(buffer, 0, sizeof(buffer));
	}
	close(new_socket);
	close(_socketFd);
	return ;

}

std::ostream	&operator<<(std::ostream &os, const Server &s)
{
	os << '\n' << "IRC Server Details " << '\n';
	os << "- " << s.getName() << '\n';
	os << "- " << s.getPort() << '\n';
	os << "- " << s.getPasswd() << '\n';
	return (os);
}
