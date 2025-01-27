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
            command_ = joint_.getPosition();

            if(!n.getParam("gain", gain_))
            {
                ROS_ERROR("Could not find the gain parameter value");
                return false;
            }

            sub_command_ = n.subscribe<std_msgs::Float64>("command", 1, &MyPositionController::setCommandCB, this);

            return true;
        }

        void update(const ros::Time& time, const ros::Duration& period)
        {
            double error = command_ -joint_.getPosition();
            double commanded_effort = error*gain_;
            joint_.setCommand(commanded_effort);
        }

        void setCommandCB(const std_msgs::Float64ConstPtr& msg)
        {
            command_ = msg->data;
        }

        void starting(const ros::Time& time){}
        void stopping(const ros::Time& time){}

        private:
            hardware_interface::JointHandle joint_;
            double gain_;
            double command_;
            ros::Subscriber sub_command_;

    };

    PLUGINLIB_EXPORT_CLASS(my_controller_ns::MyPositionController, controller_interface::ControllerBase);

}

