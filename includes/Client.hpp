/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 18:19:05 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/12 15:24:51 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <iomanip>
# include <exception>
# include <string>

class Client
{
	private:
		int         _fd;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _hostname;
		std::string _buffer;
		bool        _passOk;
		bool        _registered;
	protected:
	public:
		Client(int fd, const std::string &hostname);
		Client(const Client &other);
		Client	&operator=(const Client &other);
		~Client(void);

		int					getFd(void) const;
		const std::string	&getNickname(void) const;
		const std::string	&getUsername(void) const;
		const std::string	&getRealname(void) const;
		const std::string	&getHostname(void) const;
		std::string			&getBuffer(void);
		bool				isPassOk(void) const;
		bool				isRegistered(void) const;

		void	setNickname(const std::string &nickname);
		void	setUsername(const std::string &username);
		void	setRealname(const std::string &realname);
		void	setPassOk(bool ok);
		void	setRegistered(bool registered);

		void	send(const std::string &msg) const;

};

std::ostream	&operator<<(std::ostream &os, const Client &c);

#endif

