// CPE 400 Project: Dynamic Routing Mechanism Design in Faulty Network
// Created by Erik Miannay and Kripash Shrestha

//This program simulates the DSR routing algorithm in a highly volatile network.
//Read corresponding writeup for more information.

//To compile, just run the ‘make’ command within a directory that has ‘DSR.cpp’ and 'Node.h'
//Then run: ./DSR

#include "Node.h"
#include <time.h>

#define NUMBER_OF_NODES 13

using namespace std;

Node * pointer_for_node(char node, map<char, Node*> node_map)
//this function returns the pointer to a node, given its name
{
	map<char, Node*>::iterator find_iterator = node_map.find(node);	
	return (find_iterator->second);
}

void add_bidirectional_edge(char first_node, char second_node, map<char, Node*> node_map)
//this function adds an edge between two nodes, by adding pointers to each other in their connection's lists
{
	Node * first_node_pointer = pointer_for_node(first_node, node_map);
	Node * second_node_pointer = pointer_for_node(second_node, node_map);
	
	first_node_pointer->insert_single_connection(second_node, second_node_pointer);
	second_node_pointer->insert_single_connection(first_node, first_node_pointer);
}

void destroy_bidirectional_edge(char first_node, char second_node, map<char, Node*> node_map)
//this function removes an edge between two nodes, by deleting pointers in their connection's lists
{
	pointer_for_node(first_node, node_map)->remove_single_connection(second_node);
	pointer_for_node(second_node, node_map)->remove_single_connection(first_node);	
}

bool are_connected(char first_node, char second_node, map<char, Node*> node_map)
//this function determines whether two nodes are connected
{
	return pointer_for_node(first_node, node_map)->am_I_connected(second_node);
}

struct two_nodes
{
	char node_a;
	char node_b;
};

two_nodes randomly_select_two_nodes(char A_K[])
//this function randomly selects two nodes between A to K
{
	two_nodes selected_nodes;
	selected_nodes.node_a = A_K[rand() % NUMBER_OF_NODES];
	selected_nodes.node_b = A_K[rand() % NUMBER_OF_NODES];
	while(selected_nodes.node_a == selected_nodes.node_b)
	{
		selected_nodes.node_a = A_K[rand() % NUMBER_OF_NODES];
		selected_nodes.node_b = A_K[rand() % NUMBER_OF_NODES];		
	}
	return selected_nodes;
}

void thread_to_create_destroy_links(map<char, Node*> node_map, char A_K[])
{
//this function simulates a thread, and randomly creates and adds edges
	//randomly destroy edges 
	for(int i=0; i<(rand() % 10); i++)
	{
		two_nodes generated_nodes = randomly_select_two_nodes(A_K);
		while(!pointer_for_node(generated_nodes.node_a, node_map)->am_I_connected(generated_nodes.node_b))
		{
			generated_nodes = randomly_select_two_nodes(A_K);
		}
		cout << "-->Randomly destroyed edge between: (" << generated_nodes.node_a << ", " << generated_nodes.node_b << ")" << endl;
		destroy_bidirectional_edge(generated_nodes.node_a, generated_nodes.node_b, node_map);
	}
	
	//randomly create edges
	for(int i=0; i<(rand() % 10); i++)
	{
		two_nodes generated_nodes = randomly_select_two_nodes(A_K);
		cout << "-->Randomly created edge between: (" << generated_nodes.node_a << ", " << generated_nodes.node_b << ")" << endl;
		add_bidirectional_edge(generated_nodes.node_a, generated_nodes.node_b, node_map);
	}
}

void send_RREQ(map<char, Node*> node_map, char src, int request_ID, char destination, char A_K[]);
//function definition is below main()

