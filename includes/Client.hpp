/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 18:19:05 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/10 18:22:32 by rzamolo-         ###   ########.fr       */
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
		std::string	_nickname;
		std::string	_realname;
	protected:
	public:
		Client(std::string nickname, std::string realname);
		Client(const Client &other);
		Client	&operator=(const Client &other);
		~Client(void);

		const std::string	&getNickname(void) const;
		const std::string	&getRealname(void) const;

};

std::ostream	&operator<<(std::ostream &os, const Client &c);

#endif

