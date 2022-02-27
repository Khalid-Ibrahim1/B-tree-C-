#include <iostream>
#include <bits/stdc++.h>

using namespace std;
int height = 0;
int numOfIt = 0;
int numOfKeys = 0;
class Node{
    public:
        Node(int _t, bool _leaf); // class constructor
        int *elements; // elements variable
        int t; // min degree variable
        Node **C; // array of child pointers
        int n; // number of elements in the node
        bool leaf; // to check if the node is leaf of not

        void traverse(){ // function to traverse the tree
            // there is n elements and n+1 children, traverse through n elements
            // and first n children
            int i = 0;
            while(i < n){
                // if this is not leaf then before printing elements[i],
                // traverse the subtree rooted with child C[i].
                if (leaf == false)
                    C[i]->traverse();
                cout << " " << elements[i];
                i++;
            }
            // print the subtree rooted with last child
            if(leaf == false)
                C[i]->traverse();
        }

        Node *search(int k){
            // Find the first key greater than or equal to k
            int i = 0;
            numOfKeys = n;
            while (i < n && k > elements[i])
                i++; numOfIt++;
        
            // If the found key is equal to k, return this node
            if (elements[i] == k){
                return this;
            }
            // If key is not found here and this is a leaf node
            if (leaf == true)
                return NULL;
                
            // Go to the appropriate child
            return C[i]->search(k);
        }

        void insertIfNotFull(int k){ // function to insert an element if the node is not full
            // initialize index as index of rightmost element
            int i = n-1;
            // if a leaf node
            if (leaf == true){
                // moves the element one place ahead to insert the new element
                while (i >= 0 && elements[i] > k){
                    elements[i+1] = elements[i];
                    i--;
                }
        
                // inserts the new element in the node
                elements[i+1] = k;
                n = n+1;
            }
            else{  // if not a leaf node
                // find the child to insert the new element into it
                while (i >= 0 && elements[i] > k)
                    i--;
        
                // check if the child is full?
                if (C[i+1]->n == 2*t-1){
                    // if full than spilt
                    split(i+1, C[i+1]);
        
                    // split in half and the middle element goes up
                    // check which half will have the new element
                    if (elements[i+1] < k)
                        i++;
                }
                C[i+1]->insertIfNotFull(k);
            }
        }

        void split(int i, Node *y){  
            // make a new node
            Node *z = new Node(y->t, y->leaf);
            z->n = t - 1;
            
            // move the last (t-1) elements of y to z
            int j = 0;
            while(j < t-1){
                z->elements[j] = y->elements[j+t];
                j++;
            }
            // move the last t children of y to z
            if (y->leaf == false){
                int j = 0;
                while(j < t){
                    z->C[j] = y->C[j+t];
                    j++;
                }
            }
        
            // reduce the number of elements in y
            y->n = t - 1;
        
            // create place to the new child
            int h = n;
            while(h >= i+1)
                C[h+1] = C[h];
            // link the new child to this node
            C[i+1] = z;
        
            // an element of y will move to this node, 
            // find a location to the new element and move all greater element ahead of it
            for (int j = n-1; j >= i; j--)
                elements[j+1] = elements[j];
        
            // copy the middle element of y to this node
            elements[i] = y->elements[t-1];
        
            // increase the counter by one
            n = n + 1;
        }
    
};

Node::Node(int t1, bool leaf1){ // class constructor
            t = t1; // copy t variable from Btree to Node
            leaf = leaf1; // copy leaf variable from Btree to Node
            elements = new int[2*t-1]; // get the max number of elements
            C = new Node *[2*t]; // get the max number of nodes
            n = 0; // set the number of elements to zero
};

class BTree{
    public:
        Node *root; // root node pointer
        int t;  // the degree
        
        BTree(int _t){ // class constructor
            root = NULL;  t = _t; 
        } 
        void traverse(){
            if (root != NULL) root->traverse(); 
        } 
    
        Node* search(int k){
            if(root == NULL) 
                return NULL; 
            else 
                return root->search(k); 
        }

        void insert(int k){ // function to insert values into the tree
            // if tree is empty
            if(root == NULL){
                // allocate new root 
                root = new Node(t, true);
                root->elements[0] = k;  
                root->n = 1;  
            }
            else{ // if the tree is not empty
                // if root is full then tree grows in height
                if (root->n == 2*t-1){
                    height++;
                    // allocate new root
                    Node *s = new Node(t, false);
                    // put the old root as a child
                    s->C[0] = root;       
                    // split the old root and put one element to the new root
                    s->split(0, root);
                    // the root has two children test and see which
                    // of the two children is going to have new element
                    int i = 0;
                    if (s->elements[0] < k)
                        i++;
                    s->C[i]->insertIfNotFull(k);
                    root = s;
                }
                else  // root is not full
                    root->insertIfNotFull(k);
            }
        }

};

int main(){ //main

    int degree, insnum, x,sKey;
    char m;
    reset:cout << "Enter the degree you want: ";
    cin >> degree;
    if(degree <= 1){
        cout << "B-tree can not have a degree of less than 2" << endl;
        goto reset;
    }
    BTree t(degree); // create B-Tree with minimum degree "degree"
    do{
        cout << "\nEnter 'i' to insert, 't' to traverse,'s' to search, 'r' to show stats, 'e' to exit: ";
        cin >> m;
        switch(m)
        {
        case 'i':
        case 'I':
            cout << "How many values you want to insert: ";
            cin >> insnum;
            cout << "Enter the values: ";
            for(int i = 1 ; i <= insnum; i++){
                int nums;
                cin >> nums;
                t.insert(nums);
            }
            break;

        case 't':
        case 'T':
            cout << "B-tree traversing.. " << endl;
            t.traverse();
            break;

        case 's':
        case 'S':
            cout << "Enter the value you want to search: ";
            cin >> sKey;
            if(t.search(sKey) != NULL){
                cout << sKey << " Is Found!" << numOfIt << endl;
            }else{
                cout << sKey << " Is NOT Found!"<< endl;
            }
            break;

        case 'e':
        case 'E':
            cout << "Exiting ...";
            break;
        case 'r':
        case 'R':
            cout << "Degree = " << degree << ", height = "
            << height << ", number of iterations = " << numOfIt << ", cpu time = "
            << (degree*(log(insnum)/log(degree)))
            << ", total keys of searched node = " << numOfKeys;
            numOfIt = 0;
            break;
        default:
            cout << "False entry, try again." <<endl;
            break;
        }

    }while(m != 'e' && m != 'E');
 
    return 0;
}
/*numbers tested on
10
20
30
40
50
60
70
80
90
*/
