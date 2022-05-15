/*
    Author - Raymond Booth 862152152
    github repo - https://github.com/rmbooth92084/CS170_proj_2
*/
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <cstdlib>
using namespace std;

int test_var = INT_MIN;
vector<int> name_test_var;
//figures out the cost of the state
//currently random number is
int evaluation_function(){
    int max_num = 100;
    int temp = rand() % max_num;
    if(temp > test_var){
        test_var = temp;
    }
    return temp;
}
//for making the tree with any amount of leaves per node
struct Node
{
    vector<int> name;
    double cost;
    vector<Node *> child;//pointer to each child
};
//Fuction for helping with creating child nodes
Node *new_node(vector<int> input_name)
{
    Node *temp = new Node;
    temp->name = input_name;
    temp->cost = evaluation_function();
    return temp;
}
//function for helping with creating the root node
Node *new_root_node()
{
    Node *temp = new Node;
    temp->name.push_back(0);
    temp->cost = INT_MAX;
    return temp;
}

void print_node(Node * node){
    cout << "Node name is: ";
    for(int i = 0; i < node->name.size(); i++){
        cout << node->name[i];
    }
    cout << endl;
    cout << "The cost of it is: " << node->cost << endl;
    cout << endl;
}
void forward_selection(int num_features){
    test_var = INT_MIN;//resets the testing max value
    vector<Node *> base_features;
    vector<int> temp_name {0};
    Node *root = new_root_node();
    Node *temp;//the current node we are looking at
    int most_cost = INT_MIN, most_pos;
    //first loop and make all the base features
    for(int i = 0; i < num_features; i++){
        temp_name[0] = i + 1;
        temp = new_node(temp_name);
        base_features.push_back(temp);
        root->child.push_back(temp);

        //print_node(temp);
    }
    //find the feature that has the best cost and make it the state we are looking at
    for(int i = 0; i < base_features.size(); i ++){
        if(base_features[i]->cost > most_cost){
            most_cost = base_features[i]->cost;
            temp = base_features[i];
            most_pos = i;
        }
    }

    //removes the node we are exploring which leaves the rest of the
    //rest for making the new states
    vector<Node *>::iterator it = base_features.begin() + most_pos;
    base_features.erase(it);

    vector<int> name_temp;
    int cost_temp;//This will be used to check if there in no child with a better value
    while(1){
        cost_temp = most_cost;
        //removes feature that is already in the current state we are looking at
        for(int i = 0; i < base_features.size(); i ++){
            for(int j = 0; j < temp->name.size(); j++){
                if(base_features[i]->name[0] == temp->name[j]){
                    it = base_features.begin() + i;
                    base_features.erase(it);
                }
            }
        }
        Node * new_child;
        Node * target;
        //creates the new child states from the current node we are looking at
        for(int i = 0; i < base_features.size(); i ++){
            name_temp = temp->name;
            name_temp.push_back(base_features[i]->name[0]);
            new_child = new_node(name_temp);
            temp->child.push_back(new_child);

            if(new_child->cost > most_cost){
                most_cost = new_child->cost;
                target = new_child;
            }
            //print_node(new_child);
        }
        temp = target;

        //if we didn't find a new node with a better cost then this is the best one
        //for the algorithm
        if(cost_temp == most_cost ){
            //cout << base_features.size() << endl;
            break; 
        }
     
    }
    bool results;
    results = most_cost == test_var ? true : false;
    cout << "Test results " << results << endl;
   // cout << "current most: " << most_cost << endl;
   // cout << "overall most: " << test_var << endl;


}
void backward_elimination(int num_features){
    test_var = INT_MIN;//resets the testing max value
    vector <int> root_name;
    int most_cost;
    Node * target_node;
    vector<int>::iterator it;

    for(int i = 0; i < num_features; i++){
        root_name.push_back(i + 1);
    }

    Node * root = new_node(root_name);
    most_cost = root->cost;
    target_node = root;
    int cost_temp;//This will be used to check if there in no child with a better value
    vector<int> name_temp;
    while(1){
        cost_temp = most_cost;
        name_temp = target_node->name;
        //creating the child nodes of the target node
        for(int j = 0; j < name_temp.size() ; j++){
            //removes a feature 
            it = name_temp.begin() + j;
            name_temp.erase(it);
            //creates the node with that less feature and puts it as a child
            target_node->child.push_back(new_node(name_temp));
            //reset temp variable to make sure we are only removing one feature at a time
            name_temp = target_node->name;
        }
        vector<Node *> children = target_node->child;
        //If there is a new node that has better accuracy then that node becomes
        //the target node
        for(int k = 0; k < children.size(); k++){
            int cur_cost = children[k]->cost;
            if(cur_cost > most_cost){
                most_cost = cur_cost;
                target_node = children[k];
            }
        }

        if(cost_temp == most_cost)
            break;
    }
    bool results;
    results = most_cost == test_var ? true : false;
    cout << "Test results " << results << endl;
}
int main(){
    srand(time(0));
    for(int k = 0; k < 10; k++){
        backward_elimination(4);
    }
}