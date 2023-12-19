#include "Tree.h"

#ifdef DEBUG
long long number_of_intersect_node_calls = 0;
#endif
inline int node_id = 0;

TreeNode::TreeNode() :
    min_point{},
    max_point{},
    average_position{},
    children{},
    object{},
    cost_metric{},
    is_trunk{},
    id{++node_id}
{
}

TreeNode::TreeNode(TreeNode && tn)
{
    min_point = tn.min_point;
    max_point = tn.max_point;
    average_position = tn.average_position;
    children = std::move(tn.children);
    if (tn.object.has_value()){
        std::optional<std::unique_ptr<Object>> obj_ptr {std::move(tn.object.value())};
        object = std::move(obj_ptr);
    }
    else {
        object = {};
    }
    cost_metric = false;
    is_trunk = false;
    id = tn.id;
}


void TreeNode::add_node(std::unique_ptr<TreeNode> tn)
{
    vec3f tn_max_point = tn->max_point;
    vec3f tn_min_point = tn->min_point;
    children.emplace_back(std::move(tn)); // This move the funkin vector
    if (children.size() > 1){
        if (tn_max_point.get(0) > max_point.get(0)){
            max_point[0] = tn_max_point.get(0);
        }
        if (tn_max_point.get(1) > max_point.get(1)){
            max_point[1] = tn_max_point.get(1);
        }
        if (tn_max_point.get(2) > max_point.get(2)){
            max_point[2] = tn_max_point.get(2);
        }
        if (tn_min_point.get(0) < min_point.get(0)){
            min_point[0] = tn_min_point.get(0);
        }
        if (tn_min_point.get(1) < min_point.get(1)){
            min_point[1] = tn_min_point.get(1);
        }
        if (tn_min_point.get(2) < min_point.get(2)){
            min_point[2] = tn_min_point.get(2);
        }
    }
    else {
        max_point = tn_max_point;
        min_point = tn_min_point;
        average_position = min_point + 0.5f*max_point;
    }
    cost_metric = cost_function();
}

void TreeNode::set_object(const Object& object_to_be_added)
{
    object = std::make_unique<Object>(object_to_be_added);
    min_point = object_to_be_added.min_point;
    max_point = object_to_be_added.max_point;
    average_position = min_point + 0.5f*(max_point-min_point);
    is_trunk = false;
    cost_metric = cost_function();
}

float TreeNode::cost_function()
{
    float l = std::abs(max_point.get(2) - min_point.get(2));
    float w = std::abs(max_point.get(0) - min_point.get(0));
    float h = std::abs(max_point.get(1) - min_point.get(1));
    return 2*l*w + 2*w*h + 2*l*h;
}

float cost_function(const std::unique_ptr<TreeNode>& tn1, const std::unique_ptr<TreeNode>& tn2)
{
    float l = std::abs(std::max(tn1->max_point.get(2), tn2->max_point.get(2)) - std::min(tn1->min_point.get(2), tn2->min_point.get(2)));
    float w = std::abs(std::max(tn1->max_point.get(0), tn2->max_point.get(0)) - std::min(tn1->min_point.get(0), tn2->min_point.get(0)));
    float h = std::abs(std::max(tn1->max_point.get(1), tn2->max_point.get(1)) - std::min(tn1->min_point.get(1), tn2->min_point.get(1)));
    return 2*l*w + 2*w*h + 2*l*h;
}


static bool tree_intersection_comparison(TreeIntersection p1, TreeIntersection p2){
    return p1.cost_metric < p2.cost_metric ? true : false;
}

static std::vector<std::unique_ptr<TreeNode>> create_tree_leaf_vector(const std::vector<Object>& object_vector){
    std::vector<std::unique_ptr<TreeNode>> leaf_vector {};
    for (Object o : object_vector){
        std::unique_ptr<TreeNode> leaf {new TreeNode};
        leaf->set_object(o);
        leaf_vector.emplace_back(std::move(leaf));
    }

    return leaf_vector;
}

static float calculate_intersection_cost(const std::unique_ptr<TreeNode>& tn1, const std::unique_ptr<TreeNode>& tn2){\
    return cost_function(tn1, tn2);
}

/**
 * @brief Loop until left with one TreeNode in
 * the node_vector std::vector. Calculate the cost of every possible tree intersection 
 * in the node_vector, combine the cost with the indicies into the node_vector and push into 
 * cost_vector. Sort cost vector based on cost, smallest cost at the front (Sorting on a vector
 * may be more expensive than other data structures). In a lot of cases there are costs that
 * are reasonably small. Reasonably is defined by its proximity to the smallest cost 
 * (cost_vector[0].cost_metric). Magic number cost_epsilon defines a limit. This should 
 * probably be calculated based on the original node_vector from leafs. Something with standard 
 * deviation of position and average position. Regardless if the cost is small enough, a new 
 * TreeNode is created with the two indicies and added to the new_node_vector. Several more 
 * new nodes are created if possible. The indicies are kept track of in a unordered_set so that
 * a leaf that has already been used is used in another node, and for not adding leaves that now
 * exist in new_nodes to the new_node_vector. Swap node_vector and new_node_vector. 
 * This took a long time. 
 * 
 * @param node_vector  "Leafs" each containing an object and no children nodes.
 * @return std::unique_ptr<TreeNode> 
 */
