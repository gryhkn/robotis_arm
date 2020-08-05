#include <controller_interface/controller.h>
#include <hardware_interface/joint_command_interface.h>
#include <pluginlib/class_list_macros.h>
#include <std_msgs/Float64.h>

namespace my_controller_ns
{

    class MyPositionController : public controller_interface::Controller<hardware_interface::EffortJointInterface>
    {
        bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle &n)
        {
            std::string my_joint;
            if(!n.getParam("joint", my_joint))
            {
                ROS_ERROR("Could not find joint name");
                return false;
            }

            joint_ = hw->getHandle(my_joint);
            command_ = joint.getPosition();

            if(!n.getParam("gain", gain))
            {
                ROS_ERROR("Could not find the gain parameter value");
                return false;
            }

            sub_command_ = n.subscribe<std_msgs::Float64>("command", 1, &MyPositionController::setCommandCB, this);

            return true;
        }

    };

}

