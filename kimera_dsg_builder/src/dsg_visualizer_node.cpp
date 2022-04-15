#include <kimera_dsg_visualizer/dynamic_scene_graph_visualizer.h>
#include "kimera_dsg_builder/visualizer_plugins.h"

#include <ros/ros.h>

#include <glog/logging.h>

using kimera::VoxbloxMeshPlugin;
using kimera::PgmoMeshPlugin;

int main(int argc, char** argv) {
  ros::init(argc, argv, "dsg_visualizer_node");

  ros::NodeHandle nh;
  ros::NodeHandle nh_private("~");

  std::string visualizer_ns;
  nh_private.param<std::string>("visualizer_ns", visualizer_ns, "/dsg_visualizer");

  std::string scene_graph_input_path;
  nh_private.param<std::string>("scene_graph_input_path", scene_graph_input_path, "");

  if (scene_graph_input_path.empty()) {
    ROS_FATAL("Empty scene graph input path...");
    ros::shutdown();
    return 1;
  }

  ROS_INFO_STREAM("Loading scene graph from: " << scene_graph_input_path.c_str());
  auto scene_graph = kimera::DynamicSceneGraph::load(scene_graph_input_path);
  ROS_INFO_STREAM("Loaded scene graph: " << scene_graph->numNodes() << " nodes, "
                                         << scene_graph->numEdges() << ", "
                                         << "has mesh? "
                                         << (scene_graph->hasMesh() ? "yes" : "no"));

  ros::NodeHandle visualizer_nh(visualizer_ns);
  kimera::DynamicSceneGraphVisualizer scene_graph_visualizer(
      visualizer_nh, kimera::getDefaultLayerIds());

  bool use_voxblox_mesh_plugin;
  nh_private.param<bool>("use_voxblox_mesh_plugin", use_voxblox_mesh_plugin, false);
  if (use_voxblox_mesh_plugin) {
    // TODO(nathan) voxblox mesh plugin in rviz doesn't handle large graphs well
    scene_graph_visualizer.addPlugin(
        std::make_shared<VoxbloxMeshPlugin>(nh, "dsg_mesh"));
  } else {
    scene_graph_visualizer.addPlugin(std::make_shared<PgmoMeshPlugin>(nh, "dsg_mesh"));
  }

  scene_graph_visualizer.setGraph(scene_graph);

  scene_graph_visualizer.start();
  ROS_DEBUG("Visualizer running");
  ros::spin();

  return 0;
}
