/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:33:52 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/30 19:17:44 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int	main(int argc, const char *argv[])
{

	if (argc == 3)
	{
		int	port = std::atoi(argv[1]);

		if (port < 1024 || port > 65535)
		{
			std::cout << "Invalid port range!" << std::endl;
			return (-1);	
		}

		Server	srv("ft_irc", port, argv[2]);
		srv.start(srv.getPort());
	}
	else if (argc == 1)
	{
		Server	srv;

		srv.start(srv.getPort());	
	}
	else
		return (-1);
	return (0);
}
