#include "route_planner.h"
#include <algorithm>

using std::sort;

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y,
                           float end_x, float end_y)
    : m_Model(model) {
  // Convert inputs to percentage:
  start_x *= 0.01;
  start_y *= 0.01;
  end_x *= 0.01;
  end_y *= 0.01;

  // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to
  // the starting and ending coordinates. Store the nodes you find in the
  // RoutePlanner's start_node and end_node attributes.

  this->start_node = &(model.FindClosestNode(start_x, start_y));
  this->end_node = &(model.FindClosestNode(end_x, end_y));
}

// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another
// node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  float hVal = 0;
  hVal = node->distance(*(this->end_node));
  return hVal;
}

// TODO 4: Complete the AddNeighbors method to expand the current node by adding
// all unvisited neighbors to the open list. Tips:
// - Use the FindNeighbors() method of the current_node to populate
// current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the
// g_value.
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and
// set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
  // poluate the neighboard for the node provided as current node.
  current_node->FindNeighbors();

  // for all the neighboard, link this as the parent, give one more cost as g +
  // 1, and calculate the h value by passing in the neighbor to the calculation
  // function.

  for (RouteModel::Node *neighbor : current_node->neighbors) {
    if (neighbor->visited) {
      break;
    }
    neighbor->h_value = this->CalculateHValue(neighbor);
    neighbor->g_value =
        current_node->g_value + current_node->distance(*neighbor);
    neighbor->parent = current_node;
    neighbor->visited = true;
    this->open_list.push_back(neighbor);
  }
}

// TODO 5: Complete the NextNode method to sort the open list and return the
// next node. Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

// helper compare funciton
bool Compare(RouteModel::Node *n1, RouteModel::Node *n2) {
  float f1 = n1->g_value + n1->h_value;
  float f2 = n2->g_value + n2->h_value;
  return f1 > f2;
}

RouteModel::Node *RoutePlanner::NextNode() {
  sort(this->open_list.begin(), this->open_list.end(), Compare);
  RouteModel::Node *p = nullptr;
  p = this->open_list.back();
  this->open_list.pop_back();
  return p;
}

// TODO 6: Complete the ConstructFinalPath method to return the final path found
// from your A* search. Tips:
// - This method should take the current (final) node as an argument and
// iteratively follow the
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to
// the distance variable.
// - The returned vector should be in the correct order: the start node should
// be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node>
RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
  // DEBUG
  // std::cout << "test3:\n";

  // Create path_found vector
  distance = 0.0f;
  std::vector<RouteModel::Node> path_found;
  // std::cout << "test4:\n" << distance << "\n";

  // TODO: Implement your solution here.

  while (current_node != nullptr) {

    path_found.push_back(*current_node); // push the current node
    if (current_node->parent != nullptr) {
      distance += current_node->distance(*(current_node->parent));
    } else {
      std::cout << "**current node's parrent is null**\n";
    }
    current_node = current_node->parent; // move to the parrent. If null, this
                                         // assumed to be the end.
  }
  std::reverse(path_found.begin(), path_found.end());

  distance *= m_Model.MetricScale(); // Multiply the distance by the scale of
                                     // the map to get meters.
  return path_found;
}

// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node
// to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method
// to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits.
// This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
  RouteModel::Node *current_node = nullptr;

  // TODO: Implement your solution here.
  current_node = this->start_node;
  current_node->visited = true;
  current_node->h_value = this->CalculateHValue(current_node);
  current_node->g_value = 0;
  this->open_list.push_back(current_node);
  // this->AddNeighbors(current_node);
  int counter = 0;
  while (this->open_list.size() > 0 && counter < 10000) {
    // this->AddNeighbors(current_node);
    current_node = this->NextNode();
    if (current_node->x == this->end_node->x &&
        current_node->y == this->end_node->y) {
      break;
    }
    this->AddNeighbors(current_node);

    counter += 1; // debugging fail safe to be removed
  }
  this->m_Model.path = this->ConstructFinalPath(current_node);
}