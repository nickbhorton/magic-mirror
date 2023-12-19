#ifndef TREE_H
#define TREE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>

#include "matrix.hpp"
#include "Object.hpp"
#include "Ray.hpp"

#define DEBUG

struct TreeIntersection {
    unsigned int i;
    unsigned int j;
    float cost_metric;
};

class TreeNode {
    public:
        vec3f min_point;
        vec3f max_point;
        vec3f average_position;
        std::vector<std::unique_ptr<TreeNode>> children; // make pointers
        std::optional<std::unique_ptr<Object>> object; // make pointers
        float cost_metric;
        bool is_trunk;
        int id;

        void add_node(std::unique_ptr<TreeNode> tn);
        void set_object(const Object& object_to_be_added);
        float cost_function();

        TreeNode(); // empty constructor
        TreeNode(const TreeNode&) = delete; // copy constructor
        TreeNode(TreeNode&&); // move constructor
        TreeNode& operator=(const TreeNode& other) = delete; // copy assignment
        TreeNode& operator=(TreeNode&& other) = delete; // move assignment        
};


std::ostream &operator<<(std::ostream &os, const std::unique_ptr<TreeNode> &tn);

std::unique_ptr<TreeNode> get_trunk_node(std::vector<Object> object_vector);
bool intersect_node(const std::unique_ptr<TreeNode>& tn, const Ray& r);

#endif
