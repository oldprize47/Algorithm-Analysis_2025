/*
(1) Lecture:
    Data structures Chapter 5 Slide - Trees.Part2(Max Heap) Page 35~48
(2)Blog:
    All about how to handle strings for Case 2
    https://kyu9341.github.io/C-C/2020/01/17/C++getline()/
    https://blockdmask.tistory.com/307
    https://popawaw.tistory.com/45
*/

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

#define MAX_SIZE 30

typedef struct{
    int score;
    string student_name;
    string class_name;
} Element;

class MaxHeap{
    private:
        Element heap[MAX_SIZE];
        int index; // the number of nodes

    public:
        MaxHeap() {index = 0;}
        ~MaxHeap() {index = 0;}
        bool IsEmpty() { return (index == 0);}
        bool IsFull() { return (index == MAX_SIZE-1);}
        int HeapIndex(){return index;}
        void swap(Element *a, Element *b){
            Element temp = *a;
            *a = *b;
            *b = temp;
        }
        void insert(Element item){
            if(IsFull()){
                cout << "The heap is Full" << endl;
                return;
            }

            int i = ++index;
            while(i > 1 && (heap[i/2].score < item.score) ){
                heap[i] = heap[i/2];
                i /= 2;
            }
            heap[i] = item;
        }

        Element EXTRACT_MAX(){
            int parent,larger;
            Element item = heap[1];
            heap[1] = heap[index];
            parent = 1;
            while(index >= parent*2){
                if(index > parent*2){
                    if(heap[parent*2].score > heap[parent * 2 + 1].score){
                        larger = parent*2;
                    }
                    else{
                        larger = parent*2 + 1;
                    };
                }else{
                    larger = parent*2;
                }
                if(heap[parent].score >= heap[larger].score) break;

                swap(&heap[parent], &heap[larger]);
                parent = larger;
            }
            index--;
            return item;
        }

        void MAXIMUM(){
            cout << "Element with the largest key: [" << heap[1].student_name << ", " <<  heap[1].score << ", " << heap[1].class_name << "]" << endl;
        }

        int INCREASE_KEY(int idx, int score){
            // Check if it is higher than the original score
            if(score <= heap[idx].score){
                return -1;
            }

            heap[idx].score = score;
            cout << "Key updated. [" << heap[idx].student_name << ", " <<  heap[idx].score << ", " << heap[idx].class_name << "] has been repositioned in the queue." << endl;

            // Reposition
            while(idx > 1 && (heap[idx/2].score < score) ){\
                swap(&heap[idx], &heap[idx/2]);
                idx /= 2;
            }

            // Finishing successfully
            return 1;
        }

        void PrintHeap(){
            cout << "Current queue elements:" << endl;
            for(int i = 1; i <= index; i++){
                cout << i << ".[" << heap[i].student_name << ", " <<  heap[i].score << ", " << heap[i].class_name << "]" << endl;
            }
                
        }
};

char showMenu();

int main(){
    MaxHeap heap;
    Element item;
    string c_score;
    int error_flag = 0;
    int index = 0, score = 0;
    int sum = 0;
    while(1){
        char menu = showMenu(); 

        switch(menu){
            case 'I' :
            case 'i' :
                // Get name
                cout << "Enter the name of the student: ";
                getline(cin, item.student_name);

                // Get score
                while(1){
                    cout << "Enter the score of the element: ";
                    getline(cin, c_score);

                    // Check for 3 digits or less
                    if(c_score.length() > 3 || c_score.length() < 0){
                        cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                        continue;
                    }

                    // Converting characters to integers
                    sum = 0;
                    for(int i = 0; i < c_score.length(); i++){
                        int num = c_score[i] - '0';
                        if(num < 0 || num > 9) error_flag = 1; // Check non-integer characters
                        sum += num * pow(10,(c_score.length() - 1) - i);
                    }

                    // Final Confirmation Filter
                    if(error_flag){
                        cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                        error_flag = 0;
                        continue;
                    }else if(sum <= 100 && sum >= 0){
                        item.score = sum;
                        break;
                    }
                    cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                }

                // Get class name
                cout << "Enter the class name: ";
                getline(cin, item.class_name);

                // Insert an item into the heap
                heap.insert(item);
                cout << "New element [" << item.student_name << ", " <<  item.score << ", " << item.class_name << "] has been inserted." << endl;
                break;

            case 'd' :
            case 'D' :
                if(heap.IsEmpty()){
                    cout << "Cannot delete from an empty queue." << endl;
                    break;
                }
                item = heap.EXTRACT_MAX();
                cout << "Deleted element: [" << item.student_name << ", " <<  item.score << ", " << item.class_name << "]" << endl;
                break;

            case 'r' :
            case 'R' :
                if(heap.IsEmpty()){
                    cout << "Queue is empty." << endl;
                    break;
                }
                heap.MAXIMUM();
                break;

            case 'n' :
            case 'N' :
                if(heap.IsEmpty()){
                    cout << "Queue is empty." << endl;
                    break;
                }
                
                while(1){
                    cout << "Enter the index of the element: ";
                    cin >> index;
                    cin.ignore();

                    // Avoid excess index
                    if(index > heap.HeapIndex()){
                        cout << "The current index cannot be exceeded. Please enter it again (Current Largest Index: " << heap.HeapIndex() << ")" << endl;
                    }else{
                        break;
                    }
                }

                while(1){
                    cout << "Enter the new score: ";
                    cin >> score;
                    cin.ignore();
                    if(score > 100 && score < 0){
                        cout << "Invalid score. Please enter a valid integer between 0 and 100." << endl;
                        continue;
                    }
                    if (heap.INCREASE_KEY(index, score) > 0){ // Check if it is higher than the original score
                        break;
                    }
                    cout << "New score should be larger than current score. Please enter again." << endl;
                }
                break;

            case 'p' :
            case 'P' :
                if(heap.IsEmpty()){
                    cout << "Queue is empty." << endl;
                    break;
                }
                heap.PrintHeap();
                break;            

            case 'q' :
            case 'Q' :
                return 0;

            default :
                cout << endl;
                cout << " Invalname input. Please enter it again" << endl;    // If the input is not in the menu, it will be re-entered.
        }
    }
    return 0;
}

char showMenu(){
    char menu;
    cout << endl;
    cout << "*********** MENU ****************" << endl;
    cout << "I : Insert a new element into the queue." << endl;
    cout << "D : Delete the element with thelargest keyfrom the queue." << endl;
    cout << "R : Retrieve the element with the largest key." << endl;
    cout << "N : Increase the key of an element in the queue." << endl;
    cout << "P : Print all elements in the queue." << endl;
    cout << "Q : Quit." << endl;
    cout << endl;
    cout << "Choose menu : ";

    cin >> menu;
    cin.ignore();
    return menu;
}