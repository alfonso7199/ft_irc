/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:57:56 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/11 21:07:44 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define PORT 8080

int	main(void)
{
	int		sock = 0;
	struct sockaddr_in	serv_addr;
//	const char *hello = "Hello from client";
	std::string		line;

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		return (-1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 address from text to binary
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		std::cerr << "Invalid address / Address not supported" << std::endl;
		return (-1);
	}

	// Connect to Server
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "Connection Failed" << std::endl;
		return (-1);
	}

	// Send data
	// send(sock, hello, strlen(hello), 0);
	while (std::getline(std::cin, line))
	{
		send(sock, line.c_str(), line.size(), 0);
		std::cout << "Message sent" << std::endl;
	}

	// Close socket
	close(sock);
	return (0);
}
