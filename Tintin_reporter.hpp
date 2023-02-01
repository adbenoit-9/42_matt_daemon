#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

# include <ctime>
# include <fstream>
# include <iostream>

# define LOG_DIR	"/var/log/matt_daemon"
# define LOCK_DIR	"/var/lock"
# define LOG_FILE	"/var/log/matt_daemon/matt_daemon.log"
# define LOCK_FILE	"/var/lock/matt_daemon.lock"

class Tintin_reporter
{
	private:
		Tintin_reporter(const Tintin_reporter&);
		Tintin_reporter&	operator=(const Tintin_reporter&);

	public:
		Tintin_reporter() {}
		~Tintin_reporter() {}

		inline static const std::string inf = "INFO";
		inline static const std::string log = "LOG";
		inline static const std::string err = "ERROR";

		static void write_logs(const std::string code, const std::string msg)
		{
			char			timestamp[100];
			std::time_t		result = std::time(nullptr);
			std::fstream	fs;

			std::strftime(timestamp, sizeof timestamp, "[%d/%m/%Y-%T]", std::localtime(&result));
			errno = 0;
			fs.open(LOG_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
			if (errno != 0) {
				throw std::exception();
			}
			fs << timestamp << " [ " << code << " ] - Matt_daemon: " << msg << std::endl;
			fs.close();
		}
};

#endif
