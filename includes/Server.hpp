/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:41:15 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/04/01 17:20:13 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <iomanip>
# include <string>
# include <exception>
# include <netinet/in.h>
# include <stdio.h>
# include <unistd.h>
# include <cstring>
# include <cstdlib>
# include <fcntl.h>			//	
# include <fstream>			//	
# include <sys/socket.h>	//	
# include <arpa/inet.h>		//	accept
# include <vector>
# include <map>
# include <poll.h>

# include "Client.hpp"
# include "Channel.hpp"

# define RPL_WELCOME " 001 "
# define ERR_NOTREGISTERED " 451 "
# define ERR_ALREADYREGISTERED " 462 "
# define ERR_NEEDMOREPARAMS " 461 "
# define ERR_PASSWDMISMATCH " 464 "
# define ERR_NONICKNAMEGIVEN " 431 "
# define ERR_NICKNAMEINUSE " 433 "
# define ERR_ERRONEUSNICKNAME " 432 "

class Server
{
	private:
		std::string						_name;
		int								_port;
		std::string						_passwd;
		std::vector<struct pollfd>		_pollfds;
		std::map<int, Client>			_clients;
		std::map<std::string, Channel>	_channels;
	protected:
	public:
		Server(void);
		Server(const std::string &name, int port, const std::string &passwd);
		Server(const Server &other);
		Server	&operator=(const Server &other);
		~Server(void);

		void	setPort(int port);
		void	setName(std::string name);
		void	setPasswd(std::string passwd);

		const std::string	&getName(void) const;
		int					getPort(void) const;
		const std::string	&getPasswd(void) const;

		void	start(int port);
		int		initServerSocket(int port);
		int		acceptConnection(int serverFd);

		void	handleCommand(int fd, const std::string &cmd);
		void	sendReply(int fd, const std::string &msg);
		void	cmdPass(int fd, const std::string &params);
		void	cmdNick(int fd, const std::string &params);
		void	cmdUser(int fd, const std::string &params);
		void	cmdCap(int fd, const std::string &params);
		void	tryRegister(int fd);
};

std::ostream	&operator<<(std::ostream &os, const Server &s);

#endif

