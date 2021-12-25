# Algorithms C#

```c#
using System;
using System.Collections.Generic;

namespace CSharpDataStructuresAlgorithms
{
    class TestMergeQuickSort
    {
        public static void MergeSort(int[] array)
        {
            MergeSort(array, new int[array.Length], 0, array.Length-1 );
        }

        private static void MergeSort(int[] array, int[] temp, int left, int right)
        {
            if (left < right)
            {
                int center = left + (right - left) / 2;
                MergeSort(array, temp, left, center);
                MergeSort(array, temp, center + 1, right);
                Merge(array, temp, left, center + 1, right);
            }
        }

        private static void Merge(int[] array, int[] temp, int left, int right, int rightEndIndex)
        {
            int leftEndIndex = right - 1;
            int tempIndex = left;
            int elementNumber = rightEndIndex - left + 1;
            while (left <= leftEndIndex && right <= rightEndIndex)
            {
                if (array[left] <= array[right])
                    temp[tempIndex++] = array[left++];
                else
                    temp[tempIndex++] = array[right++];
            }

            while (left <= leftEndIndex)
            {
                temp[tempIndex++] = array[left++];
            }

            while (right <= rightEndIndex)
            {
                temp[tempIndex++] = array[right++];
            }

            for (int i = 0; i < elementNumber; i++)
            {
                array[rightEndIndex] = temp[rightEndIndex];
                rightEndIndex--;
            }
        }

        public static void QuickSort(int[] array)
        {
            if (array.Length > 0)
                QuickSort(array, 0, array.Length - 1);
        }

        private static void QuickSort(int[] array, int low, int high)
        {
            if (low > high)
                return;
            int i = low;
            int j = high;
            int threshold = array[low];
            while (i < j)
            {
                while (i < j && array[j] > threshold)
                    j--;
                if (i < j)
                    array[i++] = array[j];
                while (i < j && array[i] <= threshold)
                    i++;
                if (i < j)
                    array[j--] = array[i];
            }

            array[i] = threshold;
            QuickSort(array, low, i - 1);
            QuickSort(array, i + 1, high);
        }
        public static void Main(string[] args)
        {
            int[] scores = {50, 65, 99, 87, 74, 63, 76, 100, 92};
            //MergeSort(scores);
            QuickSort(scores);
            foreach (var score in scores)
            {
                Console.Write(score + ", ");
            }
        }
    }
}
```

Binary Tree

