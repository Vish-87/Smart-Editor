#pragma once
template <typename T>
class Stack{
    private:

        struct Node{
            T data;
            Node *next;
            Node(T d): data(d), next(nullptr){}
        };

        Node *top;


    public:

    Stack(){
        top = nullptr;
    }

    ~Stack(){
        clear();
    }

    void push(T item){
        Node *newNode = new Node(item);
        newNode -> next =top;
        top = newNode;
    }
    T pop(){
        if(isEmpty()){
            return T();
        }
        else{
            T newData = top->data;
            Node *temp = top;
            top = top ->next;
            delete temp;
            return newData;
        }
    }

    T peek(){
        if(isEmpty()){
            return T();
        }
        else{
            return top->data;
        }
    }

    bool isEmpty(){
        return top == nullptr;
        
    }

    void clear(){
        while(!isEmpty()){
            pop();
        }
    }

};