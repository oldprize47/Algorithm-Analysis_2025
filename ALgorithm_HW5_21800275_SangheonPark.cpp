/*
Lecture slide chapter 22 - page 52.
*/

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

#define MAX_NODES 10  // Maximum number of nodes

// Enum for vertex colors during DFS
typedef enum {
    WHITE,  // Not visited
    GRAY,   // Currently in recursion stack
    BLACK   // Finished processing
} COLOR;

// Structure for a node in the adjacency list
typedef struct AdjListNode {
    char label;
    struct AdjListNode* next;
} AdjListNode;

// Structure for the graph
typedef struct Graph {
    char nodeLabels[MAX_NODES];   // Array to store node labels ('A', 'B', 'C' ...)
    AdjListNode* adj[MAX_NODES];  // Adjacency list head pointer array
    int numVertices;
} Graph;

// Function to create a new graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < MAX_NODES; i++) {
        graph->adj[i] = NULL;
        graph->nodeLabels[i] = '\0';  // Initialize label array
    }
    graph->numVertices = 0;
    return graph;
}

// Function to add a vertex to the graph
void addVertex(Graph* graph, char label) {
    if (graph->numVertices < MAX_NODES) {
        // Check if the label already exists
        for (int i = 0; i < graph->numVertices; i++) {
            if (graph->nodeLabels[i] == label) {
                printf("Vertex %c already exists.\n", label);
                return;
            }
        }
        // Insert vertex in alphabetical order
        int i = 0;
        for (i = graph->numVertices - 1; i >= 0 && graph->nodeLabels[i] > label; i--)
            graph->nodeLabels[i] = graph->nodeLabels[i - 1];
        graph->nodeLabels[i + 1] = label;
        graph->numVertices++;
    } else {
        printf("Maximum number of nodes reached.\n");
    }
}

// Function to find the index of a vertex by its label
int getVertexIndex(Graph* graph, char label) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->nodeLabels[i] == label) {
            return i;
        }
    }
    return -1;  // Not found
}

// Function to add an edge
void addEdge(Graph* graph, char srcLabel, char destLabel) {
    int srcIndex = getVertexIndex(graph, srcLabel);
    int destIndex = getVertexIndex(graph, destLabel);

    if (srcIndex == -1 || destIndex == -1) {
        printf("Invalid vertex label.\n");
        return;
    }
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->label = destLabel;
    // Insert in order
    if (graph->adj[srcIndex] == 0 || graph->adj[srcIndex]->label > destLabel) {  // Insert at head if it's alphabetically first
        newNode->next = graph->adj[srcIndex];
        graph->adj[srcIndex] = newNode;
        return;
    }
    AdjListNode* present = graph->adj[srcIndex];
    while (1) {
        if (present->label == destLabel) {
            printf("\n!! Error: Duplicate edge detected. (%c -> %c) !!\n\n", srcLabel, destLabel);
            return;
        } else if (present->label < destLabel && (present->next == 0 || present->next->label > destLabel)) {  // Insert in the middle in sorted order
            newNode->next = present->next;
            present->next = newNode;
            return;
        }
        present = present->next;
    }
}

// Function to free the memory allocated for the graph
void freeGraph(Graph* graph) {
    for (int i = 0; i < MAX_NODES; i++) {
        AdjListNode* current = graph->adj[i];
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph);
}