```c#
using System;

namespace CSharpDSA
{
    public class Node
    {
        public int data;
        public Node left;
        public Node right;

        public Node(int data, Node left, Node right)
        {
            this.data = data;
            this.left = left;
            this.right = right;
        }

        public Node(int data)
        {
            this.data = data;
            this.left = null;
            this.right = null;
        }
    }

    class BinaryTree
    {
        private Node root;
        public Node GetRoot()
        {
            return root;
        }
        public void InOrder(Node root)
        {
            if (root == null)
                return;
            InOrder(root.left);
            Console.Write(root.data + ", ");
            InOrder(root.right);
        }

        public void PreOrder(Node root)
        {
            if (root == null)
                return;
            Console.Write(root.data + ", ");
            PreOrder(root.left);
            PreOrder(root.right);
        }

        public void PostOrder(Node root)
        {
            if (root == null)
                return;
            PostOrder(root.left);
            PostOrder(root.right);
            Console.Write(root.data + ", ");
        }

        public void Insert(Node node, int newData)
        {
            if (this.root == null)
            {
                this.root = new Node(newData);
                return;
            }

            int compareValue = newData - node.data;
            if (compareValue < 0)
            {
                if (node.left == null)
                    node.left = new Node(newData);
                else
                {
                    Insert(node.left, newData);
                }
            } else if (compareValue > 0)
            {
                if (node.right == null)
                    node.right = new Node(newData);
                else
                {
                    Insert(node.right, newData);
                }
            }
        }

        public Node SearchMinValue(Node node)
        {
            if (node == null || node.data == 0)
                return null;
            return node.left == null ? node : SearchMinValue(node.left);
        }

        public Node SearchMaxValue(Node node)
        {
            if (node == null || node.data == 0)
                return null;
            return node.right == null ? node : SearchMaxValue(node.right);
        }

        public Node Remove(Node node, int newData)
        {
            if (node == null)
                return node;
            int compareValue = newData - node.data;
            if (compareValue > 0)
            {
                node.right = Remove(node.right, newData);
            } else if (compareValue < 0)
            {
                node.left = Remove(node.left, newData);
            } else if (node.left != null && node.right != null)
            {
                node.data = SearchMinValue(node.right).data;
                node.right = Remove(node.right, node.data);
            }
            else
            {
                node = (node.left != null) ? node.left : node.right;
            }

            return node;
        }
    }

    class TestBinaryTree
    {
        public static void Main(string[] args)
        {
            BinaryTree binaryTree = new BinaryTree();
            binaryTree.Insert(binaryTree.GetRoot(), 60);
            binaryTree.Insert(binaryTree.GetRoot(), 40);
            binaryTree.Insert(binaryTree.GetRoot(), 20);
            binaryTree.Insert(binaryTree.GetRoot(), 10);
            binaryTree.Insert(binaryTree.GetRoot(), 30);
            binaryTree.Insert(binaryTree.GetRoot(), 50);
            binaryTree.Insert(binaryTree.GetRoot(), 80);
            binaryTree.Insert(binaryTree.GetRoot(), 70);
            binaryTree.Insert(binaryTree.GetRoot(), 90);

            //Console.WriteLine("In-order traversal binary search tree: left->root->right");
            //binaryTree.InOrder(binaryTree.GetRoot());
            //Console.WriteLine("Pre-order traversal binary search tree: root->left->right");
            //binaryTree.PreOrder(binaryTree.GetRoot());
            Console.WriteLine("Post-order traversal binary search tree: left->right->root");
            binaryTree.PostOrder(binaryTree.GetRoot());

            Console.WriteLine("\nMinimum value: ");
            Console.WriteLine(binaryTree.SearchMinValue(binaryTree.GetRoot()).data);
            Console.WriteLine("Maximum value: ");
            Console.WriteLine(binaryTree.SearchMaxValue(binaryTree.GetRoot()).data);

            binaryTree.Remove(binaryTree.GetRoot(), 40);
            binaryTree.InOrder(binaryTree.GetRoot());
        }
    }
}
```

Heap

```c#
using System;

namespace CSharpDSA
{
    class HeapSort
    {
        private int[] array;

        public void CreateHeap(int[] array)
        {
            this.array = array;
            for (int i = (array.Length - 1) / 2; i >= 0; i--)
            {
                AdjustHeap(i, array.Length - 1);
            }
        }

        public void AdjustHeap(int currentIndex, int maxLength)
        {
            int noLeafValue = array[currentIndex];
            for (int j = 2 * currentIndex + 1; j <= maxLength; j = currentIndex * 2 + 1)
            {
                if (j < maxLength && array[j] < array[j + 1])
                {
                    j++;
                }

                if (noLeafValue >= array[j])
                {
                    break;
                }

                array[currentIndex] = array[j];
                currentIndex = j;
            }

            array[currentIndex] = noLeafValue;
        }

        public void SortHeap()
        {
            for (int i = array.Length - 1; i > 0; i--)
            {
                int temp = array[0];
                array[0] = array[i];
                array[i] = temp;
                AdjustHeap(0, i - 1);
            }
        }
    }

    class TestHeapSort
    {
        public static void Main(string[] args)
        {
            HeapSort heapSort = new HeapSort();
            int[] scores = {10, 90, 20, 80, 30, 70, 40, 60, 50};
            Console.WriteLine("Before building a heap: ");
            for (int i = 0; i < scores.Length; i++)
                Console.Write(scores[i] + ", ");
            Console.Write("\n\n");

            heapSort.CreateHeap(scores);
            foreach (var t in scores)
                Console.Write(t + ", ");
            Console.Write("\n\n");

            Console.WriteLine("After heap sorting: ");
            heapSort.SortHeap();
            foreach (var t in scores)
                Console.Write(t + ", ");
            Console.Write("\n\n");
        }
    }
}
```

