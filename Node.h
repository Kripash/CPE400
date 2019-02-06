// CPE 400 Project: Dynamic Routing Mechanism Design in Faulty Network
// Created by Erik Miannay and Kripash Shrestha

//Note: Only Send_RREP() is commented; other Node members are trivial

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <time.h>

using namespace std;

struct request_check
{
	char originator_of_RREQ;
	int request_ID_of_RREQ;
};
	
class Node
{
	public:
		Node(char node_letter, map<char, Node*> connections);
		Node& operator=(const Node& other);
		Node* memory_location();

		char node_name;
		void assign_connections(map<char, Node*> connections);
		void insert_single_connection(char node_name, Node* location);
		void remove_single_connection(char node_name);
		bool am_I_connected(char node_name);
		map<char, Node*> node_connections;
		void send_RREP(char src, char destination, string RREQ_reply, int size, char RREP_source);

		vector<request_check> previous_requests;
		string RREQ_string;
		string RREP_string;
		bool received_path;
		bool received_response;
};

Node::Node(char node_letter, map<char, Node*> connections)
{
	node_name = node_letter;
	assign_connections(connections);
	received_path = false;
	received_response = false;
}

Node& Node::operator=(const Node& rhs)
{
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}

Node* Node::memory_location()
{
	return this;
}

void Node::assign_connections(map<char, Node*> connections)
{
	node_connections = connections;
}

void Node::insert_single_connection(char node_name, Node* location)
{
	node_connections.insert(std::pair<char, Node*>(node_name, location));
}

void Node::remove_single_connection(char node_name)
{
	node_connections.erase(node_name);
}

bool Node::am_I_connected(char node_name)
{
	if(node_connections.count(node_name) > 0)
		return true;
	else
		return false;
}

void Node::send_RREP(char src, char destination, string RREQ_reply, int size, char RREP_source)
{
//this simulates an RREQ message; it is implemented recursively, so it will take precedent over other function calls in main()
	//RREP received, what message to display?
	if(size == 0){
		cout	<< "Node " << node_name << " successfully received path to Node " << RREP_source
				<< " with route [" << RREQ_reply << RREP_source << "]" << endl;
	}
	else if(size == (RREQ_reply.size()-1)){		
		cout 	<< "Node " << node_name << " received RREP from Node " << RREP_source
				<< " to Node " << destination << " with route [" << RREQ_reply << RREP_source << "]" <<endl;
	}
	else if(size != RREQ_reply.size()){
		cout 	<< "Node " << node_name << " received RREP from Node " << RREQ_reply[size+1]
				<< " to Node " << destination << " with route [" << RREQ_reply << RREP_source << "]" << endl;
	}
	else {}
	
	//send next RREP along path
	received_response = true;
	map<char, Node*>::iterator find_iterator;
	Node * next_node = NULL;
	
	if((size-1) >= 0)
	{
		if(size != 0)
		{
			find_iterator = node_connections.find(RREQ_reply[size-1]);
		}
		next_node = find_iterator->second;
	}
	if(next_node != NULL)
	{
		next_node->RREP_string = RREQ_reply;
		next_node->send_RREP(next_node->node_name, destination, RREQ_reply, size-1, RREP_source);
	}
}