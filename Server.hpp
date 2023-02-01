/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 13:30:46 by adbenoit          #+#    #+#             */
/*   Updated: 2023/01/30 12:18:44 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/types.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/time.h>
# include <csignal>
# include <vector>
# include <sys/stat.h>
# include <sys/file.h>
# include <exception>

# define PORT           4242
# define BUFFER_SIZE    1024
# define NB_MAX_CLI        3

enum connect_status {
	s_ok,
	s_ko,
	s_refuse
};

enum input_status {
	s_recv,
	s_disconnect
};

class Server
{
	private:
		Server(const Server &);
		Server&	operator=(const Server &);
		
	public:
		Server();
		~Server();
		int		        init(void);
		int		        new_client(void);
		int		        get_client_input(int i, std::string& input);
		void	        reset_fds(void);
		fd_set&	        get_fds(void);
		int		        get_maxfd(void) const;
		int		        get_nb_cli(void) const;
		int		        is_newclient(void) const;

	private:
		int					_m_socket;
		struct sockaddr_in	_addr;
		fd_set				_fds;
		int					_addr_len;
		std::vector<int>	_cli_sd;
};

#endif
