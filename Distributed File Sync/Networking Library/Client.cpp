#include "Client.h"

Client::Client() {

}

Client::~Client() {

}

//times out after 30 seconds
bool Client::connect(std::string ip, unsigned short port) {
	sf::Socket::Status status = socket.connect(ip, port, sf::seconds(30));
	if (status != sf::Socket::Done) {
		return false;
	}
	return true;
}

//for sending basic strings
bool Client::sendString(std::string data) {
	sf::Socket::Status status = socket.send(data.c_str(), data.size());
	if (status != sf::Socket::Done) {
		return false;
	}
	return true;
}

//for sending packets
bool Client::send(sf::Packet& packet) {
	sf::Socket::Status status = socket.send(packet);
	if (status != sf::Socket::Done) {
		return false;
	}
	return true;
}

std::string Client::receiveString(int buffer) { //default value of 1024
	std::size_t size;
	//create buffer for incoming data
	char* data = new char[buffer];
	if (socket.receive(data, buffer, size) != sf::Socket::Done) {
		return "";
	}
	//copy buffer data to string
	std::string message(data, size);
	//free buffer memory
	delete[] data;
	return message;
}

//for receiving packets
sf::Socket::Status Client::receive(sf::Packet& packet) {
	return socket.receive(packet);
}

bool Client::handle() {
	if (todo.empty()) return false;
	//pull packet from the queue
	sf::Packet* packet = todo.front();
	std::string ip, message;
	(*packet) >> ip >> message;
	//read out message that was contained in the packet
	std::cout << ip << message << std::endl;
	todo.pop();
	//free memory used for packet
	delete packet;
	return true;
}

int Client::getTodoCount() const {
	return todo.size();
}