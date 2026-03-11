/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:41:15 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/11 23:16:18 by rzamolo-         ###   ########.fr       */
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


class Server
{
	private:
		std::string	_name;
		int			_port;
		std::string	_passwd;
		int			_socketFd;
	protected:
	public:
		Server(const std::string &name, int port, const std::string &passwd);
		Server(const Server &other);
		Server	&operator=(const Server &other);
		~Server(void);

		const std::string	&getName(void) const;
		int					getPort(void) const;
		const std::string	&getPasswd(void) const;

		void	start(void);

};

std::ostream	&operator<<(std::ostream &os, const Server &s);

#endif

