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
vector<vector<double>> data_set {
        {1 ,0.01 , 0.02 ,0.02},
        {2 ,0.01 , 0.01 ,0.03},
        {1 ,0.02 , 0.03 ,0.02},
        {1 ,0.03 , 0.02 ,0.02},
        {2 ,0.05 , 0.01 ,0.05}
};
//calculates the starting accuracy of the test without alrgorithm
//works with up to 4 different clasifacitons
double default_accuracy(vector<vector<double>> input){
    vector<int> count{0,0,0,0};
    int highest_clasif;
    int current_most = 0;
    for(int i = 0; i < input.size(); i++){
        count[input[i][0]]++;
    }
    for(int j = 0; j < count.size(); j++){
        if(count[j] > current_most) {
            current_most = count[j];
            highest_clasif = j;
        }
    }

    return current_most / (double)input.size();
}
//calculates the uclidian distance
double euclidean_distance(double x1, double y1, double x2, double y2){
    double result = pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
    return result;
}

//figures out the cost of the state

double evaluation_function(vector<int> features){
    int nearest_pos = -1;
    double nearest_dist = INT_MAX;
    double current_dist = 0;
    vector<double> temp[2];
    int temp_feature, num_correct = 0;
    //NN algorithm
    for(int i = 0; i < data_set.size(); i++){
        temp[0] = data_set[i];
        nearest_pos = -1;
        nearest_dist = INT_MAX;
        for (int j = 0; j < data_set.size(); j++){
            //if it's comparing the distance between itself and itself then skip
            if(i == j)
                continue;
            temp[1] = data_set[j];
            //gets the distance between the target instance and the other instace before square rooting it
            for(int k = 0; k < features.size(); k++){
                temp_feature = features[k];
                //preps for a variable amount of features to cal the distance
                current_dist += pow(temp[0][temp_feature] - temp[1][temp_feature], 2.0);
            }
            
            //take the square root of the calculation to find the distance between the points
            current_dist = pow(current_dist, 0.5);
            if(current_dist < nearest_dist){
                nearest_dist = current_dist;
                nearest_pos = j;
            }
            
        }
      //  cout << "Nearest neighbor for instance " << i << " is " << nearest_pos 
      //  << " at a distance of : " << nearest_dist << endl;
        //if the clasifaciton of the nearest neighbor is the same as the test node then
        //we add one to the num correct
        if(temp[0][0] == data_set[nearest_pos][0]){
            num_correct++;
            cout << "success" << endl;
        }
    }

    return (double)num_correct / (double) data_set.size();
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
    temp->cost = evaluation_function(input_name);
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
double forward_selection(int num_features){
    test_var = INT_MIN;//resets the testing max value
    vector<Node *> base_features;
    vector<int> temp_name {0};
    Node *root = new_root_node();
    Node *temp;//the current node we are looking at
    double most_cost = INT_MIN, most_pos;
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
    double cost_temp;//This will be used to check if there in no child with a better value
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
double backward_elimination(int num_features){
    test_var = INT_MIN;//resets the testing max value
    vector <int> root_name;
    double most_cost;
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
    double accuracy, start_acc = default_accuracy(data_set);
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