static std::unique_ptr<TreeNode> create_tree_node_recurse(std::vector<std::unique_ptr<TreeNode>>  node_vector) {
    float cost_epsilon = 0.1;
    while (node_vector.size() > 1){
        std::vector<TreeIntersection> cost_vector;
        for (unsigned int i = 0; i < node_vector.size(); i++){
            for (unsigned int j = i+1; j < node_vector.size(); j++){
                cost_vector.emplace_back(TreeIntersection {
                i,j,
                calculate_intersection_cost(node_vector[i], node_vector[j])
                });
            }
        }
        std::sort(cost_vector.begin(), cost_vector.end(), tree_intersection_comparison);
        float smallest_cost = cost_vector[0].cost_metric;
        unsigned int i = 0;
        std::vector<std::unique_ptr<TreeNode>> new_node_vector;
        std::unordered_set<int> used_index;
        while (cost_vector[i].cost_metric < smallest_cost + cost_epsilon && i < cost_vector.size()){
            if (used_index.find(cost_vector[i].i) == used_index.end() &&
                used_index.find(cost_vector[i].j) == used_index.end()){
                
                std::unique_ptr<TreeNode> new_node {new TreeNode};
                // std::cout << cost_vector[i].cost_metric << " " << cost_vector[i].i << " " << cost_vector[i].j << std::endl;
                new_node->add_node(std::move(node_vector[cost_vector[i].i]));
                new_node->add_node(std::move(node_vector[cost_vector[i].j]));
                used_index.insert(cost_vector[i].i);
                used_index.insert(cost_vector[i].j);
                new_node_vector.emplace_back(std::move(new_node));
                smallest_cost = cost_vector[i].cost_metric;
            }
            i++;
        }
        // std::cout << i << "\n";
        for (unsigned int i = 0; i < node_vector.size(); i++){
            if (used_index.find(i) == used_index.end()){
                new_node_vector.emplace_back(std::move(node_vector[i]));
            }
        }
        node_vector = std::move(new_node_vector);
    }
    // std::cout << "\n";
    node_vector[0]->is_trunk = true;
    return std::move(node_vector[0]);
}

bool intersect_node(const std::unique_ptr<TreeNode>& tn, const Ray& r)
{
    // This was taken from an article 
    // TODO: refine!
    #ifdef DEBUG
    ++number_of_intersect_node_calls;
    #endif
    // to make sure the denominator is not zero
    // float offset = 0.0001f;
    float tmin = (tn->min_point.get(0) - r.origin.get(0)) / r.direction.get(0); 
    float tmax = (tn->max_point.get(0) - r.origin.get(0)) / r.direction.get(0); 
 
    if (tmin > tmax) std::swap(tmin, tmax); 
 
    float tymin = (tn->min_point.get(1) - r.origin.get(1)) / r.direction.get(1); 
    float tymax = (tn->max_point.get(1) - r.origin.get(1)) / r.direction.get(1); 
 
    if (tymin > tymax) std::swap(tymin, tymax); 
 
    if ((tmin > tymax) || (tymin > tmax)) {
        return false; 
    }
    if (tymin > tmin) 
        tmin = tymin; 
 
    if (tymax < tmax) 
        tmax = tymax; 
 
    float tzmin = (tn->min_point.get(2) - r.origin.get(2)) / r.direction.get(2); 
    float tzmax = (tn->max_point.get(2) - r.origin.get(2)) / r.direction.get(2); 
    if (tzmin > tzmax) std::swap(tzmin, tzmax); 
 
    if ((tmin > tzmax) || (tzmin > tmax)) 
        return false; 
 
    if (tzmin > tmin) 
        tmin = tzmin; 
 
    if (tzmax < tmax) 
        tmax = tzmax; 
    return true;
}

std::ostream &operator<<(std::ostream &os, const std::unique_ptr<TreeNode> &tn)
{
    if (tn->object.has_value() && tn->children.size() > 0) std::cout << "SOMETHING IS VERY WRONG\n";
    os << "node(" << tn->id << ") {";
    if (tn->object.has_value()){
        os << "id:" << tn->object.value()->id << "";
    }
    if (tn->children.size() > 0){
        for (unsigned int i = 0; i < tn->children.size() - 1; i++){
            os << tn->children[i] << ", ";
        }
        if (tn->children.size() > 0){
            os << tn->children[tn->children.size() - 1];
        }
    }
    os << "}";
    return os;
}

std::unique_ptr<TreeNode> get_trunk_node(std::vector<Object> object_vector) {
    return create_tree_node_recurse(create_tree_leaf_vector(object_vector));
}