Hash table

```c#
using System;

namespace CSharpDSA
{
    public class Node
    {
        public Object key;
        public Object value;
        public int hash;
        public Node next;

        public Node(Object key, Object value, int hash, Node next)
        {
            this.key = key;
            this.value = value;
            this.hash = hash;
            this.next = next;
        }
    }

    class Hashtable
    {
        private Node[] table;
        private int capacity = 16;
        private int size;

        public Hashtable()
        {
            table = new Node[capacity];
        }

        public Hashtable(int capacity)
        {
            table = new Node[capacity];
            size = 0;
            this.capacity = capacity;
        }

        private int HashCode(Object key)
        {
            int num = 0;
            char[] chs = key.ToString().ToCharArray();
            for (int i = 0; i < chs.Length; i++)
                num += (int) chs[i];
            double avg = num * (Math.Pow(5, 0.5) - 1) / 2;
            double numeric = avg - Math.Floor(avg);
            return (int) Math.Floor(numeric * capacity);
        }

        public int Size()
        {
            return size;
        }

        public bool IsEmpty()
        {
            return size == 0 ? true : false;
        }

        public void Put(Object key, Object value)
        {
            int hash = HashCode(key);
            Node newNode = new Node(key, value, hash, null);
            Node Node = table[hash];
            while (Node != null)
            {
                if (Node.key.Equals(key))
                {
                    Node.value = value;
                    return;
                }

                Node = Node.next;
            }

            newNode.next = table[hash];
            table[hash] = newNode;
            size++;
        }

        public Object Get(Object key)
        {
            if (key == null)
            {
                return null;
            }

            int hash = HashCode(key);
            Node Node = table[hash];
            while (Node != null)
            {
                if (Node.key.Equals(key))
                {
                    return Node.value;
                }

                Node = Node.next;
            }

            return null;
        }
    }

    class TestHashtable
    {
        public static void Main(string[] args)
        {
            Hashtable table = new Hashtable();
            table.Put("david", "Good Boy Keep Going");
            table.Put("grace", "Cute Girl Keep Going");

            Console.WriteLine("david=>" + table.Get("david"));
            Console.WriteLine("grace=>" + table.Get("grace"));
        }
    }
}
```

Directed Graph and Depth-first Search

