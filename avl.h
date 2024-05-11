#include <iostream>
using namespace std;

class Node
{
public:
    int data;
    Node* left;
    Node* right;
    int height;
    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree
{
private:
    Node* root;
    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    int getHeight(Node* node)
    {
        return (node == nullptr) ? 0 : node->height;
    }

    int getBalance(Node* node)
    {
        return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    Node* rightRotate(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    Node* insertNode(Node* node, int val)
    {
        if (node == nullptr)
            return new Node(val);

        if (val <= node->data)
            node->left = insertNode(node->left, val);
        else
            node->right = insertNode(node->right, val);

        return updateHeight(node);
    }

    Node* minValueNode(Node* node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* deleteNode(Node* node, int key)
    {
        if (node == nullptr)
            return node;

        if (key < node->data)
            node->left = deleteNode(node->left, key);
        else if (key > node->data)
            node->right = deleteNode(node->right, key);
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                delete temp;
            }
            else
            {
                Node* temp = minValueNode(node->right);
                node->data = temp->data;
                node->right = deleteNode(node->right, temp->data);
            }
        }

        if (node == nullptr)
            return node;

        return updateHeight(node);
    }

    Node* updateHeight(Node* node)
    {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && getBalance(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && getBalance(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    void inOrderTraversal(Node* node)
    {
        if (node == nullptr)
            return;
        inOrderTraversal(node->left);
        cout << node->data << " ";
        inOrderTraversal(node->right);
    }

    bool searchNode(int val, Node* node)
    {
        if (node == nullptr)
            return false;

        if (val == node->data)
            return true;
        else if (val < node->data)
            return searchNode(val, node->left);
        else
            return searchNode(val, node->right);
    }

    int totalNode(int val, Node* node)
    {
        if (node == nullptr)
            return 0;

        int total = 0;
        if (val == node->data)
            total++;

        total += totalNode(val, node->left);
        total += totalNode(val, node->right);

        return total;
    }

    void clearNodes(Node* node)
    {
        if (node == nullptr)
            return;

        clearNodes(node->left);
        clearNodes(node->right);

        delete node;
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree()
    {
        clear();
    }

    void insert(int val)
    {
        root = insertNode(root, val);
    }

    void remove(int val)
    {
        root = deleteNode(root, val);
    }

    void display()
    {
        cout << "In-order traversal: ";
        inOrderTraversal(root);
        cout << "\n";
    }

    bool search(int val)
    {
        return searchNode(val, root);
    }

    int getTotal(int val)
    {
        return totalNode(val, root);
    }

    void clear()
    {
        clearNodes(root);
        root = nullptr;
    }
};
