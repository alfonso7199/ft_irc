/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:33:52 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/04/15 20:32:21 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int	main(int argc, const char *argv[])
{

	if (argc == 3)
	{
		char	*endptr;
		long	port = std::strtol(argv[1], &endptr, 10);

		if (*argv[1] == '\0' || *endptr != '\0' || port < LOWER_PORT_ALLOWED || port > HIGHER_PORT_ALLOWED)
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
