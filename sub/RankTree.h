#ifndef _RANK_TREE_H_
#define _RANK_TREE_H_

#include "stack.h"
#include "ourUtilityFunctions.h"
#include <iostream>

#include <cassert>

template <typename T>
inline Comparison RankTree_CompareUsingOperators(const T &left, const T &right)
{
    if (left < right)
        return Comparison::less;
    else if (left > right)
        return Comparison::greater;
    else
        return Comparison::equal;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &) = RankTree_CompareUsingOperators<DATA_t>>
class RankTree
{
public:
    struct Node
    {
        DATA_t __data;
        double __amount;
        Node *__left, *__right;
        int __height;

        Node(DATA_t data) : __data(data),
                            __amount(0),
                            __left(nullptr),
                            __right(nullptr),
                            __height(0)
        {
        }

        void updateValues()
        {
            __height = 1 + our::max(__left != nullptr ? __left->__height : -1,
                                    __right != nullptr ? __right->__height : -1);
        }

        int balanceFactor()
        {
            return (__left != nullptr ? __left->__height : -1) - (__right != nullptr ? __right->__height : -1);
        }

        bool isLeaf() const
        {
            return ((__left == nullptr) && (__right == nullptr));
        }

        bool hasLeft() const
        {
            return __left != nullptr;
        }

        bool hasRight() const
        {
            return __right != nullptr;
        }

        // returns a pointer to the node that "replaced" the previous after rotation
        Node *left_rotate()
        {
            Node *R = __right;
            __right = __right->__left;
            R->__left = this;

            int temp = R->__amount;
            R->__amount += __amount;
            __amount = -temp;
            if(hasRight())
            {
                __right->__amount += temp;
            }

            this->updateValues(); // the order is important
            R->updateValues();

            return R;
        }

        // returns a pointer to the node that "replaced" the previous after rotation
        Node *right_rotate()
        {
            Node *L = __left;
            __left = __left->__right;
            L->__right = this;

            int temp = L->__amount;
            L->__amount += __amount;
            __amount = -temp;
            if(hasLeft())
            {
                __left->__amount += temp;
            }

            this->updateValues(); // the order is important
            L->updateValues();

            return L;
        }

        friend void swapNodes(Node *&first, Node *&second)
        {
            if ((first == nullptr) || (second == nullptr) || (/*self swap bug (?)*/ first == second))
            {
                return;
            }
            our::swap(first->__left, second->__left);     // switch lefts
            our::swap(first->__right, second->__right);   // switch rights
            our::swap(first->__height, second->__height); // switch heights
            our::swap(first, second);                     // switch places (parents in tree)
        }

        static void restartAmount(Node *node)
        {
            node->__amount = 0;
            node->__data->resetData();
        }
    };

    RankTree();
    ~RankTree();

    void insert(DATA_t data);
    void remove(DATA_t data);

    void clear();
    bool isEmpty() const;
    int getSize() const;

    DATA_t &find(const DATA_t &data);
    const DATA_t &find(const DATA_t &data) const;

    double getPrizesAmount(DATA_t data)
    {
        Stack<Node *&> path;
        find_path(data, path);
        double amount = 0;
        while (!path.isEmpty())
        {
            amount += path.back()->__amount;
            path.pop_back();
        }
        return amount;
    }

    void restartPrizesAmounts()
    {
        restartPrizesAmounts_aux(__root);
    }

    void display();
    ///////////////////////////////////////////////////////////////////////////////////
    void addPrize(DATA_t data1, DATA_t data2, double prize)
    {
        addPrize_aux(data2, prize);
        addPrize_aux(data1, -prize);
    }

    // error classes
    class NoSuchElementException : public std::exception
    {
    public:
        const char *what() const noexcept override { return "There is no such element"; }
    };
    class ElementAlreadyExistsException : public std::exception
    {
    public:
        const char *what() const noexcept override { return "Element already exists"; }
    };

private:
    Node *__root;
    int __size;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       +---------------------------------------------------------------+
       |                                                               |
       |                        AVLTree Helper                         |
       |                                                               |
       +---------------------------------------------------------------+
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    // deletes the tree with a pointer to the root, return teh size of tree it deleted
    int clear_aux(Node *root)
    {
        if (root == nullptr)
        {
            return 0;
        }
        int size = 1 + clear_aux(root->__left) + clear_aux(root->__right);
        delete root;
        return size;
    }

