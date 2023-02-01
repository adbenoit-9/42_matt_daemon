/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:09:09 by adbenoit          #+#    #+#             */
/*   Updated: 2023/02/01 16:12:26 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : _cli_sd(NB_MAX_CLI, 0) {
	this->_addr.sin_family = AF_INET;  
	this->_addr.sin_addr.s_addr = INADDR_ANY;  
	this->_addr.sin_port = htons(PORT);
	this->_addr_len = sizeof(_addr);
	FD_ZERO(&this->_fds);
}

Server::~Server(){
	for (unsigned int i = 0; i < this->_cli_sd.size(); i++){
		close(this->_cli_sd[i]);
	}
}

int Server::get_nb_cli(void) const {
	int	nb = 0;
	for (unsigned int i = 0; i < this->_cli_sd.size(); i++){
		if (this->_cli_sd[i] != 0)
			nb++;
	}
	return nb;
}


int Server::is_newclient(void) const {
	return FD_ISSET(this->_m_socket, &this->_fds);
}

int Server::new_client(void)
{
	int	new_socket, ret;

	ret = s_ok;
	if ((new_socket = accept(this->_m_socket, (struct sockaddr *)&this->_addr,
			(socklen_t *)&this->_addr_len)) < 0) {
		ret = s_ko;
	}
	else if (this->get_nb_cli() == NB_MAX_CLI) {
		close(new_socket);
		ret = s_refuse;
	}
	else {
		for (unsigned int i = 0; i < this->_cli_sd.size(); i++)
		{
			if (this->_cli_sd[i] == 0)
			{
				this->_cli_sd[i] = new_socket;
				break;
			}
		}
	}
	return (ret);
}

void Server::reset_fds(void) {
	FD_ZERO(&this->_fds);
	FD_SET(this->_m_socket, &this->_fds);  
	for (unsigned int i = 0; i < this->_cli_sd.size(); i++) {
		if(this->_cli_sd[i] > 0) {
			FD_SET(this->_cli_sd[i], &this->_fds);
		}
	}
}

int Server::get_maxfd(void) const {
	int	maxfd = this->_m_socket;
	
	for (size_t i = 0; i < this->_cli_sd.size(); i++) {
		if (this->_cli_sd[i] > maxfd) {
			maxfd = this->_cli_sd[i];
		}
	}
	return (maxfd);
}

fd_set& Server::get_fds(void){
	return this->_fds;
}

int Server::get_client_input(int i, std::string& input) {
	size_t	valread, size;
	char	buffer[BUFFER_SIZE + 1];
	int		ret = -1;

	input = "";
	if (FD_ISSET(this->_cli_sd[i], &this->_fds)) {
		ret = s_recv;
		valread = read(this->_cli_sd[i], buffer, BUFFER_SIZE);
		size = valread;
		while (valread == BUFFER_SIZE) {
			buffer[valread] = '\0';  
			input += buffer;
			valread = read(this->_cli_sd[i], buffer, BUFFER_SIZE);
			size += valread;
		}
		if (size  == 0)  
		{  
			close(this->_cli_sd[i]);  
			this->_cli_sd[i] = 0;
			ret = s_disconnect;
		}                       
		else 
		{  
			buffer[valread] = '\0';  
			input += buffer;
			input = input.substr(0, input.find("\r")); // for telnet (ctrl + ] + q to exit)
			input = input.substr(0, input.find("\n")); // for netcat (ctrl + c to exit)
		}
	}
	return (ret);
}

int    Server::init(void) {
	int	ret = s_ok;
	
	this->_m_socket = socket(AF_INET , SOCK_STREAM , 0);
	if(this->_m_socket  == 0) {
		ret = s_ko;
	}
	else {
	// allows you to bind on the same local address even if the socket is in TIME_WAIT state
		const int enable = 1;
		if (setsockopt(this->_m_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
			perror("Matt_daemon: setsockopt");
			ret = s_ko;
		}
		else if (bind(this->_m_socket, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0) {
			perror("Matt_daemon: bind");
			ret = s_ko;
		}
		else if (listen(this->_m_socket, 3) < 0) {
			perror("Matt_daemon: listen");
			ret = s_ko;
		}
	}
	return (ret);
}
