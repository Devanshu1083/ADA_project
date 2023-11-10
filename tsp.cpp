#include<iostream>
#include<bits/stdc++.h>
using namespace std;

//number of nodes
#define N 5 
#define INF INT_MAX

class Node
{
public:
    vector <pair<int, int>> path;
    int matrix_reduced[N][N];
    int cost;
    int vertex;//current vertex
    int level;//to denote how many nodes we have visited
};
//utility function to create a new state from parent state in the state space tree
Node* newNode(int matrix_parent[N][N], vector<pair<int, int>> const &path,int level, int i, int j)
{
    Node* node = new Node;
    node->path = path;
    if (level != 0)
    {
        node->path.push_back(make_pair(i, j));
    }
    
    //copying the matrix to new node 
    memcpy(node->matrix_reduced, matrix_parent,
        sizeof node->matrix_reduced);
    
    //setting ith row and jth column of matrix of new node to infinity
    for (int k = 0; level != 0 && k < N; k++)
    {
        node->matrix_reduced[i][k] = INF;
        node->matrix_reduced[k][j] = INF;
    }
    //we cant go from j to 0 which was our start node hence it is also infinity
    node->matrix_reduced[j][0] = INF;
    //level denotes number of cities covered
    node->level = level;
    //assigning current city number
    node->vertex = j;
    return node;
}
//to get minimum cost state from state space, this comparater is used in the priority queue
class comp {
public:
    bool operator()(const Node* leftstate, const Node* rightstate) const
    {
        return (leftstate->cost > rightstate->cost);
    }
};
//funnction for finding cost and doing row reduction and column reduction
int cost_calculation(int (&matrix_reduced)[N][N])
{
    int cost = 0;
    //row reduction
    int row[N];
    fill_n(row, N, INF);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] < row[i])
                row[i] = matrix_reduced[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] != INF && row[i] != INF)
                matrix_reduced[i][j] -= row[i];
    
    //column reduction
    int col[N];
    fill_n(col, N, INF);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] < col[j])
                col[j] = matrix_reduced[i][j];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (matrix_reduced[i][j] != INF && col[j] != INF)
                matrix_reduced[i][j] -= col[j];
    
    //calculating the cost
    for (int i = 0; i < N; i++)
        cost += (row[i] != INT_MAX) ? row[i] : 0,
            cost += (col[i] != INT_MAX) ? col[i] : 0;

    return cost;
}
//utility function to print the lowest cost tour
void printPath(vector<pair<int, int>> const &list)
{
    for (int i = 0; i < list.size(); i++)
        {
            cout << list[i].first+1 << " -> ";
        }
        cout<<list[0].first+1<<endl;
}

int solve(int adjacensyMatrix[N][N])
{
    priority_queue<Node*, vector<Node*>, comp> pq;
    vector<pair<int, int>> v;
    Node* root = newNode(adjacensyMatrix, v, 0, -1, 0);
    root->cost = cost_calculation(root->matrix_reduced);
    pq.push(root);
    while (!pq.empty())
    {
        Node* min = pq.top();
        pq.pop();
        
        int i = min->vertex;
        //checking if we have traversed all the nodes i.e. completed our tour
        if (min->level == N - 1)
        {
            min->path.push_back(make_pair(i, 0));
            printPath(min->path);
            return min->cost;
        }
        
        //expanding further if tour not complete
        for (int j = 0; j < N; j++)
        {
            //not infinity means it is allowed to go to that node from current node as that node has not yet been traversed
            if (min->matrix_reduced[i][j] != INF)
            {
                Node* child = newNode(min->matrix_reduced, min->path,
                    min->level + 1, i, j);
                //calculating cost for next state 
                child->cost = min->cost + min->matrix_reduced[i][j]
                            + cost_calculation(child->matrix_reduced);

                pq.push(child);
            }
        }
        delete min;
    }
    return 0;
}

int main(){
    int adjacensyMatrix[N][N] =
    {
        { INF, 20, 30, 10, 11 },
        { 15,  INF, 16,  4,  2 },
        { 3,   5,  INF, 2,  4 },
        { 19,   6,   18,   INF, 3 },
        { 16,   4,   7,   16, INF }
    };
    

    cout << "\nCost is " << solve(adjacensyMatrix);

    return 0;
}