    void addPrize_aux(DATA_t data, double prize)
    {
        Node *temp = __root;
        int right_turns = 0, left_turns = 0;
        while (temp != nullptr && compFunction(temp->__data, data) != Comparison::equal)
        {
            while (temp != nullptr && compFunction(temp->__data, data) == Comparison::less)
            {
                if (right_turns == 0)
                {
                    temp->__amount += prize;
                }
                right_turns++;
                left_turns = 0;
                temp = temp->__right;
            }

            while (temp != nullptr && compFunction(temp->__data, data) == Comparison::greater)
            {
                if (right_turns != 0)
                {
                    temp->__amount -= prize;
                }
                left_turns++;
                right_turns = 0;
                temp = temp->__left;
            }
        }
        if(temp != nullptr && right_turns == 0)
            temp->__amount += prize;
        if(temp != nullptr && temp->hasRight())
            temp->__right->__amount -= prize;
    }

    Node **find_node(DATA_t data)
    {
        Node **temp = &__root;
        while (*temp != nullptr)
        {
            Comparison result = compFunction(data, (*temp)->__data);
            if (result == Comparison::less)
            {
                temp = &((*temp)->__left);
            }
            else if (result == Comparison::greater)
            {
                temp = &((*temp)->__right);
            }
            else
            {
                return temp;
            }
        }
        throw NoSuchElementException();
    }

    Stack<Node *&> &find_path(DATA_t data, Stack<Node *&> &empty_path)
    {
        Node **temp = &__root;
        while ((*temp) != nullptr)
        {
            empty_path.push_back(*temp);
            Comparison result = compFunction(data, (*temp)->__data);
            if (result == Comparison::less)
            {
                temp = &((*temp)->__left);
            }
            else if (result == Comparison::greater)
            {
                temp = &((*temp)->__right);
            }
            else
            {
                break;
            }
        }
        if ((*temp) == nullptr)
        {
            empty_path.push_back(*temp);
        }

        return empty_path;
    }

    void balance(Stack<Node *&> &path)
    {
        Node *curr = nullptr;
        while (!path.isEmpty())
        {
            curr = path.back();
            if (curr == nullptr)
            {

                path.pop_back();
                continue;
            }

            Node *&curr_reference = path.back();
            path.pop_back();
            // update height
            curr->updateValues();

            if (curr->balanceFactor() >= 2 && curr->__left->balanceFactor() >= 0)
            { // left - left
                curr_reference = curr->right_rotate();
            }
            else if (curr->balanceFactor() >= 2)
            { // left - right
                curr_reference->__left = curr->__left->left_rotate();
                curr_reference = curr->right_rotate();
            }

            else if (curr->balanceFactor() <= -2 and curr->__right->balanceFactor() <= 0)
            { // right - right
                curr_reference = curr->left_rotate();
            }
            else if (curr->balanceFactor() <= -2)
            { // right - left
                curr_reference->__right = curr->__right->right_rotate();
                curr_reference = curr->left_rotate();
            }
        }
    }
    /**
     * @return -    true if insertion is successful
     *         -    false otherwise (duplicates)
     */
    bool insert_aux(DATA_t data)
    {
        Stack<Node *&> path;
        find_path(data, path);
        if (path.back() != nullptr)
        { // duplicate
            return false;
        }
        path.back() = new Node(data); // beware of bad_alloc

        Stack<Node *&> tempPath;
        find_path(data, tempPath);
        Node *node = tempPath.back();
        while (!tempPath.isEmpty())
        {
            node->__amount -= tempPath.back()->__amount;
            tempPath.pop_back();
        }

        path.pop_back(); // new inserted node dont need balancing
        // balance path
        balance(path /*, true*/);
        return true;
    }

