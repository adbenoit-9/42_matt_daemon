/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matt_daemon.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 18:33:37 by adbenoit          #+#    #+#             */
/*   Updated: 2023/01/30 16:46:41 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATT_DAEMON_HPP
# define MATT_DAEMON_HPP

# include <exception>
# include "Server.hpp"
# include "debug.hpp"
# include "Tintin_reporter.hpp"

class Matt_daemon {
	private:
		Matt_daemon();
		Matt_daemon(const Matt_daemon&);
		Matt_daemon&	operator=(const Matt_daemon&);
		void			_log_clients_input(void);
		
    public:
		Matt_daemon(int fd_lock);
		~Matt_daemon();
		void	start(void);
		void	init_server(void);
		void	lock_file(void);
		void	unlock_file(void);

        class FunctionError : public std::exception
		{
			public:
				virtual const char*	what() const throw();
		};

        class FileLock : public std::exception
		{
			public:
				virtual const char*	what() const throw();
		};

		class PermissionError : public std::exception
		{
			public:
				virtual const char*	what() const throw();
		};

		class Quit : public std::exception
		{
			public:
				virtual const char*	what() const throw();
		};

	private:
		int		_fd_lock;
		Server	_server;
};

#endif
