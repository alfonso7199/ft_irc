/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:42:49 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/12 00:36:54 by rzamolo-         ###   ########.fr       */
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
	char				buffer[1024] = {0};
	int					opt = 1;
	struct sockaddr_in	address;
	int					new_socket;

	socklen_t	addrlen = sizeof(address);

	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);

	if (_socketFd < 0)
	{
		perror("Fail to create socket");
		exit (EXIT_FAILURE);
	}

	if (bind(_socketFd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Fail to bind");
		exit (EXIT_FAILURE);
	}

	if (listen(_socketFd, 3) < 0)
	{
		perror("Fail to listen");
		exit (EXIT_FAILURE);
	}

	new_socket = accept(_socketFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (new_socket < 0)
	{
		perror("accept");
		exit (EXIT_FAILURE);
	}

	read (new_socket, buffer, 1024);
	std::cout << "Message from client: " << buffer << std::endl;
	memset(buffer, 0, sizeof(buffer));

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
