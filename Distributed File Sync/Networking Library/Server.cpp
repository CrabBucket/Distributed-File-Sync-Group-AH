#include "Server.h"

Server::Server() {

}

Server::~Server() {

}

sf::IpAddress Server::accept() {
	sf::TcpSocket* client = new sf::TcpSocket();
	//accept next connection request
	if (listener.accept(*client) != sf::Socket::Done) {
		std::cout << "Server could not accept" << std::endl;
		return sf::IpAddress::None;
	}
	//log clients ip
	clients[client->getRemoteAddress()] = client;
	return client->getRemoteAddress();
}

void Server::accept(int n) {
	sf::SocketSelector selector;
	selector.add(listener);
	//accept 10 connections, breaks if nothing requested for 10 seconds
	for (int i = 0; i < n; i++) {
		if (selector.wait(sf::seconds(10.f)))
		{
			accept();
		}
		else {
			break;
		}
	}
}

bool Server::listen(unsigned short port) {
	if (listener.listen(port) != sf::Socket::Done) {
		return false;
	}
	return true;
}

//for sending basic strings
bool Server::send(const std::string& data, const sf::IpAddress& dest) {
	sf::Socket::Status status = clients[dest]->send(data.c_str(), data.size());
	if (status != sf::Socket::Done) {
		return false;
	}
	return true;
}
//for sending packets
bool Server::send(sf::Packet& packet, const sf::IpAddress& dest) {
	sf::Socket::Status status = clients[dest]->send(packet);
	if (status != sf::Socket::Done) {
		return false;
	}
	return true;
}

//for receiving basic strings
std::string Server::receiveString(const sf::IpAddress& source, int buffer) { //default value of 1024
	std::size_t size;
	std::cout << buffer << std::endl;
	//create a buffer
	char* data = new char[buffer];
	if (clients[source]->receive(data, buffer, size) != sf::Socket::Done) {
		return "null";
	}
	//copy message to string
	std::string message(data, size);
	//free the buffer memory
	delete[] data;
	return message;
}

//for receiving packets
bool Server::receive(sf::Packet& packet, const sf::IpAddress& source) {
	if (clients[source]->receive(packet) != sf::Socket::Done) {
		return false;
	}
	return true;
}

bool Server::handle() {
	if (todo.empty()) return false;
	//grab packet from front of queue
	sf::Packet* packet = todo.front();
	std::string ip;
	(*packet) >> ip;
	//send packet back to sender
	if (send(*packet, ip)) {
		std::cout << "sent massage to " << ip << std::endl;
		todo.pop();
		//free memory used for packet
		delete packet;
		return true;
	}
	return false;
}

int Server::getTodoCount() const {
	return todo.size();
}

std::vector<sf::IpAddress> Server::getClientIps() {
	std::vector<sf::IpAddress> ips;
	std::map<sf::IpAddress, sf::TcpSocket*>::iterator it = clients.begin();
	//iterates through map of client ips
	while (it != clients.end()) {
		ips.push_back(it->first);
		it++;
	}
	return ips;
}

std::string Server::getNBytes(std::ifstream& file, std::size_t startPosition, int n) {
	std::string buffer = "";
	char c;
	//move to start position and read n bytes
	file.seekg(startPosition);
	for (int i = 0; i < n; i++) {
		if (file.get(c)) {
			buffer += c;
		}
		else {
			break;
		}
	}
	return buffer;
}