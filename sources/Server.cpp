/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:42:49 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/04/01 18:25:52 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _name("Unkown"), _port(6667), _passwd("")
{
	return ;
}

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

// ===========================================================================
// Setters
// ===========================================================================
void	Server::setPort(int port)
{
		this->_port = port;
}

void	Server::setName(std::string name)
{
		this->_name = name;
}

void	Server::setPasswd(std::string passwd)
{
		this->_passwd = passwd;
}

// ===========================================================================
// Getters
// ===========================================================================
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

void	Server::sendReply(int fd, const std::string &msg)
{
	std::string	full_str = msg + "\r\n";
	::send(fd, full_str.c_str(), full_str.size(), 0);
}

void	Server::cmdPass(int fd, const std::string &params)
{
	Client		&client = this->_clients.find(fd)->second;
	std::string	password = params;

	if (client.isRegistered())
	{
		sendReply(fd, ":" + this->_name + ERR_ALREADYREGISTERED + client.getNickname() + ": You may not register");
		return ;
	}
	if (!password.empty() && password[0] == ':')
		password = password.substr(1);
	if (password.empty())
	{
		sendReply(fd, ":" + this->_name + ERR_NEEDMOREPARAMS + "* PASS :Not enough parameters");
		return ;
	}
	if (password == this->_passwd)
		client.setPassOk(true);
	else
		sendReply(fd, ":" + this->_name + ERR_PASSWDMISMATCH + "* PASS :Incorrect");
}

void	Server::cmdNick(int fd, const std::string &params)
{
	Client							&client = this->_clients.find(fd)->second;
	std::map<int, Client>::iterator	it = this->_clients.begin();

	if (params.empty())
	{
		sendReply(fd, ":" + this->_name + ERR_NONICKNAMEGIVEN + "* :No nickname given");
		return ;
	}
	while (it != this->_clients.end())
	{
		if (it->first != fd && it->second.getNickname() == params)
		{
			sendReply(fd, ":" + this->_name + ERR_NICKNAMEINUSE + "* " + params + " :Nickname is already in use");
			return ;
		}
		++it;
	}
	client.setNickname(params);
}

void	Server::cmdUser(int fd, const std::string &params)
{
		Client						&client = this->_clients.find(fd)->second;
		std::string					username;
		size_t						space = params.find(' ');
		size_t						colon = params.find(':');

		if (client.isRegistered())
		{
			sendReply(fd, ":" + this->_name + ERR_ALREADYREGISTERED + client.getNickname());
			return ;
		}
		if (space != std::string::npos)
			username = params.substr(0, space);
		else
			username = params;
		if (username.empty())
		{
			sendReply(fd, ":" + this->_name + ERR_NEEDMOREPARAMS + "* USER :Not enough parameters!");
			return ;
		}
		client.setUsername(username);
		if (colon != std::string::npos)
			client.setRealname(params.substr(colon + 1));
}

void	Server::cmdCap(int fd, const std::string &params)
{
	std::string	subcommand;
	size_t		space = params.find(' ');
	
	if (space != std::string::npos)
		subcommand = params.substr(0, space);
	else
		subcommand = params;

	if (subcommand == "LS")
		sendReply(fd, ":" + this->_name + " CAP * LS :");
	else if (subcommand == "END")
		tryRegister(fd);
}

