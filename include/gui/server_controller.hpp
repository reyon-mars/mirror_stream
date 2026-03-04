#pragma once
#include "net/sock_config.hpp"
#include "net/socket.hpp"
#include <QObject>
#include <QString>
#include <atomic>
#include <thread>

namespace gui
{
	class server_controller : public QObject
	{
		Q_OBJECT
	public:
		explicit server_controller(QObject *parent = nullptr);
		~server_controller() override;

		void start(net::sock_config cfg);

		void stop();

	signals:
		void log_msg(const QString &);
		void client_connected(const QString &);

	private:
		void server_loop(net::sock_config cfg);

		std::thread worker_;
		std::atomic_bool running_{false};
	};
} 
