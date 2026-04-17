/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:42:49 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/04/17 21:45:53 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) : _name("Unknown"), _port(6667), _passwd("")
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
	if (::send(fd, full_str.c_str(), full_str.size(), 0) < 0)
		disconnectClient(fd);
}

void	Server::tryRegister(int fd)
{
	Client					&client = this->_clients.find(fd)->second;

	if (client.isRegistered())
		return ;
	if ((!client.isPassOk() && !this->_passwd.empty()) || client.getNickname().empty() || client.getUsername().empty())
		return ;
	client.setRegistered(true);
	sendReply(fd, ":" + this->_name + RPL_WELCOME + client.getNickname() + " :Welcome to myIRC Server! [" + client.getNickname() + "]");
//	sendReply(fd, ":" + this->_name + RPL_YOURHOST + client.getNickname() + " :Your host is " + this->_name);
//	sendReply(fd, ":" + this->_name + RPL_CREATED + client.getNickname() + " :Server created recently");
//	sendReply(fd, ":" + this->_name + RPL_MOTDSTART + client.getNickname() + " :- myIRC Message of the day -");
//	sendReply(fd, ":" + this->_name + RPL_MOTD + client.getNickname() + " :- <texto>");
//	sendReply(fd, ":" + this->_name + RPL_ENDOFMOTD + client.getNickname() + " :End of /MOTD command");
	sendReply(fd, ":" + this->_name + ERR_NOMOTD + client.getNickname() + " :MOTD File is missing");
}

// WeeChat: CAP LS 302, PASS <senha>, NICK <nick>, USER <user> ..., CAP END
void	Server::start(int port)
{
		int				fd = 0;
		struct	pollfd	pfd;
		std::string		hostname;

		fd = initServerSocket(port);
		if (fd < 0)
			return ;
		pfd.fd = fd;
		pfd.events = POLLIN;
		pfd.revents = 0;
		this->_pollfds.push_back(pfd);
		while (true)
		{
				int	ready = poll(this->_pollfds.data(), this->_pollfds.size(), -1);

				if (ready < 0)
				{
					perror("Poll");
					break ;
				}
				size_t	i = 0;
				while (i < this->_pollfds.size())
				{
					if (this->_pollfds[i].revents == 0)
					{
						i++;
						continue ;
					}
					if (_pollfds[i].fd == fd && (_pollfds[i].revents & POLLIN))
					{
						int	clientFd = acceptConnection(fd, hostname);
						if (clientFd != -1)
						{
							this->_clients.insert(std::make_pair(clientFd, Client(clientFd, hostname)));
							struct pollfd cpfd;
							cpfd.fd = clientFd;
							cpfd.events = POLLIN;
							cpfd.revents = 0;
							this->_pollfds.push_back(cpfd);
						}
					}
					else if (_pollfds[i].revents & POLLIN)
					{
						char	buf[BUFFER_LIMIT_SIZE];
						int		bytes = recv(_pollfds[i].fd, buf, sizeof(buf) - 1, 0);
						if (bytes <= 0)
						{
							disconnectClient(_pollfds[i].fd);
							i--;
						}
						else
						{
							buf[bytes] = '\0';
							std::string	&cbuf = _clients.find(_pollfds[i].fd)->second.getBuffer();
							cbuf += buf;
							if (cbuf.size() > BUFFER_LIMIT_SIZE)
							{
								disconnectClient(_pollfds[i].fd);
								i--;
								continue;
							}

							size_t	pos;
							bool	disconnected = false;
							while ((pos = cbuf.find("\r\n")) != std::string::npos)
							{
								std::string	cmd = cbuf.substr(0, pos);
								cbuf.erase(0, pos + 2);
								if (!cmd.empty())
								{
									int	curFd = _pollfds[i].fd;
									handleCommand(curFd, cmd);
									if (this->_clients.find(curFd) == this->_clients.end())
									{
										disconnected = true;
										break ;
									}
								}
							}
							if (disconnected)
							{
								i--;
								continue ;
							}
						}
					}
					else if (_pollfds[i].revents & (POLLHUP | POLLERR))
					{
						disconnectClient(_pollfds[i].fd);
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

	space = cmd.find(' ');
	if (space != std::string::npos)
	{
		command = cmd.substr(0, space);
		params = cmd.substr(space + 1);
	}
	else
		command = cmd;
	
	for (size_t j =0; j < command.size(); j++)
		command[j] = std::toupper(command[j]);
	std::cout << "[fd = " << fd << "] CMD: " << command << " | PARAMS: " << params << std::endl;
	if (command != "CAP" && command != "PASS" &&
		command != "NICK" && command != "USER" &&
		command != "QUIT")
	{
		if (this->_clients.find(fd)->second.isRegistered() == false)
		{
			sendReply(fd, ":" + this->_name + ERR_NOTREGISTERED + "* :You have not registered");
			return ;
		}
	}
	if (command == "CAP")
		cmdCap(fd, params);
	else if (command == "PASS")
		cmdPass(fd, params);
	else if (command == "NICK")
		cmdNick(fd, params);
	else if (command == "USER")
		cmdUser(fd, params);
	else if (command == "JOIN")
		cmdJoin(fd, params);
	else if (command == "PRIVMSG")
		cmdPrivmsg(fd, params);
	else if (command == "INVITE")
		cmdInvite(fd, params);
	else if (command == "TOPIC")
		cmdTopic(fd, params);
	else if (command == "MODE")
		cmdMode(fd, params);
	else if (command == "KICK")
		cmdKick(fd, params);
	else if (command == "QUIT")
		cmdQuit(fd, params);
	else if (command == "PING")
		sendReply(fd, ":" + this->_name + " PONG " + this->_name + " " + params); // Adjusting PONG message to avoid disconnection in modern IRC clients
//		sendReply(fd, "PONG " + params);

/*
	else if (command == "OPER")
		cmdOper(fd, params);
	else if (command == "PART")
		cmdPart(fd, params);
	else if (command == "NAMES")
		cmdNames(fd, params);
	else if (command == "LIST")
		cmdList(fd, params);
	else if (command == "KILL")
		cmdKill(fd, params);
*/
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

int	Server::acceptConnection(int serverFd, std::string &hostname)
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
		hostname = ip;
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
