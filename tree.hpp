#pragma once

#ifndef __TREE_HPP__
#define __TREE_HPP__

#include <vector>
#include <memory>
#include <utility>
#include <algorithm>
#include <type_traits>

template<typename T>
class Node
{
public:
    using Type = std::conditional_t<std::is_void_v<T>, std::add_pointer_t<T>, T>;
    using Class = Node<Type>;
    using Pointer = std::add_pointer_t<Class>;
    using NodeContainer = std::vector<Pointer>;
    using ValueContainer = std::vector<Type>;

private:
    Pointer _parent;
    NodeContainer _children;
    Type _value;

protected:
    Pointer parent(Pointer node) {
        _parent = node;

        return node;
    }

    Pointer initChildrenFromValues(const ValueContainer &values) {
        _children.clear();

        for (auto i = values.begin(); i != values.end(); i++) {
            this->add(*i);
        }

        return this;
    }

    Pointer initChildrenFromNodes(const NodeContainer &nodes) {
        _children.clear();

        _children = nodes;

        for (auto i = _children.begin(); i != _children.end(); i++) {
            (*i)->parent(this);
        }

        return this;
    }

public:
    Node() : _parent(nullptr), _children({}) {
    }

    Node(const ValueContainer &values) : _parent(nullptr) {
        this->initChildrenFromValues(values);
    }
    Node(const NodeContainer &nodes) : _parent(nullptr) {
        this->initChildrenFromNodes(nodes);
    }

    Node(
        Pointer parent,
        const ValueContainer &values
    ) : _parent(this->parent(parent)) {
        this->initChildrenFromValues(values);
    }
    Node(
        Pointer parent,
        const NodeContainer &nodes
    ) : _parent(this->parent(parent)) {
        this->initChildrenFromNodes(nodes);
    }

    Node(const Type &value) : _parent(nullptr), _children({}), _value(value) {
    }

    Node(
        const Type &value,
        Pointer parent
    ) : _parent(this->parent(parent)),
        _children({}),
        _value(value) {
    }
    Node(
        const Type &value,
        const ValueContainer &values
    ) : _parent(nullptr),
        _value(value) {
        this->initChildrenFromValues(values);
    }
    Node(
        const Type &value,
        const NodeContainer &nodes
    ) : _parent(nullptr),
        _value(value) {
        this->initChildrenFromNodes(nodes);
    }

    Node(
        const Type &value,
        Pointer parent,
        const ValueContainer &values
    ) : _parent(this->parent(parent)),
        _value(value) {
        this->initChildrenFromValues(values);
    }
    Node(
        const Type &value,
        Pointer parent,
        const NodeContainer &nodes
    ) : _parent(this->parent(parent)),
        _value(value) {
        this->initChildrenFromNodes(nodes);
    }

    bool root() const {
        return _parent == nullptr;
    }

    bool leaf() const {
        return _children.empty();
    }

    Pointer parent() const {
        return _parent;
    }

    NodeContainer children() const {
        return _children;
    }

    size_t count() const {
        return _children.size();
    }

    T value() const {
        return _value;
    }

    Pointer value(const Type &value) {
        _value = value;

        return this;
    }

    Pointer add(const Type &value) {
        return this->add(new Class(value, this));
    }

    Pointer add(Pointer node) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->parent() == nullptr) {
            node->parent(this);
        }

        _children.push_back(node);

        return node;
    }

    Pointer remove() {
        if (_parent == nullptr) {
            return nullptr;
        }

       return _parent->remove(this);
    }

    Pointer remove(Pointer node) {
        for (auto i = _children.begin(); i != _children.end(); i++) {
            if (*i == node) {
                _children.erase(i);

                break;
            }
        }

        return this;
    }

    NodeContainer siblings() const {
        if (_parent == nullptr) {
            return {};
        }

        NodeContainer parentChildren = _parent->children(), result;
        std::copy_if(
            parentChildren.begin(),
            parentChildren.end(),
            std::back_inserter(result),
            [this](Pointer node) {
                return node != this;
            }
        );

        return result;
    }

    int level() const {
        int result = 0;

        auto parent = _parent;
        while (parent != nullptr) {
            ++result;
            parent = parent->parent();
        }

        return result;
    }

    Pointer first() const {
        return _children.empty() ? nullptr : *_children.begin();
    }

    Pointer last() const {
        return _children.empty() ? nullptr : *(_children.end() - 1);
    }

    Pointer copy() const {
        auto result = new Class(_value, nullptr);

        for (auto i = _children.begin(); i != _children.end(); i++) {
            result->add((*i)->copy());
        }

        return result;
    }

    Pointer duplicate(bool returnDuplicated = false) {
        if (_parent == nullptr) {
            return this;
        }

        auto result = _parent->add(this->copy());
        if (returnDuplicated) {
            return result;
        }

        return this;
    }

    template<typename F>
    Pointer forEach(F callback) {
        if (_children.empty()) {
            return this;
        }

        for (size_t i = 0; i < _children.size(); i++) {
            (*callback)(_children[i], i);
        }

        return this;
    }
};

#define NODE(type, ...) (new Node<type>(__VA_ARGS__))
#define TREE(type, ...) NODE(type, __VA_ARGS__)

#define NODE_FOREACH(tree, node, index, func) \
    tree->forEach([](decltype(tree) node, size_t index) func)
#define TREE_FOREACH(tree, node, index, func) NODE_FOREACH(tree, node, index, func) 

#endif
