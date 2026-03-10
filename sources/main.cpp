/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:33:52 by rzamolo-          #+#    #+#             */
/*   Updated: 2026/03/10 18:30:52 by rzamolo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int	main(int argc, const char *argv[])
{
	(void)argc;
	(void)argv;
	try
	{
		Server	s1("Charlie", 123, "XPTO");
		Server	s2("Michael", 456, "XYZ");
		Server	s3(s1); // Oscar


		std::cout << "Server Charlie : " << s1 << std::endl;
		std::cout << "Server Michael : " << s2 << std::endl;
		s1 = s2;
		std::cout << "Server Charlie: " << s1 << std::endl;
		std::cout << "Server Oscar: " << s3 << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	try
	{
		Client	c1("Charlie Brown", "Peanut");
		Client	c2("Adam", "He-man");

		std::cout << c1 << std::endl;
		std::cout << c2 << std::endl;
		c1 = c2;
		std::cout << c1 << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (0);
}