```c#
using System;
using System.Collections.Generic;

namespace CSharpDSA
{
    public class Vertex
    {
        public string data;
        public bool visited;

        public Vertex(string data, bool visited)
        {
            this.data = data;
            this.visited = visited;
        }
    }

    public class Graph
    {
        private int maxVertexSzie;  // 2D array size
        private int size;           // current vertex size
        private Vertex[] vertices;
        private int[,] adjacencyMatrix;
        private Stack<Int32> stack; // stack saves current vertices

        public Graph(int maxVertexSzie)
        {
            this.maxVertexSzie = maxVertexSzie;
            vertices = new Vertex[maxVertexSzie];
            adjacencyMatrix = new int[maxVertexSzie, maxVertexSzie];
            stack = new Stack<Int32>();
        }

        public void AddVertex(string data)
        {
            Vertex vertex = new Vertex(data, false);
            vertices[size] = vertex;
            size++;
        }

        public void AddEdge(int from, int to)
        {
            adjacencyMatrix[from, to] = 1;
        }

        public void DepthFirstSearch()
        {
            Vertex firstVertex = vertices[0];
            firstVertex.visited = true;
            Console.Write(firstVertex.data);
            stack.Push(0);

            while (stack.Count > 0)
            {
                int row = stack.Peek(); // returns object at the top of the stack<T> without removing it
                int col = FindAdjacencyUnVisitedVertex(row);
                if (col == -1)
                    stack.Pop();
                else
                {
                    vertices[col].visited = true;
                    Console.Write("->" + vertices[col].data);
                    stack.Push(col);
                }
            }

            Clear();
        }

        public int FindAdjacencyUnVisitedVertex(int row)
        {
            for (int col = 0; col < size; col++)
            {
                if (adjacencyMatrix[row, col] == 1 && !vertices[col].visited)
                    return col;
            }

            return -1;
        }

        public void Clear()
        {
            for (int i = 0; i < size; i++)
                vertices[i].visited = false;
        }

        public int[,] GetAdjacencyMatrix()
        {
            return adjacencyMatrix;
        }

        public Vertex[] GetVertices()
        {
            return vertices;
        }
    }

    public class TestGraph
    {
        public static void Main(string[] args)
        {
            Graph graph = new Graph(5);
            graph.AddVertex("A");
            graph.AddVertex("B");
            graph.AddVertex("C");
            graph.AddVertex("D");
            graph.AddVertex("E");

            graph.AddEdge(0, 1);
            graph.AddEdge(0, 2);
            graph.AddEdge(0, 3);
            graph.AddEdge(1, 2);
            graph.AddEdge(1, 3);
            graph.AddEdge(2, 3);
            graph.AddEdge(3, 4);

            PrintGraph(graph);
            Console.Write("\nDepth-first search traversal output: \n");
            graph.DepthFirstSearch();
        }

        public static void PrintGraph(Graph graph)
        {
            Console.Write("2D array traversal vertex edge and adjacent array: \n");
            for (int i = 0; i < graph.GetVertices().Length; i++)
            {
                Console.Write(graph.GetVertices()[i].data + " ");
            }

            Console.Write("\n");

            for (int i = 0; i < graph.GetAdjacencyMatrix().GetLength(0); i++)
            {
                Console.Write(graph.GetVertices()[i].data + " ");
                for (int j = 0; j < graph.GetAdjacencyMatrix().GetLength(0); j++)
                {
                    Console.Write(graph.GetAdjacencyMatrix()[i, j] + " ");
                }

                Console.Write("\n");
            }
        }
    }
}
```

Directed Graph and Breadth-first Search