void	Server::tryRegister(int fd)
{
	Client					&client = this->_clients.find(fd)->second;

	if (client.isRegistered())
		return ;
	if (!client.isPassOk() || client.getNickname().empty() || client.getUsername().empty())
		return ;
	client.setRegistered(true);
	sendReply(fd, ":" + this->_name + RPL_WELCOME + client.getNickname() + " :Welcome to myIRC Server!" + client.getNickname());
}
// WeeChat: CAP LS 302, PASS <senha>, NICK <nick>, USER <user> ..., CAP END
void	Server::start(int port)
{
		int				fd = 0;
		struct	pollfd	pfd;

		fd = initServerSocket(port);
		if (fd < 0)
			return ;
		pfd.fd = fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		_pollfds.push_back(pfd);
		while (true)
		{
				int	ready = poll(_pollfds.data(), _pollfds.size(), -1);

				if (ready < 0)
				{
					perror("Poll");
					break ;
				}
				size_t	i = 0;
				while (i < _pollfds.size())
				{
					if (_pollfds[i].revents == 0)
					{
						i++;
						continue ;
					}
					if (_pollfds[i].fd == fd && (_pollfds[i].revents & POLLIN))
					{
						int	clientFd = acceptConnection(fd);
						if (clientFd != -1)
						{
							_clients.insert(std::make_pair(clientFd, Client(clientFd, "host")));
							struct pollfd cpfd;
							cpfd.fd = clientFd;
							cpfd.events = POLLIN;
							cpfd.revents = 0;
							_pollfds.push_back(cpfd);
						}
					}
					else if (_pollfds[i].revents & POLLIN)
					{
						char	buf[512];
						int		bytes = recv(_pollfds[i].fd, buf, sizeof(buf) - 1, 0);
						if (bytes <= 0)
						{
							close(_pollfds[i].fd);
							_clients.erase(_pollfds[i].fd);
							_pollfds.erase(_pollfds.begin() + i);
							i--;
						}
						else
						{
							buf[bytes] = '\0';
							std::string	&cbuf = _clients.find(_pollfds[i].fd)->second.getBuffer();
							cbuf += buf;

							size_t	pos;
							while ((pos = cbuf.find("\r\n")) != std::string::npos)
							{
								std::string	cmd = cbuf.substr(0, pos);
								cbuf.erase(0, pos + 2);
								if (!cmd.empty())
									handleCommand(_pollfds[i]. fd, cmd);
							}
						}
					}
					else if (_pollfds[i].revents & (POLLHUP | POLLERR))
					{
						close(_pollfds[i].fd);
						_clients.erase(_pollfds[i].fd);
						_pollfds.erase(_pollfds.begin() + i);
						i--;
					}
					i++;
				}
		}
}

void	Server::handleCommand(int fd, const std::string &cmd)
{
	std::string	command;
	std::string	params;
	size_t		space;

	(void)fd;
	space = cmd.find(' ');
	if (space != std::string::npos)
	{
		command = cmd.substr(0, space);
		params = cmd.substr(space + 1);
	}
	else
		command = cmd;

	if (command == "CAP")
		cmdCap(fd, params);
	else if (command == "PASS")
		cmdPass(fd, params);
	else if (command == "NICK")
		cmdNick(fd, params);
	else if (command == "USER")
		cmdUser(fd, params);
	else if (command == "JOIN")
		std::cout << "Cmd: JOIN" << std::endl;
	else if (command == "PRIVMSG")
		std::cout << "Cmd: PRIVMSG" << std::endl;
	else if (command == "KICK")
		std::cout << "Cmd: KICK" << std::endl;
	else if (command == "QUIT")
		std::cout << "Cmd: QUIT" << std::endl;
}

int	Server::initServerSocket(int port)
{
		int					fd;
		int					opt = 1;
		struct sockaddr_in	addr = {};

		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0)
		{
				perror("Socket");
				return (-1);
		}

		// Avoid error 'Address already in use' on restart
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		// Set socket as non-block
		fcntl(fd, F_SETFL, O_NONBLOCK);

		addr.sin_family = AF_INET; // Use IPv4
		addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
		addr.sin_port = htons(port);	// Convert byte order

		if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
				perror("Bind");
				close(fd);
				return (-1);
		}

		if (listen(fd, SOMAXCONN) < 0)
		{
				perror("Listen");
				close(fd);
				return (-1);
		}
		return (fd);
}

int	Server::acceptConnection(int serverFd)
{
		struct sockaddr_in	clientAddr;
		socklen_t			addrLen;
		int					clientFd;
		char				ip[INET_ADDRSTRLEN];

		addrLen = sizeof(clientAddr);
		clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrLen);
		if (clientFd < 0)
		{
				if (errno != EAGAIN && errno != EWOULDBLOCK) // Check if there is no pending connections
						perror("Accept");
				return (-1);
		}

		fcntl(clientFd, F_SETFL, O_NONBLOCK);
		inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		printf("New connection from: %s\n", ip);
		return (clientFd);
}

std::ostream	&operator<<(std::ostream &os, const Server &s)
{
	os << '\n' << "IRC Server Details " << '\n';
	os << "- " << s.getName() << '\n';
	os << "- " << s.getPort() << '\n';
	os << "- " << s.getPasswd() << '\n';
	return (os);
}
