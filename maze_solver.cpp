// In this draft I am taking input of maze matrix from another text file. e.g., "input.txt" will store the matrix.
// However we need to give the coordinates of source and destination from the stdin only i.e., from the terminal.

#include<bits/stdc++.h>
#define MAX 10

using namespace std;

const char block_char = '#';
int row, col;

template <typename T>
class Graph {
	unordered_map<T, list<pair<T, int>>>m;

public:
	void addEdge(T u, T v, int dist, bool bidir = true)
	{
		m[u].push_back(make_pair(v, dist));
		if (bidir)
			m[v].push_back(make_pair(u, dist));
	}

	bool edgeAlreadyExist(T u, T v, int dist, bool bidir = true)
	{
		pair<T, int> p1 = make_pair(v, dist);
		pair<T, int> p2 = make_pair(u, dist);

		if (m[u].find(p1) != m[u].end())
		{
			if (bidir == true)
			{
				if (m[v].find(p2) != m[v].end())
					return true;
				else
					return false;
			}
			return true;
		}
		else
			return false;
	}

	void printAdj()
	{
		//let try to print the adj list
		//Iterate over all the key value pairs in the map
		for (auto j : m)
		{
			cout << j.first << " -> ";  // j.first is similar to 'i' in arr[i].
			//Iterate over the list of cities
			for (auto l : j.second) // j.second is similar to 'arr[i]' in arr[i].
			{
				cout << "(" << l.first << ", " << l.second << ")";
			}
			cout << endl;
		}
	}

	vector<T> dijkstraSSSP(T src, T destination)
	{
		//Check if src & destination both exists in graph or not
		vector<T>path;
		if (m.find(src) == m.end() or m.find(destination) == m.end())
		{
			cout << "\nNo path exists...\n\n";
			cout << "Either src or destination or both of them are blocked squares.\n";
			cout << "Distance between src and destination = INFINITE" << endl;
			return path;
		}

		unordered_map<T, T>parent;
		parent[src] = src;	//In case T is int.

		unordered_map<T, int>dist;	//It is shortest distance of every node from the src node
		//Set all distance to infinity
		for (auto j : m)
			dist[j.first] = INT_MAX;
		//Make a set to find a out node with the minimum distance
		set<pair<int, T>>s; // We made distance as the first member of set because we wanted to sort the pairs according to the distance.
		//Here set will work as a priority queue which keeps the shortest distance vertex at the top.
		dist[src] = 0;
		s.insert(make_pair(0, src));
		while (!s.empty())
		{
			//Find the pair at the front
			auto p = *(s.begin());
			T node = p.second;
			int nodeDist = p.first;
			s.erase(s.begin()); //Popping out the top of priority queue


			//Iterate over neighbours/children of the current node
			for (auto childPair : m[node])
			{
				if (nodeDist + childPair.second < dist[childPair.first])
				{
					//In the set updation of a particular is not possible
					//we have to remove the old pair and insert the new pair to simulation updation
					T dest = childPair.first;
					auto f = s.find(make_pair(dist[dest], dest));
					if (f != s.end())
						s.erase(f);
					//Insert the new pair
					dist[dest] = nodeDist + childPair.second;
					parent[dest] = node;
					s.insert(make_pair(dist[dest], dest));
				}
			}
		}
		//Lets print distance to all other node from src
		// for (auto d : dist)
		// {
		// 	cout << d.first << " is located at distance of " << d.second << endl;
		// }

		cout << endl;
		int dist_temp = dist[destination];
		cout << "Distance between src & dest = ";

		if (dist_temp == INT_MAX)
			cout << "INFINITE";
		else
			cout << dist_temp;

		cout << endl;


		//Print the path
		if (dist[destination] != INT_MAX)
		{
			stack<T>st;
			T node = destination;
			while (parent[node] != node)
			{
				st.push(node);
				node = parent[node];
			}
			st.push(src);

			cout << "Shortest Path: ";
			while (!st.empty())
			{
				cout << st.top() << " ";
				path.push_back(st.top());
				st.pop();
			}
			cout << endl;
		}
		else
			cout << "\nNo path exists...\n";

		return path;
	}
};