```c#
using System;
using System.Collections.Generic;

namespace CSharpDSA
{
    public class Vertex
    {
        public string data;
        public bool visited;

        public Vertex(string data, bool visited)
        {
            this.data = data;
            this.visited = visited;
        }
    }

    public class Graph
    {
        private int maxVertexSzie;  // 2D array size
        private int size;           // current vertex size
        private Vertex[] vertices;
        private int[,] adjacencyMatrix;
        private Queue<Int32> queue;  // Queue saves current vertices

        public Graph(int maxVertexSzie)
        {
            this.maxVertexSzie = maxVertexSzie;
            vertices = new Vertex[maxVertexSzie];
            adjacencyMatrix = new int[maxVertexSzie, maxVertexSzie];
            queue = new Queue<Int32>();
        }

        public void AddVertex(string data)
        {
            Vertex vertex = new Vertex(data, false);
            vertices[size] = vertex;
            size++;
        }

        public void AddEdge(int from, int to)
        {
            adjacencyMatrix[from, to] = 1;
        }

        //public void DepthFirstSearch()
        //{
        //    Vertex firstVertex = vertices[0];
        //    firstVertex.visited = true;
        //    Console.Write(firstVertex.data);
        //    stack.Push(0);

        //    while (stack.Count > 0)
        //    {
        //        int row = stack.Peek(); // returns object at the top of the stack<T> without removing it
        //        int col = FindAdjacencyUnVisitedVertex(row);
        //        if (col == -1)
        //            stack.Pop();
        //        else
        //        {
        //            vertices[col].visited = true;
        //            Console.Write("->" + vertices[col].data);
        //            stack.Push(col);
        //        }
        //    }

        //    Clear();
        //}

        public void BreadthFirstSearch()
        {
            Vertex firstVertex = vertices[0];
            firstVertex.visited = true;
            Console.Write(firstVertex.data);
            queue.Enqueue(0);

            int col;
            while (queue.Count > 0)
            {
                int head = queue.Dequeue();
                col = FindAdjacencyUnVisitedVertex(head);
                while (col != -1)
                {
                    vertices[col].visited = true;
                    Console.Write("->" + vertices[col].data);
                    queue.Enqueue(col);
                    col = FindAdjacencyUnVisitedVertex(head);
                }
            }

            Clear();
        }

        public int FindAdjacencyUnVisitedVertex(int row)
        {
            for (int col = 0; col < size; col++)
            {
                if (adjacencyMatrix[row, col] == 1 && !vertices[col].visited)
                    return col;
            }

            return -1;
        }

        public void Clear()
        {
            for (int i = 0; i < size; i++)
                vertices[i].visited = false;
        }

        public int[,] GetAdjacencyMatrix()
        {
            return adjacencyMatrix;
        }

        public Vertex[] GetVertices()
        {
            return vertices;
        }
    }

    public class TestGraph
    {
        public static void Main(string[] args)
        {
            Graph graph = new Graph(5);
            graph.AddVertex("A");
            graph.AddVertex("B");
            graph.AddVertex("C");
            graph.AddVertex("D");
            graph.AddVertex("E");

            graph.AddEdge(0, 1);
            graph.AddEdge(0, 2);
            graph.AddEdge(0, 3);
            graph.AddEdge(1, 2);
            graph.AddEdge(1, 3);
            graph.AddEdge(2, 3);
            graph.AddEdge(3, 4);

            PrintGraph(graph);
            Console.Write("\nDepth-first search traversal output: \n");
            graph.BreadthFirstSearch();
        }

        public static void PrintGraph(Graph graph)
        {
            Console.Write("2D array traversal vertex edge and adjacent array: \n");
            for (int i = 0; i < graph.GetVertices().Length; i++)
            {
                Console.Write(graph.GetVertices()[i].data + " ");
            }

            Console.Write("\n");

            for (int i = 0; i < graph.GetAdjacencyMatrix().GetLength(0); i++)
            {
                Console.Write(graph.GetVertices()[i].data + " ");
                for (int j = 0; j < graph.GetAdjacencyMatrix().GetLength(0); j++)
                {
                    Console.Write(graph.GetAdjacencyMatrix()[i, j] + " ");
                }

                Console.Write("\n");
            }
        }
    }
}
```

Directed Graph Topological Sorting

