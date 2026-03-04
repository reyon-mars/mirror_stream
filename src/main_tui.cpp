#include "net/connection.hpp"
#include "net/net_except.hpp"
#include "net/socket.hpp"
#include "ui/term.hpp"
#include "ui/terminal_ui.hpp"
#include "utils/logger.hpp"
#include "utils/thread_pool.hpp"
#include <iostream>
#include <string>

int main()
{
	auto& term_ctx = ui::term::terminal_context::get_instance();
	term_ctx.clear();

	try
	{
		utils::thread_pool t_pool;

		net::sock_config config{};
		ui::terminal_ui::run(config);

		net::Socket echo_server(config);

		echo_server.bind(config.sock_addr);
		echo_server.listen();

		std::cout << "Echo server listening on " << config.sock_addr.ip << " : " << config.sock_addr.port << "\n";

		while (true)
		{
			try
			{
				net::Socket client = echo_server.accept();
				utils::logger::log("New client connected.");

				t_pool.submit(
					[client = std::move(client)]() mutable
					{
						net::echo(std::move(client));
					});
			}
			catch (const net::net_except& e)
			{
				utils::logger::log_err(std::string("Accept error: ") + e.what());
			}
		}
	}
	catch (const net::net_except& e)
	{
		utils::logger::log_err(std::string("Server error: ") + e.what());
		return 1;
	}
	catch (const std::exception& e)
	{
		utils::logger::log_err(std::string("Unexpected error: ") + e.what());
		return 2;
	}
	return 0;
}