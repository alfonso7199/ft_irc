/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 19:19:02 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/04/15 19:19:08 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