bool isBlocked(char mat[][MAX], int n, int m, int i, int j)
{
	if (i < 0 or j < 0)
		return true;

	if (mat[i][j] == block_char)
		return true;

	return false;
}

void printMat(char mat[][MAX], int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << mat[i][j];
		cout << endl;
	}
}

void printPath(char mat[][MAX], int n, int m, vector<int>path)
{
	int size = path.size();
	char solved_mat[MAX][MAX];

	//Copy mat[][] into solved_mat[][], so that original matrix is not changed.
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			solved_mat[i][j] = mat[i][j];

	for (int i = 0; i < size; i++)
	{
		int x = path[i] / m;
		int y = path[i] % m;
		solved_mat[x][y] = '.';	//Path will be highlighted by '.'
	}

	cout << "\n\n-----SOLVED MAZE:-----\n\n";
	printMat(solved_mat, n, m);
}

template<typename T>
void convertToGraph(char mat[][MAX], int n, int m, Graph<T>&g)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (isBlocked(mat, n, m, i, j) == true)
				continue;

			int vertex1 = i * m + j;
			//Check if square above and square left is not a block_char
			bool sq_above = isBlocked(mat, n, m, i - 1, j);
			bool sq_left = isBlocked(mat, n, m, i, j - 1);

			//In case there is no free square above or in the right of current square (i, j),
			//then we create a UNIDIRECTIONAL edge from current square (or current vertex) to vertex '-1'.
			if (sq_above == true and sq_left == true)
				g.addEdge(vertex1, -1, 1, false);

			if (sq_above == false)
			{
				int vertex2 = (i - 1) * m + j;
				g.addEdge(vertex1, vertex2, 1);
			}

			if (sq_left == false)
			{
				int vertex2 = i * m + j - 1;
				g.addEdge(vertex1, vertex2, 1);
			}
		}
	}

}

pair<int, int> inputMatrix(char mat[][MAX], string filename)
{
	FILE* input_file = fopen(filename.c_str(), "r");
	if (input_file == nullptr) {
		exit(1);
	}

	int i = 0, j = 0;
	char ch;

	int row = 0, col = 0;
	int flag = 0;
	while ((ch = fgetc(input_file)) != EOF)
	{
		if (ch == '\n')
		{
			if (flag == 1)
				i++;
			j = 0;
		}
		else
		{
			mat[i][j] = ch;
			flag = 1;
			row = max(row, i);
			col = max(col, j);
			j++;
		}


	}
	fclose(input_file);
	row++, col++;
	pair<int, int>p = make_pair(row, col);
	return p;
}

int main()
{
	char mat[MAX][MAX];
	int n, m;	//Dimensions of the maze: n x m

	// n = length in downward direction
	// m =  length in rightward direction

	string filename = "input.txt"; // This file will contain only the maze matrix
	// But we have to give the source and destination coordinates from the console only.
	pair<int, int>dimensions = inputMatrix(mat, filename);
	n = dimensions.first;
	m = dimensions.second;

	cout << "Dimensions of maze: n = " << n << ", m = " << m << endl;
	cout << "\nMaze Matrix: \n\n";
	printMat(mat, n, m);
	cout << endl;

	Graph<int>g;
	convertToGraph(mat, n, m, g);

	//g.printAdj();
	int ux, uy;
	cout << "Enter the coordinates of starting point: ";
	cin >> ux >> uy;

	int vx, vy;
	cout << "Enter the coordinates of destination point: ";
	cin >> vx >> vy;

	cout << endl;

	int src = ux * m + uy;
	int dest = vx * m + vy;

	cout << "src = " << src << ", dest = " << dest << endl;

	vector<int>path = g.dijkstraSSSP(src, dest);
	if (path.size() > 0)
		printPath(mat, n, m, path);
	return 0;
}
