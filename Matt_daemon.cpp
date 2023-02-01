/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matt_daemon.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:34:07 by adbenoit          #+#    #+#             */
/*   Updated: 2023/01/31 12:04:31 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Matt_daemon.hpp"

Matt_daemon::Matt_daemon(int fd_lock) : _fd_lock(fd_lock) {
	Tintin_reporter::write_logs(Tintin_reporter::inf, "Started.");
}

Matt_daemon::~Matt_daemon() {
	try {
		Tintin_reporter::write_logs(Tintin_reporter::inf, "Quitting.");
	}
	catch(const std::exception& e) {}
	if (this->_fd_lock != -1) {
		this->unlock_file();
		close(this->_fd_lock);
	}
	remove(LOCK_FILE);	
}

void    Matt_daemon::init_server(void) {
	int	ret;

	Tintin_reporter::write_logs(Tintin_reporter::inf, "Creating server.");
	ret = this->_server.init();
	if (ret == s_ko) {
		Tintin_reporter::write_logs(Tintin_reporter::err, "Server failed to create.");
		throw FunctionError();
	} 
	Tintin_reporter::write_logs(Tintin_reporter::inf, "Server created.");
}

void    Matt_daemon::lock_file() {
	int	ret;
	
	ret = flock(this->_fd_lock, LOCK_EX | LOCK_NB);
	if (ret == -1) {
		Tintin_reporter::write_logs(Tintin_reporter::err, "Error file locked.");
		throw FileLock();
	}
}

void    Matt_daemon::unlock_file() {
	flock(this->_fd_lock, LOCK_UN);
}

void	Matt_daemon::_log_clients_input(void) {
	std::string	input;
	int			ret;
	
	for (int i = 0; i < NB_MAX_CLI; i++) {
		ret = this->_server.get_client_input(i, input);
		if (ret == s_recv) {
			if (input == "quit") {
				Tintin_reporter::write_logs(Tintin_reporter::inf, "Request quit.");
				throw Quit();
			}
			Tintin_reporter::write_logs(Tintin_reporter::log, "User input: " + input);
		}
		else if (ret == s_disconnect) {
			Tintin_reporter::write_logs(Tintin_reporter::log, "Client disconnected.");
		}
	}
}

void    Matt_daemon::start(void) {
	int			valselect;
	int			ret;
	char		pid[30];

	sprintf(pid, "started. PID: %d.", getpid());
	Tintin_reporter::write_logs(Tintin_reporter::inf, "Entering Daemon mode.");
	Tintin_reporter::write_logs(Tintin_reporter::inf, pid);
	while(1)  
	{  
		this->_server.reset_fds();
		valselect = select(this->_server.get_maxfd() + 1, &this->_server.get_fds(),
							NULL , NULL , NULL);
		if (valselect >= 0) {
			if (this->_server.is_newclient()) {
				const std::string	log_msg[] = { "New connection.",
										"Connection failed.", "Max clients" };
				const std::string	log_code[] = { Tintin_reporter::log,
										Tintin_reporter::err, Tintin_reporter::err };
										
				ret = this->_server.new_client();
				Tintin_reporter::write_logs(log_code[ret], log_msg[ret]);
			}
			this->_log_clients_input();
		}
	}
}

const char* Matt_daemon::FunctionError::what() const throw() {
	return "Matt_daemon: Failed";
}

const char* Matt_daemon::FileLock::what() const throw() {
	return "Can't open /var/log/matt_daemon/matt_daemon.lock";
}

const char* Matt_daemon::PermissionError::what() const throw() {
	return "Matt_daemon have to run with root rights.";
}

const char* Matt_daemon::Quit::what() const throw() {
	return "Quit";
}
