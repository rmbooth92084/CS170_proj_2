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
    int max_num = 10;
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
       /* //looks though the new list of features and figures out which has the most cost
        for(int k = 0; k < temp->child.size(); k++){
            int cost = temp->child[k]->cost;
            if(cost > most_cost){
                most_cost = cost;
                most_pos = k;
                temp = temp->child[k];
            }
        }*/
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
int main(){
    srand(time(0));
    for(int k = 0; k < 10; k++){
        forward_selection(4);
    }
}