    bool remove_aux(DATA_t data)
    {
        Stack<Node *&> path;
        find_path(data, path);
        if (path.isEmpty() || path.back() == nullptr)
        {
            return false;
        }
        // path NOT empty and back is VALID
        Node *curr = path.back();
        assert(curr != nullptr);
        if (curr->isLeaf())
        {
            delete curr;
            path.back() = nullptr;
            path.pop_back();
        }
        else if (curr->hasLeft() && !curr->hasRight())
        {
            path.back() = curr->__left;
            delete curr;
            path.pop_back();
        }
        else if (!curr->hasLeft() && curr->hasRight())
        {
            path.back() = curr->__right;
            delete curr;
            path.pop_back();
        }
        else
        { // (curr->hasLeft() && curr->hasRight())

            Stack<Node *&> path_to_successor;
            // find successor
            Node **successor = &curr->__right; // go right once
            while ((*successor) != nullptr)    // go all the way to the left
            {
                path_to_successor.push_back(*successor);
                successor = &((*successor)->__left);
            }

            if (path_to_successor.isEmpty())
            {
                Node *toDelete = path.back();
                path.back() = (*successor);
                delete toDelete;
            }

            else
            {
                our::swap(path.back()->__data, path_to_successor.back()->__data);
                if (path_to_successor.back()->isLeaf())
                {
                    delete path_to_successor.back();
                    path_to_successor.back() = nullptr;
                    path_to_successor.pop_back();
                }
                else
                {
                    Node *toDelete = path_to_successor.back();
                    path_to_successor.back() = path_to_successor.back()->__right;
                    delete toDelete;
                    path_to_successor.pop_back();
                }
            }
            balance(path_to_successor /*, false */);
        }
        // balance path
        balance(path /*, false */);

        return true;
    }

    void restartPrizesAmounts_aux(Node *const root) const
    {
        if (root == nullptr)
        {
            return;
        }
        restartPrizesAmounts_aux(root->__left);
        Node::restartAmount(root);
        restartPrizesAmounts_aux(root->__right);
    }

    void display(Node *cur, int depth = 0, int state = 0)
    { // state: 1 -> left, 2 -> right , 0 -> root
        if (cur->__left)
            display(cur->__left, depth + 1, 1);

        for (int i = 0; i < depth; i++)
            printf("     ");

        if (state == 1) // left
            printf("┌───");
        else if (state == 2) // right
            printf("└───");

        std::cout << "[" << cur->__data->getID() << ",  " << cur->__amount << "]" << std::endl;

        if (cur->__right)
            display(cur->__right, depth + 1, 2);
    }

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       +---------------------------------------------------------------+
       |                                                               |
       |                    Node Implementations                       |
       |                                                               |
       +---------------------------------------------------------------+
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    Node *find_common_father(const DATA_t &data1, const DATA_t &data2)
    {
        Comparison result = compFunction(data1, data2);
        if (result == Comparison::greater)
        {
            return nullptr;
        }
        Node *temp = __root;
        while (temp != nullptr)
        {
            Comparison result1 = compFunction(temp->__data, data1);
            Comparison result2 = compFunction(data2, temp->__data);
            if (result1 == Comparison::less) // else temp >= data1
            {
                temp = temp->__right;
            }
            else if (result2 == Comparison::less) // else data2 >= temp
            {
                temp = temp->__left;
            }
            else // data1 <= temp <= data2
            {
                break;
            }
        }
        return temp;
    }
};

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
RankTree<DATA_t, compFunction>::RankTree() : __root(nullptr), __size(0)
{
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
RankTree<DATA_t, compFunction>::~RankTree()
{
    clear();
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void RankTree<DATA_t, compFunction>::insert(DATA_t data)
{
    if (insert_aux(data)) // insert successful
    {
        __size++;
    }
    else
    {
        throw ElementAlreadyExistsException();
    }
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void RankTree<DATA_t, compFunction>::remove(DATA_t data)
{
    if (remove_aux(data)) // deletion successful
    {
        __size--;
    }
    else
    {
        throw NoSuchElementException();
    }
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void RankTree<DATA_t, compFunction>::clear()
{
    __size -= clear_aux(__root);
    __root = nullptr;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
bool RankTree<DATA_t, compFunction>::isEmpty() const
{
    return (__root == nullptr);
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
int RankTree<DATA_t, compFunction>::getSize() const
{
    return __size;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
DATA_t &RankTree<DATA_t, compFunction>::find(const DATA_t &data)
{
    Node *temp = *find_node(data);
    if (temp == nullptr)
    {
        throw NoSuchElementException();
    }
    return temp->__data;
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
const DATA_t &RankTree<DATA_t, compFunction>::find(const DATA_t &data) const
{
    return find(data);
}

template <typename DATA_t, Comparison (*compFunction)(const DATA_t &, const DATA_t &)>
void RankTree<DATA_t, compFunction>::display()
{
    std::cout << "\n";
    if (!isEmpty())
        display(__root);
    else
        std::cout << "Empty";
    std::cout << "\n";
}

#endif // _AVL_TREE_H_