// Function to print the graph (adjacency list representation)
void printGraph(Graph* graph) {
    printf("Adjacency List:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%c: ", graph->nodeLabels[i]);
        AdjListNode* current = graph->adj[i];
        while (current != NULL) {
            printf("%c -> ", current->label);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Depth First Search Visit function
void DFS_VISIT(Graph* graph, int u, COLOR color[], int* time, int d[], int f[]) {
    int v = 0;
    color[u] = GRAY;
    d[u] = ++(*time);  // Record discovery time
    AdjListNode* current = graph->adj[u];
    while (current != NULL) {
        v = getVertexIndex(graph, current->label);
        if (color[v] == WHITE)
            DFS_VISIT(graph, v, color, time, d, f);
        current = current->next;
    }
    color[u] = BLACK;  // Mark as finished
    f[u] = ++(*time);  // Record finishing time
}

// Depth First Search function
void DFS(Graph* graph, COLOR color[], int* time, int d[], int f[]) {
    // Initialize all nodes to unvisited
    for (int i = 0; i < graph->numVertices; i++) {
        color[i] = WHITE;
    }
    *time = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (color[i] == WHITE) {
            DFS_VISIT(graph, i, color, time, d, f);
        }
    }
}

void QuickSort(int list[], int idx[], int left, int right) {
    // Descending version
    int pivot, i, j;
    if (left < right) {
        i = left;
        j = right + 1;
        pivot = list[left];
        do {
            do i++;
            while (i <= right && list[i] > pivot);
            do j--;
            while (j > left && list[j] < pivot);
            if (i < j) {  // Swap if i < j
                swap(list[i], list[j]);
                swap(idx[i], idx[j]);
            }

        } while (i < j);  // Exit loop when i < j
        swap(list[left], list[j]);  // Swap pivot with list[j]
        swap(idx[left], idx[j]);

        QuickSort(list, idx, left, j - 1);
        QuickSort(list, idx, j + 1, right);
    }
}

int main() {
    int idx[MAX_NODES] = {0};
    int size;
    Graph* graph = createGraph();
    printf("Enter the number of nodes: ");
    scanf(" %d", &size);

    // Add nodes
    for (int i = 0; i < size; i++) {
        addVertex(graph, 'A' + i);
        idx[i] = i;
    }

    // HW 5
    addEdge(graph, 'A', 'F');
    addEdge(graph, 'A', 'C');
    addEdge(graph, 'A', 'B');
    addEdge(graph, 'B', 'D');
    addEdge(graph, 'B', 'C');
    addEdge(graph, 'D', 'A');
    addEdge(graph, 'D', 'C');
    addEdge(graph, 'E', 'G');
    addEdge(graph, 'E', 'C');
    addEdge(graph, 'F', 'A');
    addEdge(graph, 'F', 'C');
    addEdge(graph, 'G', 'E');
    addEdge(graph, 'G', 'D');

    // Week 10 - Exercise 1
    //  addEdge(graph, 'A', 'H');
    //  addEdge(graph, 'B', 'A');
    //  addEdge(graph, 'B', 'D');
    //  addEdge(graph, 'B', 'F');
    //  addEdge(graph, 'C', 'F');
    //  addEdge(graph, 'C', 'D');
    //  addEdge(graph, 'D', 'A');
    //  addEdge(graph, 'D', 'H');
    //  addEdge(graph, 'D', 'I');
    //  addEdge(graph, 'D', 'G');
    //  addEdge(graph, 'E', 'I');

    printGraph(graph);
    printf("\n");

    COLOR color[MAX_NODES];
    int time = 0;
    int d[MAX_NODES];  // Discovery times
    int f[MAX_NODES];  // Finishing times

    DFS(graph, color, &time, d, f);

    printf("\nDFS Results:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %c: Discovery Time = %d, Finishing Time = %d, Color = ", graph->nodeLabels[i], d[i], f[i]);
        if (color[i] == WHITE)
            printf("WHITE");
        else if (color[i] == GRAY)
            printf("GRAY");
        else if (color[i] == BLACK)
            printf("BLACK");
        printf("\n");
    }

    QuickSort(f, idx, 0, graph->numVertices - 1);
    cout << endl;
    cout << "Topological Sort Result: " << endl;
    cout << graph->nodeLabels[idx[0]];
    for (int i = 1; i < graph->numVertices; i++) {
        cout << " -> " << graph->nodeLabels[idx[i]];
    }
    freeGraph(graph);  // Free all dynamically allocated memory

    return 0;
}
