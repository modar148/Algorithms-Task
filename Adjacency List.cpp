#include <bits/stdc++.h>

using namespace std;
const int MAX = 1001;
int State[MAX];
int TimeIn[MAX] , TimeOut[MAX] , TimerIn = 0 , TimerOut = 0;
vector<int> adj[MAX] , Reversed_adj[MAX] , BackEdge_adj[MAX] , TreeEdge_adj[MAX];

// This Part Has The Depth First Search Algorithm.
void DFS(int Node , int NumberOfNodes)
{
    State[Node] = 1;
    TimeIn[Node] = ++TimerIn;

    for(auto NewNode : adj[Node])
    {
        if(State[NewNode] == 0)
        {
            DFS(NewNode , NumberOfNodes);
        }
    }
    State[Node] = 2;
    TimeOut[Node] = ++TimerOut;
}
void DFS_INIT()
{
    TimerIn = 0 , TimerOut = 0;
    for(int i = 1; i < MAX; i++)
    {
        TimeIn[i] = 0;
        TimeOut[i] = 0;
        State[i] = 0;
    }
}
// This Part Has The Strongly Connected Component Algorithm.
void SCC_DFS(int Node ,int NumberOfNodes, int &Counter , vector<int> Curradj[])
{
    State[Node] = 1;
    Counter++;

    for(auto NewNode : Curradj[Node])
    {
        if(State[NewNode] == 0)
        {
            SCC_DFS(NewNode , NumberOfNodes , Counter , Curradj);
        }
    }
    State[Node] = 2;
}
bool SCC_Check(int NumberOfNodes)
{
    int Counter = 0;
    SCC_DFS(1 , NumberOfNodes , Counter , adj);
    if(Counter != NumberOfNodes) return 0;
    DFS_INIT();
    Counter = 0;
    SCC_DFS(1 , NumberOfNodes , Counter , Reversed_adj);
    return (Counter == NumberOfNodes);
}


// This Part Has The Melhlorn Algorithm.
stack<int> Rstack , Ostack;
bool InOstack[MAX];
int Comp[MAX];
vector<int> DFS_Order;
void Melhorn(int Node , int NumberOfNodes)
{
    State[Node] = 1;
    TimeIn[Node] = ++TimerIn;
    Rstack.push(Node);
    Ostack.push(Node);
    InOstack[Node] = 1;
    for(auto NewNode : adj[Node])
    {
        if(State[NewNode] == 0)
        {
            Melhorn(NewNode , NumberOfNodes);
        }
        else
        {
            if(InOstack[NewNode])
            {
                while(TimeIn[NewNode] < TimeIn[Rstack.top()])
                {
                    Rstack.pop();
                }
            }
        }
    }
    State[Node] = 2;
    if(Node == Rstack.top())
    {
        Rstack.pop();
        int Poped;
        do{
            Poped = Ostack.top();
            Ostack.pop();
            Comp[Poped] = Node;
            InOstack[Poped] = 0;
        }while(Poped != Node);
    }
}


//This Part Has The Jens Schmidt_Algorithm.
void DFS_DirectEdges(int Node , int NumberOfNodes , int Parent = -1)
{
    State[Node] = 1;
    DFS_Order.push_back(Node);
    for(auto NewNode : adj[Node])
    {
        if(State[NewNode] == 2)
        {
            continue;
        }
        if(State[NewNode] && NewNode != Parent)
        {
            BackEdge_adj[NewNode].push_back(Node);
        }
        if(State[NewNode] == 0)
        {
            TreeEdge_adj[NewNode].push_back(Node);
            DFS_DirectEdges(NewNode , NumberOfNodes , Node);
        }
    }
    State[Node] = 2;
}
void DFS_Chain(int Node , int NumberOfNodes , vector<int> &Chain)
{
    State[Node] = 1;
    for(auto NewNode : TreeEdge_adj[Node])
    {
        if(State[NewNode] == 2) continue;
        Chain.push_back(NewNode);
        if(!State[NewNode]) DFS_Chain(NewNode , NumberOfNodes , Chain);
    }
    State[Node] = 2;
}
void JensSchmidt_Algorithm(int NumberOfNodes , int NumberOfEdges)
{
    DFS_DirectEdges(1 , NumberOfEdges);
    DFS_INIT();
    vector<vector<int>> AllChains;
    int AllEdges = 0;
    for(auto Node : DFS_Order)
    {
        vector<int> CurrChain;
        for(auto NewNode : BackEdge_adj[Node])
        {
            CurrChain.push_back(Node);
            CurrChain.push_back(NewNode);
            if(State[NewNode] == 0) State[Node] = 1 , DFS_Chain(NewNode , NumberOfNodes , CurrChain) , State[Node] = 2;
            AllEdges += CurrChain.size() - 1;
            AllChains.push_back(CurrChain);
            CurrChain.clear();
        }
    }
    if(AllEdges < NumberOfEdges)
    {
        cout << "The Graph Is Neither Two-Edge-Connected Nor Two-Connected." << endl;
        return;
    }
    for(int i = 1; i < AllChains.size(); i++)
    {
        int n = AllChains[i].size();
        bool IsCycle =  (AllChains[i][0] == AllChains[i][n - 1]);
        if(IsCycle)
        {
            cout << "The Graph Is Two-Edge-Connected But Not Two-Connected." << endl;
            return;
        }
    }
    cout << "The Graph Is Two-Edge-Connected And Two-Connected." << endl;
    return;

}
int main()
{
    cout << "Enter the number of nodes and edges of the graph : ";
    int Nodes , EdgesNum;
    cin >> Nodes >> EdgesNum;
    bool Is_Directed = 1;
    cout << "If The Graph is Directed Press 1 , Otherwise press 0 : ";
    cin >> Is_Directed;
    cout << "\nNow enter the edges : \n";
    for(int i = 0; i < EdgesNum; i++)
    {
        int Node1 , Node2;
        cin >> Node1 >> Node2;
        if(Is_Directed)
        {
            adj[Node1].push_back(Node2);
            Reversed_adj[Node2].push_back(Node1);
        }
        else
        {
            adj[Node1].push_back(Node2);
            adj[Node2].push_back(Node1);
        }
    }
    if(Is_Directed)
    {
        DFS_INIT();
        bool IsSCC = SCC_Check(Nodes);
        cout << "The Given Graph Is" <<(!IsSCC ? " Not" : "")<< " SCC." << endl;
        DFS_INIT();
        cout << "Applying Melhlorn Algorithm..." << endl;
        for(int i = 1; i <= Nodes; i++)
        {
            if(State[i] == 0)
            Melhorn(i , Nodes);
        }
        cout << "Algorithm Applied :-D" << endl;
        for(int i = 1; i <= Nodes; i++)
        {
            cout <<"Node :" << i << " Representative : " << Comp[i] << endl;
        }
    }
    else
    {
        DFS_INIT();
        JensSchmidt_Algorithm(Nodes , EdgesNum);
    }
    return 0;
}
