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

int evaluation_function();
int test_var = INT_MIN;
vector<int> name_test_var;

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
//figures out the cost of the state
//currently random number is
int evaluation_function(){
    int max_num = 10;
    int temp = rand() % max_num;
    if(temp > test_var){
        test_var = temp;
    }
    return temp;
}
/*
//tests if the names are the same
//currently doesn't account for names with different order of states
bool check_name(vector<int> test, vector<int> target){
    if(test.size() != target.size())
        return false;
    for(int i = 0; i < test.size(); i++){
        if(test[i] != target[i])
            return false;
    }
    return true;
}*/

void forward_selection(int num_features){
    vector<Node *> base_features;
    vector<Node *> new_features;
    vector<Node *> new_features_temp;
    vector<int> temp_name {0};
    Node *root = new_root_node();
    Node *temp;//the current node we are looking at
    int i, most_cost = INT_MIN, most_pos;
    //first loop for all the base_features;
    for(i = 0; i < num_features; i++){
        temp_name[0] = i + 1;
        temp = new_node(temp_name);
        base_features.push_back(temp);
        root->child.push_back(temp);
    }
    for(i = 0; i < base_features.size(); i ++){
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

    new_features = base_features;
    vector<int> name_temp;
    int cost_temp;//This will be used to check if there in no child with a better value
    while(1){
        cost_temp = most_cost;
        //creates the new child states from the current node we are looking at
        for(i = 0; i < new_features.size(); i ++){
            name_temp = temp->name;
            name_temp.push_back(new_features[i]->name[0]);
            new_features_temp.push_back(new_node(name_temp));
            //garbage collection
            name_temp.clear();
            name_temp.shrink_to_fit();
        }
        new_features = new_features_temp;
        //garbage collection
        new_features_temp.clear();
        new_features_temp.shrink_to_fit();
        //looks though the new list of features and figures out which has the most cost
        for(i = 0; i < new_features.size(); i ++){
            if(new_features[i]->cost > most_cost){
                most_cost = base_features[i]->cost;
                most_pos = i;
                temp = base_features[i];
            }
        }
        //if our current node is the most then we found the best one
        if(cost_temp == most_cost)
            break;        
    }
    cout << "most cost from algorithm: " << most_cost << endl;
    cout << "most cost from all nodes made: " << test_var << endl;
    cout << endl;
}
int main(){
    srand(time(0));
    forward_selection(4);
}