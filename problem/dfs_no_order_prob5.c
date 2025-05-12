// Lecture slide chapter 16.
// Blog: ** URL here **
// Book: “Algorithm analysis in C++” by [저자 이름]

#include <stdio.h>
#include <stdlib.h>

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
        // 알파벳순으로 수정예정
        graph->numVertices++;
        for (int i = graph->numVertices - 1; i >= 0; i--) {
            if (graph->nodeLabels[i - 1] > label) {
                graph->nodeLabels[i] = graph->nodeLabels[i - 1];
            } else {
                graph->nodeLabels[i] = label;
                break;
            }
        }

        // graph->nodeLabels[graph->numVertices++] = label;
        // graph->numVertices++;
    } else {
        printf("Maximum number of nodes reached.\n");
    }
}

// Function to find the index of a vertex by its label
int getVertexIndex(Graph* graph, char label) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->nodeLabels[i] == label) {
            // printf("index is %d\n", i);
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
    // 알파벳 순?
    // 소스 라벨의 인덱스로 가서 넥스트의 라벨과 현재 라벨을 비교
    // 현재 라벨이 더 작으면 뉴 노드의 넥스트에 넥스트 노드를 넣기
    // 현재 노드의 라벨이 나보다 작고 현재의 넥스트의 라벨이 나보다 크면 거기에 연결결
    // 그 다음에 직전 노드의 넥스트에 내 노드를 연결.

    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    // AdjListNode* present = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->label = destLabel;
    if (graph->adj[srcIndex] == 0 || graph->adj[srcIndex]->label > destLabel) {
        newNode->next = graph->adj[srcIndex];
        graph->adj[srcIndex] = newNode;
        return;
    }
    AdjListNode* present = graph->adj[srcIndex];
    while (1) {
        if (present->label == destLabel) {
            printf("\n!! Error: Duplicate edge detected. (%c -> %c) !!\n\n", srcLabel, destLabel);
            return;
        } else if (present->label < destLabel && (present->next == 0 || present->next->label > destLabel)) {
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
    // Write code here
    color[u] = GRAY;
    *time++;
    d[u] = *time;
    // 이 노드와 연결된 모든 노드 방문
    // 그 노드의 인덱스 알아내기기
    // for (int i = 0; i < graph->adj[i] != NULL; i++) {
    // }
    color[u] = BLACK;
    *time++;
    f[u] = *time;
}

// Depth First Search function
void DFS(Graph* graph, COLOR color[], int* time, int d[], int f[]) {
    // Write code here
    for (int i = 0; i < graph->numVertices; i++) {
        color[i] = WHITE;
    }
    *time = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (color[i] = WHITE) {
            DFS_VISIT(graph, i, color, time, d, f);
        }
    }
}

int main() {
    int size;
    Graph* graph = createGraph();
    printf("Enter the number of nodes: ");
    scanf(" %d", &size);

    // Add nodes
    // for (int i = 0; i < size; i++) {
    //     addVertex(graph, 'A' + i);
    // }

    addVertex(graph, 'B');
    addVertex(graph, 'C');
    addVertex(graph, 'A');
    addVertex(graph, 'I');
    addVertex(graph, 'G');
    addVertex(graph, 'H');
    addVertex(graph, 'D');
    addVertex(graph, 'F');
    addVertex(graph, 'E');

    addEdge(graph, 'A', 'H');
    addEdge(graph, 'B', 'F');
    addEdge(graph, 'B', 'A');
    addEdge(graph, 'B', 'D');
    addEdge(graph, 'C', 'D');
    addEdge(graph, 'C', 'F');
    addEdge(graph, 'D', 'G');
    addEdge(graph, 'D', 'I');
    addEdge(graph, 'D', 'H');
    addEdge(graph, 'D', 'A');
    addEdge(graph, 'E', 'I');
    addEdge(graph, 'B', 'A');

    printGraph(graph);
    printf("\n");

    COLOR color[MAX_NODES];
    int time = -1;
    int d[MAX_NODES];  // Discovery times
    int f[MAX_NODES];  // Finishing times

    // DFS(graph, color, &time, d, f);

    /*
    printf("\nDFS Results:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %c: Discovery Time = %d, Finishing Time = %d, Color = ",
               graph->nodeLabels[i], d[i], f[i]);
        if (color[i] == WHITE) printf("WHITE");
        else if (color[i] == GRAY) printf("GRAY");
        else if (color[i] == BLACK) printf("BLACK");
        printf("\n");
    }
    */

    freeGraph(graph);

    return 0;
}