int main()
{
	char A_K[NUMBER_OF_NODES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M'};

	//instantiate nodes
	map<char, Node*>	nothing;
	map<char, Node*>	node_map;
	Node				A('A', nothing);
	Node				B('B', nothing);
	Node				C('C', nothing);
	Node				D('D', nothing);
	Node				E('E', nothing);
	Node				F('F', nothing);
	Node				G('G', nothing);
	Node				H('H', nothing);
	Node				I('I', nothing);
	Node				J('J', nothing);
	Node				K('K', nothing);
	Node				L('L', nothing);
	Node				M('M', nothing);

	//define map
	node_map.insert(pair<char, Node*>('A', A.memory_location()));
	node_map.insert(pair<char, Node*>('B', B.memory_location()));
	node_map.insert(pair<char, Node*>('C', C.memory_location()));
	node_map.insert(pair<char, Node*>('D', D.memory_location()));
	node_map.insert(pair<char, Node*>('E', E.memory_location()));
	node_map.insert(pair<char, Node*>('F', F.memory_location()));
	node_map.insert(pair<char, Node*>('G', G.memory_location()));
	node_map.insert(pair<char, Node*>('H', H.memory_location()));
	node_map.insert(pair<char, Node*>('I', I.memory_location()));
	node_map.insert(pair<char, Node*>('J', J.memory_location()));
	node_map.insert(pair<char, Node*>('K', K.memory_location()));
	node_map.insert(pair<char, Node*>('L', L.memory_location()));
	node_map.insert(pair<char, Node*>('M', M.memory_location()));

	//create edges
	add_bidirectional_edge('A', 'B', node_map);
	add_bidirectional_edge('B', 'C', node_map);
	add_bidirectional_edge('C', 'D', node_map);
	add_bidirectional_edge('D', 'E', node_map);
	add_bidirectional_edge('D', 'F', node_map);
	add_bidirectional_edge('E', 'F', node_map);
	add_bidirectional_edge('E', 'H', node_map);
	add_bidirectional_edge('E', 'K', node_map);
	add_bidirectional_edge('F', 'G', node_map);
	add_bidirectional_edge('G', 'H', node_map);
	add_bidirectional_edge('G', 'I', node_map);
	add_bidirectional_edge('H', 'I', node_map);
	add_bidirectional_edge('H', 'J', node_map);
	add_bidirectional_edge('H', 'K', node_map);
	add_bidirectional_edge('I', 'J', node_map);
	add_bidirectional_edge('J', 'K', node_map);
	add_bidirectional_edge('L', 'M', node_map);

	//simulation
	srand(time(NULL));
	
	//fixed network example
	cout << endl << "**************************************" << endl;
	cout << "First, fixed network simulation:" << endl;
	cout << endl << "Route Discovery: A to K" << endl;
	
	send_RREQ(node_map, 'A', rand(), 'K', A_K);
	
	cout << endl << "-->Destroyed edge between B and C." << endl; 
	destroy_bidirectional_edge('B', 'C', node_map);
	
	cout << endl << "Try to find route between A to K, again:" << endl;
	send_RREQ(node_map, 'A', rand(), 'K', A_K);
	
	cout << endl << "-->Restore edge between B and C, to get get original network back." << endl; 
	add_bidirectional_edge('B', 'C', node_map);
	
	//highly volatile example
	cout << endl << "**************************************" << endl;
	cout << "Now, highly volatile network simulation:" << endl;
	
	for(int times=0; times<5; times++)
	{
		cout << endl << "--------------------------------" << endl;
		cout << endl << "Volatile simulation #" << times+1 << endl;
		//find path between two random edges
		two_nodes generated_nodes = randomly_select_two_nodes(A_K);
		cout << endl << "Route Discovery: " << generated_nodes.node_a << " to " << generated_nodes.node_b << endl;
		
		send_RREQ(node_map, generated_nodes.node_a, rand(), generated_nodes.node_b, A_K);
		
		//call simulated thread
		thread_to_create_destroy_links(node_map,A_K);
		
		cout << endl << "--------------------------------" << endl;
	}
	
	//redo fixed network, to see if path between A and K changed
	cout << endl << "**************************************" << endl;
	cout << "Lastly, see if path between A to K changed after all of those edges being destroyed/created:" << endl;
	cout << endl << "Route Discovery: A to K" << endl;
	
	send_RREQ(node_map, 'A', rand(), 'K', A_K);
	
	return 0;
}

void send_RREQ(map<char, Node*> node_map, char src, int request_ID, char destination, char A_K[])
{
//this simulates an RREQ message
	time_t start = time(0);
	Node * current_node = pointer_for_node(src, node_map);
	current_node->received_path = true;
	
	//current_node refers to current node being traversed
	//originator refers to where RREQ was started
	
	vector<char> neighbor_vector;
	while(1)
	{
		bool check = true;
		//did current_node already ask its neighbors to check for (originator_of_RREQ, request_ID)? if so, ignore this RREQ
		for(int i = 0; i < current_node->previous_requests.size(); i++)
		{
			if(	current_node->previous_requests[i].originator_of_RREQ == src &&
				current_node->previous_requests[i].request_ID_of_RREQ == request_ID)
			{
				check = false; //ignore
			}
		}
		//no previous ask? so now current_node will check its neighbors then and ask each of its neighbors
		if(check)
		{
			//save this RREQ to previous requests
			request_check insert_record = {src, request_ID};
			current_node-> previous_requests.push_back(insert_record);
			//if current_node is desired node, no need to ask neighbors
			if(current_node->node_name != destination)
			{
				//for each of current_node’s neighbors, ask RREQ
				for(map<char, Node*>::const_iterator it = current_node->node_connections.begin(); it != current_node->node_connections.end(); it++)
				{
					Node * neighbor_node = it->second;
					if(neighbor_node->received_path == false)
					{
						//send neighbor_node current path taken from originator
						neighbor_node->RREQ_string = (current_node->RREQ_string + (current_node->node_name));
						neighbor_node->received_path = true;
						if(neighbor_node->node_name == destination)
						{ 
						//found destination!
							cout 	<< "Node " << neighbor_node->node_name << " received a RREQ from Node " << current_node->node_name
									<< " to get to this node! So, begin RREP ["
									<< (neighbor_node->RREQ_string + neighbor_node->node_name) << "]" << endl;
							//now, begin journey back to RREQ originator by starting RREP
							neighbor_node->send_RREP(destination, src, neighbor_node->RREQ_string, (neighbor_node->RREQ_string).size(), destination);
						}
						else
						{
						//did not find, so neighbor_node will now ask its neighbors
							cout	<< "Node " << neighbor_node->node_name << " received a RREQ from Node " << current_node->node_name
									<< " to get to Node " << destination << ", list of identifiers: " << neighbor_node->RREQ_string << endl;
						}
					}
					//add neighbor_node to queue, to forward RREQ
					neighbor_vector.push_back(it->first);
				}
			}
			else
			{
				//do nothing
			}
		}
		//update current_node to next in queue
		if(neighbor_vector.size() > 0)
		{
			current_node = pointer_for_node(neighbor_vector[0], node_map);
			neighbor_vector.erase(neighbor_vector.begin());
		}
		//no response after 1.0 seconds means destination isn't in network
		if(difftime(time(0), start) > 1.0)
		{
			Node * verify = pointer_for_node(src, node_map);
			if(!verify->received_response)
			{
				cout << "No route could be found from Node " << src << " to Node " << destination << endl;
			}
			break;
		}
	}
	//reset received
	Node * reset_node = NULL;
	for(int i=0; i<NUMBER_OF_NODES; i++)
	{	reset_node = pointer_for_node(A_K[i], node_map);
		reset_node->received_path = false;
		reset_node->RREP_string = "";
		reset_node->RREQ_string = "";
	}
	Node * reset_source_node = pointer_for_node(src, node_map);
	reset_source_node->received_response = false;
	reset_source_node->RREP_string = "";
	reset_source_node->RREQ_string = "";
}