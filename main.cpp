/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adbenoit <adbenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:44:23 by adbenoit          #+#    #+#             */
/*   Updated: 2023/02/01 16:47:12 by adbenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Matt_daemon.hpp"

static void	sig_handler(int signo)
{
	(void)signo;
	Tintin_reporter::write_logs(Tintin_reporter::inf, "Signal handler.");
	throw Matt_daemon::Quit();
}

/* Creates log/lock directories and files. */
static void	create_env(int *fd_lock) {
	int	ret;

	if (getuid() != 0) {
		throw Matt_daemon::PermissionError();
	}
	ret = mkdir(LOG_DIR, 0777);
	if (ret == -1 && errno != EEXIST) {
		perror(LOG_DIR);
		throw Matt_daemon::FunctionError();
	}
	ret = mkdir(LOCK_DIR, 0777);
	if (ret == -1 && errno != EEXIST) {
		perror(LOCK_DIR);
		throw Matt_daemon::FunctionError();
	}
	*fd_lock = open(LOCK_FILE, O_CREAT, 0644);
	if (*fd_lock == -1) {
		perror(LOCK_FILE);
		throw Matt_daemon::FunctionError();
	}
}

int	main()
{
	int	pid, fd_lock;
	int	ret = s_ok;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTERM, sig_handler);
	try {
		create_env(&fd_lock);
		pid = fork();
		if (pid == 0) {
			Matt_daemon daemon(fd_lock);
			daemon.lock_file();
			daemon.init_server();
			daemon.start();
		}
		else if (pid < 0) {
			perror("Matt_daemon: fork");
			throw Matt_daemon::FunctionError();
		}
	}
	catch(const Matt_daemon::FileLock& e) {
		std::cerr << e.what() << std::endl;
	}
	catch(const Matt_daemon::Quit& e) {
	}
	catch(const Matt_daemon::PermissionError& e) {
		std::cerr << e.what() << std::endl;
		ret = s_ko;
	}
	catch(const Matt_daemon::FunctionError& e) {
		ret = s_ko;
	}
	catch(const std::exception& e) {
		ret = s_ko;
	}
	PRINT_DEBUG("Process ends", __FILE__, __LINE__);
	return (ret);
}
