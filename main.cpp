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
#include <fstream>
using namespace std;

int test_var = INT_MIN;//debuigging variable
vector<int> name_test_var;//debugging variable
bool large;
bool uniuqe = true;
vector<vector<double>> data_set; //global variable for dealing with testing data
/*vector<vector<double>> data_set {
        {1 ,0.01 , 0.02 ,0.02},
        {2 ,0.01 , 0.01 ,0.03},
        {1 ,0.02 , 0.03 ,0.02},
        {1 ,0.03 , 0.02 ,0.02},
        {2 ,0.05 , 0.01 ,0.05}
};*/
double standard_deviation(double mean, double num_features, int feature){
    int i, j;
    double result = 0, sum = 0, size = data_set.size();
    for(j = 0; j < size; j++){
        sum += pow(data_set[j][feature] - mean, 2);
    }
    sum /= size - 1;
    result = pow(sum, 0.5);
    return result;
}
void normalize(){
    double mean = 0;;
    double deviation;
    int i, j, num_features = data_set[0].size() - 1;
    //has is start calculating the mean of the first feature
    for(i = 1; i <= num_features; i++){
        for(j = 0; j < data_set.size(); j++){
            mean += data_set[j][i];
        }
        deviation = standard_deviation(mean, num_features, i);
        //this normalizes the features after calculating the SD and mean of that funciton
        for(j = 0; j < data_set.size(); j++){
            data_set[j][i] = (data_set[j][i] - mean) / deviation;
        }
    }
}
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
//figures out the cost of the state by using corss-validation with
//the neariest neighbor algorithm
double evaluation_function(vector<int> features){
    int nearest_pos = -1;
    double nearest_dist = INT_MAX;
    double current_dist = 0;
    vector<double> temp[2];
    int temp_feature, num_correct = 0;
    //NN algorithm with leave one out cross-validation
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
            //cout << "success" << endl;
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
//global variable for getting a pointer on the final node for
//ui output purposes
Node * results;

void print_node(Node * node){
    cout << "The feature(s) ";
    for(int i = 0; i < node->name.size(); i++){
        cout << node->name[i] << " ";
    }
    cout << " accuracy is: " << node->cost << endl;
}
//this funciton does forward selection to make a tree of different
//combonations of features to find a local maximum starting with no features
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
//This funciton does backward elimination to make a tree of combonations of
//features to find a local maximum starting with all the from features
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
    double cost_temp;//This will be used to check if there in no child with a better value
    vector<int> name_temp;
    while(1){
        cost_temp = most_cost;
        name_temp = target_node->name;
        Node * node_temp;
        int num_children = name_temp.size();
        //creating all the child nodes of the target node
        for(int j = 0; j < num_children; j++){
            //removes a feature 
            it = name_temp.begin() + j;
            //removes it to make a new node with one less feature
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
            double cur_cost = children[k]->cost;
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
         << "(2) Backward Elimination" << endl;
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
        cout << "Invalid input" << endl;
    }
    cout << "out of algorithm" << endl;

    cout << "The best accuracy though the algorithm is: " << accuracy << " on state ";
    for(int i = 0; i <results->name.size(); i++){
        cout << results->name[i] << " ";
    }
    cout << endl;

}
//reads in data from a text file to test the algorithms
void read_in_data(){
    fstream my_file;
    vector<double> temp;
    //for reading the file
    if(large){
        if(uniuqe)
            my_file.open("CS170_Spring_2022_Large_data__5.txt", ios::in);
        else
            my_file.open("Large-test-dataset.txt", ios::in);
    }
    else{
        if(uniuqe)
            my_file.open("CS170_Spring_2022_small_data__5.txt", ios::in);
        else
            my_file.open("small-test-dataset.txt", ios::in);
    }
    if(my_file.is_open()){
        
        double class_id;
        if(large)
            temp = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//for the large data set 
        else
            temp = {0,0,0,0,0,0,0,0,0,0,0};//for the small data set
        
        while(my_file >> class_id){
            temp[0] = class_id; //puts class id in the 0 position
            //get all the feature values in the line before getting the name of the next line
            for(int i=1; i < temp.size(); i++){
                my_file >> temp[i];
            }
            data_set.push_back(temp);
        }
        my_file.close();
    }
    else{
        cout << "Failed to read file" << endl;
    }
    /*
    for(int i = 0; i < data_set.size(); i++){
        cout << data_set[i][0]  << " "<< data_set[i][1] << endl;
    }
    */
}
int main(){
    int temp;
    cout << "Select data set: (1) Large (2) Small"  << endl;
    cin >> temp;
    if(temp == 1)
        large = true;
    else if(temp == 2)
        large = false;

    read_in_data();

    cout << "Normalize data? : (1) Normalize (2) Not normalize"  << endl;
    int temp2;
    cin >> temp2;
    if(temp2 == 1)
        normalize();
    ui();
}