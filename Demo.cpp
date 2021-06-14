
#include <iostream>
#include <stdexcept>
#include <vector>
#include "BinaryTree.hpp"

using namespace std;
using namespace ariel;

enum class _order {
    inorder,
    postorder,
    preorder,
};

template <typename T>
bool isEqual(BinaryTree<T> &tree, vector<T> const &v2, _order order) {
    // This function checks if a given tree order is equal to some vector.
    if (order == _order::inorder) {
        auto end = tree.end_inorder();
        auto pair = std::mismatch(tree.begin_inorder(), tree.end_inorder(), v2.begin());
        return (pair.first == end && pair.second == v2.end());
    } else if (order == _order::postorder) {
        auto end = tree.end_postorder();
        auto pair = std::mismatch(tree.begin_postorder(), tree.end_postorder(), v2.begin());
        return (pair.first == end && pair.second == v2.end());
    }
    auto end = tree.end_preorder();
    auto pair = std::mismatch(tree.begin_preorder(), tree.end_preorder(), v2.begin());
    return (pair.first == end && pair.second == v2.end());
}


template <typename T>
BinaryTree<T> create_dummy(int adder = 0) {
    BinaryTree<T> tree;

    // create the following tree
    //                    1
    //              2          3
    //         4        5
    //               10   11
    //             20       23
    tree.add_root(1 + adder);
    tree.add_left(1 + adder, 2 + adder);
    tree.add_right(1 + adder, 3 + adder);
    tree.add_left(2 + adder, 4 + adder);
    tree.add_right(2 + adder, 5 + adder);
    tree.add_left(5 + adder, 10 + adder);
    tree.add_right(5 + adder, 11 + adder);
    tree.add_left(10 + adder, 20 + adder);
    tree.add_right(11 + adder, 23 + adder);

    return tree;
}


int main(){
    cout << "TESTS !!!!" << endl;
    BinaryTree<int> tree = create_dummy<int>();
     // inorder : 4, 10, 5, 11, 2, 1, 3
    vector<int> inorder = {4, 2, 20, 10, 5, 11, 23, 1, 3};
    //postorder : 4, 20, 10, 23, 11, 5, 2, 3, 1
    vector<int> postorder = {4, 20, 10, 23, 11, 5, 2, 3, 1};
    //preorder : 1, 2, 4, 5, 10, 20, 11, 23, 3
    vector<int> preorder = {1, 2, 4, 5, 10, 20, 11, 23, 3};
    // transform(preorder.begin(), preorder.end(), preorder.begin(), [](int val) { return val + 1; });

    cout << tree << endl;
    // for(auto it : tree){
    //     cout << it <<  " , ";
    // }

   
    return 0; 
}