```c#
using System;
using System.Collections.Generic;

namespace CSharpDSA
{
    public class Vertex
    {
        public string data;
        public bool visited;

        public Vertex(string data, bool visited)
        {
            this.data = data;
            this.visited = visited;
        }
    }

    public class Graph
    {
        private int maxVertexSzie;  // 2D array size
        private int size;           // current vertex size
        private Vertex[] vertices;
        private int[,] adjacencyMatrix;
        private Vertex[] topologys;
        public Graph(int maxVertexSzie)
        {
            this.maxVertexSzie = maxVertexSzie;
            vertices = new Vertex[maxVertexSzie];
            adjacencyMatrix = new int[maxVertexSzie, maxVertexSzie];
            topologys = new Vertex[maxVertexSzie];
        }

        public void AddVertex(string data)
        {
            Vertex vertex = new Vertex(data, false);
            vertices[size] = vertex;
            size++;
        }

        public void AddEdge(int from, int to)
        {
            adjacencyMatrix[from, to] = 1;
        }

        public void TopologySort()
        {
            while (size > 0)
            {
                int noSuccessorVertex = GetNoSuccessorVertex();
                if (noSuccessorVertex == -1)
                {
                    Console.Write("There is ring in Graph");
                    return;
                }

                topologys[size - 1] = vertices[noSuccessorVertex];
                RemoveVertex(noSuccessorVertex);
            }
        }

        public void RemoveVertex(int vertex)
        {
            if (vertex != size - 1)
            {
                for (int i = vertex; i < size - 1; i++)
                {
                    vertices[i] = vertices[i + 1];
                }

                for (int row = vertex; row < size - 1; row++)
                {
                    for (int col = 0; col < size - 1; col++)
                        adjacencyMatrix[row, col] = adjacencyMatrix[row + 1, col];
                }

                for (int col = vertex; col < size - 1; col++)
                {
                    for (int row = 0; row < size - 1; row++)
                        adjacencyMatrix[row, col] = adjacencyMatrix[row, col + 1];
                }
            }

            size--;
        }

        public int GetNoSuccessorVertex()
        {
            bool existSuccessor = false;
            for (int row = 0; row < size; row++)
            {
                existSuccessor = false;
                for (int col = 0; col < size; col++)
                {
                    if (adjacencyMatrix[row, col] == 1)
                    {
                        existSuccessor = true;
                        break;
                    }
                }

                if (!existSuccessor)
                {
                    return row;
                }
            }

            return -1;
        }

        public Vertex[] GetTopologys()
        {
            return topologys;
        }

        public int FindAdjacencyUnVisitedVertex(int row)
        {
            for (int col = 0; col < size; col++)
            {
                if (adjacencyMatrix[row, col] == 1 && !vertices[col].visited)
                    return col;
            }

            return -1;
        }

        public void Clear()
        {
            for (int i = 0; i < size; i++)
                vertices[i].visited = false;
        }

        public int[,] GetAdjacencyMatrix()
        {
            return adjacencyMatrix;
        }

        public Vertex[] GetVertices()
        {
            return vertices;
        }
    }

    public class TestGraph
    {
        public static void Main(string[] args)
        {
            Graph graph = new Graph(5);
            graph.AddVertex("A");
            graph.AddVertex("B");
            graph.AddVertex("C");
            graph.AddVertex("D");
            graph.AddVertex("E");

            graph.AddEdge(0, 1);
            graph.AddEdge(0, 2);
            graph.AddEdge(0, 3);
            graph.AddEdge(1, 2);
            graph.AddEdge(1, 3);
            graph.AddEdge(2, 3);
            graph.AddEdge(3, 4);

            PrintGraph(graph);
            Console.Write("\nDepth-first search traversal output: \n");
            Console.WriteLine("Directed Graph Topological Sorting:");
            graph.TopologySort();
            for (int i = 0; i < graph.GetTopologys().Length; i++)
            {
                Console.Write(graph.GetTopologys()[i].data + "->");
            }
        }

        public static void PrintGraph(Graph graph)
        {
            Console.Write("2D array traversal vertex edge and adjacent array: \n");
            for (int i = 0; i < graph.GetVertices().Length; i++)
            {
                Console.Write(graph.GetVertices()[i].data + " ");
            }

            Console.Write("\n");

            for (int i = 0; i < graph.GetAdjacencyMatrix().GetLength(0); i++)
            {
                Console.Write(graph.GetVertices()[i].data + " ");
                for (int j = 0; j < graph.GetAdjacencyMatrix().GetLength(0); j++)
                {
                    Console.Write(graph.GetAdjacencyMatrix()[i, j] + " ");
                }

                Console.Write("\n");
            }
        }
    }
}
```

Tower of Hanoi

```c#
using System;
using System.Collections.Generic;

namespace CSharpDSA
{
    class TowersOfHanoi
    {
        public static void Main(string[] args)
        {
            Console.WriteLine("Please enter the numebr of discs:");
            int n = Convert.ToInt32(Console.ReadLine());
            Hanoi(n, 'A', 'B', 'C');
        }
        public static void Hanoi(int n, char A, char B, char C)
        {
            if (n == 1)
                Console.WriteLine("Move " + n + " " + A + " to " + C);
            else
            {
                Hanoi(n - 1, A, C, B);  // move n-1 disc from A through C to B
                Console.WriteLine("Move " + n + " from " + A + " to " + C);
                Hanoi(n - 1, B, A, C);  // move n-1 disc from B through A to C
            }
        }
    }
}
```

