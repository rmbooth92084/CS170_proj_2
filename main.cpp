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

Node * results;

void print_node(Node * node){
    cout << "The feature(s) ";
    for(int i = 0; i < node->name.size(); i++){
        cout << node->name[i];
    }
    cout << " accuracy is: " << node->cost << endl;
    cout << endl;
}
int forward_selection(int num_features){
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

        print_node(temp);
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
                temp = new_child;
            }
            print_node(new_child);
        }

        //if we didn't find a new node with a better cost then this is the best one
        //for the algorithm
        if(cost_temp == most_cost ){
            //cout << base_features.size() << endl;
            break; 
        }
     
    }
    results = temp;
    return temp->cost;
    /*
    bool results;
    results = most_cost == test_var ? true : false;
    cout << "Test results " << results << endl;*/
   // cout << "current most: " << most_cost << endl;
   // cout << "overall most: " << test_var << endl;


}
int backward_elimination(int num_features){
    test_var = INT_MIN;//resets the testing max value
    vector <int> root_name;
    int most_cost;
    Node * target_node;
    vector<int>::iterator it;

    for(int i = 0; i < num_features; i++){
        root_name.push_back(i + 1);
    }

    Node * root = new_node(root_name);
    print_node(root);
    most_cost = root->cost;
    target_node = root;
    int cost_temp;//This will be used to check if there in no child with a better value
    vector<int> name_temp;
    while(1){
        cost_temp = most_cost;
        name_temp = target_node->name;
        Node * node_temp;
        //creating the child nodes of the target node
        for(int j = 0; j < name_temp.size() ; j++){
            //removes a feature 
            it = name_temp.begin() + j;
            name_temp.erase(it);
            node_temp = new_node(name_temp);
            //creates the node with that less feature and puts it as a child
            target_node->child.push_back(node_temp);
            //reset temp variable to make sure we are only removing one feature at a time
            name_temp = target_node->name;

            print_node(node_temp);
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
    results = target_node;
    return target_node->cost;
    /*
    bool results;
    results = most_cost == test_var ? true : false;
    cout << "Test results " << results << endl;*/
}
void ui(){
    int accuracy, start_acc = evaluation_function();
    cout << "Welcome to Raymond Booth's Feature Seleciton Algorithm" << endl
         << "Please enter the number of features you want: ";
    int user_features_input;
    cin >> user_features_input;
    cout << endl;

    cout << "Please select the alforithm you want to run" << endl
         << "(1) Foward Seleciton" << endl
         << "(2) Backward Elimination" << endl
         << "(3) Raymond's Special Algorithm (not made yet)" << endl;
    int user_al_input;
    cin >> user_al_input;
    cout << "Using no features, the accuracy is: " << start_acc << endl;
    
    if(user_al_input == 1){
        accuracy = forward_selection(user_features_input);
    }
    else if(user_al_input == 2){
        accuracy = backward_elimination(user_features_input);
    }
    else{
        cout << "not implemented yet" << endl;
    }
    cout << "out of algorithm" << endl;

    cout << "The best accuracy is: " << accuracy << " on state ";
    for(int i = 0; i <results->name.size(); i++){
        cout << results->name[i];
    }
    cout << endl;

}
int main(){
    srand(time(0));
    /*
    cout << "backwards elemintion random test results (1 pass, 0 fail) :" << endl;
    for(int k = 0; k < 5; k++){
        backward_elimination(4);
    }
    cout << "forward selection random test results (1 pass, 0 fail) :" << endl;
    for(int k = 0; k < 5; k++){
        forward_selection(4);
    }*/
    ui();
}