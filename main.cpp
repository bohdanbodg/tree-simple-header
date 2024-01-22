#include <iostream>
#include <string>

#include "tree.hpp"

using namespace std;

template<int COUNT = 2, char FILL = ' '>
string padd(int num) {
    return string(num * COUNT, FILL);
}

template<typename T, typename F1, typename F2, typename F3>
void look(
    Node<T> *node,
    F1 onGeneral,
    F2 onChild,
    F3 onEmpty,
    int step = 0
) {
    (*onGeneral)(node, step);

    auto children = node->children();
    if (children.empty()) {
        (*onEmpty)(node, step);

        return;
    }

    for (auto i = children.begin(); i != children.end(); i++) {
        (*onChild)(node, *i, step);

        look(*i, onGeneral, onChild, onEmpty, step + 1);
    }
}

template<typename T>
void outputTree(Node<T> *node) {
    look<T>(
        node,
        [](Node<T> *node, int) {
            cout << node->value() << ":" << endl;
        },
        [](Node<T> *, Node<T> *, int step) {
            cout << padd(step + 1);
        },
        [](Node<T> *, int step) {
            cout << padd(step + 1) << "<empty>" << endl;
        }
    );
}

int main(int argc, char const *argv[]) {
    auto tree = new Node<string>("0");
    tree->add("1")
        ->add("1.1")
        ->parent()
        ->add("1.2");
    tree->add("2")
        ->add("2.1")
        ->parent()
        ->add("2.2");

    outputTree(tree);

    cout << endl << padd<8, '='>(8) << endl << endl;

    auto node2 = new Node<string>("a");
    node2->add("b")
        ->add("c")
        ->add("d")
        ->add("e")
        ->add("f")
        ->add("g")
        ->add("h")
        ->add("i")
        ->add("j")
        ->add("k")
        ->add("l")
        ->add("m")
        ->add("n")
        ->add("o")
        ->add("p")
        ->add("q")
        ->add("r")
        ->add("s")
        ->add("t")
        ->add("u")
        ->add("v")
        ->add("w")->duplicate()
        ->add("x")
        ->add("y")
        ->add("z")
        ->remove()
        ->remove();

    outputTree(node2);

    cout << endl << padd<8, '='>(8) << endl << endl;

    auto tree2 = new Node<int>(5, {100, 200, 300, 400});
    cout << "v: " << tree2->value() << endl;
    TREE_FOREACH(tree2, node, i, {
        cout << "v[" << i << "]: " << node->value() << endl;
    });

    cout << endl << padd<8, '='>(8) << endl << endl;

    auto t = TREE(string, "Hello")
        ->add(NODE(string, "World"));
    cout << t->parent()->value() << " " << t->value() << "!" << endl;

    return 0;
}
